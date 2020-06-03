#ifndef CBMTRBCALIBRATOR_H
#define CBMTRBCALIBRATOR_H

#include "TObject.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
#include "TROOT.h"

#include "CbmRichTrbDefines.h"

enum enu_calibMode {
   etn_IMPORT,  // import calibration tables from the file and use them
   etn_ONLINE,  // use first data to calibrate; the channel has to get at least fCalibrationPeriod messages to get calibrated
   etn_NOCALIB, // use linear function going from origin to (512, n) which means that the fine time is not calibrated
   etn_IDEAL,   // use almost linear function - close to real calibration but idealized
   etn_NOFINE   // ignore fine time counter at all
};

class CbmTrbCalibrator : public TObject
{
public: // methods
   /*
    * Destructor.
    */
   ~CbmTrbCalibrator();

   /*
    * Generate calibration tables. Not needed if they were imported from the external file.
    */
   void GenHistos();

   /*
    * Singleton class object returner.
    */
   static CbmTrbCalibrator* Instance();

   /*
    *
    */
   void SetMode(enu_calibMode mode) { fCalibMode = mode; }

   /*
    * Set the period of calibration.
    */
   void SetCalibrationPeriod(UInt_t period) { fCalibrationPeriod = period; }

   /*
    * Get the period of calibration.
    */
   //UInt_t GetCalibrationPeriod() { return fCalibrationPeriod; }

   /*
    * Add raw fine time received from TDC and unpacked by the CbmRichTrbUnpack.
    */
   void AddFineTime(UShort_t inTDCid, UShort_t inCHid, UShort_t fineCnt);
   
   /*
    * Return time in ns.
    * Fine time counter is 10 bits => UShort_t is enough.
    */
   Double_t GetFineTimeCalibrated(UShort_t inTDCid, UShort_t inCHid, UShort_t fineCnt);

   /*
    * Return full time in ns.
    * The calibration mode is chosen according to the singleton calibrator object state.
    */
   Double_t GetFullTime(UInt_t inTDCid, UInt_t inCHid, UInt_t epoch, UInt_t coarse, UInt_t fine);

   /*
    * Set the name of the file from which the calibration tables will be imported
    */
   void SetInputFilename(TString filename) { fInputFilename = filename; }

   /*
    * Set the name of the file from which the corrections will be imported.
    */
   void SetCorrInputFilename(TString filename) { fInputCorrFilename = filename; }

   /*
    * Import the calibration information from the root file.
    */
   void Import(/*const char* filename = "calibration.root"*/);

   /*
    * Export the calibration information into the root file.
    */
   void Export(const char* filename = "calibration.root");

   /*
    * Import per-channel corrections from the external file
    */
   void ImportCorrections();
   
   /*
    * Draw a canvas with the flags indicating whilch channels have been calibrated.
    */
   void Draw(Option_t* ="");

   /*
    * Force calibration of all the channels using the data already in the tables no matter what the period is.
    */
   void ForceCalibration();

private: // methods

   /*
    * Constructor - private because the class is singleton.
    */
   CbmTrbCalibrator();

   /*
    * Pseudo-calibration - used during debug.
    * Return time in ns.
    * Fine time counter is 10 bits => UShort_t is enough.
    * AlmostLinear has tails.
    */
   Double_t GetLinearCalibratedFT(UShort_t fineCnt);
   Double_t GetAlmostLinearCalibratedFT(UShort_t fineCnt);

   /*
    * Return really calibrated fine time. DoCalibrate() should have been called before.
    * If NextRawEvent() had been called for each event and calibrationPeriod is correctly defined
    * then the object of this class automatically handles calibration.
    * Return time in ns.
    * Fine time counter is 10 bits => UShort_t is enough.
    */
   Double_t GetRealCalibratedFT(UShort_t inTDCid, UShort_t inCHid, UShort_t fineCnt);

   /*
    * Per se calibration of a certain channel of a certain TDC of a certain TRB basing on the buffer accumulated by AddFineTime().
    */
   void DoCalibrate(UShort_t inTDCid, UShort_t inCHid);

private: // data members

   /*
    * Current calibration mode (see description of enu_calibMode)
    */
   enu_calibMode fCalibMode;

   /*
    * True if calibration histograms have already been created
    */
   Bool_t fTablesCreated;

   /*
    * Minimum number of fine time counters taken into account to start calibration.
    */
   UInt_t fCalibrationPeriod;

   /*
    * Root folder for all the calibration data.
    */
   TFolder* fTRBroot;

   /*
    * Input file name
    */
   TString fInputFilename;

   /*
    * Name of the input file with per-channel corrections
    */
   TString fInputCorrFilename;
   
   //FIXME temporary solution for 1 TRB setup - 8*8*2
   // Update for beamtime data - 4*4 PMTs camera. Each PMT 8*8 pixels. Each pixel has 2 edges. Thus 4*4*8*8*2
   Double_t fCorrections[4*4*8*8*2];

   /*
    * Calibration has already been done - use calibation table,
    * Otherwise - use linear claibration.
    */
   TH1C* fCalibrationDoneHisto[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC];

   /*
    * Flushed after calibration.
    */
   TH1D* fhLeadingFineBuffer[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC][TRB_TDC3_CHANNELS];
   
   /*
    * Accumulates across all the received data.
    */
   TH1D* fhLeadingFine[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC][TRB_TDC3_CHANNELS];

   /*
    * Renewes at calibration.
    * Initialized with `identity` (linear =1 function) before the first calibration.
    */
   TH1D* fhCalcBinWidth[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC][TRB_TDC3_CHANNELS];
	
   /*
    * Renewes at calibration.
    * Initialized with `identity` (linear y=k*x function) before the first calibration.
    */
   TH1D* fhCalBinTime[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC][TRB_TDC3_CHANNELS];
   
   /*
    * For each channel - counter of fine time counters taken into accout for calibration.
    */
   UInt_t fFTcounter[TRB_TDC3_NUMBOARDS][TRB_TDC3_NUMTDC][TRB_TDC3_CHANNELS];
   TH2D* fhFTcounter[TRB_TDC3_NUMBOARDS];

   /*
    * Pointer to the singleton class object.
    */
   static CbmTrbCalibrator* fInstance;

   CbmTrbCalibrator(const CbmTrbCalibrator&);
   CbmTrbCalibrator operator=(const CbmTrbCalibrator&);


   ClassDef(CbmTrbCalibrator,1)
};

#endif // CBMTRBCALIBRATOR_H
