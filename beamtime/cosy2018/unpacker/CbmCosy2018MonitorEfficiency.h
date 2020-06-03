// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                   CbmCosy2018MonitorEfficiency                    -----
// -----                Created 18/04/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMCOSY2018MONITOREFFICIENCY_H
#define CBMCOSY2018MONITOREFFICIENCY_H

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

///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
class Cosy2018TestCluster
{
public:
   Cosy2018TestCluster( stsxyter::FinalHit hitFirst );

   Bool_t CheckAddHit( stsxyter::FinalHit hitCandidate );

   std::vector< stsxyter::FinalHit > fvHits;
   Double_t fdMeanTime;
   UShort_t fusTotalAdc;
   UShort_t fusFirstChannel;
   UShort_t fusLastChannel;
   Double_t fdWeightedCenter;

};
///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

class CbmCosy2018MonitorEfficiency: public CbmTSUnpack
{
public:

   CbmCosy2018MonitorEfficiency();
   virtual ~CbmCosy2018MonitorEfficiency();

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
   void SetCoincidenceBorderHodo( Double_t dCenterPos, Double_t dBorderVal );
   void SetCoincidenceBorderHodoBoth( Double_t dCenterPos, Double_t dBorderVal );
   void SetCoincidenceBorderSts1( Double_t dCenterPos, Double_t dBorderVal );
   void SetCoincidenceBorderSts2( Double_t dCenterPos, Double_t dBorderVal );
   void SetCoincidenceBorderHodoSts1( Double_t dCenterPos, Double_t dBorderVal );
   void SetCoincidenceBorderHodoSts2( Double_t dCenterPos, Double_t dBorderVal );
   void SetCoincidenceBorder( Double_t dCenterPos, Double_t dBorderVal );

   void SetStripsOffset1( Int_t iOffN, Int_t iOffP ) { fiStripsOffsetN1 = iOffN; fiStripsOffsetP1 = iOffP; }
   void SetStripsOffset2( Int_t iOffN, Int_t iOffP ) { fiStripsOffsetN2 = iOffN; fiStripsOffsetP2 = iOffP; }
   void SetPositionOffsetSts1( Double_t dOffsetX, Double_t dOffsetY ) { fdOffsetS1X = dOffsetX; fdOffsetS1Y = dOffsetY; }
   void SetPositionOffsetSts2( Double_t dOffsetX, Double_t dOffsetY ) { fdOffsetS2X = dOffsetX; fdOffsetS2Y = dOffsetY; }

   void SetPositionsMmZ( Double_t dPosH1, Double_t dPosH2, Double_t dPosS1, Double_t dPosS2 );

   void EnableDeadCorr( Bool_t bEnable = kTRUE ) { fbDeadCorrEna = bEnable; }

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
   Bool_t                fbDeadCorrEna;
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
   std::vector<TH1 *> fhHodoChanCntRawGood;
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
   Double_t fdCoincCenterHodo;  // ns
   Double_t fdCoincBorderHodo;  // ns, +/-
   Double_t fdCoincMinHodo;     // ns
   Double_t fdCoincMaxHodo;     // ns
   Double_t fdCoincCenterHodoBoth; // ns
   Double_t fdCoincBorderHodoBoth; // ns, +/-
   Double_t fdCoincMinHodoBoth;    // ns
   Double_t fdCoincMaxHodoBoth;    // ns
   Double_t fdCoincCenterSts1;  // ns
   Double_t fdCoincBorderSts1;  // ns, +/-
   Double_t fdCoincMinSts1;     // ns
   Double_t fdCoincMaxSts1;     // ns
   Double_t fdCoincCenterSts2;  // ns
   Double_t fdCoincBorderSts2;  // ns, +/-
   Double_t fdCoincMinSts2;     // ns
   Double_t fdCoincMaxSts2;     // ns
   Double_t fdCoincCenterHodoS1; // ns
   Double_t fdCoincBorderHodoS1; // ns, +/-
   Double_t fdCoincMinHodoS1;    // ns
   Double_t fdCoincMaxHodoS1;    // ns
   Double_t fdCoincCenterHodoS2; // ns
   Double_t fdCoincBorderHodoS2; // ns, +/-
   Double_t fdCoincMinHodoS2;    // ns
   Double_t fdCoincMaxHodoS2;    // ns
   Double_t fdCoincCenter;      // ns
   Double_t fdCoincBorder;      // ns, +/-
   Double_t fdCoincMin;         // ns
   Double_t fdCoincMax;         // ns
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
   Double_t       fdStripRangeNP;
   const Int_t    kiNbFibers    =   64;
   const Double_t kdHodoSzXY    =   64; // [mm]
   const Double_t kdHodoMidXY   = kdHodoSzXY / 2.0; // [mm]
   const Double_t kdPitchHodoMm = kdHodoSzXY/ kiNbFibers; // [mm]
   Int_t fiStripsOffsetN1; // of channel 0 relative to center strip
   Int_t fiStripsOffsetP1; // of channel 0 relative to center strip
   Int_t fiStripsOffsetN2; // of channel 0 relative to center strip
   Int_t fiStripsOffsetP2; // of channel 0 relative to center strip
   TH2 * fhStsSortedMapS1;
   TH2 * fhStsSortedMapS2;
   TH2 * fhStsSortedMapS1Coinc;
   TH2 * fhStsSortedMapS2Coinc;

