// ------------------------------------------------------------------
// -----                     TMbsCalibTofPar                   -----
// -----              Created 08/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef _TMBSCALIBTOFPAR_H_
#define _TMBSCALIBTOFPAR_H_

#include "TofTdcDef.h"

#include "FairParGenericSet.h"
#include "TArrayI.h"
#include "TArrayD.h"

class TMbsCalibTofPar : public FairParGenericSet 
{
   public:
      TMbsCalibTofPar() ;

      TMbsCalibTofPar( const char* name,
                      const char* title="Mbs Calib parameters for the ToF",
                      const char* context="TestDefaultContext" );

      virtual ~TMbsCalibTofPar();
      
      // Accessors
      Bool_t  IsDebug() const { return (1 == fuCalibDebug)? kTRUE: kFALSE; };
      
      Bool_t  CalibRefClocks() const {return (1 == fuCalibScalRefClk)? kTRUE: kFALSE; };
      
      Int_t   GetMinHitCalib() const { return fiMinNbHitsCalib; };
      Int_t   GetMinHitCalibNewOnly() const { return fiFactMinNbHitsOnlyNewCalib*fiMinNbHitsCalib; };
      TString GetInitCalFilename() const {return fsInitialCalibrationFilename; };
      Int_t   GetNbCalibBoards( UInt_t uType) const;
      Int_t   GetInitialCalInd( UInt_t uType, UInt_t uBoard ) const;
      Bool_t  EnaCalibOutput() const 
         { return (1 == fuEnableCalibOutput)? kTRUE: kFALSE; };
      Bool_t  EnaSingleCalib() const 
         { return (1 == fuSingleChannelCalibFilesEnable)? kTRUE: kFALSE; };
      Bool_t  EnaSingleCalibOutput() const 
         { return (0 < fuSingleChannelCalibFilesOutput)? kTRUE: kFALSE; };
      Int_t   GetSingleCalOutMode() const { return fuSingleChannelCalibFilesOutput; };
      
      Bool_t  UseCoarse() const 
         { return (1 == fuUseCoarseCorrectedTime)? kTRUE: kFALSE; };
      
      Int_t   GetNbOffsetBoard( UInt_t uType) const;
      Int_t   GetOffsetBoardInd( UInt_t uType, UInt_t uSlot ) const;
      Bool_t  EnaTimeOffset() const 
         { return (1 == fuEnableTimeOffsetCalibration)? kTRUE: kFALSE; };
      Int_t   GetTimeOffsetVal( UInt_t uType, UInt_t uSlot, UInt_t uChan ) const;
      Bool_t  EnaTotOffset() const 
         { return (1 == fuEnableTotCalibration)? kTRUE: kFALSE; };
      Int_t   GetTotOffsetVal( UInt_t uType, UInt_t uSlot, UInt_t uChan ) const;
      
      Bool_t  EnaManuTdcTdcOffset() const 
         { return (1 == fuManualTdcOffsetEnable)? kTRUE: kFALSE; };
      Int_t   GetTdcTdcOffset( UInt_t uType, UInt_t uBoard ) const;
      Bool_t  EnaAutoTdcTdcOffset() const 
         { return (1 == fuAutomaticOffsetEnable)? kTRUE: kFALSE; };
      Int_t   GetMainTdcTdcOffset( UInt_t uType ) const;
      
      Int_t   GetTotMode( UInt_t uType ) const;
      Bool_t  CheckTotInvFlag( UInt_t uType, UInt_t uSlot ) const;
      Int_t   GetTotInvFlag( UInt_t uType, UInt_t uSlot, UInt_t uChan ) const;
         
      Bool_t  IsTimeHistEna() const 
         { return (1 == fuTimeHistEnable)? kTRUE: kFALSE; };
      Bool_t  IsSingleTimeHistEna() const 
         { return (1 == fuTimeHistSinglesEnable)? kTRUE: kFALSE; };
      Bool_t  IsTotHistEna() const 
         { return (1 == fuTotHistoEnable)? kTRUE: kFALSE; };
      Bool_t  IsMultiDistHistEna() const 
         { return (1 == fuMultiHitsDistanceHistoEnable)? kTRUE: kFALSE; };
      
