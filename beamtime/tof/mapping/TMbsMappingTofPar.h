// ------------------------------------------------------------------
// -----                     TMbsMappingTofPar                  -----
// -----              Created 09/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _TMBSMAPPINGTOFPAR_H_
#define _TMBSMAPPINGTOFPAR_H_

#include "TofTdcDef.h"

#include "FairParGenericSet.h"
#include "TArrayI.h"
#include "TArrayD.h"

class TMbsMappingTofPar : public FairParGenericSet 
{
   public:
      TMbsMappingTofPar() ;

      TMbsMappingTofPar( const char* name,
                      const char* title="Mbs Mapping parameters for the ToF",
                      const char* context="TestDefaultContext" );

      virtual ~TMbsMappingTofPar();
      
      // Accessors
      Bool_t  IsDebug()        const { return (1 == fuMappingDebug)? kTRUE: kFALSE; };
      Bool_t  UseDigiExp()     const { return (1 == fiUseExtendedDigi)? kTRUE: kFALSE; };
      Int_t   GetNbMappedTdc() const { return fiNbMappedTdc; };
      Int_t   GetMappedTdcUId( Int_t iMappedTdcInd ) const;
      Int_t   GetMappedTdcInd( Int_t iTdcUId )       const;
      Int_t   GetMapping( Int_t iMappedTdcInd, Int_t iMappedChan ) const;
      Int_t   GetNbMappedDet() const { return fiNbMappedDet; };
      Int_t   GetMappedDetUId( Int_t iMappedDetInd ) const;
      Int_t   GetMappedDetInd( Int_t iMappedDetUId ) const;
      Int_t   GetSmTypeNbCh(   Int_t iSmType ) const;
            
      // Example taken from CbmTofDigiPar
      void clear(void);
      void putParams(FairParamList*);
      Bool_t getParams(FairParamList*);
      virtual void printParams();
    
   private:
      /** DEBUG */
         // Switch ON/OFF debug tests, output & histos in calibration
      UInt_t   fuMappingDebug;  
      
      /** Mapping **/
         // Nb of TDC mapped (1)
      Int_t    fiNbMappedTdc;
         // Nb of channels per TDC (2)
      TArrayI  fiNbChTdc;
         // TDC Unique ID (aka 1st channel rising edge ID)
      TArrayI  fiTdcUniqueId;
         // TDC to Detector Mapping: for each tdc channel provides the Detector Unique ID to use
      TArrayI  fiMapping;
         // Use extended Digis: Time and Tot stored as double
         // while "normal" digis rebin both to make them fit together in a single 32b integer
      Int_t    fiUseExtendedDigi;
         // Nb of Detectors mapped (3)
      Int_t    fiNbMappedDet;
         // Det Unique ID (aka 1st channel left side ID)
      TArrayI  fiDetUniqueId;
      
      // TODO: To be defined somewhere else later!!
      static const Int_t fgkiNbDetType = 11;
      static Int_t fgkiNbChDetType[fgkiNbDetType];
      
      TMbsMappingTofPar(const TMbsMappingTofPar&);
      TMbsMappingTofPar& operator=(const TMbsMappingTofPar&);
    
  ClassDef(TMbsMappingTofPar, 1)
};

#endif //TMBSMAPPINGTOFPAR_H