   ///------ Coincidence building and effciency measurement ---------///
   Double_t fdPosZH1; // [mm]
   Double_t fdPosZH2; // [mm]
   Double_t fdPosZS1; // [mm]
   Double_t fdPosZS2; // [mm]
   Double_t fdRatioZS1;
   Double_t fdRatioZS2;
   Double_t fdOffsetS1X; // [mm]
   Double_t fdOffsetS1Y; // [mm]
   Double_t fdOffsetS2X; // [mm]
   Double_t fdOffsetS2Y; // [mm]

   std::vector< std::pair< stsxyter::FinalHit, stsxyter::FinalHit > > fvPairsH1; //!
   std::vector< std::pair< stsxyter::FinalHit, stsxyter::FinalHit > > fvPairsH2; //!
   std::vector< std::pair< stsxyter::FinalHit, stsxyter::FinalHit > > fvPairsS1; //!
   std::vector< std::pair< stsxyter::FinalHit, stsxyter::FinalHit > > fvPairsS2; //!
   std::vector< std::pair< UInt_t, UInt_t > > fvPairsHodo;
   std::vector< std::pair< UInt_t, UInt_t > > fvPairsHodoS1;
   std::vector< std::pair< UInt_t, UInt_t > > fvPairsHodoS2;
   std::vector< std::pair< UInt_t, UInt_t > > fvPairsHodoS1S2;
   std::vector< std::pair< UInt_t, UInt_t > > fvPairsHodoS2S1;

   TH1 * fhNbY1CoincPerX1;
   TH1 * fhNbY2CoincPerX2;
   TH1 * fhNbP1CoincPerN1;
   TH1 * fhNbP2CoincPerN2;
   TH1 * fhNbH2CoincPerH1;
   TH1 * fhNbS1CoincPerHodo;
   TH1 * fhNbS2CoincPerHodo;
   TH1 * fhNbS1CoincPerHodoS2;
   TH1 * fhNbS2CoincPerHodoS1;
   TH1 * fhNbS1CoincPerHodoGeoCut;
   TH1 * fhNbS2CoincPerHodoGeoCut;
   TH1 * fhNbS1CoincPerHodoS2GeoCut;
   TH1 * fhNbS2CoincPerHodoS1GeoCut;

   TH2 * fhChanMapAllMatchS1;
   TH2 * fhChanMapAllMatchS2;
   TH2 * fhChanMapBestMatchS1;
   TH2 * fhChanMapBestMatchS2;

   TH1 * fhDtAllPairsS1;
   TH1 * fhDtAllPairsS2;
   TH1 * fhDtAllPairsHodoS1;
   TH1 * fhDtAllPairsHodoS2;
   TH1 * fhDtBestPairsH1;
   TH1 * fhDtBestPairsH2;
   TH1 * fhDtBestPairsS1;
   TH1 * fhDtBestPairsS2;
   TH1 * fhDtBestPairsHodo;
   TH1 * fhDtBestPairsHodoS1;
   TH1 * fhDtBestPairsHodoS2;
   TH1 * fhDtBestPairsHodoS1S2;
   TH1 * fhDtBestPairsHodoS2S1;
   TH1 * fhDtBestPairsHodoS1GeoCut;
   TH1 * fhDtBestPairsHodoS2GeoCut;
   TH1 * fhDtBestPairsHodoS1S2GeoCut;
   TH1 * fhDtBestPairsHodoS2S1GeoCut;

