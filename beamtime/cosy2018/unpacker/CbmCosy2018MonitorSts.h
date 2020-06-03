// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorSts                         -----
// -----                Created 15/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMCOSY2018MONITORSTS_H
#define CBMCOSY2018MONITORSTS_H

#include "Timeslice.hpp"

// Data
#include "StsXyterMessage.h"
#include "StsXyterFinalHit.h"

// CbmRoot
#include "CbmTSUnpack.h"
#include "CbmHistManager.h"

// C++11
#include <chrono>

// C/C++
#include <vector>
#include <set>
#include <map>

class CbmCern2017UnpackParSts;

class CbmCosy2018MonitorSts: public CbmTSUnpack
{
public:

   CbmCosy2018MonitorSts();
   virtual ~CbmCosy2018MonitorSts();

   virtual Bool_t Init();

   virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);

   virtual void Reset();

   virtual void Finish();

   void SetParContainers();

   Bool_t InitContainers();

   Bool_t ReInitContainers();

   void FillOutput(boost::any);

   void SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize = 5 );

   void ResetAllHistos();
   void SaveAllHistos( TString sFileName = "" );

   void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
   size_t GetMsOverlap()                      { return fuOverlapMsNb; }

   void SetPrintMessage( Bool_t bPrintMessOn = kTRUE,
                         stsxyter::MessagePrintMask ctrl = stsxyter::MessagePrintMask::msg_print_Hex |
                                                           stsxyter::MessagePrintMask::msg_print_Human )
                        { fbPrintMessages = bPrintMessOn; fPrintMessCtrl = ctrl; }
   void EnableChanHitDtPlot( Bool_t bEnable = kTRUE ) { fbChanHitDtEna = bEnable; }
   void EnableDualStsMode( Bool_t bEnable = kTRUE ) { fbDualStsEna = bEnable; }
   void SetLongDurationLimits( UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1 );

   void SetStripsOffset1( Int_t iOffN, Int_t iOffP ) { fiStripsOffsetN1 = iOffN; fiStripsOffsetP1 = iOffP; }
   void SetStripsOffset2( Int_t iOffN, Int_t iOffP ) { fiStripsOffsetN2 = iOffN; fiStripsOffsetP2 = iOffP; }

private:
   size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

   // Parameters
   CbmCern2017UnpackParSts* fUnpackPar; //!
   UInt_t                   fuNrOfDpbs;       //! Total number of Sts DPBs in system
   std::map<UInt_t, UInt_t> fDpbIdIndexMap;   //! Map of DPB Identifier to DPB index
   UInt_t                   fuNbElinksPerDpb; //! Number of possible eLinks per DPB
   UInt_t                   fuNbStsXyters;    //! Number of StsXyter ASICs
   UInt_t                   fuNbChanPerAsic;  //! Number of channels per StsXyter ASIC => should be constant somewhere!!!!
   std::vector< std::vector< UInt_t > > fvuElinkToAsic;   //! Vector holding for each link the corresponding ASIC index [fuNrOfDpbs * fuNbElinksPerDpb]

   // Constants
   static const Int_t    kiMaxNbFlibLinks = 16;
   static const UInt_t   kuBytesPerMessage = 4;

   // Internal Control/status of monitor
      // Task configuration values
   Bool_t                fbPrintMessages;
   stsxyter::MessagePrintMask fPrintMessCtrl;
   Bool_t                fbChanHitDtEna;
   Bool_t                fbDualStsEna;
      // TS/MS info
   ULong64_t             fulCurrentTsIdx;
   ULong64_t             fulCurrentMsIdx;
      // Current data properties
   std::map< stsxyter::MessType, UInt_t > fmMsgCounter;
   UInt_t                fuCurrentEquipmentId;  //! Current equipment ID, tells from which DPB the current MS is originating
   UInt_t                fuCurrDpbId; //! Temp holder until Current equipment ID is properly filled in MS
   UInt_t                fuCurrDpbIdx;          //! Index of the DPB from which the MS currently unpacked is coming
   Int_t                 fiRunStartDateTimeSec; //! Start of run time since "epoch" in s, for the plots with date as X axis
   Int_t                 fiBinSizeDatePlots;    //! Bin size in s for the plots with date as X axis
//   std::vector< std::vector< UInt_t > > fvuCurrentTsMsb;       //! Current TS MSB for each eLink
      // Data format control
   std::vector< ULong64_t > fvulCurrentTsMsb;                   //! Current TS MSB for each DPB
   std::vector< UInt_t    > fvuCurrentTsMsbCycle;               //! Current TS MSB cycle for DPB
   std::vector< UInt_t    > fvuElinkLastTsHit;                  //! TS from last hit for DPB
      // Hits comparison
   std::vector< std::vector< ULong64_t > > fvulChanLastHitTime; //! Last hit time in bins for each Channel
   std::vector< std::vector< Double_t  > > fvdChanLastHitTime;  //! Last hit time in ns   for each Channel
   std::vector< Double_t >                               fvdMsTime;              //! Header time of each MS
   std::vector< std::vector< std::vector< UInt_t > > >   fvuChanNbHitsInMs;      //! Number of hits in each MS for each Channel
   std::vector< std::vector< std::vector< Double_t > > > fvdChanLastHitTimeInMs; //! Last hit time in bins in each MS for each Channel
   std::vector< std::vector< std::vector< UShort_t > > > fvusChanLastHitAdcInMs; //! Last hit ADC in bins in each MS for each Channel