      Bool_t  IsTdcOffsetEna() const 
         { return (1 == fuTdcOffsetEnable)? kTRUE: kFALSE; };
      Bool_t  GetTdcOffEnaFlag( UInt_t uType, UInt_t uBoard ) const;
      UInt_t  GetTdcOffsetMainTdc() const
         { return fuTdcOffsetMainTdc; };
      Bool_t  IsTrbLinCalEna() const 
         { return (1 == fuTrbOffsetLinearEnable)? kTRUE: kFALSE; };
      Int_t   GetChanResTest() const { return fiChannelResolutionTest;};

      // Example taken from CbmTofDigiPar
      void clear(void);
      void putParams(FairParamList*);
      Bool_t getParams(FairParamList*);
      virtual void printParams();
    
   private:
      /** DEBUG */
         // Switch ON/OFF debug tests, output & histos in calibration
      UInt_t   fuCalibDebug; 

      /** Scalers Calib params **/
         // If 1, the calibrator will try to update the reference clock frequency using the reference counts
         // and the MBS LINUX clock saved in the TRIGLOG event. As this clock as only a bining of 1ms, the rate is 
         // calculated at time intervals of 50ms to 500ms 
      UInt_t   fuCalibScalRefClk;
      /** TDC Calib params **/
         // Minimal number of hits in the finetime histogram before we start applying corrections based on New data 
         // Corrections are updated on multiples of this value
      Int_t    fiMinNbHitsCalib;   
         // Number of time the minimal number of hits after which corrections use only new data
         // 0 desactivate and when initial calibration is available use always its addition with new data 
      Int_t    fiFactMinNbHitsOnlyNewCalib;
      
         // Calibration loading
            // If not empty or -, use the bin to ns calibration  histograms from this file as initial calibration
      TString  fsInitialCalibrationFilename;
            // Nb of boards for which the calibration where measured
      Int_t    fiNbVftxCalib;
      Int_t    fiNbCaenCalib;
      Int_t    fiNbTrb3Calib;
      Int_t    fiNbGet4Calib;
            // Tdc index of the initial calibration histograms for individual VFTX 
            // (should allow board slot change if calibration stable over time)
      TArrayI  fiInitialCalIndexVftx;
      TArrayI  fiInitialCalIndexCaen;
      TArrayI  fiInitialCalIndexTrb3;
      TArrayI  fiInitialCalIndexGet4;
            // Save all bin to ns calibration histos to a root file with date/time as name
      UInt_t   fuEnableCalibOutput;
            // Enable use a one calibration file per channel in ./Calib/ folder
            // Name is built using tdc type + curr. board index + channel index
      UInt_t   fuSingleChannelCalibFilesEnable;
            // 0 = Not output of single channel calib file, 1 = single channel calib file with new data, 2 = Old + new
      UInt_t   fuSingleChannelCalibFilesOutput;
      
         // Switch between calibrated time and calibrated time with correction for coarse counter cycle (relative to trigger)
      UInt_t   fuUseCoarseCorrectedTime;
      
         // Board/chip dependent offsets (constant over time!!!)
            // Nb of board for which the offset where measured
      Int_t    fiNbVftxOffset;
      Int_t    fiNbCaenOffset;
      Int_t    fiNbTrb3Offset;
      Int_t    fiNbGet4Offset;
            // Tdc hardware board #X in slot/data #Y
            // (should allow board slot change)
      TArrayI  fiBoardIndexVftx;
      TArrayI  fiBoardIndexCaen;
      TArrayI  fiBoardIndexTrb3;
      TArrayI  fiBoardIndexGet4;
            // Time offset calibration
               // Enable/disable time offset calibration
      UInt_t   fuEnableTimeOffsetCalibration;
               // Time offset per channel, depends on TDC board, not TDC slot!!!!!
      TArrayD  fdTimeOffsetVftx;
      TArrayD  fdTimeOffsetCaen; 
      TArrayD  fdTimeOffsetTrb3;
      TArrayD  fdTimeOffsetGet4;
               // Time offset per channel, independent of TDC board, this is a mean value for each TDC type!!!!!
      TArrayD  fdMeanTimeOffsetVftx;
      TArrayD  fdMeanTimeOffsetCaen;
      TArrayD  fdMeanTimeOffsetTrb3;
      TArrayD  fdMeanTimeOffsetGet4;