   TH2 * fhMapBestPairsH1;
   TH2 * fhMapBestPairsH2;
   TH2 * fhMapBestPairsS1;
   TH2 * fhMapBestPairsS2;
   TH2 * fhMapBestPairsHodo_H1;
   TH2 * fhMapBestPairsHodo_H2;
   TH2 * fhMapBestPairsHodo_ProjS1;
   TH2 * fhMapBestPairsHodo_ProjS2;
   TH2 * fhMapBestPairsHodoS1_H1;
   TH2 * fhMapBestPairsHodoS1_H2;
   TH2 * fhMapBestPairsHodoS1_S1;
   TH2 * fhMapBestPairsHodoS1_Proj;
   TH2 * fhMapBestPairsHodoS2_H1;
   TH2 * fhMapBestPairsHodoS2_H2;
   TH2 * fhMapBestPairsHodoS2_S2;
   TH2 * fhMapBestPairsHodoS2_Proj;
   TH2 * fhMapBestPairsHodoS1S2_H1;
   TH2 * fhMapBestPairsHodoS1S2_H2;
   TH2 * fhMapBestPairsHodoS1S2_S1;
   TH2 * fhMapBestPairsHodoS1S2_Proj;
   TH2 * fhMapBestPairsHodoS2S1_H1;
   TH2 * fhMapBestPairsHodoS2S1_H2;
   TH2 * fhMapBestPairsHodoS2S1_S2;
   TH2 * fhMapBestPairsHodoS2S1_Proj;

   TH2 * fhMapBestPairsHodoGeoCutS1_H1;
   TH2 * fhMapBestPairsHodoGeoCutS1_H2;
   TH2 * fhMapBestPairsHodoGeoCutS1_Proj;
   TH2 * fhMapBestPairsHodoGeoCutS2_H1;
   TH2 * fhMapBestPairsHodoGeoCutS2_H2;
   TH2 * fhMapBestPairsHodoGeoCutS2_Proj;
   TH2 * fhMapBestPairsHodoS1GeoCut_H1;
   TH2 * fhMapBestPairsHodoS1GeoCut_H2;
   TH2 * fhMapBestPairsHodoS1GeoCut_S1;
   TH2 * fhMapBestPairsHodoS1GeoCut_Proj;
   TH2 * fhMapBestPairsHodoS2GeoCut_H1;
   TH2 * fhMapBestPairsHodoS2GeoCut_H2;
   TH2 * fhMapBestPairsHodoS2GeoCut_S2;
   TH2 * fhMapBestPairsHodoS2GeoCut_Proj;
   TH2 * fhMapBestPairsHodoS1S2GeoCut_H1;
   TH2 * fhMapBestPairsHodoS1S2GeoCut_H2;
   TH2 * fhMapBestPairsHodoS1S2GeoCut_S1;
   TH2 * fhMapBestPairsHodoS1S2GeoCut_Proj;
   TH2 * fhMapBestPairsHodoS2S1GeoCut_H1;
   TH2 * fhMapBestPairsHodoS2S1GeoCut_H2;
   TH2 * fhMapBestPairsHodoS2S1GeoCut_S2;
   TH2 * fhMapBestPairsHodoS2S1GeoCut_Proj;

   TH2 * fhResidualsBestPairsHodoS1;
   TH2 * fhResidualsBestPairsHodoS2;
   TH2 * fhResidualsBestPairsHodoS1S2;
   TH2 * fhResidualsBestPairsHodoS2S1;
   TH2 * fhResidualsBestPairsHodoS1GeoCut;
   TH2 * fhResidualsBestPairsHodoS2GeoCut;
   TH2 * fhResidualsBestPairsHodoS1S2GeoCut;
   TH2 * fhResidualsBestPairsHodoS2S1GeoCut;

   TH2 * fhAdcRawBestPairsHodoS1N;
   TH2 * fhAdcRawBestPairsHodoS1P;
   TH2 * fhAdcRawBestPairsHodoS2N;
   TH2 * fhAdcRawBestPairsHodoS2P;
   TH2 * fhAdcRawBestPairsHodoS1S2N;
   TH2 * fhAdcRawBestPairsHodoS1S2P;
   TH2 * fhAdcRawBestPairsHodoS2S1N;
   TH2 * fhAdcRawBestPairsHodoS2S1P;