//   std::vector< std::vector< std::multiset< stsxyter::FinalHit > > > fvmChanHitsInTs; //! All hits (time & ADC) in bins in last TS for each Channel
      // Starting state book-keeping
   Double_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
   Double_t              fdStartTimeMsSz;       /** Time of first microslice, used as reference for evolution plots**/
   std::chrono::steady_clock::time_point ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

      // Hits time-sorting
   std::multiset< stsxyter::FinalHit > fvmHitsInTs; //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted by multiset with "<" operator
   stsxyter::FinalHit fLastSortedHit1N; //! Last sorted hit for STS 1 N
   stsxyter::FinalHit fLastSortedHit1P; //! Last sorted hit for STS 1 P
   stsxyter::FinalHit fLastSortedHit2N; //! Last sorted hit for STS 2 N
   stsxyter::FinalHit fLastSortedHit2P; //! Last sorted hit for STS 2 P
      // Coincidence histos
   UInt_t fuMaxNbMicroslices;

   // Histograms
   CbmHistManager* fHM;                 //! Histogram manager
   TH1* fhStsMessType;
   TH1* fhStsSysMessType;
   TH2* fhStsMessTypePerDpb;
   TH2* fhStsSysMessTypePerDpb;
   TH2* fhStsDpbRawTsMsb;
   TH2* fhStsDpbRawTsMsbSx;
   TH2* fhStsDpbRawTsMsbDpb;
   TH2* fhStsMessTypePerElink;
   TH2* fhStsSysMessTypePerElink;

   std::vector<TH1*> fhStsChanCounts;
   std::vector<TH2*> fhStsChanRawAdc;
   std::vector<TProfile*> fhStsChanRawAdcProf;
   std::vector<TH2*> fhStsChanRawTs;
   std::vector<TH2*> fhStsChanMissEvt;
   std::vector<TH2*> fhStsChanHitRateEvo;
   std::vector<TH1*> fhStsXyterRateEvo;
   std::vector<TH2*> fhStsChanHitRateEvoLong;
   std::vector<TH1*> fhStsXyterRateEvoLong;
   std::vector<TH2*> fhStsChanHitDt;
   std::vector<TH2*> fhStsChanHitDtNeg;
   std::vector<TH2*> fhStsChanHitsPerMs;
   TH2* fhStsAsicTsMsb;

   Bool_t fbLongHistoEnable;
   UInt_t fuLongHistoNbSeconds;
   UInt_t fuLongHistoBinSizeSec;
   UInt_t fuLongHistoBinNb;
   std::vector<TH1*> fhFebRateEvoLong;
   std::vector<TH2*> fhFebChRateEvoLong;
/*
   std::vector<TH1*> fhFebRateEvoDate;
*/

      // Coincidences in sorted hits
   Double_t fdCoincBorder = 50.0; // ns, +/-
   TH1 * fhStsSortedDtN1P1;
   TH1 * fhStsSortedDtN2P2;
   TH1 * fhStsSortedDtN1N2;
   TH1 * fhStsSortedDtP1P2;
   TH1 * fhStsSortedDtN1P2;
   TH1 * fhStsSortedDtP1N2;
   TH2 * fhStsSortedMapN1P1;
   TH2 * fhStsSortedMapN2P2;
   TH2 * fhStsSortedMapN1N2;
   TH2 * fhStsSortedMapP1P2;
   TH2 * fhStsSortedMapN1P2;
   TH2 * fhStsSortedMapP1N2;
   TH1 * fhStsSortedCntEvoN1P1;
   TH1 * fhStsSortedCntEvoN2P2;
   TH1 * fhStsSortedCntEvoN1N2;
   TH1 * fhStsSortedCntEvoP1P2;
   TH1 * fhStsSortedCntEvoN1P2;
   TH1 * fhStsSortedCntEvoP1N2;

   TH1 * fhStsSortedDtN1P1N2P2;
   TH2 * fhStsSortedMapN1P1Coinc;
   TH2 * fhStsSortedMapN2P2Coinc;
   TH2 * fhStsSortedDtVsDiffN;
   TH2 * fhStsSortedDtVsDiffP;

   const Double_t kdStereoAngle =    7.5; // [Deg]
   Double_t       fdStereoAngleTan;
   const Double_t kdPitchMm     =    0.058; // [mm]
   const Int_t    kiNbStrips    = 1024;
   const Double_t kdSensorsSzX  =   60; // [mm], active is 59.570 mm (kiNbStrips*kdPitchMm)
   const Double_t kdSensorsSzY  =   40; // [mm], active is 39.703 mm
   Int_t fiStripsOffsetN1; // of channel 0 relative to center strip
   Int_t fiStripsOffsetP1; // of channel 0 relative to center strip
   Int_t fiStripsOffsetN2; // of channel 0 relative to center strip
   Int_t fiStripsOffsetP2; // of channel 0 relative to center strip
   TH2 * fhStsSortedMapX1Y1;
   TH2 * fhStsSortedMapX2Y2;
   TH2 * fhStsSortedMapX1Y1Coinc;
   TH2 * fhStsSortedMapX2Y2Coinc;

   TCanvas*  fcMsSizeAll;
   TH1*      fhMsSz[kiMaxNbFlibLinks];
   TProfile* fhMsSzTime[kiMaxNbFlibLinks];

   void CreateHistograms();

   void FillHitInfo(   stsxyter::Message mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx, const UInt_t & uMsIdx );
   void FillTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
   void FillEpochInfo( stsxyter::Message mess );

   void ComputeCoordinatesSensor1( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );
   void ComputeCoordinatesSensor2( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );

   CbmCosy2018MonitorSts(const CbmCosy2018MonitorSts&);
   CbmCosy2018MonitorSts operator=(const CbmCosy2018MonitorSts&);

   ClassDef(CbmCosy2018MonitorSts, 1)
};

#endif // CBMCOSY2018MONITORSTS_H
