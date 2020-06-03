// ------------------------------------------------------------------
// -----                     TMbsCalibTdcTof.h                  -----
// -----              Created 20/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TMBSCALIBTDCTOF_H_
#define TMBSCALIBTDCTOF_H_

#include "TObject.h"

#include <vector>

#include "TofTdcDef.h"

class TMbsUnpackTofPar;
class TMbsCalibTofPar;

class TTofCalibData;

class TFile;
class TDirectory;
class TClonesArray;
class TH1;
class TH2;
class TString;
class TProfile;

class TMbsCalibTdcTof : public TObject
{
   public:
      TMbsCalibTdcTof();
      TMbsCalibTdcTof( TMbsUnpackTofPar * parIn, TMbsCalibTofPar *parCalIn = NULL );
      ~TMbsCalibTdcTof();
      
      virtual void Clear(Option_t *option);
      
      Bool_t RegisterInput();
      Bool_t RegisterOutput();
      void   SetSaveTdcs( Bool_t bSaveTdcs=kTRUE );
      Bool_t InitiTdcCalib();
      Bool_t CalibTdc();
      Bool_t ClearCalib();
      Bool_t CloseTdcCalib();
      void  SetCalibFilename( TString sFilenameIn = "" );
      void  SetCalibOutFolder( TString sFoldernameIn = "" );

      // Fix for big trigger time deviation in GSI Sep14 data
      void  SetSep14Fix( Bool_t inGsiSep14Fix = kTRUE) { fbGsiSep14Fix = inGsiSep14Fix; };
   
      // Calibration mode where only the reference channels are processed (Calib and histo filling)
      void  EnaTdcRefMoniMode( Bool_t inTdcRefMoni = kTRUE) { fbTdcRefMoniMode = inTdcRefMoni; };

      // Histograms
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      Bool_t WriteHistogramms( TDirectory* inDir);
      Bool_t DeleteHistograms();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TMbsCalibTdcTof(const TMbsCalibTdcTof&);
      TMbsCalibTdcTof& operator=(const TMbsCalibTdcTof&);
      
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      TMbsCalibTofPar     *fMbsCalibPar;

      // Fix for big trigger time deviation in GSI Sep14 data
      Bool_t fbGsiSep14Fix;
      Int_t  fiOffsetGsiSep14Fix; // common offset for all crazy boards

      // Calibration mode where only the reference channels are processed (Calib and histo filling)
      Bool_t fbTdcRefMoniMode;
      
      // Histograms
      
      // Histograms objects
      std::vector< TH1* > fhDnlChan[ toftdc::NbTdcTypes ];
      std::vector< TH1* > fhDnlSum[ toftdc::NbTdcTypes ];
      std::vector< TH1* > fhBinSizeChan[ toftdc::NbTdcTypes ];
      std::vector< TH1* > fhTimeToTrigg[ toftdc::NbTdcTypes ];
      std::vector< TH1* > fhTimeToTriggSingles[ toftdc::NbTdcTypes ];
      std::vector< TH1* > fhToT[ toftdc::NbTdcTypes ];
      std::vector< TH2* > fhMultiDist[ toftdc::NbTdcTypes ];
      std::vector< TH2* > fhMultiplicity[ toftdc::NbTdcTypes ];
         // For reference channels comparison
      std::vector< TH2* > fhTdcReferenceComp[ toftdc::NbTdcTypes ];
      std::vector< TH2* > fhTdcResolutionTest[ toftdc::NbTdcTypes ];
      std::vector< TH1* > fhTdcTestToRef[ toftdc::NbTdcTypes ];
      
      // Input 
      Bool_t CheckAllTdcValid();
      
      // Input objects
      TClonesArray        * fCaenBoardCollection;
      TClonesArray        * fVftxBoardCollection;
      TClonesArray        * fTrb3BoardCollection;
      TClonesArray        * fGet4BoardCollection;
      
      // Output
      Bool_t ClearOutput();
      
      // Output objects
      Bool_t                fbSaveCalibTdcs;
      TClonesArray        * fCalibDataCollection;
      TClonesArray        * fCalibRefCollection[ toftdc::NbTdcTypes ];
      
