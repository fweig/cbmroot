// ------------------------------------------------------------------
// -----                     TMbsCalibScalTof.h                 -----
// -----              Created 08/07/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------ 

#ifndef TMBSCALIBSCALTOF_H_
#define TMBSCALIBSCALTOF_H_

// TOF headers
#include "TofScalerDef.h"

// ROOT headers
#include "TObject.h"

// C/C++ headers
#include <vector>

class TMbsUnpackTofPar;
class TMbsCalibTofPar;

class TFile;
class TDirectory;
class TClonesArray;
class TH1;
class TH2;
class TProfile;
class TString;

class TMbsCalibScalTof : public TObject
{
   public:
      TMbsCalibScalTof();
      TMbsCalibScalTof( TMbsUnpackTofPar * parIn, TMbsCalibTofPar *parCalIn = NULL );
      ~TMbsCalibScalTof();
      
      virtual void Clear(Option_t *option);
      
      Bool_t RegisterInput();
      Bool_t RegisterOutput();
      void   SetSaveScalers( Bool_t bSaveScal=kTRUE );
      void   SetHistoUserAxis( Double_t dRangeIn, Double_t dBinSzIn )
             { fdEvoRangeUser = dRangeIn; fdEvoBinSzUser = dBinSzIn; };
      
      Bool_t InitScalersCalib();
      Bool_t CalibScalers();
      Bool_t ClearCalib();
      Bool_t CloseScalersCalib();
      
   
      // Histograms
      Bool_t CreateHistogramms();
      Bool_t FillHistograms();
      Bool_t WriteHistogramms( TDirectory* inDir);
      Bool_t DeleteHistograms();
      
   private:
      // no default Copy constructor and = OP as class not meant to be copied
      TMbsCalibScalTof(const TMbsCalibScalTof&);
      TMbsCalibScalTof& operator=(const TMbsCalibScalTof&);
      
      Bool_t InitParameters();
      TMbsUnpackTofPar    *fMbsUnpackPar;
      TMbsCalibTofPar     *fMbsCalibPar;
      
      // Histograms
      
      // Histograms objects
      std::vector< TH1* > fhRefClkRate;     // Only if Triglog present
      std::vector< TProfile* > fhRefClkRateEvo;  // Only if Triglog present
//      std::vector< TH2* > fhRefClkRateEstA;     // Only if Triglog present
//      std::vector< TH2* > fhRefClkRateEstB;     // Only if Triglog present
//      std::vector< TProfile* > fhRefClkRateEvo;  // Only if Triglog present
      std::vector< TH2* > fhRefMbsTimeComp; // Only if Triglog present
      std::vector< std::vector< std::vector< TH1* > > > fhScalersRate;    // [NbBoards][NbScalers][NbChan]
//      std::vector< std::vector< std::vector< TH1* > > > fhScalersRateEvo; // [NbBoards][NbScalers][NbChan]
      std::vector< std::vector< std::vector< TProfile* > > > fhScalersRateEvo; // [NbBoards][NbScalers][NbChan]

      // Histograms tuning
      Double_t fdEvoRangeUser;
      Double_t fdEvoBinSzUser;
      
      // Input 
      
      // Input objects
      TClonesArray * fScalerBoardCollection;
      TClonesArray * fTriglogBoardCollection;
      
      // Output
      Bool_t ClearOutput();
      
      // Output objects
      Bool_t         fbSaveCalibScalers;
      TClonesArray * fCalibScalCollection;
      
      // Calibration
      Bool_t InitCalibration();
      Bool_t Calibration( UInt_t uBoard);
      Bool_t CloseCalibration();
      
      // Calibration objects
      Bool_t bFirstEvent;
      Bool_t bFirstEvent2014;
         // First reference clock number [NbBoards]
      std::vector< UInt_t >   fvuFirstRefClk;  
         // Number of times the reference clock scaler made a cycle [NbBoards]
      std::vector< UInt_t >   fvuRefClkCycle;
         // last reference clock number [NbBoards]          
      std::vector< UInt_t >   fvuLastRefClk;                
         // First value of each scaler <= integrated counts [NbBoards][NbScalers][NbChan]
      std::vector< std::vector< std::vector< UInt_t > > > fvuLastScalers;  
      
      // Clock Calibration objects, used only if TRIGLOG board is present
      Double_t fdFirstMbsTime;        // First point for rate measurement
      Double_t fdPrevMbsTime;         // previous point for rate measurement
      std::vector< Double_t > fvdPrevMbsTimeBd;   // previous point for ref clockrate measurement
      std::vector< UInt_t >   fvuLastRefClkCal;   // last reference clock number used in frequency calibration [NbBoards]          
      std::vector< Double_t > fvdMeanRefClkFreq;  // Only if Triglog present
      
   ClassDef(TMbsCalibScalTof, 1)
};

#endif // TMBSCALIBSCALTOF_H_  
