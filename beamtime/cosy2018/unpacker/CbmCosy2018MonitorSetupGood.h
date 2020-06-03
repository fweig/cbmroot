// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorSetupGood                        -----
// -----                Created 27/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMCOSY2018MONITORSETUPGOOD_H
#define CBMCOSY2018MONITORSETUPGOOD_H

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

class CbmCern2017UnpackParHodo;
class CbmCern2017UnpackParSts;

class CbmCosy2018MonitorSetupGood: public CbmTSUnpack
{
public:

   CbmCosy2018MonitorSetupGood();
   virtual ~CbmCosy2018MonitorSetupGood();

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
   void SetHistoFileName( TString sFileName = "data/SetupHistos.root" ) { fsHistoFileFullname = sFileName; }

   void SetMsOverlap(size_t uOverlapMsNb = 1) { fuOverlapMsNb = uOverlapMsNb; }
   size_t GetMsOverlap()                      { return fuOverlapMsNb; }

   void SetPrintMessage( Bool_t bPrintMessOn = kTRUE,
                         stsxyter::MessagePrintMask ctrl = stsxyter::MessagePrintMask::msg_print_Hex |
                                                           stsxyter::MessagePrintMask::msg_print_Human )
                        { fbPrintMessages = bPrintMessOn; fPrintMessCtrl = ctrl; }
   void EnableDualStsMode( Bool_t bEnable = kTRUE ) { fbDualStsEna = bEnable; }
   void SetLongDurationLimits( UInt_t uDurationSeconds = 3600, UInt_t uBinSize = 1 );
   void SetCoincidenceBorderHodo( Double_t dNewValue ){ fdCoincBorderHodo = dNewValue;}
   void SetCoincidenceBorderSts( Double_t dNewValue ){  fdCoincBorderSts  = dNewValue;}
   void SetCoincidenceBorder( Double_t dNewValue ){     fdCoincBorder     = dNewValue;}

   void SetStripsOffset1( Int_t iOffN, Int_t iOffP ) { fiStripsOffsetN1 = iOffN; fiStripsOffsetP1 = iOffP; }
   void SetStripsOffset2( Int_t iOffN, Int_t iOffP ) { fiStripsOffsetN2 = iOffN; fiStripsOffsetP2 = iOffP; }

private:
   size_t fuOverlapMsNb;      /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/

   // Parameters
   CbmCern2017UnpackParHodo* fUnpackParHodo; //!
   CbmCern2017UnpackParSts*  fUnpackParSts; //!
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
      // Histo File name and path
   TString fsHistoFileFullname;
      // Task configuration values
   Bool_t                fbPrintMessages;
   stsxyter::MessagePrintMask fPrintMessCtrl;
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

      // Data format control
   std::vector< ULong64_t > fvulCurrentTsMsb;                   //! Current TS MSB for each DPB
   std::vector< UInt_t    > fvuCurrentTsMsbCycle;               //! Current TS MSB cycle for DPB
   std::vector< UInt_t    > fvuElinkLastTsHit;                  //! TS from last hit for DPB
      // Hits comparison
   std::vector< std::vector< ULong64_t > > fvulChanLastHitTime;   //! Last hit time in bins for each Channel
   std::vector< std::vector< Double_t  > > fvdChanLastHitTime;    //! Last hit time in ns   for each Channel
   std::vector< Double_t >                               fvdMsTime;                  //! Header time of each MS
   std::vector< std::vector< std::vector< UInt_t > > >   fvuChanNbHitsInMs;          //! Number of hits in each MS for each Channel
   std::vector< std::vector< std::vector< Double_t > > > fvdChanLastHitTimeInMs;     //! Last hit time in bins in each MS for each Channel
   std::vector< std::vector< std::vector< UShort_t > > > fvusChanLastHitAdcInMs;     //! Last hit ADC in bins in each MS for each Channel
//   std::vector< std::vector< std::multiset< stsxyter::FinalHit > > > fvmChanHitsInTs; //! All hits (time & ADC) in bins in last TS for each Channel
      // Starting state book-keeping
   Double_t              fdStartTime;           /** Time of first valid hit (TS_MSB available), used as reference for evolution plots**/
   Double_t              fdStartTimeMsSz;       /** Time of first microslice, used as reference for evolution plots**/
   std::chrono::steady_clock::time_point ftStartTimeUnix; /** Time of run Start from UNIX system, used as reference for long evolution plots against reception time **/