   TProfile   * fhEfficiency;
   TProfile2D * fhEfficiencyMapS1;
   TProfile2D * fhEfficiencyMapS2;
   TProfile2D * fhEfficiencyMapS1S2;
   TProfile2D * fhEfficiencyMapS2S1;
   TProfile2D * fhEfficiencyMapS1GeoCut;
   TProfile2D * fhEfficiencyMapS2GeoCut;
   TProfile2D * fhEfficiencyMapS1S2GeoCut;
   TProfile2D * fhEfficiencyMapS2S1GeoCut;

   ///---------------------------------------------------------------///
   TCanvas    * fcCanvasProjectionMaps;
   TCanvas    * fcCanvasEfficiencyMaps;
   ///---------------------------------------------------------------///


   TCanvas*  fcMsSizeAll;
   TH1*      fhMsSz[kiMaxNbFlibLinks];
   TProfile* fhMsSzTime[kiMaxNbFlibLinks];

   TH2 * fhTestBoundariesSensor1;
   TH2 * fhTestBoundariesSensor2;
   TH2 * fhTestBoundariesSensor1NoDead;
   TH2 * fhTestBoundariesSensor2NoDead;
   TH2 * fhTestChanMatchSensor1;
   TH2 * fhTestChanMatchSensor2;
   TH2 * fhTestChanMapSensor1;
   TH2 * fhTestChanMapSensor2;
   TH2 * fhTestChanMapPhysSensor1;
   TH2 * fhTestChanMapPhysSensor2;

   std::vector< stsxyter::FinalHit > fvHitsS1N; //!
   std::vector< stsxyter::FinalHit > fvHitsS1P; //!
   std::vector< stsxyter::FinalHit > fvHitsS2N; //!
   std::vector< stsxyter::FinalHit > fvHitsS2P; //!
   TProfile2D * fhTestMapHodoS1N;
   TProfile2D * fhTestMapHodoS1P;
   TProfile2D * fhTestMapHodoS2N;
   TProfile2D * fhTestMapHodoS2P;

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
   TH2 * fhDtNeighborChansS1N;
   TH2 * fhDtNeighborChansS1P;
   TH2 * fhDtNeighborChansS2N;
   TH2 * fhDtNeighborChansS2P;
   std::vector< stsxyter::FinalHit > fvLastHitChanS1N; //!
   std::vector< stsxyter::FinalHit > fvLastHitChanS1P; //!
   std::vector< stsxyter::FinalHit > fvLastHitChanS2N; //!
   std::vector< stsxyter::FinalHit > fvLastHitChanS2P; //!
   std::vector< Cosy2018TestCluster > fvClustersS1N; //!
   std::vector< Cosy2018TestCluster > fvClustersS1P; //!
   std::vector< Cosy2018TestCluster > fvClustersS2N; //!
   std::vector< Cosy2018TestCluster > fvClustersS2P; //!
   TH2 * fhClusterAdcVsSizeS1N;
   TH2 * fhClusterAdcVsSizeS1P;
   TH2 * fhClusterAdcVsSizeS2N;
   TH2 * fhClusterAdcVsSizeS2P;
   TH2 * fhClusterAdcVsSizeS1N_MatchS1;
   TH2 * fhClusterAdcVsSizeS1P_MatchS1;
   TH2 * fhClusterAdcVsSizeS2N_MatchS2;
   TH2 * fhClusterAdcVsSizeS2P_MatchS2;
   TH2 * fhClusterS1N_CenterNbMatchS1P;
   TH2 * fhClusterS1P_CenterNbMatchS1N;
   TH2 * fhClusterS2N_CenterNbMatchS2P;
   TH2 * fhClusterS2P_CenterNbMatchS2N;
   TH2 * fhClusterS1N_AdcNbMatchS1P;
   TH2 * fhClusterS1P_AdcNbMatchS1N;
   TH2 * fhClusterS2N_AdcNbMatchS2P;
   TH2 * fhClusterS2P_AdcNbMatchS2N;
   