         // ToT offset calibration
            // Enable/disable tot offset calibration
      UInt_t   fuEnableTotCalibration;
            // ToT offset per channel, depends on TDC board, not TDC slot!!!!!
      TArrayD  fdTotOffsetVftx;
      TArrayD  fdTotOffsetCaen;
      TArrayD  fdTotOffsetTrb3;
      TArrayD  fdTotOffsetGet4;
            // ToT offset per channel, independent of TDC board, this is a mean value for each TDC type!!!!!
      TArrayD  fdMeanTotOffsetVftx;
      TArrayD  fdMeanTotOffsetCaen;
      TArrayD  fdMeanTotOffsetTrb3;
      TArrayD  fdMeanTotOffsetGet4;
      
         // TDC to TDC offsets, e.g. due to diff. clock cycle locking on reset
            // Enable a manual X clock TDC offset use
      UInt_t   fuManualTdcOffsetEnable;
            // Offset to main TDC, in clock period
      TArrayI  fiTdcToTdcOffsetVftx;
      TArrayI  fiTdcToTdcOffsetCaen;
      TArrayI  fiTdcToTdcOffsetTrb3;
      TArrayI  fiTdcToTdcOffsetGet4;
            // Enable an automatic X clock TDC offset detection
      UInt_t   fuAutomaticOffsetEnable;
            // TDC to be used for the automatic offset detection
      Int_t   fiMainReferenceTdcVftx;
      Int_t   fiMainReferenceTdcCaen;
      Int_t   fiMainReferenceTdcTrb3;
      Int_t   fiMainReferenceTdcGet4;
      
      /** Tot calculation **/
         // Tot calculation mode
         //    0 = None
         //    1 = Same TDC channel, diff data
         //    2 = 2 TDC channels per input channel or 2 input channels per front end channel (even = Falling, odd = Rising)
         //    3 = Same input channels in 2 consecutive boards (even = Rising, odd = Falling)
         //    4 = Same TDC channel, same data
      Int_t   fiTotModeVftx;
      Int_t   fiTotModeCaen;
      Int_t   fiTotModeTrb3;
      Int_t   fiTotModeGet4;
         // Nb of board for which inversion flags are provided
      Int_t    fiNbVftxTotInvFlag;
      Int_t    fiNbCaenTotInvFlag;
      Int_t    fiNbTrb3TotInvFlag;
      Int_t    fiNbGet4TotInvFlag;
         // Tot inversion flags ( default is signal not inverted)
      TArrayI  fiTotInversionFlagVftx;
      TArrayI  fiTotInversionFlagCaen;
      TArrayI  fiTotInversionFlagTrb3;
      TArrayI  fiTotInversionFlagGet4;
      
      /** Histos control **/
         // Trigger time - calibrated time Histograms for every TDC channel
      UInt_t   fuTimeHistEnable;
         // Trigger time - calibrated time Histograms for every TDC channel with only 1 hit in event
      UInt_t   fuTimeHistSinglesEnable;
         // Tot Histograms for every input channel if possible/available (depends on TDC type)
      UInt_t   fuTotHistoEnable;
         // Time distance between consecutive multiple hits on same channel
      UInt_t   fuMultiHitsDistanceHistoEnable;

      /** TDC offsets **/
         // Enable the use of TDC to TDC offsets for each event using reference channel
      UInt_t   fuTdcOffsetEnable;
         // If the use of TDC to TDC offsets from ref ch is not enabled globally, enable it on a TDC basis
      TArrayI  fiTdcOffsetEnaFlagVftx;
      TArrayI  fiTdcOffsetEnaFlagCaen;
      TArrayI  fiTdcOffsetEnaFlagTrb3;
      TArrayI  fiTdcOffsetEnaFlagGet4;
      // Index of the TDC to be used as main reference for the TDC to TDC offset using reference channel
      UInt_t   fuTdcOffsetMainTdc;
         // Enable the use of linear calibration for the TRB3-TDC reference channel time calculation
      UInt_t   fuTrbOffsetLinearEnable;
         // DEBUG: Channel to be used in all TDCs to build a TDC to TDC resolution check, -1 is disable!!!!
      Int_t    fiChannelResolutionTest;

      TMbsCalibTofPar(const TMbsCalibTofPar&);
      TMbsCalibTofPar& operator=(const TMbsCalibTofPar&);
      void FillBoardsNbPerType();
    
  ClassDef(TMbsCalibTofPar, 2)
};

#endif //TMBSCALIBTOFPAR_H
   
