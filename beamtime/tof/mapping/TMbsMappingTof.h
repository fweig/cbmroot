// ------------------------------------------------------------------
// -----                    TMbsMappingTof                       -----
// -----              Created 09/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _TMBSMAPPINGTOF_H_
#define _TMBSMAPPINGTOF_H_

#include "FairTask.h"

#include <vector>

#include "TofTdcDef.h"

// Parameters
class TMbsUnpackTofPar;
class TMbsCalibTofPar;
class TMbsMappingTofPar;

// Output Data
class TTofCalibData;

// ROOT
class TFile;
class TDirectory;
class TClonesArray;
class TH1;
class TH2;
class TString;

class TMbsMappingTof : public FairTask {
   public:
      TMbsMappingTof();
      TMbsMappingTof(const char* name, Int_t mode = 1, Int_t verbose = 1);
      virtual ~TMbsMappingTof();

      // Fairtask specific functions
      virtual void SetParContainers();
      virtual InitStatus Init();
      virtual InitStatus ReInit();
      virtual void Exec(Option_t* option);
      virtual void Finish();
   
      // Histograms
      void WriteHistogramms();
      void SetFillHistogramms( Bool_t bFillHistos = kTRUE ) { fbFillHistos = bFillHistos; }
      void SetDuplicatePadDigis( Bool_t bDuplicate = kTRUE ) { fbDuplicatePadDigis = bDuplicate; }

      void SetSaveDigis( Bool_t bSaveDigis = kTRUE );
   private:
      TMbsMappingTof(const TMbsMappingTof&);
      TMbsMappingTof operator=(const TMbsMappingTof&);
      
      // Parameters
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      TMbsCalibTofPar     *fMbsCalibPar;
      TMbsMappingTofPar   *fMbsMappingPar;
      
      // Histograms
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      void DeleteHistograms(); // TODO
      
      // Histograms objects
      std::vector< std::vector< Int_t > > fviNbHitInThisEvent;
      std::vector< TH2* > fhDetChEvtMul;
      std::vector< TH2* > fhDetChTotMul;
      std::vector< TH2* > fhDetChHitsPerEvt;
      std::vector< TH2* > fhDetChCoincSides;
      std::vector< TH2* > fhDetChCoincLeft;
      std::vector< TH2* > fhDetChCoincRight;
         // DEBUG
      std::vector< std::vector< Bool_t > > fbDebTdcChEvtThere[ toftdc::NbTdcTypes ];
      std::vector< TH2 * > fhDebTdcChEvtCoinc[ toftdc::NbTdcTypes ];
      
      // Input 
      Bool_t RegisterInput();
      
      // Input objects
      TClonesArray        * fCalibDataCollection;

      // For trigger rejection
      TClonesArray * fTriglogBoardCollection;
      
      // Output
      Bool_t RegisterOutput();
      Bool_t ClearOutput();
      
      // Output objects
      Bool_t                fbSaveMappedDigis;
      Bool_t                fbFillHistos;
      Bool_t                fbDuplicatePadDigis;
      TClonesArray        * fCbmTofDigiCollection;
      
      // Mapping
      Int_t GetTdcUniqueId( UInt_t uType, UInt_t uBoard, UInt_t uChannel = 0, UInt_t uEdge = 0 );
         // -----   Bit fields sizes   --------------------------------------------------
      static const Int_t fgkiTypeSize    =   4;
      static const Int_t fgkiBoardSize   =  12;
      static const Int_t fgkiChannelSize =  12;
      static const Int_t fgkEdgeSize     =   1;
         // -----------------------------------------------------------------------------
         // -----   Bit masks -----------------------------------------------------------
      static const Int_t fgklTypeMask;
      static const Int_t fgklBoardMask;
      static const Int_t fgklChannelMask;
      static const Int_t fgklEdgeMask;
         // -----------------------------------------------------------------------------
         // -----   Bit shifts   --------------------------------------------------------
      static const Int_t fgkiTypeOffs;
      static const Int_t fgkiBoardOffs;
      static const Int_t fgkiChannelOffs;
      static const Int_t fgkiEdgeOffs;
         // -----------------------------------------------------------------------------
      Bool_t MapTdcDataToDet();
      
      ClassDef(TMbsMappingTof, 1);
};
#endif // _TMBSMAPPINGTOF_H_ 