   TH2 * fhDtNeighborChansH1X;
   TH2 * fhDtNeighborChansH1Y;
   TH2 * fhDtNeighborChansH2X;
   TH2 * fhDtNeighborChansH2Y;
   std::vector< stsxyter::FinalHit > fvLastHitChanH1X; //!
   std::vector< stsxyter::FinalHit > fvLastHitChanH1Y; //!
   std::vector< stsxyter::FinalHit > fvLastHitChanH2X; //!
   std::vector< stsxyter::FinalHit > fvLastHitChanH2Y; //!
   std::vector< Cosy2018TestCluster > fvClustersH1X; //!
   std::vector< Cosy2018TestCluster > fvClustersH1Y; //!
   std::vector< Cosy2018TestCluster > fvClustersH2X; //!
   std::vector< Cosy2018TestCluster > fvClustersH2Y; //!
   TH2 * fhClusterAdcVsSizeH1X;
   TH2 * fhClusterAdcVsSizeH1Y;
   TH2 * fhClusterAdcVsSizeH2X;
   TH2 * fhClusterAdcVsSizeH2Y;
   TH2 * fhClusterPosition_MatchH1;
   TH2 * fhClusterPosition_MatchH2;
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///


   void CreateHistograms();

   void FillHitInfo(   stsxyter::Message mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx, const UInt_t & uMsIdx );
   void FillTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx = 0, UInt_t uMsIdx = 0);
   void FillEpochInfo( stsxyter::Message mess );

   void ComputeCoordinatesHodo1( Int_t iChanX, Int_t iChanY, Double_t & dPosX, Double_t & dPosY );
   void ComputeCoordinatesHodo2( Int_t iChanX, Int_t iChanY, Double_t & dPosX, Double_t & dPosY );
   void ComputeCoordinatesSensor1( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );
   void ComputeCoordinatesSensor2( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY );
   void ComputeProjectionSensor1( Double_t dPosXH1, Double_t dPosYH1,
                                  Double_t dPosXH2, Double_t dPosYH2,
                                  Double_t & dPosX, Double_t & dPosY );
   void ComputeProjectionSensor2( Double_t dPosXH1, Double_t dPosYH1,
                                  Double_t dPosXH2, Double_t dPosYH2,
                                  Double_t & dPosX, Double_t & dPosY );

   Bool_t CheckPhysPairSensor1( Int_t iChanN, Int_t iChanP ); // Position in P/N strips
   Bool_t CheckPhysPairSensor2( Int_t iChanN, Int_t iChanP ); // Position in P/N strips
   Bool_t CheckPointInsideSensor1( Double_t dPosX, Double_t dPosY ); // Position in mm inside sensor XY frame
   Bool_t CheckPointInsideSensor2( Double_t dPosX, Double_t dPosY ); // Position in mm inside sensor XY frame

   static const UInt_t kuNbDeadStripsS1N = 4;
   const UInt_t kuDeadStripsS1N[ kuNbDeadStripsS1N ] = { 124, 125, 126, 127 };
   static const UInt_t kuNbDeadStripsS1P = 5;
   const UInt_t kuDeadStripsS1P[ kuNbDeadStripsS1P ] = { 1, 123, 124, 126, 127 };
   static const UInt_t kuNbDeadStripsS2N = 9 + 2;
   const UInt_t kuDeadStripsS2N[ kuNbDeadStripsS2N ] = { 0, 1, 2, 3, 4, 6, 8, 10, 12, 126, 127 };
   static const UInt_t kuNbDeadStripsS2P = 8 + 3 + 4;
   const UInt_t kuDeadStripsS2P[ kuNbDeadStripsS2P ] = { 0, 1, 2, 3, 4, 5, 6, 8, 36, 40, 64, 124, 125, 126, 127 };
   Bool_t CheckPointInsideSensor1NoDead( Double_t dPosX, Double_t dPosY ); // Position in mm inside sensor XY frame
   Bool_t CheckPointInsideSensor2NoDead( Double_t dPosX, Double_t dPosY ); // Position in mm inside sensor XY frame

   CbmCosy2018MonitorEfficiency(const CbmCosy2018MonitorEfficiency&);
   CbmCosy2018MonitorEfficiency operator=(const CbmCosy2018MonitorEfficiency&);

   ClassDef(CbmCosy2018MonitorEfficiency, 1)
};

#endif // CBMCOSY2018MONITOREFFICIENCY_H
