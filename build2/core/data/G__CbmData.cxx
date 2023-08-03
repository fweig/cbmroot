// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__CbmData
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

#include "TBuffer.h"
#include "TVirtualObject.h"
#include <vector>
#include "TSchemaHelper.h"

#include <Logger.h>

// Header files passed as explicit arguments
#include "CbmDigiData.h"
#include "CbmDigiEvent.h"
#include "CbmDigiTimeslice.h"
#include "CbmBmonDigiData.h"
#include "CbmStsDigiData.h"
#include "CbmMuchDigiData.h"
#include "CbmRichDigiData.h"
#include "CbmTrdDigiData.h"
#include "CbmTofDigiData.h"
#include "CbmPsdDigiData.h"
#include "CbmAddress.h"
#include "CbmTimeSlice.h"
#include "CbmEvent.h"
#include "CbmEventStore.h"
#include "CbmHit.h"
#include "CbmPixelHit.h"
#include "CbmStripHit.h"
#include "CbmTrack.h"
#include "CbmTrackMatch.h"
#include "CbmCluster.h"
#include "CbmModuleList.h"
#include "CbmErrorMessage.h"
#include "CbmMCTrack.h"
#include "CbmMCEventInfo.h"
#include "CbmMCEventList.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmTrackMatchNew.h"
#include "CbmTsEventHeader.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiContainer.h"
#include "CbmMvdCluster.h"
#include "CbmMvdDetectorId.h"
#include "CbmMvdDigi.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmMvdHitMatch.h"
#include "CbmBmonDigi.h"
#include "CbmStsAddress.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsTrack.h"
#include "CbmRichRing.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichTrbDigi.h"
#include "CbmRichDigi.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigi.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchTrack.h"
#include "CbmMuchAddress.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"
#include "CbmTrdPoint.h"
#include "CbmTrdDigi.h"
#include "CbmTrdAddress.h"
#include "CbmTrdCluster.h"
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTofDetectorId.h"
#include "CbmTofDetectorId_v07a.h"
#include "CbmTofDetectorId_v12b.h"
#include "CbmTofDetectorId_v14a.h"
#include "CbmTofDetectorId_v21a.h"
#include "CbmTofAddress.h"
#include "CbmTofDigi.h"
#include "CbmTofTrack.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletParam.h"
#include "CbmPsdDigi.h"
#include "CbmPsdDsp.h"
#include "CbmPsdHit.h"
#include "CbmPsdPoint.h"
#include "CbmPsdAddress.h"
#include "CbmPsdMCbmHit.h"
#include "CbmGlobalTrack.h"
#include "CbmVertex.h"
#include "CbmTrackParam.h"
#include "AccDataSis18.h"
#include "StsXyterMessage.h"
#include "gDpbMessv100.h"
#include "CriGet4Mess001.h"
#include "TimesliceMetaData.h"
#include "PsdGbtReader-v0.00.h"
#include "PsdGbtReader-v1.00.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace CbmStsAddress {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *CbmStsAddress_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("CbmStsAddress", 0 /*version*/, "CbmStsAddress.h", 52,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &CbmStsAddress_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *CbmStsAddress_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_CbmBmonDigi(void *p = nullptr);
   static void *newArray_CbmBmonDigi(Long_t size, void *p);
   static void delete_CbmBmonDigi(void *p);
   static void deleteArray_CbmBmonDigi(void *p);
   static void destruct_CbmBmonDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBmonDigi*)
   {
      ::CbmBmonDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBmonDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBmonDigi", ::CbmBmonDigi::Class_Version(), "CbmBmonDigi.h", 30,
                  typeid(::CbmBmonDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBmonDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBmonDigi) );
      instance.SetNew(&new_CbmBmonDigi);
      instance.SetNewArray(&newArray_CbmBmonDigi);
      instance.SetDelete(&delete_CbmBmonDigi);
      instance.SetDeleteArray(&deleteArray_CbmBmonDigi);
      instance.SetDestructor(&destruct_CbmBmonDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBmonDigi*)
   {
      return GenerateInitInstanceLocal((::CbmBmonDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBmonDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmBmonDigiData(void *p = nullptr);
   static void *newArray_CbmBmonDigiData(Long_t size, void *p);
   static void delete_CbmBmonDigiData(void *p);
   static void deleteArray_CbmBmonDigiData(void *p);
   static void destruct_CbmBmonDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmBmonDigiData*)
   {
      ::CbmBmonDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmBmonDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmBmonDigiData", ::CbmBmonDigiData::Class_Version(), "CbmBmonDigiData.h", 30,
                  typeid(::CbmBmonDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmBmonDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmBmonDigiData) );
      instance.SetNew(&new_CbmBmonDigiData);
      instance.SetNewArray(&newArray_CbmBmonDigiData);
      instance.SetDelete(&delete_CbmBmonDigiData);
      instance.SetDeleteArray(&deleteArray_CbmBmonDigiData);
      instance.SetDestructor(&destruct_CbmBmonDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmBmonDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmBmonDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmBmonDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmAddress_Dictionary();
   static void CbmAddress_TClassManip(TClass*);
   static void *new_CbmAddress(void *p = nullptr);
   static void *newArray_CbmAddress(Long_t size, void *p);
   static void delete_CbmAddress(void *p);
   static void deleteArray_CbmAddress(void *p);
   static void destruct_CbmAddress(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmAddress*)
   {
      ::CbmAddress *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmAddress));
      static ::ROOT::TGenericClassInfo 
         instance("CbmAddress", "CbmAddress.h", 27,
                  typeid(::CbmAddress), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmAddress_Dictionary, isa_proxy, 0,
                  sizeof(::CbmAddress) );
      instance.SetNew(&new_CbmAddress);
      instance.SetNewArray(&newArray_CbmAddress);
      instance.SetDelete(&delete_CbmAddress);
      instance.SetDeleteArray(&deleteArray_CbmAddress);
      instance.SetDestructor(&destruct_CbmAddress);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmAddress*)
   {
      return GenerateInitInstanceLocal((::CbmAddress*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmAddress*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmAddress_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmAddress*)nullptr)->GetClass();
      CbmAddress_TClassManip(theClass);
   return theClass;
   }

   static void CbmAddress_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchAddress(void *p = nullptr);
   static void *newArray_CbmMuchAddress(Long_t size, void *p);
   static void delete_CbmMuchAddress(void *p);
   static void deleteArray_CbmMuchAddress(void *p);
   static void destruct_CbmMuchAddress(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchAddress*)
   {
      ::CbmMuchAddress *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchAddress >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchAddress", ::CbmMuchAddress::Class_Version(), "CbmMuchAddress.h", 55,
                  typeid(::CbmMuchAddress), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchAddress::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchAddress) );
      instance.SetNew(&new_CbmMuchAddress);
      instance.SetNewArray(&newArray_CbmMuchAddress);
      instance.SetDelete(&delete_CbmMuchAddress);
      instance.SetDeleteArray(&deleteArray_CbmMuchAddress);
      instance.SetDestructor(&destruct_CbmMuchAddress);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchAddress*)
   {
      return GenerateInitInstanceLocal((::CbmMuchAddress*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchAddress*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchDigi(void *p = nullptr);
   static void *newArray_CbmMuchDigi(Long_t size, void *p);
   static void delete_CbmMuchDigi(void *p);
   static void deleteArray_CbmMuchDigi(void *p);
   static void destruct_CbmMuchDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchDigi*)
   {
      ::CbmMuchDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchDigi", ::CbmMuchDigi::Class_Version(), "CbmMuchDigi.h", 37,
                  typeid(::CbmMuchDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchDigi) );
      instance.SetNew(&new_CbmMuchDigi);
      instance.SetNewArray(&newArray_CbmMuchDigi);
      instance.SetDelete(&delete_CbmMuchDigi);
      instance.SetDeleteArray(&deleteArray_CbmMuchDigi);
      instance.SetDestructor(&destruct_CbmMuchDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchDigi*)
   {
      return GenerateInitInstanceLocal((::CbmMuchDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchDigiData(void *p = nullptr);
   static void *newArray_CbmMuchDigiData(Long_t size, void *p);
   static void delete_CbmMuchDigiData(void *p);
   static void deleteArray_CbmMuchDigiData(void *p);
   static void destruct_CbmMuchDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchDigiData*)
   {
      ::CbmMuchDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchDigiData", ::CbmMuchDigiData::Class_Version(), "CbmMuchDigiData.h", 29,
                  typeid(::CbmMuchDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchDigiData) );
      instance.SetNew(&new_CbmMuchDigiData);
      instance.SetNewArray(&newArray_CbmMuchDigiData);
      instance.SetDelete(&delete_CbmMuchDigiData);
      instance.SetDeleteArray(&deleteArray_CbmMuchDigiData);
      instance.SetDestructor(&destruct_CbmMuchDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmMuchDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmPsdAddress_Dictionary();
   static void CbmPsdAddress_TClassManip(TClass*);
   static void *new_CbmPsdAddress(void *p = nullptr);
   static void *newArray_CbmPsdAddress(Long_t size, void *p);
   static void delete_CbmPsdAddress(void *p);
   static void deleteArray_CbmPsdAddress(void *p);
   static void destruct_CbmPsdAddress(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdAddress*)
   {
      ::CbmPsdAddress *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmPsdAddress));
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdAddress", "CbmPsdAddress.h", 35,
                  typeid(::CbmPsdAddress), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmPsdAddress_Dictionary, isa_proxy, 0,
                  sizeof(::CbmPsdAddress) );
      instance.SetNew(&new_CbmPsdAddress);
      instance.SetNewArray(&newArray_CbmPsdAddress);
      instance.SetDelete(&delete_CbmPsdAddress);
      instance.SetDeleteArray(&deleteArray_CbmPsdAddress);
      instance.SetDestructor(&destruct_CbmPsdAddress);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdAddress*)
   {
      return GenerateInitInstanceLocal((::CbmPsdAddress*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdAddress*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmPsdAddress_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmPsdAddress*)nullptr)->GetClass();
      CbmPsdAddress_TClassManip(theClass);
   return theClass;
   }

   static void CbmPsdAddress_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdDigi(void *p = nullptr);
   static void *newArray_CbmPsdDigi(Long_t size, void *p);
   static void delete_CbmPsdDigi(void *p);
   static void deleteArray_CbmPsdDigi(void *p);
   static void destruct_CbmPsdDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdDigi*)
   {
      ::CbmPsdDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdDigi", ::CbmPsdDigi::Class_Version(), "CbmPsdDigi.h", 36,
                  typeid(::CbmPsdDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdDigi) );
      instance.SetNew(&new_CbmPsdDigi);
      instance.SetNewArray(&newArray_CbmPsdDigi);
      instance.SetDelete(&delete_CbmPsdDigi);
      instance.SetDeleteArray(&deleteArray_CbmPsdDigi);
      instance.SetDestructor(&destruct_CbmPsdDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdDigi*)
   {
      return GenerateInitInstanceLocal((::CbmPsdDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdDigiData(void *p = nullptr);
   static void *newArray_CbmPsdDigiData(Long_t size, void *p);
   static void delete_CbmPsdDigiData(void *p);
   static void deleteArray_CbmPsdDigiData(void *p);
   static void destruct_CbmPsdDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdDigiData*)
   {
      ::CbmPsdDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdDigiData", ::CbmPsdDigiData::Class_Version(), "CbmPsdDigiData.h", 29,
                  typeid(::CbmPsdDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdDigiData) );
      instance.SetNew(&new_CbmPsdDigiData);
      instance.SetNewArray(&newArray_CbmPsdDigiData);
      instance.SetDelete(&delete_CbmPsdDigiData);
      instance.SetDeleteArray(&deleteArray_CbmPsdDigiData);
      instance.SetDestructor(&destruct_CbmPsdDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmPsdDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichDigi(void *p = nullptr);
   static void *newArray_CbmRichDigi(Long_t size, void *p);
   static void delete_CbmRichDigi(void *p);
   static void deleteArray_CbmRichDigi(void *p);
   static void destruct_CbmRichDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichDigi*)
   {
      ::CbmRichDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichDigi", ::CbmRichDigi::Class_Version(), "CbmRichDigi.h", 31,
                  typeid(::CbmRichDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichDigi) );
      instance.SetNew(&new_CbmRichDigi);
      instance.SetNewArray(&newArray_CbmRichDigi);
      instance.SetDelete(&delete_CbmRichDigi);
      instance.SetDeleteArray(&deleteArray_CbmRichDigi);
      instance.SetDestructor(&destruct_CbmRichDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichDigi*)
   {
      return GenerateInitInstanceLocal((::CbmRichDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichDigiData(void *p = nullptr);
   static void *newArray_CbmRichDigiData(Long_t size, void *p);
   static void delete_CbmRichDigiData(void *p);
   static void deleteArray_CbmRichDigiData(void *p);
   static void destruct_CbmRichDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichDigiData*)
   {
      ::CbmRichDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichDigiData", ::CbmRichDigiData::Class_Version(), "CbmRichDigiData.h", 29,
                  typeid(::CbmRichDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichDigiData) );
      instance.SetNew(&new_CbmRichDigiData);
      instance.SetNewArray(&newArray_CbmRichDigiData);
      instance.SetDelete(&delete_CbmRichDigiData);
      instance.SetDeleteArray(&deleteArray_CbmRichDigiData);
      instance.SetDestructor(&destruct_CbmRichDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmRichDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsDigi(void *p = nullptr);
   static void *newArray_CbmStsDigi(Long_t size, void *p);
   static void delete_CbmStsDigi(void *p);
   static void deleteArray_CbmStsDigi(void *p);
   static void destruct_CbmStsDigi(void *p);

   // Schema evolution read functions
   static void read_CbmStsDigi_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
#if 0
      static Int_t id_fTime = oldObj->GetId("fTime");
      static Int_t id_fAddress = oldObj->GetId("fAddress");
      static Int_t id_fChannel = oldObj->GetId("fChannel");
      static Int_t id_fCharge = oldObj->GetId("fCharge");
#endif
      struct CbmStsDigi_Onfile {
         int64_t &fTime;
         int32_t &fAddress;
         uint16_t &fChannel;
         uint16_t &fCharge;
         CbmStsDigi_Onfile(int64_t &onfile_fTime, int32_t &onfile_fAddress, uint16_t &onfile_fChannel, uint16_t &onfile_fCharge ): fTime(onfile_fTime), fAddress(onfile_fAddress), fChannel(onfile_fChannel), fCharge(onfile_fCharge) {}
      };
      static Long_t offset_Onfile_CbmStsDigi_fTime = oldObj->GetClass()->GetDataMemberOffset("fTime");
      static Long_t offset_Onfile_CbmStsDigi_fAddress = oldObj->GetClass()->GetDataMemberOffset("fAddress");
      static Long_t offset_Onfile_CbmStsDigi_fChannel = oldObj->GetClass()->GetDataMemberOffset("fChannel");
      static Long_t offset_Onfile_CbmStsDigi_fCharge = oldObj->GetClass()->GetDataMemberOffset("fCharge");
      char *onfile_add = (char*)oldObj->GetObject();
      CbmStsDigi_Onfile onfile(
         *(int64_t*)(onfile_add+offset_Onfile_CbmStsDigi_fTime),
         *(int32_t*)(onfile_add+offset_Onfile_CbmStsDigi_fAddress),
         *(uint16_t*)(onfile_add+offset_Onfile_CbmStsDigi_fChannel),
         *(uint16_t*)(onfile_add+offset_Onfile_CbmStsDigi_fCharge) );

      CbmStsDigi* newObj = (CbmStsDigi*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      newObj->SetAddressAndTime(onfile.fAddress, onfile.fTime); \
            newObj->SetChannelAndCharge(onfile.fChannel, onfile.fCharge); \
          
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsDigi*)
   {
      ::CbmStsDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsDigi", ::CbmStsDigi::Class_Version(), "CbmStsDigi.h", 40,
                  typeid(::CbmStsDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsDigi) );
      instance.SetNew(&new_CbmStsDigi);
      instance.SetNewArray(&newArray_CbmStsDigi);
      instance.SetDelete(&delete_CbmStsDigi);
      instance.SetDeleteArray(&deleteArray_CbmStsDigi);
      instance.SetDestructor(&destruct_CbmStsDigi);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "CbmStsDigi";
      rule->fTarget      = "";
      rule->fSource      = "int64_t fTime; int32_t fAddress; uint16_t fChannel; uint16_t fCharge";
      rule->fFunctionPtr = (void *)TFunc2void( read_CbmStsDigi_0);
      rule->fCode        = " newObj->SetAddressAndTime(onfile.fAddress, onfile.fTime); \\n            newObj->SetChannelAndCharge(onfile.fChannel, onfile.fCharge); \\n          ";
      rule->fVersion     = "[7]";
      rule->fInclude     = "Logger.h";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsDigi*)
   {
      return GenerateInitInstanceLocal((::CbmStsDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsDigiData(void *p = nullptr);
   static void *newArray_CbmStsDigiData(Long_t size, void *p);
   static void delete_CbmStsDigiData(void *p);
   static void deleteArray_CbmStsDigiData(void *p);
   static void destruct_CbmStsDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsDigiData*)
   {
      ::CbmStsDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsDigiData", ::CbmStsDigiData::Class_Version(), "CbmStsDigiData.h", 29,
                  typeid(::CbmStsDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsDigiData) );
      instance.SetNew(&new_CbmStsDigiData);
      instance.SetNewArray(&newArray_CbmStsDigiData);
      instance.SetDelete(&delete_CbmStsDigiData);
      instance.SetDeleteArray(&deleteArray_CbmStsDigiData);
      instance.SetDestructor(&destruct_CbmStsDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmStsDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmTofAddress_Dictionary();
   static void CbmTofAddress_TClassManip(TClass*);
   static void *new_CbmTofAddress(void *p = nullptr);
   static void *newArray_CbmTofAddress(Long_t size, void *p);
   static void delete_CbmTofAddress(void *p);
   static void deleteArray_CbmTofAddress(void *p);
   static void destruct_CbmTofAddress(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofAddress*)
   {
      ::CbmTofAddress *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmTofAddress));
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofAddress", "CbmTofAddress.h", 48,
                  typeid(::CbmTofAddress), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmTofAddress_Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofAddress) );
      instance.SetNew(&new_CbmTofAddress);
      instance.SetNewArray(&newArray_CbmTofAddress);
      instance.SetDelete(&delete_CbmTofAddress);
      instance.SetDeleteArray(&deleteArray_CbmTofAddress);
      instance.SetDestructor(&destruct_CbmTofAddress);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofAddress*)
   {
      return GenerateInitInstanceLocal((::CbmTofAddress*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofAddress*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmTofAddress_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmTofAddress*)nullptr)->GetClass();
      CbmTofAddress_TClassManip(theClass);
   return theClass;
   }

   static void CbmTofAddress_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofDigi(void *p = nullptr);
   static void *newArray_CbmTofDigi(Long_t size, void *p);
   static void delete_CbmTofDigi(void *p);
   static void deleteArray_CbmTofDigi(void *p);
   static void destruct_CbmTofDigi(void *p);

   // Schema evolution read functions
   static void read_CbmTofDigi_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
#if 0
      static Int_t id_fuAddress = oldObj->GetId("fuAddress");
#endif
      struct CbmTofDigi_Onfile {
         uint32_t &fuAddress;
         CbmTofDigi_Onfile(uint32_t &onfile_fuAddress ): fuAddress(onfile_fuAddress) {}
      };
      static Long_t offset_Onfile_CbmTofDigi_fuAddress = oldObj->GetClass()->GetDataMemberOffset("fuAddress");
      char *onfile_add = (char*)oldObj->GetObject();
      CbmTofDigi_Onfile onfile(
         *(uint32_t*)(onfile_add+offset_Onfile_CbmTofDigi_fuAddress) );

      static TClassRef cls("CbmTofDigi");
      static Long_t offset_fuAddress = cls->GetDataMemberOffset("fuAddress");
      uint32_t& fuAddress = *(uint32_t*)(target+offset_fuAddress);
      CbmTofDigi* newObj = (CbmTofDigi*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      uint32_t system = (onfile.fuAddress >> 0) & ((1 << 4) - 1); \
           uint32_t smId = (onfile.fuAddress >> 4) & ((1 << 8) - 1); \
           uint32_t smType = (onfile.fuAddress >> 12) & ((1 << 4) - 1); \
           uint32_t rpcId = (onfile.fuAddress >> 16) & ((1 << 7) - 1); \
           uint32_t chSide = (onfile.fuAddress >> 23) & ((1 << 1) - 1); \
           uint32_t chId = (onfile.fuAddress >> 24) & ((1 << 8) - 1); \
           if ( smId > 127 || rpcId > 63 || chId > 63 ) { \
             LOG(error) << "You are trying to read an outdated version of CbmTofDigi"; \
             LOG(error) << "where the unique tof address can't be converted"; \
             LOG(error) << "automatically to the new tof addressing scheme."; \
             LOG(fatal) << "Stop execution."; \
           } \
           uint32_t rpcType = 0; \
           fuAddress = (system & ((1 << 4) - 1)) \
                     + ((smId & ((1 << 7) - 1)) << 4) \
                     + ((smType & ((1 << 4) - 1)) << 11) \
                     + ((chSide & ((1 << 1) - 1)) << 21) \
                     + ((rpcId & ((1 << 6) - 1)) << 15) \
                     + ((chId & ((1 << 6) - 1)) << 22) \
                     + ((rpcType & ((1 << 4) - 1)) << 28); \
         
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofDigi*)
   {
      ::CbmTofDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofDigi", ::CbmTofDigi::Class_Version(), "CbmTofDigi.h", 47,
                  typeid(::CbmTofDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofDigi) );
      instance.SetNew(&new_CbmTofDigi);
      instance.SetNewArray(&newArray_CbmTofDigi);
      instance.SetDelete(&delete_CbmTofDigi);
      instance.SetDeleteArray(&deleteArray_CbmTofDigi);
      instance.SetDestructor(&destruct_CbmTofDigi);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "CbmTofDigi";
      rule->fTarget      = "fuAddress";
      rule->fSource      = "uint32_t fuAddress";
      rule->fFunctionPtr = (void *)TFunc2void( read_CbmTofDigi_0);
      rule->fCode        = " uint32_t system = (onfile.fuAddress >> 0) & ((1 << 4) - 1); \\n           uint32_t smId = (onfile.fuAddress >> 4) & ((1 << 8) - 1); \\n           uint32_t smType = (onfile.fuAddress >> 12) & ((1 << 4) - 1); \\n           uint32_t rpcId = (onfile.fuAddress >> 16) & ((1 << 7) - 1); \\n           uint32_t chSide = (onfile.fuAddress >> 23) & ((1 << 1) - 1); \\n           uint32_t chId = (onfile.fuAddress >> 24) & ((1 << 8) - 1); \\n           if ( smId > 127 || rpcId > 63 || chId > 63 ) { \\n             LOG(error) << \"You are trying to read an outdated version of CbmTofDigi\"; \\n             LOG(error) << \"where the unique tof address can't be converted\"; \\n             LOG(error) << \"automatically to the new tof addressing scheme.\"; \\n             LOG(fatal) << \"Stop execution.\"; \\n           } \\n           uint32_t rpcType = 0; \\n           fuAddress = (system & ((1 << 4) - 1)) \\n                     + ((smId & ((1 << 7) - 1)) << 4) \\n                     + ((smType & ((1 << 4) - 1)) << 11) \\n                     + ((chSide & ((1 << 1) - 1)) << 21) \\n                     + ((rpcId & ((1 << 6) - 1)) << 15) \\n                     + ((chId & ((1 << 6) - 1)) << 22) \\n                     + ((rpcType & ((1 << 4) - 1)) << 28); \\n         ";
      rule->fVersion     = "[1-2]";
      rule->fInclude     = "Logger.h";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofDigi*)
   {
      return GenerateInitInstanceLocal((::CbmTofDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofDigiData(void *p = nullptr);
   static void *newArray_CbmTofDigiData(Long_t size, void *p);
   static void delete_CbmTofDigiData(void *p);
   static void deleteArray_CbmTofDigiData(void *p);
   static void destruct_CbmTofDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofDigiData*)
   {
      ::CbmTofDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofDigiData", ::CbmTofDigiData::Class_Version(), "CbmTofDigiData.h", 30,
                  typeid(::CbmTofDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofDigiData) );
      instance.SetNew(&new_CbmTofDigiData);
      instance.SetNewArray(&newArray_CbmTofDigiData);
      instance.SetDelete(&delete_CbmTofDigiData);
      instance.SetDeleteArray(&deleteArray_CbmTofDigiData);
      instance.SetDestructor(&destruct_CbmTofDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmTofDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdDigi(void *p = nullptr);
   static void *newArray_CbmTrdDigi(Long_t size, void *p);
   static void delete_CbmTrdDigi(void *p);
   static void deleteArray_CbmTrdDigi(void *p);
   static void destruct_CbmTrdDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdDigi*)
   {
      ::CbmTrdDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdDigi", ::CbmTrdDigi::Class_Version(), "CbmTrdDigi.h", 21,
                  typeid(::CbmTrdDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdDigi) );
      instance.SetNew(&new_CbmTrdDigi);
      instance.SetNewArray(&newArray_CbmTrdDigi);
      instance.SetDelete(&delete_CbmTrdDigi);
      instance.SetDeleteArray(&deleteArray_CbmTrdDigi);
      instance.SetDestructor(&destruct_CbmTrdDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdDigi*)
   {
      return GenerateInitInstanceLocal((::CbmTrdDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdDigiData(void *p = nullptr);
   static void *newArray_CbmTrdDigiData(Long_t size, void *p);
   static void delete_CbmTrdDigiData(void *p);
   static void deleteArray_CbmTrdDigiData(void *p);
   static void destruct_CbmTrdDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdDigiData*)
   {
      ::CbmTrdDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdDigiData", ::CbmTrdDigiData::Class_Version(), "CbmTrdDigiData.h", 29,
                  typeid(::CbmTrdDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdDigiData) );
      instance.SetNew(&new_CbmTrdDigiData);
      instance.SetNewArray(&newArray_CbmTrdDigiData);
      instance.SetDelete(&delete_CbmTrdDigiData);
      instance.SetDeleteArray(&deleteArray_CbmTrdDigiData);
      instance.SetDestructor(&destruct_CbmTrdDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmTrdDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDigiData(void *p = nullptr);
   static void *newArray_CbmDigiData(Long_t size, void *p);
   static void delete_CbmDigiData(void *p);
   static void deleteArray_CbmDigiData(void *p);
   static void destruct_CbmDigiData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiData*)
   {
      ::CbmDigiData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiData", ::CbmDigiData::Class_Version(), "CbmDigiData.h", 31,
                  typeid(::CbmDigiData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigiData::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiData) );
      instance.SetNew(&new_CbmDigiData);
      instance.SetNewArray(&newArray_CbmDigiData);
      instance.SetDelete(&delete_CbmDigiData);
      instance.SetDeleteArray(&deleteArray_CbmDigiData);
      instance.SetDestructor(&destruct_CbmDigiData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiData*)
   {
      return GenerateInitInstanceLocal((::CbmDigiData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmDigiEvent(void *p = nullptr);
   static void *newArray_CbmDigiEvent(Long_t size, void *p);
   static void delete_CbmDigiEvent(void *p);
   static void deleteArray_CbmDigiEvent(void *p);
   static void destruct_CbmDigiEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiEvent*)
   {
      ::CbmDigiEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiEvent", ::CbmDigiEvent::Class_Version(), "CbmDigiEvent.h", 19,
                  typeid(::CbmDigiEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigiEvent::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiEvent) );
      instance.SetNew(&new_CbmDigiEvent);
      instance.SetNewArray(&newArray_CbmDigiEvent);
      instance.SetDelete(&delete_CbmDigiEvent);
      instance.SetDeleteArray(&deleteArray_CbmDigiEvent);
      instance.SetDestructor(&destruct_CbmDigiEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiEvent*)
   {
      return GenerateInitInstanceLocal((::CbmDigiEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiTimeslice_Dictionary();
   static void CbmDigiTimeslice_TClassManip(TClass*);
   static void *new_CbmDigiTimeslice(void *p = nullptr);
   static void *newArray_CbmDigiTimeslice(Long_t size, void *p);
   static void delete_CbmDigiTimeslice(void *p);
   static void deleteArray_CbmDigiTimeslice(void *p);
   static void destruct_CbmDigiTimeslice(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiTimeslice*)
   {
      ::CbmDigiTimeslice *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmDigiTimeslice));
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiTimeslice", "CbmDigiTimeslice.h", 21,
                  typeid(::CbmDigiTimeslice), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiTimeslice_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiTimeslice) );
      instance.SetNew(&new_CbmDigiTimeslice);
      instance.SetNewArray(&newArray_CbmDigiTimeslice);
      instance.SetDelete(&delete_CbmDigiTimeslice);
      instance.SetDeleteArray(&deleteArray_CbmDigiTimeslice);
      instance.SetDestructor(&destruct_CbmDigiTimeslice);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiTimeslice*)
   {
      return GenerateInitInstanceLocal((::CbmDigiTimeslice*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiTimeslice*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiTimeslice_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiTimeslice*)nullptr)->GetClass();
      CbmDigiTimeslice_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiTimeslice_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmLink(void *p = nullptr);
   static void *newArray_CbmLink(Long_t size, void *p);
   static void delete_CbmLink(void *p);
   static void deleteArray_CbmLink(void *p);
   static void destruct_CbmLink(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmLink*)
   {
      ::CbmLink *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmLink >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmLink", ::CbmLink::Class_Version(), "CbmLink.h", 22,
                  typeid(::CbmLink), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmLink::Dictionary, isa_proxy, 4,
                  sizeof(::CbmLink) );
      instance.SetNew(&new_CbmLink);
      instance.SetNewArray(&newArray_CbmLink);
      instance.SetDelete(&delete_CbmLink);
      instance.SetDeleteArray(&deleteArray_CbmLink);
      instance.SetDestructor(&destruct_CbmLink);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmLink*)
   {
      return GenerateInitInstanceLocal((::CbmLink*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmLink*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMatch(void *p = nullptr);
   static void *newArray_CbmMatch(Long_t size, void *p);
   static void delete_CbmMatch(void *p);
   static void deleteArray_CbmMatch(void *p);
   static void destruct_CbmMatch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMatch*)
   {
      ::CbmMatch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMatch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMatch", ::CbmMatch::Class_Version(), "CbmMatch.h", 26,
                  typeid(::CbmMatch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMatch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMatch) );
      instance.SetNew(&new_CbmMatch);
      instance.SetNewArray(&newArray_CbmMatch);
      instance.SetDelete(&delete_CbmMatch);
      instance.SetDeleteArray(&deleteArray_CbmMatch);
      instance.SetDestructor(&destruct_CbmMatch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMatch*)
   {
      return GenerateInitInstanceLocal((::CbmMatch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMatch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTimeSlice(void *p = nullptr);
   static void *newArray_CbmTimeSlice(Long_t size, void *p);
   static void delete_CbmTimeSlice(void *p);
   static void deleteArray_CbmTimeSlice(void *p);
   static void destruct_CbmTimeSlice(void *p);
   static void streamer_CbmTimeSlice(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTimeSlice*)
   {
      ::CbmTimeSlice *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTimeSlice >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTimeSlice", ::CbmTimeSlice::Class_Version(), "CbmTimeSlice.h", 33,
                  typeid(::CbmTimeSlice), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTimeSlice::Dictionary, isa_proxy, 16,
                  sizeof(::CbmTimeSlice) );
      instance.SetNew(&new_CbmTimeSlice);
      instance.SetNewArray(&newArray_CbmTimeSlice);
      instance.SetDelete(&delete_CbmTimeSlice);
      instance.SetDeleteArray(&deleteArray_CbmTimeSlice);
      instance.SetDestructor(&destruct_CbmTimeSlice);
      instance.SetStreamerFunc(&streamer_CbmTimeSlice);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTimeSlice*)
   {
      return GenerateInitInstanceLocal((::CbmTimeSlice*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTimeSlice*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmVertex(void *p = nullptr);
   static void *newArray_CbmVertex(Long_t size, void *p);
   static void delete_CbmVertex(void *p);
   static void deleteArray_CbmVertex(void *p);
   static void destruct_CbmVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmVertex*)
   {
      ::CbmVertex *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmVertex >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmVertex", ::CbmVertex::Class_Version(), "CbmVertex.h", 31,
                  typeid(::CbmVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmVertex::Dictionary, isa_proxy, 4,
                  sizeof(::CbmVertex) );
      instance.SetNew(&new_CbmVertex);
      instance.SetNewArray(&newArray_CbmVertex);
      instance.SetDelete(&delete_CbmVertex);
      instance.SetDeleteArray(&deleteArray_CbmVertex);
      instance.SetDestructor(&destruct_CbmVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmVertex*)
   {
      return GenerateInitInstanceLocal((::CbmVertex*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmVertex*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmEvent(void *p = nullptr);
   static void *newArray_CbmEvent(Long_t size, void *p);
   static void delete_CbmEvent(void *p);
   static void deleteArray_CbmEvent(void *p);
   static void destruct_CbmEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmEvent*)
   {
      ::CbmEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmEvent", ::CbmEvent::Class_Version(), "CbmEvent.h", 34,
                  typeid(::CbmEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmEvent::Dictionary, isa_proxy, 4,
                  sizeof(::CbmEvent) );
      instance.SetNew(&new_CbmEvent);
      instance.SetNewArray(&newArray_CbmEvent);
      instance.SetDelete(&delete_CbmEvent);
      instance.SetDeleteArray(&deleteArray_CbmEvent);
      instance.SetDestructor(&destruct_CbmEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmEvent*)
   {
      return GenerateInitInstanceLocal((::CbmEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CbmDigiContainer(void *p);
   static void deleteArray_CbmDigiContainer(void *p);
   static void destruct_CbmDigiContainer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiContainer*)
   {
      ::CbmDigiContainer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiContainer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiContainer", ::CbmDigiContainer::Class_Version(), "CbmDigiContainer.h", 34,
                  typeid(::CbmDigiContainer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmDigiContainer::Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiContainer) );
      instance.SetDelete(&delete_CbmDigiContainer);
      instance.SetDeleteArray(&deleteArray_CbmDigiContainer);
      instance.SetDestructor(&destruct_CbmDigiContainer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiContainer*)
   {
      return GenerateInitInstanceLocal((::CbmDigiContainer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiContainer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmPsdDspgR_Dictionary();
   static void CbmDigiVectorlECbmPsdDspgR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmPsdDspgR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmPsdDspgR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmPsdDspgR(void *p);
   static void deleteArray_CbmDigiVectorlECbmPsdDspgR(void *p);
   static void destruct_CbmDigiVectorlECbmPsdDspgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmPsdDsp>*)
   {
      ::CbmDigiVector<CbmPsdDsp> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmPsdDsp> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmPsdDsp>", ::CbmDigiVector<CbmPsdDsp>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmPsdDsp>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmPsdDspgR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmPsdDsp>) );
      instance.SetNew(&new_CbmDigiVectorlECbmPsdDspgR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmPsdDspgR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmPsdDspgR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmPsdDspgR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmPsdDspgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmPsdDsp>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmPsdDsp>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDsp>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmPsdDspgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDsp>*)nullptr)->GetClass();
      CbmDigiVectorlECbmPsdDspgR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmPsdDspgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmPsdDigigR_Dictionary();
   static void CbmDigiVectorlECbmPsdDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmPsdDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmPsdDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmPsdDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmPsdDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmPsdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmPsdDigi>*)
   {
      ::CbmDigiVector<CbmPsdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmPsdDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmPsdDigi>", ::CbmDigiVector<CbmPsdDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmPsdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmPsdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmPsdDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmPsdDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmPsdDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmPsdDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmPsdDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmPsdDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmPsdDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmPsdDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmPsdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmPsdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmPsdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmTofDigigR_Dictionary();
   static void CbmDigiVectorlECbmTofDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmTofDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmTofDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmTofDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmTofDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmTofDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmTofDigi>*)
   {
      ::CbmDigiVector<CbmTofDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmTofDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmTofDigi>", ::CbmDigiVector<CbmTofDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmTofDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmTofDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmTofDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmTofDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmTofDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmTofDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmTofDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmTofDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmTofDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmTofDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTofDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmTofDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTofDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmTofDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmTofDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmTrdDigigR_Dictionary();
   static void CbmDigiVectorlECbmTrdDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmTrdDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmTrdDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmTrdDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmTrdDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmTrdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmTrdDigi>*)
   {
      ::CbmDigiVector<CbmTrdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmTrdDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmTrdDigi>", ::CbmDigiVector<CbmTrdDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmTrdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmTrdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmTrdDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmTrdDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmTrdDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmTrdDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmTrdDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmTrdDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmTrdDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmTrdDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTrdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmTrdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTrdDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmTrdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmTrdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmMuchBeamTimeDigigR_Dictionary();
   static void CbmDigiVectorlECbmMuchBeamTimeDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmMuchBeamTimeDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)
   {
      ::CbmDigiVector<CbmMuchBeamTimeDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmMuchBeamTimeDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmMuchBeamTimeDigi>", ::CbmDigiVector<CbmMuchBeamTimeDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmMuchBeamTimeDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmMuchBeamTimeDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmMuchBeamTimeDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmMuchBeamTimeDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmMuchBeamTimeDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmMuchBeamTimeDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmMuchBeamTimeDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmMuchBeamTimeDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmMuchBeamTimeDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmMuchBeamTimeDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmMuchBeamTimeDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmMuchDigigR_Dictionary();
   static void CbmDigiVectorlECbmMuchDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmMuchDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmMuchDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmMuchDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmMuchDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmMuchDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmMuchDigi>*)
   {
      ::CbmDigiVector<CbmMuchDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmMuchDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmMuchDigi>", ::CbmDigiVector<CbmMuchDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmMuchDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmMuchDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmMuchDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmMuchDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmMuchDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmMuchDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmMuchDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmMuchDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmMuchDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmMuchDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmMuchDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmMuchDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmMuchDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmRichDigigR_Dictionary();
   static void CbmDigiVectorlECbmRichDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmRichDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmRichDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmRichDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmRichDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmRichDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmRichDigi>*)
   {
      ::CbmDigiVector<CbmRichDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmRichDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmRichDigi>", ::CbmDigiVector<CbmRichDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmRichDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmRichDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmRichDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmRichDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmRichDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmRichDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmRichDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmRichDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmRichDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmRichDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmRichDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmRichDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmRichDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmRichDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmRichDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmStsDigigR_Dictionary();
   static void CbmDigiVectorlECbmStsDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmStsDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmStsDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmStsDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmStsDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmStsDigi>*)
   {
      ::CbmDigiVector<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmStsDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmStsDigi>", ::CbmDigiVector<CbmStsDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmStsDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmStsDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmStsDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmStsDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmStsDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmStsDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmStsDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmStsDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmStsDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiVectorlECbmMvdDigigR_Dictionary();
   static void CbmDigiVectorlECbmMvdDigigR_TClassManip(TClass*);
   static void *new_CbmDigiVectorlECbmMvdDigigR(void *p = nullptr);
   static void *newArray_CbmDigiVectorlECbmMvdDigigR(Long_t size, void *p);
   static void delete_CbmDigiVectorlECbmMvdDigigR(void *p);
   static void deleteArray_CbmDigiVectorlECbmMvdDigigR(void *p);
   static void destruct_CbmDigiVectorlECbmMvdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiVector<CbmMvdDigi>*)
   {
      ::CbmDigiVector<CbmMvdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmDigiVector<CbmMvdDigi> >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiVector<CbmMvdDigi>", ::CbmDigiVector<CbmMvdDigi>::Class_Version(), "CbmDigiVector.h", 28,
                  typeid(::CbmDigiVector<CbmMvdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiVectorlECbmMvdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiVector<CbmMvdDigi>) );
      instance.SetNew(&new_CbmDigiVectorlECbmMvdDigigR);
      instance.SetNewArray(&newArray_CbmDigiVectorlECbmMvdDigigR);
      instance.SetDelete(&delete_CbmDigiVectorlECbmMvdDigigR);
      instance.SetDeleteArray(&deleteArray_CbmDigiVectorlECbmMvdDigigR);
      instance.SetDestructor(&destruct_CbmDigiVectorlECbmMvdDigigR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiVector<CbmMvdDigi>*)
   {
      return GenerateInitInstanceLocal((::CbmDigiVector<CbmMvdDigi>*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMvdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiVectorlECbmMvdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMvdDigi>*)nullptr)->GetClass();
      CbmDigiVectorlECbmMvdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiVectorlECbmMvdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmEventStore(void *p = nullptr);
   static void *newArray_CbmEventStore(Long_t size, void *p);
   static void delete_CbmEventStore(void *p);
   static void deleteArray_CbmEventStore(void *p);
   static void destruct_CbmEventStore(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmEventStore*)
   {
      ::CbmEventStore *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmEventStore >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmEventStore", ::CbmEventStore::Class_Version(), "CbmEventStore.h", 43,
                  typeid(::CbmEventStore), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmEventStore::Dictionary, isa_proxy, 4,
                  sizeof(::CbmEventStore) );
      instance.SetNew(&new_CbmEventStore);
      instance.SetNewArray(&newArray_CbmEventStore);
      instance.SetDelete(&delete_CbmEventStore);
      instance.SetDeleteArray(&deleteArray_CbmEventStore);
      instance.SetDestructor(&destruct_CbmEventStore);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmEventStore*)
   {
      return GenerateInitInstanceLocal((::CbmEventStore*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmEventStore*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmHit(void *p = nullptr);
   static void *newArray_CbmHit(Long_t size, void *p);
   static void delete_CbmHit(void *p);
   static void deleteArray_CbmHit(void *p);
   static void destruct_CbmHit(void *p);
   static void streamer_CbmHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmHit*)
   {
      ::CbmHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmHit", ::CbmHit::Class_Version(), "CbmHit.h", 43,
                  typeid(::CbmHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmHit::Dictionary, isa_proxy, 16,
                  sizeof(::CbmHit) );
      instance.SetNew(&new_CbmHit);
      instance.SetNewArray(&newArray_CbmHit);
      instance.SetDelete(&delete_CbmHit);
      instance.SetDeleteArray(&deleteArray_CbmHit);
      instance.SetDestructor(&destruct_CbmHit);
      instance.SetStreamerFunc(&streamer_CbmHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmHit*)
   {
      return GenerateInitInstanceLocal((::CbmHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPixelHit(void *p = nullptr);
   static void *newArray_CbmPixelHit(Long_t size, void *p);
   static void delete_CbmPixelHit(void *p);
   static void deleteArray_CbmPixelHit(void *p);
   static void destruct_CbmPixelHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPixelHit*)
   {
      ::CbmPixelHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPixelHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPixelHit", ::CbmPixelHit::Class_Version(), "CbmPixelHit.h", 25,
                  typeid(::CbmPixelHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPixelHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPixelHit) );
      instance.SetNew(&new_CbmPixelHit);
      instance.SetNewArray(&newArray_CbmPixelHit);
      instance.SetDelete(&delete_CbmPixelHit);
      instance.SetDeleteArray(&deleteArray_CbmPixelHit);
      instance.SetDestructor(&destruct_CbmPixelHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPixelHit*)
   {
      return GenerateInitInstanceLocal((::CbmPixelHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPixelHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStripHit(void *p = nullptr);
   static void *newArray_CbmStripHit(Long_t size, void *p);
   static void delete_CbmStripHit(void *p);
   static void deleteArray_CbmStripHit(void *p);
   static void destruct_CbmStripHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStripHit*)
   {
      ::CbmStripHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStripHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStripHit", ::CbmStripHit::Class_Version(), "CbmStripHit.h", 26,
                  typeid(::CbmStripHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStripHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStripHit) );
      instance.SetNew(&new_CbmStripHit);
      instance.SetNewArray(&newArray_CbmStripHit);
      instance.SetDelete(&delete_CbmStripHit);
      instance.SetDeleteArray(&deleteArray_CbmStripHit);
      instance.SetDestructor(&destruct_CbmStripHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStripHit*)
   {
      return GenerateInitInstanceLocal((::CbmStripHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStripHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrack(void *p = nullptr);
   static void *newArray_CbmTrack(Long_t size, void *p);
   static void delete_CbmTrack(void *p);
   static void deleteArray_CbmTrack(void *p);
   static void destruct_CbmTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrack*)
   {
      ::CbmTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrack", ::CbmTrack::Class_Version(), "CbmTrack.h", 37,
                  typeid(::CbmTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrack) );
      instance.SetNew(&new_CbmTrack);
      instance.SetNewArray(&newArray_CbmTrack);
      instance.SetDelete(&delete_CbmTrack);
      instance.SetDeleteArray(&deleteArray_CbmTrack);
      instance.SetDestructor(&destruct_CbmTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrack*)
   {
      return GenerateInitInstanceLocal((::CbmTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackMatch(void *p = nullptr);
   static void *newArray_CbmTrackMatch(Long_t size, void *p);
   static void delete_CbmTrackMatch(void *p);
   static void deleteArray_CbmTrackMatch(void *p);
   static void destruct_CbmTrackMatch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackMatch*)
   {
      ::CbmTrackMatch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackMatch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackMatch", ::CbmTrackMatch::Class_Version(), "CbmTrackMatch.h", 23,
                  typeid(::CbmTrackMatch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackMatch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackMatch) );
      instance.SetNew(&new_CbmTrackMatch);
      instance.SetNewArray(&newArray_CbmTrackMatch);
      instance.SetDelete(&delete_CbmTrackMatch);
      instance.SetDeleteArray(&deleteArray_CbmTrackMatch);
      instance.SetDestructor(&destruct_CbmTrackMatch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackMatch*)
   {
      return GenerateInitInstanceLocal((::CbmTrackMatch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackMatch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmCluster(void *p = nullptr);
   static void *newArray_CbmCluster(Long_t size, void *p);
   static void delete_CbmCluster(void *p);
   static void deleteArray_CbmCluster(void *p);
   static void destruct_CbmCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmCluster*)
   {
      ::CbmCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmCluster", ::CbmCluster::Class_Version(), "CbmCluster.h", 30,
                  typeid(::CbmCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmCluster) );
      instance.SetNew(&new_CbmCluster);
      instance.SetNewArray(&newArray_CbmCluster);
      instance.SetDelete(&delete_CbmCluster);
      instance.SetDeleteArray(&deleteArray_CbmCluster);
      instance.SetDestructor(&destruct_CbmCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmCluster*)
   {
      return GenerateInitInstanceLocal((::CbmCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmModuleList_Dictionary();
   static void CbmModuleList_TClassManip(TClass*);
   static void *new_CbmModuleList(void *p = nullptr);
   static void *newArray_CbmModuleList(Long_t size, void *p);
   static void delete_CbmModuleList(void *p);
   static void deleteArray_CbmModuleList(void *p);
   static void destruct_CbmModuleList(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmModuleList*)
   {
      ::CbmModuleList *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmModuleList));
      static ::ROOT::TGenericClassInfo 
         instance("CbmModuleList", "CbmModuleList.h", 31,
                  typeid(::CbmModuleList), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmModuleList_Dictionary, isa_proxy, 0,
                  sizeof(::CbmModuleList) );
      instance.SetNew(&new_CbmModuleList);
      instance.SetNewArray(&newArray_CbmModuleList);
      instance.SetDelete(&delete_CbmModuleList);
      instance.SetDeleteArray(&deleteArray_CbmModuleList);
      instance.SetDestructor(&destruct_CbmModuleList);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmModuleList*)
   {
      return GenerateInitInstanceLocal((::CbmModuleList*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmModuleList*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmModuleList_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmModuleList*)nullptr)->GetClass();
      CbmModuleList_TClassManip(theClass);
   return theClass;
   }

   static void CbmModuleList_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmErrorMessage(void *p = nullptr);
   static void *newArray_CbmErrorMessage(Long_t size, void *p);
   static void delete_CbmErrorMessage(void *p);
   static void deleteArray_CbmErrorMessage(void *p);
   static void destruct_CbmErrorMessage(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmErrorMessage*)
   {
      ::CbmErrorMessage *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmErrorMessage >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmErrorMessage", ::CbmErrorMessage::Class_Version(), "CbmErrorMessage.h", 44,
                  typeid(::CbmErrorMessage), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmErrorMessage::Dictionary, isa_proxy, 4,
                  sizeof(::CbmErrorMessage) );
      instance.SetNew(&new_CbmErrorMessage);
      instance.SetNewArray(&newArray_CbmErrorMessage);
      instance.SetDelete(&delete_CbmErrorMessage);
      instance.SetDeleteArray(&deleteArray_CbmErrorMessage);
      instance.SetDestructor(&destruct_CbmErrorMessage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmErrorMessage*)
   {
      return GenerateInitInstanceLocal((::CbmErrorMessage*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmErrorMessage*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMCTrack(void *p = nullptr);
   static void *newArray_CbmMCTrack(Long_t size, void *p);
   static void delete_CbmMCTrack(void *p);
   static void deleteArray_CbmMCTrack(void *p);
   static void destruct_CbmMCTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCTrack*)
   {
      ::CbmMCTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCTrack", ::CbmMCTrack::Class_Version(), "CbmMCTrack.h", 40,
                  typeid(::CbmMCTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMCTrack) );
      instance.SetNew(&new_CbmMCTrack);
      instance.SetNewArray(&newArray_CbmMCTrack);
      instance.SetDelete(&delete_CbmMCTrack);
      instance.SetDeleteArray(&deleteArray_CbmMCTrack);
      instance.SetDestructor(&destruct_CbmMCTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCTrack*)
   {
      return GenerateInitInstanceLocal((::CbmMCTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMCEventInfo(void *p = nullptr);
   static void *newArray_CbmMCEventInfo(Long_t size, void *p);
   static void delete_CbmMCEventInfo(void *p);
   static void deleteArray_CbmMCEventInfo(void *p);
   static void destruct_CbmMCEventInfo(void *p);
   static void streamer_CbmMCEventInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCEventInfo*)
   {
      ::CbmMCEventInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCEventInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCEventInfo", ::CbmMCEventInfo::Class_Version(), "CbmMCEventInfo.h", 33,
                  typeid(::CbmMCEventInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCEventInfo::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMCEventInfo) );
      instance.SetNew(&new_CbmMCEventInfo);
      instance.SetNewArray(&newArray_CbmMCEventInfo);
      instance.SetDelete(&delete_CbmMCEventInfo);
      instance.SetDeleteArray(&deleteArray_CbmMCEventInfo);
      instance.SetDestructor(&destruct_CbmMCEventInfo);
      instance.SetStreamerFunc(&streamer_CbmMCEventInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCEventInfo*)
   {
      return GenerateInitInstanceLocal((::CbmMCEventInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCEventInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMCEventList(void *p = nullptr);
   static void *newArray_CbmMCEventList(Long_t size, void *p);
   static void delete_CbmMCEventList(void *p);
   static void deleteArray_CbmMCEventList(void *p);
   static void destruct_CbmMCEventList(void *p);
   static void streamer_CbmMCEventList(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMCEventList*)
   {
      ::CbmMCEventList *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMCEventList >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMCEventList", ::CbmMCEventList::Class_Version(), "CbmMCEventList.h", 43,
                  typeid(::CbmMCEventList), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMCEventList::Dictionary, isa_proxy, 16,
                  sizeof(::CbmMCEventList) );
      instance.SetNew(&new_CbmMCEventList);
      instance.SetNewArray(&newArray_CbmMCEventList);
      instance.SetDelete(&delete_CbmMCEventList);
      instance.SetDeleteArray(&deleteArray_CbmMCEventList);
      instance.SetDestructor(&destruct_CbmMCEventList);
      instance.SetStreamerFunc(&streamer_CbmMCEventList);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMCEventList*)
   {
      return GenerateInitInstanceLocal((::CbmMCEventList*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMCEventList*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackMatchNew(void *p = nullptr);
   static void *newArray_CbmTrackMatchNew(Long_t size, void *p);
   static void delete_CbmTrackMatchNew(void *p);
   static void deleteArray_CbmTrackMatchNew(void *p);
   static void destruct_CbmTrackMatchNew(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackMatchNew*)
   {
      ::CbmTrackMatchNew *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackMatchNew >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackMatchNew", ::CbmTrackMatchNew::Class_Version(), "CbmTrackMatchNew.h", 23,
                  typeid(::CbmTrackMatchNew), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackMatchNew::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrackMatchNew) );
      instance.SetNew(&new_CbmTrackMatchNew);
      instance.SetNewArray(&newArray_CbmTrackMatchNew);
      instance.SetDelete(&delete_CbmTrackMatchNew);
      instance.SetDeleteArray(&deleteArray_CbmTrackMatchNew);
      instance.SetDestructor(&destruct_CbmTrackMatchNew);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackMatchNew*)
   {
      return GenerateInitInstanceLocal((::CbmTrackMatchNew*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackMatchNew*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTsEventHeader(void *p = nullptr);
   static void *newArray_CbmTsEventHeader(Long_t size, void *p);
   static void delete_CbmTsEventHeader(void *p);
   static void deleteArray_CbmTsEventHeader(void *p);
   static void destruct_CbmTsEventHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTsEventHeader*)
   {
      ::CbmTsEventHeader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTsEventHeader >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTsEventHeader", ::CbmTsEventHeader::Class_Version(), "CbmTsEventHeader.h", 13,
                  typeid(::CbmTsEventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTsEventHeader::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTsEventHeader) );
      instance.SetNew(&new_CbmTsEventHeader);
      instance.SetNewArray(&newArray_CbmTsEventHeader);
      instance.SetDelete(&delete_CbmTsEventHeader);
      instance.SetDeleteArray(&deleteArray_CbmTsEventHeader);
      instance.SetDestructor(&destruct_CbmTsEventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTsEventHeader*)
   {
      return GenerateInitInstanceLocal((::CbmTsEventHeader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTsEventHeader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmDigiBranchBase_Dictionary();
   static void CbmDigiBranchBase_TClassManip(TClass*);
   static void delete_CbmDigiBranchBase(void *p);
   static void deleteArray_CbmDigiBranchBase(void *p);
   static void destruct_CbmDigiBranchBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmDigiBranchBase*)
   {
      ::CbmDigiBranchBase *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmDigiBranchBase));
      static ::ROOT::TGenericClassInfo 
         instance("CbmDigiBranchBase", "CbmDigiBranchBase.h", 29,
                  typeid(::CbmDigiBranchBase), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmDigiBranchBase_Dictionary, isa_proxy, 4,
                  sizeof(::CbmDigiBranchBase) );
      instance.SetDelete(&delete_CbmDigiBranchBase);
      instance.SetDeleteArray(&deleteArray_CbmDigiBranchBase);
      instance.SetDestructor(&destruct_CbmDigiBranchBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmDigiBranchBase*)
   {
      return GenerateInitInstanceLocal((::CbmDigiBranchBase*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmDigiBranchBase*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmDigiBranchBase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmDigiBranchBase*)nullptr)->GetClass();
      CbmDigiBranchBase_TClassManip(theClass);
   return theClass;
   }

   static void CbmDigiBranchBase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdDetectorId(void *p = nullptr);
   static void *newArray_CbmMvdDetectorId(Long_t size, void *p);
   static void delete_CbmMvdDetectorId(void *p);
   static void deleteArray_CbmMvdDetectorId(void *p);
   static void destruct_CbmMvdDetectorId(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdDetectorId*)
   {
      ::CbmMvdDetectorId *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdDetectorId >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdDetectorId", ::CbmMvdDetectorId::Class_Version(), "CbmMvdDetectorId.h", 37,
                  typeid(::CbmMvdDetectorId), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdDetectorId::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdDetectorId) );
      instance.SetNew(&new_CbmMvdDetectorId);
      instance.SetNewArray(&newArray_CbmMvdDetectorId);
      instance.SetDelete(&delete_CbmMvdDetectorId);
      instance.SetDeleteArray(&deleteArray_CbmMvdDetectorId);
      instance.SetDestructor(&destruct_CbmMvdDetectorId);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdDetectorId*)
   {
      return GenerateInitInstanceLocal((::CbmMvdDetectorId*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdDetectorId*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdCluster(void *p = nullptr);
   static void *newArray_CbmMvdCluster(Long_t size, void *p);
   static void delete_CbmMvdCluster(void *p);
   static void deleteArray_CbmMvdCluster(void *p);
   static void destruct_CbmMvdCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdCluster*)
   {
      ::CbmMvdCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdCluster", ::CbmMvdCluster::Class_Version(), "CbmMvdCluster.h", 31,
                  typeid(::CbmMvdCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdCluster) );
      instance.SetNew(&new_CbmMvdCluster);
      instance.SetNewArray(&newArray_CbmMvdCluster);
      instance.SetDelete(&delete_CbmMvdCluster);
      instance.SetDeleteArray(&deleteArray_CbmMvdCluster);
      instance.SetDestructor(&destruct_CbmMvdCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdCluster*)
   {
      return GenerateInitInstanceLocal((::CbmMvdCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdDigi(void *p = nullptr);
   static void *newArray_CbmMvdDigi(Long_t size, void *p);
   static void delete_CbmMvdDigi(void *p);
   static void deleteArray_CbmMvdDigi(void *p);
   static void destruct_CbmMvdDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdDigi*)
   {
      ::CbmMvdDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdDigi", ::CbmMvdDigi::Class_Version(), "CbmMvdDigi.h", 25,
                  typeid(::CbmMvdDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdDigi) );
      instance.SetNew(&new_CbmMvdDigi);
      instance.SetNewArray(&newArray_CbmMvdDigi);
      instance.SetDelete(&delete_CbmMvdDigi);
      instance.SetDeleteArray(&deleteArray_CbmMvdDigi);
      instance.SetDestructor(&destruct_CbmMvdDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdDigi*)
   {
      return GenerateInitInstanceLocal((::CbmMvdDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdHit(void *p = nullptr);
   static void *newArray_CbmMvdHit(Long_t size, void *p);
   static void delete_CbmMvdHit(void *p);
   static void deleteArray_CbmMvdHit(void *p);
   static void destruct_CbmMvdHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdHit*)
   {
      ::CbmMvdHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdHit", ::CbmMvdHit::Class_Version(), "CbmMvdHit.h", 34,
                  typeid(::CbmMvdHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdHit) );
      instance.SetNew(&new_CbmMvdHit);
      instance.SetNewArray(&newArray_CbmMvdHit);
      instance.SetDelete(&delete_CbmMvdHit);
      instance.SetDeleteArray(&deleteArray_CbmMvdHit);
      instance.SetDestructor(&destruct_CbmMvdHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdHit*)
   {
      return GenerateInitInstanceLocal((::CbmMvdHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdPoint(void *p = nullptr);
   static void *newArray_CbmMvdPoint(Long_t size, void *p);
   static void delete_CbmMvdPoint(void *p);
   static void deleteArray_CbmMvdPoint(void *p);
   static void destruct_CbmMvdPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdPoint*)
   {
      ::CbmMvdPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdPoint", ::CbmMvdPoint::Class_Version(), "CbmMvdPoint.h", 34,
                  typeid(::CbmMvdPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdPoint) );
      instance.SetNew(&new_CbmMvdPoint);
      instance.SetNewArray(&newArray_CbmMvdPoint);
      instance.SetDelete(&delete_CbmMvdPoint);
      instance.SetDeleteArray(&deleteArray_CbmMvdPoint);
      instance.SetDestructor(&destruct_CbmMvdPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdPoint*)
   {
      return GenerateInitInstanceLocal((::CbmMvdPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMvdHitMatch(void *p = nullptr);
   static void *newArray_CbmMvdHitMatch(Long_t size, void *p);
   static void delete_CbmMvdHitMatch(void *p);
   static void deleteArray_CbmMvdHitMatch(void *p);
   static void destruct_CbmMvdHitMatch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMvdHitMatch*)
   {
      ::CbmMvdHitMatch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMvdHitMatch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMvdHitMatch", ::CbmMvdHitMatch::Class_Version(), "CbmMvdHitMatch.h", 22,
                  typeid(::CbmMvdHitMatch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMvdHitMatch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMvdHitMatch) );
      instance.SetNew(&new_CbmMvdHitMatch);
      instance.SetNewArray(&newArray_CbmMvdHitMatch);
      instance.SetDelete(&delete_CbmMvdHitMatch);
      instance.SetDeleteArray(&deleteArray_CbmMvdHitMatch);
      instance.SetDestructor(&destruct_CbmMvdHitMatch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMvdHitMatch*)
   {
      return GenerateInitInstanceLocal((::CbmMvdHitMatch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMvdHitMatch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsCluster(void *p = nullptr);
   static void *newArray_CbmStsCluster(Long_t size, void *p);
   static void delete_CbmStsCluster(void *p);
   static void deleteArray_CbmStsCluster(void *p);
   static void destruct_CbmStsCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsCluster*)
   {
      ::CbmStsCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsCluster", ::CbmStsCluster::Class_Version(), "CbmStsCluster.h", 35,
                  typeid(::CbmStsCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsCluster) );
      instance.SetNew(&new_CbmStsCluster);
      instance.SetNewArray(&newArray_CbmStsCluster);
      instance.SetDelete(&delete_CbmStsCluster);
      instance.SetDeleteArray(&deleteArray_CbmStsCluster);
      instance.SetDestructor(&destruct_CbmStsCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsCluster*)
   {
      return GenerateInitInstanceLocal((::CbmStsCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsHit(void *p = nullptr);
   static void *newArray_CbmStsHit(Long_t size, void *p);
   static void delete_CbmStsHit(void *p);
   static void deleteArray_CbmStsHit(void *p);
   static void destruct_CbmStsHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsHit*)
   {
      ::CbmStsHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsHit", ::CbmStsHit::Class_Version(), "CbmStsHit.h", 35,
                  typeid(::CbmStsHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsHit) );
      instance.SetNew(&new_CbmStsHit);
      instance.SetNewArray(&newArray_CbmStsHit);
      instance.SetDelete(&delete_CbmStsHit);
      instance.SetDeleteArray(&deleteArray_CbmStsHit);
      instance.SetDestructor(&destruct_CbmStsHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsHit*)
   {
      return GenerateInitInstanceLocal((::CbmStsHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsPoint(void *p = nullptr);
   static void *newArray_CbmStsPoint(Long_t size, void *p);
   static void delete_CbmStsPoint(void *p);
   static void deleteArray_CbmStsPoint(void *p);
   static void destruct_CbmStsPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsPoint*)
   {
      ::CbmStsPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsPoint", ::CbmStsPoint::Class_Version(), "CbmStsPoint.h", 32,
                  typeid(::CbmStsPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsPoint) );
      instance.SetNew(&new_CbmStsPoint);
      instance.SetNewArray(&newArray_CbmStsPoint);
      instance.SetDelete(&delete_CbmStsPoint);
      instance.SetDeleteArray(&deleteArray_CbmStsPoint);
      instance.SetDestructor(&destruct_CbmStsPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsPoint*)
   {
      return GenerateInitInstanceLocal((::CbmStsPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmStsTrack(void *p = nullptr);
   static void *newArray_CbmStsTrack(Long_t size, void *p);
   static void delete_CbmStsTrack(void *p);
   static void deleteArray_CbmStsTrack(void *p);
   static void destruct_CbmStsTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmStsTrack*)
   {
      ::CbmStsTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmStsTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmStsTrack", ::CbmStsTrack::Class_Version(), "CbmStsTrack.h", 42,
                  typeid(::CbmStsTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmStsTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmStsTrack) );
      instance.SetNew(&new_CbmStsTrack);
      instance.SetNewArray(&newArray_CbmStsTrack);
      instance.SetDelete(&delete_CbmStsTrack);
      instance.SetDeleteArray(&deleteArray_CbmStsTrack);
      instance.SetDestructor(&destruct_CbmStsTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmStsTrack*)
   {
      return GenerateInitInstanceLocal((::CbmStsTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmStsTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichRing(void *p = nullptr);
   static void *newArray_CbmRichRing(Long_t size, void *p);
   static void delete_CbmRichRing(void *p);
   static void deleteArray_CbmRichRing(void *p);
   static void destruct_CbmRichRing(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichRing*)
   {
      ::CbmRichRing *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichRing >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichRing", ::CbmRichRing::Class_Version(), "CbmRichRing.h", 14,
                  typeid(::CbmRichRing), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichRing::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichRing) );
      instance.SetNew(&new_CbmRichRing);
      instance.SetNewArray(&newArray_CbmRichRing);
      instance.SetDelete(&delete_CbmRichRing);
      instance.SetDeleteArray(&deleteArray_CbmRichRing);
      instance.SetDestructor(&destruct_CbmRichRing);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichRing*)
   {
      return GenerateInitInstanceLocal((::CbmRichRing*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichRing*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichHit(void *p = nullptr);
   static void *newArray_CbmRichHit(Long_t size, void *p);
   static void delete_CbmRichHit(void *p);
   static void deleteArray_CbmRichHit(void *p);
   static void destruct_CbmRichHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichHit*)
   {
      ::CbmRichHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichHit", ::CbmRichHit::Class_Version(), "CbmRichHit.h", 23,
                  typeid(::CbmRichHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichHit) );
      instance.SetNew(&new_CbmRichHit);
      instance.SetNewArray(&newArray_CbmRichHit);
      instance.SetDelete(&delete_CbmRichHit);
      instance.SetDeleteArray(&deleteArray_CbmRichHit);
      instance.SetDestructor(&destruct_CbmRichHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichHit*)
   {
      return GenerateInitInstanceLocal((::CbmRichHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichPoint(void *p = nullptr);
   static void *newArray_CbmRichPoint(Long_t size, void *p);
   static void delete_CbmRichPoint(void *p);
   static void deleteArray_CbmRichPoint(void *p);
   static void destruct_CbmRichPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichPoint*)
   {
      ::CbmRichPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichPoint", ::CbmRichPoint::Class_Version(), "CbmRichPoint.h", 29,
                  typeid(::CbmRichPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichPoint) );
      instance.SetNew(&new_CbmRichPoint);
      instance.SetNewArray(&newArray_CbmRichPoint);
      instance.SetDelete(&delete_CbmRichPoint);
      instance.SetDeleteArray(&deleteArray_CbmRichPoint);
      instance.SetDestructor(&destruct_CbmRichPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichPoint*)
   {
      return GenerateInitInstanceLocal((::CbmRichPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmRichTrbDigi(void *p = nullptr);
   static void *newArray_CbmRichTrbDigi(Long_t size, void *p);
   static void delete_CbmRichTrbDigi(void *p);
   static void deleteArray_CbmRichTrbDigi(void *p);
   static void destruct_CbmRichTrbDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmRichTrbDigi*)
   {
      ::CbmRichTrbDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmRichTrbDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmRichTrbDigi", ::CbmRichTrbDigi::Class_Version(), "CbmRichTrbDigi.h", 13,
                  typeid(::CbmRichTrbDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmRichTrbDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmRichTrbDigi) );
      instance.SetNew(&new_CbmRichTrbDigi);
      instance.SetNewArray(&newArray_CbmRichTrbDigi);
      instance.SetDelete(&delete_CbmRichTrbDigi);
      instance.SetDeleteArray(&deleteArray_CbmRichTrbDigi);
      instance.SetDestructor(&destruct_CbmRichTrbDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmRichTrbDigi*)
   {
      return GenerateInitInstanceLocal((::CbmRichTrbDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmRichTrbDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchPixelHit(void *p = nullptr);
   static void *newArray_CbmMuchPixelHit(Long_t size, void *p);
   static void delete_CbmMuchPixelHit(void *p);
   static void deleteArray_CbmMuchPixelHit(void *p);
   static void destruct_CbmMuchPixelHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchPixelHit*)
   {
      ::CbmMuchPixelHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchPixelHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchPixelHit", ::CbmMuchPixelHit::Class_Version(), "CbmMuchPixelHit.h", 22,
                  typeid(::CbmMuchPixelHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchPixelHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchPixelHit) );
      instance.SetNew(&new_CbmMuchPixelHit);
      instance.SetNewArray(&newArray_CbmMuchPixelHit);
      instance.SetDelete(&delete_CbmMuchPixelHit);
      instance.SetDeleteArray(&deleteArray_CbmMuchPixelHit);
      instance.SetDestructor(&destruct_CbmMuchPixelHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchPixelHit*)
   {
      return GenerateInitInstanceLocal((::CbmMuchPixelHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchPixelHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchPoint(void *p = nullptr);
   static void *newArray_CbmMuchPoint(Long_t size, void *p);
   static void delete_CbmMuchPoint(void *p);
   static void deleteArray_CbmMuchPoint(void *p);
   static void destruct_CbmMuchPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchPoint*)
   {
      ::CbmMuchPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchPoint", ::CbmMuchPoint::Class_Version(), "CbmMuchPoint.h", 27,
                  typeid(::CbmMuchPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchPoint) );
      instance.SetNew(&new_CbmMuchPoint);
      instance.SetNewArray(&newArray_CbmMuchPoint);
      instance.SetDelete(&delete_CbmMuchPoint);
      instance.SetDeleteArray(&deleteArray_CbmMuchPoint);
      instance.SetDestructor(&destruct_CbmMuchPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchPoint*)
   {
      return GenerateInitInstanceLocal((::CbmMuchPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchCluster(void *p = nullptr);
   static void *newArray_CbmMuchCluster(Long_t size, void *p);
   static void delete_CbmMuchCluster(void *p);
   static void deleteArray_CbmMuchCluster(void *p);
   static void destruct_CbmMuchCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchCluster*)
   {
      ::CbmMuchCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchCluster", ::CbmMuchCluster::Class_Version(), "CbmMuchCluster.h", 24,
                  typeid(::CbmMuchCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchCluster) );
      instance.SetNew(&new_CbmMuchCluster);
      instance.SetNewArray(&newArray_CbmMuchCluster);
      instance.SetDelete(&delete_CbmMuchCluster);
      instance.SetDeleteArray(&deleteArray_CbmMuchCluster);
      instance.SetDestructor(&destruct_CbmMuchCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchCluster*)
   {
      return GenerateInitInstanceLocal((::CbmMuchCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchBeamTimeDigi(void *p = nullptr);
   static void *newArray_CbmMuchBeamTimeDigi(Long_t size, void *p);
   static void delete_CbmMuchBeamTimeDigi(void *p);
   static void deleteArray_CbmMuchBeamTimeDigi(void *p);
   static void destruct_CbmMuchBeamTimeDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchBeamTimeDigi*)
   {
      ::CbmMuchBeamTimeDigi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchBeamTimeDigi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchBeamTimeDigi", ::CbmMuchBeamTimeDigi::Class_Version(), "CbmMuchBeamTimeDigi.h", 33,
                  typeid(::CbmMuchBeamTimeDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchBeamTimeDigi::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchBeamTimeDigi) );
      instance.SetNew(&new_CbmMuchBeamTimeDigi);
      instance.SetNewArray(&newArray_CbmMuchBeamTimeDigi);
      instance.SetDelete(&delete_CbmMuchBeamTimeDigi);
      instance.SetDeleteArray(&deleteArray_CbmMuchBeamTimeDigi);
      instance.SetDestructor(&destruct_CbmMuchBeamTimeDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchBeamTimeDigi*)
   {
      return GenerateInitInstanceLocal((::CbmMuchBeamTimeDigi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchBeamTimeDigi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchDigiMatch(void *p = nullptr);
   static void *newArray_CbmMuchDigiMatch(Long_t size, void *p);
   static void delete_CbmMuchDigiMatch(void *p);
   static void deleteArray_CbmMuchDigiMatch(void *p);
   static void destruct_CbmMuchDigiMatch(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchDigiMatch*)
   {
      ::CbmMuchDigiMatch *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchDigiMatch >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchDigiMatch", ::CbmMuchDigiMatch::Class_Version(), "CbmMuchDigiMatch.h", 21,
                  typeid(::CbmMuchDigiMatch), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchDigiMatch::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchDigiMatch) );
      instance.SetNew(&new_CbmMuchDigiMatch);
      instance.SetNewArray(&newArray_CbmMuchDigiMatch);
      instance.SetDelete(&delete_CbmMuchDigiMatch);
      instance.SetDeleteArray(&deleteArray_CbmMuchDigiMatch);
      instance.SetDestructor(&destruct_CbmMuchDigiMatch);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchDigiMatch*)
   {
      return GenerateInitInstanceLocal((::CbmMuchDigiMatch*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchDigiMatch*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmMuchTrack(void *p = nullptr);
   static void *newArray_CbmMuchTrack(Long_t size, void *p);
   static void delete_CbmMuchTrack(void *p);
   static void deleteArray_CbmMuchTrack(void *p);
   static void destruct_CbmMuchTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmMuchTrack*)
   {
      ::CbmMuchTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmMuchTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmMuchTrack", ::CbmMuchTrack::Class_Version(), "CbmMuchTrack.h", 20,
                  typeid(::CbmMuchTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmMuchTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmMuchTrack) );
      instance.SetNew(&new_CbmMuchTrack);
      instance.SetNewArray(&newArray_CbmMuchTrack);
      instance.SetDelete(&delete_CbmMuchTrack);
      instance.SetDeleteArray(&deleteArray_CbmMuchTrack);
      instance.SetDestructor(&destruct_CbmMuchTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmMuchTrack*)
   {
      return GenerateInitInstanceLocal((::CbmMuchTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmMuchTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmTrdAddress_Dictionary();
   static void CbmTrdAddress_TClassManip(TClass*);
   static void *new_CbmTrdAddress(void *p = nullptr);
   static void *newArray_CbmTrdAddress(Long_t size, void *p);
   static void delete_CbmTrdAddress(void *p);
   static void deleteArray_CbmTrdAddress(void *p);
   static void destruct_CbmTrdAddress(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdAddress*)
   {
      ::CbmTrdAddress *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmTrdAddress));
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdAddress", "CbmTrdAddress.h", 33,
                  typeid(::CbmTrdAddress), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmTrdAddress_Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdAddress) );
      instance.SetNew(&new_CbmTrdAddress);
      instance.SetNewArray(&newArray_CbmTrdAddress);
      instance.SetDelete(&delete_CbmTrdAddress);
      instance.SetDeleteArray(&deleteArray_CbmTrdAddress);
      instance.SetDestructor(&destruct_CbmTrdAddress);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdAddress*)
   {
      return GenerateInitInstanceLocal((::CbmTrdAddress*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdAddress*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmTrdAddress_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmTrdAddress*)nullptr)->GetClass();
      CbmTrdAddress_TClassManip(theClass);
   return theClass;
   }

   static void CbmTrdAddress_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdHit(void *p = nullptr);
   static void *newArray_CbmTrdHit(Long_t size, void *p);
   static void delete_CbmTrdHit(void *p);
   static void deleteArray_CbmTrdHit(void *p);
   static void destruct_CbmTrdHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdHit*)
   {
      ::CbmTrdHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdHit", ::CbmTrdHit::Class_Version(), "CbmTrdHit.h", 40,
                  typeid(::CbmTrdHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdHit) );
      instance.SetNew(&new_CbmTrdHit);
      instance.SetNewArray(&newArray_CbmTrdHit);
      instance.SetDelete(&delete_CbmTrdHit);
      instance.SetDeleteArray(&deleteArray_CbmTrdHit);
      instance.SetDestructor(&destruct_CbmTrdHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdHit*)
   {
      return GenerateInitInstanceLocal((::CbmTrdHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdTrack(void *p = nullptr);
   static void *newArray_CbmTrdTrack(Long_t size, void *p);
   static void delete_CbmTrdTrack(void *p);
   static void deleteArray_CbmTrdTrack(void *p);
   static void destruct_CbmTrdTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdTrack*)
   {
      ::CbmTrdTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdTrack", ::CbmTrdTrack::Class_Version(), "CbmTrdTrack.h", 25,
                  typeid(::CbmTrdTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdTrack) );
      instance.SetNew(&new_CbmTrdTrack);
      instance.SetNewArray(&newArray_CbmTrdTrack);
      instance.SetDelete(&delete_CbmTrdTrack);
      instance.SetDeleteArray(&deleteArray_CbmTrdTrack);
      instance.SetDestructor(&destruct_CbmTrdTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdTrack*)
   {
      return GenerateInitInstanceLocal((::CbmTrdTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdPoint(void *p = nullptr);
   static void *newArray_CbmTrdPoint(Long_t size, void *p);
   static void delete_CbmTrdPoint(void *p);
   static void deleteArray_CbmTrdPoint(void *p);
   static void destruct_CbmTrdPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdPoint*)
   {
      ::CbmTrdPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdPoint", ::CbmTrdPoint::Class_Version(), "CbmTrdPoint.h", 30,
                  typeid(::CbmTrdPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdPoint) );
      instance.SetNew(&new_CbmTrdPoint);
      instance.SetNewArray(&newArray_CbmTrdPoint);
      instance.SetDelete(&delete_CbmTrdPoint);
      instance.SetDeleteArray(&deleteArray_CbmTrdPoint);
      instance.SetDestructor(&destruct_CbmTrdPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdPoint*)
   {
      return GenerateInitInstanceLocal((::CbmTrdPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrdCluster(void *p = nullptr);
   static void *newArray_CbmTrdCluster(Long_t size, void *p);
   static void delete_CbmTrdCluster(void *p);
   static void deleteArray_CbmTrdCluster(void *p);
   static void destruct_CbmTrdCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdCluster*)
   {
      ::CbmTrdCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrdCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdCluster", ::CbmTrdCluster::Class_Version(), "CbmTrdCluster.h", 27,
                  typeid(::CbmTrdCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrdCluster::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdCluster) );
      instance.SetNew(&new_CbmTrdCluster);
      instance.SetNewArray(&newArray_CbmTrdCluster);
      instance.SetDelete(&delete_CbmTrdCluster);
      instance.SetDeleteArray(&deleteArray_CbmTrdCluster);
      instance.SetDestructor(&destruct_CbmTrdCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdCluster*)
   {
      return GenerateInitInstanceLocal((::CbmTrdCluster*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdCluster*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CbmTrdRawMessageSpadic_Dictionary();
   static void CbmTrdRawMessageSpadic_TClassManip(TClass*);
   static void *new_CbmTrdRawMessageSpadic(void *p = nullptr);
   static void *newArray_CbmTrdRawMessageSpadic(Long_t size, void *p);
   static void delete_CbmTrdRawMessageSpadic(void *p);
   static void deleteArray_CbmTrdRawMessageSpadic(void *p);
   static void destruct_CbmTrdRawMessageSpadic(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrdRawMessageSpadic*)
   {
      ::CbmTrdRawMessageSpadic *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CbmTrdRawMessageSpadic));
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrdRawMessageSpadic", "CbmTrdRawMessageSpadic.h", 73,
                  typeid(::CbmTrdRawMessageSpadic), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &CbmTrdRawMessageSpadic_Dictionary, isa_proxy, 4,
                  sizeof(::CbmTrdRawMessageSpadic) );
      instance.SetNew(&new_CbmTrdRawMessageSpadic);
      instance.SetNewArray(&newArray_CbmTrdRawMessageSpadic);
      instance.SetDelete(&delete_CbmTrdRawMessageSpadic);
      instance.SetDeleteArray(&deleteArray_CbmTrdRawMessageSpadic);
      instance.SetDestructor(&destruct_CbmTrdRawMessageSpadic);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrdRawMessageSpadic*)
   {
      return GenerateInitInstanceLocal((::CbmTrdRawMessageSpadic*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrdRawMessageSpadic*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CbmTrdRawMessageSpadic_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CbmTrdRawMessageSpadic*)nullptr)->GetClass();
      CbmTrdRawMessageSpadic_TClassManip(theClass);
   return theClass;
   }

   static void CbmTrdRawMessageSpadic_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofHit(void *p = nullptr);
   static void *newArray_CbmTofHit(Long_t size, void *p);
   static void delete_CbmTofHit(void *p);
   static void deleteArray_CbmTofHit(void *p);
   static void destruct_CbmTofHit(void *p);

   // Schema evolution read functions
   static void read_CbmTofHit_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
      CbmTofHit* newObj = (CbmTofHit*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      \
           LOG(error); \
           LOG(error) << "You are trying to read an outdated version of CbmTofHit"; \
           LOG(error) << "where the unique tof address can't be converted"; \
           LOG(error) << "automatically to the new tof addressing scheme."; \
           LOG(error); \
           LOG(fatal) << "Stop execution."; \
         
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofHit*)
   {
      ::CbmTofHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofHit", ::CbmTofHit::Class_Version(), "CbmTofHit.h", 31,
                  typeid(::CbmTofHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofHit::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofHit) );
      instance.SetNew(&new_CbmTofHit);
      instance.SetNewArray(&newArray_CbmTofHit);
      instance.SetDelete(&delete_CbmTofHit);
      instance.SetDeleteArray(&deleteArray_CbmTofHit);
      instance.SetDestructor(&destruct_CbmTofHit);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "CbmTofHit";
      rule->fTarget      = "";
      rule->fSource      = "";
      rule->fFunctionPtr = (void *)TFunc2void( read_CbmTofHit_0);
      rule->fCode        = " \\n           LOG(error); \\n           LOG(error) << \"You are trying to read an outdated version of CbmTofHit\"; \\n           LOG(error) << \"where the unique tof address can't be converted\"; \\n           LOG(error) << \"automatically to the new tof addressing scheme.\"; \\n           LOG(error); \\n           LOG(fatal) << \"Stop execution.\"; \\n         ";
      rule->fVersion     = "[1-4]";
      rule->fInclude     = "Logger.h";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofHit*)
   {
      return GenerateInitInstanceLocal((::CbmTofHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofPoint(void *p = nullptr);
   static void *newArray_CbmTofPoint(Long_t size, void *p);
   static void delete_CbmTofPoint(void *p);
   static void deleteArray_CbmTofPoint(void *p);
   static void destruct_CbmTofPoint(void *p);

   // Schema evolution read functions
   static void read_CbmTofPoint_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
      CbmTofPoint* newObj = (CbmTofPoint*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      \
           LOG(error); \
           LOG(error) << "You are trying to read an outdated version of CbmTofPoint"; \
           LOG(error) << "where the unique tof address can't be converted"; \
           LOG(error) << "automatically to the new tof addressing scheme."; \
           LOG(error); \
           LOG(fatal) << "Stop execution."; \
         
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofPoint*)
   {
      ::CbmTofPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofPoint", ::CbmTofPoint::Class_Version(), "CbmTofPoint.h", 44,
                  typeid(::CbmTofPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofPoint) );
      instance.SetNew(&new_CbmTofPoint);
      instance.SetNewArray(&newArray_CbmTofPoint);
      instance.SetDelete(&delete_CbmTofPoint);
      instance.SetDeleteArray(&deleteArray_CbmTofPoint);
      instance.SetDestructor(&destruct_CbmTofPoint);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "CbmTofPoint";
      rule->fTarget      = "";
      rule->fSource      = "";
      rule->fFunctionPtr = (void *)TFunc2void( read_CbmTofPoint_0);
      rule->fCode        = " \\n           LOG(error); \\n           LOG(error) << \"You are trying to read an outdated version of CbmTofPoint\"; \\n           LOG(error) << \"where the unique tof address can't be converted\"; \\n           LOG(error) << \"automatically to the new tof addressing scheme.\"; \\n           LOG(error); \\n           LOG(fatal) << \"Stop execution.\"; \\n         ";
      rule->fVersion     = "[1-3]";
      rule->fInclude     = "Logger.h";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofPoint*)
   {
      return GenerateInitInstanceLocal((::CbmTofPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTrack(void *p = nullptr);
   static void *newArray_CbmTofTrack(Long_t size, void *p);
   static void delete_CbmTofTrack(void *p);
   static void deleteArray_CbmTofTrack(void *p);
   static void destruct_CbmTofTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrack*)
   {
      ::CbmTofTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrack", ::CbmTofTrack::Class_Version(), "CbmTofTrack.h", 33,
                  typeid(::CbmTofTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrack) );
      instance.SetNew(&new_CbmTofTrack);
      instance.SetNewArray(&newArray_CbmTofTrack);
      instance.SetDelete(&delete_CbmTofTrack);
      instance.SetDeleteArray(&deleteArray_CbmTofTrack);
      instance.SetDestructor(&destruct_CbmTofTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrack*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTrackletParam(void *p = nullptr);
   static void *newArray_CbmTofTrackletParam(Long_t size, void *p);
   static void delete_CbmTofTrackletParam(void *p);
   static void deleteArray_CbmTofTrackletParam(void *p);
   static void destruct_CbmTofTrackletParam(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTrackletParam*)
   {
      ::CbmTofTrackletParam *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTrackletParam >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTrackletParam", ::CbmTofTrackletParam::Class_Version(), "CbmTofTrackletParam.h", 32,
                  typeid(::CbmTofTrackletParam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTrackletParam::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTrackletParam) );
      instance.SetNew(&new_CbmTofTrackletParam);
      instance.SetNewArray(&newArray_CbmTofTrackletParam);
      instance.SetDelete(&delete_CbmTofTrackletParam);
      instance.SetDeleteArray(&deleteArray_CbmTofTrackletParam);
      instance.SetDestructor(&destruct_CbmTofTrackletParam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTrackletParam*)
   {
      return GenerateInitInstanceLocal((::CbmTofTrackletParam*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTrackletParam*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTofTracklet(void *p = nullptr);
   static void *newArray_CbmTofTracklet(Long_t size, void *p);
   static void delete_CbmTofTracklet(void *p);
   static void deleteArray_CbmTofTracklet(void *p);
   static void destruct_CbmTofTracklet(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTofTracklet*)
   {
      ::CbmTofTracklet *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTofTracklet >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTofTracklet", ::CbmTofTracklet::Class_Version(), "CbmTofTracklet.h", 30,
                  typeid(::CbmTofTracklet), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTofTracklet::Dictionary, isa_proxy, 4,
                  sizeof(::CbmTofTracklet) );
      instance.SetNew(&new_CbmTofTracklet);
      instance.SetNewArray(&newArray_CbmTofTracklet);
      instance.SetDelete(&delete_CbmTofTracklet);
      instance.SetDeleteArray(&deleteArray_CbmTofTracklet);
      instance.SetDestructor(&destruct_CbmTofTracklet);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTofTracklet*)
   {
      return GenerateInitInstanceLocal((::CbmTofTracklet*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTofTracklet*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdDsp(void *p = nullptr);
   static void *newArray_CbmPsdDsp(Long_t size, void *p);
   static void delete_CbmPsdDsp(void *p);
   static void deleteArray_CbmPsdDsp(void *p);
   static void destruct_CbmPsdDsp(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdDsp*)
   {
      ::CbmPsdDsp *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdDsp >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdDsp", ::CbmPsdDsp::Class_Version(), "CbmPsdDsp.h", 30,
                  typeid(::CbmPsdDsp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdDsp::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdDsp) );
      instance.SetNew(&new_CbmPsdDsp);
      instance.SetNewArray(&newArray_CbmPsdDsp);
      instance.SetDelete(&delete_CbmPsdDsp);
      instance.SetDeleteArray(&deleteArray_CbmPsdDsp);
      instance.SetDestructor(&destruct_CbmPsdDsp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdDsp*)
   {
      return GenerateInitInstanceLocal((::CbmPsdDsp*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdDsp*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdHit(void *p = nullptr);
   static void *newArray_CbmPsdHit(Long_t size, void *p);
   static void delete_CbmPsdHit(void *p);
   static void deleteArray_CbmPsdHit(void *p);
   static void destruct_CbmPsdHit(void *p);
   static void streamer_CbmPsdHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdHit*)
   {
      ::CbmPsdHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdHit", ::CbmPsdHit::Class_Version(), "CbmPsdHit.h", 25,
                  typeid(::CbmPsdHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdHit::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPsdHit) );
      instance.SetNew(&new_CbmPsdHit);
      instance.SetNewArray(&newArray_CbmPsdHit);
      instance.SetDelete(&delete_CbmPsdHit);
      instance.SetDeleteArray(&deleteArray_CbmPsdHit);
      instance.SetDestructor(&destruct_CbmPsdHit);
      instance.SetStreamerFunc(&streamer_CbmPsdHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdHit*)
   {
      return GenerateInitInstanceLocal((::CbmPsdHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdPoint(void *p = nullptr);
   static void *newArray_CbmPsdPoint(Long_t size, void *p);
   static void delete_CbmPsdPoint(void *p);
   static void deleteArray_CbmPsdPoint(void *p);
   static void destruct_CbmPsdPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdPoint*)
   {
      ::CbmPsdPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdPoint", ::CbmPsdPoint::Class_Version(), "CbmPsdPoint.h", 28,
                  typeid(::CbmPsdPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdPoint::Dictionary, isa_proxy, 4,
                  sizeof(::CbmPsdPoint) );
      instance.SetNew(&new_CbmPsdPoint);
      instance.SetNewArray(&newArray_CbmPsdPoint);
      instance.SetDelete(&delete_CbmPsdPoint);
      instance.SetDeleteArray(&deleteArray_CbmPsdPoint);
      instance.SetDestructor(&destruct_CbmPsdPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdPoint*)
   {
      return GenerateInitInstanceLocal((::CbmPsdPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmPsdMCbmHit(void *p = nullptr);
   static void *newArray_CbmPsdMCbmHit(Long_t size, void *p);
   static void delete_CbmPsdMCbmHit(void *p);
   static void deleteArray_CbmPsdMCbmHit(void *p);
   static void destruct_CbmPsdMCbmHit(void *p);
   static void streamer_CbmPsdMCbmHit(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmPsdMCbmHit*)
   {
      ::CbmPsdMCbmHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmPsdMCbmHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmPsdMCbmHit", ::CbmPsdMCbmHit::Class_Version(), "CbmPsdMCbmHit.h", 28,
                  typeid(::CbmPsdMCbmHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmPsdMCbmHit::Dictionary, isa_proxy, 16,
                  sizeof(::CbmPsdMCbmHit) );
      instance.SetNew(&new_CbmPsdMCbmHit);
      instance.SetNewArray(&newArray_CbmPsdMCbmHit);
      instance.SetDelete(&delete_CbmPsdMCbmHit);
      instance.SetDeleteArray(&deleteArray_CbmPsdMCbmHit);
      instance.SetDestructor(&destruct_CbmPsdMCbmHit);
      instance.SetStreamerFunc(&streamer_CbmPsdMCbmHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmPsdMCbmHit*)
   {
      return GenerateInitInstanceLocal((::CbmPsdMCbmHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmPsdMCbmHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmTrackParam(void *p = nullptr);
   static void *newArray_CbmTrackParam(Long_t size, void *p);
   static void delete_CbmTrackParam(void *p);
   static void deleteArray_CbmTrackParam(void *p);
   static void destruct_CbmTrackParam(void *p);
   static void streamer_CbmTrackParam(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmTrackParam*)
   {
      ::CbmTrackParam *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmTrackParam >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmTrackParam", ::CbmTrackParam::Class_Version(), "CbmTrackParam.h", 25,
                  typeid(::CbmTrackParam), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmTrackParam::Dictionary, isa_proxy, 16,
                  sizeof(::CbmTrackParam) );
      instance.SetNew(&new_CbmTrackParam);
      instance.SetNewArray(&newArray_CbmTrackParam);
      instance.SetDelete(&delete_CbmTrackParam);
      instance.SetDeleteArray(&deleteArray_CbmTrackParam);
      instance.SetDestructor(&destruct_CbmTrackParam);
      instance.SetStreamerFunc(&streamer_CbmTrackParam);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmTrackParam*)
   {
      return GenerateInitInstanceLocal((::CbmTrackParam*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmTrackParam*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CbmGlobalTrack(void *p = nullptr);
   static void *newArray_CbmGlobalTrack(Long_t size, void *p);
   static void delete_CbmGlobalTrack(void *p);
   static void deleteArray_CbmGlobalTrack(void *p);
   static void destruct_CbmGlobalTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CbmGlobalTrack*)
   {
      ::CbmGlobalTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CbmGlobalTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CbmGlobalTrack", ::CbmGlobalTrack::Class_Version(), "CbmGlobalTrack.h", 32,
                  typeid(::CbmGlobalTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CbmGlobalTrack::Dictionary, isa_proxy, 4,
                  sizeof(::CbmGlobalTrack) );
      instance.SetNew(&new_CbmGlobalTrack);
      instance.SetNewArray(&newArray_CbmGlobalTrack);
      instance.SetDelete(&delete_CbmGlobalTrack);
      instance.SetDeleteArray(&deleteArray_CbmGlobalTrack);
      instance.SetDestructor(&destruct_CbmGlobalTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CbmGlobalTrack*)
   {
      return GenerateInitInstanceLocal((::CbmGlobalTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CbmGlobalTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AccTimingEvent(void *p = nullptr);
   static void *newArray_AccTimingEvent(Long_t size, void *p);
   static void delete_AccTimingEvent(void *p);
   static void deleteArray_AccTimingEvent(void *p);
   static void destruct_AccTimingEvent(void *p);
   static void streamer_AccTimingEvent(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AccTimingEvent*)
   {
      ::AccTimingEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AccTimingEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AccTimingEvent", ::AccTimingEvent::Class_Version(), "AccDataSis18.h", 11,
                  typeid(::AccTimingEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AccTimingEvent::Dictionary, isa_proxy, 16,
                  sizeof(::AccTimingEvent) );
      instance.SetNew(&new_AccTimingEvent);
      instance.SetNewArray(&newArray_AccTimingEvent);
      instance.SetDelete(&delete_AccTimingEvent);
      instance.SetDeleteArray(&deleteArray_AccTimingEvent);
      instance.SetDestructor(&destruct_AccTimingEvent);
      instance.SetStreamerFunc(&streamer_AccTimingEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AccTimingEvent*)
   {
      return GenerateInitInstanceLocal((::AccTimingEvent*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AccTimingEvent*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AccStatusTs(void *p = nullptr);
   static void *newArray_AccStatusTs(Long_t size, void *p);
   static void delete_AccStatusTs(void *p);
   static void deleteArray_AccStatusTs(void *p);
   static void destruct_AccStatusTs(void *p);
   static void streamer_AccStatusTs(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AccStatusTs*)
   {
      ::AccStatusTs *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AccStatusTs >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("AccStatusTs", ::AccStatusTs::Class_Version(), "AccDataSis18.h", 90,
                  typeid(::AccStatusTs), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AccStatusTs::Dictionary, isa_proxy, 16,
                  sizeof(::AccStatusTs) );
      instance.SetNew(&new_AccStatusTs);
      instance.SetNewArray(&newArray_AccStatusTs);
      instance.SetDelete(&delete_AccStatusTs);
      instance.SetDeleteArray(&deleteArray_AccStatusTs);
      instance.SetDestructor(&destruct_AccStatusTs);
      instance.SetStreamerFunc(&streamer_AccStatusTs);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AccStatusTs*)
   {
      return GenerateInitInstanceLocal((::AccStatusTs*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::AccStatusTs*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *stsxytercLcLMessage_Dictionary();
   static void stsxytercLcLMessage_TClassManip(TClass*);
   static void *new_stsxytercLcLMessage(void *p = nullptr);
   static void *newArray_stsxytercLcLMessage(Long_t size, void *p);
   static void delete_stsxytercLcLMessage(void *p);
   static void deleteArray_stsxytercLcLMessage(void *p);
   static void destruct_stsxytercLcLMessage(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::stsxyter::Message*)
   {
      ::stsxyter::Message *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::stsxyter::Message));
      static ::ROOT::TGenericClassInfo 
         instance("stsxyter::Message", "StsXyterMessage.h", 171,
                  typeid(::stsxyter::Message), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &stsxytercLcLMessage_Dictionary, isa_proxy, 0,
                  sizeof(::stsxyter::Message) );
      instance.SetNew(&new_stsxytercLcLMessage);
      instance.SetNewArray(&newArray_stsxytercLcLMessage);
      instance.SetDelete(&delete_stsxytercLcLMessage);
      instance.SetDeleteArray(&deleteArray_stsxytercLcLMessage);
      instance.SetDestructor(&destruct_stsxytercLcLMessage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::stsxyter::Message*)
   {
      return GenerateInitInstanceLocal((::stsxyter::Message*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::stsxyter::Message*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *stsxytercLcLMessage_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::stsxyter::Message*)nullptr)->GetClass();
      stsxytercLcLMessage_TClassManip(theClass);
   return theClass;
   }

   static void stsxytercLcLMessage_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *gdpbv100cLcLMessage_Dictionary();
   static void gdpbv100cLcLMessage_TClassManip(TClass*);
   static void *new_gdpbv100cLcLMessage(void *p = nullptr);
   static void *newArray_gdpbv100cLcLMessage(Long_t size, void *p);
   static void delete_gdpbv100cLcLMessage(void *p);
   static void deleteArray_gdpbv100cLcLMessage(void *p);
   static void destruct_gdpbv100cLcLMessage(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::gdpbv100::Message*)
   {
      ::gdpbv100::Message *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::gdpbv100::Message));
      static ::ROOT::TGenericClassInfo 
         instance("gdpbv100::Message", "gDpbMessv100.h", 134,
                  typeid(::gdpbv100::Message), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &gdpbv100cLcLMessage_Dictionary, isa_proxy, 0,
                  sizeof(::gdpbv100::Message) );
      instance.SetNew(&new_gdpbv100cLcLMessage);
      instance.SetNewArray(&newArray_gdpbv100cLcLMessage);
      instance.SetDelete(&delete_gdpbv100cLcLMessage);
      instance.SetDeleteArray(&deleteArray_gdpbv100cLcLMessage);
      instance.SetDestructor(&destruct_gdpbv100cLcLMessage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::gdpbv100::Message*)
   {
      return GenerateInitInstanceLocal((::gdpbv100::Message*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::gdpbv100::Message*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *gdpbv100cLcLMessage_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::gdpbv100::Message*)nullptr)->GetClass();
      gdpbv100cLcLMessage_TClassManip(theClass);
   return theClass;
   }

   static void gdpbv100cLcLMessage_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *gdpbv100cLcLFullMessage_Dictionary();
   static void gdpbv100cLcLFullMessage_TClassManip(TClass*);
   static void *new_gdpbv100cLcLFullMessage(void *p = nullptr);
   static void *newArray_gdpbv100cLcLFullMessage(Long_t size, void *p);
   static void delete_gdpbv100cLcLFullMessage(void *p);
   static void deleteArray_gdpbv100cLcLFullMessage(void *p);
   static void destruct_gdpbv100cLcLFullMessage(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::gdpbv100::FullMessage*)
   {
      ::gdpbv100::FullMessage *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::gdpbv100::FullMessage));
      static ::ROOT::TGenericClassInfo 
         instance("gdpbv100::FullMessage", "gDpbMessv100.h", 343,
                  typeid(::gdpbv100::FullMessage), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &gdpbv100cLcLFullMessage_Dictionary, isa_proxy, 0,
                  sizeof(::gdpbv100::FullMessage) );
      instance.SetNew(&new_gdpbv100cLcLFullMessage);
      instance.SetNewArray(&newArray_gdpbv100cLcLFullMessage);
      instance.SetDelete(&delete_gdpbv100cLcLFullMessage);
      instance.SetDeleteArray(&deleteArray_gdpbv100cLcLFullMessage);
      instance.SetDestructor(&destruct_gdpbv100cLcLFullMessage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::gdpbv100::FullMessage*)
   {
      return GenerateInitInstanceLocal((::gdpbv100::FullMessage*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::gdpbv100::FullMessage*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *gdpbv100cLcLFullMessage_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::gdpbv100::FullMessage*)nullptr)->GetClass();
      gdpbv100cLcLFullMessage_TClassManip(theClass);
   return theClass;
   }

   static void gdpbv100cLcLFullMessage_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *critof001cLcLMessage_Dictionary();
   static void critof001cLcLMessage_TClassManip(TClass*);
   static void *new_critof001cLcLMessage(void *p = nullptr);
   static void *newArray_critof001cLcLMessage(Long_t size, void *p);
   static void delete_critof001cLcLMessage(void *p);
   static void deleteArray_critof001cLcLMessage(void *p);
   static void destruct_critof001cLcLMessage(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::critof001::Message*)
   {
      ::critof001::Message *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::critof001::Message));
      static ::ROOT::TGenericClassInfo 
         instance("critof001::Message", "CriGet4Mess001.h", 138,
                  typeid(::critof001::Message), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &critof001cLcLMessage_Dictionary, isa_proxy, 0,
                  sizeof(::critof001::Message) );
      instance.SetNew(&new_critof001cLcLMessage);
      instance.SetNewArray(&newArray_critof001cLcLMessage);
      instance.SetDelete(&delete_critof001cLcLMessage);
      instance.SetDeleteArray(&deleteArray_critof001cLcLMessage);
      instance.SetDestructor(&destruct_critof001cLcLMessage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::critof001::Message*)
   {
      return GenerateInitInstanceLocal((::critof001::Message*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::critof001::Message*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *critof001cLcLMessage_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::critof001::Message*)nullptr)->GetClass();
      critof001cLcLMessage_TClassManip(theClass);
   return theClass;
   }

   static void critof001cLcLMessage_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *critof001cLcLFullMessage_Dictionary();
   static void critof001cLcLFullMessage_TClassManip(TClass*);
   static void *new_critof001cLcLFullMessage(void *p = nullptr);
   static void *newArray_critof001cLcLFullMessage(Long_t size, void *p);
   static void delete_critof001cLcLFullMessage(void *p);
   static void deleteArray_critof001cLcLFullMessage(void *p);
   static void destruct_critof001cLcLFullMessage(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::critof001::FullMessage*)
   {
      ::critof001::FullMessage *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::critof001::FullMessage));
      static ::ROOT::TGenericClassInfo 
         instance("critof001::FullMessage", "CriGet4Mess001.h", 306,
                  typeid(::critof001::FullMessage), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &critof001cLcLFullMessage_Dictionary, isa_proxy, 0,
                  sizeof(::critof001::FullMessage) );
      instance.SetNew(&new_critof001cLcLFullMessage);
      instance.SetNewArray(&newArray_critof001cLcLFullMessage);
      instance.SetDelete(&delete_critof001cLcLFullMessage);
      instance.SetDeleteArray(&deleteArray_critof001cLcLFullMessage);
      instance.SetDestructor(&destruct_critof001cLcLFullMessage);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::critof001::FullMessage*)
   {
      return GenerateInitInstanceLocal((::critof001::FullMessage*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::critof001::FullMessage*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *critof001cLcLFullMessage_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::critof001::FullMessage*)nullptr)->GetClass();
      critof001cLcLFullMessage_TClassManip(theClass);
   return theClass;
   }

   static void critof001cLcLFullMessage_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_TimesliceMetaData(void *p = nullptr);
   static void *newArray_TimesliceMetaData(Long_t size, void *p);
   static void delete_TimesliceMetaData(void *p);
   static void deleteArray_TimesliceMetaData(void *p);
   static void destruct_TimesliceMetaData(void *p);
   static void streamer_TimesliceMetaData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TimesliceMetaData*)
   {
      ::TimesliceMetaData *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TimesliceMetaData >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("TimesliceMetaData", ::TimesliceMetaData::Class_Version(), "TimesliceMetaData.h", 14,
                  typeid(::TimesliceMetaData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TimesliceMetaData::Dictionary, isa_proxy, 16,
                  sizeof(::TimesliceMetaData) );
      instance.SetNew(&new_TimesliceMetaData);
      instance.SetNewArray(&newArray_TimesliceMetaData);
      instance.SetDelete(&delete_TimesliceMetaData);
      instance.SetDeleteArray(&deleteArray_TimesliceMetaData);
      instance.SetDestructor(&destruct_TimesliceMetaData);
      instance.SetStreamerFunc(&streamer_TimesliceMetaData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TimesliceMetaData*)
   {
      return GenerateInitInstanceLocal((::TimesliceMetaData*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TimesliceMetaData*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *PsdDataV000cLcLPsdGbtReader_Dictionary();
   static void PsdDataV000cLcLPsdGbtReader_TClassManip(TClass*);
   static void *new_PsdDataV000cLcLPsdGbtReader(void *p = nullptr);
   static void *newArray_PsdDataV000cLcLPsdGbtReader(Long_t size, void *p);
   static void delete_PsdDataV000cLcLPsdGbtReader(void *p);
   static void deleteArray_PsdDataV000cLcLPsdGbtReader(void *p);
   static void destruct_PsdDataV000cLcLPsdGbtReader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PsdDataV000::PsdGbtReader*)
   {
      ::PsdDataV000::PsdGbtReader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PsdDataV000::PsdGbtReader));
      static ::ROOT::TGenericClassInfo 
         instance("PsdDataV000::PsdGbtReader", "PsdGbtReader-v0.00.h", 15,
                  typeid(::PsdDataV000::PsdGbtReader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PsdDataV000cLcLPsdGbtReader_Dictionary, isa_proxy, 0,
                  sizeof(::PsdDataV000::PsdGbtReader) );
      instance.SetNew(&new_PsdDataV000cLcLPsdGbtReader);
      instance.SetNewArray(&newArray_PsdDataV000cLcLPsdGbtReader);
      instance.SetDelete(&delete_PsdDataV000cLcLPsdGbtReader);
      instance.SetDeleteArray(&deleteArray_PsdDataV000cLcLPsdGbtReader);
      instance.SetDestructor(&destruct_PsdDataV000cLcLPsdGbtReader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PsdDataV000::PsdGbtReader*)
   {
      return GenerateInitInstanceLocal((::PsdDataV000::PsdGbtReader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PsdDataV000::PsdGbtReader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PsdDataV000cLcLPsdGbtReader_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PsdDataV000::PsdGbtReader*)nullptr)->GetClass();
      PsdDataV000cLcLPsdGbtReader_TClassManip(theClass);
   return theClass;
   }

   static void PsdDataV000cLcLPsdGbtReader_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PsdDataV100cLcLPsdGbtReader_Dictionary();
   static void PsdDataV100cLcLPsdGbtReader_TClassManip(TClass*);
   static void *new_PsdDataV100cLcLPsdGbtReader(void *p = nullptr);
   static void *newArray_PsdDataV100cLcLPsdGbtReader(Long_t size, void *p);
   static void delete_PsdDataV100cLcLPsdGbtReader(void *p);
   static void deleteArray_PsdDataV100cLcLPsdGbtReader(void *p);
   static void destruct_PsdDataV100cLcLPsdGbtReader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PsdDataV100::PsdGbtReader*)
   {
      ::PsdDataV100::PsdGbtReader *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PsdDataV100::PsdGbtReader));
      static ::ROOT::TGenericClassInfo 
         instance("PsdDataV100::PsdGbtReader", "PsdGbtReader-v1.00.h", 18,
                  typeid(::PsdDataV100::PsdGbtReader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PsdDataV100cLcLPsdGbtReader_Dictionary, isa_proxy, 0,
                  sizeof(::PsdDataV100::PsdGbtReader) );
      instance.SetNew(&new_PsdDataV100cLcLPsdGbtReader);
      instance.SetNewArray(&newArray_PsdDataV100cLcLPsdGbtReader);
      instance.SetDelete(&delete_PsdDataV100cLcLPsdGbtReader);
      instance.SetDeleteArray(&deleteArray_PsdDataV100cLcLPsdGbtReader);
      instance.SetDestructor(&destruct_PsdDataV100cLcLPsdGbtReader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PsdDataV100::PsdGbtReader*)
   {
      return GenerateInitInstanceLocal((::PsdDataV100::PsdGbtReader*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PsdDataV100::PsdGbtReader*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PsdDataV100cLcLPsdGbtReader_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PsdDataV100::PsdGbtReader*)nullptr)->GetClass();
      PsdDataV100cLcLPsdGbtReader_TClassManip(theClass);
   return theClass;
   }

   static void PsdDataV100cLcLPsdGbtReader_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CbmBmonDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBmonDigi::Class_Name()
{
   return "CbmBmonDigi";
}

//______________________________________________________________________________
const char *CbmBmonDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBmonDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBmonDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBmonDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmBmonDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmBmonDigiData::Class_Name()
{
   return "CbmBmonDigiData";
}

//______________________________________________________________________________
const char *CbmBmonDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmBmonDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmBmonDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmBmonDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmBmonDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchAddress::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchAddress::Class_Name()
{
   return "CbmMuchAddress";
}

//______________________________________________________________________________
const char *CbmMuchAddress::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchAddress*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchAddress::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchAddress*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchAddress::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchAddress*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchAddress::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchAddress*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchDigi::Class_Name()
{
   return "CbmMuchDigi";
}

//______________________________________________________________________________
const char *CbmMuchDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchDigiData::Class_Name()
{
   return "CbmMuchDigiData";
}

//______________________________________________________________________________
const char *CbmMuchDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdDigi::Class_Name()
{
   return "CbmPsdDigi";
}

//______________________________________________________________________________
const char *CbmPsdDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdDigiData::Class_Name()
{
   return "CbmPsdDigiData";
}

//______________________________________________________________________________
const char *CbmPsdDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichDigi::Class_Name()
{
   return "CbmRichDigi";
}

//______________________________________________________________________________
const char *CbmRichDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichDigiData::Class_Name()
{
   return "CbmRichDigiData";
}

//______________________________________________________________________________
const char *CbmRichDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsDigi::Class_Name()
{
   return "CbmStsDigi";
}

//______________________________________________________________________________
const char *CbmStsDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsDigiData::Class_Name()
{
   return "CbmStsDigiData";
}

//______________________________________________________________________________
const char *CbmStsDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofDigi::Class_Name()
{
   return "CbmTofDigi";
}

//______________________________________________________________________________
const char *CbmTofDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofDigiData::Class_Name()
{
   return "CbmTofDigiData";
}

//______________________________________________________________________________
const char *CbmTofDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdDigi::Class_Name()
{
   return "CbmTrdDigi";
}

//______________________________________________________________________________
const char *CbmTrdDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdDigiData::Class_Name()
{
   return "CbmTrdDigiData";
}

//______________________________________________________________________________
const char *CbmTrdDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigiData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigiData::Class_Name()
{
   return "CbmDigiData";
}

//______________________________________________________________________________
const char *CbmDigiData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigiData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigiData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigiData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigiEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigiEvent::Class_Name()
{
   return "CbmDigiEvent";
}

//______________________________________________________________________________
const char *CbmDigiEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigiEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigiEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigiEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmLink::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmLink::Class_Name()
{
   return "CbmLink";
}

//______________________________________________________________________________
const char *CbmLink::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLink*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmLink::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmLink*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmLink::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLink*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmLink::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmLink*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMatch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMatch::Class_Name()
{
   return "CbmMatch";
}

//______________________________________________________________________________
const char *CbmMatch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMatch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMatch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMatch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMatch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMatch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMatch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMatch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTimeSlice::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTimeSlice::Class_Name()
{
   return "CbmTimeSlice";
}

//______________________________________________________________________________
const char *CbmTimeSlice::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTimeSlice*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTimeSlice::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTimeSlice*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTimeSlice::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTimeSlice*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTimeSlice::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTimeSlice*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmVertex::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmVertex::Class_Name()
{
   return "CbmVertex";
}

//______________________________________________________________________________
const char *CbmVertex::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmVertex*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmVertex::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmVertex*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmVertex::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmVertex*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmVertex::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmVertex*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmEvent::Class_Name()
{
   return "CbmEvent";
}

//______________________________________________________________________________
const char *CbmEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmDigiContainer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmDigiContainer::Class_Name()
{
   return "CbmDigiContainer";
}

//______________________________________________________________________________
const char *CbmDigiContainer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiContainer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmDigiContainer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiContainer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmDigiContainer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiContainer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmDigiContainer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiContainer*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmPsdDsp>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmPsdDsp>::Class_Name()
{
   return "CbmDigiVector<CbmPsdDsp>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmPsdDsp>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDsp>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmPsdDsp>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDsp>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmPsdDsp>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDsp>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmPsdDsp>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDsp>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmPsdDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmPsdDigi>::Class_Name()
{
   return "CbmDigiVector<CbmPsdDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmPsdDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmPsdDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmPsdDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmPsdDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmPsdDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmTofDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmTofDigi>::Class_Name()
{
   return "CbmDigiVector<CbmTofDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmTofDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTofDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmTofDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTofDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmTofDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTofDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmTofDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTofDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmTrdDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmTrdDigi>::Class_Name()
{
   return "CbmDigiVector<CbmTrdDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmTrdDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTrdDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmTrdDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTrdDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmTrdDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTrdDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmTrdDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmTrdDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmMuchBeamTimeDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmMuchBeamTimeDigi>::Class_Name()
{
   return "CbmDigiVector<CbmMuchBeamTimeDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmMuchBeamTimeDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmMuchBeamTimeDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmMuchBeamTimeDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmMuchBeamTimeDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchBeamTimeDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmMuchDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmMuchDigi>::Class_Name()
{
   return "CbmDigiVector<CbmMuchDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmMuchDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmMuchDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmMuchDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmMuchDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMuchDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmRichDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmRichDigi>::Class_Name()
{
   return "CbmDigiVector<CbmRichDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmRichDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmRichDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmRichDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmRichDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmRichDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmRichDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmRichDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmRichDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmStsDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmStsDigi>::Class_Name()
{
   return "CbmDigiVector<CbmStsDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmStsDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmStsDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmStsDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmStsDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmStsDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmStsDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmStsDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmStsDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr CbmDigiVector<CbmMvdDigi>::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmMvdDigi>::Class_Name()
{
   return "CbmDigiVector<CbmMvdDigi>";
}

//______________________________________________________________________________
template <> const char *CbmDigiVector<CbmMvdDigi>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMvdDigi>*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
template <> int CbmDigiVector<CbmMvdDigi>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMvdDigi>*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmMvdDigi>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMvdDigi>*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *CbmDigiVector<CbmMvdDigi>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmDigiVector<CbmMvdDigi>*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmEventStore::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmEventStore::Class_Name()
{
   return "CbmEventStore";
}

//______________________________________________________________________________
const char *CbmEventStore::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEventStore*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmEventStore::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmEventStore*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmEventStore::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEventStore*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmEventStore::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmEventStore*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmHit::Class_Name()
{
   return "CbmHit";
}

//______________________________________________________________________________
const char *CbmHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPixelHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPixelHit::Class_Name()
{
   return "CbmPixelHit";
}

//______________________________________________________________________________
const char *CbmPixelHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPixelHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPixelHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPixelHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPixelHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStripHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStripHit::Class_Name()
{
   return "CbmStripHit";
}

//______________________________________________________________________________
const char *CbmStripHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStripHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStripHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStripHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStripHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStripHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStripHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStripHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrack::Class_Name()
{
   return "CbmTrack";
}

//______________________________________________________________________________
const char *CbmTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackMatch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackMatch::Class_Name()
{
   return "CbmTrackMatch";
}

//______________________________________________________________________________
const char *CbmTrackMatch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackMatch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackMatch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackMatch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmCluster::Class_Name()
{
   return "CbmCluster";
}

//______________________________________________________________________________
const char *CbmCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmErrorMessage::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmErrorMessage::Class_Name()
{
   return "CbmErrorMessage";
}

//______________________________________________________________________________
const char *CbmErrorMessage::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmErrorMessage*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmErrorMessage::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmErrorMessage*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmErrorMessage::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmErrorMessage*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmErrorMessage::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmErrorMessage*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCTrack::Class_Name()
{
   return "CbmMCTrack";
}

//______________________________________________________________________________
const char *CbmMCTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCEventInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCEventInfo::Class_Name()
{
   return "CbmMCEventInfo";
}

//______________________________________________________________________________
const char *CbmMCEventInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCEventInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCEventInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCEventInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMCEventList::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMCEventList::Class_Name()
{
   return "CbmMCEventList";
}

//______________________________________________________________________________
const char *CbmMCEventList::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventList*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMCEventList::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventList*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMCEventList::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventList*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMCEventList::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMCEventList*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackMatchNew::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackMatchNew::Class_Name()
{
   return "CbmTrackMatchNew";
}

//______________________________________________________________________________
const char *CbmTrackMatchNew::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatchNew*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackMatchNew::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatchNew*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackMatchNew::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatchNew*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackMatchNew::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackMatchNew*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTsEventHeader::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTsEventHeader::Class_Name()
{
   return "CbmTsEventHeader";
}

//______________________________________________________________________________
const char *CbmTsEventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTsEventHeader*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTsEventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTsEventHeader*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTsEventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTsEventHeader*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTsEventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTsEventHeader*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdDetectorId::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdDetectorId::Class_Name()
{
   return "CbmMvdDetectorId";
}

//______________________________________________________________________________
const char *CbmMvdDetectorId::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetectorId*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdDetectorId::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetectorId*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdDetectorId::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetectorId*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdDetectorId::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDetectorId*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdCluster::Class_Name()
{
   return "CbmMvdCluster";
}

//______________________________________________________________________________
const char *CbmMvdCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdDigi::Class_Name()
{
   return "CbmMvdDigi";
}

//______________________________________________________________________________
const char *CbmMvdDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdHit::Class_Name()
{
   return "CbmMvdHit";
}

//______________________________________________________________________________
const char *CbmMvdHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdPoint::Class_Name()
{
   return "CbmMvdPoint";
}

//______________________________________________________________________________
const char *CbmMvdPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMvdHitMatch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMvdHitMatch::Class_Name()
{
   return "CbmMvdHitMatch";
}

//______________________________________________________________________________
const char *CbmMvdHitMatch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitMatch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMvdHitMatch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitMatch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMvdHitMatch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitMatch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMvdHitMatch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMvdHitMatch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsCluster::Class_Name()
{
   return "CbmStsCluster";
}

//______________________________________________________________________________
const char *CbmStsCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsHit::Class_Name()
{
   return "CbmStsHit";
}

//______________________________________________________________________________
const char *CbmStsHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsPoint::Class_Name()
{
   return "CbmStsPoint";
}

//______________________________________________________________________________
const char *CbmStsPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmStsTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmStsTrack::Class_Name()
{
   return "CbmStsTrack";
}

//______________________________________________________________________________
const char *CbmStsTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmStsTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmStsTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmStsTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmStsTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichRing::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichRing::Class_Name()
{
   return "CbmRichRing";
}

//______________________________________________________________________________
const char *CbmRichRing::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRing*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichRing::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRing*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichRing::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRing*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichRing::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichRing*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichHit::Class_Name()
{
   return "CbmRichHit";
}

//______________________________________________________________________________
const char *CbmRichHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichPoint::Class_Name()
{
   return "CbmRichPoint";
}

//______________________________________________________________________________
const char *CbmRichPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmRichTrbDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmRichTrbDigi::Class_Name()
{
   return "CbmRichTrbDigi";
}

//______________________________________________________________________________
const char *CbmRichTrbDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrbDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmRichTrbDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrbDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmRichTrbDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrbDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmRichTrbDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmRichTrbDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchPixelHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchPixelHit::Class_Name()
{
   return "CbmMuchPixelHit";
}

//______________________________________________________________________________
const char *CbmMuchPixelHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPixelHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchPixelHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPixelHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchPixelHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPixelHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchPixelHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPixelHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchPoint::Class_Name()
{
   return "CbmMuchPoint";
}

//______________________________________________________________________________
const char *CbmMuchPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchCluster::Class_Name()
{
   return "CbmMuchCluster";
}

//______________________________________________________________________________
const char *CbmMuchCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchBeamTimeDigi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchBeamTimeDigi::Class_Name()
{
   return "CbmMuchBeamTimeDigi";
}

//______________________________________________________________________________
const char *CbmMuchBeamTimeDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchBeamTimeDigi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchBeamTimeDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchBeamTimeDigi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchBeamTimeDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchBeamTimeDigi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchBeamTimeDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchBeamTimeDigi*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchDigiMatch::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchDigiMatch::Class_Name()
{
   return "CbmMuchDigiMatch";
}

//______________________________________________________________________________
const char *CbmMuchDigiMatch::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiMatch*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchDigiMatch::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiMatch*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchDigiMatch::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiMatch*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchDigiMatch::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchDigiMatch*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmMuchTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmMuchTrack::Class_Name()
{
   return "CbmMuchTrack";
}

//______________________________________________________________________________
const char *CbmMuchTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmMuchTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmMuchTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmMuchTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmMuchTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdHit::Class_Name()
{
   return "CbmTrdHit";
}

//______________________________________________________________________________
const char *CbmTrdHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdTrack::Class_Name()
{
   return "CbmTrdTrack";
}

//______________________________________________________________________________
const char *CbmTrdTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdPoint::Class_Name()
{
   return "CbmTrdPoint";
}

//______________________________________________________________________________
const char *CbmTrdPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrdCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrdCluster::Class_Name()
{
   return "CbmTrdCluster";
}

//______________________________________________________________________________
const char *CbmTrdCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrdCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrdCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrdCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrdCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofHit::Class_Name()
{
   return "CbmTofHit";
}

//______________________________________________________________________________
const char *CbmTofHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofPoint::Class_Name()
{
   return "CbmTofPoint";
}

//______________________________________________________________________________
const char *CbmTofPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrack::Class_Name()
{
   return "CbmTofTrack";
}

//______________________________________________________________________________
const char *CbmTofTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTrackletParam::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTrackletParam::Class_Name()
{
   return "CbmTofTrackletParam";
}

//______________________________________________________________________________
const char *CbmTofTrackletParam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletParam*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTrackletParam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletParam*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTrackletParam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletParam*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTrackletParam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTrackletParam*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTofTracklet::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTofTracklet::Class_Name()
{
   return "CbmTofTracklet";
}

//______________________________________________________________________________
const char *CbmTofTracklet::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTracklet*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTofTracklet::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTracklet*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTofTracklet::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTracklet*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTofTracklet::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTofTracklet*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdDsp::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdDsp::Class_Name()
{
   return "CbmPsdDsp";
}

//______________________________________________________________________________
const char *CbmPsdDsp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDsp*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdDsp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDsp*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdDsp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDsp*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdDsp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdDsp*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdHit::Class_Name()
{
   return "CbmPsdHit";
}

//______________________________________________________________________________
const char *CbmPsdHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdPoint::Class_Name()
{
   return "CbmPsdPoint";
}

//______________________________________________________________________________
const char *CbmPsdPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmPsdMCbmHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmPsdMCbmHit::Class_Name()
{
   return "CbmPsdMCbmHit";
}

//______________________________________________________________________________
const char *CbmPsdMCbmHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmPsdMCbmHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmPsdMCbmHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmPsdMCbmHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmPsdMCbmHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmTrackParam::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmTrackParam::Class_Name()
{
   return "CbmTrackParam";
}

//______________________________________________________________________________
const char *CbmTrackParam::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackParam*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmTrackParam::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackParam*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmTrackParam::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackParam*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmTrackParam::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmTrackParam*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CbmGlobalTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CbmGlobalTrack::Class_Name()
{
   return "CbmGlobalTrack";
}

//______________________________________________________________________________
const char *CbmGlobalTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CbmGlobalTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CbmGlobalTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CbmGlobalTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CbmGlobalTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AccTimingEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AccTimingEvent::Class_Name()
{
   return "AccTimingEvent";
}

//______________________________________________________________________________
const char *AccTimingEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AccTimingEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AccTimingEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AccTimingEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AccTimingEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AccTimingEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AccTimingEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AccTimingEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AccStatusTs::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AccStatusTs::Class_Name()
{
   return "AccStatusTs";
}

//______________________________________________________________________________
const char *AccStatusTs::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AccStatusTs*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AccStatusTs::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AccStatusTs*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AccStatusTs::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AccStatusTs*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AccStatusTs::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AccStatusTs*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TimesliceMetaData::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TimesliceMetaData::Class_Name()
{
   return "TimesliceMetaData";
}

//______________________________________________________________________________
const char *TimesliceMetaData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimesliceMetaData*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TimesliceMetaData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TimesliceMetaData*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TimesliceMetaData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimesliceMetaData*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TimesliceMetaData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TimesliceMetaData*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CbmBmonDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBmonDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBmonDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBmonDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBmonDigi(void *p) {
      return  p ? new(p) ::CbmBmonDigi : new ::CbmBmonDigi;
   }
   static void *newArray_CbmBmonDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBmonDigi[nElements] : new ::CbmBmonDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBmonDigi(void *p) {
      delete ((::CbmBmonDigi*)p);
   }
   static void deleteArray_CbmBmonDigi(void *p) {
      delete [] ((::CbmBmonDigi*)p);
   }
   static void destruct_CbmBmonDigi(void *p) {
      typedef ::CbmBmonDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBmonDigi

//______________________________________________________________________________
void CbmBmonDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmBmonDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmBmonDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmBmonDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmBmonDigiData(void *p) {
      return  p ? new(p) ::CbmBmonDigiData : new ::CbmBmonDigiData;
   }
   static void *newArray_CbmBmonDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmBmonDigiData[nElements] : new ::CbmBmonDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmBmonDigiData(void *p) {
      delete ((::CbmBmonDigiData*)p);
   }
   static void deleteArray_CbmBmonDigiData(void *p) {
      delete [] ((::CbmBmonDigiData*)p);
   }
   static void destruct_CbmBmonDigiData(void *p) {
      typedef ::CbmBmonDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmBmonDigiData

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmAddress(void *p) {
      return  p ? new(p) ::CbmAddress : new ::CbmAddress;
   }
   static void *newArray_CbmAddress(Long_t nElements, void *p) {
      return p ? new(p) ::CbmAddress[nElements] : new ::CbmAddress[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmAddress(void *p) {
      delete ((::CbmAddress*)p);
   }
   static void deleteArray_CbmAddress(void *p) {
      delete [] ((::CbmAddress*)p);
   }
   static void destruct_CbmAddress(void *p) {
      typedef ::CbmAddress current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmAddress

//______________________________________________________________________________
void CbmMuchAddress::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchAddress.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchAddress::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchAddress::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchAddress(void *p) {
      return  p ? new(p) ::CbmMuchAddress : new ::CbmMuchAddress;
   }
   static void *newArray_CbmMuchAddress(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchAddress[nElements] : new ::CbmMuchAddress[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchAddress(void *p) {
      delete ((::CbmMuchAddress*)p);
   }
   static void deleteArray_CbmMuchAddress(void *p) {
      delete [] ((::CbmMuchAddress*)p);
   }
   static void destruct_CbmMuchAddress(void *p) {
      typedef ::CbmMuchAddress current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchAddress

//______________________________________________________________________________
void CbmMuchDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchDigi(void *p) {
      return  p ? new(p) ::CbmMuchDigi : new ::CbmMuchDigi;
   }
   static void *newArray_CbmMuchDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchDigi[nElements] : new ::CbmMuchDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchDigi(void *p) {
      delete ((::CbmMuchDigi*)p);
   }
   static void deleteArray_CbmMuchDigi(void *p) {
      delete [] ((::CbmMuchDigi*)p);
   }
   static void destruct_CbmMuchDigi(void *p) {
      typedef ::CbmMuchDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchDigi

//______________________________________________________________________________
void CbmMuchDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchDigiData(void *p) {
      return  p ? new(p) ::CbmMuchDigiData : new ::CbmMuchDigiData;
   }
   static void *newArray_CbmMuchDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchDigiData[nElements] : new ::CbmMuchDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchDigiData(void *p) {
      delete ((::CbmMuchDigiData*)p);
   }
   static void deleteArray_CbmMuchDigiData(void *p) {
      delete [] ((::CbmMuchDigiData*)p);
   }
   static void destruct_CbmMuchDigiData(void *p) {
      typedef ::CbmMuchDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchDigiData

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdAddress(void *p) {
      return  p ? new(p) ::CbmPsdAddress : new ::CbmPsdAddress;
   }
   static void *newArray_CbmPsdAddress(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdAddress[nElements] : new ::CbmPsdAddress[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdAddress(void *p) {
      delete ((::CbmPsdAddress*)p);
   }
   static void deleteArray_CbmPsdAddress(void *p) {
      delete [] ((::CbmPsdAddress*)p);
   }
   static void destruct_CbmPsdAddress(void *p) {
      typedef ::CbmPsdAddress current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdAddress

//______________________________________________________________________________
void CbmPsdDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdDigi(void *p) {
      return  p ? new(p) ::CbmPsdDigi : new ::CbmPsdDigi;
   }
   static void *newArray_CbmPsdDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdDigi[nElements] : new ::CbmPsdDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdDigi(void *p) {
      delete ((::CbmPsdDigi*)p);
   }
   static void deleteArray_CbmPsdDigi(void *p) {
      delete [] ((::CbmPsdDigi*)p);
   }
   static void destruct_CbmPsdDigi(void *p) {
      typedef ::CbmPsdDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdDigi

//______________________________________________________________________________
void CbmPsdDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdDigiData(void *p) {
      return  p ? new(p) ::CbmPsdDigiData : new ::CbmPsdDigiData;
   }
   static void *newArray_CbmPsdDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdDigiData[nElements] : new ::CbmPsdDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdDigiData(void *p) {
      delete ((::CbmPsdDigiData*)p);
   }
   static void deleteArray_CbmPsdDigiData(void *p) {
      delete [] ((::CbmPsdDigiData*)p);
   }
   static void destruct_CbmPsdDigiData(void *p) {
      typedef ::CbmPsdDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdDigiData

//______________________________________________________________________________
void CbmRichDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichDigi(void *p) {
      return  p ? new(p) ::CbmRichDigi : new ::CbmRichDigi;
   }
   static void *newArray_CbmRichDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichDigi[nElements] : new ::CbmRichDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichDigi(void *p) {
      delete ((::CbmRichDigi*)p);
   }
   static void deleteArray_CbmRichDigi(void *p) {
      delete [] ((::CbmRichDigi*)p);
   }
   static void destruct_CbmRichDigi(void *p) {
      typedef ::CbmRichDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichDigi

//______________________________________________________________________________
void CbmRichDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichDigiData(void *p) {
      return  p ? new(p) ::CbmRichDigiData : new ::CbmRichDigiData;
   }
   static void *newArray_CbmRichDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichDigiData[nElements] : new ::CbmRichDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichDigiData(void *p) {
      delete ((::CbmRichDigiData*)p);
   }
   static void deleteArray_CbmRichDigiData(void *p) {
      delete [] ((::CbmRichDigiData*)p);
   }
   static void destruct_CbmRichDigiData(void *p) {
      typedef ::CbmRichDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichDigiData

//______________________________________________________________________________
void CbmStsDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsDigi(void *p) {
      return  p ? new(p) ::CbmStsDigi : new ::CbmStsDigi;
   }
   static void *newArray_CbmStsDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsDigi[nElements] : new ::CbmStsDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsDigi(void *p) {
      delete ((::CbmStsDigi*)p);
   }
   static void deleteArray_CbmStsDigi(void *p) {
      delete [] ((::CbmStsDigi*)p);
   }
   static void destruct_CbmStsDigi(void *p) {
      typedef ::CbmStsDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsDigi

//______________________________________________________________________________
void CbmStsDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsDigiData(void *p) {
      return  p ? new(p) ::CbmStsDigiData : new ::CbmStsDigiData;
   }
   static void *newArray_CbmStsDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsDigiData[nElements] : new ::CbmStsDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsDigiData(void *p) {
      delete ((::CbmStsDigiData*)p);
   }
   static void deleteArray_CbmStsDigiData(void *p) {
      delete [] ((::CbmStsDigiData*)p);
   }
   static void destruct_CbmStsDigiData(void *p) {
      typedef ::CbmStsDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsDigiData

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofAddress(void *p) {
      return  p ? new(p) ::CbmTofAddress : new ::CbmTofAddress;
   }
   static void *newArray_CbmTofAddress(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofAddress[nElements] : new ::CbmTofAddress[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofAddress(void *p) {
      delete ((::CbmTofAddress*)p);
   }
   static void deleteArray_CbmTofAddress(void *p) {
      delete [] ((::CbmTofAddress*)p);
   }
   static void destruct_CbmTofAddress(void *p) {
      typedef ::CbmTofAddress current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofAddress

//______________________________________________________________________________
void CbmTofDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofDigi(void *p) {
      return  p ? new(p) ::CbmTofDigi : new ::CbmTofDigi;
   }
   static void *newArray_CbmTofDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofDigi[nElements] : new ::CbmTofDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofDigi(void *p) {
      delete ((::CbmTofDigi*)p);
   }
   static void deleteArray_CbmTofDigi(void *p) {
      delete [] ((::CbmTofDigi*)p);
   }
   static void destruct_CbmTofDigi(void *p) {
      typedef ::CbmTofDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofDigi

//______________________________________________________________________________
void CbmTofDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofDigiData(void *p) {
      return  p ? new(p) ::CbmTofDigiData : new ::CbmTofDigiData;
   }
   static void *newArray_CbmTofDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofDigiData[nElements] : new ::CbmTofDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofDigiData(void *p) {
      delete ((::CbmTofDigiData*)p);
   }
   static void deleteArray_CbmTofDigiData(void *p) {
      delete [] ((::CbmTofDigiData*)p);
   }
   static void destruct_CbmTofDigiData(void *p) {
      typedef ::CbmTofDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofDigiData

//______________________________________________________________________________
void CbmTrdDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdDigi(void *p) {
      return  p ? new(p) ::CbmTrdDigi : new ::CbmTrdDigi;
   }
   static void *newArray_CbmTrdDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdDigi[nElements] : new ::CbmTrdDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdDigi(void *p) {
      delete ((::CbmTrdDigi*)p);
   }
   static void deleteArray_CbmTrdDigi(void *p) {
      delete [] ((::CbmTrdDigi*)p);
   }
   static void destruct_CbmTrdDigi(void *p) {
      typedef ::CbmTrdDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdDigi

//______________________________________________________________________________
void CbmTrdDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdDigiData(void *p) {
      return  p ? new(p) ::CbmTrdDigiData : new ::CbmTrdDigiData;
   }
   static void *newArray_CbmTrdDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdDigiData[nElements] : new ::CbmTrdDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdDigiData(void *p) {
      delete ((::CbmTrdDigiData*)p);
   }
   static void deleteArray_CbmTrdDigiData(void *p) {
      delete [] ((::CbmTrdDigiData*)p);
   }
   static void destruct_CbmTrdDigiData(void *p) {
      typedef ::CbmTrdDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdDigiData

//______________________________________________________________________________
void CbmDigiData::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiData::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiData(void *p) {
      return  p ? new(p) ::CbmDigiData : new ::CbmDigiData;
   }
   static void *newArray_CbmDigiData(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiData[nElements] : new ::CbmDigiData[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiData(void *p) {
      delete ((::CbmDigiData*)p);
   }
   static void deleteArray_CbmDigiData(void *p) {
      delete [] ((::CbmDigiData*)p);
   }
   static void destruct_CbmDigiData(void *p) {
      typedef ::CbmDigiData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiData

//______________________________________________________________________________
void CbmDigiEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiEvent(void *p) {
      return  p ? new(p) ::CbmDigiEvent : new ::CbmDigiEvent;
   }
   static void *newArray_CbmDigiEvent(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiEvent[nElements] : new ::CbmDigiEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiEvent(void *p) {
      delete ((::CbmDigiEvent*)p);
   }
   static void deleteArray_CbmDigiEvent(void *p) {
      delete [] ((::CbmDigiEvent*)p);
   }
   static void destruct_CbmDigiEvent(void *p) {
      typedef ::CbmDigiEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiEvent

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiTimeslice(void *p) {
      return  p ? new(p) ::CbmDigiTimeslice : new ::CbmDigiTimeslice;
   }
   static void *newArray_CbmDigiTimeslice(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiTimeslice[nElements] : new ::CbmDigiTimeslice[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiTimeslice(void *p) {
      delete ((::CbmDigiTimeslice*)p);
   }
   static void deleteArray_CbmDigiTimeslice(void *p) {
      delete [] ((::CbmDigiTimeslice*)p);
   }
   static void destruct_CbmDigiTimeslice(void *p) {
      typedef ::CbmDigiTimeslice current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiTimeslice

//______________________________________________________________________________
void CbmLink::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmLink.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmLink::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmLink::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmLink(void *p) {
      return  p ? new(p) ::CbmLink : new ::CbmLink;
   }
   static void *newArray_CbmLink(Long_t nElements, void *p) {
      return p ? new(p) ::CbmLink[nElements] : new ::CbmLink[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmLink(void *p) {
      delete ((::CbmLink*)p);
   }
   static void deleteArray_CbmLink(void *p) {
      delete [] ((::CbmLink*)p);
   }
   static void destruct_CbmLink(void *p) {
      typedef ::CbmLink current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmLink

//______________________________________________________________________________
void CbmMatch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMatch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMatch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMatch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMatch(void *p) {
      return  p ? new(p) ::CbmMatch : new ::CbmMatch;
   }
   static void *newArray_CbmMatch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMatch[nElements] : new ::CbmMatch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMatch(void *p) {
      delete ((::CbmMatch*)p);
   }
   static void deleteArray_CbmMatch(void *p) {
      delete [] ((::CbmMatch*)p);
   }
   static void destruct_CbmMatch(void *p) {
      typedef ::CbmMatch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMatch

//______________________________________________________________________________
void CbmTimeSlice::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTimeSlice.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TNamed::Streamer(R__b);
      void *ptr_fType = (void*)&fType;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fType);
      R__b >> fStartTime;
      R__b >> fLength;
      R__b >> fIsEmpty;
      {
         map<ECbmModuleId,int32_t> &R__stl =  fNofData;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            ECbmModuleId R__t;
            Int_t readtemp;
            R__b >> readtemp;
            R__t = static_cast<ECbmModuleId>(readtemp);
            int R__t2;
            R__b >> R__t2;
            typedef enum ECbmModuleId Value_t;
            std::pair<Value_t const, int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> fTimeDataFirst;
      R__b >> fTimeDataLast;
      fMatch.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, CbmTimeSlice::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmTimeSlice::IsA(), kTRUE);
      TNamed::Streamer(R__b);
      R__b << (Int_t)fType;
      R__b << fStartTime;
      R__b << fLength;
      R__b << fIsEmpty;
      {
         map<ECbmModuleId,int32_t> &R__stl =  fNofData;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<ECbmModuleId,int32_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {  void *ptr_enum = (void*)&((*R__k).first );
               R__b >> *reinterpret_cast<Int_t*>(ptr_enum); }
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << fTimeDataFirst;
      R__b << fTimeDataLast;
      fMatch.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTimeSlice(void *p) {
      return  p ? new(p) ::CbmTimeSlice : new ::CbmTimeSlice;
   }
   static void *newArray_CbmTimeSlice(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTimeSlice[nElements] : new ::CbmTimeSlice[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTimeSlice(void *p) {
      delete ((::CbmTimeSlice*)p);
   }
   static void deleteArray_CbmTimeSlice(void *p) {
      delete [] ((::CbmTimeSlice*)p);
   }
   static void destruct_CbmTimeSlice(void *p) {
      typedef ::CbmTimeSlice current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmTimeSlice(TBuffer &buf, void *obj) {
      ((::CbmTimeSlice*)obj)->::CbmTimeSlice::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmTimeSlice

//______________________________________________________________________________
void CbmVertex::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmVertex.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmVertex::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmVertex::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmVertex(void *p) {
      return  p ? new(p) ::CbmVertex : new ::CbmVertex;
   }
   static void *newArray_CbmVertex(Long_t nElements, void *p) {
      return p ? new(p) ::CbmVertex[nElements] : new ::CbmVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmVertex(void *p) {
      delete ((::CbmVertex*)p);
   }
   static void deleteArray_CbmVertex(void *p) {
      delete [] ((::CbmVertex*)p);
   }
   static void destruct_CbmVertex(void *p) {
      typedef ::CbmVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmVertex

//______________________________________________________________________________
void CbmEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmEvent(void *p) {
      return  p ? new(p) ::CbmEvent : new ::CbmEvent;
   }
   static void *newArray_CbmEvent(Long_t nElements, void *p) {
      return p ? new(p) ::CbmEvent[nElements] : new ::CbmEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmEvent(void *p) {
      delete ((::CbmEvent*)p);
   }
   static void deleteArray_CbmEvent(void *p) {
      delete [] ((::CbmEvent*)p);
   }
   static void destruct_CbmEvent(void *p) {
      typedef ::CbmEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmEvent

//______________________________________________________________________________
void CbmDigiContainer::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiContainer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiContainer::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiContainer::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigiContainer(void *p) {
      delete ((::CbmDigiContainer*)p);
   }
   static void deleteArray_CbmDigiContainer(void *p) {
      delete [] ((::CbmDigiContainer*)p);
   }
   static void destruct_CbmDigiContainer(void *p) {
      typedef ::CbmDigiContainer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiContainer

//______________________________________________________________________________
template <> void CbmDigiVector<CbmPsdDsp>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmPsdDsp>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmPsdDsp>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmPsdDsp>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmPsdDspgR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmPsdDsp> : new ::CbmDigiVector<CbmPsdDsp>;
   }
   static void *newArray_CbmDigiVectorlECbmPsdDspgR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmPsdDsp>[nElements] : new ::CbmDigiVector<CbmPsdDsp>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmPsdDspgR(void *p) {
      delete ((::CbmDigiVector<CbmPsdDsp>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmPsdDspgR(void *p) {
      delete [] ((::CbmDigiVector<CbmPsdDsp>*)p);
   }
   static void destruct_CbmDigiVectorlECbmPsdDspgR(void *p) {
      typedef ::CbmDigiVector<CbmPsdDsp> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmPsdDsp>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmPsdDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmPsdDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmPsdDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmPsdDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmPsdDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmPsdDigi> : new ::CbmDigiVector<CbmPsdDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmPsdDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmPsdDigi>[nElements] : new ::CbmDigiVector<CbmPsdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmPsdDigigR(void *p) {
      delete ((::CbmDigiVector<CbmPsdDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmPsdDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmPsdDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmPsdDigigR(void *p) {
      typedef ::CbmDigiVector<CbmPsdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmPsdDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmTofDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmTofDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmTofDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmTofDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmTofDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmTofDigi> : new ::CbmDigiVector<CbmTofDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmTofDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmTofDigi>[nElements] : new ::CbmDigiVector<CbmTofDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmTofDigigR(void *p) {
      delete ((::CbmDigiVector<CbmTofDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmTofDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmTofDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmTofDigigR(void *p) {
      typedef ::CbmDigiVector<CbmTofDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmTofDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmTrdDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmTrdDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmTrdDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmTrdDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmTrdDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmTrdDigi> : new ::CbmDigiVector<CbmTrdDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmTrdDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmTrdDigi>[nElements] : new ::CbmDigiVector<CbmTrdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmTrdDigigR(void *p) {
      delete ((::CbmDigiVector<CbmTrdDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmTrdDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmTrdDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmTrdDigigR(void *p) {
      typedef ::CbmDigiVector<CbmTrdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmTrdDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmMuchBeamTimeDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmMuchBeamTimeDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmMuchBeamTimeDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmMuchBeamTimeDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmMuchBeamTimeDigi> : new ::CbmDigiVector<CbmMuchBeamTimeDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmMuchBeamTimeDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmMuchBeamTimeDigi>[nElements] : new ::CbmDigiVector<CbmMuchBeamTimeDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p) {
      delete ((::CbmDigiVector<CbmMuchBeamTimeDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmMuchBeamTimeDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmMuchBeamTimeDigigR(void *p) {
      typedef ::CbmDigiVector<CbmMuchBeamTimeDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmMuchBeamTimeDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmMuchDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmMuchDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmMuchDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmMuchDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmMuchDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmMuchDigi> : new ::CbmDigiVector<CbmMuchDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmMuchDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmMuchDigi>[nElements] : new ::CbmDigiVector<CbmMuchDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmMuchDigigR(void *p) {
      delete ((::CbmDigiVector<CbmMuchDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmMuchDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmMuchDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmMuchDigigR(void *p) {
      typedef ::CbmDigiVector<CbmMuchDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmMuchDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmRichDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmRichDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmRichDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmRichDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmRichDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmRichDigi> : new ::CbmDigiVector<CbmRichDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmRichDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmRichDigi>[nElements] : new ::CbmDigiVector<CbmRichDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmRichDigigR(void *p) {
      delete ((::CbmDigiVector<CbmRichDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmRichDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmRichDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmRichDigigR(void *p) {
      typedef ::CbmDigiVector<CbmRichDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmRichDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmStsDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmStsDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmStsDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmStsDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmStsDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmStsDigi> : new ::CbmDigiVector<CbmStsDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmStsDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmStsDigi>[nElements] : new ::CbmDigiVector<CbmStsDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmStsDigigR(void *p) {
      delete ((::CbmDigiVector<CbmStsDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmStsDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmStsDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmStsDigigR(void *p) {
      typedef ::CbmDigiVector<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmStsDigi>

//______________________________________________________________________________
template <> void CbmDigiVector<CbmMvdDigi>::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmDigiVector<CbmMvdDigi>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmDigiVector<CbmMvdDigi>::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmDigiVector<CbmMvdDigi>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmDigiVectorlECbmMvdDigigR(void *p) {
      return  p ? new(p) ::CbmDigiVector<CbmMvdDigi> : new ::CbmDigiVector<CbmMvdDigi>;
   }
   static void *newArray_CbmDigiVectorlECbmMvdDigigR(Long_t nElements, void *p) {
      return p ? new(p) ::CbmDigiVector<CbmMvdDigi>[nElements] : new ::CbmDigiVector<CbmMvdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmDigiVectorlECbmMvdDigigR(void *p) {
      delete ((::CbmDigiVector<CbmMvdDigi>*)p);
   }
   static void deleteArray_CbmDigiVectorlECbmMvdDigigR(void *p) {
      delete [] ((::CbmDigiVector<CbmMvdDigi>*)p);
   }
   static void destruct_CbmDigiVectorlECbmMvdDigigR(void *p) {
      typedef ::CbmDigiVector<CbmMvdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiVector<CbmMvdDigi>

//______________________________________________________________________________
void CbmEventStore::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmEventStore.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmEventStore::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmEventStore::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmEventStore(void *p) {
      return  p ? new(p) ::CbmEventStore : new ::CbmEventStore;
   }
   static void *newArray_CbmEventStore(Long_t nElements, void *p) {
      return p ? new(p) ::CbmEventStore[nElements] : new ::CbmEventStore[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmEventStore(void *p) {
      delete ((::CbmEventStore*)p);
   }
   static void deleteArray_CbmEventStore(void *p) {
      delete [] ((::CbmEventStore*)p);
   }
   static void destruct_CbmEventStore(void *p) {
      typedef ::CbmEventStore current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmEventStore

//______________________________________________________________________________
void CbmHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      void *ptr_fType = (void*)&fType;
      R__b >> *reinterpret_cast<Int_t*>(ptr_fType);
      R__b >> fZ;
      R__b >> fDz;
      R__b >> fRefId;
      R__b >> fAddress;
      R__b >> fTime;
      R__b >> fTimeError;
      R__b >> fMatch;
      R__b.CheckByteCount(R__s, R__c, CbmHit::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << (Int_t)fType;
      R__b << fZ;
      R__b << fDz;
      R__b << fRefId;
      R__b << fAddress;
      R__b << fTime;
      R__b << fTimeError;
      R__b << fMatch;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmHit(void *p) {
      return  p ? new(p) ::CbmHit : new ::CbmHit;
   }
   static void *newArray_CbmHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmHit[nElements] : new ::CbmHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmHit(void *p) {
      delete ((::CbmHit*)p);
   }
   static void deleteArray_CbmHit(void *p) {
      delete [] ((::CbmHit*)p);
   }
   static void destruct_CbmHit(void *p) {
      typedef ::CbmHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmHit(TBuffer &buf, void *obj) {
      ((::CbmHit*)obj)->::CbmHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmHit

//______________________________________________________________________________
void CbmPixelHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPixelHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPixelHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPixelHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPixelHit(void *p) {
      return  p ? new(p) ::CbmPixelHit : new ::CbmPixelHit;
   }
   static void *newArray_CbmPixelHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPixelHit[nElements] : new ::CbmPixelHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPixelHit(void *p) {
      delete ((::CbmPixelHit*)p);
   }
   static void deleteArray_CbmPixelHit(void *p) {
      delete [] ((::CbmPixelHit*)p);
   }
   static void destruct_CbmPixelHit(void *p) {
      typedef ::CbmPixelHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPixelHit

//______________________________________________________________________________
void CbmStripHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStripHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStripHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStripHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStripHit(void *p) {
      return  p ? new(p) ::CbmStripHit : new ::CbmStripHit;
   }
   static void *newArray_CbmStripHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStripHit[nElements] : new ::CbmStripHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStripHit(void *p) {
      delete ((::CbmStripHit*)p);
   }
   static void deleteArray_CbmStripHit(void *p) {
      delete [] ((::CbmStripHit*)p);
   }
   static void destruct_CbmStripHit(void *p) {
      typedef ::CbmStripHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStripHit

//______________________________________________________________________________
void CbmTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrack(void *p) {
      return  p ? new(p) ::CbmTrack : new ::CbmTrack;
   }
   static void *newArray_CbmTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrack[nElements] : new ::CbmTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrack(void *p) {
      delete ((::CbmTrack*)p);
   }
   static void deleteArray_CbmTrack(void *p) {
      delete [] ((::CbmTrack*)p);
   }
   static void destruct_CbmTrack(void *p) {
      typedef ::CbmTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrack

//______________________________________________________________________________
void CbmTrackMatch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackMatch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackMatch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackMatch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackMatch(void *p) {
      return  p ? new(p) ::CbmTrackMatch : new ::CbmTrackMatch;
   }
   static void *newArray_CbmTrackMatch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackMatch[nElements] : new ::CbmTrackMatch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackMatch(void *p) {
      delete ((::CbmTrackMatch*)p);
   }
   static void deleteArray_CbmTrackMatch(void *p) {
      delete [] ((::CbmTrackMatch*)p);
   }
   static void destruct_CbmTrackMatch(void *p) {
      typedef ::CbmTrackMatch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackMatch

//______________________________________________________________________________
void CbmCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmCluster(void *p) {
      return  p ? new(p) ::CbmCluster : new ::CbmCluster;
   }
   static void *newArray_CbmCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmCluster[nElements] : new ::CbmCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmCluster(void *p) {
      delete ((::CbmCluster*)p);
   }
   static void deleteArray_CbmCluster(void *p) {
      delete [] ((::CbmCluster*)p);
   }
   static void destruct_CbmCluster(void *p) {
      typedef ::CbmCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmCluster

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmModuleList(void *p) {
      return  p ? new(p) ::CbmModuleList : new ::CbmModuleList;
   }
   static void *newArray_CbmModuleList(Long_t nElements, void *p) {
      return p ? new(p) ::CbmModuleList[nElements] : new ::CbmModuleList[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmModuleList(void *p) {
      delete ((::CbmModuleList*)p);
   }
   static void deleteArray_CbmModuleList(void *p) {
      delete [] ((::CbmModuleList*)p);
   }
   static void destruct_CbmModuleList(void *p) {
      typedef ::CbmModuleList current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmModuleList

//______________________________________________________________________________
void CbmErrorMessage::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmErrorMessage.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmErrorMessage::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmErrorMessage::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmErrorMessage(void *p) {
      return  p ? new(p) ::CbmErrorMessage : new ::CbmErrorMessage;
   }
   static void *newArray_CbmErrorMessage(Long_t nElements, void *p) {
      return p ? new(p) ::CbmErrorMessage[nElements] : new ::CbmErrorMessage[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmErrorMessage(void *p) {
      delete ((::CbmErrorMessage*)p);
   }
   static void deleteArray_CbmErrorMessage(void *p) {
      delete [] ((::CbmErrorMessage*)p);
   }
   static void destruct_CbmErrorMessage(void *p) {
      typedef ::CbmErrorMessage current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmErrorMessage

//______________________________________________________________________________
void CbmMCTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMCTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMCTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCTrack(void *p) {
      return  p ? new(p) ::CbmMCTrack : new ::CbmMCTrack;
   }
   static void *newArray_CbmMCTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCTrack[nElements] : new ::CbmMCTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCTrack(void *p) {
      delete ((::CbmMCTrack*)p);
   }
   static void deleteArray_CbmMCTrack(void *p) {
      delete [] ((::CbmMCTrack*)p);
   }
   static void destruct_CbmMCTrack(void *p) {
      typedef ::CbmMCTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMCTrack

//______________________________________________________________________________
void CbmMCEventInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCEventInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fFileId;
      R__b >> fEventId;
      R__b >> fTime;
      R__b.CheckByteCount(R__s, R__c, CbmMCEventInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMCEventInfo::IsA(), kTRUE);
      R__b << fFileId;
      R__b << fEventId;
      R__b << fTime;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCEventInfo(void *p) {
      return  p ? new(p) ::CbmMCEventInfo : new ::CbmMCEventInfo;
   }
   static void *newArray_CbmMCEventInfo(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCEventInfo[nElements] : new ::CbmMCEventInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCEventInfo(void *p) {
      delete ((::CbmMCEventInfo*)p);
   }
   static void deleteArray_CbmMCEventInfo(void *p) {
      delete [] ((::CbmMCEventInfo*)p);
   }
   static void destruct_CbmMCEventInfo(void *p) {
      typedef ::CbmMCEventInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMCEventInfo(TBuffer &buf, void *obj) {
      ((::CbmMCEventInfo*)obj)->::CbmMCEventInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMCEventInfo

//______________________________________________________________________________
void CbmMCEventList::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMCEventList.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TNamed::Streamer(R__b);
      {
         vector<CbmMCEventInfo> &R__stl =  fEvents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            CbmMCEventInfo R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      R__b >> fIsSorted;
      R__b.CheckByteCount(R__s, R__c, CbmMCEventList::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmMCEventList::IsA(), kTRUE);
      TNamed::Streamer(R__b);
      {
         vector<CbmMCEventInfo> &R__stl =  fEvents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<CbmMCEventInfo>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((CbmMCEventInfo&)(*R__k)).Streamer(R__b);
            }
         }
      }
      R__b << fIsSorted;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMCEventList(void *p) {
      return  p ? new(p) ::CbmMCEventList : new ::CbmMCEventList;
   }
   static void *newArray_CbmMCEventList(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMCEventList[nElements] : new ::CbmMCEventList[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMCEventList(void *p) {
      delete ((::CbmMCEventList*)p);
   }
   static void deleteArray_CbmMCEventList(void *p) {
      delete [] ((::CbmMCEventList*)p);
   }
   static void destruct_CbmMCEventList(void *p) {
      typedef ::CbmMCEventList current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmMCEventList(TBuffer &buf, void *obj) {
      ((::CbmMCEventList*)obj)->::CbmMCEventList::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmMCEventList

//______________________________________________________________________________
void CbmTrackMatchNew::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackMatchNew.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrackMatchNew::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrackMatchNew::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackMatchNew(void *p) {
      return  p ? new(p) ::CbmTrackMatchNew : new ::CbmTrackMatchNew;
   }
   static void *newArray_CbmTrackMatchNew(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackMatchNew[nElements] : new ::CbmTrackMatchNew[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackMatchNew(void *p) {
      delete ((::CbmTrackMatchNew*)p);
   }
   static void deleteArray_CbmTrackMatchNew(void *p) {
      delete [] ((::CbmTrackMatchNew*)p);
   }
   static void destruct_CbmTrackMatchNew(void *p) {
      typedef ::CbmTrackMatchNew current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrackMatchNew

//______________________________________________________________________________
void CbmTsEventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTsEventHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTsEventHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTsEventHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTsEventHeader(void *p) {
      return  p ? new(p) ::CbmTsEventHeader : new ::CbmTsEventHeader;
   }
   static void *newArray_CbmTsEventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTsEventHeader[nElements] : new ::CbmTsEventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTsEventHeader(void *p) {
      delete ((::CbmTsEventHeader*)p);
   }
   static void deleteArray_CbmTsEventHeader(void *p) {
      delete [] ((::CbmTsEventHeader*)p);
   }
   static void destruct_CbmTsEventHeader(void *p) {
      typedef ::CbmTsEventHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTsEventHeader

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CbmDigiBranchBase(void *p) {
      delete ((::CbmDigiBranchBase*)p);
   }
   static void deleteArray_CbmDigiBranchBase(void *p) {
      delete [] ((::CbmDigiBranchBase*)p);
   }
   static void destruct_CbmDigiBranchBase(void *p) {
      typedef ::CbmDigiBranchBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmDigiBranchBase

//______________________________________________________________________________
void CbmMvdDetectorId::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdDetectorId.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdDetectorId::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdDetectorId::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdDetectorId(void *p) {
      return  p ? new(p) ::CbmMvdDetectorId : new ::CbmMvdDetectorId;
   }
   static void *newArray_CbmMvdDetectorId(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdDetectorId[nElements] : new ::CbmMvdDetectorId[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdDetectorId(void *p) {
      delete ((::CbmMvdDetectorId*)p);
   }
   static void deleteArray_CbmMvdDetectorId(void *p) {
      delete [] ((::CbmMvdDetectorId*)p);
   }
   static void destruct_CbmMvdDetectorId(void *p) {
      typedef ::CbmMvdDetectorId current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdDetectorId

//______________________________________________________________________________
void CbmMvdCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdCluster(void *p) {
      return  p ? new(p) ::CbmMvdCluster : new ::CbmMvdCluster;
   }
   static void *newArray_CbmMvdCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdCluster[nElements] : new ::CbmMvdCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdCluster(void *p) {
      delete ((::CbmMvdCluster*)p);
   }
   static void deleteArray_CbmMvdCluster(void *p) {
      delete [] ((::CbmMvdCluster*)p);
   }
   static void destruct_CbmMvdCluster(void *p) {
      typedef ::CbmMvdCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdCluster

//______________________________________________________________________________
void CbmMvdDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdDigi(void *p) {
      return  p ? new(p) ::CbmMvdDigi : new ::CbmMvdDigi;
   }
   static void *newArray_CbmMvdDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdDigi[nElements] : new ::CbmMvdDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdDigi(void *p) {
      delete ((::CbmMvdDigi*)p);
   }
   static void deleteArray_CbmMvdDigi(void *p) {
      delete [] ((::CbmMvdDigi*)p);
   }
   static void destruct_CbmMvdDigi(void *p) {
      typedef ::CbmMvdDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdDigi

//______________________________________________________________________________
void CbmMvdHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdHit(void *p) {
      return  p ? new(p) ::CbmMvdHit : new ::CbmMvdHit;
   }
   static void *newArray_CbmMvdHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdHit[nElements] : new ::CbmMvdHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdHit(void *p) {
      delete ((::CbmMvdHit*)p);
   }
   static void deleteArray_CbmMvdHit(void *p) {
      delete [] ((::CbmMvdHit*)p);
   }
   static void destruct_CbmMvdHit(void *p) {
      typedef ::CbmMvdHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdHit

//______________________________________________________________________________
void CbmMvdPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdPoint(void *p) {
      return  p ? new(p) ::CbmMvdPoint : new ::CbmMvdPoint;
   }
   static void *newArray_CbmMvdPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdPoint[nElements] : new ::CbmMvdPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdPoint(void *p) {
      delete ((::CbmMvdPoint*)p);
   }
   static void deleteArray_CbmMvdPoint(void *p) {
      delete [] ((::CbmMvdPoint*)p);
   }
   static void destruct_CbmMvdPoint(void *p) {
      typedef ::CbmMvdPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdPoint

//______________________________________________________________________________
void CbmMvdHitMatch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMvdHitMatch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMvdHitMatch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMvdHitMatch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMvdHitMatch(void *p) {
      return  p ? new(p) ::CbmMvdHitMatch : new ::CbmMvdHitMatch;
   }
   static void *newArray_CbmMvdHitMatch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMvdHitMatch[nElements] : new ::CbmMvdHitMatch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMvdHitMatch(void *p) {
      delete ((::CbmMvdHitMatch*)p);
   }
   static void deleteArray_CbmMvdHitMatch(void *p) {
      delete [] ((::CbmMvdHitMatch*)p);
   }
   static void destruct_CbmMvdHitMatch(void *p) {
      typedef ::CbmMvdHitMatch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMvdHitMatch

//______________________________________________________________________________
void CbmStsCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsCluster(void *p) {
      return  p ? new(p) ::CbmStsCluster : new ::CbmStsCluster;
   }
   static void *newArray_CbmStsCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsCluster[nElements] : new ::CbmStsCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsCluster(void *p) {
      delete ((::CbmStsCluster*)p);
   }
   static void deleteArray_CbmStsCluster(void *p) {
      delete [] ((::CbmStsCluster*)p);
   }
   static void destruct_CbmStsCluster(void *p) {
      typedef ::CbmStsCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsCluster

//______________________________________________________________________________
void CbmStsHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsHit(void *p) {
      return  p ? new(p) ::CbmStsHit : new ::CbmStsHit;
   }
   static void *newArray_CbmStsHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsHit[nElements] : new ::CbmStsHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsHit(void *p) {
      delete ((::CbmStsHit*)p);
   }
   static void deleteArray_CbmStsHit(void *p) {
      delete [] ((::CbmStsHit*)p);
   }
   static void destruct_CbmStsHit(void *p) {
      typedef ::CbmStsHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsHit

//______________________________________________________________________________
void CbmStsPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsPoint(void *p) {
      return  p ? new(p) ::CbmStsPoint : new ::CbmStsPoint;
   }
   static void *newArray_CbmStsPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsPoint[nElements] : new ::CbmStsPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsPoint(void *p) {
      delete ((::CbmStsPoint*)p);
   }
   static void deleteArray_CbmStsPoint(void *p) {
      delete [] ((::CbmStsPoint*)p);
   }
   static void destruct_CbmStsPoint(void *p) {
      typedef ::CbmStsPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsPoint

//______________________________________________________________________________
void CbmStsTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmStsTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmStsTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmStsTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmStsTrack(void *p) {
      return  p ? new(p) ::CbmStsTrack : new ::CbmStsTrack;
   }
   static void *newArray_CbmStsTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmStsTrack[nElements] : new ::CbmStsTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmStsTrack(void *p) {
      delete ((::CbmStsTrack*)p);
   }
   static void deleteArray_CbmStsTrack(void *p) {
      delete [] ((::CbmStsTrack*)p);
   }
   static void destruct_CbmStsTrack(void *p) {
      typedef ::CbmStsTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmStsTrack

//______________________________________________________________________________
void CbmRichRing::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichRing.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichRing::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichRing::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichRing(void *p) {
      return  p ? new(p) ::CbmRichRing : new ::CbmRichRing;
   }
   static void *newArray_CbmRichRing(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichRing[nElements] : new ::CbmRichRing[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichRing(void *p) {
      delete ((::CbmRichRing*)p);
   }
   static void deleteArray_CbmRichRing(void *p) {
      delete [] ((::CbmRichRing*)p);
   }
   static void destruct_CbmRichRing(void *p) {
      typedef ::CbmRichRing current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichRing

//______________________________________________________________________________
void CbmRichHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichHit(void *p) {
      return  p ? new(p) ::CbmRichHit : new ::CbmRichHit;
   }
   static void *newArray_CbmRichHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichHit[nElements] : new ::CbmRichHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichHit(void *p) {
      delete ((::CbmRichHit*)p);
   }
   static void deleteArray_CbmRichHit(void *p) {
      delete [] ((::CbmRichHit*)p);
   }
   static void destruct_CbmRichHit(void *p) {
      typedef ::CbmRichHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichHit

//______________________________________________________________________________
void CbmRichPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichPoint(void *p) {
      return  p ? new(p) ::CbmRichPoint : new ::CbmRichPoint;
   }
   static void *newArray_CbmRichPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichPoint[nElements] : new ::CbmRichPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichPoint(void *p) {
      delete ((::CbmRichPoint*)p);
   }
   static void deleteArray_CbmRichPoint(void *p) {
      delete [] ((::CbmRichPoint*)p);
   }
   static void destruct_CbmRichPoint(void *p) {
      typedef ::CbmRichPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichPoint

//______________________________________________________________________________
void CbmRichTrbDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmRichTrbDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmRichTrbDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmRichTrbDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmRichTrbDigi(void *p) {
      return  p ? new(p) ::CbmRichTrbDigi : new ::CbmRichTrbDigi;
   }
   static void *newArray_CbmRichTrbDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmRichTrbDigi[nElements] : new ::CbmRichTrbDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmRichTrbDigi(void *p) {
      delete ((::CbmRichTrbDigi*)p);
   }
   static void deleteArray_CbmRichTrbDigi(void *p) {
      delete [] ((::CbmRichTrbDigi*)p);
   }
   static void destruct_CbmRichTrbDigi(void *p) {
      typedef ::CbmRichTrbDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmRichTrbDigi

//______________________________________________________________________________
void CbmMuchPixelHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchPixelHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchPixelHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchPixelHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchPixelHit(void *p) {
      return  p ? new(p) ::CbmMuchPixelHit : new ::CbmMuchPixelHit;
   }
   static void *newArray_CbmMuchPixelHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchPixelHit[nElements] : new ::CbmMuchPixelHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchPixelHit(void *p) {
      delete ((::CbmMuchPixelHit*)p);
   }
   static void deleteArray_CbmMuchPixelHit(void *p) {
      delete [] ((::CbmMuchPixelHit*)p);
   }
   static void destruct_CbmMuchPixelHit(void *p) {
      typedef ::CbmMuchPixelHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchPixelHit

//______________________________________________________________________________
void CbmMuchPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchPoint(void *p) {
      return  p ? new(p) ::CbmMuchPoint : new ::CbmMuchPoint;
   }
   static void *newArray_CbmMuchPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchPoint[nElements] : new ::CbmMuchPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchPoint(void *p) {
      delete ((::CbmMuchPoint*)p);
   }
   static void deleteArray_CbmMuchPoint(void *p) {
      delete [] ((::CbmMuchPoint*)p);
   }
   static void destruct_CbmMuchPoint(void *p) {
      typedef ::CbmMuchPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchPoint

//______________________________________________________________________________
void CbmMuchCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchCluster(void *p) {
      return  p ? new(p) ::CbmMuchCluster : new ::CbmMuchCluster;
   }
   static void *newArray_CbmMuchCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchCluster[nElements] : new ::CbmMuchCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchCluster(void *p) {
      delete ((::CbmMuchCluster*)p);
   }
   static void deleteArray_CbmMuchCluster(void *p) {
      delete [] ((::CbmMuchCluster*)p);
   }
   static void destruct_CbmMuchCluster(void *p) {
      typedef ::CbmMuchCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchCluster

//______________________________________________________________________________
void CbmMuchBeamTimeDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchBeamTimeDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchBeamTimeDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchBeamTimeDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchBeamTimeDigi(void *p) {
      return  p ? new(p) ::CbmMuchBeamTimeDigi : new ::CbmMuchBeamTimeDigi;
   }
   static void *newArray_CbmMuchBeamTimeDigi(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchBeamTimeDigi[nElements] : new ::CbmMuchBeamTimeDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchBeamTimeDigi(void *p) {
      delete ((::CbmMuchBeamTimeDigi*)p);
   }
   static void deleteArray_CbmMuchBeamTimeDigi(void *p) {
      delete [] ((::CbmMuchBeamTimeDigi*)p);
   }
   static void destruct_CbmMuchBeamTimeDigi(void *p) {
      typedef ::CbmMuchBeamTimeDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchBeamTimeDigi

//______________________________________________________________________________
void CbmMuchDigiMatch::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchDigiMatch.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchDigiMatch::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchDigiMatch::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchDigiMatch(void *p) {
      return  p ? new(p) ::CbmMuchDigiMatch : new ::CbmMuchDigiMatch;
   }
   static void *newArray_CbmMuchDigiMatch(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchDigiMatch[nElements] : new ::CbmMuchDigiMatch[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchDigiMatch(void *p) {
      delete ((::CbmMuchDigiMatch*)p);
   }
   static void deleteArray_CbmMuchDigiMatch(void *p) {
      delete [] ((::CbmMuchDigiMatch*)p);
   }
   static void destruct_CbmMuchDigiMatch(void *p) {
      typedef ::CbmMuchDigiMatch current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchDigiMatch

//______________________________________________________________________________
void CbmMuchTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmMuchTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmMuchTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmMuchTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmMuchTrack(void *p) {
      return  p ? new(p) ::CbmMuchTrack : new ::CbmMuchTrack;
   }
   static void *newArray_CbmMuchTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmMuchTrack[nElements] : new ::CbmMuchTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmMuchTrack(void *p) {
      delete ((::CbmMuchTrack*)p);
   }
   static void deleteArray_CbmMuchTrack(void *p) {
      delete [] ((::CbmMuchTrack*)p);
   }
   static void destruct_CbmMuchTrack(void *p) {
      typedef ::CbmMuchTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmMuchTrack

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdAddress(void *p) {
      return  p ? new(p) ::CbmTrdAddress : new ::CbmTrdAddress;
   }
   static void *newArray_CbmTrdAddress(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdAddress[nElements] : new ::CbmTrdAddress[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdAddress(void *p) {
      delete ((::CbmTrdAddress*)p);
   }
   static void deleteArray_CbmTrdAddress(void *p) {
      delete [] ((::CbmTrdAddress*)p);
   }
   static void destruct_CbmTrdAddress(void *p) {
      typedef ::CbmTrdAddress current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdAddress

//______________________________________________________________________________
void CbmTrdHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdHit(void *p) {
      return  p ? new(p) ::CbmTrdHit : new ::CbmTrdHit;
   }
   static void *newArray_CbmTrdHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdHit[nElements] : new ::CbmTrdHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdHit(void *p) {
      delete ((::CbmTrdHit*)p);
   }
   static void deleteArray_CbmTrdHit(void *p) {
      delete [] ((::CbmTrdHit*)p);
   }
   static void destruct_CbmTrdHit(void *p) {
      typedef ::CbmTrdHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdHit

//______________________________________________________________________________
void CbmTrdTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdTrack(void *p) {
      return  p ? new(p) ::CbmTrdTrack : new ::CbmTrdTrack;
   }
   static void *newArray_CbmTrdTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdTrack[nElements] : new ::CbmTrdTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdTrack(void *p) {
      delete ((::CbmTrdTrack*)p);
   }
   static void deleteArray_CbmTrdTrack(void *p) {
      delete [] ((::CbmTrdTrack*)p);
   }
   static void destruct_CbmTrdTrack(void *p) {
      typedef ::CbmTrdTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdTrack

//______________________________________________________________________________
void CbmTrdPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdPoint(void *p) {
      return  p ? new(p) ::CbmTrdPoint : new ::CbmTrdPoint;
   }
   static void *newArray_CbmTrdPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdPoint[nElements] : new ::CbmTrdPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdPoint(void *p) {
      delete ((::CbmTrdPoint*)p);
   }
   static void deleteArray_CbmTrdPoint(void *p) {
      delete [] ((::CbmTrdPoint*)p);
   }
   static void destruct_CbmTrdPoint(void *p) {
      typedef ::CbmTrdPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdPoint

//______________________________________________________________________________
void CbmTrdCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrdCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTrdCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTrdCluster::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdCluster(void *p) {
      return  p ? new(p) ::CbmTrdCluster : new ::CbmTrdCluster;
   }
   static void *newArray_CbmTrdCluster(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdCluster[nElements] : new ::CbmTrdCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdCluster(void *p) {
      delete ((::CbmTrdCluster*)p);
   }
   static void deleteArray_CbmTrdCluster(void *p) {
      delete [] ((::CbmTrdCluster*)p);
   }
   static void destruct_CbmTrdCluster(void *p) {
      typedef ::CbmTrdCluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdCluster

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrdRawMessageSpadic(void *p) {
      return  p ? new(p) ::CbmTrdRawMessageSpadic : new ::CbmTrdRawMessageSpadic;
   }
   static void *newArray_CbmTrdRawMessageSpadic(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrdRawMessageSpadic[nElements] : new ::CbmTrdRawMessageSpadic[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrdRawMessageSpadic(void *p) {
      delete ((::CbmTrdRawMessageSpadic*)p);
   }
   static void deleteArray_CbmTrdRawMessageSpadic(void *p) {
      delete [] ((::CbmTrdRawMessageSpadic*)p);
   }
   static void destruct_CbmTrdRawMessageSpadic(void *p) {
      typedef ::CbmTrdRawMessageSpadic current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTrdRawMessageSpadic

//______________________________________________________________________________
void CbmTofHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofHit(void *p) {
      return  p ? new(p) ::CbmTofHit : new ::CbmTofHit;
   }
   static void *newArray_CbmTofHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofHit[nElements] : new ::CbmTofHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofHit(void *p) {
      delete ((::CbmTofHit*)p);
   }
   static void deleteArray_CbmTofHit(void *p) {
      delete [] ((::CbmTofHit*)p);
   }
   static void destruct_CbmTofHit(void *p) {
      typedef ::CbmTofHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofHit

//______________________________________________________________________________
void CbmTofPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofPoint(void *p) {
      return  p ? new(p) ::CbmTofPoint : new ::CbmTofPoint;
   }
   static void *newArray_CbmTofPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofPoint[nElements] : new ::CbmTofPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofPoint(void *p) {
      delete ((::CbmTofPoint*)p);
   }
   static void deleteArray_CbmTofPoint(void *p) {
      delete [] ((::CbmTofPoint*)p);
   }
   static void destruct_CbmTofPoint(void *p) {
      typedef ::CbmTofPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofPoint

//______________________________________________________________________________
void CbmTofTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTrack(void *p) {
      return  p ? new(p) ::CbmTofTrack : new ::CbmTofTrack;
   }
   static void *newArray_CbmTofTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTrack[nElements] : new ::CbmTofTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTrack(void *p) {
      delete ((::CbmTofTrack*)p);
   }
   static void deleteArray_CbmTofTrack(void *p) {
      delete [] ((::CbmTofTrack*)p);
   }
   static void destruct_CbmTofTrack(void *p) {
      typedef ::CbmTofTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrack

//______________________________________________________________________________
void CbmTofTrackletParam::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTrackletParam.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTrackletParam::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTrackletParam::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTrackletParam(void *p) {
      return  p ? new(p) ::CbmTofTrackletParam : new ::CbmTofTrackletParam;
   }
   static void *newArray_CbmTofTrackletParam(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTrackletParam[nElements] : new ::CbmTofTrackletParam[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTrackletParam(void *p) {
      delete ((::CbmTofTrackletParam*)p);
   }
   static void deleteArray_CbmTofTrackletParam(void *p) {
      delete [] ((::CbmTofTrackletParam*)p);
   }
   static void destruct_CbmTofTrackletParam(void *p) {
      typedef ::CbmTofTrackletParam current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTrackletParam

//______________________________________________________________________________
void CbmTofTracklet::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTofTracklet.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmTofTracklet::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmTofTracklet::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTofTracklet(void *p) {
      return  p ? new(p) ::CbmTofTracklet : new ::CbmTofTracklet;
   }
   static void *newArray_CbmTofTracklet(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTofTracklet[nElements] : new ::CbmTofTracklet[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTofTracklet(void *p) {
      delete ((::CbmTofTracklet*)p);
   }
   static void deleteArray_CbmTofTracklet(void *p) {
      delete [] ((::CbmTofTracklet*)p);
   }
   static void destruct_CbmTofTracklet(void *p) {
      typedef ::CbmTofTracklet current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmTofTracklet

//______________________________________________________________________________
void CbmPsdDsp::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdDsp.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdDsp::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdDsp::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdDsp(void *p) {
      return  p ? new(p) ::CbmPsdDsp : new ::CbmPsdDsp;
   }
   static void *newArray_CbmPsdDsp(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdDsp[nElements] : new ::CbmPsdDsp[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdDsp(void *p) {
      delete ((::CbmPsdDsp*)p);
   }
   static void deleteArray_CbmPsdDsp(void *p) {
      delete [] ((::CbmPsdDsp*)p);
   }
   static void destruct_CbmPsdDsp(void *p) {
      typedef ::CbmPsdDsp current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdDsp

//______________________________________________________________________________
void CbmPsdHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fModuleID;
      R__b >> fEdep;
      R__b.CheckByteCount(R__s, R__c, CbmPsdHit::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmPsdHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fModuleID;
      R__b << fEdep;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdHit(void *p) {
      return  p ? new(p) ::CbmPsdHit : new ::CbmPsdHit;
   }
   static void *newArray_CbmPsdHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdHit[nElements] : new ::CbmPsdHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdHit(void *p) {
      delete ((::CbmPsdHit*)p);
   }
   static void deleteArray_CbmPsdHit(void *p) {
      delete [] ((::CbmPsdHit*)p);
   }
   static void destruct_CbmPsdHit(void *p) {
      typedef ::CbmPsdHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPsdHit(TBuffer &buf, void *obj) {
      ((::CbmPsdHit*)obj)->::CbmPsdHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPsdHit

//______________________________________________________________________________
void CbmPsdPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmPsdPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmPsdPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdPoint(void *p) {
      return  p ? new(p) ::CbmPsdPoint : new ::CbmPsdPoint;
   }
   static void *newArray_CbmPsdPoint(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdPoint[nElements] : new ::CbmPsdPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdPoint(void *p) {
      delete ((::CbmPsdPoint*)p);
   }
   static void deleteArray_CbmPsdPoint(void *p) {
      delete [] ((::CbmPsdPoint*)p);
   }
   static void destruct_CbmPsdPoint(void *p) {
      typedef ::CbmPsdPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmPsdPoint

//______________________________________________________________________________
void CbmPsdMCbmHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmPsdMCbmHit.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fuModuleId;
      R__b >> fuSectionId;
      R__b >> fdEdep;
      R__b >> fdTime;
      R__b.CheckByteCount(R__s, R__c, CbmPsdMCbmHit::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmPsdMCbmHit::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fuModuleId;
      R__b << fuSectionId;
      R__b << fdEdep;
      R__b << fdTime;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmPsdMCbmHit(void *p) {
      return  p ? new(p) ::CbmPsdMCbmHit : new ::CbmPsdMCbmHit;
   }
   static void *newArray_CbmPsdMCbmHit(Long_t nElements, void *p) {
      return p ? new(p) ::CbmPsdMCbmHit[nElements] : new ::CbmPsdMCbmHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmPsdMCbmHit(void *p) {
      delete ((::CbmPsdMCbmHit*)p);
   }
   static void deleteArray_CbmPsdMCbmHit(void *p) {
      delete [] ((::CbmPsdMCbmHit*)p);
   }
   static void destruct_CbmPsdMCbmHit(void *p) {
      typedef ::CbmPsdMCbmHit current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmPsdMCbmHit(TBuffer &buf, void *obj) {
      ((::CbmPsdMCbmHit*)obj)->::CbmPsdMCbmHit::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmPsdMCbmHit

//______________________________________________________________________________
void CbmTrackParam::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmTrackParam.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairTrackParam::Streamer(R__b);
      R__b >> fPx;
      R__b >> fPy;
      R__b >> fPz;
      R__b >> fDpx;
      R__b >> fDpy;
      R__b >> fDpz;
      R__b >> fTime;
      R__b >> fDTime;
      R__b.CheckByteCount(R__s, R__c, CbmTrackParam::IsA());
   } else {
      R__c = R__b.WriteVersion(CbmTrackParam::IsA(), kTRUE);
      FairTrackParam::Streamer(R__b);
      R__b << fPx;
      R__b << fPy;
      R__b << fPz;
      R__b << fDpx;
      R__b << fDpy;
      R__b << fDpz;
      R__b << fTime;
      R__b << fDTime;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmTrackParam(void *p) {
      return  p ? new(p) ::CbmTrackParam : new ::CbmTrackParam;
   }
   static void *newArray_CbmTrackParam(Long_t nElements, void *p) {
      return p ? new(p) ::CbmTrackParam[nElements] : new ::CbmTrackParam[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmTrackParam(void *p) {
      delete ((::CbmTrackParam*)p);
   }
   static void deleteArray_CbmTrackParam(void *p) {
      delete [] ((::CbmTrackParam*)p);
   }
   static void destruct_CbmTrackParam(void *p) {
      typedef ::CbmTrackParam current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CbmTrackParam(TBuffer &buf, void *obj) {
      ((::CbmTrackParam*)obj)->::CbmTrackParam::Streamer(buf);
   }
} // end of namespace ROOT for class ::CbmTrackParam

//______________________________________________________________________________
void CbmGlobalTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class CbmGlobalTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CbmGlobalTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(CbmGlobalTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CbmGlobalTrack(void *p) {
      return  p ? new(p) ::CbmGlobalTrack : new ::CbmGlobalTrack;
   }
   static void *newArray_CbmGlobalTrack(Long_t nElements, void *p) {
      return p ? new(p) ::CbmGlobalTrack[nElements] : new ::CbmGlobalTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_CbmGlobalTrack(void *p) {
      delete ((::CbmGlobalTrack*)p);
   }
   static void deleteArray_CbmGlobalTrack(void *p) {
      delete [] ((::CbmGlobalTrack*)p);
   }
   static void destruct_CbmGlobalTrack(void *p) {
      typedef ::CbmGlobalTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CbmGlobalTrack

//______________________________________________________________________________
void AccTimingEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class AccTimingEvent.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fulPlannedUTC;
      R__b >> fulPlannedTAI;
      R__b >> fulRawEvent;
      R__b >> fulRawParams;
      R__b >> fuRawTimingFlags;
      R__b >> fulExecutedUTC;
      R__b >> fulExecutedTAI;
      R__b.CheckByteCount(R__s, R__c, AccTimingEvent::IsA());
   } else {
      R__c = R__b.WriteVersion(AccTimingEvent::IsA(), kTRUE);
      R__b << fulPlannedUTC;
      R__b << fulPlannedTAI;
      R__b << fulRawEvent;
      R__b << fulRawParams;
      R__b << fuRawTimingFlags;
      R__b << fulExecutedUTC;
      R__b << fulExecutedTAI;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AccTimingEvent(void *p) {
      return  p ? new(p) ::AccTimingEvent : new ::AccTimingEvent;
   }
   static void *newArray_AccTimingEvent(Long_t nElements, void *p) {
      return p ? new(p) ::AccTimingEvent[nElements] : new ::AccTimingEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_AccTimingEvent(void *p) {
      delete ((::AccTimingEvent*)p);
   }
   static void deleteArray_AccTimingEvent(void *p) {
      delete [] ((::AccTimingEvent*)p);
   }
   static void destruct_AccTimingEvent(void *p) {
      typedef ::AccTimingEvent current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AccTimingEvent(TBuffer &buf, void *obj) {
      ((::AccTimingEvent*)obj)->::AccTimingEvent::Streamer(buf);
   }
} // end of namespace ROOT for class ::AccTimingEvent

//______________________________________________________________________________
void AccStatusTs::Streamer(TBuffer &R__b)
{
   // Stream an object of class AccStatusTs.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fuSpillIndexAtStart;
      fLastEvtBeforeTs.Streamer(R__b);
      {
         vector<AccTimingEvent> &R__stl =  fvEventsDuringTS;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            AccTimingEvent R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, AccStatusTs::IsA());
   } else {
      R__c = R__b.WriteVersion(AccStatusTs::IsA(), kTRUE);
      R__b << fuSpillIndexAtStart;
      fLastEvtBeforeTs.Streamer(R__b);
      {
         vector<AccTimingEvent> &R__stl =  fvEventsDuringTS;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<AccTimingEvent>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((AccTimingEvent&)(*R__k)).Streamer(R__b);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AccStatusTs(void *p) {
      return  p ? new(p) ::AccStatusTs : new ::AccStatusTs;
   }
   static void *newArray_AccStatusTs(Long_t nElements, void *p) {
      return p ? new(p) ::AccStatusTs[nElements] : new ::AccStatusTs[nElements];
   }
   // Wrapper around operator delete
   static void delete_AccStatusTs(void *p) {
      delete ((::AccStatusTs*)p);
   }
   static void deleteArray_AccStatusTs(void *p) {
      delete [] ((::AccStatusTs*)p);
   }
   static void destruct_AccStatusTs(void *p) {
      typedef ::AccStatusTs current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AccStatusTs(TBuffer &buf, void *obj) {
      ((::AccStatusTs*)obj)->::AccStatusTs::Streamer(buf);
   }
} // end of namespace ROOT for class ::AccStatusTs

namespace ROOT {
   // Wrappers around operator new
   static void *new_stsxytercLcLMessage(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::stsxyter::Message : new ::stsxyter::Message;
   }
   static void *newArray_stsxytercLcLMessage(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::stsxyter::Message[nElements] : new ::stsxyter::Message[nElements];
   }
   // Wrapper around operator delete
   static void delete_stsxytercLcLMessage(void *p) {
      delete ((::stsxyter::Message*)p);
   }
   static void deleteArray_stsxytercLcLMessage(void *p) {
      delete [] ((::stsxyter::Message*)p);
   }
   static void destruct_stsxytercLcLMessage(void *p) {
      typedef ::stsxyter::Message current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::stsxyter::Message

namespace ROOT {
   // Wrappers around operator new
   static void *new_gdpbv100cLcLMessage(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::gdpbv100::Message : new ::gdpbv100::Message;
   }
   static void *newArray_gdpbv100cLcLMessage(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::gdpbv100::Message[nElements] : new ::gdpbv100::Message[nElements];
   }
   // Wrapper around operator delete
   static void delete_gdpbv100cLcLMessage(void *p) {
      delete ((::gdpbv100::Message*)p);
   }
   static void deleteArray_gdpbv100cLcLMessage(void *p) {
      delete [] ((::gdpbv100::Message*)p);
   }
   static void destruct_gdpbv100cLcLMessage(void *p) {
      typedef ::gdpbv100::Message current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::gdpbv100::Message

namespace ROOT {
   // Wrappers around operator new
   static void *new_gdpbv100cLcLFullMessage(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::gdpbv100::FullMessage : new ::gdpbv100::FullMessage;
   }
   static void *newArray_gdpbv100cLcLFullMessage(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::gdpbv100::FullMessage[nElements] : new ::gdpbv100::FullMessage[nElements];
   }
   // Wrapper around operator delete
   static void delete_gdpbv100cLcLFullMessage(void *p) {
      delete ((::gdpbv100::FullMessage*)p);
   }
   static void deleteArray_gdpbv100cLcLFullMessage(void *p) {
      delete [] ((::gdpbv100::FullMessage*)p);
   }
   static void destruct_gdpbv100cLcLFullMessage(void *p) {
      typedef ::gdpbv100::FullMessage current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::gdpbv100::FullMessage

namespace ROOT {
   // Wrappers around operator new
   static void *new_critof001cLcLMessage(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::critof001::Message : new ::critof001::Message;
   }
   static void *newArray_critof001cLcLMessage(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::critof001::Message[nElements] : new ::critof001::Message[nElements];
   }
   // Wrapper around operator delete
   static void delete_critof001cLcLMessage(void *p) {
      delete ((::critof001::Message*)p);
   }
   static void deleteArray_critof001cLcLMessage(void *p) {
      delete [] ((::critof001::Message*)p);
   }
   static void destruct_critof001cLcLMessage(void *p) {
      typedef ::critof001::Message current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::critof001::Message

namespace ROOT {
   // Wrappers around operator new
   static void *new_critof001cLcLFullMessage(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::critof001::FullMessage : new ::critof001::FullMessage;
   }
   static void *newArray_critof001cLcLFullMessage(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::critof001::FullMessage[nElements] : new ::critof001::FullMessage[nElements];
   }
   // Wrapper around operator delete
   static void delete_critof001cLcLFullMessage(void *p) {
      delete ((::critof001::FullMessage*)p);
   }
   static void deleteArray_critof001cLcLFullMessage(void *p) {
      delete [] ((::critof001::FullMessage*)p);
   }
   static void destruct_critof001cLcLFullMessage(void *p) {
      typedef ::critof001::FullMessage current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::critof001::FullMessage

//______________________________________________________________________________
void TimesliceMetaData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TimesliceMetaData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fulStartTimeNs;
      R__b >> fulDurationNs;
      R__b >> fulOverlapNs;
      R__b >> fulIndex;
      R__b.CheckByteCount(R__s, R__c, TimesliceMetaData::IsA());
   } else {
      R__c = R__b.WriteVersion(TimesliceMetaData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fulStartTimeNs;
      R__b << fulDurationNs;
      R__b << fulOverlapNs;
      R__b << fulIndex;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TimesliceMetaData(void *p) {
      return  p ? new(p) ::TimesliceMetaData : new ::TimesliceMetaData;
   }
   static void *newArray_TimesliceMetaData(Long_t nElements, void *p) {
      return p ? new(p) ::TimesliceMetaData[nElements] : new ::TimesliceMetaData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TimesliceMetaData(void *p) {
      delete ((::TimesliceMetaData*)p);
   }
   static void deleteArray_TimesliceMetaData(void *p) {
      delete [] ((::TimesliceMetaData*)p);
   }
   static void destruct_TimesliceMetaData(void *p) {
      typedef ::TimesliceMetaData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TimesliceMetaData(TBuffer &buf, void *obj) {
      ((::TimesliceMetaData*)obj)->::TimesliceMetaData::Streamer(buf);
   }
} // end of namespace ROOT for class ::TimesliceMetaData

namespace ROOT {
   // Wrappers around operator new
   static void *new_PsdDataV000cLcLPsdGbtReader(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PsdDataV000::PsdGbtReader : new ::PsdDataV000::PsdGbtReader;
   }
   static void *newArray_PsdDataV000cLcLPsdGbtReader(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PsdDataV000::PsdGbtReader[nElements] : new ::PsdDataV000::PsdGbtReader[nElements];
   }
   // Wrapper around operator delete
   static void delete_PsdDataV000cLcLPsdGbtReader(void *p) {
      delete ((::PsdDataV000::PsdGbtReader*)p);
   }
   static void deleteArray_PsdDataV000cLcLPsdGbtReader(void *p) {
      delete [] ((::PsdDataV000::PsdGbtReader*)p);
   }
   static void destruct_PsdDataV000cLcLPsdGbtReader(void *p) {
      typedef ::PsdDataV000::PsdGbtReader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PsdDataV000::PsdGbtReader

namespace ROOT {
   // Wrappers around operator new
   static void *new_PsdDataV100cLcLPsdGbtReader(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PsdDataV100::PsdGbtReader : new ::PsdDataV100::PsdGbtReader;
   }
   static void *newArray_PsdDataV100cLcLPsdGbtReader(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::PsdDataV100::PsdGbtReader[nElements] : new ::PsdDataV100::PsdGbtReader[nElements];
   }
   // Wrapper around operator delete
   static void delete_PsdDataV100cLcLPsdGbtReader(void *p) {
      delete ((::PsdDataV100::PsdGbtReader*)p);
   }
   static void deleteArray_PsdDataV100cLcLPsdGbtReader(void *p) {
      delete [] ((::PsdDataV100::PsdGbtReader*)p);
   }
   static void destruct_PsdDataV100cLcLPsdGbtReader(void *p) {
      typedef ::PsdDataV100::PsdGbtReader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PsdDataV100::PsdGbtReader

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
   static TClass *vectorlEunsignedsPintgR_Dictionary();
   static void vectorlEunsignedsPintgR_TClassManip(TClass*);
   static void *new_vectorlEunsignedsPintgR(void *p = nullptr);
   static void *newArray_vectorlEunsignedsPintgR(Long_t size, void *p);
   static void delete_vectorlEunsignedsPintgR(void *p);
   static void deleteArray_vectorlEunsignedsPintgR(void *p);
   static void destruct_vectorlEunsignedsPintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<unsigned int>*)
   {
      vector<unsigned int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<unsigned int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<unsigned int>", -2, "vector", 389,
                  typeid(vector<unsigned int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEunsignedsPintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<unsigned int>) );
      instance.SetNew(&new_vectorlEunsignedsPintgR);
      instance.SetNewArray(&newArray_vectorlEunsignedsPintgR);
      instance.SetDelete(&delete_vectorlEunsignedsPintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEunsignedsPintgR);
      instance.SetDestructor(&destruct_vectorlEunsignedsPintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<unsigned int> >()));

      ::ROOT::AddClassAlternate("vector<unsigned int>","std::vector<unsigned int, std::allocator<unsigned int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<unsigned int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEunsignedsPintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<unsigned int>*)nullptr)->GetClass();
      vectorlEunsignedsPintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEunsignedsPintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEunsignedsPintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int> : new vector<unsigned int>;
   }
   static void *newArray_vectorlEunsignedsPintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<unsigned int>[nElements] : new vector<unsigned int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEunsignedsPintgR(void *p) {
      delete ((vector<unsigned int>*)p);
   }
   static void deleteArray_vectorlEunsignedsPintgR(void *p) {
      delete [] ((vector<unsigned int>*)p);
   }
   static void destruct_vectorlEunsignedsPintgR(void *p) {
      typedef vector<unsigned int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<unsigned int>

namespace ROOT {
   static TClass *vectorlEstsxytercLcLMessagegR_Dictionary();
   static void vectorlEstsxytercLcLMessagegR_TClassManip(TClass*);
   static void *new_vectorlEstsxytercLcLMessagegR(void *p = nullptr);
   static void *newArray_vectorlEstsxytercLcLMessagegR(Long_t size, void *p);
   static void delete_vectorlEstsxytercLcLMessagegR(void *p);
   static void deleteArray_vectorlEstsxytercLcLMessagegR(void *p);
   static void destruct_vectorlEstsxytercLcLMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<stsxyter::Message>*)
   {
      vector<stsxyter::Message> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<stsxyter::Message>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<stsxyter::Message>", -2, "vector", 389,
                  typeid(vector<stsxyter::Message>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstsxytercLcLMessagegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<stsxyter::Message>) );
      instance.SetNew(&new_vectorlEstsxytercLcLMessagegR);
      instance.SetNewArray(&newArray_vectorlEstsxytercLcLMessagegR);
      instance.SetDelete(&delete_vectorlEstsxytercLcLMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlEstsxytercLcLMessagegR);
      instance.SetDestructor(&destruct_vectorlEstsxytercLcLMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<stsxyter::Message> >()));

      ::ROOT::AddClassAlternate("vector<stsxyter::Message>","std::vector<stsxyter::Message, std::allocator<stsxyter::Message> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<stsxyter::Message>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstsxytercLcLMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<stsxyter::Message>*)nullptr)->GetClass();
      vectorlEstsxytercLcLMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstsxytercLcLMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstsxytercLcLMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<stsxyter::Message> : new vector<stsxyter::Message>;
   }
   static void *newArray_vectorlEstsxytercLcLMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<stsxyter::Message>[nElements] : new vector<stsxyter::Message>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstsxytercLcLMessagegR(void *p) {
      delete ((vector<stsxyter::Message>*)p);
   }
   static void deleteArray_vectorlEstsxytercLcLMessagegR(void *p) {
      delete [] ((vector<stsxyter::Message>*)p);
   }
   static void destruct_vectorlEstsxytercLcLMessagegR(void *p) {
      typedef vector<stsxyter::Message> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<stsxyter::Message>

namespace ROOT {
   static TClass *vectorlEshortgR_Dictionary();
   static void vectorlEshortgR_TClassManip(TClass*);
   static void *new_vectorlEshortgR(void *p = nullptr);
   static void *newArray_vectorlEshortgR(Long_t size, void *p);
   static void delete_vectorlEshortgR(void *p);
   static void deleteArray_vectorlEshortgR(void *p);
   static void destruct_vectorlEshortgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<short>*)
   {
      vector<short> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<short>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<short>", -2, "vector", 389,
                  typeid(vector<short>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEshortgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<short>) );
      instance.SetNew(&new_vectorlEshortgR);
      instance.SetNewArray(&newArray_vectorlEshortgR);
      instance.SetDelete(&delete_vectorlEshortgR);
      instance.SetDeleteArray(&deleteArray_vectorlEshortgR);
      instance.SetDestructor(&destruct_vectorlEshortgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<short> >()));

      ::ROOT::AddClassAlternate("vector<short>","std::vector<short, std::allocator<short> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<short>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEshortgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<short>*)nullptr)->GetClass();
      vectorlEshortgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEshortgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEshortgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short> : new vector<short>;
   }
   static void *newArray_vectorlEshortgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<short>[nElements] : new vector<short>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEshortgR(void *p) {
      delete ((vector<short>*)p);
   }
   static void deleteArray_vectorlEshortgR(void *p) {
      delete [] ((vector<short>*)p);
   }
   static void destruct_vectorlEshortgR(void *p) {
      typedef vector<short> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<short>

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
   static TClass *vectorlEgdpbv100cLcLMessagegR_Dictionary();
   static void vectorlEgdpbv100cLcLMessagegR_TClassManip(TClass*);
   static void *new_vectorlEgdpbv100cLcLMessagegR(void *p = nullptr);
   static void *newArray_vectorlEgdpbv100cLcLMessagegR(Long_t size, void *p);
   static void delete_vectorlEgdpbv100cLcLMessagegR(void *p);
   static void deleteArray_vectorlEgdpbv100cLcLMessagegR(void *p);
   static void destruct_vectorlEgdpbv100cLcLMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<gdpbv100::Message>*)
   {
      vector<gdpbv100::Message> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<gdpbv100::Message>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<gdpbv100::Message>", -2, "vector", 389,
                  typeid(vector<gdpbv100::Message>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgdpbv100cLcLMessagegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<gdpbv100::Message>) );
      instance.SetNew(&new_vectorlEgdpbv100cLcLMessagegR);
      instance.SetNewArray(&newArray_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDelete(&delete_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlEgdpbv100cLcLMessagegR);
      instance.SetDestructor(&destruct_vectorlEgdpbv100cLcLMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<gdpbv100::Message> >()));

      ::ROOT::AddClassAlternate("vector<gdpbv100::Message>","std::vector<gdpbv100::Message, std::allocator<gdpbv100::Message> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<gdpbv100::Message>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgdpbv100cLcLMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<gdpbv100::Message>*)nullptr)->GetClass();
      vectorlEgdpbv100cLcLMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgdpbv100cLcLMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgdpbv100cLcLMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::Message> : new vector<gdpbv100::Message>;
   }
   static void *newArray_vectorlEgdpbv100cLcLMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<gdpbv100::Message>[nElements] : new vector<gdpbv100::Message>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgdpbv100cLcLMessagegR(void *p) {
      delete ((vector<gdpbv100::Message>*)p);
   }
   static void deleteArray_vectorlEgdpbv100cLcLMessagegR(void *p) {
      delete [] ((vector<gdpbv100::Message>*)p);
   }
   static void destruct_vectorlEgdpbv100cLcLMessagegR(void *p) {
      typedef vector<gdpbv100::Message> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<gdpbv100::Message>

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
   static TClass *vectorlEHitTypegR_Dictionary();
   static void vectorlEHitTypegR_TClassManip(TClass*);
   static void *new_vectorlEHitTypegR(void *p = nullptr);
   static void *newArray_vectorlEHitTypegR(Long_t size, void *p);
   static void delete_vectorlEHitTypegR(void *p);
   static void deleteArray_vectorlEHitTypegR(void *p);
   static void destruct_vectorlEHitTypegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<HitType>*)
   {
      vector<HitType> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<HitType>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<HitType>", -2, "vector", 389,
                  typeid(vector<HitType>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEHitTypegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<HitType>) );
      instance.SetNew(&new_vectorlEHitTypegR);
      instance.SetNewArray(&newArray_vectorlEHitTypegR);
      instance.SetDelete(&delete_vectorlEHitTypegR);
      instance.SetDeleteArray(&deleteArray_vectorlEHitTypegR);
      instance.SetDestructor(&destruct_vectorlEHitTypegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<HitType> >()));

      ::ROOT::AddClassAlternate("vector<HitType>","std::vector<HitType, std::allocator<HitType> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<HitType>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEHitTypegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<HitType>*)nullptr)->GetClass();
      vectorlEHitTypegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEHitTypegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEHitTypegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HitType> : new vector<HitType>;
   }
   static void *newArray_vectorlEHitTypegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<HitType>[nElements] : new vector<HitType>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEHitTypegR(void *p) {
      delete ((vector<HitType>*)p);
   }
   static void deleteArray_vectorlEHitTypegR(void *p) {
      delete [] ((vector<HitType>*)p);
   }
   static void destruct_vectorlEHitTypegR(void *p) {
      typedef vector<HitType> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<HitType>

namespace ROOT {
   static TClass *vectorlECbmTrdRawMessageSpadicgR_Dictionary();
   static void vectorlECbmTrdRawMessageSpadicgR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdRawMessageSpadicgR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdRawMessageSpadicgR(Long_t size, void *p);
   static void delete_vectorlECbmTrdRawMessageSpadicgR(void *p);
   static void deleteArray_vectorlECbmTrdRawMessageSpadicgR(void *p);
   static void destruct_vectorlECbmTrdRawMessageSpadicgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdRawMessageSpadic>*)
   {
      vector<CbmTrdRawMessageSpadic> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdRawMessageSpadic>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdRawMessageSpadic>", -2, "vector", 389,
                  typeid(vector<CbmTrdRawMessageSpadic>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdRawMessageSpadicgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmTrdRawMessageSpadic>) );
      instance.SetNew(&new_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetNewArray(&newArray_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetDelete(&delete_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdRawMessageSpadicgR);
      instance.SetDestructor(&destruct_vectorlECbmTrdRawMessageSpadicgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdRawMessageSpadic> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdRawMessageSpadic>","std::vector<CbmTrdRawMessageSpadic, std::allocator<CbmTrdRawMessageSpadic> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdRawMessageSpadic>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdRawMessageSpadicgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdRawMessageSpadic>*)nullptr)->GetClass();
      vectorlECbmTrdRawMessageSpadicgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdRawMessageSpadicgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdRawMessageSpadic> : new vector<CbmTrdRawMessageSpadic>;
   }
   static void *newArray_vectorlECbmTrdRawMessageSpadicgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdRawMessageSpadic>[nElements] : new vector<CbmTrdRawMessageSpadic>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      delete ((vector<CbmTrdRawMessageSpadic>*)p);
   }
   static void deleteArray_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      delete [] ((vector<CbmTrdRawMessageSpadic>*)p);
   }
   static void destruct_vectorlECbmTrdRawMessageSpadicgR(void *p) {
      typedef vector<CbmTrdRawMessageSpadic> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdRawMessageSpadic>

namespace ROOT {
   static TClass *vectorlECbmTrdDigigR_Dictionary();
   static void vectorlECbmTrdDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmTrdDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmTrdDigigR(Long_t size, void *p);
   static void delete_vectorlECbmTrdDigigR(void *p);
   static void deleteArray_vectorlECbmTrdDigigR(void *p);
   static void destruct_vectorlECbmTrdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTrdDigi>*)
   {
      vector<CbmTrdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTrdDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTrdDigi>", -2, "vector", 389,
                  typeid(vector<CbmTrdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTrdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmTrdDigi>) );
      instance.SetNew(&new_vectorlECbmTrdDigigR);
      instance.SetNewArray(&newArray_vectorlECbmTrdDigigR);
      instance.SetDelete(&delete_vectorlECbmTrdDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTrdDigigR);
      instance.SetDestructor(&destruct_vectorlECbmTrdDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTrdDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmTrdDigi>","std::vector<CbmTrdDigi, std::allocator<CbmTrdDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTrdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTrdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTrdDigi>*)nullptr)->GetClass();
      vectorlECbmTrdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTrdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTrdDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdDigi> : new vector<CbmTrdDigi>;
   }
   static void *newArray_vectorlECbmTrdDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTrdDigi>[nElements] : new vector<CbmTrdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTrdDigigR(void *p) {
      delete ((vector<CbmTrdDigi>*)p);
   }
   static void deleteArray_vectorlECbmTrdDigigR(void *p) {
      delete [] ((vector<CbmTrdDigi>*)p);
   }
   static void destruct_vectorlECbmTrdDigigR(void *p) {
      typedef vector<CbmTrdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTrdDigi>

namespace ROOT {
   static TClass *vectorlECbmTofHitgR_Dictionary();
   static void vectorlECbmTofHitgR_TClassManip(TClass*);
   static void *new_vectorlECbmTofHitgR(void *p = nullptr);
   static void *newArray_vectorlECbmTofHitgR(Long_t size, void *p);
   static void delete_vectorlECbmTofHitgR(void *p);
   static void deleteArray_vectorlECbmTofHitgR(void *p);
   static void destruct_vectorlECbmTofHitgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofHit>*)
   {
      vector<CbmTofHit> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofHit>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofHit>", -2, "vector", 389,
                  typeid(vector<CbmTofHit>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofHitgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmTofHit>) );
      instance.SetNew(&new_vectorlECbmTofHitgR);
      instance.SetNewArray(&newArray_vectorlECbmTofHitgR);
      instance.SetDelete(&delete_vectorlECbmTofHitgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofHitgR);
      instance.SetDestructor(&destruct_vectorlECbmTofHitgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofHit> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofHit>","std::vector<CbmTofHit, std::allocator<CbmTofHit> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofHit>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofHitgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofHit>*)nullptr)->GetClass();
      vectorlECbmTofHitgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofHitgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofHitgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofHit> : new vector<CbmTofHit>;
   }
   static void *newArray_vectorlECbmTofHitgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofHit>[nElements] : new vector<CbmTofHit>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofHitgR(void *p) {
      delete ((vector<CbmTofHit>*)p);
   }
   static void deleteArray_vectorlECbmTofHitgR(void *p) {
      delete [] ((vector<CbmTofHit>*)p);
   }
   static void destruct_vectorlECbmTofHitgR(void *p) {
      typedef vector<CbmTofHit> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofHit>

namespace ROOT {
   static TClass *vectorlECbmTofDigigR_Dictionary();
   static void vectorlECbmTofDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmTofDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmTofDigigR(Long_t size, void *p);
   static void delete_vectorlECbmTofDigigR(void *p);
   static void deleteArray_vectorlECbmTofDigigR(void *p);
   static void destruct_vectorlECbmTofDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmTofDigi>*)
   {
      vector<CbmTofDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmTofDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmTofDigi>", -2, "vector", 389,
                  typeid(vector<CbmTofDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmTofDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmTofDigi>) );
      instance.SetNew(&new_vectorlECbmTofDigigR);
      instance.SetNewArray(&newArray_vectorlECbmTofDigigR);
      instance.SetDelete(&delete_vectorlECbmTofDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmTofDigigR);
      instance.SetDestructor(&destruct_vectorlECbmTofDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmTofDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmTofDigi>","std::vector<CbmTofDigi, std::allocator<CbmTofDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmTofDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmTofDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmTofDigi>*)nullptr)->GetClass();
      vectorlECbmTofDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmTofDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmTofDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofDigi> : new vector<CbmTofDigi>;
   }
   static void *newArray_vectorlECbmTofDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmTofDigi>[nElements] : new vector<CbmTofDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmTofDigigR(void *p) {
      delete ((vector<CbmTofDigi>*)p);
   }
   static void deleteArray_vectorlECbmTofDigigR(void *p) {
      delete [] ((vector<CbmTofDigi>*)p);
   }
   static void destruct_vectorlECbmTofDigigR(void *p) {
      typedef vector<CbmTofDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmTofDigi>

namespace ROOT {
   static TClass *vectorlECbmStsDigigR_Dictionary();
   static void vectorlECbmStsDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmStsDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmStsDigigR(Long_t size, void *p);
   static void delete_vectorlECbmStsDigigR(void *p);
   static void deleteArray_vectorlECbmStsDigigR(void *p);
   static void destruct_vectorlECbmStsDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmStsDigi>*)
   {
      vector<CbmStsDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmStsDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmStsDigi>", -2, "vector", 389,
                  typeid(vector<CbmStsDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmStsDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmStsDigi>) );
      instance.SetNew(&new_vectorlECbmStsDigigR);
      instance.SetNewArray(&newArray_vectorlECbmStsDigigR);
      instance.SetDelete(&delete_vectorlECbmStsDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmStsDigigR);
      instance.SetDestructor(&destruct_vectorlECbmStsDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmStsDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmStsDigi>","std::vector<CbmStsDigi, std::allocator<CbmStsDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmStsDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmStsDigi>*)nullptr)->GetClass();
      vectorlECbmStsDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmStsDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmStsDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi> : new vector<CbmStsDigi>;
   }
   static void *newArray_vectorlECbmStsDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmStsDigi>[nElements] : new vector<CbmStsDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmStsDigigR(void *p) {
      delete ((vector<CbmStsDigi>*)p);
   }
   static void deleteArray_vectorlECbmStsDigigR(void *p) {
      delete [] ((vector<CbmStsDigi>*)p);
   }
   static void destruct_vectorlECbmStsDigigR(void *p) {
      typedef vector<CbmStsDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmStsDigi>

namespace ROOT {
   static TClass *vectorlECbmRichDigigR_Dictionary();
   static void vectorlECbmRichDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmRichDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmRichDigigR(Long_t size, void *p);
   static void delete_vectorlECbmRichDigigR(void *p);
   static void deleteArray_vectorlECbmRichDigigR(void *p);
   static void destruct_vectorlECbmRichDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmRichDigi>*)
   {
      vector<CbmRichDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmRichDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmRichDigi>", -2, "vector", 389,
                  typeid(vector<CbmRichDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmRichDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmRichDigi>) );
      instance.SetNew(&new_vectorlECbmRichDigigR);
      instance.SetNewArray(&newArray_vectorlECbmRichDigigR);
      instance.SetDelete(&delete_vectorlECbmRichDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmRichDigigR);
      instance.SetDestructor(&destruct_vectorlECbmRichDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmRichDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmRichDigi>","std::vector<CbmRichDigi, std::allocator<CbmRichDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmRichDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmRichDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmRichDigi>*)nullptr)->GetClass();
      vectorlECbmRichDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmRichDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmRichDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichDigi> : new vector<CbmRichDigi>;
   }
   static void *newArray_vectorlECbmRichDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmRichDigi>[nElements] : new vector<CbmRichDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmRichDigigR(void *p) {
      delete ((vector<CbmRichDigi>*)p);
   }
   static void deleteArray_vectorlECbmRichDigigR(void *p) {
      delete [] ((vector<CbmRichDigi>*)p);
   }
   static void destruct_vectorlECbmRichDigigR(void *p) {
      typedef vector<CbmRichDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmRichDigi>

namespace ROOT {
   static TClass *vectorlECbmPsdDspgR_Dictionary();
   static void vectorlECbmPsdDspgR_TClassManip(TClass*);
   static void *new_vectorlECbmPsdDspgR(void *p = nullptr);
   static void *newArray_vectorlECbmPsdDspgR(Long_t size, void *p);
   static void delete_vectorlECbmPsdDspgR(void *p);
   static void deleteArray_vectorlECbmPsdDspgR(void *p);
   static void destruct_vectorlECbmPsdDspgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmPsdDsp>*)
   {
      vector<CbmPsdDsp> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmPsdDsp>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmPsdDsp>", -2, "vector", 389,
                  typeid(vector<CbmPsdDsp>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmPsdDspgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmPsdDsp>) );
      instance.SetNew(&new_vectorlECbmPsdDspgR);
      instance.SetNewArray(&newArray_vectorlECbmPsdDspgR);
      instance.SetDelete(&delete_vectorlECbmPsdDspgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmPsdDspgR);
      instance.SetDestructor(&destruct_vectorlECbmPsdDspgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmPsdDsp> >()));

      ::ROOT::AddClassAlternate("vector<CbmPsdDsp>","std::vector<CbmPsdDsp, std::allocator<CbmPsdDsp> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmPsdDsp>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmPsdDspgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmPsdDsp>*)nullptr)->GetClass();
      vectorlECbmPsdDspgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmPsdDspgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmPsdDspgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDsp> : new vector<CbmPsdDsp>;
   }
   static void *newArray_vectorlECbmPsdDspgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDsp>[nElements] : new vector<CbmPsdDsp>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmPsdDspgR(void *p) {
      delete ((vector<CbmPsdDsp>*)p);
   }
   static void deleteArray_vectorlECbmPsdDspgR(void *p) {
      delete [] ((vector<CbmPsdDsp>*)p);
   }
   static void destruct_vectorlECbmPsdDspgR(void *p) {
      typedef vector<CbmPsdDsp> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmPsdDsp>

namespace ROOT {
   static TClass *vectorlECbmPsdDigigR_Dictionary();
   static void vectorlECbmPsdDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmPsdDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmPsdDigigR(Long_t size, void *p);
   static void delete_vectorlECbmPsdDigigR(void *p);
   static void deleteArray_vectorlECbmPsdDigigR(void *p);
   static void destruct_vectorlECbmPsdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmPsdDigi>*)
   {
      vector<CbmPsdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmPsdDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmPsdDigi>", -2, "vector", 389,
                  typeid(vector<CbmPsdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmPsdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmPsdDigi>) );
      instance.SetNew(&new_vectorlECbmPsdDigigR);
      instance.SetNewArray(&newArray_vectorlECbmPsdDigigR);
      instance.SetDelete(&delete_vectorlECbmPsdDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmPsdDigigR);
      instance.SetDestructor(&destruct_vectorlECbmPsdDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmPsdDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmPsdDigi>","std::vector<CbmPsdDigi, std::allocator<CbmPsdDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmPsdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmPsdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmPsdDigi>*)nullptr)->GetClass();
      vectorlECbmPsdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmPsdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmPsdDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDigi> : new vector<CbmPsdDigi>;
   }
   static void *newArray_vectorlECbmPsdDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmPsdDigi>[nElements] : new vector<CbmPsdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmPsdDigigR(void *p) {
      delete ((vector<CbmPsdDigi>*)p);
   }
   static void deleteArray_vectorlECbmPsdDigigR(void *p) {
      delete [] ((vector<CbmPsdDigi>*)p);
   }
   static void destruct_vectorlECbmPsdDigigR(void *p) {
      typedef vector<CbmPsdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmPsdDigi>

namespace ROOT {
   static TClass *vectorlECbmMvdDigigR_Dictionary();
   static void vectorlECbmMvdDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmMvdDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmMvdDigigR(Long_t size, void *p);
   static void delete_vectorlECbmMvdDigigR(void *p);
   static void deleteArray_vectorlECbmMvdDigigR(void *p);
   static void destruct_vectorlECbmMvdDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMvdDigi>*)
   {
      vector<CbmMvdDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMvdDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMvdDigi>", -2, "vector", 389,
                  typeid(vector<CbmMvdDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMvdDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmMvdDigi>) );
      instance.SetNew(&new_vectorlECbmMvdDigigR);
      instance.SetNewArray(&newArray_vectorlECbmMvdDigigR);
      instance.SetDelete(&delete_vectorlECbmMvdDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMvdDigigR);
      instance.SetDestructor(&destruct_vectorlECbmMvdDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMvdDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmMvdDigi>","std::vector<CbmMvdDigi, std::allocator<CbmMvdDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMvdDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMvdDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMvdDigi>*)nullptr)->GetClass();
      vectorlECbmMvdDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMvdDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMvdDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdDigi> : new vector<CbmMvdDigi>;
   }
   static void *newArray_vectorlECbmMvdDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMvdDigi>[nElements] : new vector<CbmMvdDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMvdDigigR(void *p) {
      delete ((vector<CbmMvdDigi>*)p);
   }
   static void deleteArray_vectorlECbmMvdDigigR(void *p) {
      delete [] ((vector<CbmMvdDigi>*)p);
   }
   static void destruct_vectorlECbmMvdDigigR(void *p) {
      typedef vector<CbmMvdDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMvdDigi>

namespace ROOT {
   static TClass *vectorlECbmMuchDigigR_Dictionary();
   static void vectorlECbmMuchDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchDigigR(Long_t size, void *p);
   static void delete_vectorlECbmMuchDigigR(void *p);
   static void deleteArray_vectorlECbmMuchDigigR(void *p);
   static void destruct_vectorlECbmMuchDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchDigi>*)
   {
      vector<CbmMuchDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchDigi>", -2, "vector", 389,
                  typeid(vector<CbmMuchDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmMuchDigi>) );
      instance.SetNew(&new_vectorlECbmMuchDigigR);
      instance.SetNewArray(&newArray_vectorlECbmMuchDigigR);
      instance.SetDelete(&delete_vectorlECbmMuchDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchDigigR);
      instance.SetDestructor(&destruct_vectorlECbmMuchDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchDigi>","std::vector<CbmMuchDigi, std::allocator<CbmMuchDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchDigi>*)nullptr)->GetClass();
      vectorlECbmMuchDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchDigi> : new vector<CbmMuchDigi>;
   }
   static void *newArray_vectorlECbmMuchDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchDigi>[nElements] : new vector<CbmMuchDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchDigigR(void *p) {
      delete ((vector<CbmMuchDigi>*)p);
   }
   static void deleteArray_vectorlECbmMuchDigigR(void *p) {
      delete [] ((vector<CbmMuchDigi>*)p);
   }
   static void destruct_vectorlECbmMuchDigigR(void *p) {
      typedef vector<CbmMuchDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchDigi>

namespace ROOT {
   static TClass *vectorlECbmMuchBeamTimeDigigR_Dictionary();
   static void vectorlECbmMuchBeamTimeDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmMuchBeamTimeDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmMuchBeamTimeDigigR(Long_t size, void *p);
   static void delete_vectorlECbmMuchBeamTimeDigigR(void *p);
   static void deleteArray_vectorlECbmMuchBeamTimeDigigR(void *p);
   static void destruct_vectorlECbmMuchBeamTimeDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMuchBeamTimeDigi>*)
   {
      vector<CbmMuchBeamTimeDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMuchBeamTimeDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMuchBeamTimeDigi>", -2, "vector", 389,
                  typeid(vector<CbmMuchBeamTimeDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMuchBeamTimeDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmMuchBeamTimeDigi>) );
      instance.SetNew(&new_vectorlECbmMuchBeamTimeDigigR);
      instance.SetNewArray(&newArray_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDelete(&delete_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMuchBeamTimeDigigR);
      instance.SetDestructor(&destruct_vectorlECbmMuchBeamTimeDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMuchBeamTimeDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmMuchBeamTimeDigi>","std::vector<CbmMuchBeamTimeDigi, std::allocator<CbmMuchBeamTimeDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMuchBeamTimeDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMuchBeamTimeDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMuchBeamTimeDigi>*)nullptr)->GetClass();
      vectorlECbmMuchBeamTimeDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMuchBeamTimeDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMuchBeamTimeDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchBeamTimeDigi> : new vector<CbmMuchBeamTimeDigi>;
   }
   static void *newArray_vectorlECbmMuchBeamTimeDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMuchBeamTimeDigi>[nElements] : new vector<CbmMuchBeamTimeDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMuchBeamTimeDigigR(void *p) {
      delete ((vector<CbmMuchBeamTimeDigi>*)p);
   }
   static void deleteArray_vectorlECbmMuchBeamTimeDigigR(void *p) {
      delete [] ((vector<CbmMuchBeamTimeDigi>*)p);
   }
   static void destruct_vectorlECbmMuchBeamTimeDigigR(void *p) {
      typedef vector<CbmMuchBeamTimeDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMuchBeamTimeDigi>

namespace ROOT {
   static TClass *vectorlECbmMatchgR_Dictionary();
   static void vectorlECbmMatchgR_TClassManip(TClass*);
   static void *new_vectorlECbmMatchgR(void *p = nullptr);
   static void *newArray_vectorlECbmMatchgR(Long_t size, void *p);
   static void delete_vectorlECbmMatchgR(void *p);
   static void deleteArray_vectorlECbmMatchgR(void *p);
   static void destruct_vectorlECbmMatchgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMatch>*)
   {
      vector<CbmMatch> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMatch>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMatch>", -2, "vector", 389,
                  typeid(vector<CbmMatch>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMatchgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmMatch>) );
      instance.SetNew(&new_vectorlECbmMatchgR);
      instance.SetNewArray(&newArray_vectorlECbmMatchgR);
      instance.SetDelete(&delete_vectorlECbmMatchgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMatchgR);
      instance.SetDestructor(&destruct_vectorlECbmMatchgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMatch> >()));

      ::ROOT::AddClassAlternate("vector<CbmMatch>","std::vector<CbmMatch, std::allocator<CbmMatch> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMatch>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMatchgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMatch>*)nullptr)->GetClass();
      vectorlECbmMatchgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMatchgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMatchgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMatch> : new vector<CbmMatch>;
   }
   static void *newArray_vectorlECbmMatchgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMatch>[nElements] : new vector<CbmMatch>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMatchgR(void *p) {
      delete ((vector<CbmMatch>*)p);
   }
   static void deleteArray_vectorlECbmMatchgR(void *p) {
      delete [] ((vector<CbmMatch>*)p);
   }
   static void destruct_vectorlECbmMatchgR(void *p) {
      typedef vector<CbmMatch> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMatch>

namespace ROOT {
   static TClass *vectorlECbmMCEventInfogR_Dictionary();
   static void vectorlECbmMCEventInfogR_TClassManip(TClass*);
   static void *new_vectorlECbmMCEventInfogR(void *p = nullptr);
   static void *newArray_vectorlECbmMCEventInfogR(Long_t size, void *p);
   static void delete_vectorlECbmMCEventInfogR(void *p);
   static void deleteArray_vectorlECbmMCEventInfogR(void *p);
   static void destruct_vectorlECbmMCEventInfogR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmMCEventInfo>*)
   {
      vector<CbmMCEventInfo> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmMCEventInfo>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmMCEventInfo>", -2, "vector", 389,
                  typeid(vector<CbmMCEventInfo>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmMCEventInfogR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmMCEventInfo>) );
      instance.SetNew(&new_vectorlECbmMCEventInfogR);
      instance.SetNewArray(&newArray_vectorlECbmMCEventInfogR);
      instance.SetDelete(&delete_vectorlECbmMCEventInfogR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmMCEventInfogR);
      instance.SetDestructor(&destruct_vectorlECbmMCEventInfogR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmMCEventInfo> >()));

      ::ROOT::AddClassAlternate("vector<CbmMCEventInfo>","std::vector<CbmMCEventInfo, std::allocator<CbmMCEventInfo> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmMCEventInfo>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmMCEventInfogR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmMCEventInfo>*)nullptr)->GetClass();
      vectorlECbmMCEventInfogR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmMCEventInfogR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmMCEventInfogR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCEventInfo> : new vector<CbmMCEventInfo>;
   }
   static void *newArray_vectorlECbmMCEventInfogR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmMCEventInfo>[nElements] : new vector<CbmMCEventInfo>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmMCEventInfogR(void *p) {
      delete ((vector<CbmMCEventInfo>*)p);
   }
   static void deleteArray_vectorlECbmMCEventInfogR(void *p) {
      delete [] ((vector<CbmMCEventInfo>*)p);
   }
   static void destruct_vectorlECbmMCEventInfogR(void *p) {
      typedef vector<CbmMCEventInfo> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmMCEventInfo>

namespace ROOT {
   static TClass *vectorlECbmLinkgR_Dictionary();
   static void vectorlECbmLinkgR_TClassManip(TClass*);
   static void *new_vectorlECbmLinkgR(void *p = nullptr);
   static void *newArray_vectorlECbmLinkgR(Long_t size, void *p);
   static void delete_vectorlECbmLinkgR(void *p);
   static void deleteArray_vectorlECbmLinkgR(void *p);
   static void destruct_vectorlECbmLinkgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmLink>*)
   {
      vector<CbmLink> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmLink>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmLink>", -2, "vector", 389,
                  typeid(vector<CbmLink>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmLinkgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<CbmLink>) );
      instance.SetNew(&new_vectorlECbmLinkgR);
      instance.SetNewArray(&newArray_vectorlECbmLinkgR);
      instance.SetDelete(&delete_vectorlECbmLinkgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmLinkgR);
      instance.SetDestructor(&destruct_vectorlECbmLinkgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmLink> >()));

      ::ROOT::AddClassAlternate("vector<CbmLink>","std::vector<CbmLink, std::allocator<CbmLink> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmLink>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmLinkgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmLink>*)nullptr)->GetClass();
      vectorlECbmLinkgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmLinkgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmLinkgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLink> : new vector<CbmLink>;
   }
   static void *newArray_vectorlECbmLinkgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmLink>[nElements] : new vector<CbmLink>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmLinkgR(void *p) {
      delete ((vector<CbmLink>*)p);
   }
   static void deleteArray_vectorlECbmLinkgR(void *p) {
      delete [] ((vector<CbmLink>*)p);
   }
   static void destruct_vectorlECbmLinkgR(void *p) {
      typedef vector<CbmLink> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmLink>

namespace ROOT {
   static TClass *vectorlECbmEventStoregR_Dictionary();
   static void vectorlECbmEventStoregR_TClassManip(TClass*);
   static void *new_vectorlECbmEventStoregR(void *p = nullptr);
   static void *newArray_vectorlECbmEventStoregR(Long_t size, void *p);
   static void delete_vectorlECbmEventStoregR(void *p);
   static void deleteArray_vectorlECbmEventStoregR(void *p);
   static void destruct_vectorlECbmEventStoregR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmEventStore>*)
   {
      vector<CbmEventStore> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmEventStore>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmEventStore>", -2, "vector", 389,
                  typeid(vector<CbmEventStore>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmEventStoregR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmEventStore>) );
      instance.SetNew(&new_vectorlECbmEventStoregR);
      instance.SetNewArray(&newArray_vectorlECbmEventStoregR);
      instance.SetDelete(&delete_vectorlECbmEventStoregR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmEventStoregR);
      instance.SetDestructor(&destruct_vectorlECbmEventStoregR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmEventStore> >()));

      ::ROOT::AddClassAlternate("vector<CbmEventStore>","std::vector<CbmEventStore, std::allocator<CbmEventStore> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmEventStore>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmEventStoregR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmEventStore>*)nullptr)->GetClass();
      vectorlECbmEventStoregR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmEventStoregR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmEventStoregR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmEventStore> : new vector<CbmEventStore>;
   }
   static void *newArray_vectorlECbmEventStoregR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmEventStore>[nElements] : new vector<CbmEventStore>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmEventStoregR(void *p) {
      delete ((vector<CbmEventStore>*)p);
   }
   static void deleteArray_vectorlECbmEventStoregR(void *p) {
      delete [] ((vector<CbmEventStore>*)p);
   }
   static void destruct_vectorlECbmEventStoregR(void *p) {
      typedef vector<CbmEventStore> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmEventStore>

namespace ROOT {
   static TClass *vectorlECbmEventgR_Dictionary();
   static void vectorlECbmEventgR_TClassManip(TClass*);
   static void *new_vectorlECbmEventgR(void *p = nullptr);
   static void *newArray_vectorlECbmEventgR(Long_t size, void *p);
   static void delete_vectorlECbmEventgR(void *p);
   static void deleteArray_vectorlECbmEventgR(void *p);
   static void destruct_vectorlECbmEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmEvent>*)
   {
      vector<CbmEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmEvent>", -2, "vector", 389,
                  typeid(vector<CbmEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmEventgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmEvent>) );
      instance.SetNew(&new_vectorlECbmEventgR);
      instance.SetNewArray(&newArray_vectorlECbmEventgR);
      instance.SetDelete(&delete_vectorlECbmEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmEventgR);
      instance.SetDestructor(&destruct_vectorlECbmEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmEvent> >()));

      ::ROOT::AddClassAlternate("vector<CbmEvent>","std::vector<CbmEvent, std::allocator<CbmEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmEvent>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmEvent>*)nullptr)->GetClass();
      vectorlECbmEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmEvent> : new vector<CbmEvent>;
   }
   static void *newArray_vectorlECbmEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmEvent>[nElements] : new vector<CbmEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmEventgR(void *p) {
      delete ((vector<CbmEvent>*)p);
   }
   static void deleteArray_vectorlECbmEventgR(void *p) {
      delete [] ((vector<CbmEvent>*)p);
   }
   static void destruct_vectorlECbmEventgR(void *p) {
      typedef vector<CbmEvent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmEvent>

namespace ROOT {
   static TClass *vectorlECbmErrorMessagegR_Dictionary();
   static void vectorlECbmErrorMessagegR_TClassManip(TClass*);
   static void *new_vectorlECbmErrorMessagegR(void *p = nullptr);
   static void *newArray_vectorlECbmErrorMessagegR(Long_t size, void *p);
   static void delete_vectorlECbmErrorMessagegR(void *p);
   static void deleteArray_vectorlECbmErrorMessagegR(void *p);
   static void destruct_vectorlECbmErrorMessagegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmErrorMessage>*)
   {
      vector<CbmErrorMessage> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmErrorMessage>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmErrorMessage>", -2, "vector", 389,
                  typeid(vector<CbmErrorMessage>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmErrorMessagegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmErrorMessage>) );
      instance.SetNew(&new_vectorlECbmErrorMessagegR);
      instance.SetNewArray(&newArray_vectorlECbmErrorMessagegR);
      instance.SetDelete(&delete_vectorlECbmErrorMessagegR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmErrorMessagegR);
      instance.SetDestructor(&destruct_vectorlECbmErrorMessagegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmErrorMessage> >()));

      ::ROOT::AddClassAlternate("vector<CbmErrorMessage>","std::vector<CbmErrorMessage, std::allocator<CbmErrorMessage> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmErrorMessage>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmErrorMessagegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmErrorMessage>*)nullptr)->GetClass();
      vectorlECbmErrorMessagegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmErrorMessagegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmErrorMessagegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmErrorMessage> : new vector<CbmErrorMessage>;
   }
   static void *newArray_vectorlECbmErrorMessagegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmErrorMessage>[nElements] : new vector<CbmErrorMessage>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmErrorMessagegR(void *p) {
      delete ((vector<CbmErrorMessage>*)p);
   }
   static void deleteArray_vectorlECbmErrorMessagegR(void *p) {
      delete [] ((vector<CbmErrorMessage>*)p);
   }
   static void destruct_vectorlECbmErrorMessagegR(void *p) {
      typedef vector<CbmErrorMessage> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmErrorMessage>

namespace ROOT {
   static TClass *vectorlECbmDigiEventgR_Dictionary();
   static void vectorlECbmDigiEventgR_TClassManip(TClass*);
   static void *new_vectorlECbmDigiEventgR(void *p = nullptr);
   static void *newArray_vectorlECbmDigiEventgR(Long_t size, void *p);
   static void delete_vectorlECbmDigiEventgR(void *p);
   static void deleteArray_vectorlECbmDigiEventgR(void *p);
   static void destruct_vectorlECbmDigiEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmDigiEvent>*)
   {
      vector<CbmDigiEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmDigiEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmDigiEvent>", -2, "vector", 389,
                  typeid(vector<CbmDigiEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmDigiEventgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmDigiEvent>) );
      instance.SetNew(&new_vectorlECbmDigiEventgR);
      instance.SetNewArray(&newArray_vectorlECbmDigiEventgR);
      instance.SetDelete(&delete_vectorlECbmDigiEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmDigiEventgR);
      instance.SetDestructor(&destruct_vectorlECbmDigiEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmDigiEvent> >()));

      ::ROOT::AddClassAlternate("vector<CbmDigiEvent>","std::vector<CbmDigiEvent, std::allocator<CbmDigiEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmDigiEvent>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmDigiEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmDigiEvent>*)nullptr)->GetClass();
      vectorlECbmDigiEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmDigiEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmDigiEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmDigiEvent> : new vector<CbmDigiEvent>;
   }
   static void *newArray_vectorlECbmDigiEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmDigiEvent>[nElements] : new vector<CbmDigiEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmDigiEventgR(void *p) {
      delete ((vector<CbmDigiEvent>*)p);
   }
   static void deleteArray_vectorlECbmDigiEventgR(void *p) {
      delete [] ((vector<CbmDigiEvent>*)p);
   }
   static void destruct_vectorlECbmDigiEventgR(void *p) {
      typedef vector<CbmDigiEvent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmDigiEvent>

namespace ROOT {
   static TClass *vectorlECbmBmonDigigR_Dictionary();
   static void vectorlECbmBmonDigigR_TClassManip(TClass*);
   static void *new_vectorlECbmBmonDigigR(void *p = nullptr);
   static void *newArray_vectorlECbmBmonDigigR(Long_t size, void *p);
   static void delete_vectorlECbmBmonDigigR(void *p);
   static void deleteArray_vectorlECbmBmonDigigR(void *p);
   static void destruct_vectorlECbmBmonDigigR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CbmBmonDigi>*)
   {
      vector<CbmBmonDigi> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CbmBmonDigi>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CbmBmonDigi>", -2, "vector", 389,
                  typeid(vector<CbmBmonDigi>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECbmBmonDigigR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CbmBmonDigi>) );
      instance.SetNew(&new_vectorlECbmBmonDigigR);
      instance.SetNewArray(&newArray_vectorlECbmBmonDigigR);
      instance.SetDelete(&delete_vectorlECbmBmonDigigR);
      instance.SetDeleteArray(&deleteArray_vectorlECbmBmonDigigR);
      instance.SetDestructor(&destruct_vectorlECbmBmonDigigR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CbmBmonDigi> >()));

      ::ROOT::AddClassAlternate("vector<CbmBmonDigi>","std::vector<CbmBmonDigi, std::allocator<CbmBmonDigi> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CbmBmonDigi>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECbmBmonDigigR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CbmBmonDigi>*)nullptr)->GetClass();
      vectorlECbmBmonDigigR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECbmBmonDigigR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECbmBmonDigigR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmBmonDigi> : new vector<CbmBmonDigi>;
   }
   static void *newArray_vectorlECbmBmonDigigR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CbmBmonDigi>[nElements] : new vector<CbmBmonDigi>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECbmBmonDigigR(void *p) {
      delete ((vector<CbmBmonDigi>*)p);
   }
   static void deleteArray_vectorlECbmBmonDigigR(void *p) {
      delete [] ((vector<CbmBmonDigi>*)p);
   }
   static void destruct_vectorlECbmBmonDigigR(void *p) {
      typedef vector<CbmBmonDigi> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CbmBmonDigi>

namespace ROOT {
   static TClass *vectorlEAccTimingEventgR_Dictionary();
   static void vectorlEAccTimingEventgR_TClassManip(TClass*);
   static void *new_vectorlEAccTimingEventgR(void *p = nullptr);
   static void *newArray_vectorlEAccTimingEventgR(Long_t size, void *p);
   static void delete_vectorlEAccTimingEventgR(void *p);
   static void deleteArray_vectorlEAccTimingEventgR(void *p);
   static void destruct_vectorlEAccTimingEventgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<AccTimingEvent>*)
   {
      vector<AccTimingEvent> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<AccTimingEvent>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<AccTimingEvent>", -2, "vector", 389,
                  typeid(vector<AccTimingEvent>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEAccTimingEventgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<AccTimingEvent>) );
      instance.SetNew(&new_vectorlEAccTimingEventgR);
      instance.SetNewArray(&newArray_vectorlEAccTimingEventgR);
      instance.SetDelete(&delete_vectorlEAccTimingEventgR);
      instance.SetDeleteArray(&deleteArray_vectorlEAccTimingEventgR);
      instance.SetDestructor(&destruct_vectorlEAccTimingEventgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<AccTimingEvent> >()));

      ::ROOT::AddClassAlternate("vector<AccTimingEvent>","std::vector<AccTimingEvent, std::allocator<AccTimingEvent> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<AccTimingEvent>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEAccTimingEventgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<AccTimingEvent>*)nullptr)->GetClass();
      vectorlEAccTimingEventgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEAccTimingEventgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEAccTimingEventgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AccTimingEvent> : new vector<AccTimingEvent>;
   }
   static void *newArray_vectorlEAccTimingEventgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<AccTimingEvent>[nElements] : new vector<AccTimingEvent>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEAccTimingEventgR(void *p) {
      delete ((vector<AccTimingEvent>*)p);
   }
   static void deleteArray_vectorlEAccTimingEventgR(void *p) {
      delete [] ((vector<AccTimingEvent>*)p);
   }
   static void destruct_vectorlEAccTimingEventgR(void *p) {
      typedef vector<AccTimingEvent> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<AccTimingEvent>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary();
   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOintgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOintgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,int>*)
   {
      map<pair<int,int>,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,int>", -2, "map", 100,
                  typeid(map<pair<int,int>,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,int>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOintgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,int> >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,int>","std::map<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int> : new map<pair<int,int>,int>;
   }
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int>[nElements] : new map<pair<int,int>,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOintgR(void *p) {
      delete ((map<pair<int,int>,int>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p) {
      delete [] ((map<pair<int,int>,int>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p) {
      typedef map<pair<int,int>,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,int>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOintgR_Dictionary();
   static void maplEECbmModuleIdcOintgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOintgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOintgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOintgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOintgR(void *p);
   static void destruct_maplEECbmModuleIdcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,int>*)
   {
      map<ECbmModuleId,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,int>", -2, "map", 100,
                  typeid(map<ECbmModuleId,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,int>) );
      instance.SetNew(&new_maplEECbmModuleIdcOintgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOintgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOintgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,int> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,int>","std::map<ECbmModuleId, int, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,int>*)nullptr)->GetClass();
      maplEECbmModuleIdcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,int> : new map<ECbmModuleId,int>;
   }
   static void *newArray_maplEECbmModuleIdcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,int>[nElements] : new map<ECbmModuleId,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOintgR(void *p) {
      delete ((map<ECbmModuleId,int>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOintgR(void *p) {
      delete [] ((map<ECbmModuleId,int>*)p);
   }
   static void destruct_maplEECbmModuleIdcOintgR(void *p) {
      typedef map<ECbmModuleId,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,int>

namespace ROOT {
   static TClass *maplEECbmModuleIdcOTObjectmUgR_Dictionary();
   static void maplEECbmModuleIdcOTObjectmUgR_TClassManip(TClass*);
   static void *new_maplEECbmModuleIdcOTObjectmUgR(void *p = nullptr);
   static void *newArray_maplEECbmModuleIdcOTObjectmUgR(Long_t size, void *p);
   static void delete_maplEECbmModuleIdcOTObjectmUgR(void *p);
   static void deleteArray_maplEECbmModuleIdcOTObjectmUgR(void *p);
   static void destruct_maplEECbmModuleIdcOTObjectmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmModuleId,TObject*>*)
   {
      map<ECbmModuleId,TObject*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmModuleId,TObject*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmModuleId,TObject*>", -2, "map", 100,
                  typeid(map<ECbmModuleId,TObject*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmModuleIdcOTObjectmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmModuleId,TObject*>) );
      instance.SetNew(&new_maplEECbmModuleIdcOTObjectmUgR);
      instance.SetNewArray(&newArray_maplEECbmModuleIdcOTObjectmUgR);
      instance.SetDelete(&delete_maplEECbmModuleIdcOTObjectmUgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmModuleIdcOTObjectmUgR);
      instance.SetDestructor(&destruct_maplEECbmModuleIdcOTObjectmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmModuleId,TObject*> >()));

      ::ROOT::AddClassAlternate("map<ECbmModuleId,TObject*>","std::map<ECbmModuleId, TObject*, std::less<ECbmModuleId>, std::allocator<std::pair<ECbmModuleId const, TObject*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmModuleId,TObject*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmModuleIdcOTObjectmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmModuleId,TObject*>*)nullptr)->GetClass();
      maplEECbmModuleIdcOTObjectmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmModuleIdcOTObjectmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmModuleIdcOTObjectmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,TObject*> : new map<ECbmModuleId,TObject*>;
   }
   static void *newArray_maplEECbmModuleIdcOTObjectmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmModuleId,TObject*>[nElements] : new map<ECbmModuleId,TObject*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmModuleIdcOTObjectmUgR(void *p) {
      delete ((map<ECbmModuleId,TObject*>*)p);
   }
   static void deleteArray_maplEECbmModuleIdcOTObjectmUgR(void *p) {
      delete [] ((map<ECbmModuleId,TObject*>*)p);
   }
   static void destruct_maplEECbmModuleIdcOTObjectmUgR(void *p) {
      typedef map<ECbmModuleId,TObject*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmModuleId,TObject*>

namespace ROOT {
   static TClass *maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR_Dictionary();
   static void maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR_TClassManip(TClass*);
   static void *new_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p = nullptr);
   static void *newArray_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(Long_t size, void *p);
   static void delete_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p);
   static void deleteArray_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p);
   static void destruct_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<ECbmDataType,vector<unsigned int> >*)
   {
      map<ECbmDataType,vector<unsigned int> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<ECbmDataType,vector<unsigned int> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<ECbmDataType,vector<unsigned int> >", -2, "map", 100,
                  typeid(map<ECbmDataType,vector<unsigned int> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<ECbmDataType,vector<unsigned int> >) );
      instance.SetNew(&new_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR);
      instance.SetNewArray(&newArray_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR);
      instance.SetDelete(&delete_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR);
      instance.SetDestructor(&destruct_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<ECbmDataType,vector<unsigned int> > >()));

      ::ROOT::AddClassAlternate("map<ECbmDataType,vector<unsigned int> >","std::map<ECbmDataType, std::vector<unsigned int, std::allocator<unsigned int> >, std::less<ECbmDataType>, std::allocator<std::pair<ECbmDataType const, std::vector<unsigned int, std::allocator<unsigned int> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<ECbmDataType,vector<unsigned int> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<ECbmDataType,vector<unsigned int> >*)nullptr)->GetClass();
      maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmDataType,vector<unsigned int> > : new map<ECbmDataType,vector<unsigned int> >;
   }
   static void *newArray_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<ECbmDataType,vector<unsigned int> >[nElements] : new map<ECbmDataType,vector<unsigned int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p) {
      delete ((map<ECbmDataType,vector<unsigned int> >*)p);
   }
   static void deleteArray_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p) {
      delete [] ((map<ECbmDataType,vector<unsigned int> >*)p);
   }
   static void destruct_maplEECbmDataTypecOvectorlEunsignedsPintgRsPgR(void *p) {
      typedef map<ECbmDataType,vector<unsigned int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<ECbmDataType,vector<unsigned int> >

namespace {
  void TriggerDictionaryInitialization_libCbmData_Impl() {
    static const char* headers[] = {
"CbmDigiData.h",
"CbmDigiEvent.h",
"CbmDigiTimeslice.h",
"CbmBmonDigiData.h",
"CbmStsDigiData.h",
"CbmMuchDigiData.h",
"CbmRichDigiData.h",
"CbmTrdDigiData.h",
"CbmTofDigiData.h",
"CbmPsdDigiData.h",
"CbmAddress.h",
"CbmTimeSlice.h",
"CbmEvent.h",
"CbmEventStore.h",
"CbmHit.h",
"CbmPixelHit.h",
"CbmStripHit.h",
"CbmTrack.h",
"CbmTrackMatch.h",
"CbmCluster.h",
"CbmModuleList.h",
"CbmErrorMessage.h",
"CbmMCTrack.h",
"CbmMCEventInfo.h",
"CbmMCEventList.h",
"CbmLink.h",
"CbmMatch.h",
"CbmTrackMatchNew.h",
"CbmTsEventHeader.h",
"CbmDigiBranchBase.h",
"CbmDigiContainer.h",
"CbmMvdCluster.h",
"CbmMvdDetectorId.h",
"CbmMvdDigi.h",
"CbmMvdHit.h",
"CbmMvdPoint.h",
"CbmMvdHitMatch.h",
"CbmBmonDigi.h",
"CbmStsAddress.h",
"CbmStsCluster.h",
"CbmStsDigi.h",
"CbmStsHit.h",
"CbmStsPoint.h",
"CbmStsTrack.h",
"CbmRichRing.h",
"CbmRichHit.h",
"CbmRichPoint.h",
"CbmRichTrbDigi.h",
"CbmRichDigi.h",
"CbmMuchPixelHit.h",
"CbmMuchPoint.h",
"CbmMuchCluster.h",
"CbmMuchDigi.h",
"CbmMuchBeamTimeDigi.h",
"CbmMuchDigiMatch.h",
"CbmMuchTrack.h",
"CbmMuchAddress.h",
"CbmTrdHit.h",
"CbmTrdTrack.h",
"CbmTrdPoint.h",
"CbmTrdDigi.h",
"CbmTrdAddress.h",
"CbmTrdCluster.h",
"CbmTrdRawMessageSpadic.h",
"CbmTofHit.h",
"CbmTofPoint.h",
"CbmTofDetectorId.h",
"CbmTofDetectorId_v07a.h",
"CbmTofDetectorId_v12b.h",
"CbmTofDetectorId_v14a.h",
"CbmTofDetectorId_v21a.h",
"CbmTofAddress.h",
"CbmTofDigi.h",
"CbmTofTrack.h",
"CbmTofTracklet.h",
"CbmTofTrackletParam.h",
"CbmPsdDigi.h",
"CbmPsdDsp.h",
"CbmPsdHit.h",
"CbmPsdPoint.h",
"CbmPsdAddress.h",
"CbmPsdMCbmHit.h",
"CbmGlobalTrack.h",
"CbmVertex.h",
"CbmTrackParam.h",
"AccDataSis18.h",
"StsXyterMessage.h",
"gDpbMessv100.h",
"CriGet4Mess001.h",
"TimesliceMetaData.h",
"PsdGbtReader-v0.00.h",
"PsdGbtReader-v1.00.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include",
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
"/home/cbmdata/_deps/fairsoft_apr22/install/include/root",
"/home/heinemann/gpuunpacker/cbmroot/build2/core/data/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCbmData dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$CbmBmonDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmBmonDigi;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$CbmMuchDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmMuchDigi;
class __attribute__((annotate("$clingAutoload$CbmPsdDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmPsdDigi;
class __attribute__((annotate("$clingAutoload$CbmRichDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmRichDigi;
class __attribute__((annotate("$clingAutoload$CbmStsDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmStsDigi;
class __attribute__((annotate("$clingAutoload$CbmTofDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmTofDigi;
class __attribute__((annotate("$clingAutoload$CbmTrdDigi.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmTrdDigi;
class __attribute__((annotate("$clingAutoload$CbmMatch.h")))  __attribute__((annotate("$clingAutoload$CbmTimeSlice.h")))  CbmMatch;
class __attribute__((annotate("$clingAutoload$CbmDigiEvent.h")))  CbmDigiEvent;
class __attribute__((annotate("$clingAutoload$CbmEvent.h")))  CbmEvent;
class __attribute__((annotate("$clingAutoload$CbmEventStore.h")))  CbmEventStore;
class __attribute__((annotate("$clingAutoload$CbmPsdDsp.h")))  CbmPsdDsp;
class __attribute__((annotate("$clingAutoload$CbmMuchBeamTimeDigi.h")))  CbmMuchBeamTimeDigi;
class __attribute__((annotate("$clingAutoload$CbmMvdDigi.h")))  CbmMvdDigi;
class __attribute__((annotate("$clingAutoload$CbmTrdRawMessageSpadic.h")))  CbmTrdRawMessageSpadic;
namespace gdpbv100{class __attribute__((annotate("$clingAutoload$gDpbMessv100.h")))  Message;}
namespace stsxyter{class __attribute__((annotate("$clingAutoload$StsXyterMessage.h")))  Message;}
class __attribute__((annotate("$clingAutoload$CbmErrorMessage.h")))  CbmErrorMessage;
class __attribute__((annotate("$clingAutoload$CbmBmonDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmBmonDigiData;
class __attribute__((annotate("$clingAutoload$CbmAddress.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmAddress;
class __attribute__((annotate("$clingAutoload$CbmMuchAddress.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmMuchAddress;
class __attribute__((annotate("$clingAutoload$CbmMuchDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmMuchDigiData;
class __attribute__((annotate("$clingAutoload$CbmPsdAddress.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmPsdAddress;
class __attribute__((annotate("$clingAutoload$CbmPsdDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmPsdDigiData;
class __attribute__((annotate("$clingAutoload$CbmRichDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmRichDigiData;
class __attribute__((annotate("$clingAutoload$CbmStsDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmStsDigiData;
class __attribute__((annotate("$clingAutoload$CbmTofAddress.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmTofAddress;
class __attribute__((annotate("$clingAutoload$CbmTofDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmTofDigiData;
class __attribute__((annotate("$clingAutoload$CbmTrdDigiData.h")))  __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmTrdDigiData;
class __attribute__((annotate("$clingAutoload$CbmDigiData.h")))  CbmDigiData;
class __attribute__((annotate("$clingAutoload$CbmDigiTimeslice.h")))  CbmDigiTimeslice;
class __attribute__((annotate("$clingAutoload$CbmLink.h")))  __attribute__((annotate("$clingAutoload$CbmTimeSlice.h")))  CbmLink;
class __attribute__((annotate("$clingAutoload$CbmTimeSlice.h")))  CbmTimeSlice;
class __attribute__((annotate("$clingAutoload$CbmVertex.h")))  __attribute__((annotate("$clingAutoload$CbmEvent.h")))  CbmVertex;
class __attribute__((annotate("$clingAutoload$CbmDigiContainer.h")))  __attribute__((annotate("$clingAutoload$CbmEventStore.h")))  CbmDigiContainer;
template <class Digi> class __attribute__((annotate("$clingAutoload$CbmDigiVector.h")))  __attribute__((annotate("$clingAutoload$CbmEventStore.h")))  CbmDigiVector;

class __attribute__((annotate("$clingAutoload$CbmHit.h")))  CbmHit;
class __attribute__((annotate("$clingAutoload$CbmPixelHit.h")))  CbmPixelHit;
class __attribute__((annotate("$clingAutoload$CbmStripHit.h")))  CbmStripHit;
class __attribute__((annotate("$clingAutoload$CbmTrack.h")))  CbmTrack;
class __attribute__((annotate("$clingAutoload$CbmTrackMatch.h")))  CbmTrackMatch;
class __attribute__((annotate("$clingAutoload$CbmCluster.h")))  CbmCluster;
class __attribute__((annotate("$clingAutoload$CbmModuleList.h")))  CbmModuleList;
class __attribute__((annotate("$clingAutoload$CbmMCTrack.h")))  CbmMCTrack;
class __attribute__((annotate("$clingAutoload$CbmMCEventInfo.h")))  CbmMCEventInfo;
class __attribute__((annotate("$clingAutoload$CbmMCEventList.h")))  CbmMCEventList;
class __attribute__((annotate("$clingAutoload$CbmTrackMatchNew.h")))  CbmTrackMatchNew;
class __attribute__((annotate("$clingAutoload$CbmTsEventHeader.h")))  CbmTsEventHeader;
class __attribute__((annotate("$clingAutoload$CbmDigiBranchBase.h")))  CbmDigiBranchBase;
class __attribute__((annotate("$clingAutoload$CbmMvdDetectorId.h")))  __attribute__((annotate("$clingAutoload$CbmMvdCluster.h")))  CbmMvdDetectorId;
class __attribute__((annotate("$clingAutoload$CbmMvdCluster.h")))  CbmMvdCluster;
class __attribute__((annotate("$clingAutoload$CbmMvdHit.h")))  CbmMvdHit;
class __attribute__((annotate("$clingAutoload$CbmMvdPoint.h")))  CbmMvdPoint;
class __attribute__((annotate("$clingAutoload$CbmMvdHitMatch.h")))  CbmMvdHitMatch;
class __attribute__((annotate("$clingAutoload$CbmStsCluster.h")))  CbmStsCluster;
class __attribute__((annotate("$clingAutoload$CbmStsHit.h")))  CbmStsHit;
class __attribute__((annotate("$clingAutoload$CbmStsPoint.h")))  CbmStsPoint;
class __attribute__((annotate("$clingAutoload$CbmStsTrack.h")))  CbmStsTrack;
class __attribute__((annotate("$clingAutoload$CbmRichRing.h")))  CbmRichRing;
class __attribute__((annotate("$clingAutoload$CbmRichHit.h")))  CbmRichHit;
class __attribute__((annotate("$clingAutoload$CbmRichPoint.h")))  CbmRichPoint;
class __attribute__((annotate("$clingAutoload$CbmRichTrbDigi.h")))  CbmRichTrbDigi;
class __attribute__((annotate("$clingAutoload$CbmMuchPixelHit.h")))  CbmMuchPixelHit;
class __attribute__((annotate("$clingAutoload$CbmMuchPoint.h")))  CbmMuchPoint;
class __attribute__((annotate("$clingAutoload$CbmMuchCluster.h")))  CbmMuchCluster;
class __attribute__((annotate("$clingAutoload$CbmMuchDigiMatch.h")))  CbmMuchDigiMatch;
class __attribute__((annotate("$clingAutoload$CbmMuchTrack.h")))  CbmMuchTrack;
class __attribute__((annotate("$clingAutoload$CbmTrdAddress.h")))  __attribute__((annotate("$clingAutoload$CbmTrdHit.h")))  CbmTrdAddress;
class __attribute__((annotate("$clingAutoload$CbmTrdHit.h")))  CbmTrdHit;
class __attribute__((annotate("$clingAutoload$CbmTrdTrack.h")))  CbmTrdTrack;
class __attribute__((annotate("$clingAutoload$CbmTrdPoint.h")))  CbmTrdPoint;
class __attribute__((annotate(R"ATTRDUMP(cluster of digi for the TRD detector)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$CbmTrdCluster.h")))  CbmTrdCluster;
class __attribute__((annotate("$clingAutoload$CbmTofHit.h")))  CbmTofHit;
class __attribute__((annotate("$clingAutoload$CbmTofPoint.h")))  CbmTofPoint;
class __attribute__((annotate("$clingAutoload$CbmTofTrack.h")))  CbmTofTrack;
class __attribute__((annotate("$clingAutoload$CbmTofTrackletParam.h")))  __attribute__((annotate("$clingAutoload$CbmTofTracklet.h")))  CbmTofTrackletParam;
class __attribute__((annotate("$clingAutoload$CbmTofTracklet.h")))  CbmTofTracklet;
class __attribute__((annotate("$clingAutoload$CbmPsdHit.h")))  CbmPsdHit;
class __attribute__((annotate("$clingAutoload$CbmPsdPoint.h")))  CbmPsdPoint;
class __attribute__((annotate("$clingAutoload$CbmPsdMCbmHit.h")))  CbmPsdMCbmHit;
class __attribute__((annotate("$clingAutoload$CbmTrackParam.h")))  __attribute__((annotate("$clingAutoload$CbmGlobalTrack.h")))  CbmTrackParam;
class __attribute__((annotate("$clingAutoload$CbmGlobalTrack.h")))  CbmGlobalTrack;
class __attribute__((annotate("$clingAutoload$AccDataSis18.h")))  AccTimingEvent;
class __attribute__((annotate("$clingAutoload$AccDataSis18.h")))  AccStatusTs;
namespace gdpbv100{class __attribute__((annotate("$clingAutoload$gDpbMessv100.h")))  FullMessage;}
namespace critof001{class __attribute__((annotate("$clingAutoload$CriGet4Mess001.h")))  Message;}
namespace critof001{class __attribute__((annotate("$clingAutoload$CriGet4Mess001.h")))  FullMessage;}
class __attribute__((annotate("$clingAutoload$TimesliceMetaData.h")))  TimesliceMetaData;
namespace PsdDataV000{class __attribute__((annotate("$clingAutoload$PsdGbtReader-v0.00.h")))  PsdGbtReader;}
namespace PsdDataV100{class __attribute__((annotate("$clingAutoload$PsdGbtReader-v1.00.h")))  PsdGbtReader;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCbmData dictionary payload"

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
#include "CbmDigiData.h"
#include "CbmDigiEvent.h"
#include "CbmDigiTimeslice.h"
#include "CbmBmonDigiData.h"
#include "CbmStsDigiData.h"
#include "CbmMuchDigiData.h"
#include "CbmRichDigiData.h"
#include "CbmTrdDigiData.h"
#include "CbmTofDigiData.h"
#include "CbmPsdDigiData.h"
#include "CbmAddress.h"
#include "CbmTimeSlice.h"
#include "CbmEvent.h"
#include "CbmEventStore.h"
#include "CbmHit.h"
#include "CbmPixelHit.h"
#include "CbmStripHit.h"
#include "CbmTrack.h"
#include "CbmTrackMatch.h"
#include "CbmCluster.h"
#include "CbmModuleList.h"
#include "CbmErrorMessage.h"
#include "CbmMCTrack.h"
#include "CbmMCEventInfo.h"
#include "CbmMCEventList.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmTrackMatchNew.h"
#include "CbmTsEventHeader.h"
#include "CbmDigiBranchBase.h"
#include "CbmDigiContainer.h"
#include "CbmMvdCluster.h"
#include "CbmMvdDetectorId.h"
#include "CbmMvdDigi.h"
#include "CbmMvdHit.h"
#include "CbmMvdPoint.h"
#include "CbmMvdHitMatch.h"
#include "CbmBmonDigi.h"
#include "CbmStsAddress.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsPoint.h"
#include "CbmStsTrack.h"
#include "CbmRichRing.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichTrbDigi.h"
#include "CbmRichDigi.h"
#include "CbmMuchPixelHit.h"
#include "CbmMuchPoint.h"
#include "CbmMuchCluster.h"
#include "CbmMuchDigi.h"
#include "CbmMuchBeamTimeDigi.h"
#include "CbmMuchDigiMatch.h"
#include "CbmMuchTrack.h"
#include "CbmMuchAddress.h"
#include "CbmTrdHit.h"
#include "CbmTrdTrack.h"
#include "CbmTrdPoint.h"
#include "CbmTrdDigi.h"
#include "CbmTrdAddress.h"
#include "CbmTrdCluster.h"
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTofHit.h"
#include "CbmTofPoint.h"
#include "CbmTofDetectorId.h"
#include "CbmTofDetectorId_v07a.h"
#include "CbmTofDetectorId_v12b.h"
#include "CbmTofDetectorId_v14a.h"
#include "CbmTofDetectorId_v21a.h"
#include "CbmTofAddress.h"
#include "CbmTofDigi.h"
#include "CbmTofTrack.h"
#include "CbmTofTracklet.h"
#include "CbmTofTrackletParam.h"
#include "CbmPsdDigi.h"
#include "CbmPsdDsp.h"
#include "CbmPsdHit.h"
#include "CbmPsdPoint.h"
#include "CbmPsdAddress.h"
#include "CbmPsdMCbmHit.h"
#include "CbmGlobalTrack.h"
#include "CbmVertex.h"
#include "CbmTrackParam.h"
#include "AccDataSis18.h"
#include "StsXyterMessage.h"
#include "gDpbMessv100.h"
#include "CriGet4Mess001.h"
#include "TimesliceMetaData.h"
#include "PsdGbtReader-v0.00.h"
#include "PsdGbtReader-v1.00.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"AccStatusTs", payloadCode, "@",
"AccTimingEvent", payloadCode, "@",
"CbmAddress", payloadCode, "@",
"CbmBmonDigi", payloadCode, "@",
"CbmBmonDigiData", payloadCode, "@",
"CbmCluster", payloadCode, "@",
"CbmDigiBranchBase", payloadCode, "@",
"CbmDigiContainer", payloadCode, "@",
"CbmDigiData", payloadCode, "@",
"CbmDigiEvent", payloadCode, "@",
"CbmDigiTimeslice", payloadCode, "@",
"CbmDigiVector<CbmMuchBeamTimeDigi>", payloadCode, "@",
"CbmDigiVector<CbmMuchDigi>", payloadCode, "@",
"CbmDigiVector<CbmMvdDigi>", payloadCode, "@",
"CbmDigiVector<CbmPsdDigi>", payloadCode, "@",
"CbmDigiVector<CbmPsdDsp>", payloadCode, "@",
"CbmDigiVector<CbmRichDigi>", payloadCode, "@",
"CbmDigiVector<CbmStsDigi>", payloadCode, "@",
"CbmDigiVector<CbmTofDigi>", payloadCode, "@",
"CbmDigiVector<CbmTrdDigi>", payloadCode, "@",
"CbmErrorMessage", payloadCode, "@",
"CbmEvent", payloadCode, "@",
"CbmEventStore", payloadCode, "@",
"CbmGlobalTrack", payloadCode, "@",
"CbmHit", payloadCode, "@",
"CbmLink", payloadCode, "@",
"CbmMCEventInfo", payloadCode, "@",
"CbmMCEventList", payloadCode, "@",
"CbmMCTrack", payloadCode, "@",
"CbmMatch", payloadCode, "@",
"CbmModuleList", payloadCode, "@",
"CbmMuchAddress", payloadCode, "@",
"CbmMuchBeamTimeDigi", payloadCode, "@",
"CbmMuchCluster", payloadCode, "@",
"CbmMuchDigi", payloadCode, "@",
"CbmMuchDigiData", payloadCode, "@",
"CbmMuchDigiMatch", payloadCode, "@",
"CbmMuchPixelHit", payloadCode, "@",
"CbmMuchPoint", payloadCode, "@",
"CbmMuchTrack", payloadCode, "@",
"CbmMvdCluster", payloadCode, "@",
"CbmMvdDetectorId", payloadCode, "@",
"CbmMvdDigi", payloadCode, "@",
"CbmMvdHit", payloadCode, "@",
"CbmMvdHitMatch", payloadCode, "@",
"CbmMvdPoint", payloadCode, "@",
"CbmPixelHit", payloadCode, "@",
"CbmPsdAddress", payloadCode, "@",
"CbmPsdDigi", payloadCode, "@",
"CbmPsdDigiData", payloadCode, "@",
"CbmPsdDsp", payloadCode, "@",
"CbmPsdHit", payloadCode, "@",
"CbmPsdMCbmHit", payloadCode, "@",
"CbmPsdPoint", payloadCode, "@",
"CbmRichDigi", payloadCode, "@",
"CbmRichDigiData", payloadCode, "@",
"CbmRichHit", payloadCode, "@",
"CbmRichPoint", payloadCode, "@",
"CbmRichRing", payloadCode, "@",
"CbmRichTrbDigi", payloadCode, "@",
"CbmStripHit", payloadCode, "@",
"CbmStsCluster", payloadCode, "@",
"CbmStsDigi", payloadCode, "@",
"CbmStsDigiData", payloadCode, "@",
"CbmStsHit", payloadCode, "@",
"CbmStsPoint", payloadCode, "@",
"CbmStsTrack", payloadCode, "@",
"CbmTimeSlice", payloadCode, "@",
"CbmTofAddress", payloadCode, "@",
"CbmTofDigi", payloadCode, "@",
"CbmTofDigiData", payloadCode, "@",
"CbmTofHit", payloadCode, "@",
"CbmTofPoint", payloadCode, "@",
"CbmTofTrack", payloadCode, "@",
"CbmTofTracklet", payloadCode, "@",
"CbmTofTrackletParam", payloadCode, "@",
"CbmTrack", payloadCode, "@",
"CbmTrackMatch", payloadCode, "@",
"CbmTrackMatchNew", payloadCode, "@",
"CbmTrackParam", payloadCode, "@",
"CbmTrdAddress", payloadCode, "@",
"CbmTrdCluster", payloadCode, "@",
"CbmTrdDigi", payloadCode, "@",
"CbmTrdDigiData", payloadCode, "@",
"CbmTrdHit", payloadCode, "@",
"CbmTrdPoint", payloadCode, "@",
"CbmTrdRawMessageSpadic", payloadCode, "@",
"CbmTrdTrack", payloadCode, "@",
"CbmTsEventHeader", payloadCode, "@",
"CbmVertex", payloadCode, "@",
"ECbmDataType", payloadCode, "@",
"ECbmModuleId", payloadCode, "@",
"ECbmTreeAccess", payloadCode, "@",
"EStsElementLevel", payloadCode, "@",
"PsdDataV000::PsdGbtReader", payloadCode, "@",
"PsdDataV100::PsdGbtReader", payloadCode, "@",
"TimesliceMetaData", payloadCode, "@",
"critof001::FullMessage", payloadCode, "@",
"critof001::Message", payloadCode, "@",
"gdpbv100::FullMessage", payloadCode, "@",
"gdpbv100::Message", payloadCode, "@",
"stsxyter::Message", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCbmData",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCbmData_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCbmData_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCbmData() {
  TriggerDictionaryInitialization_libCbmData_Impl();
}
