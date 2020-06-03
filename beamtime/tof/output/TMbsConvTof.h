// ------------------------------------------------------------------
// -----                    TMbsConvTof                       -----
// -----              Created 11/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _TMBSCONVTOF_H_
#define _TMBSCONVTOF_H_

#include "FairTask.h"

#include <vector>

#include "TofTdcDef.h"


// Parameters
class TMbsUnpackTofPar;
class TMbsCalibTofPar;
class TMbsMappingTofPar;
class TMbsConvTofPar;

// Input Data
class TTofCalibData;
/*
class CbmTofDigiExp;
class CbmTofDigi;
*/
#include "CbmTofDigi.h"

// Output Data
class Plastics_Hit;
class Rpc_Hit;
class Scalers_Event;
class TVftxBoardData;

// ROOT
class TFile;
class TDirectory;
class TClonesArray;
class TH1;
class TH2;
class TString;

class TMbsConvTof : public FairTask {
   public:
      TMbsConvTof();
      TMbsConvTof(const char* name, Int_t mode = 1, Int_t verbose = 1);
      virtual ~TMbsConvTof();

      // Fairtask specific functions
      virtual void SetParContainers();
      virtual InitStatus Init();
      virtual InitStatus ReInit();
      virtual void Exec(Option_t* option);
      virtual void Finish();
   
      // Histograms
      void WriteHistogramms();
      
   private:
      TMbsConvTof(const TMbsConvTof&);
      TMbsConvTof operator=(const TMbsConvTof&);
      
      // Parameters
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      TMbsCalibTofPar     *fMbsCalibPar;
      TMbsMappingTofPar   *fMbsMappingPar;
      TMbsConvTofPar      *fMbsConvPar;
      
      // Histograms
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      void DeleteHistograms(); // TODO
      
      // Histograms objects
      std::vector< TH2* > fhRpcHitPosition;
      std::vector< TH2* > fhRpcTotComp;
      std::vector< TH1* > fhPlasticHitPosition;
      std::vector< TH2* > fhPlasticTotComp;
      
      // Input 
      Bool_t RegisterInput();
      
      // Input objects
      TClonesArray        * fTriglogBoardCollection;
      TClonesArray        * fScalerBoardCollection;
      TClonesArray        * fCaenBoardCollection;
      TClonesArray        * fVftxBoardCollection;
      TClonesArray        * fGet4BoardCollection;
      TClonesArray        * fCalibDataCollection;
      TClonesArray        * fCalibScalCollection;
      TClonesArray        * fCbmTofDigiCollection; 
      TClonesArray        * fRateDataCollection;
      
      // Output to ROOT file
      Bool_t RegisterOutput();
      Bool_t FillOutput();
      Bool_t ClearOutput();
      Bool_t DeleteOutput();
      
      // Output objects
      TString sOutputFilename;
      TFile * fOutputFile;
      TTree * fOutputTree;
      
      // Triglog
      UInt_t uTriglogSyncNb;
      UInt_t uTriglogPattern;
      UInt_t uTriglogInputPattern;
      UInt_t uTriglogTimeSec;
      UInt_t TriglogTimeMilsec;
      Bool_t FillTriglogData();
      Bool_t ClearTriglogData();
      
      // VFTX calibrated TDC boards conversion
      std::vector< TVftxBoardData >               fvVftxCalibBoards;
      Bool_t FillVftxBoards();
      Bool_t ClearVftxBoards();
      
      // Calibrated scalers: Rates, clock time, ...
      // + Mapped Rates when available
      std::vector< Scalers_Event   >              fvScalerEvents;     // Dim 1 = Scaler Board, 1st for (TRIGLOG + 1 ScalOrMu + Detectors)
      Bool_t FillCalibScaler();
      Bool_t FillMappedScaler();
      Bool_t ClearScalerEvent();
      
      // Fired Channel building & Conversion
      std::vector< Bool_t >                       fvbMappedDet; 
      std::vector< Int_t >                        fviNbChannels; 
      std::vector< CbmTofDigi >                ** fvClassedDigis; // Dim 1 = Detector, Dim 2 = Channel * Side, Dim 3 = Digi Ind
      std::vector< std::vector< Plastics_Hit  > > fvPlasticHits;     // Dim 1 = Plastic,  Dim 2 = Hit Ind
      std::vector< std::vector< Rpc_Hit       > > fvRpcHits;         // Dim 1 = RPC,      Dim 2 = Hit Ind
      std::vector< TVftxBoardData  >              fvVftxBoards;      // Dim 1 = VFTX board
      Bool_t InitTempDigiStorage();
      Bool_t FillTempDigiStorage();
      Bool_t BuildFiredChannels();
      Bool_t ClearTempDigiStorage();
      Bool_t DeleteTempDigiStorage();
      
      Int_t fiNbEvents;
      
      ClassDef(TMbsConvTof, 1);
};
#endif // _TMBSCONVTOF_H_  
