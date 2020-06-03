// ------------------------------------------------------------------
// -----                    TMbsCalibTof                       -----
// -----              Created 21/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _TMBSCALIBTOF_H_
#define _TMBSCALIBTOF_H_

#include "FairTask.h"

#include <vector>

#include "TofTdcDef.h"

class TMbsUnpackTofPar;
class TMbsCalibTofPar;

class TMbsCalibTdcTof;
class TTofCalibData;

class TMbsCalibScalTof;
class TTofCalibScaler;

class TFile;
class TDirectory;
class TClonesArray;
class TH1;
class TH2;
class TString;

class TMbsCalibTof : public FairTask {
   public:
      TMbsCalibTof();
      TMbsCalibTof(const char* name, Int_t mode = 1, Int_t verbose = 1);
      virtual ~TMbsCalibTof();

      // Fairtask specific functions
      virtual void SetParContainers();
      virtual InitStatus Init();
      virtual InitStatus ReInit();
      virtual void Exec(Option_t* option);
      virtual void Finish();
      
      void SetSaveScalers( Bool_t bSaveScal=kTRUE ) {fbSaveCalibScalers = bSaveScal; };
      void SetScalHistoUserAxis( Double_t dRangeIn, Double_t dBinSzIn )
             { fdScalersEvoRangeUser = dRangeIn; fdScalersEvoBinSzUser = dBinSzIn; };

      void SetSaveTdcs( Bool_t bSaveTdcs=kTRUE ) {fbSaveCalibTdcs = bSaveTdcs; };
      void SetTdcCalibFilename( TString sFilenameIn="" ) {fsTdcCalibFilename = sFilenameIn; };
      void SetTdcCalibOutFoldername( TString sFoldernameIn="" ) {fsTdcCalibOutFoldername = sFoldernameIn; };

      // Fix for big trigger time deviation in GSI Sep14 data
      void  SetTdcCalibSep14Fix( Bool_t inGsiSep14Fix = kTRUE) { fbTdcCalibGsiSep14Fix = inGsiSep14Fix; };

      // Calibration mode where only the reference channels are processed (Calib and histo filling)
      void  EnaTdcRefMoniMode( Bool_t inTdcRefMoni = kTRUE) { fbTdcRefMoniMode = inTdcRefMoni; };

   private:
      TMbsCalibTof(const TMbsCalibTof&);
      TMbsCalibTof operator=(const TMbsCalibTof&);
      
      // Parameters
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      TMbsCalibTofPar     *fMbsCalibPar;
      
      // Calibration Processors
      Bool_t CreateCalibrators();
      Bool_t InitCalibrators();
      Bool_t ClearCalibrators();
      Bool_t ExecCalibrators();
      Bool_t CloseCalibrators();
      TMbsCalibTdcTof     *fTdcCalibrator;
      TMbsCalibScalTof    *fScalerCalibrator;
      
      // Histograms, including Task own histos
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      void  WriteHistogramms();
      void  DeleteHistograms();
      
      // Input 
      Bool_t RegisterInput();
      
      // Output
      Bool_t RegisterOutput();
      Bool_t ClearOutput();
      
      // For trigger rejection
      TClonesArray * fTriglogBoardCollection;

      Bool_t fbSaveCalibScalers;
      Double_t fdScalersEvoRangeUser;
      Double_t fdScalersEvoBinSzUser;

      Bool_t fbSaveCalibTdcs;
      TString fsTdcCalibOutFoldername;
      TString fsTdcCalibFilename;

      // Fix for big trigger time deviation in GSI Sep14 data
      Bool_t fbTdcCalibGsiSep14Fix;

      // Calibration mode where only the reference channels are processed (Calib and histo filling)
      Bool_t fbTdcRefMoniMode;

      ClassDef(TMbsCalibTof, 1);
};
#endif // _TMBSCALIBTOF_H_ 