      // Hits time-sorting
   std::vector< stsxyter::FinalHit > fvmHitsInTs; //! All hits (time in bins, ADC in bins, asic, channel) in last TS, sorted by multiset with "<" operator
   stsxyter::FinalHit fLastSortedHit1X; //! Last sorted hit for Hodo 1 X
   stsxyter::FinalHit fLastSortedHit1Y; //! Last sorted hit for Hodo 1 Y
   stsxyter::FinalHit fLastSortedHit2X; //! Last sorted hit for Hodo 2 X
   stsxyter::FinalHit fLastSortedHit2Y; //! Last sorted hit for Hodo 2 Y
   stsxyter::FinalHit fLastSortedHit1N; //! Last sorted hit for STS 1 N
   stsxyter::FinalHit fLastSortedHit1P; //! Last sorted hit for STS 1 P
   stsxyter::FinalHit fLastSortedHit2N; //! Last sorted hit for STS 2 N
   stsxyter::FinalHit fLastSortedHit2P; //! Last sorted hit for STS 2 P
      // Coincidence histos
   UInt_t fuMaxNbMicroslices;
      // Rate evolution histos
   Bool_t fbLongHistoEnable;
   UInt_t fuLongHistoNbSeconds;
   UInt_t fuLongHistoBinSizeSec;
   UInt_t fuLongHistoBinNb;

   // Histograms
   CbmHistManager* fHM;                 //! Histogram manager
   TH1* fhHodoMessType;
   TH1* fhHodoSysMessType;
   TH2* fhHodoMessTypePerDpb;
   TH2* fhHodoSysMessTypePerDpb;
/*
   TH2* fhStsDpbRawTsMsb;
   TH2* fhStsDpbRawTsMsbSx;
   TH2* fhStsDpbRawTsMsbDpb;
*/
   TH2* fhHodoMessTypePerElink;
   TH2* fhHodoSysMessTypePerElink;

   std::vector<TH1 *> fhHodoChanCntRaw;
   std::vector<TH2 *> fhHodoChanAdcRaw;
   std::vector<TProfile*> fhHodoChanAdcRawProf;
   std::vector<TH2*> fhHodoChanRawTs;
   std::vector<TH2*> fhHodoChanMissEvt;
   std::vector<TH2*> fhHodoChanMissEvtEvo;
   std::vector<TH2*> fhHodoChanHitRateEvo;
   std::vector<TH1*> fhHodoFebRateEvo;
   std::vector<TH1*> fhHodoFebMissEvtEvo;
   std::vector<TH2*> fhHodoChanHitRateEvoLong;
   std::vector<TH1*> fhHodoFebRateEvoLong;

      // Coincidences in sorted hits
   Double_t fdCoincBorderHodo; // ns, +/-
   Double_t fdCoincBorderSts; // ns, +/-
   Double_t fdCoincBorder; // ns, +/-
         // Single detector maps
   TH1 * fhSetupSortedDtX1Y1;
   TH1 * fhSetupSortedDtX2Y2;
   TH1 * fhSetupSortedDtN1P1;
   TH1 * fhSetupSortedDtN2P2;
   TH1 * fhSetupSortedDtX1Y1X2Y2;
   TH1 * fhSetupSortedDtN1P1N2P2;
   TH1 * fhSetupSortedDtX1Y1X2Y2N1P1;
   TH1 * fhSetupSortedDtX1Y1X2Y2N2P2;
   TH1 * fhSetupSortedDtH1H2S1S2;
   TH2 * fhSetupSortedMapX1Y1;
   TH2 * fhSetupSortedMapX2Y2;
   TH2 * fhSetupSortedMapN1P1;
   TH2 * fhSetupSortedMapN2P2;
   TH1 * fhSetupSortedCntEvoX1Y1;
   TH1 * fhSetupSortedCntEvoX2Y2;
   TH1 * fhSetupSortedCntEvoN1P1;
   TH1 * fhSetupSortedCntEvoN2P2;