      // Calibration
      Bool_t InitCalibration();
      Bool_t GetHistosFromUnpack();
      Bool_t LoadCalibrationFile();
      Bool_t LoadSingleCalibrations();
      Bool_t CalibFactorsInit( UInt_t uType, UInt_t uBoard, UInt_t uChan );
      Bool_t Calibration( UInt_t uType, UInt_t uBoard);
      Bool_t CalibFactorsCalc( UInt_t uType, UInt_t uBoard, UInt_t uChan, Bool_t bWithInitial );
      Bool_t WriteCalibrationFile();
      Bool_t WriteSingleCalibrations();
      
      // Calibration objects      
      TFile* fileCalibrationIn;
      TDirectory* oldDir;
      TString fsCalibOutFoldername;
      TString fsCalibFilename;
      std::vector< TH1* >     fhInitialCalibHisto[ toftdc::NbTdcTypes ];
      std::vector< TH1* >     fhFineTime[ toftdc::NbTdcTypes ];
      std::vector< Int_t >    fiNbHitsForCalib[ toftdc::NbTdcTypes ];
      std::vector< Bool_t >   fbCalibAvailable[ toftdc::NbTdcTypes ];
      std::vector< std::vector< Double_t > > fdCorr[ toftdc::NbTdcTypes ];
      std::vector< Double_t > fdBoardTriggerTime[ toftdc::NbTdcTypes ];
      
      // Tot calculation
      Bool_t CreateTotVariables();
      Bool_t ClearTotVariables();
      Bool_t DeleteTotVariables();
      Bool_t BuildTotSplitChannels( UInt_t uType, UInt_t uBoard );
      Bool_t BuildTotSplitBoards( UInt_t uType  );
      // Tot objects
      TClonesArray        * xTofTdcDataPrevArray[ toftdc::NbTdcTypes ];
      TClonesArray        * xTempCalibData[ toftdc::NbTdcTypes ];
      
      // TDC to TDC offset objects   
      Bool_t GetRefHistosFromUnpack();
      Bool_t LoadCalibrationFileRef(); // TODO in a smarter way ???
      Bool_t LoadSingleCalibrationsRef();
      Bool_t CalibFactorsInitReference( UInt_t uType, UInt_t uBoard);
      Bool_t CalibFactorsCalcReference( UInt_t uType, UInt_t uBoard, Bool_t bWithInitial );
      Bool_t WriteCalibrationFileRef( TString outDir ); // TODO in a smart way ???
      Bool_t WriteSingleCalibrationsRef();
      Bool_t CalibrateReference(UInt_t uType, UInt_t uBoard);
      Bool_t ClearTdcReference();
      Bool_t TdcOffsetCalc(UInt_t uType, UInt_t uBoard);
      Bool_t CreateReferenceHistogramms();
      Bool_t FillReferenceHistograms();
      Bool_t WriteReferenceHistogramms( TDirectory* inDir);
      Bool_t DeleteReferenceHistograms();

      std::vector< TH1* >     fhInitialCalibHistoRef[ toftdc::NbTdcTypes ];
      std::vector< TH1* >     fhFineTimeRef[ toftdc::NbTdcTypes ];
      std::vector< Int_t >    fiLinCalRefMinBin[ toftdc::NbTdcTypes ]; 
      std::vector< Int_t >    fiLinCalRefMaxBin[ toftdc::NbTdcTypes ]; 
      std::vector< std::vector< Double_t > > fdCorrRef[ toftdc::NbTdcTypes ];
      std::vector< Double_t > fdTdcReference[ toftdc::NbTdcTypes ]; 
      std::vector< Double_t > fdTdcOffsets[ toftdc::NbTdcTypes ]; 
      Bool_t fbFirstEventPassedRef[ toftdc::NbTdcTypes ];
      std::vector< Double_t > fdTdcReferenceFirstEvent[ toftdc::NbTdcTypes ];
      TProfile*               fhTdcOffsetFirstEvent[ toftdc::NbTdcTypes ];

   ClassDef(TMbsCalibTdcTof, 1)
};

#endif // TMBSCALIBTDCTOF_H_ 
