// ------------------------------------------------------------------
// -----                     TMbsConvTofPar                     -----
// -----              Created 11/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _TMBSCONVTOFPAR_H_
#define _TMBSCONVTOFPAR_H_

#include "FairParGenericSet.h"
#include "TArrayI.h"
#include "TArrayD.h"

class TMbsConvTofPar : public FairParGenericSet 
{
   public:
      TMbsConvTofPar() ;

      TMbsConvTofPar( const char* name,
                      const char* title="Mbs Mapping parameters for the ToF",
                      const char* context="TestDefaultContext" );

      virtual ~TMbsConvTofPar();
      
      // Accessors
      Bool_t  IsDebug()        const { return (1 == fuConvDebug)? kTRUE: kFALSE; };
      TString GetOutFilename() const { return fsFilenameOut; };
      
      Bool_t  TriglogEnabled() const { return (1 == fiEnableTriglog)? kTRUE: kFALSE; };
      Int_t   GetNbOutScal()   const { return fiNbOutScalers; };
      Int_t   GetScalerInd(    Int_t outScalerInd ) const;
      Int_t   GetNbOutVftx()   const { return fiNbOutVftx; };
      Int_t   GetVftxInd(      Int_t outBoardInd ) const;
      Int_t   GetVftxOutInd(   Int_t inputBoardInd ) const;
         // General
      Int_t   GetNbDetectors() const { return fiNbRpc + fiNbPlastics; };
      Int_t   GetDetectorUid(  Int_t detInd ) const;
      Int_t   GetDetectorInd(  Int_t channelAddress ) const;
      Int_t   GetDetChTyp(     Int_t detInd ) const;
         // RPC
      Int_t   GetNbRpc()       const { return fiNbRpc; };
      Int_t   GetRpcUid(       Int_t rpcInd ) const;
      Int_t   GetRpcInd(       Int_t channelAddress ) const;
      Int_t   GetRpcChType(    Int_t rpcInd ) const;
         // PLASTIC
      Int_t   GetNbPlastic()   const { return fiNbPlastics; };
      Int_t   GetPlasticUid(   Int_t plasticInd ) const;
      Int_t   GetPlasticInd(   Int_t channelAddress ) const;
      Int_t   GetPlasticEndNb( Int_t plasticInd ) const;
            
      // Example taken from CbmTofDigiPar
      void clear(void);
      void putParams(FairParamList*);
      Bool_t getParams(FairParamList*);
      virtual void printParams();
    
   private:
      /** DEBUG */
         // Switch ON/OFF debug tests, output & histos in calibration
      UInt_t   fuConvDebug; 
      
      /** Conversion **/
         // Name and path of the ROOT file in which the tree will be save
      TString  fsFilenameOut;
         // Enable/Disable the Triglog data in the output tree
      Int_t    fiEnableTriglog;
         // Nb of scaler boards saved in the TTree
      Int_t    fiNbOutScalers;
         // Index of the scaler boards to be save under specified output scaler index
      TArrayI  fiScalersIndex;
         // Number of scalers groups for each of the output scalers (maybe in scaler unpacker parameter)
//      TArrayI  fiScalersNb;
         // Number of channels per scaler group for each of the output scalers (maybe in scaler unpacker parameter)
//      TArrayI  fiScalersChNb;
         // Number of VFTX boards saved in output TTree
      Int_t    fiNbOutVftx;
         // Index of the VFTX boards to be save under specified output VFTX index
      TArrayI  fiVftxIndex;
         // Number of RPCs to be saved in output TTree
      Int_t    fiNbRpc;
         // Unique address of the RPC to be saved under specified output RPC index
      TArrayI  fiRpcUniqueAddress;
         // Channel type ( 1 = pad, 2 = strip, ...) for specified output RPC index
      TArrayI  fiRpcChType;
         // Number of Plastics to be saved in output TTree
      Int_t    fiNbPlastics;
         // Unique address of the Plastics (mapped as a 1 channel RPC) to be saved under specified output plastic index
      TArrayI  fiPlasticUniqueAddress;
         // Ends readout type ( 1 = single end, 2 = both ends, ...) for specified output plastic index
      TArrayI  fiPlasticEndsNb;
      
      TMbsConvTofPar(const TMbsConvTofPar&);
      TMbsConvTofPar& operator=(const TMbsConvTofPar&);
    
  ClassDef(TMbsConvTofPar, 1)
};

#endif //TMBSCONVTOFPAR_H