         // Full Hodo maps
   TH1 * fhBothHodoSortedDtX1Y1;
   TH1 * fhBothHodoSortedDtX2Y2;
   TH1 * fhBothHodoSortedDtX1Y1X2Y2N1P1;
   TH1 * fhBothHodoSortedDtX1Y1X2Y2N2P2;
   TH1 * fhBothHodoSortedDtH1H2S1S2;
   TH2 * fhBothHodoSortedMapX1Y1;
   TH2 * fhBothHodoSortedMapX2Y2;
   TH1 * fhBothHodoSortedCntEvoX1Y1;
   TH1 * fhBothHodoSortedCntEvoX2Y2;

         // H + Sn maps
   TH1 * fhH1H2S1SortedDtX1Y1;
   TH1 * fhH1H2S1SortedDtX2Y2;
   TH1 * fhH1H2S1SortedDtN1P1;
   TH1 * fhH1H2S1SortedDtN2P2;
   TH2 * fhH1H2S1SortedMapX1Y1;
   TH2 * fhH1H2S1SortedMapX2Y2;
   TH2 * fhH1H2S1SortedMapN1P1;
   TH2 * fhH1H2S1SortedMapN2P2;
   TH1 * fhH1H2S1SortedCntEvoX1Y1;
   TH1 * fhH1H2S1SortedCntEvoX2Y2;
   TH1 * fhH1H2S1SortedCntEvoN1P1;
   TH1 * fhH1H2S1SortedCntEvoN2P2;

   TH1 * fhH1H2S2SortedDtX1Y1;
   TH1 * fhH1H2S2SortedDtX2Y2;
   TH1 * fhH1H2S2SortedDtN1P1;
   TH1 * fhH1H2S2SortedDtN2P2;
   TH2 * fhH1H2S2SortedMapX1Y1;
   TH2 * fhH1H2S2SortedMapX2Y2;
   TH2 * fhH1H2S2SortedMapN1P1;
   TH2 * fhH1H2S2SortedMapN2P2;
   TH1 * fhH1H2S2SortedCntEvoX1Y1;
   TH1 * fhH1H2S2SortedCntEvoX2Y2;
   TH1 * fhH1H2S2SortedCntEvoN1P1;
   TH1 * fhH1H2S2SortedCntEvoN2P2;

         // Full System maps
   TH1 * fhSystSortedDtX1Y1;
   TH1 * fhSystSortedDtX2Y2;
   TH1 * fhSystSortedDtN1P1;
   TH1 * fhSystSortedDtN2P2;
   TH1 * fhSystSortedDtX1Y1X2Y2;
   TH2 * fhSystSortedMapX1Y1;
   TH2 * fhSystSortedMapX2Y2;
   TH2 * fhSystSortedMapN1P1;
   TH2 * fhSystSortedMapN2P2;
   TH1 * fhSystSortedCntEvoX1Y1;
   TH1 * fhSystSortedCntEvoX2Y2;
   TH1 * fhSystSortedCntEvoN1P1;
   TH1 * fhSystSortedCntEvoN2P2;

   TH2 * fhSystSortedDtN1X1vsN1X2;
   TH2 * fhSystSortedDtP1X1vsP1X2;
   TH2 * fhSystSortedDtN1X1vsP1X1;
   TH2 * fhSystSortedDtSts1Hodo1vsSts1Hodo2;
   TH2 * fhSystSortedDtSts2Hodo1vsSts2Hodo2;
   TH2 * fhSystSortedDtAllVsMapX1;
   TH2 * fhSystSortedDtAllVsMapY1;
   TH2 * fhSystSortedDtAllVsMapX2;
   TH2 * fhSystSortedDtAllVsMapY2;

   TH1 * fhSetupSortedDtX1;
   TH1 * fhSetupSortedDtY1;
   TH1 * fhSetupSortedDtX2;
   TH1 * fhSetupSortedDtY2;
   TH1 * fhSetupSortedDtN1;
   TH1 * fhSetupSortedDtP1;
   TH1 * fhSetupSortedDtN2;
   TH1 * fhSetupSortedDtP2;

   /// Counting hits without proper timestamp
   std::vector< std::vector< ULong64_t > > fvulTimeLastHitAsicChan;
   std::vector< std::vector< Int_t > >     fviAdcLastHitAsicChan;
   std::vector< std::vector< UInt_t > >    fvuNbSameHitAsicChan;
   std::vector< std::vector< UInt_t > >    fvuNbSameFullHitAsicChan;

   std::vector< ULong64_t > fvulStartTimeLastS;
   std::vector< UInt_t >    fvuNbHitDiffTsAsicLastS;
   std::vector< UInt_t >    fvuNbHitSameTsAsicLastS;
   std::vector< UInt_t >    fvuNbHitSameTsAdcAsicLastS;

   std::vector< Bool_t > fvbAsicHasDuplicInMs;

   std::vector< TH2 * >        fhSetupSortedNbSameTsChan;
   std::vector< TH2 * >        fhSetupSortedSameTsAdcChan;
   std::vector< TH2 * >        fhSetupSortedSameTsAdcDiff;
   std::vector< TProfile2D * > fhSetupSortedRatioSameTsChan;
   std::vector< TH2 * >        fhSetupSortedNbConsSameTsChan;
   std::vector< TH2 * >        fhSetupSortedNbConsSameTsAdcChan;
   std::vector< TH2 * >        fhSetupSortedNbConsSameTsVsTsAdc;

   std::vector< TH2 * >        fhSetupSortedAsicRatioSameTsVsFlux;
   std::vector< TH2 * >        fhSetupSortedAsicRatioSameTsAdcVsFlux;
   std::vector< TH2 * >        fhSetupSortedAsicRatioSameAdcSameTsVsFlux;

   TH1 * fhSetupSortedCleanMsDtH1H2S1S2;
   TProfile * fhRatioMsDuplicateQuality;
   std::vector< TProfile * > fhEvoMsDuplicateQuality;
   TH1 * fhSizeCleanMs;

   TH2 * fhHodoX1SpillEvo;
   TH2 * fhHodoY1SpillEvo;
   TH2 * fhHodoX2SpillEvo;
   TH2 * fhHodoY2SpillEvo;

   Double_t   fdSpillEvoLength;
   TProfile * fhHodoX1SpillEvoProf;
   TProfile * fhHodoY1SpillEvoProf;
   TProfile * fhHodoX2SpillEvoProf;
   TProfile * fhHodoY2SpillEvoProf;

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
   TH2 * fhStsSortedMapS1;
   TH2 * fhStsSortedMapS2;
   TH2 * fhStsSortedMapS1Coinc;
   TH2 * fhStsSortedMapS2Coinc;

   TCanvas*  fcMsSizeAll;
   TH1*      fhMsSz[kiMaxNbFlibLinks];
   TProfile* fhMsSzTime[kiMaxNbFlibLinks];

   void CreateHistograms();

   void FillHitInfo(   stsxyter::Message mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx, const UInt_t & uMsIdx );
   void FillTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
   void FillEpochInfo( stsxyter::Message mess );

   void ComputeCoordinatesSensor1( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );
   void ComputeCoordinatesSensor2( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );

   CbmCosy2018MonitorSetupGood(const CbmCosy2018MonitorSetupGood&);
   CbmCosy2018MonitorSetupGood operator=(const CbmCosy2018MonitorSetupGood&);

   ClassDef(CbmCosy2018MonitorSetupGood, 1)
};

#endif // CBMCOSY2018MONITORSETUPGOOD_H
