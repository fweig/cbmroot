// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorEfficiency                        -----
// -----                Created 27/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCosy2018MonitorEfficiency.h"

// Data

// CbmRoot
#include "CbmCern2017UnpackParHodo.h"
#include "CbmCern2017UnpackParSts.h"
#include "CbmHistManager.h"

// FairRoot
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"

// Root
#include "TClonesArray.h"
#include "TString.h"
#include "TRandom.h"
#include "THttpServer.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include <TFile.h>

// C++11

// C/C++
#include <iostream>
#include <stdint.h>
#include <iomanip>

Cosy2018TestCluster::Cosy2018TestCluster( stsxyter::FinalHit hitFirst ) :
   fvHits( 1, hitFirst ),
   fdMeanTime( hitFirst.GetTs() * stsxyter::kdClockCycleNs ),
   fusTotalAdc( hitFirst.GetAdc() ),
   fusFirstChannel( hitFirst.GetChan() ),
   fusLastChannel( hitFirst.GetChan() ),
   fdWeightedCenter( hitFirst.GetChan() )
{
}
Bool_t Cosy2018TestCluster::CheckAddHit( stsxyter::FinalHit hitCandidate )
{
   /// First check if within cut at +/- 40 ns
   Double_t dDt = hitCandidate.GetTs() * stsxyter::kdClockCycleNs - fdMeanTime;
   if( -40.0 < dDt && dDt < 40.0 )
   {
      UShort_t usChan = hitCandidate.GetChan();

      /// Then check if we are inside cluster or at +/- 1 channel
      if( fusFirstChannel <= usChan + 1 &&
          usChan <= fusLastChannel + 1 )
      {
         fdMeanTime   = ( fdMeanTime * fvHits.size() + hitCandidate.GetTs() * stsxyter::kdClockCycleNs ) / ( fvHits.size() + 1 );

         fdWeightedCenter = fdWeightedCenter * fusTotalAdc + usChan * hitCandidate.GetAdc();

         fusTotalAdc += hitCandidate.GetAdc();

         fdWeightedCenter /= fusTotalAdc;

         if( usChan < fusFirstChannel )
            fusFirstChannel = usChan;
         else if( fusLastChannel < usChan )
            fusLastChannel = usChan;


         fvHits.push_back( hitCandidate );

         return kTRUE;
      } // if ch within [ First -1; Last + 1]
   } // if( -40.0 < dDt && dDt < 40.0 )

   return kFALSE;
}


Bool_t bCosy2018ResetEfficiency = kFALSE;
Bool_t bCosy2018WriteEfficiency = kFALSE;

CbmCosy2018MonitorEfficiency::CbmCosy2018MonitorEfficiency() :
   CbmTSUnpack(),
   fuOverlapMsNb(0),
   fUnpackParHodo(NULL),
   fUnpackParSts(NULL),
   fuNrOfDpbs(0),
   fDpbIdIndexMap(),
   fuNbElinksPerDpb(0),
   fuNbStsXyters(0),
   fuNbChanPerAsic(0),
   fvuElinkToAsic(),
   fsHistoFileFullname( "data/SetupHistos.root" ),
   fbPrintMessages( kFALSE ),
   fPrintMessCtrl( stsxyter::MessagePrintMask::msg_print_Human ),
   fbDualStsEna( kFALSE ),
   fbDeadCorrEna( kFALSE ),
   fulCurrentTsIdx( 0 ),
   fulCurrentMsIdx( 0 ),
   fmMsgCounter(),
   fuCurrentEquipmentId(0),
   fuCurrDpbId(0),
   fuCurrDpbIdx(0),
   fiRunStartDateTimeSec(-1),
   fiBinSizeDatePlots(-1),
   fvulCurrentTsMsb(),
   fvuCurrentTsMsbCycle(),
   fvuElinkLastTsHit(),
   fvulChanLastHitTime(),
   fvdChanLastHitTime(),
   fvdMsTime(),
   fvuChanNbHitsInMs(),
   fvdChanLastHitTimeInMs(),
   fvusChanLastHitAdcInMs(),
//   fvmChanHitsInTs(),
   fdStartTime(-1.0),
   fdStartTimeMsSz(-1.0),
   ftStartTimeUnix( std::chrono::steady_clock::now() ),
   fvmHitsInTs(),
   fLastSortedHit1X(),
   fLastSortedHit1Y(),
   fLastSortedHit2X(),
   fLastSortedHit2Y(),
   fLastSortedHit1N(),
   fLastSortedHit1P(),
   fLastSortedHit2N(),
   fLastSortedHit2P(),
   fuMaxNbMicroslices(100),
   fbLongHistoEnable( kFALSE ),
   fuLongHistoNbSeconds( 0 ),
   fuLongHistoBinSizeSec( 0 ),
   fuLongHistoBinNb( 0 ),
   fHM(new CbmHistManager()),
   fhHodoMessType(NULL),
   fhHodoSysMessType(NULL),
   fhHodoMessTypePerDpb(NULL),
   fhHodoSysMessTypePerDpb(NULL),
   fhHodoMessTypePerElink(NULL),
   fhHodoSysMessTypePerElink(NULL),
   fhHodoChanCntRaw(),
   fhHodoChanCntRawGood(),
   fhHodoChanAdcRaw(),
   fhHodoChanAdcRawProf(),
   fhHodoChanRawTs(),
   fhHodoChanMissEvt(),
   fhHodoChanMissEvtEvo(),
   fhHodoChanHitRateEvo(),
   fhHodoFebRateEvo(),
   fhHodoFebMissEvtEvo(),
   fhHodoChanHitRateEvoLong(),
   fhHodoFebRateEvoLong(),
   fdCoincCenterHodo(   0.0 ),
   fdCoincBorderHodo(  50.0 ),
   fdCoincMinHodo( fdCoincCenterHodo - fdCoincBorderHodo ),
   fdCoincMaxHodo( fdCoincCenterHodo + fdCoincBorderHodo ),
   fdCoincCenterHodoBoth(   0.0 ),
   fdCoincBorderHodoBoth( 100.0 ),
   fdCoincMinHodoBoth( fdCoincCenterHodoBoth - fdCoincBorderHodoBoth ),
   fdCoincMaxHodoBoth( fdCoincCenterHodoBoth + fdCoincBorderHodoBoth ),
   fdCoincCenterSts1(   0.0 ),
   fdCoincBorderSts1(  75.0 ),
   fdCoincMinSts1( fdCoincCenterSts1 - fdCoincBorderSts1 ),
   fdCoincMaxSts1( fdCoincCenterSts1 + fdCoincBorderSts1 ),
   fdCoincCenterSts2(   0.0 ),
   fdCoincBorderSts2(  75.0 ),
   fdCoincMinSts2( fdCoincCenterSts2 - fdCoincBorderSts2 ),
   fdCoincMaxSts2( fdCoincCenterSts2 + fdCoincBorderSts2 ),
   fdCoincCenterHodoS1(   0.0 ),
   fdCoincBorderHodoS1(  50.0 ),
   fdCoincMinHodoS1( fdCoincCenterHodoS1 - fdCoincBorderHodoS1 ),
   fdCoincMaxHodoS1( fdCoincCenterHodoS1 + fdCoincBorderHodoS1 ),
   fdCoincCenterHodoS2(   0.0 ),
   fdCoincBorderHodoS2(  50.0 ),
   fdCoincMinHodoS2( fdCoincCenterHodoS2 - fdCoincBorderHodoS1 ),
   fdCoincMaxHodoS2( fdCoincCenterHodoS2 + fdCoincBorderHodoS1 ),
   fdCoincCenter(       0.0 ),
   fdCoincBorder(     150.0 ),
   fdCoincMin( fdCoincCenter - fdCoincBorder ),
   fdCoincMax( fdCoincCenter + fdCoincBorder ),
   fhSetupSortedDtX1Y1(NULL),
   fhSetupSortedDtX2Y2(NULL),
   fhSetupSortedDtN1P1(NULL),
   fhSetupSortedDtN2P2(NULL),
   fhSetupSortedDtX1Y1X2Y2(NULL),
   fhSetupSortedDtN1P1N2P2(NULL),
   fhSetupSortedDtX1Y1X2Y2N1P1(NULL),
   fhSetupSortedDtX1Y1X2Y2N2P2(NULL),
   fhSetupSortedDtH1H2S1S2(NULL),
   fhSetupSortedMapX1Y1(NULL),
   fhSetupSortedMapX2Y2(NULL),
   fhSetupSortedMapN1P1(NULL),
   fhSetupSortedMapN2P2(NULL),
   fhSetupSortedCntEvoX1Y1(NULL),
   fhSetupSortedCntEvoX2Y2(NULL),
   fhSetupSortedCntEvoN1P1(NULL),
   fhSetupSortedCntEvoN2P2(NULL),
   fhSetupSortedDtX1(NULL),
   fhSetupSortedDtY1(NULL),
   fhSetupSortedDtX2(NULL),
   fhSetupSortedDtY2(NULL),
   fhSetupSortedDtN1(NULL),
   fhSetupSortedDtP1(NULL),
   fhSetupSortedDtN2(NULL),
   fhSetupSortedDtP2(NULL),
   fvulTimeLastHitAsicChan(),
   fviAdcLastHitAsicChan(),
   fvuNbSameHitAsicChan(),
   fvuNbSameFullHitAsicChan(),
   fvulStartTimeLastS(),
   fvuNbHitDiffTsAsicLastS(),
   fvuNbHitSameTsAsicLastS(),
   fvuNbHitSameTsAdcAsicLastS(),
   fvbAsicHasDuplicInMs(),
   fhSetupSortedNbSameTsChan(),
   fhSetupSortedSameTsAdcChan(),
   fhSetupSortedSameTsAdcDiff(),
   fhSetupSortedRatioSameTsChan(),
   fhSetupSortedNbConsSameTsChan(),
   fhSetupSortedNbConsSameTsAdcChan(),
   fhSetupSortedNbConsSameTsVsTsAdc(),
   fhSetupSortedAsicRatioSameTsVsFlux(),
   fhSetupSortedAsicRatioSameTsAdcVsFlux(),
   fhSetupSortedAsicRatioSameAdcSameTsVsFlux(),
   fhSetupSortedCleanMsDtH1H2S1S2(NULL),
   fhRatioMsDuplicateQuality(NULL),
   fhEvoMsDuplicateQuality(),
   fhSizeCleanMs(),
   fhHodoX1SpillEvo(NULL),
   fhHodoY1SpillEvo(NULL),
   fhHodoX2SpillEvo(NULL),
   fhHodoY2SpillEvo(NULL),
   fdSpillEvoLength( 700.0 ),
   fhHodoX1SpillEvoProf(NULL),
   fhHodoY1SpillEvoProf(NULL),
   fhHodoX2SpillEvoProf(NULL),
   fhHodoY2SpillEvoProf(NULL),
   fdStereoAngleTan( TMath::Tan( kdStereoAngle * TMath::DegToRad() ) ),
   fdStripRangeNP( TMath::Ceil( kdSensorsSzY * fdStereoAngleTan / kdPitchMm ) ),
   fiStripsOffsetN1(  69 ),
   fiStripsOffsetP1( -69 ),
   fiStripsOffsetN2(  69 ),
   fiStripsOffsetP2( -69 ),
   fhStsSortedMapS1(NULL),
   fhStsSortedMapS2(NULL),
   fhStsSortedMapS1Coinc(NULL),
   fhStsSortedMapS2Coinc(NULL),
   fdPosZH1( 0.0 ),
   fdPosZH2( 1.0 ),
   fdPosZS1( 0.5 ),
   fdPosZS2( 0.5 ),
   fdRatioZS1( fdPosZS1 / ( fdPosZH2 / fdPosZH1 ) ),
   fdRatioZS2( fdPosZS2 / ( fdPosZH2 / fdPosZH1 ) ),
   fdOffsetS1X( 0.0 ),
   fdOffsetS1Y( 0.0 ),
   fdOffsetS2X( 0.0 ),
   fdOffsetS2Y( 0.0 ),
   fvPairsH1(),
   fvPairsH2(),
   fvPairsS1(),
   fvPairsS2(),
   fvPairsHodo(),
   fvPairsHodoS1(),
   fvPairsHodoS2(),
   fvPairsHodoS1S2(),
   fvPairsHodoS2S1(),
   fhNbY1CoincPerX1(NULL),
   fhNbY2CoincPerX2(NULL),
   fhNbP1CoincPerN1(NULL),
   fhNbP2CoincPerN2(NULL),
   fhNbH2CoincPerH1(NULL),
   fhNbS1CoincPerHodo(NULL),
   fhNbS2CoincPerHodo(NULL),
   fhNbS1CoincPerHodoS2(NULL),
   fhNbS2CoincPerHodoS1(NULL),
   fhNbS1CoincPerHodoGeoCut(NULL),
   fhNbS2CoincPerHodoGeoCut(NULL),
   fhNbS1CoincPerHodoS2GeoCut(NULL),
   fhNbS2CoincPerHodoS1GeoCut(NULL),
   fhChanMapAllMatchS1(NULL),
   fhChanMapAllMatchS2(NULL),
   fhChanMapBestMatchS1(NULL),
   fhChanMapBestMatchS2(NULL),
   fhDtAllPairsS1(NULL),
   fhDtAllPairsS2(NULL),
   fhDtAllPairsHodoS1(NULL),
   fhDtAllPairsHodoS2(NULL),
   fhDtBestPairsH1(NULL),
   fhDtBestPairsH2(NULL),
   fhDtBestPairsS1(NULL),
   fhDtBestPairsS2(NULL),
   fhDtBestPairsHodo(NULL),
   fhDtBestPairsHodoS1(NULL),
   fhDtBestPairsHodoS2(NULL),
   fhDtBestPairsHodoS1S2(NULL),
   fhDtBestPairsHodoS2S1(NULL),
   fhDtBestPairsHodoS1GeoCut(NULL),
   fhDtBestPairsHodoS2GeoCut(NULL),
   fhDtBestPairsHodoS1S2GeoCut(NULL),
   fhDtBestPairsHodoS2S1GeoCut(NULL),
   fhMapBestPairsH1(NULL),
   fhMapBestPairsH2(NULL),
   fhMapBestPairsS1(NULL),
   fhMapBestPairsS2(NULL),
   fhMapBestPairsHodo_H1(NULL),
   fhMapBestPairsHodo_H2(NULL),
   fhMapBestPairsHodo_ProjS1(NULL),
   fhMapBestPairsHodo_ProjS2(NULL),
   fhMapBestPairsHodoS1_H1(NULL),
   fhMapBestPairsHodoS1_H2(NULL),
   fhMapBestPairsHodoS1_S1(NULL),
   fhMapBestPairsHodoS1_Proj(NULL),
   fhMapBestPairsHodoS2_H1(NULL),
   fhMapBestPairsHodoS2_H2(NULL),
   fhMapBestPairsHodoS2_S2(NULL),
   fhMapBestPairsHodoS2_Proj(NULL),
   fhMapBestPairsHodoS1S2_H1(NULL),
   fhMapBestPairsHodoS1S2_H2(NULL),
   fhMapBestPairsHodoS1S2_S1(NULL),
   fhMapBestPairsHodoS1S2_Proj(NULL),
   fhMapBestPairsHodoS2S1_H1(NULL),
   fhMapBestPairsHodoS2S1_H2(NULL),
   fhMapBestPairsHodoS2S1_S2(NULL),
   fhMapBestPairsHodoS2S1_Proj(NULL),
   fhMapBestPairsHodoGeoCutS1_H1(NULL),
   fhMapBestPairsHodoGeoCutS1_H2(NULL),
   fhMapBestPairsHodoGeoCutS1_Proj(NULL),
   fhMapBestPairsHodoGeoCutS2_H1(NULL),
   fhMapBestPairsHodoGeoCutS2_H2(NULL),
   fhMapBestPairsHodoGeoCutS2_Proj(NULL),
   fhMapBestPairsHodoS1GeoCut_H1(NULL),
   fhMapBestPairsHodoS1GeoCut_H2(NULL),
   fhMapBestPairsHodoS1GeoCut_S1(NULL),
   fhMapBestPairsHodoS1GeoCut_Proj(NULL),
   fhMapBestPairsHodoS2GeoCut_H1(NULL),
   fhMapBestPairsHodoS2GeoCut_H2(NULL),
   fhMapBestPairsHodoS2GeoCut_S2(NULL),
   fhMapBestPairsHodoS2GeoCut_Proj(NULL),
   fhMapBestPairsHodoS1S2GeoCut_H1(NULL),
   fhMapBestPairsHodoS1S2GeoCut_H2(NULL),
   fhMapBestPairsHodoS1S2GeoCut_S1(NULL),
   fhMapBestPairsHodoS1S2GeoCut_Proj(NULL),
   fhMapBestPairsHodoS2S1GeoCut_H1(NULL),
   fhMapBestPairsHodoS2S1GeoCut_H2(NULL),
   fhMapBestPairsHodoS2S1GeoCut_S2(NULL),
   fhMapBestPairsHodoS2S1GeoCut_Proj(NULL),
   fhResidualsBestPairsHodoS1(NULL),
   fhResidualsBestPairsHodoS2(NULL),
   fhResidualsBestPairsHodoS1S2(NULL),
   fhResidualsBestPairsHodoS2S1(NULL),
   fhResidualsBestPairsHodoS1GeoCut(NULL),
   fhResidualsBestPairsHodoS2GeoCut(NULL),
   fhResidualsBestPairsHodoS1S2GeoCut(NULL),
   fhResidualsBestPairsHodoS2S1GeoCut(NULL),
   fhAdcRawBestPairsHodoS1N(NULL),
   fhAdcRawBestPairsHodoS1P(NULL),
   fhAdcRawBestPairsHodoS2N(NULL),
   fhAdcRawBestPairsHodoS2P(NULL),
   fhAdcRawBestPairsHodoS1S2N(NULL),
   fhAdcRawBestPairsHodoS1S2P(NULL),
   fhAdcRawBestPairsHodoS2S1N(NULL),
   fhAdcRawBestPairsHodoS2S1P(NULL),
   fhEfficiency(NULL),
   fhEfficiencyMapS1(NULL),
   fhEfficiencyMapS2(NULL),
   fhEfficiencyMapS1S2(NULL),
   fhEfficiencyMapS2S1(NULL),
   fhEfficiencyMapS1GeoCut(NULL),
   fhEfficiencyMapS2GeoCut(NULL),
   fhEfficiencyMapS1S2GeoCut(NULL),
   fhEfficiencyMapS2S1GeoCut(NULL),
   fcCanvasProjectionMaps(NULL),
   fcCanvasEfficiencyMaps(NULL),
   fcMsSizeAll(NULL),
   fhTestBoundariesSensor1(NULL),
   fhTestBoundariesSensor2(NULL),
   fhTestBoundariesSensor1NoDead(NULL),
   fhTestBoundariesSensor2NoDead(NULL),
   fhTestChanMatchSensor1(NULL),
   fhTestChanMatchSensor2(NULL),
   fhTestChanMapSensor1(NULL),
   fhTestChanMapSensor2(NULL),
   fhTestChanMapPhysSensor1(NULL),
   fhTestChanMapPhysSensor2(NULL),
   fvHitsS1N(),
   fvHitsS1P(),
   fvHitsS2N(),
   fvHitsS2P(),
   fhTestMapHodoS1N(NULL),
   fhTestMapHodoS1P(NULL),
   fhTestMapHodoS2N(NULL),
   fhTestMapHodoS2P(NULL),
   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
   fhDtNeighborChansS1N(NULL),
   fhDtNeighborChansS1P(NULL),
   fhDtNeighborChansS2N(NULL),
   fhDtNeighborChansS2P(NULL),
   fvLastHitChanS1N(),
   fvLastHitChanS1P(),
   fvLastHitChanS2N(),
   fvLastHitChanS2P(),
   fvClustersS1N(),
   fvClustersS1P(),
   fvClustersS2N(),
   fvClustersS2P(),
   fhClusterAdcVsSizeS1N(NULL),
   fhClusterAdcVsSizeS1P(NULL),
   fhClusterAdcVsSizeS2N(NULL),
   fhClusterAdcVsSizeS2P(NULL),
   fhClusterAdcVsSizeS1N_MatchS1(NULL),
   fhClusterAdcVsSizeS1P_MatchS1(NULL),
   fhClusterAdcVsSizeS2N_MatchS2(NULL),
   fhClusterAdcVsSizeS2P_MatchS2(NULL),
   fhClusterS1N_CenterNbMatchS1P(NULL),
   fhClusterS1P_CenterNbMatchS1N(NULL),
   fhClusterS2N_CenterNbMatchS2P(NULL),
   fhClusterS2P_CenterNbMatchS2N(NULL),
   fhClusterS1N_AdcNbMatchS1P(NULL),
   fhClusterS1P_AdcNbMatchS1N(NULL),
   fhClusterS2N_AdcNbMatchS2P(NULL),
   fhClusterS2P_AdcNbMatchS2N(NULL),
   fhDtNeighborChansH1X(NULL),
   fhDtNeighborChansH1Y(NULL),
   fhDtNeighborChansH2X(NULL),
   fhDtNeighborChansH2Y(NULL),
   fvLastHitChanH1X(),
   fvLastHitChanH1Y(),
   fvLastHitChanH2X(),
   fvLastHitChanH2Y(),
   fvClustersH1X(),
   fvClustersH1Y(),
   fvClustersH2X(),
   fvClustersH2Y(),
   fhClusterAdcVsSizeH1X(NULL),
   fhClusterAdcVsSizeH1Y(NULL),
   fhClusterAdcVsSizeH2X(NULL),
   fhClusterAdcVsSizeH2Y(NULL),
   fhClusterPosition_MatchH1(NULL),
   fhClusterPosition_MatchH2(NULL)
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
{
}

CbmCosy2018MonitorEfficiency::~CbmCosy2018MonitorEfficiency()
{
}

Bool_t CbmCosy2018MonitorEfficiency::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmCosy2018MonitorEfficiency::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackParHodo = (CbmCern2017UnpackParHodo*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParHodo"));
   fUnpackParSts  = (CbmCern2017UnpackParSts*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParSts"));
}


Bool_t CbmCosy2018MonitorEfficiency::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmCosy2018MonitorEfficiency::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();

   fuNrOfDpbs       = fUnpackParHodo->GetNrOfDpbs();
   fuNbElinksPerDpb = fUnpackParHodo->GetNbElinksPerDpb();
   fuNbStsXyters    = fUnpackParHodo->GetNbStsXyters();
   fuNbChanPerAsic  = fUnpackParHodo->GetNbChanPerAsic();


   LOG(info) << "Nr. of STS DPBs:       " << fuNrOfDpbs;

   fDpbIdIndexMap.clear();
   fvuElinkToAsic.resize( fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fDpbIdIndexMap[ fUnpackParHodo->GetDpbId( uDpb )  ] = uDpb;
      LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = "
                << std::setw(4) << std::hex << fUnpackParHodo->GetDpbId( uDpb )
                << std::dec
                << " => " << fDpbIdIndexMap[ fUnpackParHodo->GetDpbId( uDpb )  ];

      fvuElinkToAsic[uDpb].resize( fuNbElinksPerDpb );
      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
         fvuElinkToAsic[uDpb][uElink] = fUnpackParHodo->GetElinkToAsicIdx( uDpb * fuNbElinksPerDpb + uElink );
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   LOG(info) << "Nr. eLinks per DPB:    " << fuNbElinksPerDpb;
   LOG(info) << "Nr. of StsXyter ASICs: " << fuNbStsXyters;
   LOG(info) << "Nb. channels per ASIC: " << fuNbChanPerAsic;

   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      LOG(info) << "ASIC ID for eLinks in DPB #" << std::setw(2) << uDpb << ": ";

      std::stringstream ss;
      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      {
         if( 0 == uElink % 10 )
            ss << "\n------> ";

         ss << std::setw( 5 ) << fvuElinkToAsic[uDpb][uElink] << " ";
      } // for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      LOG(info) << ss.str();
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   LOG(info) << "ASIC Idx for Hodo 1: " << fUnpackParHodo->GetAsicIndexHodo1();
   LOG(info) << "ASIC Idx for Hodo 2: " << fUnpackParHodo->GetAsicIndexHodo2();

   LOG(info) << "XY swapped in Hodo 1: " << fUnpackParHodo->IsXySwappedHodo1();
   LOG(info) << "XY swapped in Hodo 2: " << fUnpackParHodo->IsXySwappedHodo2();

   LOG(info) << "X axis inverted in Hodo 1: " << fUnpackParHodo->IsXInvertedHodo1();
   LOG(info) << "X axis inverted in Hodo 1: " << fUnpackParHodo->IsYInvertedHodo1();
   LOG(info) << "X axis inverted in Hodo 2: " << fUnpackParHodo->IsXInvertedHodo2();
   LOG(info) << "X axis inverted in Hodo 2: " << fUnpackParHodo->IsYInvertedHodo2();

   LOG(info) << "STS Tan(alpha): " << fdStereoAngleTan;
   LOG(info) << "STS strip range NP: " << fdStripRangeNP;

   // Internal status initialization
   fvulCurrentTsMsb.resize( fuNrOfDpbs );
   fvuCurrentTsMsbCycle.resize( fuNrOfDpbs );
   fvuElinkLastTsHit.resize( fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fvulCurrentTsMsb[uDpb]     = 0;
      fvuCurrentTsMsbCycle[uDpb] = 0;
/*
      fvuElinkLastTsHit[uDpb].resize( fuNbElinksPerDpb );
      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      {
         fvuElinkLastTsHit[uDpb][uElink]  = 0;
      } // for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
*/
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   fvulChanLastHitTime.resize( fuNbStsXyters );
   fvdChanLastHitTime.resize( fuNbStsXyters );
   fvdMsTime.resize( fuMaxNbMicroslices );
   fvuChanNbHitsInMs.resize( fuNbStsXyters );
   fvdChanLastHitTimeInMs.resize( fuNbStsXyters );
   fvusChanLastHitAdcInMs.resize( fuNbStsXyters );
//   fvmChanHitsInTs.resize( fuNbStsXyters );
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fvulChanLastHitTime[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvdChanLastHitTime[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvuChanNbHitsInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvdChanLastHitTimeInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvusChanLastHitAdcInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
//      fvmChanHitsInTs[ uXyterIdx ].resize( fuNbChanPerAsic );
      for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
      {
         fvulChanLastHitTime[ uXyterIdx ][ uChan ] = 0;
         fvdChanLastHitTime[ uXyterIdx ][ uChan ] = -1.0;

         fvuChanNbHitsInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
         fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
         fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
//         fvmChanHitsInTs[ uXyterIdx ][ uChan ].clear();
         for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
         {
            fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
            fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
         } // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
      } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   LOG(info) << "CbmCosy2018MonitorEfficiency::ReInitContainers => Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
   LOG(info) << "CbmCosy2018MonitorEfficiency::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
   LOG(info) << "CbmCosy2018MonitorEfficiency::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuMaxNbMicroslices;

   /// Counting of hits without proper timestamp
   fvulTimeLastHitAsicChan.resize( fuNbStsXyters );
   fviAdcLastHitAsicChan.resize( fuNbStsXyters );
   fvuNbSameHitAsicChan.resize( fuNbStsXyters );
   fvuNbSameFullHitAsicChan.resize( fuNbStsXyters );
   fvulStartTimeLastS.resize( fuNbStsXyters );
   fvuNbHitDiffTsAsicLastS.resize( fuNbStsXyters );
   fvuNbHitSameTsAsicLastS.resize( fuNbStsXyters );
   fvuNbHitSameTsAdcAsicLastS.resize( fuNbStsXyters );
   fvbAsicHasDuplicInMs.resize( fuNbStsXyters );
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fvulTimeLastHitAsicChan[ uXyterIdx ].resize( fuNbChanPerAsic );
      fviAdcLastHitAsicChan[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvuNbSameHitAsicChan[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvuNbSameFullHitAsicChan[ uXyterIdx ].resize( fuNbChanPerAsic );
      for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
      {
         fvulTimeLastHitAsicChan[ uXyterIdx ][ uChan ] = 0;
         fviAdcLastHitAsicChan[ uXyterIdx ][ uChan ]   = 0;
         fvuNbSameHitAsicChan[ uXyterIdx ][ uChan ]    = 0;
         fvuNbSameFullHitAsicChan[ uXyterIdx ][ uChan ]    = 0;
      } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )

      fvulStartTimeLastS[ uXyterIdx ]         = 0;
      fvuNbHitDiffTsAsicLastS[ uXyterIdx ]    = 0;
      fvuNbHitSameTsAsicLastS[ uXyterIdx ]    = 0;
      fvuNbHitSameTsAdcAsicLastS[ uXyterIdx ] = 0;
      fvbAsicHasDuplicInMs[ uXyterIdx ] = kFALSE;
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fvLastHitChanS1N.resize( fuNbChanPerAsic );
   fvLastHitChanS1P.resize( fuNbChanPerAsic );
   fvLastHitChanS2N.resize( fuNbChanPerAsic );
   fvLastHitChanS2P.resize( fuNbChanPerAsic );

   fvLastHitChanH1X.resize( fuNbChanPerAsic/2 );
   fvLastHitChanH1Y.resize( fuNbChanPerAsic/2 );
   fvLastHitChanH2X.resize( fuNbChanPerAsic/2 );
   fvLastHitChanH2Y.resize( fuNbChanPerAsic/2 );

   LOG(info) << "CbmCosy2018MonitorEfficiency::ReInitContainers =>  Dual STS mode:  " << fbDualStsEna;
   return kTRUE;
}

void CbmCosy2018MonitorEfficiency::SetCoincidenceBorderHodo( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenterHodo = dCenterPos;
   fdCoincBorderHodo = dBorderVal;
   fdCoincMinHodo    = dCenterPos - dBorderVal;
   fdCoincMaxHodo    = dCenterPos + dBorderVal;
}
void CbmCosy2018MonitorEfficiency::SetCoincidenceBorderHodoBoth( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenterHodoBoth = dCenterPos;
   fdCoincBorderHodoBoth = dBorderVal;
   fdCoincMinHodoBoth    = dCenterPos - dBorderVal;
   fdCoincMaxHodoBoth    = dCenterPos + dBorderVal;
}
void CbmCosy2018MonitorEfficiency::SetCoincidenceBorderSts1( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenterSts1 = dCenterPos;
   fdCoincBorderSts1 = dBorderVal;
   fdCoincMinSts1    = dCenterPos - dBorderVal;
   fdCoincMaxSts1    = dCenterPos + dBorderVal;
}
void CbmCosy2018MonitorEfficiency::SetCoincidenceBorderSts2( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenterSts2 = dCenterPos;
   fdCoincBorderSts2 = dBorderVal;
   fdCoincMinSts2    = dCenterPos - dBorderVal;
   fdCoincMaxSts2    = dCenterPos + dBorderVal;
}
void CbmCosy2018MonitorEfficiency::SetCoincidenceBorderHodoSts1( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenterHodoS1 = dCenterPos;
   fdCoincBorderHodoS1 = dBorderVal;
   fdCoincMinHodoS1    = dCenterPos - dBorderVal;
   fdCoincMaxHodoS1    = dCenterPos + dBorderVal;
}
void CbmCosy2018MonitorEfficiency::SetCoincidenceBorderHodoSts2( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenterHodoS2 = dCenterPos;
   fdCoincBorderHodoS2 = dBorderVal;
   fdCoincMinHodoS2    = dCenterPos - dBorderVal;
   fdCoincMaxHodoS2    = dCenterPos + dBorderVal;
}
void CbmCosy2018MonitorEfficiency::SetCoincidenceBorder( Double_t dCenterPos, Double_t dBorderVal )
{
   fdCoincCenter     = dCenterPos;
   fdCoincBorder     = dBorderVal;
   fdCoincMin        = dCenterPos - dBorderVal;
   fdCoincMax        = dCenterPos + dBorderVal;
}

void CbmCosy2018MonitorEfficiency::SetPositionsMmZ( Double_t dPosH1, Double_t dPosH2, Double_t dPosS1, Double_t dPosS2 )
{
   if( dPosH1 == dPosH2 )
      LOG(fatal) << "CbmCosy2018MonitorEfficiency::SetPositionsZ => Hodo 1 and 2 cannot have the same Z position "
                 << "as projection to STS would lead to a division by 0!!! "
                 << dPosH1 << " VS " << dPosH2;

   fdPosZH1   = dPosH1;
   fdPosZH2   = dPosH2;
   fdPosZS1   = dPosS1;
   fdPosZS2   = dPosS2;
   fdRatioZS1 = ( fdPosZS1 - fdPosZH1 ) / ( fdPosZH2 - fdPosZH1 );
   fdRatioZS2 = ( fdPosZS2 - fdPosZH1 ) / ( fdPosZH2 - fdPosZH1 );
}

void CbmCosy2018MonitorEfficiency::CreateHistograms()
{
   LOG(info) << "Create histograms for " << GetName();

   TString sHistName{""};
   TString title{""};

   sHistName = "hHodoMessageType";
   title = "Nb of message for each type; Type";
   fhHodoMessType = new TH1I(sHistName, title, 5, 0., 5.);
   fhHodoMessType->GetXaxis()->SetBinLabel( 1, "Dummy");
   fhHodoMessType->GetXaxis()->SetBinLabel( 2, "Hit");
   fhHodoMessType->GetXaxis()->SetBinLabel( 3, "TsMsb");
   fhHodoMessType->GetXaxis()->SetBinLabel( 4, "Epoch");
   fhHodoMessType->GetXaxis()->SetBinLabel( 5, "Empty");
/* *** Missing int + MessType OP!!!! ****
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

   sHistName = "hHodoSysMessType";
   title = "Nb of system message for each type; System Type";
   fhHodoSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
/*
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/

   sHistName = "hHodoMessageTypePerDpb";
   title = "Nb of message of each type for each DPB; DPB; Type";
   fhHodoMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 5, 0., 5.);
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 1, "Dummy");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 2, "Hit");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 3, "TsMsb");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 4, "Epoch");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 5, "Empty");
/* *** Missing int + MessType OP!!!! ****
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

   sHistName = "hHodoSysMessTypePerDpb";
   title = "Nb of system message of each type for each DPB; DPB; System Type";
   fhHodoSysMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);
/*
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/

   sHistName = "hHodoMessageTypePerElink";
   title = "Nb of message of each type for each eLink; eLink; Type";
   fhHodoMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 5, 0., 5.);
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 1,       "Dummy");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 2,         "Hit");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 3,       "TsMsb");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 4, "ReadDataAck");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 5,         "Ack");
/* *** Missing int + MessType OP!!!! ****
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

   sHistName = "hHodoSysMessTypePerElink";
   title = "Nb of system message of each type for each eLink; eLink; System Type";
   fhHodoSysMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 17, 0., 17.);
/*
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/

   // Number of rate bins =
   //      9 for the sub-unit decade
   //    + 9 for each unit of each decade * 10 for the subdecade range
   //    + 1 for the closing bin top edge
   const Int_t iNbDecadesRate    = 9;
   const Int_t iNbStepsDecade    = 9;
   const Int_t iNbSubStepsInStep = 10;
   const Int_t iNbBinsRate = iNbStepsDecade
                           + iNbStepsDecade * iNbSubStepsInStep * iNbDecadesRate
                           + 1;
   Double_t dBinsRate[iNbBinsRate];
      // First fill sub-unit decade
   for( Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU ++ )
      dBinsRate[ iSubU ] = 0.1 * ( 1 + iSubU );
   std::cout << std::endl;
      // Then fill the main decades
   Double_t dSubstepSize = 1.0 / iNbSubStepsInStep;
   for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   {
      Double_t dBase = std::pow( 10, iDecade );
      Int_t iDecadeIdx = iNbStepsDecade
                       + iDecade * iNbStepsDecade * iNbSubStepsInStep;
      for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
      {
         Int_t iStepIdx = iDecadeIdx + iStep * iNbSubStepsInStep;
         for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
         {
            dBinsRate[ iStepIdx + iSubStep ] = dBase * (1 + iStep)
                                             + dBase * dSubstepSize * iSubStep;
         } // for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
      } // for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
   } // for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   dBinsRate[ iNbBinsRate - 1 ] = std::pow( 10, iNbDecadesRate );

   UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
   fuLongHistoBinNb = uAlignedLimit / fuLongHistoBinSizeSec;

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      // Channel counts
      sHistName = Form( "hHodoChanCntRaw_%03u", uXyterIdx );
      title = Form( "Hits Count per channel, StsXyter #%03u; Channel; Hits []", uXyterIdx );
      fhHodoChanCntRaw.push_back( new TH1I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "hHodoChanCntRawGood_%03u", uXyterIdx );
      title = Form( "Hits Count per channel in good MS, StsXyter #%03u; Channel; Hits []", uXyterIdx );
      fhHodoChanCntRawGood.push_back( new TH1I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Raw Adc Distribution
      sHistName = Form( "hHodoChanAdcRaw_%03u", uXyterIdx );
      title = Form( "Raw Adc distribution per channel, StsXyter #%03u; Channel []; Adc []; Hits []", uXyterIdx );
      fhHodoChanAdcRaw.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 ) );

      // Raw Adc Distribution profile
      sHistName = Form( "hHodoChanAdcRawProfc_%03u", uXyterIdx );
      title = Form( "Raw Adc prodile per channel, StsXyter #%03u; Channel []; Adc []", uXyterIdx );
      fhHodoChanAdcRawProf.push_back( new TProfile(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Raw Ts Distribution
      sHistName = Form( "hHodoChanRawTs_%03u", uXyterIdx );
      title = Form( "Raw Timestamp distribution per channel, StsXyter #%03u; Channel []; Ts []; Hits []", uXyterIdx );
      fhHodoChanRawTs.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins -0.5 ) );

      // Missed event flag
      sHistName = Form( "hHodoChanMissEvt_%03u", uXyterIdx );
      title = Form( "Missed Event flags per channel, StsXyter #%03u; Channel []; Miss Evt []; Hits []", uXyterIdx );
      fhHodoChanMissEvt.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 2, -0.5, 1.5 ) );

      // Missed event flag counts evolution

      sHistName = Form( "hHodoChanMissEvtEvo_%03u", uXyterIdx );
      title = Form( "Missed Evt flags per second & channel in StsXyter #%03u; Time [s]; Channel []; Missed Evt flags []", uXyterIdx );
      fhHodoChanMissEvtEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Missed event flag counts evo per StsXyter

      sHistName = Form( "hHodoFebMissEvtEvo%03u", uXyterIdx );
      title = Form( "Missed Evt flags per second in StsXyter #%03u; Time [s]; Missed Evt flags []", uXyterIdx );
      fhHodoFebMissEvtEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );

      // Hit rates evo per channel
      sHistName = Form( "hHodoChanRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [s]; Channel []; Hits []", uXyterIdx );
      fhHodoChanHitRateEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Hit rates evo per StsXyter
      sHistName = Form( "hHodoFebRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [s]; Hits []", uXyterIdx );
      fhHodoFebRateEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );

      // Hit rates evo per channel, 1 minute bins, 24h
      sHistName = Form( "hHodoChanRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [min]; Channel []; Hits []", uXyterIdx );
      fhHodoChanHitRateEvoLong.push_back( new TH2D( sHistName, title,
                                                fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Hit rates evo per StsXyter, 1 minute bins, 24h
      sHistName = Form( "hHodoFebRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [min]; Hits []", uXyterIdx );
      fhHodoFebRateEvoLong.push_back( new TH1D(sHistName, title,
                                                fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5 ) );

   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   UInt_t uNbBinEvo = 32768 + 1;
   Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

   UInt_t uNbBinDtHodo     = static_cast<UInt_t>( (fdCoincMaxHodo     - fdCoincMinHodo    )/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtBothHodo = static_cast<UInt_t>( (fdCoincMaxHodoBoth - fdCoincMinHodoBoth)/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtSts1     = static_cast<UInt_t>( (fdCoincMaxSts1     - fdCoincMinSts1    )/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtSts2     = static_cast<UInt_t>( (fdCoincMaxSts2     - fdCoincMinSts2    )/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtHodoS1   = static_cast<UInt_t>( (fdCoincMaxHodoS1   - fdCoincMinHodoS1  )/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtHodoS2   = static_cast<UInt_t>( (fdCoincMaxHodoS2   - fdCoincMinHodoS2  )/stsxyter::kdClockCycleNs );
//   UInt_t uNbBinDtSyst     = static_cast<UInt_t>( (fdCoincMax         - fdCoincMin        )/stsxyter::kdClockCycleNs );

   /// Coincidences inside each detector ----------------------------///
   sHistName = "fhSetupSortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhSetupSortedDtX1Y1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhSetupSortedDtX2Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtN1P1";
   title =  "Time diff for hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhSetupSortedDtN1P1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtN2P2";
   title =  "Time diff for hits Sts 2 N and Sts 2 P; tP2 - tN2 [ns]; Counts";
   fhSetupSortedDtN2P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtX1Y1X2Y2";
   title =  "Time diff for hits Hodo 1 (X,Y) vs Hodo 2 (X,Y); t<X2,Y2> - t<X1,Y1> [ns]; Counts";
   fhSetupSortedDtX1Y1X2Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtN1P1N2P2";
   title =  "Time diff for hits STS 1 (N,P) vs STS 2 (N,P); t<N2,P2> - t<N1,P1> [ns]; Counts";
   fhSetupSortedDtN1P1N2P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtX1Y1X2Y2N1P1";
   title =  "Time diff for hits Hodo (X1,Y1,X2,Y2) vs Sts 1 (N,P); t<N1,P1> - t<X1,Y1,X2,Y2> [ns]; Counts";
   fhSetupSortedDtX1Y1X2Y2N1P1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtX1Y1X2Y2N2P2";
   title =  "Time diff for hits Hodo (X1,Y1,X2,Y2) vs Sts 2 (N,P); t<N2,P2> - t<X1,Y1,X2,Y2> [ns]; Counts";
   fhSetupSortedDtX1Y1X2Y2N2P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedDtH1H2S1S2";
   title =  "Time diff for hits Hodo (X1,Y1,X2,Y2) vs Sts (N1,P1,N2,P2); t<N1,P1,N2,P2> - t<X1,Y1,X2,Y2> [ns]; Counts";
   fhSetupSortedDtH1H2S1S2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhSetupSortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and Y; X channel Hodo 1 []; Y channel Hodo 1 []; hits []";
   fhSetupSortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSetupSortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 2 axis X and Y; X channel Hodo 2 []; Y channel Hodo 2 []; hits []";
   fhSetupSortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSetupSortedMapN1P1";
   title = "Sorted hits in coincidence for Sts 1 sides N and P; N channel Sts 1 []; P channel Sts 1 []; hits []";
   fhSetupSortedMapN1P1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhSetupSortedMapN2P2";
   title = "Sorted hits in coincidence for Sts 2 sides N and P; N channel Sts 2 []; P channel Sts 2 []; hits []";
   fhSetupSortedMapN2P2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   // Coincidence counts evolution between some axis of the hodoscopes
   sHistName = "hSetupSortedCntEvoX1Y1";
   title = "Nb of coincidences in both X1 and Y1 per s; Time [s]; X1-Y1 coincidences []";
   fhSetupSortedCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hSetupSortedCntEvoX2Y2";
   title = "Nb of coincidences in both X2 and Y2 per s; Time [s]; X2-Y2 coincidences []";
   fhSetupSortedCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hSetupSortedCntEvoN1P1";
   title = "Nb of coincidences in both N1 and P1 per s; Time [s]; N1-P1 coincidences []";
   fhSetupSortedCntEvoN1P1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hSetupSortedCntEvoN2P2";
   title = "Nb of coincidences in both N2 and P2 per s; Time [s]; N2-P2 coincidences []";
   fhSetupSortedCntEvoN2P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhSetupSortedDtX1";
   title =  "Time diff for consecutive hits in Hodo 1 X; tX1n - tX1 [ns]; Counts";
   fhSetupSortedDtX1 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5 );

   sHistName = "fhSetupSortedDtY1";
   title =  "Time diff for consecutive hits in Hodo 1 Y; tY1n - tY1 [ns]; Counts";
   fhSetupSortedDtY1 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);

   sHistName = "fhSetupSortedDtX2";
   title =  "Time diff for consecutive hits in Hodo 2 X; tX2n - tX2 [ns]; Counts";
   fhSetupSortedDtX2 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);

   sHistName = "fhSetupSortedDtY2";
   title =  "Time diff for consecutive hits in Hodo 2 Y; tY2n - tY2 [ns]; Counts";
   fhSetupSortedDtY2 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);

   sHistName = "fhSetupSortedDtN1";
   title =  "Time diff for consecutive hits in Sts 1 Y; tN1n - tN1 [ns]; Counts";
   fhSetupSortedDtN1 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);

   sHistName = "fhSetupSortedDtP1";
   title =  "Time diff for consecutive hits in Sts 1 Y; tP1n - tP1 [ns]; Counts";
   fhSetupSortedDtP1 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);

   sHistName = "fhSetupSortedDtN2";
   title =  "Time diff for consecutive hits in Sts 1 Y; tN2n - tN2 [ns]; Counts";
   fhSetupSortedDtN2 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);

   sHistName = "fhSetupSortedDtP2";
   title =  "Time diff for consecutive hits in Sts 1 Y; tP2n - tP2 [ns]; Counts";
   fhSetupSortedDtP2 = new TH1I(sHistName, title, uNbBinEvo, -0.5, 2*dMaxEdgeEvo -0.5);
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///


///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   Double_t dAdcMinMax = stsxyter::kuHitNbAdcBins + 0.5;
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      sHistName = Form( "fhSetupSortedNbSameTsChan%02u", uXyterIdx);
      title =  Form( "Nb Hits with same TS per second & channel Asic %02u; Time [s]; Channel []; Hits []", uXyterIdx);
      fhSetupSortedNbSameTsChan.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "fhSetupSortedSameTsAdcChan%02u", uXyterIdx);
      title = Form( "Adc distribution per channel for Hits with same TS Asic %02u; Channel []; Adc []; Hits []", uXyterIdx);
      fhSetupSortedSameTsAdcChan.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 ) );

      sHistName = Form( "fhSetupSortedSameTsAdcDiff%02u", uXyterIdx);
      title = Form( "Adc difference vs consecutive hit idx per channel for Hits with same TS Asic %02u; Nb same TS before []; Adc diff []; Hits []", uXyterIdx);
      fhSetupSortedSameTsAdcDiff.push_back( new TH2I(sHistName, title,
                                 256, -0.5, 255.5,
                                 2*dAdcMinMax, -dAdcMinMax, dAdcMinMax ) );

      sHistName = Form( "fhSetupSortedRatioSameTsChan%02u", uXyterIdx);
      title =  Form( "Ratio Hits with same TS per second & channel Asic %02u; Time [s]; Channel []; Hits []", uXyterIdx);
      fhSetupSortedRatioSameTsChan.push_back( new TProfile2D( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "fhSetupSortedNbConsSameTsChan%02u", uXyterIdx);
      title = Form( "Nb consecutive Hits with same TS per channel Asic %02u; Channel []; Hits with same TS []; []", uXyterIdx);
      fhSetupSortedNbConsSameTsChan.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 1024, -0.5, 10240.5 ) );

      sHistName = Form( "fhSetupSortedNbConsSameTsAdcChan%02u", uXyterIdx);
      title = Form( "Nb consecutive Hits with same TS and ADC per channel Asic %02u; Channel []; Hits with same TS and ADC []; []", uXyterIdx);
      fhSetupSortedNbConsSameTsAdcChan.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 256, -0.5, 255.5 ) );

      sHistName = Form( "fhSetupSortedNbConsSameTsVsTsAdc%02u", uXyterIdx);
      title = Form( "Nb consecutive Hits with same TS vs same TS and ADC Asic %02u; Hits with same TS []; Hits with same TS and ADC []; []", uXyterIdx);
      fhSetupSortedNbConsSameTsVsTsAdc.push_back( new TH2I(sHistName, title,
                                 256, -0.5, 255.5,
                                 256, -0.5, 255.5 ) );

      sHistName = Form( "fhSetupSortedAsicRatioSameTsVsFlux%02u", uXyterIdx);
      title = Form( "Ratio of hits with same TS vs Hit flux Asic %02u; Hits flux [1/s]; Same Ts / All Hits []; []", uXyterIdx);
      fhSetupSortedAsicRatioSameTsVsFlux.push_back( new TH2I(sHistName, title,
                                 iNbBinsRate - 1, dBinsRate,
                                 101, -0.005, 1.005 ) );

      sHistName = Form( "fhSetupSortedAsicRatioSameTsAdcVsFlux%02u", uXyterIdx);
      title = Form( "Ratio of hits with same TS and ADC vs Hit flux Asic %02u; Hits flux [1/s]; Same Ts and ADC / All Hits []; []", uXyterIdx);
      fhSetupSortedAsicRatioSameTsAdcVsFlux.push_back( new TH2I(sHistName, title,
                                 iNbBinsRate - 1, dBinsRate,
                                 101, -0.005, 1.005 ) );

      sHistName = Form( "fhSetupSortedAsicRatioSameAdcSameTsVsFlux%02u", uXyterIdx);
      title = Form( "Ratio of hits with same ADC amond same TS vs Hit flux Asic %02u; Hits flux [1/s]; Same TS and ADC / Same TS []; []", uXyterIdx);
      fhSetupSortedAsicRatioSameAdcSameTsVsFlux.push_back( new TH2I(sHistName, title,
                                 iNbBinsRate - 1, dBinsRate,
                                 101, -0.005, 1.005 ) );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhHodoX1SpillEvo";
   title = "Hodo X1 counts vs time in spill; time [s]; X1 channel []; Hits []";
   fhHodoX1SpillEvo = new TH2I(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhHodoY1SpillEvo";
   title = "Hodo Y1 counts vs time in spill; time [s]; Y1 channel []; Hits []";
   fhHodoY1SpillEvo = new TH2I(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   sHistName = "fhHodoX2SpillEvo";
   title = "Hodo X2 counts vs time in spill; time [s]; X2 channel []; Hits []";
   fhHodoX2SpillEvo = new TH2I(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   sHistName = "fhHodoY2SpillEvo";
   title = "Hodo Y2 counts vs time in spill; time [s]; Y2 channel []; Hits []";
   fhHodoY2SpillEvo = new TH2I(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhHodoX1SpillEvoProf";
   title = "Hodo X1 counts vs time in spill; time [s]; X1 channel []; Hits []";
   fhHodoX1SpillEvoProf = new TProfile(sHistName, title,
                              10*fdSpillEvoLength, 0, fdSpillEvoLength);

   sHistName = "fhHodoY1SpillEvoProf";
   title = "Hodo Y1 counts vs time in spill; time [s]; Y1 channel []; Hits []";
   fhHodoY1SpillEvoProf = new TProfile(sHistName, title,
                              10*fdSpillEvoLength, 0, fdSpillEvoLength);
   sHistName = "fhHodoX2SpillEvoProf";
   title = "Hodo X2 counts vs time in spill; time [s]; X2 channel []; Hits []";
   fhHodoX2SpillEvoProf = new TProfile(sHistName, title,
                              10*fdSpillEvoLength, 0, fdSpillEvoLength);
   sHistName = "fhHodoY2SpillEvoProf";
   title = "Hodo Y2 counts vs time in spill; time [s]; Y2 channel []; Hits []";
   fhHodoY2SpillEvoProf = new TProfile(sHistName, title,
                              10*fdSpillEvoLength, 0, fdSpillEvoLength);

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   sHistName = "fhRatioMsDuplicateQuality";
   title = "Fraction of MS with a given number od ASICs without duplicated hits; Clean Asics Nb []; MS fraction []";
   fhRatioMsDuplicateQuality = new TProfile(sHistName, title, fuNbStsXyters + 1, 0, fuNbStsXyters + 1 );

   for( UInt_t uXyterIdx = 0; uXyterIdx <= fuNbStsXyters; ++uXyterIdx )
   {
      sHistName = Form( "fhEvoMsDuplicateQuality%02u", uXyterIdx);
      title = Form( "Fraction of MS with %02u ASICs without duplicate hits vs time; time [s]; MS fraction []", uXyterIdx);
      fhEvoMsDuplicateQuality.push_back( new TProfile(sHistName, title, 300000, 0, 300.0 ) );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   sHistName = "fhSizeCleanMs";
   title = "Size in Hits of MS without duplicated hits; Size [Hits]; MS Nb []";
   fhSizeCleanMs = new TH1I(sHistName, title, 500000, 0, 500000 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   Double_t dMapSizeMmX = kdSensorsSzX + 100 * kdPitchMm;
   Double_t dMapSizeMmY = kdSensorsSzY + 100 * kdPitchMm;
   Int_t iNbBinsX = dMapSizeMmX / kdPitchMm; // 58 um bin
   Int_t iNbBinsY = dMapSizeMmY / kdPitchMm / 7.54; // 116 um bin

   Double_t dMapProjSizeMmXY =  70;
   Int_t    iMapProjNbBinsXY = 140;
/*
   iNbBinsX / 4, -dMapSizeMmX, dMapSizeMmX,
   iNbBinsY / 4, -dMapSizeMmY, dMapSizeMmY );
*/

   sHistName = "fhStsSortedMapS1";
   title = "Sorted hits in coincidence for Sts 1 axis X and Y; X1 [mm]; Y1 [mm]; Hits []";
   fhStsSortedMapS1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   if( kTRUE == fbDualStsEna )
   {
      sHistName = "fhStsSortedMapS2";
      title = "Sorted hits in coincidence for Sts 2 axis X and Y; X2 [mm]; Y2 [mm]; Hits []";
      fhStsSortedMapS2 = new TH2I( sHistName, title,
                                     iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                     iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

      sHistName = "fhStsSortedMapS1Coinc";
      title = "Sorted hits in coincidence for Sts 1 axis X and Y if Sts2 in coinc; X1 [mm]; Y1 [mm]; Hits []";
      fhStsSortedMapS1Coinc = new TH2I( sHistName, title,
                                     iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                     iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

      sHistName = "fhStsSortedMapS2Coinc";
      title = "Sorted hits in coincidence for Sts 2 axis X and Y if Sts1 in coinc; X2 [mm]; Y2 [mm]; Hits []";
      fhStsSortedMapS2Coinc = new TH2I( sHistName, title,
                                     iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                     iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );
   } // if( kTRUE == fbDualStsEna )
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhNbY1CoincPerX1";
   title = "Nb of Y1 hits in coincidence with each X1 hit; Y1 Hits in window [Hits]; Counts []";
   fhNbY1CoincPerX1 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbY2CoincPerX2";
   title = "Nb of Y2 hits in coincidence with each X2 hit; Y2 Hits in window [Hits]; Counts []";
   fhNbY2CoincPerX2 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbP1CoincPerN1";
   title = "Nb of P1 hits in coincidence with each N1 hit; P1 Hits in window [Hits]; Counts []";
   fhNbP1CoincPerN1 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbP2CoincPerN2";
   title = "Nb of P2 hits in coincidence with each N2 hit; P2 Hits in window [Hits]; Counts []";
   fhNbP2CoincPerN2 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbH2CoincPerH1";
   title = "Nb of H2 hit pairs in coincidence with each H1 hit pair; H2 pairs in window [Hit pairs]; Counts []";
   fhNbH2CoincPerH1 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS1CoincPerHodo";
   title = "Nb of S1 hit pairs in coincidence with each Hodo pair; S1 pairs in window [Hit pairs]; Counts []";
   fhNbS1CoincPerHodo = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS2CoincPerHodo";
   title = "Nb of S2 hit pairs in coincidence with each Hodo pair; S2 pairs in window [Hit pairs]; Counts []";
   fhNbS2CoincPerHodo = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS1CoincPerHodoS2";
   title = "Nb of S1 hit pairs in coincidence with each Hodo pair if matching STS 2 Hit; S1 pairs in window [Hit pairs]; Counts []";
   fhNbS1CoincPerHodoS2 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS2CoincPerHodoS1";
   title = "Nb of S2 hit pairs in coincidence with each Hodo pair if matching STS 1 Hit; S2 pairs in window [Hit pairs]; Counts []";
   fhNbS2CoincPerHodoS1 = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS1CoincPerHodoGeoCut";
   title = "Nb of S1 hit pairs in coincidence with each Hodo pair, w/ GeoCut; S1 pairs in window [Hit pairs]; Counts []";
   fhNbS1CoincPerHodoGeoCut = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS2CoincPerHodoGeoCut";
   title = "Nb of S2 hit pairs in coincidence with each Hodo pair, w/ GeoCut; S2 pairs in window [Hit pairs]; Counts []";
   fhNbS2CoincPerHodoGeoCut = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS1CoincPerHodoS2GeoCut";
   title = "Nb of S1 hit pairs in coincidence with each Hodo pair if matching STS 2 Hit, w/ GeoCut; S1 pairs in window [Hit pairs]; Counts []";
   fhNbS1CoincPerHodoS2GeoCut = new TH1I(sHistName, title, 256, 0, 256 );

   sHistName = "fhNbS2CoincPerHodoS1GeoCut";
   title = "Nb of S2 hit pairs in coincidence with each Hodo pair if matching STS 1 Hit, w/ GeoCut; S2 pairs in window [Hit pairs]; Counts []";
   fhNbS2CoincPerHodoS1GeoCut = new TH1I(sHistName, title, 256, 0, 256 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhChanMapAllMatchS1";
   title = "Map of channels in dt coincidence for Sts 1 sides N and P; N channel Sts 1 []; P channel Sts 1 []; hits []";
   fhChanMapAllMatchS1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhChanMapAllMatchS2";
   title = "Map of channels in dt coincidence for Sts 2 sides N and P; N channel Sts 2 []; P channel Sts 2 []; hits []";
   fhChanMapAllMatchS2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhChanMapBestMatchS1";
   title = "Map of channels in best dt coincidence for Sts 1 sides N and P; N channel Sts 1 []; P channel Sts 1 []; hits []";
   fhChanMapBestMatchS1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhChanMapBestMatchS2";
   title = "Map of channels in best dt coincidence for Sts 2 sides N and P; N channel Sts 2 []; P channel Sts 2 []; hits []";
   fhChanMapBestMatchS2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhDtAllPairsS1";
   title =  "Time diff for pair of hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhDtAllPairsS1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhDtAllPairsS2";
   title =  "Time diff for pair of hits Sts 2 N and Sts 1 P; tP2 - tN2 [ns]; Counts";
   fhDtAllPairsS2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhDtAllPairsHodoS1";
   title =  "Time diff for pairs of hits Hodo (1,2) vs STS 1 (N,P); t<N1,P1> - t<H1,H2> [ns]; Counts";
   fhDtAllPairsHodoS1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhDtAllPairsHodoS2";
   title =  "Time diff for pairs of hits Hodo (1,2) vs STS 2 (N,P); t<N2,P2> - t<H1,H2> [ns]; Counts";
   fhDtAllPairsHodoS2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhDtBestPairsH1";
   title =  "Time diff for best pair of hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhDtBestPairsH1 = new TH1I(sHistName, title, uNbBinDtHodo, fdCoincMinHodo, fdCoincMaxHodo );

   sHistName = "fhDtBestPairsH2";
   title =  "Time diff for best pair of hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhDtBestPairsH2 = new TH1I(sHistName, title, uNbBinDtHodo, fdCoincMinHodo, fdCoincMaxHodo );

   sHistName = "fhDtBestPairsS1";
   title =  "Time diff for best pair of hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhDtBestPairsS1 = new TH1I(sHistName, title, uNbBinDtSts1, fdCoincMinSts1, fdCoincMaxSts1 );

   sHistName = "fhDtBestPairsS2";
   title =  "Time diff for best pair of hits Sts 2 N and Sts 1 P; tP2 - tN2 [ns]; Counts";
   fhDtBestPairsS2 = new TH1I(sHistName, title, uNbBinDtSts2, fdCoincMinSts2, fdCoincMaxSts2 );

   sHistName = "fhDtBestPairsHodo";
   title =  "Time diff for best pair of hits Hodo 1 (X,Y) vs Hodo 2 (X,Y); t<X2,Y2> - t<X1,Y1> [ns]; Counts";
   fhDtBestPairsHodo = new TH1I(sHistName, title, uNbBinDtBothHodo, fdCoincMinHodoBoth, fdCoincMaxHodoBoth );

   sHistName = "fhDtBestPairsHodoS1";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 1 (N,P); t<N1,P1> - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS1 = new TH1I(sHistName, title, uNbBinDtHodoS1, fdCoincMinHodoS1, fdCoincMaxHodoS1 );

   sHistName = "fhDtBestPairsHodoS2";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 2 (N,P); t<N2,P2> - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS2 = new TH1I(sHistName, title, uNbBinDtHodoS2, fdCoincMinHodoS2, fdCoincMaxHodoS2 );

   sHistName = "fhDtBestPairsHodoS1S2";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 1 (N,P) if matching STS 2 Hit; t<N1,P1>|S2 - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS1S2 = new TH1I(sHistName, title, uNbBinDtHodoS1, fdCoincMinHodoS1, fdCoincMaxHodoS1 );

   sHistName = "fhDtBestPairsHodoS2S1";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 2 (N,P) if matching STS 1 Hit; t<N2,P2>|S1 - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS2S1 = new TH1I(sHistName, title, uNbBinDtHodoS2, fdCoincMinHodoS2, fdCoincMaxHodoS2 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhMapBestPairsH1";
   title = "Position for best pair of hits in Hodo 1 X and Hodo 1 Y; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsH1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsH2";
   title = "Position for best pair of hits in Hodo 2 X and Hodo 2 Y; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsH2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsS1";
   title = "Position for best pair of hits in Sts 1 axis X and Y; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsS1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsS2";
   title = "Position for best pair of hits in Sts 1 axis X and Y; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsS2 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodo_H1";
   title = "Position in Hodo 1 for best pair of hits in Hodo 1 and 2; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodo_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodo_H2";
   title = "Position in Hodo 2 for best pair of hits in Hodo 1 and 2; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodo_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodo_ProjS1";
   title = "Position in Proj in STS 1 for best pair of Hodo Hits; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodo_ProjS1 = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodo_ProjS2";
   title = "Position in Proj in STS 2 for best pair of Hodo Hits; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodo_ProjS2 = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1_H1";
   title = "Position in Hodo 1 for best pair of hits in STS 1 and Hodo; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1_H2";
   title = "Position in Hodo 2 for best pair of hits in STS 1 and Hodo; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS1_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1_S1";
   title = "Position in STS 1 for best pair of hits in Sts 1 and Hodo; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1_S1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1_Proj";
   title = "Position in Proj in STS 1 for best pair of hits in Sts 1 and Hodo; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS1_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2_H1";
   title = "Position in Hodo 1 for best pair of hits in STS 2 and Hodo; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS2_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2_H2";
   title = "Position in Hodo 2 for best pair of hits in STS 2 and Hodo; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2_S2";
   title = "Position in STS 2 for best pair of hits in Sts 2 and Hodo; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2_S2 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2_Proj";
   title = "Position in Proj in STS 2 for best pair of hits in Sts 2 and Hodo; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS2_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2_H1";
   title = "Position in Hodo 1 for best pair of hits in Sts 1 and Hodo is STS2 match; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1S2_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2_H2";
   title = "Position in Hodo 2 for best pair of hits in Sts 1 and Hodo is STS2 match; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS1S2_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2_S1";
   title = "Position in STS 1 for best pair of hits in Sts 1 and Hodo is STS2 match; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1S2_S1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2_Proj";
   title = "Position in Proj in STS 1 for best pair of hits in Sts 1 and Hodo is STS2 match; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS1S2_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1_H1";
   title = "Position in Hodo 1 for best pair of hits in Sts 2 and Hodo id STS1 match; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS2S1_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1_H2";
   title = "Position in Hodo 2 for best pair of hits in Sts 2 and Hodo id STS1 match; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2S1_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1_S2";
   title = "Position in STS 2 for best pair of hits in Sts 2 and Hodo id STS1 match; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2S1_S2 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1_Proj";
   title = "Position in Proj in STS 2 for best pair of hits in Sts 2 and Hodo id STS1 match; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS2S1_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhResidualsBestPairsHodoS1";
   title = "Residuals in STS 1 for best pair of hits in Sts 1 and Hodo; X1- X1p [mm]; Y1 - Y1p [mm]; Hits []";
   fhResidualsBestPairsHodoS1 = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );

   sHistName = "fhResidualsBestPairsHodoS2";
   title = "Residuals in STS 2 for best pair of hits in Sts 2 and Hodo; X2 - X2p [mm]; Y2 - Y2p [mm]; Hits []";
   fhResidualsBestPairsHodoS2 = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );

   sHistName = "fhResidualsBestPairsHodoS1S2";
   title = "Residuals in STS 1 for best pair of hits in Sts 1 and Hodo is STS2 match; X1- X1p [mm]; Y1 - Y1p [mm]; Hits []";
   fhResidualsBestPairsHodoS1S2 = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );

   sHistName = "fhResidualsBestPairsHodoS2S1";
   title = "Residuals in STS 2 for best pair of hits in Sts 2 and Hodo id STS1 match; X2 - X2p [mm]; Y2 - Y2p [mm]; Hits []";
   fhResidualsBestPairsHodoS2S1 = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhAdcRawBestPairsHodoS1N";
   title = "Raw Adc distribution per channel, S1 N beat pair Hodo; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS1N =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS1P";
   title = "Raw Adc distribution per channel, S1 P beat pair Hodo; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS1P =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS2N";
   title = "Raw Adc distribution per channel, S2 N beat pair Hodo; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS2N =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS2P";
   title = "Raw Adc distribution per channel, S2 P beat pair Hodo; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS2P =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS1S2N";
   title = "Raw Adc distribution per channel, S1 N beat pair Hodo, if S2 hit OK; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS1S2N =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS1S2P";
   title = "Raw Adc distribution per channel, S1 P beat pair Hodo, if S2 hit OK; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS1S2P =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS2S1N";
   title = "Raw Adc distribution per channel, S2 N beat pair Hodo, if S1 hit OK; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS2S1N =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );

   sHistName = "fhAdcRawBestPairsHodoS2S1P";
   title = "Raw Adc distribution per channel, S2 P beat pair Hodo, if S1 hit OK; Channel []; Adc []; Hits []";
   fhAdcRawBestPairsHodoS2S1P =  new TH2I(sHistName, title,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                              stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );


///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhDtBestPairsHodoS1GeoCut";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 1 (N,P), w/ GeoCut; t<N1,P1> - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS1GeoCut = new TH1I(sHistName, title, uNbBinDtHodoS1, fdCoincMinHodoS1, fdCoincMaxHodoS1 );

   sHistName = "fhDtBestPairsHodoS2GeoCut";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 2 (N,P), w/ GeoCut; t<N2,P2> - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS2GeoCut = new TH1I(sHistName, title, uNbBinDtHodoS2, fdCoincMinHodoS2, fdCoincMaxHodoS2 );

   sHistName = "fhDtBestPairsHodoS1S2GeoCut";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 1 (N,P) if matching STS 2 Hit, w/ GeoCut; t<N1,P1>|S2 - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS1S2GeoCut = new TH1I(sHistName, title, uNbBinDtHodoS1, fdCoincMinHodoS1, fdCoincMaxHodoS1 );

   sHistName = "fhDtBestPairsHodoS2S1GeoCut";
   title =  "Time diff for best pair of hits Hodo (1,2) vs STS 2 (N,P) if matching STS 1 Hit, w/ GeoCut; t<N2,P2>|S1 - t<H1,H2> [ns]; Counts";
   fhDtBestPairsHodoS2S1GeoCut = new TH1I(sHistName, title, uNbBinDtHodoS2, fdCoincMinHodoS2, fdCoincMaxHodoS2 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhMapBestPairsHodoGeoCutS1_H1";
   title = "Position in Hodo 1 for best pair of hits in Hodo 1 and 2, if proj inside S1 area; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoGeoCutS1_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoGeoCutS1_H2";
   title = "Position in Hodo 2 for best pair of hits in Hodo 1 and 2, if proj inside S1 area; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoGeoCutS1_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoGeoCutS1_Proj";
   title = "Position in Proj in STS 1 for best pair of hits in Hodo 1 and 2, if proj inside S1 area; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoGeoCutS1_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoGeoCutS2_H1";
   title = "Position in Hodo 1 for best pair of hits in Hodo 1 and 2, if proj inside S2 area; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoGeoCutS2_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoGeoCutS2_H2";
   title = "Position in Hodo 2 for best pair of hits in Hodo 1 and 2, if proj inside S2 area; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoGeoCutS2_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoGeoCutS2_Proj";
   title = "Position in Proj in STS 2 for best pair of hits in Hodo 1 and 2, if proj inside S2 area; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoGeoCutS2_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1GeoCut_H1";
   title = "Position in Hodo 1 for best pair of hits in STS 1 and Hodo, w/ GeoCut; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1GeoCut_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1GeoCut_H2";
   title = "Position in Hodo 2 for best pair of hits in STS 1 and Hodo, w/ GeoCut; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS1GeoCut_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1GeoCut_S1";
   title = "Position in STS 1 for best pair of hits in Sts 1 and Hodo, w/ GeoCut; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1GeoCut_S1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1GeoCut_Proj";
   title = "Position in Proj in STS 1 for best pair of hits in Sts 1 and Hodo, w/ GeoCut; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS1GeoCut_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2GeoCut_H1";
   title = "Position in Hodo 1 for best pair of hits in STS 2 and Hodo, w/ GeoCut; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS2GeoCut_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2GeoCut_H2";
   title = "Position in Hodo 2 for best pair of hits in STS 2 and Hodo, w/ GeoCut; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2GeoCut_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2GeoCut_S2";
   title = "Position in STS 2 for best pair of hits in Sts 2 and Hodo, w/ GeoCut; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2GeoCut_S2 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2GeoCut_Proj";
   title = "Position in Proj in STS 2 for best pair of hits in Sts 2 and Hodo, w/ GeoCut; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS2GeoCut_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2GeoCut_H1";
   title = "Position in Hodo 1 for best pair of hits in Sts 1 and Hodo is STS2 match, w/ GeoCut; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1S2GeoCut_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2GeoCut_H2";
   title = "Position in Hodo 2 for best pair of hits in Sts 1 and Hodo is STS2 match, w/ GeoCut; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS1S2GeoCut_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2GeoCut_S1";
   title = "Position in STS 1 for best pair of hits in Sts 1 and Hodo is STS2 match, w/ GeoCut; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS1S2GeoCut_S1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS1S2GeoCut_Proj";
   title = "Position in Proj in STS 1 for best pair of hits in Sts 1 and Hodo is STS2 match, w/ GeoCut; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS1S2GeoCut_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1GeoCut_H1";
   title = "Position in Hodo 1 for best pair of hits in Sts 2 and Hodo id STS1 match, w/ GeoCut; X1 [mm]; Y1 [mm]; Hits []";
   fhMapBestPairsHodoS2S1GeoCut_H1 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1GeoCut_H2";
   title = "Position in Hodo 2 for best pair of hits in Sts 2 and Hodo id STS1 match, w/ GeoCut; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2S1GeoCut_H2 = new TH2I( sHistName, title,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0,
                                  kiNbFibers, -kdHodoSzXY / 2.0, kdHodoSzXY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1GeoCut_S2";
   title = "Position in STS 2 for best pair of hits in Sts 2 and Hodo id STS1 match, w/ GeoCut; X2 [mm]; Y2 [mm]; Hits []";
   fhMapBestPairsHodoS2S1GeoCut_S2 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   sHistName = "fhMapBestPairsHodoS2S1GeoCut_Proj";
   title = "Position in Proj in STS 2 for best pair of hits in Sts 2 and Hodo id STS1 match, w/ GeoCut; Xp [mm]; Yp [mm]; Hits []";
   fhMapBestPairsHodoS2S1GeoCut_Proj = new TH2I( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhResidualsBestPairsHodoS1GeoCut";
   title = "Residuals in STS 1 for best pair of hits in Sts 1 and Hodo, w/ GeoCut; X1- X1p [mm]; Y1 - Y1p [mm]; Hits []";
   fhResidualsBestPairsHodoS1GeoCut = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );

   sHistName = "fhResidualsBestPairsHodoS2GeoCut";
   title = "Residuals in STS 2 for best pair of hits in Sts 2 and Hodo, w/ GeoCut; X2 - X2p [mm]; Y2 - Y2p [mm]; Hits []";
   fhResidualsBestPairsHodoS2GeoCut = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );

   sHistName = "fhResidualsBestPairsHodoS1S2GeoCut";
   title = "Residuals in STS 1 for best pair of hits in Sts 1 and Hodo is STS2 match, w/ GeoCut; X1- X1p [mm]; Y1 - Y1p [mm]; Hits []";
   fhResidualsBestPairsHodoS1S2GeoCut = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );

   sHistName = "fhResidualsBestPairsHodoS2S1GeoCut";
   title = "Residuals in STS 2 for best pair of hits in Sts 2 and Hodo id STS1 match, w/ GeoCut; X2 - X2p [mm]; Y2 - Y2p [mm]; Hits []";
   fhResidualsBestPairsHodoS2S1GeoCut = new TH2I( sHistName, title,
                                  iNbBinsX * 2, -dMapSizeMmX * 2 / 2.0, dMapSizeMmX * 2 / 2.0,
                                  iNbBinsY * 3, -dMapSizeMmY * 3 / 2.0, dMapSizeMmY * 3 / 2.0 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhEfficiency";
   title = "Efficiency of the STS sensors, w/o other, w/ other, w/ Geo Cut; ; Efficiency []";
   fhEfficiency = new TProfile(sHistName, title, 8, 0, 8 );
   fhEfficiency->GetXaxis()->SetBinLabel( 1, "S1");
   fhEfficiency->GetXaxis()->SetBinLabel( 2, "S2");
   fhEfficiency->GetXaxis()->SetBinLabel( 3, "S1S2");
   fhEfficiency->GetXaxis()->SetBinLabel( 4, "S2S1");
   fhEfficiency->GetXaxis()->SetBinLabel( 5, "S1 GC");
   fhEfficiency->GetXaxis()->SetBinLabel( 6, "S2 GC");
   fhEfficiency->GetXaxis()->SetBinLabel( 7, "S1S2 GC");
   fhEfficiency->GetXaxis()->SetBinLabel( 8, "S2S1 GC");

   sHistName = "fhEfficiencyMapS1";
   title = "STS 1 efficiency vs Position in Proj for Hodo pairs; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS1 = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS2";
   title = "STS 2 efficiency vs Position in Proj for Hodo pairs; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS2 = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS1S2";
   title = "STS 1 efficiency vs Position in Proj for Hodo pairs, if S2 match; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS1S2 = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS2S1";
   title = "STS 2 efficiency vs Position in Proj for Hodo pairs, if S1 match; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS2S1 = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS1GeoCut";
   title = "STS 1 efficiency vs Position in Proj for Hodo pairs, w/ GeoCut; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS1GeoCut = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS2GeoCut";
   title = "STS 2 efficiency vs Position in Proj for Hodo pairs, w/ GeoCut; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS2GeoCut = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS1S2GeoCut";
   title = "STS 1 efficiency vs Position in Proj for Hodo pairs, if S2 match, w/ GeoCut; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS1S2GeoCut = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

   sHistName = "fhEfficiencyMapS2S1GeoCut";
   title = "STS 2 efficiency vs Position in Proj for Hodo pairs, if S1 match, w/ GeoCut; Xp [mm]; Yp [mm]; Efficiency []";
   fhEfficiencyMapS2S1GeoCut = new TProfile2D( sHistName, title,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0,
                                  iMapProjNbBinsXY, -dMapProjSizeMmXY / 2.0, dMapProjSizeMmXY / 2.0 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhTestBoundariesSensor1";
   title = "Map of sensor 1 boundaries, 10 um steps; Xp [mm]; Yp [mm]; Counts []";
   fhTestBoundariesSensor1 = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestBoundariesSensor2";
   title = "Map of sensor 2boundaries, 10 um steps; Xp [mm]; Yp [mm]; Hits []";
   fhTestBoundariesSensor2 = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestBoundariesSensor1NoDead";
   title = "Map of sensor 1 boundaries Dead out, 10 um steps; Xp [mm]; Yp [mm]; Hits []";
   fhTestBoundariesSensor1NoDead = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestBoundariesSensor2NoDead";
   title = "Map of sensor 2 boundaries dead out, 10 um steps; Xp [mm]; Yp [mm]; Hits []";
   fhTestBoundariesSensor2NoDead = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestChanMatchSensor1";
   title = "Map of sensor 1 N-P channels matchs; Chan N [mm]; Chan P [mm]; Hits []";
   fhTestChanMatchSensor1 = new TH2I( sHistName, title, 128, -0.5, 127.5, 128, -0.5, 127.5 );

   sHistName = "fhTestChanMatchSensor2";
   title = "Map of sensor 2 N-P channels matchs; Chan N [mm]; Chan P [mm]; Hits []";
   fhTestChanMatchSensor2 = new TH2I( sHistName, title, 128, -0.5, 127.5, 128, -0.5, 127.5 );

   sHistName = "fhTestChanMapSensor1";
   title = "Map of sensor 1 N-P channels pairs points; Xp [mm]; Yp [mm]; Hits []";
   fhTestChanMapSensor1 = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestChanMapSensor2";
   title = "Map of sensor 2 N-P channels pairs points; Xp [mm]; Yp [mm]; Hits []";
   fhTestChanMapSensor2 = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestChanMapPhysSensor1";
   title = "Map of sensor 1 N-P channels Physical pairs points; Xp [mm]; Yp [mm]; Hits []";
   fhTestChanMapPhysSensor1 = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   sHistName = "fhTestChanMapPhysSensor2";
   title = "Map of sensor 2 N-P channels Physical pairs points; Xp [mm]; Yp [mm]; Hits []";
   fhTestChanMapPhysSensor2 = new TH2I( sHistName, title, 240, -60, 60, 240, -60, 60 );

   for( Double_t dTestX = -60; dTestX < 60; dTestX += 0.01 )
      for( Double_t dTestY = -60; dTestY < 60; dTestY += 0.01 )
      {
         if( CheckPointInsideSensor1( dTestX, dTestY ) )
            fhTestBoundariesSensor1->Fill( dTestX, dTestY );
         if( CheckPointInsideSensor2( dTestX, dTestY ) )
            fhTestBoundariesSensor2->Fill( dTestX, dTestY );

         if( CheckPointInsideSensor1NoDead( dTestX, dTestY ) )
            fhTestBoundariesSensor1NoDead->Fill( dTestX, dTestY );
         if( CheckPointInsideSensor2NoDead( dTestX, dTestY ) )
            fhTestBoundariesSensor2NoDead->Fill( dTestX, dTestY );
      } // Loop on X,Y

   for( Int_t iTestN = 0; iTestN < 128; ++iTestN )
      for( Int_t iTestP = 0; iTestP < 128; ++iTestP )
      {

         Double_t dX, dY;
         ComputeCoordinatesSensor1( iTestN, iTestP, dX, dY );
         fhTestChanMapSensor1->Fill( dX, dY );
         if( CheckPhysPairSensor1( iTestN, iTestP ) )
         {
            fhTestChanMatchSensor1->Fill( iTestN, iTestP );
            fhTestChanMapPhysSensor1->Fill( dX, dY );
         }

         ComputeCoordinatesSensor2( iTestN, iTestP, dX, dY );
         fhTestChanMapSensor2->Fill( dX, dY );
         if( CheckPhysPairSensor2( iTestN, iTestP ) )
         {
            fhTestChanMatchSensor2->Fill( iTestN, iTestP );
            fhTestChanMapPhysSensor2->Fill( dX, dY );
         }
      } // Loop on N,P

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhTestMapHodoS1N";
   title = "Position in Proj in STS 1 for Hodo pairs matching a hit in STS 1 N; Xp [mm]; Yp [mm]; Fraction of hits []";
   fhTestMapHodoS1N = new TProfile2D( sHistName, title,
                                  iNbBinsX / 2, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY / 2, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );
   sHistName = "fhTestMapHodoS1P";
   title = "Position in Proj in STS 1 for Hodo pairs matching a hit in STS 1 P; Xp [mm]; Yp [mm]; Fraction of hits []";
   fhTestMapHodoS1P = new TProfile2D( sHistName, title,
                                  iNbBinsX / 2, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY / 2, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );
   sHistName = "fhTestMapHodoS2N";
   title = "Position in Proj in STS 2 for Hodo pairs matching a hit in STS 2 N; Xp [mm]; Yp [mm]; Fraction of hits []";
   fhTestMapHodoS2N = new TProfile2D( sHistName, title,
                                  iNbBinsX / 2, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY / 2, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );
   sHistName = "fhTestMapHodoS2P";
   title = "Position in Proj in STS 2 for Hodo pairs matching a hit in STS 2 P; Xp [mm]; Yp [mm]; Fraction of hits []";
   fhTestMapHodoS2P = new TProfile2D( sHistName, title,
                                  iNbBinsX / 2, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY / 2, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
   sHistName = "fhDtNeighborChansS1N";
   title = "Dt to hits on neighbors, per channel, in STS 1 N; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansS1N = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhDtNeighborChansS1P";
   title = "Dt to hits on neighbors, per channel, in STS 1 P; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansS1P = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhDtNeighborChansS2N";
   title = "Dt to hits on neighbors, per channel, in STS 2 N; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansS2N = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhDtNeighborChansS2P";
   title = "Dt to hits on neighbors, per channel, in STS 2 P; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansS2P = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   // Adc Distribution
   sHistName = "fhClusterAdcVsSizeS1N";
   title = "Adc distribution per cluster vs cluster size, STS 1 N; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS1N = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterAdcVsSizeS1P";
   title = "Adc distribution per cluster vs cluster size, STS 1 P; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS1P = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterAdcVsSizeS2N";
   title = "Adc distribution per cluster vs cluster size, STS 2 N; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS2N = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterAdcVsSizeS2P";
   title = "Adc distribution per cluster vs cluster size, STS 2 P; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS2P = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhClusterAdcVsSizeS1N_MatchS1";
   title = "Adc distribution per cluster vs cluster size, STS 1 N, if match with P side; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS1N_MatchS1 = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterAdcVsSizeS1P_MatchS1";
   title = "Adc distribution per cluster vs cluster size, STS 1 P, if match with N side; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS1P_MatchS1 = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterAdcVsSizeS2N_MatchS2";
   title = "Adc distribution per cluster vs cluster size, STS 2 N, if match with P side; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS2N_MatchS2 = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterAdcVsSizeS2P_MatchS2";
   title = "Adc distribution per cluster vs cluster size, STS 2 P, if match with N side; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeS2P_MatchS2 = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhClusterS1N_CenterNbMatchS1P";
   title = "Nb match P side vs center strip, STS 1 N; Center [ch];  Nb Match [cluster]; Cnts []";
   fhClusterS1N_CenterNbMatchS1P = new TH2I(sHistName, title,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterS1P_CenterNbMatchS1N";
   title = "Nb match N side vs center strip, STS 1 P; Center [ch];  Nb Match [cluster]; Cnts []";
   fhClusterS1P_CenterNbMatchS1N = new TH2I(sHistName, title,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterS2N_CenterNbMatchS2P";
   title = "Nb match P side vs center strip, STS 2 N; Center [ch];  Nb Match [cluster]; Cnts []";
   fhClusterS2N_CenterNbMatchS2P = new TH2I(sHistName, title,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterS2P_CenterNbMatchS2N";
   title = "Nb match N side vs center strip, STS 2 P; Center [ch];  Nb Match [cluster]; Cnts []";
   fhClusterS2P_CenterNbMatchS2N = new TH2I(sHistName, title,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhClusterS1N_AdcNbMatchS1P";
   title = "Adc distribution per cluster vs Nb matches with P cluster, STS 1 N; Adc [];  Nb Match [clusters]; Cnts []";
   fhClusterS1N_AdcNbMatchS1P = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterS1P_AdcNbMatchS1N";
   title = "Adc distribution per cluster vs Nb matches with N cluster, STS 1 P; Adc [];  Nb Match [clusters]; Cnts []";
   fhClusterS1P_AdcNbMatchS1N = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterS2N_AdcNbMatchS2P";
   title = "Adc distribution per cluster vs Nb matches with P cluster, STS 2 N; Adc [];  Nb Match [clusters]; Cnts []";
   fhClusterS2N_AdcNbMatchS2P = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   sHistName = "fhClusterS2P_AdcNbMatchS2N";
   title = "Adc distribution per cluster vs Nb matches with N cluster, STS 2 P; Adc [];  Nb Match [clusters]; Cnts []";
   fhClusterS2P_AdcNbMatchS2N = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );


   sHistName = "fhDtNeighborChansH1X";
   title = "Dt to hits on neighbors, per channel, in Hodo 1 X; Dt( ch + 1 - ch) [ns]; Strip []; Counts []";
   fhDtNeighborChansH1X = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhDtNeighborChansH1Y";
   title = "Dt to hits on neighbors, per channel, in Hodo 1 Y; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansH1Y = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhDtNeighborChansH2X";
   title = "Dt to hits on neighbors, per channel, in Hodo 2 X; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansH2X = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhDtNeighborChansH2Y";
   title = "Dt to hits on neighbors, per channel, in Hodo 2 Y; Dt( ch + 1 - ch) [ns]; Channel []; Counts []";
   fhDtNeighborChansH2Y = new TH2I( sHistName, title,
                                  101, -50.5 * stsxyter::kdClockCycleNs, 50.5 * stsxyter::kdClockCycleNs,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhClusterAdcVsSizeH1X";
   title = "Adc distribution per cluster vs cluster size, Hodo 1 X; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeH1X = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   sHistName = "fhClusterAdcVsSizeH1Y";
   title = "Adc distribution per cluster vs cluster size, Hodo 1 Y; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeH1Y = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   sHistName = "fhClusterAdcVsSizeH2X";
   title = "Adc distribution per cluster vs cluster size, Hodo 2 X; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeH2X = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   sHistName = "fhClusterAdcVsSizeH2Y";
   title = "Adc distribution per cluster vs cluster size, Hodo 2 Y; Adc [];  Cluster Sz [Ch]; Cnts []";
   fhClusterAdcVsSizeH2Y = new TH2I(sHistName, title,
                               3*stsxyter::kuHitNbAdcBins, -0.5, 3*stsxyter::kuHitNbAdcBins -0.5,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhClusterPosition_MatchH1";
   title = "Cluste position, Hodo 1 XY match; X [];  Y []; Cnts []";
   fhClusterPosition_MatchH1 = new TH2I(sHistName, title,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   sHistName = "fhClusterPosition_MatchH2";
   title = "Cluste position, Hodo 2 XY match; X [];  Y []; Cnts []";
   fhClusterPosition_MatchH2 = new TH2I(sHistName, title,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                               fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

/*
   // Distribution of the TS_MSB per StsXyter
   sHistName = "hHodoFebTsMsb";
   title = "Raw Timestamp Msb distribution per StsXyter; Ts MSB []; StsXyter []; Hits []";
   fhHodoFebTsMsb = new TH2I( sHistName, title, stsxyter::kuTsMsbNbTsBins, -0.5,   stsxyter::kuTsMsbNbTsBins - 0.5,
                                                fuNbStsXyters, -0.5, fuNbStsXyters - 0.5 );
   if( server ) server->Register("/StsRaw", fhHodoFebTsMsb );
*/
   // Miscroslice properties histos
   for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )
   {
      fhMsSz[ component ] = NULL;
      fhMsSzTime[ component ] = NULL;
   } // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )

   // Online histo browser commands
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
   if( server )
   {
      server->Register("/Test", fhTestBoundariesSensor1 );
      server->Register("/Test", fhTestBoundariesSensor2 );
      server->Register("/Test", fhTestBoundariesSensor1NoDead );
      server->Register("/Test", fhTestBoundariesSensor2NoDead );
      server->Register("/Test", fhTestChanMatchSensor1 );
      server->Register("/Test", fhTestChanMatchSensor2 );
      server->Register("/Test", fhTestChanMapSensor1 );
      server->Register("/Test", fhTestChanMapSensor2 );
      server->Register("/Test", fhTestChanMapPhysSensor1 );
      server->Register("/Test", fhTestChanMapPhysSensor2 );
      server->Register("/Test", fhTestMapHodoS1N );
      server->Register("/Test", fhTestMapHodoS1P );
      server->Register("/Test", fhTestMapHodoS2N );
      server->Register("/Test", fhTestMapHodoS2P );

      server->Register("/HodoRaw", fhHodoMessType );
      server->Register("/HodoRaw", fhHodoSysMessType );
      server->Register("/HodoRaw", fhHodoMessTypePerDpb );
      server->Register("/HodoRaw", fhHodoSysMessTypePerDpb );
      server->Register("/HodoRaw", fhHodoMessTypePerElink );
      server->Register("/HodoRaw", fhHodoSysMessTypePerElink );
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {
         server->Register("/HodoRaw", fhHodoChanCntRaw[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanCntRawGood[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanAdcRaw[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanAdcRawProf[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanRawTs[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanMissEvt[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanMissEvtEvo[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoFebMissEvtEvo[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanHitRateEvo[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoFebRateEvo[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanHitRateEvoLong[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoFebRateEvoLong[ uXyterIdx ] );
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      server->Register("/Setup", fhSetupSortedDtX1Y1 );
      server->Register("/Setup", fhSetupSortedDtX2Y2 );
      server->Register("/Setup", fhSetupSortedDtN1P1 );
      server->Register("/Setup", fhSetupSortedDtN2P2 );
      server->Register("/Setup", fhSetupSortedDtX1Y1X2Y2 );
      server->Register("/Setup", fhSetupSortedDtX1Y1X2Y2N1P1 );
      server->Register("/Setup", fhSetupSortedDtX1Y1X2Y2N2P2 );
      server->Register("/Setup", fhSetupSortedDtH1H2S1S2 );
      server->Register("/Setup", fhSetupSortedMapX1Y1 );
      server->Register("/Setup", fhSetupSortedMapX2Y2 );
      server->Register("/Setup", fhSetupSortedMapN1P1 );
      server->Register("/Setup", fhSetupSortedMapN2P2 );
      server->Register("/Setup", fhSetupSortedCntEvoX1Y1 );
      server->Register("/Setup", fhSetupSortedCntEvoX2Y2 );
      server->Register("/Setup", fhSetupSortedCntEvoN1P1 );
      server->Register("/Setup", fhSetupSortedCntEvoN2P2 );

      server->Register("/Setup", fhStsSortedMapS1 );
      if( kTRUE == fbDualStsEna )
      {
         server->Register("/Setup", fhStsSortedMapS2 );
         server->Register("/Setup", fhStsSortedMapS1Coinc );
         server->Register("/Setup", fhStsSortedMapS2Coinc );
      } // if( kTRUE == fbDualStsEna )

      server->Register("/FebDt", fhSetupSortedDtX1 );
      server->Register("/FebDt", fhSetupSortedDtY1 );
      server->Register("/FebDt", fhSetupSortedDtX2 );
      server->Register("/FebDt", fhSetupSortedDtY2 );
      server->Register("/FebDt", fhSetupSortedDtN1 );
      server->Register("/FebDt", fhSetupSortedDtP1 );
      server->Register("/FebDt", fhSetupSortedDtN2 );
      server->Register("/FebDt", fhSetupSortedDtP2 );

      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {
         server->Register("/FebDupli", fhSetupSortedNbSameTsChan[ uXyterIdx ]  );
         server->Register("/FebDupli", fhSetupSortedSameTsAdcChan[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedSameTsAdcDiff[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedRatioSameTsChan[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedNbConsSameTsChan[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedNbConsSameTsAdcChan[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedNbConsSameTsVsTsAdc[ uXyterIdx ] );

         server->Register("/FebDupli", fhSetupSortedAsicRatioSameTsVsFlux[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedAsicRatioSameTsAdcVsFlux[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedAsicRatioSameAdcSameTsVsFlux[ uXyterIdx ] );
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

      server->Register("/FebDupli", fhRatioMsDuplicateQuality  );
      for( UInt_t uXyterIdx = 0; uXyterIdx <= fuNbStsXyters; ++uXyterIdx )
      {
         server->Register("/FebDupli", fhEvoMsDuplicateQuality[ uXyterIdx ] );
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      server->Register("/FebDupli", fhSizeCleanMs  );

      server->Register("/Spill", fhHodoX1SpillEvo );
      server->Register("/Spill", fhHodoY1SpillEvo );
      server->Register("/Spill", fhHodoX2SpillEvo );
      server->Register("/Spill", fhHodoY2SpillEvo );
      server->Register("/Spill", fhHodoX1SpillEvoProf );
      server->Register("/Spill", fhHodoY1SpillEvoProf );
      server->Register("/Spill", fhHodoX2SpillEvoProf );
      server->Register("/Spill", fhHodoY2SpillEvoProf );

      server->Register("/Multi", fhNbY1CoincPerX1 );
      server->Register("/Multi", fhNbY2CoincPerX2 );
      server->Register("/Multi", fhNbP1CoincPerN1 );
      server->Register("/Multi", fhNbP2CoincPerN2 );
      server->Register("/Multi", fhNbH2CoincPerH1 );
      server->Register("/Multi", fhNbS1CoincPerHodo );
      server->Register("/Multi", fhNbS2CoincPerHodo );
      server->Register("/Multi", fhNbS1CoincPerHodoS2 );
      server->Register("/Multi", fhNbS2CoincPerHodoS1 );

      server->Register("/Cuts", fhChanMapAllMatchS1 );
      server->Register("/Cuts", fhChanMapAllMatchS2 );
      server->Register("/Cuts", fhChanMapBestMatchS1 );
      server->Register("/Cuts", fhChanMapBestMatchS2 );

      server->Register("/Cuts", fhDtAllPairsS1 );
      server->Register("/Cuts", fhDtAllPairsS2 );
      server->Register("/Cuts", fhDtAllPairsHodoS1 );
      server->Register("/Cuts", fhDtAllPairsHodoS2 );
      server->Register("/Cuts", fhDtBestPairsH1 );
      server->Register("/Cuts", fhDtBestPairsH2 );
      server->Register("/Cuts", fhDtBestPairsS1 );
      server->Register("/Cuts", fhDtBestPairsS2 );
      server->Register("/Cuts", fhDtBestPairsHodo );
      server->Register("/Cuts", fhDtBestPairsHodoS1 );
      server->Register("/Cuts", fhDtBestPairsHodoS2 );
      server->Register("/Cuts", fhDtBestPairsHodoS1S2 );
      server->Register("/Cuts", fhDtBestPairsHodoS2S1 );
      server->Register("/Cuts", fhMapBestPairsH1 );
      server->Register("/Cuts", fhMapBestPairsH2 );
      server->Register("/Cuts", fhMapBestPairsS1 );
      server->Register("/Cuts", fhMapBestPairsS2 );
      server->Register("/Cuts", fhMapBestPairsHodo_H1 );
      server->Register("/Cuts", fhMapBestPairsHodo_H2 );
      server->Register("/Cuts", fhMapBestPairsHodo_ProjS1 );
      server->Register("/Cuts", fhMapBestPairsHodo_ProjS2 );
      server->Register("/Cuts", fhMapBestPairsHodoS1_H1 );
      server->Register("/Cuts", fhMapBestPairsHodoS1_H2 );
      server->Register("/Cuts", fhMapBestPairsHodoS1_S1 );
      server->Register("/Cuts", fhMapBestPairsHodoS1_Proj );
      server->Register("/Cuts", fhMapBestPairsHodoS2_H1 );
      server->Register("/Cuts", fhMapBestPairsHodoS2_H2 );
      server->Register("/Cuts", fhMapBestPairsHodoS2_S2 );
      server->Register("/Cuts", fhMapBestPairsHodoS2_Proj );
      server->Register("/Cuts", fhMapBestPairsHodoS1S2_H1 );
      server->Register("/Cuts", fhMapBestPairsHodoS1S2_H2 );
      server->Register("/Cuts", fhMapBestPairsHodoS1S2_S1 );
      server->Register("/Cuts", fhMapBestPairsHodoS1S2_Proj );
      server->Register("/Cuts", fhMapBestPairsHodoS2S1_H1 );
      server->Register("/Cuts", fhMapBestPairsHodoS2S1_H2 );
      server->Register("/Cuts", fhMapBestPairsHodoS2S1_S2 );
      server->Register("/Cuts", fhMapBestPairsHodoS2S1_Proj );
      server->Register("/Cuts", fhResidualsBestPairsHodoS1 );
      server->Register("/Cuts", fhResidualsBestPairsHodoS2 );
      server->Register("/Cuts", fhResidualsBestPairsHodoS1S2 );
      server->Register("/Cuts", fhResidualsBestPairsHodoS2S1 );

      server->Register("/Cuts", fhAdcRawBestPairsHodoS1N );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS1P );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS2N );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS2P );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS1S2N );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS1S2P );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS2S1N );
      server->Register("/Cuts", fhAdcRawBestPairsHodoS2S1P );

      server->Register("/GeoCuts", fhDtBestPairsHodoS1GeoCut );
      server->Register("/GeoCuts", fhDtBestPairsHodoS2GeoCut );
      server->Register("/GeoCuts", fhDtBestPairsHodoS1S2GeoCut );
      server->Register("/GeoCuts", fhDtBestPairsHodoS2S1GeoCut );
      server->Register("/GeoCuts", fhMapBestPairsHodoGeoCutS1_H1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoGeoCutS1_H2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoGeoCutS1_Proj );
      server->Register("/GeoCuts", fhMapBestPairsHodoGeoCutS2_H1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoGeoCutS2_H2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoGeoCutS2_Proj );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1GeoCut_H1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1GeoCut_H2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1GeoCut_S1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1GeoCut_Proj );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2GeoCut_H1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2GeoCut_H2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2GeoCut_S2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2GeoCut_Proj );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1S2GeoCut_H1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1S2GeoCut_H2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1S2GeoCut_S1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS1S2GeoCut_Proj );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2S1GeoCut_H1 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2S1GeoCut_H2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2S1GeoCut_S2 );
      server->Register("/GeoCuts", fhMapBestPairsHodoS2S1GeoCut_Proj );
      server->Register("/GeoCuts", fhResidualsBestPairsHodoS1GeoCut );
      server->Register("/GeoCuts", fhResidualsBestPairsHodoS2GeoCut );
      server->Register("/GeoCuts", fhResidualsBestPairsHodoS1S2GeoCut );
      server->Register("/GeoCuts", fhResidualsBestPairsHodoS2S1GeoCut );

      server->Register("/Eff", fhEfficiency );
      server->Register("/Eff", fhEfficiencyMapS1 );
      server->Register("/Eff", fhEfficiencyMapS2 );
      server->Register("/Eff", fhEfficiencyMapS1S2 );
      server->Register("/Eff", fhEfficiencyMapS2S1 );
      server->Register("/Eff", fhEfficiencyMapS1GeoCut );
      server->Register("/Eff", fhEfficiencyMapS2GeoCut );
      server->Register("/Eff", fhEfficiencyMapS1S2GeoCut );
      server->Register("/Eff", fhEfficiencyMapS2S1GeoCut );

      server->Register("/Adc", fhDtNeighborChansS1N );
      server->Register("/Adc", fhDtNeighborChansS1P );
      server->Register("/Adc", fhDtNeighborChansS2N );
      server->Register("/Adc", fhDtNeighborChansS2P );
      server->Register("/Adc", fhClusterAdcVsSizeS1N );
      server->Register("/Adc", fhClusterAdcVsSizeS1P );
      server->Register("/Adc", fhClusterAdcVsSizeS2N );
      server->Register("/Adc", fhClusterAdcVsSizeS2P );
      server->Register("/Adc", fhClusterAdcVsSizeS1N_MatchS1 );
      server->Register("/Adc", fhClusterAdcVsSizeS1P_MatchS1 );
      server->Register("/Adc", fhClusterAdcVsSizeS2N_MatchS2 );
      server->Register("/Adc", fhClusterAdcVsSizeS2P_MatchS2 );

      server->Register("/Adc", fhClusterS1N_CenterNbMatchS1P );
      server->Register("/Adc", fhClusterS1P_CenterNbMatchS1N );
      server->Register("/Adc", fhClusterS2N_CenterNbMatchS2P );
      server->Register("/Adc", fhClusterS2P_CenterNbMatchS2N );
      server->Register("/Adc", fhClusterS1N_AdcNbMatchS1P );
      server->Register("/Adc", fhClusterS1P_AdcNbMatchS1N );
      server->Register("/Adc", fhClusterS2N_AdcNbMatchS2P );
      server->Register("/Adc", fhClusterS2P_AdcNbMatchS2N );

      server->Register("/Adc", fhDtNeighborChansH1X );
      server->Register("/Adc", fhDtNeighborChansH1Y );
      server->Register("/Adc", fhDtNeighborChansH2X );
      server->Register("/Adc", fhDtNeighborChansH2Y );
      server->Register("/Adc", fhClusterAdcVsSizeH1X );
      server->Register("/Adc", fhClusterAdcVsSizeH1Y );
      server->Register("/Adc", fhClusterAdcVsSizeH2X );
      server->Register("/Adc", fhClusterAdcVsSizeH2Y );
      server->Register("/Adc", fhClusterPosition_MatchH1 );
      server->Register("/Adc", fhClusterPosition_MatchH2 );

      server->RegisterCommand("/Reset_All_Hodo", "bCosy2018ResetEfficiency=kTRUE");
      server->RegisterCommand("/Write_All_Hodo", "bCosy2018WriteEfficiency=kTRUE");

      server->Restrict("/Reset_All_Hodo", "allow=admin");
      server->Restrict("/Write_All_Hodo", "allow=admin");
   } // if( server )

   /** Create summary Canvases for CERN 2017 **/
   Double_t w = 10;
   Double_t h = 10;

      // Summary per StsXyter
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      TCanvas* cStsSumm = new TCanvas( Form("cStsSum_%03u", uXyterIdx ),
                                       Form("Summary plots for StsXyter %03u", uXyterIdx ),
                                       w, h);
      cStsSumm->Divide( 2, 2 );

      cStsSumm->cd(1);
      gPad->SetLogy();
      fhHodoChanCntRaw[ uXyterIdx ]->Draw();

      cStsSumm->cd(2);
      gPad->SetLogz();
      fhHodoChanAdcRaw[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(3);
      gPad->SetLogz();
      fhHodoChanHitRateEvo[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(4);
//      gPad->SetLogy();
      fhHodoChanAdcRawProf[ uXyterIdx ]->Draw();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

//====================================================================//

//====================================================================//
   TCanvas* cSetupSortedDt = new TCanvas( "cSetupSortedDt",
                                    "Hodoscopes coincidence maps",
                                    w, h);
   cSetupSortedDt->Divide( 3, 3 );

   cSetupSortedDt->cd(1);
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtX1Y1->Draw( "" );

   cSetupSortedDt->cd(2);
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtX2Y2->Draw( "" );

   cSetupSortedDt->cd(3);
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtN1P1->Draw( "" );

   cSetupSortedDt->cd(4 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtX1Y1X2Y2->Draw( "" );

   cSetupSortedDt->cd(5 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtN1P1N2P2->Draw( "" );

   cSetupSortedDt->cd(6 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtX1Y1X2Y2N1P1->Draw( "" );

   cSetupSortedDt->cd(7 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtX1Y1X2Y2N2P2->Draw( "" );

   cSetupSortedDt->cd(8 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtH1H2S1S2->Draw( "" );

   if( kTRUE == fbDualStsEna )
   {
      cSetupSortedDt->cd(4);
      gPad->SetGridx();
      gPad->SetLogy();
      fhSetupSortedDtN2P2->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cSetupSortedMaps = new TCanvas( "cSetupSortedMaps",
                                    "Hodoscopes coincidence maps",
                                    w, h);

   if( kTRUE == fbDualStsEna )
      cSetupSortedMaps->Divide( 2, 2 );
      else cSetupSortedMaps->Divide( 3, 1 );

   cSetupSortedMaps->cd(1);
   gPad->SetLogz();
   fhSetupSortedMapX1Y1->Draw( "colz" );

   cSetupSortedMaps->cd(2);
   gPad->SetLogz();
   fhSetupSortedMapX2Y2->Draw( "colz" );

   cSetupSortedMaps->cd(3);
   gPad->SetLogz();
   fhSetupSortedMapN1P1->Draw( "colz" );

   if( kTRUE == fbDualStsEna )
   {
      cSetupSortedMaps->cd(4);
      gPad->SetLogz();
      fhSetupSortedMapN2P2->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cSetupSortedCoincEvo = new TCanvas( "cSetupSortedCoincEvo",
                                    "Hodoscopes coincidence rate evolution",
                                    w, h);
   cSetupSortedCoincEvo->Divide( 2, 2 );

   cSetupSortedCoincEvo->cd(1);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSetupSortedCntEvoX1Y1->Draw( "" );

   cSetupSortedCoincEvo->cd(2);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSetupSortedCntEvoX2Y2->Draw( "" );

   cSetupSortedCoincEvo->cd(3);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSetupSortedCntEvoN1P1->Draw( "" );

   cSetupSortedCoincEvo->cd(4);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSetupSortedCntEvoN2P2->Draw( "" );
//====================================================================//

//====================================================================//
   TCanvas* cFebDt = new TCanvas( "cFebDt",
                                    "Hodoscopes coincidence quality",
                                    w, h);

   if( kTRUE == fbDualStsEna )
      cFebDt->Divide( 2, 4 );
      else cFebDt->Divide( 2, 3 );

   cFebDt->cd(1);
   gPad->SetLogy();
   fhSetupSortedDtX1->Draw( "" );

   cFebDt->cd(2);
   gPad->SetLogy();
   fhSetupSortedDtY1->Draw( "" );

   cFebDt->cd(3);
   gPad->SetLogy();
   fhSetupSortedDtX2->Draw( "" );

   cFebDt->cd(4);
   gPad->SetLogy();
   fhSetupSortedDtY2->Draw( "" );

   cFebDt->cd(5);
   gPad->SetLogy();
   fhSetupSortedDtN1->Draw( "" );

   cFebDt->cd(6);
   gPad->SetLogy();
   fhSetupSortedDtP1->Draw( "" );

   if( kTRUE == fbDualStsEna )
   {
      cFebDt->cd(7);
      gPad->SetLogy();
      fhSetupSortedDtN2->Draw( "" );

      cFebDt->cd(8);
      gPad->SetLogy();
      fhSetupSortedDtP2->Draw( "" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cFebSameTs = new TCanvas( "cFebSameTs",
                                    "Hodoscopes coincidence quality",
                                    w, h);

   cFebSameTs->Divide( 2, fuNbStsXyters/2 + fuNbStsXyters%2 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebSameTs->cd(1 + uXyterIdx );
      gPad->SetLogz();
      fhSetupSortedNbSameTsChan[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebSameTsAdc = new TCanvas( "cFebSameTsAdc",
                                    "Hodoscopes coincidence quality",
                                    w, h);

   cFebSameTsAdc->Divide( 2, fuNbStsXyters/2 + fuNbStsXyters%2 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebSameTsAdc->cd(1 + uXyterIdx );
      gPad->SetLogz();
      fhSetupSortedSameTsAdcChan[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebDupliVsChan = new TCanvas( "cFebDupliVsChan",
                                    "Hodoscopes coincidence quality",
                                    w, h);

   cFebDupliVsChan->Divide( fuNbStsXyters, 3 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebDupliVsChan->cd( 1 + uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhSetupSortedNbConsSameTsChan[ uXyterIdx ]->Draw( "colz" );

      cFebDupliVsChan->cd( 1 + fuNbStsXyters +uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhSetupSortedNbConsSameTsAdcChan[ uXyterIdx ]->Draw( "colz" );

      cFebDupliVsChan->cd( 1 + 2*fuNbStsXyters + uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhSetupSortedNbConsSameTsVsTsAdc[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebDupliVsRate = new TCanvas( "cFebDupliVsRate",
                                    "Hodoscopes coincidence quality",
                                    w, h);

   cFebDupliVsRate->Divide( fuNbStsXyters, 3 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebDupliVsRate->cd( 1 + uXyterIdx );
//      gPad->SetGridx();
//      gPad->SetGridy();
      gPad->SetLogx();
      gPad->SetLogz();
      fhSetupSortedAsicRatioSameTsVsFlux[ uXyterIdx ]->Draw( "colz" );

      cFebDupliVsRate->cd( 1 + fuNbStsXyters + uXyterIdx );
//      gPad->SetGridx();
//      gPad->SetGridy();
      gPad->SetLogx();
      gPad->SetLogz();
      fhSetupSortedAsicRatioSameTsAdcVsFlux[ uXyterIdx ]->Draw( "colz" );

      cFebDupliVsRate->cd( 1 + 2*fuNbStsXyters + uXyterIdx );
//      gPad->SetGridx();
//      gPad->SetGridy();
      gPad->SetLogx();
      gPad->SetLogz();
      fhSetupSortedAsicRatioSameAdcSameTsVsFlux[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cStsMapXY = new TCanvas( "cStsMapXY",
                                    "Sts XY maps",
                                    w, h);
   if( kTRUE == fbDualStsEna )
   {
      cStsMapXY->Divide( 2, 2 );
   } // if( kTRUE == fbDualStsEna )

   cStsMapXY->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhStsSortedMapS1->Draw( "colz" );

   if( kTRUE == fbDualStsEna )
   {
      cStsMapXY->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedMapS2->Draw( "colz" );

      cStsMapXY->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedMapS1Coinc->Draw( "colz" );

      cStsMapXY->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedMapS2Coinc->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cStsCuts = new TCanvas( "cStsCuts",
                                    "Sts cuts maps",
                                    w, h);

   cStsCuts->Divide( 6, 4 );

   // Single Det pairs
   cStsCuts->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodo_H1->Draw( "colz" );

   cStsCuts->cd(7);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodo_H2->Draw( "colz" );

   cStsCuts->cd(13);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsS1->Draw( "colz" );

   cStsCuts->cd(19);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsS2->Draw( "colz" );

   // Hodo 1 maps with cuts
   cStsCuts->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1_H1->Draw( "colz" );

   cStsCuts->cd(8);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2_H1->Draw( "colz" );

   cStsCuts->cd(14);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1S2_H1->Draw( "colz" );

   cStsCuts->cd(20);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2S1_H1->Draw( "colz" );

   // Hodo 2 maps with cuts
   cStsCuts->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1_H2->Draw( "colz" );

   cStsCuts->cd(9);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2_H2->Draw( "colz" );

   cStsCuts->cd(15);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1S2_H2->Draw( "colz" );

   cStsCuts->cd(21);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2S1_H2->Draw( "colz" );

   // Sts maps with cuts
   cStsCuts->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1_S1->Draw( "colz" );

   cStsCuts->cd(10);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2_S2->Draw( "colz" );

   cStsCuts->cd(16);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1S2_S1->Draw( "colz" );

   cStsCuts->cd(22);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2S1_S2->Draw( "colz" );

   // Projections on STS sencsor plane
   cStsCuts->cd(5);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1_Proj->Draw( "colz" );

   cStsCuts->cd(11);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2_Proj->Draw( "colz" );

   cStsCuts->cd(17);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1S2_Proj->Draw( "colz" );

   cStsCuts->cd(23);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2S1_Proj->Draw( "colz" );

   // Residuals
   cStsCuts->cd(6);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhResidualsBestPairsHodoS1->Draw( "colz" );

   cStsCuts->cd(12);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhResidualsBestPairsHodoS2->Draw( "colz" );

   cStsCuts->cd(18);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhResidualsBestPairsHodoS1S2->Draw( "colz" );

   cStsCuts->cd(24);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhResidualsBestPairsHodoS2S1->Draw( "colz" );
//====================================================================//

//====================================================================//
   fcCanvasProjectionMaps = new TCanvas( "cProjMap",
                                    "Hodo pairs projection in STS sensor plane maps",
                                    w, h);
   fcCanvasProjectionMaps->Divide( 3, 2 );

   // S1
   fcCanvasProjectionMaps->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodo_ProjS1->Draw( "colz" );

   fcCanvasProjectionMaps->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1_Proj->Draw( "colz" );

   fcCanvasProjectionMaps->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS1GeoCut_Proj->Draw( "colz" );

   // S1
   fcCanvasProjectionMaps->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodo_ProjS2->Draw( "colz" );

   fcCanvasProjectionMaps->cd(5);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2_Proj->Draw( "colz" );

   fcCanvasProjectionMaps->cd(6);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhMapBestPairsHodoS2GeoCut_Proj->Draw( "colz" );
//====================================================================//

//====================================================================//
   fcCanvasEfficiencyMaps = new TCanvas( "cEffMap",
                                    "Sts Efficiency maps",
                                    w, h);
   fcCanvasEfficiencyMaps->Divide( 4, 2 );

   // S1
   fcCanvasEfficiencyMaps->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS1->Draw( "colz" );

   fcCanvasEfficiencyMaps->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS1GeoCut->Draw( "colz" );

   fcCanvasEfficiencyMaps->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS1S2->Draw( "colz" );

   fcCanvasEfficiencyMaps->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS1S2GeoCut->Draw( "colz" );

   // S1
   fcCanvasEfficiencyMaps->cd(5);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS2->Draw( "colz" );

   fcCanvasEfficiencyMaps->cd(6);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS2GeoCut->Draw( "colz" );

   fcCanvasEfficiencyMaps->cd(7);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS2S1->Draw( "colz" );

   fcCanvasEfficiencyMaps->cd(8);
   gPad->SetGridx();
   gPad->SetGridy();
   fhEfficiencyMapS2S1GeoCut->Draw( "colz" );
//====================================================================//

  /** Recovers/Create Ms Size Canvase for CERN 2016 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas *>( gROOT->FindObject( "cMsSizeAll" ) );
  if( NULL == fcMsSizeAll )
  {
     fcMsSizeAll = new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
     fcMsSizeAll->Divide( 4, 4 );
      LOG(info) << "Created MS size canvas in STS monitor";
  } // if( NULL == fcMsSizeAll )
      else LOG(info) << "Recovered MS size canvas in STS monitor";

  /*****************************/
}

Bool_t CbmCosy2018MonitorEfficiency::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bCosy2018ResetEfficiency )
   {
      ResetAllHistos();
      bCosy2018ResetEfficiency = kFALSE;
   } // if( bCosy2018ResetEfficiency )
   if( bCosy2018WriteEfficiency )
   {
      SaveAllHistos( fsHistoFileFullname );
      bCosy2018WriteEfficiency = kFALSE;
   } // if( bCosy2018WriteEfficiency )

   LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
              << "microslices.";

   if( component < kiMaxNbFlibLinks )
      if( NULL == fhMsSz[ component ] )
      {
         TString sMsSzName = Form("MsSz_link_%02lu", component);
         TString sMsSzTitle = Form("Size of MS for nDPB of link %02lu; Ms Size [bytes]", component);
         fhMsSz[ component ] = new TH1F( sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000. );
         fHM->Add(sMsSzName.Data(), fhMsSz[ component ] );
         if (server) server->Register("/FlibRaw", fhMsSz[ component ] );
         sMsSzName = Form("MsSzTime_link_%02lu", component);
         sMsSzTitle = Form("Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]", component);
         fhMsSzTime[ component ] =  new TProfile( sMsSzName.Data(), sMsSzTitle.Data(), 15000, 0., 300. );
         fHM->Add( sMsSzName.Data(), fhMsSzTime[ component ] );
         if (server) server->Register("/FlibRaw", fhMsSzTime[ component ] );
         if( NULL != fcMsSizeAll )
         {
            fcMsSizeAll->cd( 1 + component );
            gPad->SetLogy();
            fhMsSzTime[ component ]->Draw("hist le0");
         } // if( NULL != fcMsSizeAll )
         LOG(info) << "Added MS size histo for component: " << component
                   << " (DPB)";
      } // if( NULL == fhMsSz[ component ] )

//   Int_t messageType = -111;
   // Loop over microslices
   size_t numCompMsInTs = ts.num_microslices(component);
/*
   if( fulCurrentTsIdx + 1 != ts.index() && 0 == component)
      LOG(info) << "Missed TS from  " << fulCurrentTsIdx
                << " to " << ts.index();
*/
   fulCurrentTsIdx = ts.index();

   if( fuMaxNbMicroslices < numCompMsInTs )
   {
      fuMaxNbMicroslices = numCompMsInTs;

      fvdMsTime.resize( fuMaxNbMicroslices );
      fvuChanNbHitsInMs.resize( fuNbStsXyters );
      fvdChanLastHitTimeInMs.resize( fuNbStsXyters );
      fvusChanLastHitAdcInMs.resize( fuNbStsXyters );
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {
         fvuChanNbHitsInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
         fvdChanLastHitTimeInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
         fvusChanLastHitAdcInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
         for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
         {
            fvuChanNbHitsInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
            fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
            fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
            for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
            {
               fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
               fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
               fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            } // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
         } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      LOG(info) << "CbmCosy2018MonitorEfficiency::DoUnpack => Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
      LOG(info) << "CbmCosy2018MonitorEfficiency::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
      LOG(info) << "CbmCosy2018MonitorEfficiency::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuMaxNbMicroslices;
   } // if( fuMaxNbMicroslices < numCompMsInTs )

   for( size_t m = 0; m < numCompMsInTs; ++m )
   {
      // Ignore overlap ms if number defined by user
      if( numCompMsInTs - fuOverlapMsNb <= m )
         continue;

      auto msDescriptor = ts.descriptor(component, m);
      fuCurrentEquipmentId = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(component, m));

      uint32_t size = msDescriptor.size;
      fulCurrentMsIdx = msDescriptor.idx;
      Double_t dMsTime = (1e-9) * static_cast<double>(fulCurrentMsIdx);
      LOG(debug) << "Microslice: " << fulCurrentMsIdx
                 << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
                 << " has size: " << size;

      fuCurrDpbId  = static_cast< uint32_t >( fuCurrentEquipmentId & 0xFFFF );
      fuCurrDpbIdx = fDpbIdIndexMap[ fuCurrDpbId ];

      if( component < kiMaxNbFlibLinks )
      {
         if( fdStartTimeMsSz < 0 )
            fdStartTimeMsSz = dMsTime;
         fhMsSz[ component ]->Fill( size );
         fhMsSzTime[ component ]->Fill( dMsTime - fdStartTimeMsSz, size);
      } // if( component < kiMaxNbFlibLinks )

      // Store MS time for coincidence plots
      fvdMsTime[ m ] = dMsTime;

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage) )
                              / kuBytesPerMessage;

      // Prepare variables for the loop on contents
      const uint32_t* pInBuff = reinterpret_cast<const uint32_t*>( msContent );

      for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )
      {
         // Fill message
         uint32_t ulData = static_cast<uint32_t>(pInBuff[uIdx]);

         stsxyter::Message mess( static_cast< uint32_t >( ulData & 0xFFFFFFFF ) );

         // Print message if requested
         if( fbPrintMessages )
            mess.PrintMess( std::cout, fPrintMessCtrl );

         stsxyter::MessType typeMess = mess.GetMessType();
         fmMsgCounter[ typeMess ] ++;

         fhHodoMessType->Fill( static_cast< uint16_t > (typeMess) );
         fhHodoMessTypePerDpb->Fill( fuCurrDpbIdx, static_cast< uint16_t > (typeMess) );

         switch( typeMess )
         {
            case stsxyter::MessType::Hit :
            {
               // Extract the eLink and Asic indices => Should GO IN the fill method now that obly hits are link/asic specific!
               UShort_t usElinkIdx = mess.GetLinkIndex();
               if( fuNbElinksPerDpb <= usElinkIdx )
               {
                  LOG(fatal) << "CbmCosy2018MonitorEfficiency::DoUnpack => "
                             << "eLink index out of bounds!"
                             << usElinkIdx << " VS " << fuNbElinksPerDpb;
               } // if( fuNbElinksPerDpb <= usElinkIdx )
               fhHodoMessTypePerElink->Fill( fuCurrDpbIdx * fuNbElinksPerDpb + usElinkIdx,
                                            static_cast< uint16_t > (typeMess) );

               UInt_t   uAsicIdx   = fvuElinkToAsic[fuCurrDpbIdx][usElinkIdx];

               FillHitInfo( mess, usElinkIdx, uAsicIdx, m );
               break;
            } // case stsxyter::MessType::Hit :
            case stsxyter::MessType::TsMsb :
            {
               FillTsMsbInfo( mess, uIdx, m );
               break;
            } // case stsxyter::MessType::TsMsb :
            case stsxyter::MessType::Epoch :
            {
               // The first message in the TS is a special ones: EPOCH
               FillEpochInfo( mess );

               if( 0 < uIdx )
                  LOG(info) << "CbmCosy2018MonitorEfficiency::DoUnpack => "
                            << "EPOCH message at unexpected position in MS: message "
                            << uIdx << " VS message 0 expected!";
               break;
            } // case stsxyter::MessType::TsMsb :
            case stsxyter::MessType::Empty :
            {
//               FillTsMsbInfo( mess );
               break;
            } // case stsxyter::MessType::Empty :
            case stsxyter::MessType::Dummy :
            {
               break;
            } // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
            default:
            {
               LOG(fatal) << "CbmCosy2018MonitorEfficiency::DoUnpack => "
                          << "Unknown message type, should never happen, stopping here!";
            }
         } // switch( mess.GetMessType() )
      } // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )

      // Sort the buffer of hits
      std::sort( fvmHitsInTs.begin(), fvmHitsInTs.end() );

      // Time differences plotting using the fully time sorted hits
      Double_t dLastTimeX1 = -1e12;
      Double_t dLastTimeY1 = -1e12;
      Double_t dLastTimeX2 = -1e12;
      Double_t dLastTimeY2 = -1e12;
      Double_t dLastTimeN1 = -1e12;
      Double_t dLastTimeP1 = -1e12;
      Double_t dLastTimeN2 = -1e12;
      Double_t dLastTimeP2 = -1e12;
      Double_t dDtX1Y1         = -1e12;
      Double_t dDtX2Y2         = -1e12;
      Double_t dDtN1P1         = -1e12;
      Double_t dDtN2P2         = -1e12;
      Double_t dDtX1Y1X2Y2     = -1e12;
      Double_t dDtN1P1N2P2     = -1e12;
      Double_t dDtX1Y1X2Y2N1P1 = -1e12;
      Double_t dDtX1Y1X2Y2N2P2 = -1e12;
      Double_t dDtH1H2S1S2     = -1e12;
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
         fvbAsicHasDuplicInMs[ uXyterIdx ] = kFALSE;
      Bool_t bDuplicatedHitsInMs = kFALSE;
      if( 0 < fvmHitsInTs.size() )
      {
         ULong64_t ulLastHitTime = ( *( fvmHitsInTs.rbegin() ) ).GetTs();
         std::vector< stsxyter::FinalHit >::iterator it;

//         std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
//         Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();

         for( it  = fvmHitsInTs.begin();
              it != fvmHitsInTs.end() && (*it).GetTs() < ulLastHitTime - 320; // 320 * 3.125 ns = 1000 ns
              ++it )
         {
            UShort_t usAsicIdx = (*it).GetAsic();
            UShort_t usChanIdx = (*it).GetChan();
            ULong64_t ulHitTs  = (*it).GetTs();
            UShort_t  usHitAdc = (*it).GetAdc();

//            Bool_t bHitInX = usChanIdx < fuNbChanPerAsic/2;

            Double_t dTimeSinceStartSec = (ulHitTs * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;
            if( 0 == ulHitTs - fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] ||
                ( 0 < fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] &&
                  0 == ( ulHitTs - fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] ) % 512 ) )
            {
               if( 0 == fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] - usHitAdc )
               {
                  fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] ++;
                  fvuNbHitSameTsAdcAsicLastS[ usAsicIdx ] ++;
               } // if( 0 == fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] - usHitAdc )
                  else
                  {
                     fhSetupSortedNbConsSameTsAdcChan[ usAsicIdx ]->Fill( usChanIdx, fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] );
                     fhSetupSortedNbConsSameTsVsTsAdc[ usAsicIdx ]->Fill( fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ],
                                                                          fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] );
                     fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] = 0;
                  } // else of if( 0 == fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] - usHitAdc )

               fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ] ++;
               fvuNbHitSameTsAsicLastS[ usAsicIdx ] ++;
/*
               LOG(info) << Form("Bad Hit on asic %u chan %3u, #%3u => TS %9llu ADC %2u  VS TS %9llu ADC %2u",
                                  usAsicIdx, usChanIdx, fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ],
                                  ulHitTs, usHitAdc,
                                  fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ],
                                  fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] );
*/
               fhSetupSortedNbSameTsChan[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx );
               fhSetupSortedSameTsAdcChan[ usAsicIdx ]->Fill( usChanIdx, usHitAdc );
               fhSetupSortedSameTsAdcChan[ usAsicIdx ]->Fill( usChanIdx, fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] );
               fhSetupSortedSameTsAdcDiff[ usAsicIdx ]->Fill( fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ],
                                                                static_cast< Int_t >( usHitAdc )
                                                              - fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] );
               fhSetupSortedRatioSameTsChan[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 1.0 );

               /// Jump the duplicated hits without updating the othe variables
               fvbAsicHasDuplicInMs[ usAsicIdx ] = kTRUE;
               bDuplicatedHitsInMs = kTRUE;
               continue;
            } // if( 0 == ulHitTs - fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] )
               else
               {
/*
                  if( 100 < fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ] )
                     LOG(fatal) << " More than 100 consecutive hits with same TS on asic"
                                << usAsicIdx << " chan " << usChanIdx
                                << " => " << fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ];
*/
                  fhSetupSortedNbConsSameTsVsTsAdc[ usAsicIdx ]->Fill( fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ],
                                                                       fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] );

                  fhSetupSortedNbConsSameTsChan[ usAsicIdx ]->Fill( usChanIdx, fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ] );
                  fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ] = 0;

                  fhSetupSortedNbConsSameTsAdcChan[ usAsicIdx ]->Fill( usChanIdx, fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] );
                  fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] = 0;

                  fhSetupSortedRatioSameTsChan[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 0.0 );

                  fvuNbHitDiffTsAsicLastS[ usAsicIdx ]++;
               } // else of if( 0 == ulHitTs - fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] )
            fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] = ulHitTs;
            fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] = usHitAdc;
         } // loop on hits untils hits within 100 ns of last one or last one itself are reached

         // Remove all hits in case some duplicated ones where found
         if( kTRUE == bDuplicatedHitsInMs )
            fvmHitsInTs.erase( fvmHitsInTs.begin(), fvmHitsInTs.end() );
            else fhSizeCleanMs->Fill( fvmHitsInTs.size() );

         std::vector< stsxyter::FinalHit >::iterator itFirstMatchY1 = fvmHitsInTs.begin();
         std::vector< stsxyter::FinalHit >::iterator itFirstMatchY2 = fvmHitsInTs.begin();
         std::vector< stsxyter::FinalHit >::iterator itFirstMatchP1 = fvmHitsInTs.begin();
         std::vector< stsxyter::FinalHit >::iterator itFirstMatchP2 = fvmHitsInTs.begin();
         for( it  = fvmHitsInTs.begin();
              it != fvmHitsInTs.end() && (*it).GetTs() < ulLastHitTime - 320; // 320 * 3.125 ns = 1000 ns
              ++it )
         {
            UShort_t usAsicIdx = (*it).GetAsic();
            UShort_t usChanIdx = (*it).GetChan();
            ULong64_t ulHitTs  = (*it).GetTs();
//            UShort_t  usHitAdc = (*it).GetAdc();

            fhHodoChanCntRawGood[usAsicIdx]->Fill(usChanIdx);

            Bool_t bHitInX = usChanIdx < fuNbChanPerAsic/2;

            Double_t dTimeSinceStartSec = (ulHitTs * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;
            if( fUnpackParHodo->GetAsicIndexHodo1() == usAsicIdx )
            {
               if( fUnpackParHodo->IsXySwappedHodo1() )
                  bHitInX = !bHitInX;

               if( bHitInX )
               {
                  Double_t dDtSameFeb = ( ulHitTs - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                  fhSetupSortedDtX1->Fill( dDtSameFeb );

                  fLastSortedHit1X = (*it);
                  dLastTimeX1      = ulHitTs;

                  /// Compute fiber Idx of the last Hodo hits
                  UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo1() )
                     uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                  UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo1() )
                     uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                  UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo2() )
                     uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                  UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo2() )
                     uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

                  if( dTimeSinceStartSec < fdSpillEvoLength )
                  {
                     fhHodoX1SpillEvo->Fill( dTimeSinceStartSec, uFiberIdxX1 );
                     fhHodoX1SpillEvoProf->Fill( dTimeSinceStartSec, uFiberIdxX1 );
                  } // if( dTimeSinceStartSec < fdSpillEvoLength )

                  dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                  dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                  dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                                  * stsxyter::kdClockCycleNs / 2.0;
                  dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  if( kTRUE == fbDualStsEna )
                  {
                     dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                     dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                     * stsxyter::kdClockCycleNs / 2.0;
                     dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                        - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                       ) * stsxyter::kdClockCycleNs;
                     dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                        - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                       ) * stsxyter::kdClockCycleNs / 4.0;
                  } // if( kTRUE == fbDualStsEna )

                  fhSetupSortedDtX1Y1->Fill(         dDtX1Y1 );
                  fhSetupSortedDtX1Y1X2Y2->Fill(     dDtX1Y1X2Y2 );
                  fhSetupSortedDtX1Y1X2Y2N1P1->Fill( dDtX1Y1X2Y2N1P1 );
                  if( kTRUE == fbDualStsEna )
                  {
                     fhSetupSortedDtX1Y1X2Y2N2P2->Fill( dDtX1Y1X2Y2N2P2 );
                     fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );
                  } // if( kTRUE == fbDualStsEna )

                  if( fdCoincMinHodo < dDtX1Y1 && dDtX1Y1 < fdCoincMaxHodo )
                  {
                     fhSetupSortedMapX1Y1->Fill( uFiberIdxX1, uFiberIdxY1 );
                     fhSetupSortedCntEvoX1Y1->Fill( dTimeSinceStartSec );
                  } // if( fdCoincMinHodo < dDtX1Y1 && dDtX1Y1 < fdCoincMaxHodo )

                  /// Multiplicity estimation
                  UInt_t uNbY1CoincPerX1 = 0;
                  Double_t dDtHodo1 = 1e12;
                  Double_t dDtBestPair = 1e12;
                  std::vector< stsxyter::FinalHit >::iterator itBestY1 = fvmHitsInTs.end();
                  for( std::vector< stsxyter::FinalHit >::iterator itY1  = itFirstMatchY1; itY1 != fvmHitsInTs.end(); ++itY1 )
                  {
                     Bool_t bHitInY1 = ( (*itY1).GetChan() >= fuNbChanPerAsic/2 ) != fUnpackParHodo->IsXySwappedHodo1();

                     if( fUnpackParHodo->GetAsicIndexHodo1() == (*itY1).GetAsic() && bHitInY1 )
                     {
                        dDtHodo1 = ( static_cast< Double_t >( (*itY1).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                   ) * stsxyter::kdClockCycleNs;
                        if( fdCoincMinHodo < dDtHodo1 )
                        {
                           if( dDtHodo1 < fdCoincMaxHodo )
                           {
                              uNbY1CoincPerX1 ++;
                              if( TMath::Abs( dDtHodo1 - fdCoincCenterHodo ) < dDtBestPair )
                              {
                                 itBestY1 = itY1;
                                 dDtBestPair = TMath::Abs( dDtHodo1 - fdCoincCenterHodo );
                              } // if( TMath::Abs( dDtHodo1 - fdCoincCenterHodo ) < dDtBestPair )
                           } // if( dDtHodo1 < fdCoincMaxHodo )
                              else break;
                        } // if( fdCoincMinHodo < dDtHodo1 )
                           else itFirstMatchY1 = itY1;
                     } // if( fUnpackParHodo->GetAsicIndexHodo1() == (*itY1).GetAsic() && bHitInY1 )
                  } // for( itY1  = itFirstMatchY1; itY1 != fvmHitsInTs.end(); ++itY1 )
                  if( 0 < uNbY1CoincPerX1 )
                  {
                     fvPairsH1.push_back( std::pair< stsxyter::FinalHit, stsxyter::FinalHit >( (*it), (*itBestY1) ) );

                     fhDtBestPairsH1->Fill( ( static_cast< Double_t >( (*itBestY1).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                            ) * stsxyter::kdClockCycleNs );

                     Double_t dPosX, dPosY;
                     ComputeCoordinatesHodo1( (*it).GetChan(), (*itBestY1).GetChan(), dPosX, dPosY );
                     fhMapBestPairsH1->Fill( dPosX, dPosY );
                  } // if( 0 < uNbY1CoincPerX1 )
                  fhNbY1CoincPerX1->Fill( uNbY1CoincPerX1 );

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
                  if( 0 < uFiberIdxX1 )
                  {
                     Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanH1X[ uFiberIdxX1 - 1].GetTs() )
                                            ) * stsxyter::kdClockCycleNs;

                     fhDtNeighborChansH1X->Fill( dDtPrevChan, uFiberIdxX1 - 1 );
                  } // if( 0 < (*it).GetChan() )
                  if( uFiberIdxX1 < fuNbChanPerAsic/2 - 1 )
                  {
                     Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanH1X[ uFiberIdxX1 + 1 ].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                            ) * stsxyter::kdClockCycleNs;

                     fhDtNeighborChansH1X->Fill( dDtNextChan, uFiberIdxX1 );
                  } // if( 0 < (*it).GetChan() )

                  fvLastHitChanH1X[ uFiberIdxX1 ] = (*it);

                  Bool_t bFoundCluster = kFALSE;
                  for( UInt_t uCluster = 0; uCluster < fvClustersH1X.size(); ++uCluster )
                  {
                     if( kTRUE == fvClustersH1X[ uCluster ].CheckAddHit( (*it) ) )
                     {
                        bFoundCluster = kTRUE;
                        break;
                     } // if( kTRUE == fvClustersH1X[ uCluster ].CheckAddHit( (*it) ) )
                  } // for( UInt_t uCluster = 0; uCluster < fvClustersH1X.size(); ++uCluster )
                  if( kFALSE == bFoundCluster )
                  {
                     Cosy2018TestCluster newClust( (*it) );
                     fvClustersH1X.push_back( newClust );
                  } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
               } // if( bHitInX )
                  else
                  {
                     Double_t dDtSameFeb = ( ulHitTs - dLastTimeY1 ) * stsxyter::kdClockCycleNs;
                     fhSetupSortedDtY1->Fill( dDtSameFeb );

                     fLastSortedHit1Y = (*it);
                     dLastTimeY1      = ulHitTs;

                     /// Compute fiber Idx of the last Hodo hits
                     UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
                     if( fUnpackParHodo->IsXInvertedHodo1() )
                        uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                     UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
                     if( fUnpackParHodo->IsYInvertedHodo1() )
                        uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                     UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
                     if( fUnpackParHodo->IsXInvertedHodo2() )
                        uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                     UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
                     if( fUnpackParHodo->IsYInvertedHodo2() )
                        uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

                     if( dTimeSinceStartSec < fdSpillEvoLength )
                     {
                        fhHodoY1SpillEvo->Fill( dTimeSinceStartSec, uFiberIdxY1 );
                        fhHodoY1SpillEvoProf->Fill( dTimeSinceStartSec, uFiberIdxY1 );
                     } // if( dTimeSinceStartSec < fdSpillEvoLength )

                     dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                     dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                     dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                     dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                                   * stsxyter::kdClockCycleNs / 2.0;
                     dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                        - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                       ) * stsxyter::kdClockCycleNs;
                     if( kTRUE == fbDualStsEna )
                     {
                        dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                        dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                        * stsxyter::kdClockCycleNs / 2.0;
                        dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                           - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                          ) * stsxyter::kdClockCycleNs;
                        dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                           - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                          ) * stsxyter::kdClockCycleNs / 4.0;
                     } // if( kTRUE == fbDualStsEna )

                     fhSetupSortedDtX1Y1->Fill(         dDtX1Y1 );
                     fhSetupSortedDtX1Y1X2Y2->Fill(     dDtX1Y1X2Y2 );
                     fhSetupSortedDtX1Y1X2Y2N1P1->Fill( dDtX1Y1X2Y2N1P1 );
                     if( kTRUE == fbDualStsEna )
                     {
                        fhSetupSortedDtX1Y1X2Y2N2P2->Fill( dDtX1Y1X2Y2N2P2 );
                        fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );
                     } // if( kTRUE == fbDualStsEna )

                     if( fdCoincMinHodo < dDtX1Y1 && dDtX1Y1 < fdCoincMaxHodo )
                     {
                        fhSetupSortedMapX1Y1->Fill( uFiberIdxX1, uFiberIdxY1 );
                        fhSetupSortedCntEvoX1Y1->Fill( dTimeSinceStartSec );
                     } // if( fdCoincMinHodo < dDtX1Y1 && dDtX1Y1 < fdCoincMaxHodo )

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
                     if( 0 < uFiberIdxY1 )
                     {
                        Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanH1Y[ uFiberIdxY1 - 1].GetTs() )
                                               ) * stsxyter::kdClockCycleNs;

                        fhDtNeighborChansH1Y->Fill( dDtPrevChan, uFiberIdxY1 - 1 );
                     } // if( 0 < (*it).GetChan() )
                     if( uFiberIdxY1 < fuNbChanPerAsic/2 - 1 )
                     {
                        Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanH1Y[ uFiberIdxY1 + 1 ].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                               ) * stsxyter::kdClockCycleNs;

                        fhDtNeighborChansH1Y->Fill( dDtNextChan, uFiberIdxY1 );
                     } // if( 0 < (*it).GetChan() )

                     fvLastHitChanH1Y[ uFiberIdxY1 ] = (*it);

                     Bool_t bFoundCluster = kFALSE;
                     for( UInt_t uCluster = 0; uCluster < fvClustersH1Y.size(); ++uCluster )
                     {
                        if( kTRUE == fvClustersH1Y[ uCluster ].CheckAddHit( (*it) ) )
                        {
                           bFoundCluster = kTRUE;
                           break;
                        } // if( kTRUE == fvClustersH1Y[ uCluster ].CheckAddHit( (*it) ) )
                     } // for( UInt_t uCluster = 0; uCluster < fvClustersH1Y.size(); ++uCluster )
                     if( kFALSE == bFoundCluster )
                     {
                        Cosy2018TestCluster newClust( (*it) );
                        fvClustersH1Y.push_back( newClust );
                     } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
                  } // else of if( bHitInX )
            } // if( fUnpackParHodo->GetAsicIndexHodo1() == usAsicIdx )
            else if( fUnpackParHodo->GetAsicIndexHodo2() == usAsicIdx )
            {
               if( fUnpackParHodo->IsXySwappedHodo2() )
                  bHitInX = !bHitInX;

               if( bHitInX )
               {
                  Double_t dDtSameFeb = ( ulHitTs - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                  fhSetupSortedDtX2->Fill( dDtSameFeb );

                  fLastSortedHit2X = (*it);
                  dLastTimeX2      = ulHitTs;

                  /// Compute fiber Idx of the last Hodo hits
                  UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo1() )
                     uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                  UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo1() )
                     uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                  UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo2() )
                     uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                  UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo2() )
                     uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

                  if( dTimeSinceStartSec < fdSpillEvoLength )
                  {
                     fhHodoX2SpillEvo->Fill( dTimeSinceStartSec, uFiberIdxX2 );
                     fhHodoX2SpillEvoProf->Fill( dTimeSinceStartSec, uFiberIdxX2 );
                  } // if( dTimeSinceStartSec < fdSpillEvoLength )

                  dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                  dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                  dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                                * stsxyter::kdClockCycleNs / 2.0;
                  dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  if( kTRUE == fbDualStsEna )
                  {
                     dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                     dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                     * stsxyter::kdClockCycleNs / 2.0;
                     dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                        - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                       ) * stsxyter::kdClockCycleNs;
                     dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                        - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                       ) * stsxyter::kdClockCycleNs / 4.0;
                  } // if( kTRUE == fbDualStsEna )

                  fhSetupSortedDtX2Y2->Fill( dDtX2Y2 );
                  fhSetupSortedDtX1Y1X2Y2->Fill(     dDtX1Y1X2Y2 );
                  fhSetupSortedDtX1Y1X2Y2N1P1->Fill( dDtX1Y1X2Y2N1P1 );
                  if( kTRUE == fbDualStsEna )
                  {
                     fhSetupSortedDtX1Y1X2Y2N2P2->Fill( dDtX1Y1X2Y2N2P2 );
                     fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );
                  } // if( kTRUE == fbDualStsEna )

                  if( fdCoincMinHodo < dDtX2Y2 && dDtX2Y2 < fdCoincMaxHodo )
                  {
                     fhSetupSortedMapX2Y2->Fill( uFiberIdxX2, uFiberIdxY2 );
                     fhSetupSortedCntEvoX2Y2->Fill( dTimeSinceStartSec );
                  } // if( fdCoincMinHodo < dDtX2Y2 && dDtX2Y2 < fdCoincMaxHodo )

                  /// Multiplicity estimation
                  UInt_t uNbY2CoincPerX2 = 0;
                  Double_t dDtHodo2 = 1e12;
                  Double_t dDtBestPair = 1e12;
                  std::vector< stsxyter::FinalHit >::iterator itBestY2 = fvmHitsInTs.end();
                  for( std::vector< stsxyter::FinalHit >::iterator itY2  = itFirstMatchY2; itY2 != fvmHitsInTs.end(); ++itY2 )
                  {
                     Bool_t bHitInY2 = ( (*itY2).GetChan() >= fuNbChanPerAsic/2 ) != fUnpackParHodo->IsXySwappedHodo2();

                     if( fUnpackParHodo->GetAsicIndexHodo2() == (*itY2).GetAsic() && bHitInY2 )
                     {
                        dDtHodo2 = ( static_cast< Double_t >( (*itY2).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                   ) * stsxyter::kdClockCycleNs;
                        if( fdCoincMinHodo < dDtHodo2 )
                        {
                           if( dDtHodo2 < fdCoincMaxHodo )
                           {
                              uNbY2CoincPerX2 ++;
                              if( TMath::Abs( dDtHodo2 - fdCoincCenterHodo ) < dDtBestPair )
                              {
                                 itBestY2 = itY2;
                                 dDtBestPair = TMath::Abs( dDtHodo2 - fdCoincCenterHodo );
                              } // if( TMath::Abs( dDtHodo2 - fdCoincCenterHodo ) < dDtBestPair )
                           } // if( dDtHodo2 < fdCoincMaxHodo )
                              else break;
                        } // if( fdCoincMinHodo < dDtHodo2 )
                           else itFirstMatchY2 = itY2;
                     } // if( fUnpackParHodo->GetAsicIndexHodo2() == (*itY2).GetAsic() && bHitInY2 )
                  } // for( itY2  = itFirstMatchY1; itY2 != fvmHitsInTs.end(); ++itY2 )
                  if( 0 < uNbY2CoincPerX2 )
                  {
                     fvPairsH2.push_back( std::pair< stsxyter::FinalHit, stsxyter::FinalHit >( (*it), (*itBestY2) ) );

                     fhDtBestPairsH2->Fill( ( static_cast< Double_t >( (*itBestY2).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                            ) * stsxyter::kdClockCycleNs );

                     Double_t dPosX, dPosY;
                     ComputeCoordinatesHodo2( (*it).GetChan(), (*itBestY2).GetChan(), dPosX, dPosY );
                     fhMapBestPairsH2->Fill( dPosX, dPosY );
                  } // if( 0 < uNbY2CoincPerX2 )
                  fhNbY2CoincPerX2->Fill( uNbY2CoincPerX2 );

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
                  if( 0 < uFiberIdxX2 )
                  {
                     Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanH2X[ uFiberIdxX2 - 1].GetTs() )
                                            ) * stsxyter::kdClockCycleNs;

                     fhDtNeighborChansH2X->Fill( dDtPrevChan, uFiberIdxX2 - 1 );
                  } // if( 0 < (*it).GetChan() )
                  if( uFiberIdxX2 < fuNbChanPerAsic/2 - 1 )
                  {
                     Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanH2X[ uFiberIdxX2 + 1 ].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                            ) * stsxyter::kdClockCycleNs;

                     fhDtNeighborChansH2X->Fill( dDtNextChan, uFiberIdxX2 );
                  } // if( 0 < (*it).GetChan() )

                  fvLastHitChanH2X[ uFiberIdxX2 ] = (*it);

                  Bool_t bFoundCluster = kFALSE;
                  for( UInt_t uCluster = 0; uCluster < fvClustersH2X.size(); ++uCluster )
                  {
                     if( kTRUE == fvClustersH2X[ uCluster ].CheckAddHit( (*it) ) )
                     {
                        bFoundCluster = kTRUE;
                        break;
                     } // if( kTRUE == fvClustersH2X[ uCluster ].CheckAddHit( (*it) ) )
                  } // for( UInt_t uCluster = 0; uCluster < fvClustersH2X.size(); ++uCluster )
                  if( kFALSE == bFoundCluster )
                  {
                     Cosy2018TestCluster newClust( (*it) );
                     fvClustersH2X.push_back( newClust );
                  } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
               } // if( bHitInX )
                  else
                  {
                     Double_t dDtSameFeb = ( ulHitTs - dLastTimeY2 ) * stsxyter::kdClockCycleNs;
                     fhSetupSortedDtY2->Fill( dDtSameFeb );

                     fLastSortedHit2Y = (*it);
                     dLastTimeY2      = ulHitTs;

                     /// Compute fiber Idx of the last Hodo hits
                     UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
                     if( fUnpackParHodo->IsXInvertedHodo1() )
                        uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                     UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
                     if( fUnpackParHodo->IsYInvertedHodo1() )
                        uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                     UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
                     if( fUnpackParHodo->IsXInvertedHodo2() )
                        uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                     UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
                     if( fUnpackParHodo->IsYInvertedHodo2() )
                        uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

                     if( dTimeSinceStartSec < fdSpillEvoLength )
                     {
                        fhHodoY2SpillEvo->Fill( dTimeSinceStartSec, uFiberIdxY2 );
                        fhHodoY2SpillEvoProf->Fill( dTimeSinceStartSec, uFiberIdxY2 );
                     } // if( dTimeSinceStartSec < fdSpillEvoLength )

                     dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                     dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                     dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                     dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                                   * stsxyter::kdClockCycleNs / 2.0;
                     dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                        - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                       ) * stsxyter::kdClockCycleNs;
                     if( kTRUE == fbDualStsEna )
                     {
                        dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                        dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                        * stsxyter::kdClockCycleNs / 2.0;
                        dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                           - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                          ) * stsxyter::kdClockCycleNs;
                        dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                           - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                          ) * stsxyter::kdClockCycleNs / 4.0;
                     } // if( kTRUE == fbDualStsEna )

                     fhSetupSortedDtX2Y2->Fill( dDtX2Y2 );
                     fhSetupSortedDtX1Y1X2Y2->Fill(     dDtX1Y1X2Y2 );
                     fhSetupSortedDtX1Y1X2Y2N1P1->Fill( dDtX1Y1X2Y2N1P1 );
                     if( kTRUE == fbDualStsEna )
                     {
                        fhSetupSortedDtX1Y1X2Y2N2P2->Fill( dDtX1Y1X2Y2N2P2 );
                        fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );
                     } // if( kTRUE == fbDualStsEna )

                     if( fdCoincMinHodo < dDtX2Y2 && dDtX2Y2 < fdCoincMaxHodo )
                     {
                        fhSetupSortedMapX2Y2->Fill( uFiberIdxX2, uFiberIdxY2 );
                        fhSetupSortedCntEvoX2Y2->Fill( dTimeSinceStartSec );
                     } // if( fdCoincMinHodo < dDtX2Y2 && dDtX2Y2 < fdCoincMaxHodo )

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
                     if( 0 < uFiberIdxY2 )
                     {
                        Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanH2Y[ uFiberIdxY2 - 1].GetTs() )
                                               ) * stsxyter::kdClockCycleNs;

                        fhDtNeighborChansH2Y->Fill( dDtPrevChan, uFiberIdxY2 - 1 );
                     } // if( 0 < (*it).GetChan() )
                     if( uFiberIdxY2 < fuNbChanPerAsic/2 - 1 )
                     {
                        Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanH2Y[ uFiberIdxY2 + 1 ].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                               ) * stsxyter::kdClockCycleNs;

                        fhDtNeighborChansH2Y->Fill( dDtNextChan, uFiberIdxY2 );
                     } // if( 0 < (*it).GetChan() )

                     fvLastHitChanH2Y[ uFiberIdxY2 ] = (*it);

                     Bool_t bFoundCluster = kFALSE;
                     for( UInt_t uCluster = 0; uCluster < fvClustersH2Y.size(); ++uCluster )
                     {
                        if( kTRUE == fvClustersH2Y[ uCluster ].CheckAddHit( (*it) ) )
                        {
                           bFoundCluster = kTRUE;
                           break;
                        } // if( kTRUE == fvClustersH2Y[ uCluster ].CheckAddHit( (*it) ) )
                     } // for( UInt_t uCluster = 0; uCluster < fvClustersH2Y.size(); ++uCluster )
                     if( kFALSE == bFoundCluster )
                     {
                        Cosy2018TestCluster newClust( (*it) );
                        fvClustersH2Y.push_back( newClust );
                     } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
                  } // else of if( bHitInX )
            } // else if( fUnpackParHodo->GetAsicIndexHodo2() == usAsicIdx )
            else if( fUnpackParSts->GetAsicIndexSts1N() == usAsicIdx )
            {
               Double_t dDtSameFeb = ( ulHitTs - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
               fhSetupSortedDtN1->Fill( dDtSameFeb );

               fLastSortedHit1N = (*it);
               dLastTimeN1      = ulHitTs;

               /// Compute fiber Idx of the last Hodo hits
               UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
               if( fUnpackParHodo->IsXInvertedHodo1() )
                  uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
               UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
               if( fUnpackParHodo->IsYInvertedHodo1() )
                  uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
               UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
               if( fUnpackParHodo->IsXInvertedHodo2() )
                  uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
               UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
               if( fUnpackParHodo->IsYInvertedHodo2() )
                  uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

               dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
               dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
               dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
               dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                             * stsxyter::kdClockCycleNs / 2.0;
               dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                  - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                 ) * stsxyter::kdClockCycleNs;
               if( kTRUE == fbDualStsEna )
               {
                  dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                  * stsxyter::kdClockCycleNs / 2.0;
                  dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                    ) * stsxyter::kdClockCycleNs / 4.0;
               } // if( kTRUE == fbDualStsEna )

               fhSetupSortedDtN1P1->Fill( dDtN1P1 );
               fhSetupSortedDtX1Y1X2Y2N1P1->Fill( dDtX1Y1X2Y2N1P1 );
               if( kTRUE == fbDualStsEna )
               {
                  fhSetupSortedDtN1P1N2P2->Fill(     dDtN1P1N2P2 );
                  fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );
               } // if( kTRUE == fbDualStsEna )

               if( fdCoincMinSts1 < dDtN1P1 && dDtN1P1 < fdCoincMaxSts1 )
               {
                  fhSetupSortedMapN1P1->Fill( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan() );
                  fhSetupSortedCntEvoN1P1->Fill( dTimeSinceStartSec );

                  Double_t dX1, dY1;
                  ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                  fhStsSortedMapS1->Fill( dX1, dY1 );

                  if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder && kTRUE == fbDualStsEna )
                  {
                     Double_t dX2, dY2;
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapS1Coinc->Fill( dX1, dY1 );
                     fhStsSortedMapS2Coinc->Fill( dX2, dY2 );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder && kTRUE == fbDualStsEna )
               } // if( fdCoincMinSts1 < dDtN1P1 && dDtN1P1 < fdCoincMaxSts1 )

               /// Multiplicity estimation
               UInt_t uNbP1CoincPerN1 = 0;
               Double_t dDtSts1 = 1e12;
               Double_t dDtBestPair = 1e12;
               std::vector< stsxyter::FinalHit >::iterator itBestP1 = fvmHitsInTs.end();
               for( std::vector< stsxyter::FinalHit >::iterator itP1  = itFirstMatchP1; itP1 != fvmHitsInTs.end(); ++itP1 )
               {
                  if( fUnpackParSts->GetAsicIndexSts1P() == (*itP1).GetAsic() )
                  {
                     dDtSts1 = ( static_cast< Double_t >( (*itP1).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                ) * stsxyter::kdClockCycleNs;
                     if( fdCoincMinSts1 < dDtSts1 )
                     {
                        if( dDtSts1 < fdCoincMaxSts1 )
                        {
                           // Ignore unphysical pairs
                           if( kFALSE == CheckPhysPairSensor1( (*it).GetChan(), (*itP1).GetChan() ) )
                              continue;

                           uNbP1CoincPerN1 ++;
                           if( TMath::Abs( dDtSts1 - fdCoincCenterSts1 ) < dDtBestPair )
                           {
                              itBestP1 = itP1;
                              dDtBestPair = TMath::Abs( dDtSts1 - fdCoincCenterSts1 );
                           } // if( TMath::Abs( dDtSts1 - fdCoincCenterSts1 ) < dDtBestPair )
                        }// if( dDtSts1 < fdCoincMaxSts1 )
                           else break;
                     } // if( fdCoincMinSts < dDtSts1 )
                        else itFirstMatchP1 = itP1;
                  } // if( fUnpackParSts->GetAsicIndexSts1P() == usAsicIdx )
               } // for( itP1  = itFirstMatchP1; itP1 != fvmHitsInTs.end(); ++itP1 )
               if( 0 < uNbP1CoincPerN1 )
               {
                  fvPairsS1.push_back( std::pair< stsxyter::FinalHit, stsxyter::FinalHit >( (*it), (*itBestP1) ) );
                  fhDtBestPairsS1->Fill( ( static_cast< Double_t >( (*itBestP1).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                         ) * stsxyter::kdClockCycleNs );

                  fhChanMapBestMatchS1->Fill( (*it).GetChan(), (*itBestP1).GetChan() );

                  Double_t dPosX, dPosY;
                  ComputeCoordinatesSensor1( (*it).GetChan(), (*itBestP1).GetChan(), dPosX, dPosY );
                  fhMapBestPairsS1->Fill( dPosX, dPosY );
               } // if( 0 < uNbP1CoincPerN1 )
               fhNbP1CoincPerN1->Fill( uNbP1CoincPerN1 );

               fvHitsS1N.push_back( (*it) );

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
               if( 0 < usChanIdx )
               {
                  Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanS1N[ usChanIdx - 1].GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS1N->Fill( dDtPrevChan, usChanIdx - 1 );
               } // if( 0 < (*it).GetChan() )
               if( usChanIdx < fuNbChanPerAsic - 1 )
               {
                  Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanS1N[ usChanIdx + 1 ].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS1N->Fill( dDtNextChan, usChanIdx );
               } // if( 0 < (*it).GetChan() )

               fvLastHitChanS1N[ usChanIdx ] = (*it);

               Bool_t bFoundCluster = kFALSE;
               for( UInt_t uCluster = 0; uCluster < fvClustersS1N.size(); ++uCluster )
               {
                  if( kTRUE == fvClustersS1N[ uCluster ].CheckAddHit( (*it) ) )
                  {
                     bFoundCluster = kTRUE;
                     break;
                  } // if( kTRUE == fvClustersS1N[ uCluster ].CheckAddHit( (*it) ) )
               } // for( UInt_t uCluster = 0; uCluster < fvClustersS1N.size(); ++uCluster )
               if( kFALSE == bFoundCluster )
               {
                  Cosy2018TestCluster newClust( (*it) );
                  fvClustersS1N.push_back( newClust );
               } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
            } // if( fUnpackParSts->GetAsicIndexSts1N() == usAsicIdx )
            else if( fUnpackParSts->GetAsicIndexSts1P() == usAsicIdx )
            {
               Double_t dDtSameFeb = ( ulHitTs - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
               fhSetupSortedDtP1->Fill( dDtSameFeb );

               fLastSortedHit1P = (*it);
               dLastTimeP1      = ulHitTs;

               /// Compute fiber Idx of the last Hodo hits
               UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
               if( fUnpackParHodo->IsXInvertedHodo1() )
                  uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
               UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
               if( fUnpackParHodo->IsYInvertedHodo1() )
                  uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
               UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
               if( fUnpackParHodo->IsXInvertedHodo2() )
                  uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
               UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
               if( fUnpackParHodo->IsYInvertedHodo2() )
                  uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

               dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
               dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
               dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
               dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                             * stsxyter::kdClockCycleNs / 2.0;
               dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                  - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                 ) * stsxyter::kdClockCycleNs;
               if( kTRUE == fbDualStsEna )
               {
                  dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                  * stsxyter::kdClockCycleNs / 2.0;
                  dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                    ) * stsxyter::kdClockCycleNs / 4.0;
               } // if( kTRUE == fbDualStsEna )

               fhSetupSortedDtN1P1->Fill( dDtN1P1 );
               fhSetupSortedDtX1Y1X2Y2N1P1->Fill( dDtX1Y1X2Y2N1P1 );
               if( kTRUE == fbDualStsEna )
               {
                  fhSetupSortedDtN1P1N2P2->Fill(     dDtN1P1N2P2 );
                  fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );
               } // if( kTRUE == fbDualStsEna )

               if( fdCoincMinSts1 < dDtN1P1 && dDtN1P1 < fdCoincMaxSts1 )
               {
                  fhSetupSortedMapN1P1->Fill( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan() );
                  fhSetupSortedCntEvoN1P1->Fill( dTimeSinceStartSec );

                  Double_t dX1, dY1;
                  ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                  fhStsSortedMapS1->Fill( dX1, dY1 );

                  if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder && kTRUE == fbDualStsEna )
                  {
                     Double_t dX2, dY2;
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapS1Coinc->Fill( dX1, dY1 );
                     fhStsSortedMapS2Coinc->Fill( dX2, dY2 );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder && kTRUE == fbDualStsEna )
               } // if( fdCoincMinSts1 < dDtN1P1 && dDtN1P1 < fdCoincMaxSts1 )

               fvHitsS1P.push_back( (*it) );

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
               if( 0 < usChanIdx )
               {
                  Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanS1P[ usChanIdx - 1].GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS1P->Fill( dDtPrevChan, usChanIdx - 1 );
               } // if( 0 < (*it).GetChan() )
               if( usChanIdx < fuNbChanPerAsic - 1 )
               {
                  Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanS1P[ usChanIdx + 1 ].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS1P->Fill( dDtNextChan, usChanIdx );
               } // if( 0 < (*it).GetChan() )

               fvLastHitChanS1P[ usChanIdx ] = (*it);

               Bool_t bFoundCluster = kFALSE;
               for( UInt_t uCluster = 0; uCluster < fvClustersS1P.size(); ++uCluster )
               {
                  if( kTRUE == fvClustersS1P[ uCluster ].CheckAddHit( (*it) ) )
                  {
                     bFoundCluster = kTRUE;
                     break;
                  } // if( kTRUE == fvClustersS1P[ uCluster ].CheckAddHit( (*it) ) )
               } // for( UInt_t uCluster = 0; uCluster < fvClustersS1P.size(); ++uCluster )
               if( kFALSE == bFoundCluster )
               {
                  Cosy2018TestCluster newClust( (*it) );
                  fvClustersS1P.push_back( newClust );
               } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
            } // else if( fUnpackParSts->GetAsicIndexSts1P() == usAsicIdx )
            else if( kTRUE == fbDualStsEna )
            {
               if( fUnpackParSts->GetAsicIndexSts2N() == usAsicIdx )
               {
                  Double_t dDtSameFeb = ( ulHitTs - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  fhSetupSortedDtN2->Fill( dDtSameFeb );

                  fLastSortedHit2N = (*it);
                  dLastTimeN2      = ulHitTs;

                  /// Compute fiber Idx of the last Hodo hits
                  UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo1() )
                     uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                  UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo1() )
                     uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                  UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo2() )
                     uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                  UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo2() )
                     uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

                  dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                  dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                  dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                                * stsxyter::kdClockCycleNs / 2.0;
                  dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                  * stsxyter::kdClockCycleNs / 2.0;
                  dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                    ) * stsxyter::kdClockCycleNs / 4.0;

                  fhSetupSortedDtN2P2->Fill( dDtN2P2 );
                  fhSetupSortedDtN1P1N2P2->Fill(     dDtN1P1N2P2 );
                  fhSetupSortedDtX1Y1X2Y2N2P2->Fill( dDtX1Y1X2Y2N2P2 );
                  fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );

                  if( fdCoincMinSts2 < dDtN2P2 && dDtN2P2 < fdCoincMaxSts2 )
                  {
                     fhSetupSortedMapN2P2->Fill( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan() );
                     fhSetupSortedCntEvoN2P2->Fill( dTimeSinceStartSec );

                     Double_t dX2, dY2;
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapS2->Fill( dX2, dY2 );

                     if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder )
                     {
                        Double_t dX1, dY1;
                        ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                        fhStsSortedMapS1Coinc->Fill( dX1, dY1 );
                        fhStsSortedMapS2Coinc->Fill( dX2, dY2 );
                     } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
                  } // if( fdCoincMinSts2 < dDtN2P2 && dDtN2P2 < fdCoincMaxSts2 )

                  /// Multiplicity estimation
                  UInt_t uNbP2CoincPerN2 = 0;
                  Double_t dDtSts2 = 1e12;
                  Double_t dDtBestPair = 1e12;
                  std::vector< stsxyter::FinalHit >::iterator itBestP2 = fvmHitsInTs.end();
                  for( std::vector< stsxyter::FinalHit >::iterator itP2  = itFirstMatchP2; itP2 != fvmHitsInTs.end(); ++itP2 )
                  {
                     if( fUnpackParSts->GetAsicIndexSts2P() == (*itP2).GetAsic() )
                     {
                        dDtSts2 = ( static_cast< Double_t >( (*itP2).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                   ) * stsxyter::kdClockCycleNs;
                        if( fdCoincMinSts2 < dDtSts2 )
                        {
                           if( dDtSts2 < fdCoincMaxSts2 )
                           {
                              // Ignore unphysical pairs
                              if( kFALSE == CheckPhysPairSensor2( (*it).GetChan(), (*itP2).GetChan() ) )
                                 continue;

                              uNbP2CoincPerN2 ++;
                              if( TMath::Abs( dDtSts2 - fdCoincCenterSts2 ) < dDtBestPair )
                              {
                                 itBestP2 = itP2;
                                 dDtBestPair = TMath::Abs( dDtSts2 - fdCoincCenterSts2 );
                              } // if( TMath::Abs( dDtSts2 - fdCoincCenterSts2 ) < dDtBestPair )
                           } // if( dDtSts2 < fdCoincMaxSts2 )
                              else break;
                        } // if( fdCoincMinSts2 < dDtSts2 )
                           else itFirstMatchP2 = itP2;
                     } // if( fUnpackParSts->GetAsicIndexSts2P() == usAsicIdx )
                  } // for( itP2  = itFirstMatchP2; itP2 != fvmHitsInTs.end(); ++itP2 )
                  if( 0 < uNbP2CoincPerN2 )
                  {
                     fvPairsS2.push_back( std::pair< stsxyter::FinalHit, stsxyter::FinalHit >( (*it), (*itBestP2) ) );
                     fhDtBestPairsS2->Fill( ( static_cast< Double_t >( (*itBestP2).GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                            ) * stsxyter::kdClockCycleNs );

                     Double_t dPosX, dPosY;
                     ComputeCoordinatesSensor1( (*it).GetChan(), (*itBestP2).GetChan(), dPosX, dPosY );
                     fhMapBestPairsS2->Fill( dPosX, dPosY );
                  } // if( 0 < uNbP2CoincPerN2 )
                  fhNbP2CoincPerN2->Fill( uNbP2CoincPerN2 );

                  fvHitsS2N.push_back( (*it) );

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
               if( 0 < usChanIdx )
               {
                  Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanS2N[ usChanIdx - 1].GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS2N->Fill( dDtPrevChan, usChanIdx - 1 );
               } // if( 0 < (*it).GetChan() )
               if( usChanIdx < fuNbChanPerAsic - 1 )
               {
                  Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanS2N[ usChanIdx + 1].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS2N->Fill( dDtNextChan, usChanIdx );
               } // if( 0 < (*it).GetChan() )

               fvLastHitChanS2N[ usChanIdx ] = (*it);

               Bool_t bFoundCluster = kFALSE;
               for( UInt_t uCluster = 0; uCluster < fvClustersS2N.size(); ++uCluster )
               {
                  if( kTRUE == fvClustersS2N[ uCluster ].CheckAddHit( (*it) ) )
                  {
                     bFoundCluster = kTRUE;
                     break;
                  } // if( kTRUE == fvClustersS2N[ uCluster ].CheckAddHit( (*it) ) )
               } // for( UInt_t uCluster = 0; uCluster < fvClustersS2N.size(); ++uCluster )
               if( kFALSE == bFoundCluster )
               {
                  Cosy2018TestCluster newClust( (*it) );
                  fvClustersS2N.push_back( newClust );
               } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
               } // if( fUnpackParSts->GetAsicIndexSts2N() == usAsicIdx )
               else if( fUnpackParSts->GetAsicIndexSts2P() == usAsicIdx )
               {
                  Double_t dDtSameFeb = ( ulHitTs - dLastTimeP2 ) * stsxyter::kdClockCycleNs;
                  fhSetupSortedDtP2->Fill( dDtSameFeb );

                  fLastSortedHit2P = (*it);
                  dLastTimeP2      = ulHitTs;

                  /// Compute fiber Idx of the last Hodo hits
                  UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo1() )
                     uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                  UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit1Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo1() )
                     uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                  UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2X.GetChan()  );
                  if( fUnpackParHodo->IsXInvertedHodo2() )
                     uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                  UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fLastSortedHit2Y.GetChan()  );
                  if( fUnpackParHodo->IsYInvertedHodo2() )
                     uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

                  dDtX1Y1     = ( dLastTimeY1 - dLastTimeX1 ) * stsxyter::kdClockCycleNs;
                  dDtX2Y2     = ( dLastTimeY2 - dLastTimeX2 ) * stsxyter::kdClockCycleNs;
                  dDtN1P1     = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  dDtN2P2     = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  dDtX1Y1X2Y2 = ( ( dLastTimeX2 + dLastTimeY2 ) - ( dLastTimeX1 + dLastTimeY1 ) )
                                * stsxyter::kdClockCycleNs / 2.0;
                  dDtN1P1N2P2 = ( ( dLastTimeN2 + dLastTimeP2 ) - ( dLastTimeN1 + dLastTimeP1 ) )
                                  * stsxyter::kdClockCycleNs / 2.0;
                  dDtX1Y1X2Y2N1P1 = (  ( dLastTimeN1 + dLastTimeP1 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  dDtX1Y1X2Y2N2P2 = (  ( dLastTimeN2 + dLastTimeP2 ) / 2.0
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 ) / 4.0
                                    ) * stsxyter::kdClockCycleNs;
                  dDtH1H2S1S2     = (  ( dLastTimeN2 + dLastTimeP2 + dLastTimeN1 + dLastTimeP1 )
                                     - ( dLastTimeX2 + dLastTimeY2 + dLastTimeX1 + dLastTimeY1 )
                                    ) * stsxyter::kdClockCycleNs / 4.0;


                  fhSetupSortedDtN2P2->Fill( dDtN2P2 );
                  fhSetupSortedDtN1P1N2P2->Fill(     dDtN1P1N2P2 );
                  fhSetupSortedDtX1Y1X2Y2N2P2->Fill( dDtX1Y1X2Y2N2P2 );
                  fhSetupSortedDtH1H2S1S2->Fill(     dDtH1H2S1S2 );

                  if( fdCoincMinSts2 < dDtN2P2 && dDtN2P2 < fdCoincMaxSts2 )
                  {
                     fhSetupSortedMapN2P2->Fill( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan() );
                     fhSetupSortedCntEvoN2P2->Fill( dTimeSinceStartSec );

                     Double_t dX2, dY2;
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapS2->Fill( dX2, dY2 );

                     if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder )
                     {
                        Double_t dX1, dY1;
                        ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                        fhStsSortedMapS1Coinc->Fill( dX1, dY1 );
                        fhStsSortedMapS2Coinc->Fill( dX2, dY2 );
                     } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
                  } // if( fdCoincMinSts2 < dDtN2P2 && dDtN2P2 < fdCoincMaxSts2 )

                  fvHitsS2P.push_back( (*it) );

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
               if( 0 < usChanIdx )
               {
                  Double_t dDtPrevChan = ( static_cast< Double_t >( (*it).GetTs() ) - static_cast< Double_t >( fvLastHitChanS2P[ usChanIdx - 1].GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS2P->Fill( dDtPrevChan, usChanIdx - 1 );
               } // if( 0 < (*it).GetChan() )
               if( usChanIdx < fuNbChanPerAsic - 1 )
               {
                  Double_t dDtNextChan = ( static_cast< Double_t >( fvLastHitChanS2P[ usChanIdx + 1].GetTs() ) - static_cast< Double_t >( (*it).GetTs() )
                                         ) * stsxyter::kdClockCycleNs;

                  fhDtNeighborChansS2P->Fill( dDtNextChan, usChanIdx );
               } // if( 0 < (*it).GetChan() )

               fvLastHitChanS2P[ usChanIdx ] = (*it);

               Bool_t bFoundCluster = kFALSE;
               for( UInt_t uCluster = 0; uCluster < fvClustersS2P.size(); ++uCluster )
               {
                  if( kTRUE == fvClustersS2P[ uCluster ].CheckAddHit( (*it) ) )
                  {
                     bFoundCluster = kTRUE;
                     break;
                  } // if( kTRUE == fvClustersS2P[ uCluster ].CheckAddHit( (*it) ) )
               } // for( UInt_t uCluster = 0; uCluster < fvClustersS2P.size(); ++uCluster )
               if( kFALSE == bFoundCluster )
               {
                  Cosy2018TestCluster newClust( (*it) );
                  fvClustersS2P.push_back( newClust );
               } // if( kFALSE == bFoundCluster )
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
               } // else if( fUnpackParSts->GetAsicIndexSts1P() == usAsicIdx )
            } // else if( kTRUE == fbDualStsEna )
         } // loop on hits untils hits within 100 ns of last one or last one itself are reached

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
         for( UInt_t uClusterN = 0; uClusterN < fvClustersS1N.size(); ++uClusterN )
         {
            fhClusterAdcVsSizeS1N->Fill( fvClustersS1N[ uClusterN ].fusTotalAdc, fvClustersS1N[ uClusterN ].fvHits.size() );

            UInt_t uNbMatches = 0;
            for( UInt_t uClusterP = 0; uClusterP < fvClustersS1P.size(); ++uClusterP )
            {
               Double_t dDtSts1 = fvClustersS1P[ uClusterP ].fdMeanTime - fvClustersS1N[ uClusterN ].fdMeanTime;
               if( fdCoincMinSts1 < dDtSts1 )
               {
                  if( dDtSts1 < fdCoincMaxSts1 )
                  {
                     // Ignore unphysical pairs
                     if( kFALSE == CheckPhysPairSensor1( fvClustersS1N[ uClusterN ].fdWeightedCenter,
                                                         fvClustersS1P[ uClusterP ].fdWeightedCenter ) )
                        continue;

                     fhClusterAdcVsSizeS1N_MatchS1->Fill( fvClustersS1N[ uClusterN ].fusTotalAdc, fvClustersS1N[ uClusterN ].fvHits.size() );
                     fhClusterAdcVsSizeS1P_MatchS1->Fill( fvClustersS1P[ uClusterP ].fusTotalAdc, fvClustersS1P[ uClusterP ].fvHits.size() );

                     uNbMatches ++;
                  } // if( dDtSts1 < fdCoincMaxSts1 )
               } // if( fdCoincMinSts1 < dDtSts1 )
            } // for( UInt_t uClusterP = 0; uClusterP < fvClustersS1P.size(); ++uClusterP )

            fhClusterS1N_CenterNbMatchS1P->Fill( fvClustersS1N[ uClusterN ].fdWeightedCenter, uNbMatches );
            fhClusterS1N_AdcNbMatchS1P->Fill( fvClustersS1N[ uClusterN ].fusTotalAdc, uNbMatches );
         } // for( UInt_t uClusterN = 0; uClusterN < fvClustersS1N.size(); ++uClusterN )

         for( UInt_t uClusterP = 0; uClusterP < fvClustersS1P.size(); ++uClusterP )
         {
            fhClusterAdcVsSizeS1P->Fill( fvClustersS1P[ uClusterP ].fusTotalAdc, fvClustersS1P[ uClusterP ].fvHits.size() );

            UInt_t uNbMatches = 0;
            for( UInt_t uClusterN = 0; uClusterN < fvClustersS1N.size(); ++uClusterN )
            {
               Double_t dDtSts1 = fvClustersS1P[ uClusterP ].fdMeanTime - fvClustersS1N[ uClusterN ].fdMeanTime;
               if( fdCoincMinSts1 < dDtSts1 )
               {
                  if( dDtSts1 < fdCoincMaxSts1 )
                  {
                     // Ignore unphysical pairs
                     if( kFALSE == CheckPhysPairSensor1( fvClustersS1N[ uClusterN ].fdWeightedCenter,
                                                         fvClustersS1P[ uClusterP ].fdWeightedCenter ) )
                        continue;

                     fhClusterAdcVsSizeS1N_MatchS1->Fill( fvClustersS1N[ uClusterN ].fusTotalAdc, fvClustersS1N[ uClusterN ].fvHits.size() );
                     fhClusterAdcVsSizeS1P_MatchS1->Fill( fvClustersS1P[ uClusterP ].fusTotalAdc, fvClustersS1P[ uClusterP ].fvHits.size() );

                     uNbMatches ++;
                  } // if( dDtSts1 < fdCoincMaxSts1 )
               } // if( fdCoincMinSts1 < dDtSts1 )
            } // for( UInt_t uClusterN = 0; uClusterN < fvClustersS1N.size(); ++uClusterN )

            fhClusterS1P_CenterNbMatchS1N->Fill( fvClustersS1P[ uClusterP ].fdWeightedCenter, uNbMatches );
            fhClusterS1P_AdcNbMatchS1N->Fill( fvClustersS1P[ uClusterP ].fusTotalAdc, uNbMatches );
         } // for( UInt_t uClusterP = 0; uClusterP < fvClustersS1P.size(); ++uClusterP )

         for( UInt_t uClusterN = 0; uClusterN < fvClustersS2N.size(); ++uClusterN )
         {
            fhClusterAdcVsSizeS2N->Fill( fvClustersS2N[ uClusterN ].fusTotalAdc, fvClustersS2N[ uClusterN ].fvHits.size() );

            UInt_t uNbMatches = 0;
            for( UInt_t uClusterP = 0; uClusterP < fvClustersS2P.size(); ++uClusterP )
            {
               Double_t dDtSts2 = fvClustersS2P[ uClusterP ].fdMeanTime - fvClustersS2N[ uClusterN ].fdMeanTime;
               if( fdCoincMinSts2 < dDtSts2 )
               {
                  if( dDtSts2 < fdCoincMaxSts2 )
                  {
                     // Ignore unphysical pairs
                     if( kFALSE == CheckPhysPairSensor1( fvClustersS2N[ uClusterN ].fdWeightedCenter,
                                                         fvClustersS2P[ uClusterP ].fdWeightedCenter ) )
                        continue;

                     fhClusterAdcVsSizeS2N_MatchS2->Fill( fvClustersS2N[ uClusterN ].fusTotalAdc, fvClustersS2N[ uClusterN ].fvHits.size() );
                     fhClusterAdcVsSizeS2P_MatchS2->Fill( fvClustersS2P[ uClusterP ].fusTotalAdc, fvClustersS2P[ uClusterP ].fvHits.size() );

                     uNbMatches ++;
                  } // if( dDtSts2 < fdCoincMaxSts2 )
               } // if( fdCoincMinSts2 < dDtSts2 )
            } // for( UInt_t uClusterP = 0; uClusterP < fvClustersS2P.size(); ++uClusterP )

            fhClusterS2N_CenterNbMatchS2P->Fill( fvClustersS2N[ uClusterN ].fdWeightedCenter, uNbMatches );
            fhClusterS2N_AdcNbMatchS2P->Fill( fvClustersS2N[ uClusterN ].fusTotalAdc, uNbMatches );
         } // for( UInt_t uClusterN = 0; uClusterN < fvClustersS2N.size(); ++uClusterN )

         for( UInt_t uClusterP = 0; uClusterP < fvClustersS2P.size(); ++uClusterP )
         {
            fhClusterAdcVsSizeS2P->Fill( fvClustersS2P[ uClusterP ].fusTotalAdc, fvClustersS2P[ uClusterP ].fvHits.size() );

            UInt_t uNbMatches = 0;
            for( UInt_t uClusterN = 0; uClusterN < fvClustersS2N.size(); ++uClusterN )
            {
               Double_t dDtSts2 = fvClustersS2P[ uClusterP ].fdMeanTime - fvClustersS2N[ uClusterN ].fdMeanTime;
               if( fdCoincMinSts2 < dDtSts2 )
               {
                  if( dDtSts2 < fdCoincMaxSts2 )
                  {
                     // Ignore unphysical pairs
                     if( kFALSE == CheckPhysPairSensor1( fvClustersS2N[ uClusterN ].fdWeightedCenter,
                                                         fvClustersS2P[ uClusterP ].fdWeightedCenter ) )
                        continue;

                     uNbMatches ++;
                  } // if( dDtSts2 < fdCoincMaxSts2 )
               } // if( fdCoincMinSts2 < dDtSts2 )
            } // for( UInt_t uClusterN = 0; uClusterN < fvClustersS2N.size(); ++uClusterN )

            fhClusterS2P_CenterNbMatchS2N->Fill( fvClustersS2P[ uClusterP ].fdWeightedCenter, uNbMatches );
            fhClusterS2P_AdcNbMatchS2N->Fill( fvClustersS2P[ uClusterP ].fusTotalAdc, uNbMatches );
         } // for( UInt_t uCluster = 0; uCluster < fvClustersS2P.size(); ++uCluster )
         fvClustersS1N.clear();
         fvClustersS1P.clear();
         fvClustersS2N.clear();
         fvClustersS2P.clear();


         for( UInt_t uClusterX = 0; uClusterX < fvClustersH1X.size(); ++uClusterX )
         {
            fhClusterAdcVsSizeH1X->Fill( fvClustersH1X[ uClusterX ].fusTotalAdc, fvClustersH1X[ uClusterX ].fvHits.size() );

            UInt_t uNbMatches = 0;
            for( UInt_t uClusterY = 0; uClusterY < fvClustersH1Y.size(); ++uClusterY )
            {
               Double_t dDtHodo1 = fvClustersH1Y[ uClusterY ].fdMeanTime - fvClustersH1X[ uClusterX ].fdMeanTime;
               if( fdCoincMinHodo < dDtHodo1 )
               {
                  if( dDtHodo1 < fdCoincMaxHodo )
                  {
                     UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( fvClustersH1X[ uClusterX ].fdWeightedCenter  );
                     if( fUnpackParHodo->IsXInvertedHodo1() )
                        uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;
                     UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( fvClustersH1Y[ uClusterY ].fdWeightedCenter  );
                     if( fUnpackParHodo->IsYInvertedHodo1() )
                        uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;
                     fhClusterPosition_MatchH1->Fill( uFiberIdxX1, uFiberIdxY1 );

                     uNbMatches ++;
                  } // if( dDtSts1 < fdCoincMaxSts1 )
               } // if( fdCoincMinSts1 < dDtSts1 )
            } // for( UInt_t uClusterY = 0; uClusterY < fvClustersH1Y.size(); ++uClusterY )
         } // for( UInt_t uClusterX = 0; uClusterX < fvClustersH1X.size(); ++uClusterX )

         for( UInt_t uClusterY = 0; uClusterY < fvClustersH1Y.size(); ++uClusterY )
         {
            fhClusterAdcVsSizeH1Y->Fill( fvClustersH1Y[ uClusterY ].fusTotalAdc, fvClustersH1Y[ uClusterY ].fvHits.size() );
         } // for( UInt_t uClusterY = 0; uClusterY < fvClustersH1Y.size(); ++uClusterY )

         for( UInt_t uClusterX = 0; uClusterX < fvClustersH2X.size(); ++uClusterX )
         {
            fhClusterAdcVsSizeH2X->Fill( fvClustersH2X[ uClusterX ].fusTotalAdc, fvClustersH2X[ uClusterX ].fvHits.size() );

            UInt_t uNbMatches = 0;
            for( UInt_t uClusterY = 0; uClusterY < fvClustersH2Y.size(); ++uClusterY )
            {
               Double_t dDtHodo2 = fvClustersH2Y[ uClusterY ].fdMeanTime - fvClustersH2X[ uClusterX ].fdMeanTime;
               if( fdCoincMinHodo < dDtHodo2 )
               {
                  if( dDtHodo2 < fdCoincMaxHodo )
                  {
                     UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( fvClustersH2X[ uClusterX ].fdWeightedCenter  );
                     if( fUnpackParHodo->IsXInvertedHodo2() )
                        uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;
                     UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( fvClustersH2Y[ uClusterY ].fdWeightedCenter  );
                     if( fUnpackParHodo->IsYInvertedHodo2() )
                        uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;
                     fhClusterPosition_MatchH2->Fill( uFiberIdxX2, uFiberIdxY2 );

                     uNbMatches ++;
                  } // if( dDtSts2 < fdCoincMaxSts2 )
               } // if( fdCoincMinSts2 < dDtSts2 )
            } // for( UInt_t uClusterY = 0; uClusterY < fvClustersH2Y.size(); ++uClusterY )
         } // for( UInt_t uClusterX = 0; uClusterX < fvClustersH2X.size(); ++uClusterX )

         for( UInt_t uClusterY = 0; uClusterY < fvClustersH2Y.size(); ++uClusterY )
         {
            fhClusterAdcVsSizeH2Y->Fill( fvClustersH2Y[ uClusterY ].fusTotalAdc, fvClustersH2Y[ uClusterY ].fvHits.size() );
         } // for( UInt_t uCluster = 0; uCluster < fvClustersH2Y.size(); ++uCluster )

         fvClustersH1X.clear();
         fvClustersH1Y.clear();
         fvClustersH2X.clear();
         fvClustersH2Y.clear();
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

         // Remove all hits which were already used
         fvmHitsInTs.erase( fvmHitsInTs.begin(), it );

         /// Build coincidence plot for all N1P1 pairs and all N2P2 pairs
         for( UInt_t uHitN1 = 0; uHitN1 < fvHitsS1N.size(); ++uHitN1 )
         {
            Double_t dDtBestPair = 1e12;
//            UInt_t   uBestAdc    = 0;
            UInt_t uIdxBest = fvHitsS1P.size();
            for( UInt_t uHitP1 = 0; uHitP1 < fvHitsS1P.size(); ++uHitP1 )
            {
               Double_t dDtSts1 = (  static_cast< Double_t >( fvHitsS1P[ uHitP1 ].GetTs() )
                                   - static_cast< Double_t >( fvHitsS1N[ uHitN1 ].GetTs() )
                                  ) * stsxyter::kdClockCycleNs;

               fhDtAllPairsS1->Fill( dDtSts1 );
               if( fdCoincMinSts1 < dDtSts1 && dDtSts1 < fdCoincMaxSts1 )
               {
                  fhChanMapAllMatchS1->Fill( fvHitsS1N[ uHitN1 ].GetChan(), fvHitsS1P[ uHitP1 ].GetChan() );
                  if( TMath::Abs( dDtSts1 - fdCoincCenterSts1 ) < dDtBestPair )
                  {
                     uIdxBest = uHitP1;
                     dDtBestPair = TMath::Abs( dDtSts1 - fdCoincCenterSts1 );
                  } // if( TMath::Abs( dDtSts1 - fdCoincCenterSts1 ) < dDtBestPair )
/*
                  if( uBestAdc < fvHitsS1P[ uHitP1 ].GetAdc() )
                  {
                     uIdxBest = uHitP1;
                     uBestAdc = fvHitsS1P[ uHitP1 ].GetAdc();
                  } // if( uBestAdc < fvHitsS1P[ uHitP1 ].GetAdc() )
*/
               }// if( fdCoincMinSts1 < dDtSts1 && dDtSts1 < fdCoincMaxSts1 )
            } // for( UInt_t uHitP1 = 0; uHitP1 < fvHitsS1P.size(); ++uHitP1 )

            if( fvHitsS1P.size() != uIdxBest )
               fhChanMapBestMatchS1->Fill( fvHitsS1N[ uHitN1 ].GetChan(), fvHitsS1P[ uIdxBest ].GetChan() );
         } // for( UInt_t uHitN1 = 0; uHitN1 < fvHitsS1N.size(); ++uHitN1 )
         for( UInt_t uHitN2 = 0; uHitN2 < fvHitsS2N.size(); ++uHitN2 )
         {
            Double_t dDtBestPair = 1e12;
//            UInt_t   uBestAdc    = 0;
            UInt_t uIdxBest = fvHitsS2P.size();
            for( UInt_t uHitP2 = 0; uHitP2 < fvHitsS2P.size(); ++uHitP2 )
            {
               Double_t dDtSts2 = (  static_cast< Double_t >( fvHitsS2P[ uHitP2 ].GetTs() )
                                   - static_cast< Double_t >( fvHitsS2N[ uHitN2 ].GetTs() )
                                  ) * stsxyter::kdClockCycleNs;

               fhDtAllPairsS2->Fill( dDtSts2 );
               if( fdCoincMinSts2 < dDtSts2 && dDtSts2 < fdCoincMaxSts2 )
               {
                  fhChanMapAllMatchS2->Fill( fvHitsS2N[ uHitN2 ].GetChan(), fvHitsS2P[ uHitP2 ].GetChan() );

                  if( TMath::Abs( dDtSts2 - fdCoincCenterSts2 ) < dDtBestPair )
                  {
                     uIdxBest = uHitP2;
                     dDtBestPair = TMath::Abs( dDtSts2 - fdCoincCenterSts2 );
                  } // if( TMath::Abs( dDtSts2 - fdCoincCenterSts2 ) < dDtBestPair )
/*
                  if( uBestAdc < fvHitsS2P[ uHitP2 ].GetAdc() )
                  {
                     uIdxBest = uHitP2;
                     uBestAdc = fvHitsS2P[ uHitP2 ].GetAdc();
                  } // if( uBestAdc < fvHitsS1P[ uHitP1 ].GetAdc() )
*/
               }// if( fdCoincMinSts2 < dDtSts2 && dDtSts2 < fdCoincMaxSts2 )
            } // for( UInt_t uHitP2 = 0; uHitP2 < fvHitsS2P.size(); ++uHitP2 )

            if( fvHitsS2P.size() != uIdxBest )
               fhChanMapBestMatchS2->Fill( fvHitsS2N[ uHitN2 ].GetChan(), fvHitsS2P[ uIdxBest ].GetChan() );
         } // for( UInt_t uHitN2 = 0; uHitN2 < fvHitsS2N.size(); ++uHitN2 )

         /// Build Hodoscope pairs (~tracks)
         UInt_t uFirstMatchH2 = 0;
         for( UInt_t uPairH1 = 0; uPairH1 < fvPairsH1.size(); ++uPairH1 )
         {
            UInt_t uNbH2CoincPerH1 = 0;
//            Double_t dDtHodo = 1e12;
            Double_t dDtBestPair = 1e12;
            UInt_t uBestH2 = fvPairsH1.size();

            for( UInt_t uPairH2 = uFirstMatchH2; uPairH2 < fvPairsH2.size(); ++uPairH2 )
            {
               Double_t dDt = (  static_cast< Double_t >( fvPairsH2[ uPairH2 ].first.GetTs() )
                               + static_cast< Double_t >( fvPairsH2[ uPairH2 ].second.GetTs() )
                               - static_cast< Double_t >( fvPairsH1[ uPairH1 ].first.GetTs() )
                               - static_cast< Double_t >( fvPairsH1[ uPairH1 ].second.GetTs() )
                              ) * stsxyter::kdClockCycleNs / 2.0;

               if( fdCoincMinHodoBoth < dDt )
               {
                  if( dDt < fdCoincMaxHodoBoth )
                  {
                     uNbH2CoincPerH1 ++;
                     if( TMath::Abs( dDt - fdCoincCenterHodo ) < dDtBestPair )
                     {
                        uBestH2 = uPairH2;
                        dDtBestPair = TMath::Abs( dDt - fdCoincCenterHodo );
                     } // if( TMath::Abs( dDt - fdCoincCenterHodo ) < dDtBestPair )
                  } // if( dDt < fdCoincMaxHodoBoth )
                     else break;
               } // if( fdCoincMinHodoBoth < dDt )
                  else uFirstMatchH2 = uPairH2;
            } // for( UInt_t uPairH2 = uFirstMatchH2; uPairH2 < fvPairsH2.size(); ++uPairH2 )
            if( 0 < uNbH2CoincPerH1 )
            {
               fvPairsHodo.push_back( std::pair< UInt_t, UInt_t >( uPairH1, uBestH2 ) );

               fhDtBestPairsHodo->Fill( (  static_cast< Double_t >( fvPairsH2[ uBestH2 ].first.GetTs() )
                                         + static_cast< Double_t >( fvPairsH2[ uBestH2 ].second.GetTs() )
                                         - static_cast< Double_t >( fvPairsH1[ uPairH1 ].first.GetTs() )
                                         - static_cast< Double_t >( fvPairsH1[ uPairH1 ].second.GetTs() )
                                        ) * stsxyter::kdClockCycleNs / 2.0 );

               Double_t dPosX, dPosY;
               ComputeCoordinatesHodo1( fvPairsH1[ uPairH1 ].first.GetChan(), fvPairsH1[ uPairH1 ].second.GetChan(), dPosX, dPosY );
               fhMapBestPairsHodo_H1->Fill( dPosX, dPosY );

               ComputeCoordinatesHodo2( fvPairsH2[ uBestH2 ].first.GetChan(), fvPairsH2[ uBestH2 ].second.GetChan(), dPosX, dPosY );
               fhMapBestPairsHodo_H2->Fill( dPosX, dPosY );
            } // if( 0 < uNbH2CoincPerH1 )
            fhNbH2CoincPerH1->Fill( uNbH2CoincPerH1 );
         } // for( UInt_t uPairH1 = 0; uPairH1 < fvPairsH1.size(); ++uPairH1 )

         /// Loop on Hodoscope Pairs and check for each if a sensor pair is matching
         UInt_t uFirstMatchS1 = 0;
         UInt_t uFirstMatchS2 = 0;
//         UInt_t uFirstMatchS1S2 = 0;
//         UInt_t uFirstMatchS2S1 = 0;
         UInt_t uFirstMatchS1N = 0;
         UInt_t uFirstMatchS1P = 0;
         UInt_t uFirstMatchS2N = 0;
         UInt_t uFirstMatchS2P = 0;
         for( UInt_t uPairHodo = 0; uPairHodo < fvPairsHodo.size(); ++uPairHodo )
         {
            /// Compute Hodo pair coordinates
            Double_t dPosXH1, dPosYH1;
            ComputeCoordinatesHodo1( fvPairsH1[ fvPairsHodo[ uPairHodo ].first ].first.GetChan(),
                                     fvPairsH1[ fvPairsHodo[ uPairHodo ].first ].second.GetChan(),
                                     dPosXH1, dPosYH1 );

            Double_t dPosXH2, dPosYH2;
            ComputeCoordinatesHodo2( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetChan(),
                                     fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetChan(),
                                     dPosXH2, dPosYH2 );

            /// Check if projection within STS 1 active area
            Double_t dProjXS1, dProjYS1;
            ComputeProjectionSensor1( dPosXH1, dPosYH1, dPosXH2, dPosYH2,
                                      dProjXS1, dProjYS1 );
            Bool_t bHodoTrkInS1 = ( kTRUE == fbDeadCorrEna ?
                                    CheckPointInsideSensor1NoDead( dProjXS1 - fdOffsetS1X, dProjYS1 - fdOffsetS1Y) :
                                    CheckPointInsideSensor1( dProjXS1 - fdOffsetS1X, dProjYS1 - fdOffsetS1Y) );
            fhMapBestPairsHodo_ProjS1->Fill( dProjXS1, dProjYS1 );

            /// Check if projection within STS 2 active area
            Double_t dProjXS2, dProjYS2;
            ComputeProjectionSensor2( dPosXH1, dPosYH1, dPosXH2, dPosYH2,
                                      dProjXS2, dProjYS2 );
            Bool_t bHodoTrkInS2 = ( kTRUE == fbDeadCorrEna ?
                                    CheckPointInsideSensor2NoDead( dProjXS2 - fdOffsetS2X, dProjYS2 - fdOffsetS2Y):
                                    CheckPointInsideSensor2( dProjXS2 - fdOffsetS2X, dProjYS2 - fdOffsetS2Y) );
            fhMapBestPairsHodo_ProjS2->Fill( dProjXS2, dProjYS2 );

            /// Test coincidences with the P or N sides of the sensors
            Double_t dDt_Side;
            Bool_t bTestCoincFound = kFALSE;
               /// S1N
            for( UInt_t uHitS1N = uFirstMatchS1N; uHitS1N < fvHitsS1N.size(); ++uHitS1N )
            {
               dDt_Side = (  static_cast< Double_t >( fvHitsS1N[ uHitS1N ].GetTs() )
                      - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                        ) / 4.0
                     ) * stsxyter::kdClockCycleNs;
               if( fdCoincMinHodoS1 < dDt_Side )
               {
                  if( dDt_Side < fdCoincMaxHodoS1 )
                  {
                     bTestCoincFound = kTRUE;
                     break;
                  } // if( dDt_Side < fdCoincMaxHodoS1 )
                     else break;
               } // if( fdCoincMinHodoS1 < dDt_Side )
                  else uFirstMatchS1N = uHitS1N;
            } // for( UInt uHitS1N = uFirstMatchS1N; uHitS1N < fvHitsS1N.size(); ++uHitS1N )
            if( kTRUE == bTestCoincFound )
               fhTestMapHodoS1N->Fill( dProjXS1, dProjYS1, 1.0 );
               else fhTestMapHodoS1N->Fill( dProjXS1, dProjYS1, 0.0 );
               /// S1P
            bTestCoincFound = kFALSE;
            for( UInt_t uHitS1P = uFirstMatchS1P; uHitS1P < fvHitsS1P.size(); ++uHitS1P )
            {
               dDt_Side = (  static_cast< Double_t >( fvHitsS1P[ uHitS1P ].GetTs() )
                      - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                        ) / 4.0
                     ) * stsxyter::kdClockCycleNs;
               if( fdCoincMinHodoS1 < dDt_Side )
               {
                  if( dDt_Side < fdCoincMaxHodoS1 )
                  {
                     bTestCoincFound = kTRUE;
                     break;
                  } // if( dDt_Side < fdCoincMaxHodoS1 )
                     else break;
               } // if( fdCoincMinHodoS1 < dDt_Side )
                  else uFirstMatchS1P = uHitS1P;
            } // for( UInt uHitS1P = uFirstMatchS1P; uHitS1P < fvHitsS1P.size(); ++uHitS1P )
            if( kTRUE == bTestCoincFound )
               fhTestMapHodoS1P->Fill( dProjXS1, dProjYS1, 1.0 );
               else fhTestMapHodoS1P->Fill( dProjXS1, dProjYS1, 0.0 );
               /// S2N
            bTestCoincFound = kFALSE;
            for( UInt_t uHitS2N = uFirstMatchS2N; uHitS2N < fvHitsS2N.size(); ++uHitS2N )
            {
               dDt_Side = (  static_cast< Double_t >( fvHitsS2N[ uHitS2N ].GetTs() )
                      - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                        ) / 4.0
                     ) * stsxyter::kdClockCycleNs;
               if( fdCoincMinHodoS2 < dDt_Side )
               {
                  if( dDt_Side < fdCoincMaxHodoS2 )
                  {
                     bTestCoincFound = kTRUE;
                     break;
                  } // if( dDt_Side < fdCoincMaxHodoS2 )
                     else break;
               } // if( fdCoincMinHodoS2 < dDt_Side )
                  else uFirstMatchS2N = uHitS2N;
            } // for( UInt uHitS2N = uFirstMatchS2N; uHitS2N < fvHitsS2N.size(); ++uHitS2N )
            if( kTRUE == bTestCoincFound )
               fhTestMapHodoS2N->Fill( dProjXS1, dProjYS1, 1.0 );
               else fhTestMapHodoS2N->Fill( dProjXS1, dProjYS1, 0.0 );
               /// S2P
            bTestCoincFound = kFALSE;
            for( UInt_t uHitS2P = uFirstMatchS2P; uHitS2P < fvHitsS2P.size(); ++uHitS2P )
            {
               dDt_Side = (  static_cast< Double_t >( fvHitsS2P[ uHitS2P ].GetTs() )
                      - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                        ) / 4.0
                     ) * stsxyter::kdClockCycleNs;
               if( fdCoincMinHodoS2 < dDt_Side )
               {
                  if( dDt_Side < fdCoincMaxHodoS2 )
                  {
                     bTestCoincFound = kTRUE;
                     break;
                  } // if( dDt_Side < fdCoincMaxHodoS2 )
                     else break;
               } // if( fdCoincMinHodoS2 < dDt_Side )
                  else uFirstMatchS2P = uHitS2P;
            } // for( UInt uHitS2P = uFirstMatchS2P; uHitS2P < fvHitsS2P.size(); ++uHitS2P )
            if( kTRUE == bTestCoincFound )
               fhTestMapHodoS2P->Fill( dProjXS1, dProjYS1, 1.0 );
               else fhTestMapHodoS2P->Fill( dProjXS1, dProjYS1, 0.0 );

            /// Coincidences with S1
            UInt_t uNbS1CoincPerHodo = 0;
//            Double_t dDtS1 = 1e12;
            Double_t dDtBestPairS1 = 1e12;
            UInt_t uBestS1 = fvPairsS1.size();
            for( UInt_t uPairS1 = uFirstMatchS1; uPairS1 < fvPairsS1.size(); ++uPairS1 )
            {
               Double_t dDt = (  (  static_cast< Double_t >( fvPairsS1[ uPairS1 ].first.GetTs() )
                                  + static_cast< Double_t >( fvPairsS1[ uPairS1 ].second.GetTs() )
                                 ) / 2.0
                               - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                  + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                  + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                  + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                 ) / 4.0
                              ) * stsxyter::kdClockCycleNs;

               fhDtAllPairsHodoS1->Fill( dDt );
               if( fdCoincMinHodoS1 < dDt )
               {
                  if( dDt < fdCoincMaxHodoS1 )
                  {
                     uNbS1CoincPerHodo ++;
                     if( TMath::Abs( dDt - fdCoincCenter ) < dDtBestPairS1 )
                     {
                        uBestS1 = uPairS1;
                        dDtBestPairS1 = TMath::Abs( dDt - fdCoincCenter );
                     } // if( TMath::Abs( dDt - fdCoincCenter ) < dDtBestPairS1 )
                  } // if( dDt < fdCoincMaxHodoS1 )
//                     else break;
               } // if( fdCoincMinHodoS1 < dDt )
//                  else uFirstMatchS1 = uPairS1;
            } // for( UInt_t uPairS1 = uFirstMatchS1; uPairS1 < fvPairsS1.size(); ++uPairS1 )

            /// Coincidences with S2
            UInt_t uNbS2CoincPerHodo = 0;
//            Double_t dDtS2 = 1e12;
            Double_t dDtBestPairS2 = 1e12;
            UInt_t uBestS2 = fvPairsS2.size();
            for( UInt_t uPairS2 = uFirstMatchS2; uPairS2 < fvPairsS2.size(); ++uPairS2 )
            {
               Double_t dDt = (  (  static_cast< Double_t >( fvPairsS2[ uPairS2 ].first.GetTs() )
                                  + static_cast< Double_t >( fvPairsS2[ uPairS2 ].second.GetTs() )
                                 ) / 2.0
                               - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                  + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                  + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                  + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                 ) / 4.0
                              ) * stsxyter::kdClockCycleNs;

               fhDtAllPairsHodoS2->Fill( dDt );
               if( fdCoincMinHodoS2 < dDt )
               {
                  if( dDt < fdCoincMaxHodoS2 )
                  {
                     uNbS2CoincPerHodo ++;
                     if( TMath::Abs( dDt - fdCoincCenter ) < dDtBestPairS2 )
                     {
                        uBestS2 = uPairS2;
                        dDtBestPairS2 = TMath::Abs( dDt - fdCoincCenter );
                     } // if( TMath::Abs( dDt - fdCoincCenter ) < dDtBestPairS2 )
                  } // if( dDt < fdCoincMaxHodoS2 )
//                     else break;
               } // if( fdCoincMinHodoS2 < dDt )
//                  else uFirstMatchS2 = uPairS2;
            } // for( UInt_t uPairS2 = uFirstMatchS2; uPairS2 < fvPairsS2.size(); ++uPairS2 )

            /// Histos filling
               /// Without Geo cut
            fhNbS1CoincPerHodo->Fill( uNbS1CoincPerHodo );
            if( 0 < uNbS1CoincPerHodo )
            {
               fvPairsHodoS1.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS1 ) );

               fhDtBestPairsHodoS1->Fill( (  (  static_cast< Double_t >( fvPairsS1[ uBestS1 ].first.GetTs() )
                                              + static_cast< Double_t >( fvPairsS1[ uBestS1 ].second.GetTs() )
                                             ) / 2.0
                                           - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                              + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                              + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                              + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                             ) / 4.0
                                          ) * stsxyter::kdClockCycleNs );

               fhMapBestPairsHodoS1_H1->Fill( dPosXH1, dPosYH1 );
               fhMapBestPairsHodoS1_H2->Fill( dPosXH2, dPosYH2 );

               Double_t dPosXS1, dPosYS1;
               ComputeCoordinatesSensor1( fvPairsS1[ uBestS1 ].first.GetChan(),
                                          fvPairsS1[ uBestS1 ].second.GetChan(),
                                          dPosXS1, dPosYS1 );
               fhMapBestPairsHodoS1_S1->Fill( dPosXS1, dPosYS1 );
               // Apply Position Offsets
               dPosXS1 += fdOffsetS1X;
               dPosYS1 += fdOffsetS1Y;

               fhMapBestPairsHodoS1_Proj->Fill( dProjXS1, dProjYS1 );
               fhResidualsBestPairsHodoS1->Fill( dPosXS1 - dProjXS1, dPosYS1 - dProjYS1 );

               fhAdcRawBestPairsHodoS1N->Fill( fvPairsS1[ uBestS1 ].first.GetChan(), fvPairsS1[ uBestS1 ].first.GetAdc() );
               fhAdcRawBestPairsHodoS1P->Fill( fvPairsS1[ uBestS1 ].second.GetChan(), fvPairsS1[ uBestS1 ].second.GetAdc() );

               /// Check for coincidences with S2 knowing that at least 1 matching S1 hit was found
               fhNbS2CoincPerHodoS1->Fill( uNbS2CoincPerHodo );
               if( 0 < uNbS2CoincPerHodo )
               {
                  fvPairsHodoS2S1.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS2 ) );

                  fhDtBestPairsHodoS2S1->Fill( (  (  static_cast< Double_t >( fvPairsS2[ uBestS2 ].first.GetTs() )
                                                   + static_cast< Double_t >( fvPairsS2[ uBestS2 ].second.GetTs() )
                                                  ) / 2.0
                                                - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                                   + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                                   + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                                   + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                                  ) / 4.0
                                               ) * stsxyter::kdClockCycleNs );

                  fhMapBestPairsHodoS2S1_H1->Fill( dPosXH1, dPosYH1 );
                  fhMapBestPairsHodoS2S1_H2->Fill( dPosXH2, dPosYH2 );

                  Double_t dPosXS2, dPosYS2;
                  ComputeCoordinatesSensor2( fvPairsS2[ uBestS2 ].first.GetChan(),
                                             fvPairsS2[ uBestS2 ].second.GetChan(),
                                             dPosXS2, dPosYS2 );
                  fhMapBestPairsHodoS2S1_S2->Fill( dPosXS2, dPosYS2 );
                  // Apply Position Offsets
                  dPosXS2 += fdOffsetS2X;
                  dPosYS2 += fdOffsetS2Y;


                  fhMapBestPairsHodoS2S1_Proj->Fill( dProjXS2, dProjYS2 );
                  fhResidualsBestPairsHodoS2S1->Fill( dPosXS2 - dProjXS2, dPosYS2 - dProjYS2 );

                  fhAdcRawBestPairsHodoS2S1N->Fill( fvPairsS2[ uBestS2 ].first.GetChan(), fvPairsS2[ uBestS2 ].first.GetAdc() );
                  fhAdcRawBestPairsHodoS2S1P->Fill( fvPairsS2[ uBestS2 ].second.GetChan(), fvPairsS2[ uBestS2 ].second.GetAdc() );
               } // if( 0 < uNbS2CoincPerHodo )
            } // if( 0 < uNbS1CoincPerHodo )

            fhNbS2CoincPerHodo->Fill( uNbS2CoincPerHodo );
            if( 0 < uNbS2CoincPerHodo )
            {
               fvPairsHodoS2.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS2 ) );

               fhDtBestPairsHodoS2->Fill( (  (  static_cast< Double_t >( fvPairsS2[ uBestS2 ].first.GetTs() )
                                              + static_cast< Double_t >( fvPairsS2[ uBestS2 ].second.GetTs() )
                                             ) / 2.0
                                           - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                              + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                              + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                              + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                             ) / 4.0
                                          ) * stsxyter::kdClockCycleNs );

               fhMapBestPairsHodoS2_H1->Fill( dPosXH1, dPosYH1 );
               fhMapBestPairsHodoS2_H2->Fill( dPosXH2, dPosYH2 );

               Double_t dPosXS2, dPosYS2;
               ComputeCoordinatesSensor2( fvPairsS2[ uBestS2 ].first.GetChan(),
                                          fvPairsS2[ uBestS2 ].second.GetChan(),
                                          dPosXS2, dPosYS2 );
               fhMapBestPairsHodoS2_S2->Fill( dPosXS2, dPosYS2 );
               // Apply Position Offsets
               dPosXS2 += fdOffsetS2X;
               dPosYS2 += fdOffsetS2Y;

               fhMapBestPairsHodoS2_Proj->Fill( dProjXS2, dProjYS2 );
               fhResidualsBestPairsHodoS2->Fill( dPosXS2 - dProjXS2, dPosYS2 - dProjYS2 );

               fhAdcRawBestPairsHodoS2N->Fill( fvPairsS2[ uBestS2 ].first.GetChan(), fvPairsS2[ uBestS2 ].first.GetAdc() );
               fhAdcRawBestPairsHodoS2P->Fill( fvPairsS2[ uBestS2 ].second.GetChan(), fvPairsS2[ uBestS2 ].second.GetAdc() );

               /// Check for coincidences with S1 knowing that at least 1 matching S2 hit was found
               fhNbS1CoincPerHodoS2->Fill( uNbS1CoincPerHodo );
               if( 0 < uNbS1CoincPerHodo )
               {
                  fvPairsHodoS1S2.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS1 ) );

                  fhDtBestPairsHodoS1S2->Fill( (  (  static_cast< Double_t >( fvPairsS1[ uBestS1 ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsS1[ uBestS1 ].second.GetTs() )
                                                ) / 2.0
                                              - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                                ) / 4.0
                                             ) * stsxyter::kdClockCycleNs );

                  fhMapBestPairsHodoS1S2_H1->Fill( dPosXH1, dPosYH1 );
                  fhMapBestPairsHodoS1S2_H2->Fill( dPosXH2, dPosYH2 );

                  Double_t dPosXS1, dPosYS1;
                  ComputeCoordinatesSensor1( fvPairsS1[ uBestS1 ].first.GetChan(),
                                             fvPairsS1[ uBestS1 ].second.GetChan(),
                                             dPosXS1, dPosYS1 );
                  fhMapBestPairsHodoS1S2_S1->Fill( dPosXS1, dPosYS1 );
                  // Apply Position Offsets
                  dPosXS1 += fdOffsetS1X;
                  dPosYS1 += fdOffsetS1Y;

                  fhMapBestPairsHodoS1S2_Proj->Fill( dProjXS1, dProjYS1 );
                  fhResidualsBestPairsHodoS1S2->Fill( dPosXS1 - dProjXS1, dPosYS1 - dProjYS1 );

                  fhAdcRawBestPairsHodoS1S2N->Fill( fvPairsS1[ uBestS1 ].first.GetChan(), fvPairsS1[ uBestS1 ].first.GetAdc() );
                  fhAdcRawBestPairsHodoS1S2P->Fill( fvPairsS1[ uBestS1 ].second.GetChan(), fvPairsS1[ uBestS1 ].second.GetAdc() );
               } // if( 0 < uNbS1CoincPerHodo )
            } // if( 0 < uNbS2CoincPerHodo )
               /// With Geo cut
            if( bHodoTrkInS1 )
            {
               fhMapBestPairsHodoGeoCutS1_H1->Fill( dPosXH1, dPosYH1 );
               fhMapBestPairsHodoGeoCutS1_H2->Fill( dPosXH2, dPosYH2 );
               fhMapBestPairsHodoGeoCutS1_Proj->Fill( dProjXS1, dProjYS1 );

               fhNbS1CoincPerHodoGeoCut->Fill( uNbS1CoincPerHodo );
               if( 0 < uNbS1CoincPerHodo )
               {
                  fvPairsHodoS1.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS1 ) );

                  fhDtBestPairsHodoS1GeoCut->Fill( (  (  static_cast< Double_t >( fvPairsS1[ uBestS1 ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsS1[ uBestS1 ].second.GetTs() )
                                                ) / 2.0
                                              - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                                ) / 4.0
                                             ) * stsxyter::kdClockCycleNs );

                  fhMapBestPairsHodoS1GeoCut_H1->Fill( dPosXH1, dPosYH1 );
                  fhMapBestPairsHodoS1GeoCut_H2->Fill( dPosXH2, dPosYH2 );

                  Double_t dPosXS1, dPosYS1;
                  ComputeCoordinatesSensor1( fvPairsS1[ uBestS1 ].first.GetChan(),
                                             fvPairsS1[ uBestS1 ].second.GetChan(),
                                             dPosXS1, dPosYS1 );
                  fhMapBestPairsHodoS1GeoCut_S1->Fill( dPosXS1, dPosYS1 );
                  // Apply Position Offsets
                  dPosXS1 += fdOffsetS1X;
                  dPosYS1 += fdOffsetS1Y;

                  fhMapBestPairsHodoS1GeoCut_Proj->Fill( dProjXS1, dProjYS1 );
                  fhResidualsBestPairsHodoS1GeoCut->Fill( dPosXS1 - dProjXS1, dPosYS1 - dProjYS1 );

                  fhAdcRawBestPairsHodoS1N->Fill( fvPairsS1[ uBestS1 ].first.GetChan(), fvPairsS1[ uBestS1 ].first.GetAdc() );
                  fhAdcRawBestPairsHodoS1P->Fill( fvPairsS1[ uBestS1 ].second.GetChan(), fvPairsS1[ uBestS1 ].second.GetAdc() );

                  if( bHodoTrkInS2 )
                  {
                     /// Check for coincidences with S2 knowing that at least 1 matching S1 hit was found
                     fhNbS2CoincPerHodoS1GeoCut->Fill( uNbS2CoincPerHodo );
                     if( 0 < uNbS2CoincPerHodo )
                     {
                        fvPairsHodoS2S1.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS2 ) );

                        fhDtBestPairsHodoS2S1GeoCut->Fill( (  (  static_cast< Double_t >( fvPairsS2[ uBestS2 ].first.GetTs() )
                                                         + static_cast< Double_t >( fvPairsS2[ uBestS2 ].second.GetTs() )
                                                        ) / 2.0
                                                      - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                                         + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                                         + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                                        ) / 4.0
                                                     ) * stsxyter::kdClockCycleNs );

                        fhMapBestPairsHodoS2S1GeoCut_H1->Fill( dPosXH1, dPosYH1 );
                        fhMapBestPairsHodoS2S1GeoCut_H2->Fill( dPosXH2, dPosYH2 );

                        Double_t dPosXS2, dPosYS2;
                        ComputeCoordinatesSensor2( fvPairsS2[ uBestS2 ].first.GetChan(),
                                                   fvPairsS2[ uBestS2 ].second.GetChan(),
                                                   dPosXS2, dPosYS2 );
                        fhMapBestPairsHodoS2S1GeoCut_S2->Fill( dPosXS2, dPosYS2 );
                        // Apply Position Offsets
                        dPosXS2 += fdOffsetS2X;
                        dPosYS2 += fdOffsetS2Y;


                        fhMapBestPairsHodoS2S1GeoCut_Proj->Fill( dProjXS2, dProjYS2 );
                        fhResidualsBestPairsHodoS2S1GeoCut->Fill( dPosXS2 - dProjXS2, dPosYS2 - dProjYS2 );

                        fhAdcRawBestPairsHodoS2S1N->Fill( fvPairsS2[ uBestS2 ].first.GetChan(), fvPairsS2[ uBestS2 ].first.GetAdc() );
                        fhAdcRawBestPairsHodoS2S1P->Fill( fvPairsS2[ uBestS2 ].second.GetChan(), fvPairsS2[ uBestS2 ].second.GetAdc() );
                     } // if( 0 < uNbS2CoincPerHodo )
                  } // if( bHodoTrkInS2 )
               } // if( 0 < uNbS1CoincPerHodo )
            } // if( bHodoTrkInS1 )

            if( bHodoTrkInS2 )
            {
               fhMapBestPairsHodoGeoCutS2_H1->Fill( dPosXH1, dPosYH1 );
               fhMapBestPairsHodoGeoCutS2_H2->Fill( dPosXH2, dPosYH2 );
               fhMapBestPairsHodoGeoCutS2_Proj->Fill( dProjXS2, dProjYS2 );

               fhNbS2CoincPerHodoGeoCut->Fill( uNbS2CoincPerHodo );
               if( 0 < uNbS2CoincPerHodo )
               {
                  fvPairsHodoS2.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS2 ) );

                  fhDtBestPairsHodoS2GeoCut->Fill( (  (  static_cast< Double_t >( fvPairsS2[ uBestS2 ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsS2[ uBestS2 ].second.GetTs() )
                                                ) / 2.0
                                              - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                                 + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                                ) / 4.0
                                             ) * stsxyter::kdClockCycleNs );

                  fhMapBestPairsHodoS2GeoCut_H1->Fill( dPosXH1, dPosYH1 );
                  fhMapBestPairsHodoS2GeoCut_H2->Fill( dPosXH2, dPosYH2 );

                  Double_t dPosXS2, dPosYS2;
                  ComputeCoordinatesSensor2( fvPairsS2[ uBestS2 ].first.GetChan(),
                                             fvPairsS2[ uBestS2 ].second.GetChan(),
                                             dPosXS2, dPosYS2 );
                  fhMapBestPairsHodoS2GeoCut_S2->Fill( dPosXS2, dPosYS2 );
                  // Apply Position Offsets
                  dPosXS2 += fdOffsetS2X;
                  dPosYS2 += fdOffsetS2Y;

                  fhMapBestPairsHodoS2GeoCut_Proj->Fill( dProjXS2, dProjYS2 );
                  fhResidualsBestPairsHodoS2GeoCut->Fill( dPosXS2 - dProjXS2, dPosYS2 - dProjYS2 );

                  fhAdcRawBestPairsHodoS2N->Fill( fvPairsS2[ uBestS2 ].first.GetChan(), fvPairsS2[ uBestS2 ].first.GetAdc() );
                  fhAdcRawBestPairsHodoS2P->Fill( fvPairsS2[ uBestS2 ].second.GetChan(), fvPairsS2[ uBestS2 ].second.GetAdc() );

                  if( bHodoTrkInS1 )
                  {
                     /// Check for coincidences with S1 knowing that at least 1 matching S2 hit was found
                     fhNbS1CoincPerHodoS2GeoCut->Fill( uNbS1CoincPerHodo );
                     if( 0 < uNbS1CoincPerHodo )
                     {
                        fvPairsHodoS1S2.push_back( std::pair< UInt_t, UInt_t >( uPairHodo, uBestS1 ) );

                        fhDtBestPairsHodoS1S2GeoCut->Fill( (  (  static_cast< Double_t >( fvPairsS1[ uBestS1 ].first.GetTs() )
                                                       + static_cast< Double_t >( fvPairsS1[ uBestS1 ].second.GetTs() )
                                                      ) / 2.0
                                                    - (  static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].first.GetTs() )
                                                       + static_cast< Double_t >( fvPairsH1[ fvPairsHodo[ uPairHodo ].first  ].second.GetTs() )
                                                       + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].first.GetTs() )
                                                       + static_cast< Double_t >( fvPairsH2[ fvPairsHodo[ uPairHodo ].second ].second.GetTs() )
                                                      ) / 4.0
                                                   ) * stsxyter::kdClockCycleNs );

                        fhMapBestPairsHodoS1S2GeoCut_H1->Fill( dPosXH1, dPosYH1 );
                        fhMapBestPairsHodoS1S2GeoCut_H2->Fill( dPosXH2, dPosYH2 );

                        Double_t dPosXS1, dPosYS1;
                        ComputeCoordinatesSensor1( fvPairsS1[ uBestS1 ].first.GetChan(),
                                                   fvPairsS1[ uBestS1 ].second.GetChan(),
                                                   dPosXS1, dPosYS1 );
                        fhMapBestPairsHodoS1S2GeoCut_S1->Fill( dPosXS1, dPosYS1 );
                        // Apply Position Offsets
                        dPosXS1 += fdOffsetS1X;
                        dPosYS1 += fdOffsetS1Y;

                        fhMapBestPairsHodoS1S2GeoCut_Proj->Fill( dProjXS1, dProjYS1 );
                        fhResidualsBestPairsHodoS1S2GeoCut->Fill( dPosXS1 - dProjXS1, dPosYS1 - dProjYS1 );

                        fhAdcRawBestPairsHodoS1S2N->Fill( fvPairsS1[ uBestS1 ].first.GetChan(), fvPairsS1[ uBestS1 ].first.GetAdc() );
                        fhAdcRawBestPairsHodoS1S2P->Fill( fvPairsS1[ uBestS1 ].second.GetChan(), fvPairsS1[ uBestS1 ].second.GetAdc() );
                     } // if( 0 < uNbS1CoincPerHodo )
                  } // if( bHodoTrkInS1 )
               } // if( 0 < uNbS2CoincPerHodo )
            } // if( bHodoTrkInS2 )

            /// Fill the efficiency plots
            if( 0 < uNbS1CoincPerHodo )
            {
               fhEfficiency->Fill( 0., 1.0 );
               fhEfficiencyMapS1->Fill( dProjXS1, dProjYS1, 1.0 );
               if( 0 < uNbS2CoincPerHodo )
               {
                  fhEfficiency->Fill( 3., 1.0 );
                  fhEfficiencyMapS2S1->Fill( dProjXS2, dProjYS2, 1.0 );
               } // if( 0 < uNbS2CoincPerHodo )
                  else
                  {
                     fhEfficiency->Fill( 3., 0.0 );
                     fhEfficiencyMapS2S1->Fill( dProjXS2, dProjYS2, 0.0 );
                  } // else of if( 0 < uNbS2CoincPerHodo )
            } // if( 0 < uNbS1CoincPerHodo )
               else
               {
                  fhEfficiency->Fill( 0., 0.0 );
                  fhEfficiencyMapS1->Fill( dProjXS1, dProjYS1, 0.0 );
               } // else of if( 0 < uNbS1CoincPerHodo )

            if( 0 < uNbS2CoincPerHodo )
            {
               fhEfficiency->Fill( 1., 1.0 );
               fhEfficiencyMapS2->Fill( dProjXS2, dProjYS2, 1.0 );
               if( 0 < uNbS1CoincPerHodo )
               {
                  fhEfficiency->Fill( 2., 1.0 );
                  fhEfficiencyMapS1S2->Fill( dProjXS1, dProjYS1, 1.0 );
               } // if( 0 < uNbS1CoincPerHodo )
                  else
                  {
                     fhEfficiency->Fill( 2., 0.0 );
                     fhEfficiencyMapS1S2->Fill( dProjXS1, dProjYS1, 0.0 );
                  } // else of if( 0 < uNbS1CoincPerHodo )
            } // if( 0 < uNbS2CoincPerHodo )
               else
               {
                  fhEfficiency->Fill( 1., 0.0 );
                  fhEfficiencyMapS2->Fill( dProjXS2, dProjYS2, 0.0 );
               } // else of if( 0 < uNbS2CoincPerHodo )

            if( bHodoTrkInS1 )
            {
               if( 0 < uNbS1CoincPerHodo )
               {
                  fhEfficiency->Fill( 4., 1.0 );
                  fhEfficiencyMapS1GeoCut->Fill( dProjXS1, dProjYS1, 1.0 );
                  if( bHodoTrkInS2 )
                  {
                     if( 0 < uNbS2CoincPerHodo )
                     {
                        fhEfficiency->Fill( 7., 1.0 );
                        fhEfficiencyMapS2S1GeoCut->Fill( dProjXS2, dProjYS2, 1.0 );
                     } // if( 0 < uNbS2CoincPerHodo )
                        else
                        {
                           fhEfficiency->Fill( 7., 0.0 );
                           fhEfficiencyMapS2S1GeoCut->Fill( dProjXS2, dProjYS2, 0.0 );
                        } // else of if( 0 < uNbS2CoincPerHodo )
                  } // if( bHodoTrkInS2 )
               } // if( 0 < uNbS1CoincPerHodo )
                  else
                  {
                     fhEfficiency->Fill( 4., 0.0 );
                     fhEfficiencyMapS1GeoCut->Fill( dProjXS1, dProjYS1, 0.0 );
                  } // else of if( 0 < uNbS1CoincPerHodo )
            } // if( bHodoTrkInS1 )

            if( bHodoTrkInS2 )
            {
               if( 0 < uNbS2CoincPerHodo )
               {
                  fhEfficiency->Fill( 5., 1.0 );
                  fhEfficiencyMapS2GeoCut->Fill( dProjXS2, dProjYS2, 1.0 );
                  if( bHodoTrkInS1 )
                  {
                     if( 0 < uNbS1CoincPerHodo )
                     {
                        fhEfficiency->Fill( 6., 1.0 );
                        fhEfficiencyMapS1S2GeoCut->Fill( dProjXS1, dProjYS1, 1.0 );
                     } // if( 0 < uNbS1CoincPerHodo )
                        else
                        {
                           fhEfficiency->Fill( 6., 0.0 );
                           fhEfficiencyMapS1S2GeoCut->Fill( dProjXS1, dProjYS1, 0.0 );
                        } // else of if( 0 < uNbS1CoincPerHodo )
                  } // if( bHodoTrkInS1 )
               } // if( 0 < uNbS2CoincPerHodo )
                  else
                  {
                     fhEfficiency->Fill( 5., 0.0 );
                     fhEfficiencyMapS2GeoCut->Fill( dProjXS2, dProjYS2, 0.0 );
                  } // else of if( 0 < uNbS2CoincPerHodo )
            } // if( bHodoTrkInS2 )
         } // for( UInt_t uPairH1 = 0; uPairH1 < fvPairsH1.size(); ++uPairH1 )

         /// Clear the Pairs storage
         fvPairsH1.clear();
         fvPairsH2.clear();
         fvPairsS1.clear();
         fvPairsS2.clear();
         fvPairsHodo.clear();
         fvPairsHodoS1.clear();
         fvPairsHodoS2.clear();
         fvPairsHodoS1S2.clear();
         fvPairsHodoS2S1.clear();

         fvHitsS1N.clear();
         fvHitsS1P.clear();
         fvHitsS2N.clear();
         fvHitsS2P.clear();

      } // if( 0 < fvmHitsInTs.size() )

      UInt_t uNbAsicCleanInMs = 0;
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
         if( kFALSE == fvbAsicHasDuplicInMs[ uXyterIdx ] )
            uNbAsicCleanInMs++;
      Double_t dTimeSinceStartSec = dMsTime - fdStartTimeMsSz;
      for( UInt_t uXyterIdx = 0; uXyterIdx <= fuNbStsXyters; ++uXyterIdx )
      {
         if( uXyterIdx == uNbAsicCleanInMs )
         {
            fhRatioMsDuplicateQuality->Fill( uXyterIdx, 1.0 );
            fhEvoMsDuplicateQuality[ uXyterIdx ]->Fill( dTimeSinceStartSec, 1.0 );
         } // if( uXyterIdx == uNbAsicCleanInMs )
            else
            {
               fhRatioMsDuplicateQuality->Fill( uXyterIdx, 0.0 );
               fhEvoMsDuplicateQuality[ uXyterIdx ]->Fill( dTimeSinceStartSec, 0.0 );
            } // else of if( uXyterIdx != uNbAsicCleanInMs )
      } // for( UInt_t uXyterIdx = 0; uXyterIdx <= fuNbStsXyters; ++uXyterIdx )
   } // for( size_t m = 0; m < numCompMsInTs; ++m )

   // End of TS, check if stuff to do with the hits inside each MS
   // Usefull for low rate pulser tests
      // Need to do it only when last DPB is processed, as they are done one by one
   if( fuCurrDpbIdx == fuNrOfDpbs - 1 )
   {
      for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
      {
         fvdMsTime[ uMsIdx ] = 0.0;
      } // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
   } // if( fuCurrDpbIdx == fuNrOfDpbs - 1 )

   if( 0 == ts.index() % 1000 && fuCurrDpbIdx == fuNrOfDpbs - 1 )
   {
      for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
      {
         Double_t dTsMsbTime =
               static_cast<ULong64_t>( stsxyter::kuHitNbTsBins )
             * static_cast<ULong64_t>( fvulCurrentTsMsb[fuCurrDpbIdx])
             + static_cast<ULong64_t>( stsxyter::kulTsCycleNbBins )
             * static_cast<ULong64_t>( fvuCurrentTsMsbCycle[fuCurrDpbIdx] );
         dTsMsbTime *= stsxyter::kdClockCycleNs * 1e-9;

         LOG(info) << "End of TS " << std::setw(7) << ts.index()
                   << " eDPB "   << std::setw(2) << uDpb
                   << " current TS MSB counter is " << std::setw(12) << fvulCurrentTsMsb[uDpb]
                   << " current TS MSB cycle counter is " << std::setw(12) << fvuCurrentTsMsbCycle[uDpb]
                   << " current TS MSB time is " << std::setw(12) << dTsMsbTime << " s";
      }
   } // if( 0 == ts.index() % 1000 && fuCurrDpbIdx == fuNrOfDpbs - 1 )

  return kTRUE;
}

void CbmCosy2018MonitorEfficiency::FillHitInfo( stsxyter::Message mess, const UShort_t & /*usElinkIdx*/, const UInt_t & uAsicIdx, const UInt_t & uMsIdx )
{
   UShort_t usChan   = mess.GetHitChannel();
   UShort_t usRawAdc = mess.GetHitAdc();
//   UShort_t usFullTs = mess.GetHitTimeFull();
//   UShort_t usTsOver = mess.GetHitTimeOver();
   UShort_t usRawTs  = mess.GetHitTime();

   fhHodoChanCntRaw[  uAsicIdx ]->Fill( usChan );
   fhHodoChanAdcRaw[  uAsicIdx ]->Fill( usChan, usRawAdc );
   fhHodoChanAdcRawProf[  uAsicIdx ]->Fill( usChan, usRawAdc );
   fhHodoChanRawTs[   uAsicIdx ]->Fill( usChan, usRawTs );
   fhHodoChanMissEvt[ uAsicIdx ]->Fill( usChan, mess.IsHitMissedEvts() );

   // Compute the Full time stamp
//   Long64_t ulOldHitTime = fvulChanLastHitTime[ uAsicIdx ][ usChan ];
//   Double_t dOldHitTime  = fvdChanLastHitTime[ uAsicIdx ][ usChan ];

      // Use TS w/o overlap bits as they will anyway come from the TS_MSB
   fvulChanLastHitTime[ uAsicIdx ][ usChan ] = usRawTs;

   fvulChanLastHitTime[ uAsicIdx ][ usChan ] +=
               static_cast<ULong64_t>( stsxyter::kuHitNbTsBins )
             * static_cast<ULong64_t>( fvulCurrentTsMsb[fuCurrDpbIdx])
             + static_cast<ULong64_t>( stsxyter::kulTsCycleNbBins )
             * static_cast<ULong64_t>( fvuCurrentTsMsbCycle[fuCurrDpbIdx] )
             ;

//   fvuElinkLastTsHit[fuCurrDpbIdx] = usRawTs;

   // Convert the Hit time in bins to Hit time in ns
   Double_t dHitTimeNs = fvulChanLastHitTime[ uAsicIdx ][ usChan ] * stsxyter::kdClockCycleNs;
/*
   // If needed fill the hit interval plots
   if( fbChanHitDtEna )
   {
      Double_t dDeltaT = dHitTimeNs - fvdChanLastHitTime[ uAsicIdx ][ usChan ];
      if( 0 == dDeltaT )
         fhStsChanHitDtNeg[ uAsicIdx ]->Fill( 1, usChan );
         else if( 0 < dDeltaT )
            fhStsChanHitDt[ uAsicIdx ]->Fill( dDeltaT, usChan );
         else fhStsChanHitDtNeg[ uAsicIdx ]->Fill( -dDeltaT, usChan );
   } // if( fbChanHitDtEna )
*/
   // Store new value of Hit time in ns
   fvdChanLastHitTime[ uAsicIdx ][ usChan ] = fvulChanLastHitTime[ uAsicIdx ][ usChan ] * stsxyter::kdClockCycleNs;
/*
   LOG(info) << " Asic " << std::setw( 2 ) << uAsicIdx
             << " Channel " << std::setw( 3 ) << usChan
             << " Diff to last hit " << std::setw( 12 ) << ( fvulChanLastHitTime[ uAsicIdx ][ usChan ] - ulOldHitTime)
             << " in s " << std::setw( 12 ) << ( fvdChanLastHitTime[ uAsicIdx ][ usChan ] - dOldHitTime) * 1e-9;
*/
   // Pulser and MS
   fvuChanNbHitsInMs[      uAsicIdx ][ usChan ][ uMsIdx ] ++;
   fvdChanLastHitTimeInMs[ uAsicIdx ][ usChan ][ uMsIdx ] = dHitTimeNs;
   fvusChanLastHitAdcInMs[ uAsicIdx ][ usChan ][ uMsIdx ] = usRawAdc;
/*
   fvmChanHitsInTs[        uAsicIdx ][ usChan ].insert( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ],
                                                                            usRawAdc, uAsicIdx, usChan ) );
*/
   fvmHitsInTs.push_back( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ], usRawAdc, uAsicIdx, usChan ) );

/*
      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MsInTs " << std::setw( 3 ) << uMsIdx
                << " Asic " << std::setw( 2 ) << uAsicIdx
                << " Channel " << std::setw( 3 ) << usChan
                << " ADC " << std::setw( 3 ) << usRawAdc
                << " TS " << std::setw( 3 )  << usRawTs // 9 bits TS
                << " SX TsMsb " << std::setw( 2 ) << ( fvulCurrentTsMsb[fuCurrDpbIdx] & 0x1F ) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b TS_MSB after DPB
                << " DPB TsMsb " << std::setw( 6 ) << ( fvulCurrentTsMsb[fuCurrDpbIdx] >> 5 ) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b of TS_MSB after DPB
                << " TsMsb " << std::setw( 7 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " MsbCy " << std::setw( 4 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " Time " << std::setw ( 12 ) << fvulChanLastHitTime[ uAsicIdx ][ usChan ];
*/
   // Check Starting point of histos with time as X axis
   if( -1 == fdStartTime )
      fdStartTime = fvdChanLastHitTime[ uAsicIdx ][ usChan ];

   // Fill histos with time as X axis
   Double_t dTimeSinceStartSec = (fvdChanLastHitTime[ uAsicIdx ][ usChan ] - fdStartTime)* 1e-9;
   Double_t dTimeSinceStartMin = dTimeSinceStartSec / 60.0;
   fhHodoChanHitRateEvo[ uAsicIdx ]->Fill( dTimeSinceStartSec , usChan );
   fhHodoFebRateEvo[ uAsicIdx ]->Fill(   dTimeSinceStartSec );
   fhHodoChanHitRateEvoLong[ uAsicIdx ]->Fill( dTimeSinceStartMin, usChan, 1.0/60.0 );
   fhHodoFebRateEvoLong[ uAsicIdx ]->Fill(   dTimeSinceStartMin, 1.0/60.0 );
   if( mess.IsHitMissedEvts() )
   {
      fhHodoChanMissEvtEvo[ uAsicIdx ]->Fill( dTimeSinceStartSec , usChan );
      fhHodoFebMissEvtEvo[ uAsicIdx ]->Fill( dTimeSinceStartSec );
   } // if( mess.IsHitMissedEvts() )
/*
   if( kTRUE == fbLongHistoEnable )
   {
      std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
      Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();
      fhFebRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , 1.0 / fuLongHistoBinSizeSec );
      fhFebChRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , usChan, 1.0 / fuLongHistoBinSizeSec );
   } // if( kTRUE == fbLongHistoEnable )
*/

}

void CbmCosy2018MonitorEfficiency::FillTsMsbInfo( stsxyter::Message mess, UInt_t /*uMessIdx*/, UInt_t /*uMsIdx*/ )
{
   UInt_t uVal    = mess.GetTsMsbVal();
/*
   if( 0 == fuCurrDpbIdx )
      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MsInTs " << std::setw( 3 ) << uMsIdx
                << " DPB "    << std::setw( 2 ) << fuCurrDpbIdx
                << " Mess "   << std::setw( 5 ) << uMessIdx
                << " TsMsb "  << std::setw( 5 ) << uVal;
*/
/*
   if( (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1) && 0 < uVal  &&
       !( 1 == uMessIdx && usVal == fvulCurrentTsMsb[fuCurrDpbIdx] ) ) // 1st TS_MSB in MS is always a repeat of the last one in previous MS!
   {
      LOG(info) << "TS MSB not increasing by 1!  TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MsInTs " << std::setw( 3 ) << uMsIdx
                << " DPB " << std::setw( 2 ) << fuCurrDpbIdx
                << " Mess " << std::setw( 5 ) << uMessIdx
                << " Old TsMsb " << std::setw( 5 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " new TsMsb " << std::setw( 5 ) << uVal
                << " Diff " << std::setw( 5 ) << uVal - fvulCurrentTsMsb[fuCurrDpbIdx]
                << " Old MsbCy " << std::setw( 5 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx];
   } // if( (uVal != fvulCurrentTsMsb[fuCurrDpbIdx] + 1) && 0 < uVal )
*/

   // Update Status counters
   if( uVal < fvulCurrentTsMsb[fuCurrDpbIdx] )
   {

      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " DPB " << std::setw( 2 ) << fuCurrDpbIdx
                << " Old TsMsb " << std::setw( 5 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " Old MsbCy " << std::setw( 5 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " new TsMsb " << std::setw ( 5 ) << uVal;

      fvuCurrentTsMsbCycle[fuCurrDpbIdx] ++;
   } // if( uVal < fvulCurrentTsMsb[fuCurrDpbIdx] )
   fvulCurrentTsMsb[fuCurrDpbIdx] = uVal;
/*
   if( 1 < uMessIdx )
   {
      fhStsDpbRawTsMsb->Fill( fuCurrDpbIdx,      fvulCurrentTsMsb[fuCurrDpbIdx] );
      fhStsDpbRawTsMsbSx->Fill( fuCurrDpbIdx,  ( fvulCurrentTsMsb[fuCurrDpbIdx] & 0x1F ) );
      fhStsDpbRawTsMsbDpb->Fill( fuCurrDpbIdx, ( fvulCurrentTsMsb[fuCurrDpbIdx] >> 5 ) );
   } // if( 0 < uMessIdx )
*/
//   fhStsAsicTsMsb->Fill( fvulCurrentTsMsb[fuCurrDpbIdx], uAsicIdx );

   ULong64_t ulNewTsMsbTime =  static_cast< ULong64_t >( stsxyter::kuHitNbTsBins )
                             * static_cast< ULong64_t >( fvulCurrentTsMsb[fuCurrDpbIdx])
                             + static_cast< ULong64_t >( stsxyter::kulTsCycleNbBins )
                             * static_cast< ULong64_t >( fvuCurrentTsMsbCycle[fuCurrDpbIdx] );

   Double_t dUpdatePeriodInSec = 1;
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      Double_t dTimeInS = ( ulNewTsMsbTime - fvulStartTimeLastS[ uXyterIdx ] )
                          * stsxyter::kdClockCycleNs
                          * 1e-9;
      if( dUpdatePeriodInSec <= dTimeInS || (ulNewTsMsbTime < fvulStartTimeLastS[ uXyterIdx ] ) )
      {
         UInt_t uNbHitsTotal = fvuNbHitDiffTsAsicLastS[ uXyterIdx ] + fvuNbHitSameTsAsicLastS[ uXyterIdx ];
         if( 0 < uNbHitsTotal && dTimeInS < 2 * dUpdatePeriodInSec  )
         {
            Double_t dRate = static_cast< Double_t >( uNbHitsTotal ) / dTimeInS;
            Double_t dRatioSameTs = static_cast< Double_t >( fvuNbHitSameTsAsicLastS[ uXyterIdx ] )
                                   / uNbHitsTotal;
            Double_t dRatioSameTsAdc = static_cast< Double_t >( fvuNbHitSameTsAdcAsicLastS[ uXyterIdx ] )
                                      / uNbHitsTotal;
            Double_t dRatioSameAdcSameTs = static_cast< Double_t >( fvuNbHitSameTsAdcAsicLastS[ uXyterIdx ] )
                                          / fvuNbHitSameTsAsicLastS[ uXyterIdx ];

            fhSetupSortedAsicRatioSameTsVsFlux[ uXyterIdx ]->Fill( dRate, dRatioSameTs );
            fhSetupSortedAsicRatioSameTsAdcVsFlux[ uXyterIdx ]->Fill( dRate, dRatioSameTsAdc );
            fhSetupSortedAsicRatioSameAdcSameTsVsFlux[ uXyterIdx ]->Fill( dRate, dRatioSameAdcSameTs );
/*
            LOG(info) << "Here we are " << dTimeInS
                      << " " << uNbHitsTotal
                      << " " << dRate;
*/
         } // if( 0 < uNbHitsTotal && dTimeInS < 2 * dUpdatePeriodInSec  )

         fvulStartTimeLastS[ uXyterIdx ] = ulNewTsMsbTime;
         fvuNbHitDiffTsAsicLastS[ uXyterIdx ]    = 0;
         fvuNbHitSameTsAsicLastS[ uXyterIdx ]    = 0;
         fvuNbHitSameTsAdcAsicLastS[ uXyterIdx ] = 0;
      } // if( dUpdatePeriodInSec <= dTimeInS || (ulNewTsMsbTime < fvulStartTimeLastS[ uXyterIdx ] ) )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
}

void CbmCosy2018MonitorEfficiency::FillEpochInfo( stsxyter::Message /*mess*/ )
{
//   UInt_t uVal    = mess.GetTsMsbVal();
/*
   // Update Status counters
   if( usVal < fvulCurrentTsMsb[fuCurrDpbIdx] )
      fvuCurrentTsMsbCycle[fuCurrDpbIdx] ++;
   fvulCurrentTsMsb[fuCurrDpbIdx] = usVal;

//   fhStsAsicTsMsb->Fill( fvulCurrentTsMsb[fuCurrDpbIdx], uAsicIdx );
*/
}

void CbmCosy2018MonitorEfficiency::Reset()
{
}

void CbmCosy2018MonitorEfficiency::Finish()
{

   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmCosy2018MonitorEfficiency statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::MessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::MessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::MessType::Dummy ] << "\n"
             << " Epoch    messages: " << fmMsgCounter[ stsxyter::MessType::Epoch ] << "\n"
             << " Empty    messages: " << fmMsgCounter[ stsxyter::MessType::Empty ];

   LOG(info) << "-------------------------------------";

   SaveAllHistos( fsHistoFileFullname );
   SaveAllHistos();

}


void CbmCosy2018MonitorEfficiency::FillOutput(boost::any)
{
}

void CbmCosy2018MonitorEfficiency::SaveAllHistos( TString sFileName )
{
   /*******************************************************************/
   TCanvas* cFebRatioDupli = new TCanvas( "cFebRatioDupli",
                                    "Hodoscopes coincidence quality",
                                    800, 600);
   cFebRatioDupli->Divide( 2, fuNbStsXyters/2 + fuNbStsXyters%2 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebRatioDupli->cd(1 + uXyterIdx );
      gPad->SetLogz();
      fhSetupSortedNbSameTsChan[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   /*******************************************************************/

   TDirectory * oldDir = NULL;
   TFile * histoFile = NULL;
   if( "" != sFileName )
   {
      // Store current directory position to allow restore later
      oldDir = gDirectory;
      // open separate histo file in recreate mode
      histoFile = new TFile( sFileName , "RECREATE");
      histoFile->cd();
   } // if( "" != sFileName )

   /***************************/
   gDirectory->mkdir("Sts_Raw");
   gDirectory->cd("Sts_Raw");

   fhHodoMessType->Write();
   fhHodoSysMessType->Write();
   fhHodoMessTypePerDpb->Write();
   fhHodoSysMessTypePerDpb->Write();
   fhHodoMessTypePerElink->Write();
   fhHodoSysMessTypePerElink->Write();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhHodoChanCntRaw[ uXyterIdx ]->Write();
      fhHodoChanAdcRaw[ uXyterIdx ]->Write();
      fhHodoChanAdcRawProf[ uXyterIdx ]->Write();
      fhHodoChanRawTs[ uXyterIdx ]->Write();
      fhHodoChanMissEvt[ uXyterIdx ]->Write();
      fhHodoChanMissEvtEvo[ uXyterIdx ]->Write();
      fhHodoFebMissEvtEvo[ uXyterIdx ]->Write();
      fhHodoChanHitRateEvo[ uXyterIdx ]->Write();
      fhHodoFebRateEvo[ uXyterIdx ]->Write();
      fhHodoChanHitRateEvoLong[ uXyterIdx ]->Write();
      fhHodoFebRateEvoLong[ uXyterIdx ]->Write();
/*
      if( kTRUE == fbLongHistoEnable )
      {
         fhFebRateEvoLong[ uXyterIdx ]->Write();
         fhFebChRateEvoLong[ uXyterIdx ]->Write();
      } // if( kTRUE == fbLongHistoEnable )
*/
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )


   fhSetupSortedDtX1Y1->Write();
   fhSetupSortedDtX2Y2->Write();
   fhSetupSortedDtN1P1->Write();
   fhSetupSortedDtN2P2->Write();
   fhSetupSortedDtX1Y1X2Y2->Write();
   fhSetupSortedDtN1P1N2P2->Write();
   fhSetupSortedDtX1Y1X2Y2N1P1->Write();
   fhSetupSortedDtX1Y1X2Y2N2P2->Write();
   fhSetupSortedDtH1H2S1S2->Write();
   fhSetupSortedCntEvoX1Y1->Write();
   fhSetupSortedCntEvoX2Y2->Write();
   fhSetupSortedCntEvoN1P1->Write();
   fhSetupSortedCntEvoN2P2->Write();

   fhSetupSortedDtX1->Write();
   fhSetupSortedDtY1->Write();
   fhSetupSortedDtX2->Write();
   fhSetupSortedDtY2->Write();
   fhSetupSortedDtN1->Write();
   fhSetupSortedDtP1->Write();
   fhSetupSortedDtN2->Write();
   fhSetupSortedDtP2->Write();

   gDirectory->cd("..");
   /***************************/

   /***************************/
   gDirectory->mkdir("Sts_Dupli");
   gDirectory->cd("Sts_Dupli");

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhSetupSortedNbSameTsChan[ uXyterIdx ]->Write();
      fhSetupSortedSameTsAdcChan[ uXyterIdx ]->Write();
      fhSetupSortedSameTsAdcDiff[ uXyterIdx ]->Write();
      fhSetupSortedRatioSameTsChan[ uXyterIdx ]->Write();
      fhSetupSortedNbConsSameTsChan[ uXyterIdx ]->Write();
      fhSetupSortedNbConsSameTsAdcChan[ uXyterIdx ]->Write();
      fhSetupSortedNbConsSameTsVsTsAdc[ uXyterIdx ]->Write();

      fhSetupSortedAsicRatioSameTsVsFlux[ uXyterIdx ]->Write();
      fhSetupSortedAsicRatioSameTsAdcVsFlux[ uXyterIdx ]->Write();
      fhSetupSortedAsicRatioSameAdcSameTsVsFlux[ uXyterIdx ]->Write();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fhRatioMsDuplicateQuality->Write();
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhEvoMsDuplicateQuality[ uXyterIdx ]->Write();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   cFebRatioDupli->Write();
   delete cFebRatioDupli;

   gDirectory->cd("..");
   /***************************/

   /***************************/
   gDirectory->mkdir("Sts_Maps");
   gDirectory->cd("Sts_Maps");

   fhSetupSortedMapX1Y1->Write();
   fhSetupSortedMapX2Y2->Write();
   fhSetupSortedMapN1P1->Write();
   fhSetupSortedMapN2P2->Write();
   fhStsSortedMapS1->Write();
   if( kTRUE == fbDualStsEna )
   {
      fhStsSortedMapS2->Write();
      fhStsSortedMapS1Coinc->Write();
      fhStsSortedMapS2Coinc->Write();
   } // if( kTRUE == fbDualStsEna )

   gDirectory->cd("..");
   /***************************/

   /***************************/
   gDirectory->mkdir("Sts_Cuts");
   gDirectory->cd("Sts_Cuts");

   fhDtAllPairsS1->Write();
   fhDtAllPairsS2->Write();
   fhDtAllPairsHodoS1->Write();
   fhDtAllPairsHodoS2->Write();
   fhDtBestPairsH1->Write();
   fhDtBestPairsH2->Write();
   fhDtBestPairsS1->Write();
   fhDtBestPairsS2->Write();
   fhDtBestPairsHodo->Write();
   fhDtBestPairsHodoS1->Write();
   fhDtBestPairsHodoS2->Write();
   fhDtBestPairsHodoS1S2->Write();
   fhDtBestPairsHodoS2S1->Write();

   fhMapBestPairsH1->Write();
   fhMapBestPairsH2->Write();
   fhMapBestPairsS1->Write();
   fhMapBestPairsS2->Write();
   fhMapBestPairsHodo_H1->Write();
   fhMapBestPairsHodo_H2->Write();
   fhMapBestPairsHodo_ProjS1->Write();
   fhMapBestPairsHodo_ProjS2->Write();
   fhMapBestPairsHodoS1_H1->Write();
   fhMapBestPairsHodoS1_H2->Write();
   fhMapBestPairsHodoS1_S1->Write();
   fhMapBestPairsHodoS1_Proj->Write();
   fhMapBestPairsHodoS2_H1->Write();
   fhMapBestPairsHodoS2_H2->Write();
   fhMapBestPairsHodoS2_S2->Write();
   fhMapBestPairsHodoS2_Proj->Write();
   fhMapBestPairsHodoS1S2_H1->Write();
   fhMapBestPairsHodoS1S2_H2->Write();
   fhMapBestPairsHodoS1S2_S1->Write();
   fhMapBestPairsHodoS1S2_Proj->Write();
   fhMapBestPairsHodoS2S1_H1->Write();
   fhMapBestPairsHodoS2S1_H2->Write();
   fhMapBestPairsHodoS2S1_S2->Write();
   fhMapBestPairsHodoS2S1_Proj->Write();

   fhResidualsBestPairsHodoS1->Write();
   fhResidualsBestPairsHodoS2->Write();
   fhResidualsBestPairsHodoS1S2->Write();
   fhResidualsBestPairsHodoS2S1->Write();

   fhAdcRawBestPairsHodoS1N->Write();
   fhAdcRawBestPairsHodoS1P->Write();
   fhAdcRawBestPairsHodoS2N->Write();
   fhAdcRawBestPairsHodoS2P->Write();
   fhAdcRawBestPairsHodoS1S2N->Write();
   fhAdcRawBestPairsHodoS1S2P->Write();
   fhAdcRawBestPairsHodoS2S1N->Write();
   fhAdcRawBestPairsHodoS2S1P->Write();

   gDirectory->cd("..");
   /***************************/

   /***************************/
   gDirectory->mkdir("Sts_GeoCuts");
   gDirectory->cd("Sts_GeoCuts");

   fhDtBestPairsHodoS1GeoCut->Write();
   fhDtBestPairsHodoS2GeoCut->Write();
   fhDtBestPairsHodoS1S2GeoCut->Write();
   fhDtBestPairsHodoS2S1GeoCut->Write();

   fhMapBestPairsHodoGeoCutS1_H1->Write();
   fhMapBestPairsHodoGeoCutS1_H2->Write();
   fhMapBestPairsHodoGeoCutS1_Proj->Write();
   fhMapBestPairsHodoGeoCutS2_H1->Write();
   fhMapBestPairsHodoGeoCutS2_H2->Write();
   fhMapBestPairsHodoGeoCutS2_Proj->Write();

   fhMapBestPairsHodoS1GeoCut_H1->Write();
   fhMapBestPairsHodoS1GeoCut_H2->Write();
   fhMapBestPairsHodoS1GeoCut_S1->Write();
   fhMapBestPairsHodoS1GeoCut_Proj->Write();
   fhMapBestPairsHodoS2GeoCut_H1->Write();
   fhMapBestPairsHodoS2GeoCut_H2->Write();
   fhMapBestPairsHodoS2GeoCut_S2->Write();
   fhMapBestPairsHodoS2GeoCut_Proj->Write();
   fhMapBestPairsHodoS1S2GeoCut_H1->Write();
   fhMapBestPairsHodoS1S2GeoCut_H2->Write();
   fhMapBestPairsHodoS1S2GeoCut_S1->Write();
   fhMapBestPairsHodoS1S2GeoCut_Proj->Write();
   fhMapBestPairsHodoS2S1GeoCut_H1->Write();
   fhMapBestPairsHodoS2S1GeoCut_H2->Write();
   fhMapBestPairsHodoS2S1GeoCut_S2->Write();
   fhMapBestPairsHodoS2S1GeoCut_Proj->Write();

   fhResidualsBestPairsHodoS1GeoCut->Write();
   fhResidualsBestPairsHodoS2GeoCut->Write();
   fhResidualsBestPairsHodoS1S2GeoCut->Write();
   fhResidualsBestPairsHodoS2S1GeoCut->Write();

   gDirectory->cd("..");
   /***************************/

   /***************************/
   gDirectory->mkdir("Sts_Eff");
   gDirectory->cd("Sts_Eff");

   fhEfficiency->Write();
   fhEfficiencyMapS1->Write();
   fhEfficiencyMapS2->Write();
   fhEfficiencyMapS1S2->Write();
   fhEfficiencyMapS2S1->Write();
   fhEfficiencyMapS1GeoCut->Write();
   fhEfficiencyMapS2GeoCut->Write();
   fhEfficiencyMapS1S2GeoCut->Write();
   fhEfficiencyMapS2S1GeoCut->Write();

   gDirectory->cd("..");
   /***************************/

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
   gDirectory->mkdir("Adc");
   gDirectory->cd("Adc");

   fhDtNeighborChansS1N->Write();
   fhDtNeighborChansS1P->Write();
   fhDtNeighborChansS2N->Write();
   fhDtNeighborChansS2P->Write();

   fhClusterAdcVsSizeS1N->Write();
   fhClusterAdcVsSizeS1P->Write();
   fhClusterAdcVsSizeS2N->Write();
   fhClusterAdcVsSizeS2P->Write();

   fhClusterAdcVsSizeS1N_MatchS1->Write();
   fhClusterAdcVsSizeS1P_MatchS1->Write();
   fhClusterAdcVsSizeS2N_MatchS2->Write();
   fhClusterAdcVsSizeS2P_MatchS2->Write();

   fhClusterS1N_CenterNbMatchS1P->Write();
   fhClusterS1P_CenterNbMatchS1N->Write();
   fhClusterS2N_CenterNbMatchS2P->Write();
   fhClusterS2P_CenterNbMatchS2N->Write();

   fhClusterS1N_AdcNbMatchS1P->Write();
   fhClusterS1P_AdcNbMatchS1N->Write();
   fhClusterS2N_AdcNbMatchS2P->Write();
   fhClusterS2P_AdcNbMatchS2N->Write();

   fhDtNeighborChansH1X->Write();
   fhDtNeighborChansH1Y->Write();
   fhDtNeighborChansH2X->Write();
   fhDtNeighborChansH2Y->Write();

   fhClusterAdcVsSizeH1X->Write();
   fhClusterAdcVsSizeH1Y->Write();
   fhClusterAdcVsSizeH2X->Write();
   fhClusterAdcVsSizeH2Y->Write();
   fhClusterPosition_MatchH1->Write();
   fhClusterPosition_MatchH2->Write();

   gDirectory->cd("..");
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   /***************************/
   // Flib Histos
   gDirectory->mkdir("Flib_Raw");
   gDirectory->cd("Flib_Raw");
   for( UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; uLinks ++)
   {
      TString sMsSzName = Form("MsSz_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->H1( sMsSzName.Data() )->Write();

      sMsSzName = Form("MsSzTime_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->P1( sMsSzName.Data() )->Write();
   } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)

   TH1 * pMissedTsH1    = dynamic_cast< TH1 * >( gROOT->FindObjectAny( "Missed_TS" ) );
   if( NULL != pMissedTsH1 )
      pMissedTsH1->Write();

   TProfile * pMissedTsEvoP = dynamic_cast< TProfile * >( gROOT->FindObjectAny( "Missed_TS_Evo" ) );
   if( NULL != pMissedTsEvoP )
      pMissedTsEvoP->Write();

   gDirectory->cd("..");
   /***************************/

   if( "" != sFileName )
   {
      // Restore original directory position
      histoFile->Close();
      oldDir->cd();
   } // if( "" != sFileName )

}
void CbmCosy2018MonitorEfficiency::ResetAllHistos()
{
   LOG(info) << "Reseting all STS histograms.";

   fhHodoMessType->Reset();
   fhHodoSysMessType->Reset();
   fhHodoMessTypePerDpb->Reset();
   fhHodoSysMessTypePerDpb->Reset();
   fhHodoMessTypePerElink->Reset();
   fhHodoSysMessTypePerElink->Reset();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhHodoChanCntRaw[ uXyterIdx ]->Reset();
      fhHodoChanAdcRaw[ uXyterIdx ]->Reset();
      fhHodoChanAdcRawProf[ uXyterIdx ]->Reset();
      fhHodoChanRawTs[ uXyterIdx ]->Reset();
      fhHodoChanMissEvt[ uXyterIdx ]->Reset();
      fhHodoChanMissEvtEvo[ uXyterIdx ]->Reset();
      fhHodoFebMissEvtEvo[ uXyterIdx ]->Reset();
      fhHodoChanHitRateEvo[ uXyterIdx ]->Reset();
      fhHodoFebRateEvo[ uXyterIdx ]->Reset();
      fhHodoChanHitRateEvoLong[ uXyterIdx ]->Reset();
      fhHodoFebRateEvoLong[ uXyterIdx ]->Reset();
/*
      if( kTRUE == fbLongHistoEnable )
      {
         ftStartTimeUnix = std::chrono::steady_clock::now();
         fhFebRateEvoLong[ uXyterIdx ]->Reset();
         fhFebChRateEvoLong[ uXyterIdx ]->Reset();
      } // if( kTRUE == fbLongHistoEnable )
*/
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fhSetupSortedDtX1Y1->Reset();
   fhSetupSortedDtX2Y2->Reset();
   fhSetupSortedDtN1P1->Reset();
   fhSetupSortedDtN2P2->Reset();
   fhSetupSortedDtX1Y1X2Y2->Reset();
   fhSetupSortedDtN1P1N2P2->Reset();
   fhSetupSortedDtX1Y1X2Y2N1P1->Reset();
   fhSetupSortedDtX1Y1X2Y2N2P2->Reset();
   fhSetupSortedDtH1H2S1S2->Reset();
   fhSetupSortedCntEvoX1Y1->Reset();
   fhSetupSortedCntEvoX2Y2->Reset();
   fhSetupSortedCntEvoN1P1->Reset();
   fhSetupSortedCntEvoN2P2->Reset();

   fhSetupSortedDtX1->Reset();
   fhSetupSortedDtY1->Reset();
   fhSetupSortedDtX2->Reset();
   fhSetupSortedDtY2->Reset();
   fhSetupSortedDtN1->Reset();
   fhSetupSortedDtP1->Reset();
   fhSetupSortedDtN2->Reset();
   fhSetupSortedDtP2->Reset();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhSetupSortedNbSameTsChan[ uXyterIdx ]->Reset();
      fhSetupSortedSameTsAdcChan[ uXyterIdx ]->Reset();
      fhSetupSortedSameTsAdcDiff[ uXyterIdx ]->Reset();
      fhSetupSortedRatioSameTsChan[ uXyterIdx ]->Reset();
      fhSetupSortedNbConsSameTsChan[ uXyterIdx ]->Reset();
      fhSetupSortedNbConsSameTsAdcChan[ uXyterIdx ]->Reset();
      fhSetupSortedNbConsSameTsVsTsAdc[ uXyterIdx ]->Reset();

      fhSetupSortedAsicRatioSameTsVsFlux[ uXyterIdx ]->Reset();
      fhSetupSortedAsicRatioSameTsAdcVsFlux[ uXyterIdx ]->Reset();
      fhSetupSortedAsicRatioSameAdcSameTsVsFlux[ uXyterIdx ]->Reset();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fhRatioMsDuplicateQuality->Reset();
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhEvoMsDuplicateQuality[ uXyterIdx ]->Reset();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fhSetupSortedMapX1Y1->Reset();
   fhSetupSortedMapX2Y2->Reset();
   fhSetupSortedMapN1P1->Reset();
   fhSetupSortedMapN2P2->Reset();
   fhStsSortedMapS1->Reset();
   if( kTRUE == fbDualStsEna )
   {
      fhStsSortedMapS2->Reset();
      fhStsSortedMapS1Coinc->Reset();
      fhStsSortedMapS2Coinc->Reset();
   } // if( kTRUE == fbDualStsEna )

   fhDtAllPairsS1->Reset();
   fhDtAllPairsS2->Reset();
   fhDtAllPairsHodoS1->Reset();
   fhDtAllPairsHodoS2->Reset();
   fhDtBestPairsH1->Reset();
   fhDtBestPairsH2->Reset();
   fhDtBestPairsS1->Reset();
   fhDtBestPairsS2->Reset();
   fhDtBestPairsHodo->Reset();
   fhDtBestPairsHodoS1->Reset();
   fhDtBestPairsHodoS2->Reset();
   fhDtBestPairsHodoS1S2->Reset();
   fhDtBestPairsHodoS2S1->Reset();
   fhMapBestPairsH1->Reset();
   fhMapBestPairsH2->Reset();
   fhMapBestPairsS1->Reset();
   fhMapBestPairsS2->Reset();
   fhMapBestPairsHodo_H1->Reset();
   fhMapBestPairsHodo_H2->Reset();
   fhMapBestPairsHodo_ProjS1->Reset();
   fhMapBestPairsHodo_ProjS2->Reset();
   fhMapBestPairsHodoS1_H1->Reset();
   fhMapBestPairsHodoS1_H2->Reset();
   fhMapBestPairsHodoS1_S1->Reset();
   fhMapBestPairsHodoS1_Proj->Reset();
   fhMapBestPairsHodoS2_H1->Reset();
   fhMapBestPairsHodoS2_H2->Reset();
   fhMapBestPairsHodoS2_S2->Reset();
   fhMapBestPairsHodoS2_Proj->Reset();
   fhMapBestPairsHodoS1S2_H1->Reset();
   fhMapBestPairsHodoS1S2_H2->Reset();
   fhMapBestPairsHodoS1S2_S1->Reset();
   fhMapBestPairsHodoS1S2_Proj->Reset();
   fhMapBestPairsHodoS2S1_H1->Reset();
   fhMapBestPairsHodoS2S1_H2->Reset();
   fhMapBestPairsHodoS2S1_S2->Reset();
   fhMapBestPairsHodoS2S1_Proj->Reset();
   fhResidualsBestPairsHodoS1->Reset();
   fhResidualsBestPairsHodoS2->Reset();
   fhResidualsBestPairsHodoS1S2->Reset();
   fhResidualsBestPairsHodoS2S1->Reset();

   fhDtBestPairsHodoS1GeoCut->Reset();
   fhDtBestPairsHodoS2GeoCut->Reset();
   fhDtBestPairsHodoS1S2GeoCut->Reset();
   fhDtBestPairsHodoS2S1GeoCut->Reset();

   fhMapBestPairsHodoGeoCutS1_H1->Reset();
   fhMapBestPairsHodoGeoCutS1_H2->Reset();
   fhMapBestPairsHodoGeoCutS2_H1->Reset();
   fhMapBestPairsHodoGeoCutS2_H2->Reset();
   fhMapBestPairsHodoS1GeoCut_H1->Reset();
   fhMapBestPairsHodoS1GeoCut_H2->Reset();
   fhMapBestPairsHodoS1GeoCut_S1->Reset();
   fhMapBestPairsHodoS1GeoCut_Proj->Reset();
   fhMapBestPairsHodoS2GeoCut_H1->Reset();
   fhMapBestPairsHodoS2GeoCut_H2->Reset();
   fhMapBestPairsHodoS2GeoCut_S2->Reset();
   fhMapBestPairsHodoS2GeoCut_Proj->Reset();
   fhMapBestPairsHodoS1S2GeoCut_H1->Reset();
   fhMapBestPairsHodoS1S2GeoCut_H2->Reset();
   fhMapBestPairsHodoS1S2GeoCut_S1->Reset();
   fhMapBestPairsHodoS1S2GeoCut_Proj->Reset();
   fhMapBestPairsHodoS2S1GeoCut_H1->Reset();
   fhMapBestPairsHodoS2S1GeoCut_H2->Reset();
   fhMapBestPairsHodoS2S1GeoCut_S2->Reset();
   fhMapBestPairsHodoS2S1GeoCut_Proj->Reset();

   fhResidualsBestPairsHodoS1GeoCut->Reset();
   fhResidualsBestPairsHodoS2GeoCut->Reset();
   fhResidualsBestPairsHodoS1S2GeoCut->Reset();
   fhResidualsBestPairsHodoS2S1GeoCut->Reset();

   ///++++++++ Adrian ADC analysis, to be moved in other class! +++++///
   fhDtNeighborChansS1N->Reset();
   fhDtNeighborChansS1P->Reset();
   fhDtNeighborChansS2N->Reset();
   fhDtNeighborChansS2P->Reset();

   fhClusterAdcVsSizeS1N->Reset();
   fhClusterAdcVsSizeS1P->Reset();
   fhClusterAdcVsSizeS2N->Reset();
   fhClusterAdcVsSizeS2P->Reset();

   fhClusterAdcVsSizeS1N_MatchS1->Reset();
   fhClusterAdcVsSizeS1P_MatchS1->Reset();
   fhClusterAdcVsSizeS2N_MatchS2->Reset();
   fhClusterAdcVsSizeS2P_MatchS2->Reset();

   fhClusterS1N_CenterNbMatchS1P->Reset();
   fhClusterS1P_CenterNbMatchS1N->Reset();
   fhClusterS2N_CenterNbMatchS2P->Reset();
   fhClusterS2P_CenterNbMatchS2N->Reset();

   fhClusterS1N_AdcNbMatchS1P->Reset();
   fhClusterS1P_AdcNbMatchS1N->Reset();
   fhClusterS2N_AdcNbMatchS2P->Reset();
   fhClusterS2P_AdcNbMatchS2N->Reset();

   fhDtNeighborChansH1X->Reset();
   fhDtNeighborChansH1Y->Reset();
   fhDtNeighborChansH2X->Reset();
   fhDtNeighborChansH2Y->Reset();

   fhClusterAdcVsSizeH1X->Reset();
   fhClusterAdcVsSizeH1Y->Reset();
   fhClusterAdcVsSizeH2X->Reset();
   fhClusterAdcVsSizeH2Y->Reset();
   fhClusterPosition_MatchH1->Reset();
   fhClusterPosition_MatchH2->Reset();
   ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   for( UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; ++uLinks )
   {
      TString sMsSzName = Form("MsSz_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->H1( sMsSzName.Data() )->Reset();

      sMsSzName = Form("MsSzTime_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->P1( sMsSzName.Data() )->Reset();
   } // for( UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; ++uLinks )

  fdStartTime = -1;
  fdStartTimeMsSz = -1;
}

void CbmCosy2018MonitorEfficiency::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}

void CbmCosy2018MonitorEfficiency::SetLongDurationLimits( UInt_t uDurationSeconds, UInt_t uBinSize )
{
   fbLongHistoEnable     = kTRUE;
   fuLongHistoNbSeconds  = uDurationSeconds;
   fuLongHistoBinSizeSec = uBinSize;
}

void CbmCosy2018MonitorEfficiency::ComputeCoordinatesHodo1( Int_t iChanX, Int_t iChanY, Double_t & dPosX, Double_t & dPosY )
{
   dPosX = 0.0;
   dPosY = 0.0;

   UInt_t uFiberIdxX1 = fUnpackParHodo->GetChannelToFiberMap( iChanX );
   if( fUnpackParHodo->IsXInvertedHodo1() )
      uFiberIdxX1 = fuNbChanPerAsic/2 - 1 - uFiberIdxX1;

   UInt_t uFiberIdxY1 = fUnpackParHodo->GetChannelToFiberMap( iChanY );
   if( fUnpackParHodo->IsYInvertedHodo1() )
      uFiberIdxY1 = fuNbChanPerAsic/2 - 1 - uFiberIdxY1;

   dPosX = kdPitchHodoMm * uFiberIdxX1 - kdHodoMidXY;
   dPosY = kdPitchHodoMm * uFiberIdxY1 - kdHodoMidXY;
}

void CbmCosy2018MonitorEfficiency::ComputeCoordinatesHodo2( Int_t iChanX, Int_t iChanY, Double_t & dPosX, Double_t & dPosY )
{
   dPosX = 0.0;
   dPosY = 0.0;

   UInt_t uFiberIdxX2 = fUnpackParHodo->GetChannelToFiberMap( iChanX );
   if( fUnpackParHodo->IsXInvertedHodo2() )
      uFiberIdxX2 = fuNbChanPerAsic/2 - 1 - uFiberIdxX2;

   UInt_t uFiberIdxY2 = fUnpackParHodo->GetChannelToFiberMap( iChanY );
   if( fUnpackParHodo->IsYInvertedHodo2() )
      uFiberIdxY2 = fuNbChanPerAsic/2 - 1 - uFiberIdxY2;

   dPosX = kdPitchHodoMm * uFiberIdxX2 - kdHodoMidXY;
   dPosY = kdPitchHodoMm * uFiberIdxY2 - kdHodoMidXY;
}

void CbmCosy2018MonitorEfficiency::ComputeCoordinatesSensor1( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY )
{
   dPosX = 0.0;
   dPosY = 0.0;

   Int_t iCoordN = iChanN + fiStripsOffsetN1;
   Int_t iCoordP = iChanP + fiStripsOffsetP1;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      iCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      iCoordP *= -1;

   dPosX =   iCoordN             * kdPitchMm;
   dPosY = ( iCoordN + iCoordP ) * kdPitchMm / fdStereoAngleTan + kdSensorsSzY / 2.0;
}

void CbmCosy2018MonitorEfficiency::ComputeCoordinatesSensor2( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY )
{
   dPosX = 0.0;
   dPosY = 0.0;

   Int_t iCoordN = iChanN + fiStripsOffsetN2;
   Int_t iCoordP = iChanP + fiStripsOffsetP2;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      iCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      iCoordP *= -1;

   dPosX =   iCoordN             * kdPitchMm;
   dPosY = ( iCoordN + iCoordP ) * kdPitchMm / fdStereoAngleTan + kdSensorsSzY / 2.0;
}

void CbmCosy2018MonitorEfficiency::ComputeProjectionSensor1( Double_t dPosXH1, Double_t dPosYH1,
                                                             Double_t dPosXH2, Double_t dPosYH2,
                                                             Double_t & dPosX, Double_t & dPosY )
{
   dPosX = ( dPosXH2 - dPosXH1 ) * fdRatioZS1 + dPosXH1;
   dPosY = ( dPosYH2 - dPosYH1 ) * fdRatioZS1 + dPosYH1;
}

void CbmCosy2018MonitorEfficiency::ComputeProjectionSensor2( Double_t dPosXH1, Double_t dPosYH1,
                                                             Double_t dPosXH2, Double_t dPosYH2,
                                                             Double_t & dPosX, Double_t & dPosY )
{
   dPosX = ( dPosXH2 - dPosXH1 ) * fdRatioZS2 + dPosXH1;
   dPosY = ( dPosYH2 - dPosYH1 ) * fdRatioZS2 + dPosYH1;
}

Bool_t CbmCosy2018MonitorEfficiency::CheckPhysPairSensor1( Int_t iChanN, Int_t iChanP )
{
   // Position in P/N strips
   Int_t iCoordN = iChanN + fiStripsOffsetN1;
   Int_t iCoordP = iChanP + fiStripsOffsetP1;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      iCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      iCoordP *= -1;

//   if( iCoordP < - iCoordN - fdStripRangeNP / 2 || - iCoordN + fdStripRangeNP / 2 < iCoordP  )
   if( iCoordP < - iCoordN - fdStripRangeNP || - iCoordN < iCoordP  )
      return kFALSE;
      else return kTRUE;
}
Bool_t CbmCosy2018MonitorEfficiency::CheckPhysPairSensor2( Int_t iChanN, Int_t iChanP )
{
   // Position in P/N strips
   Int_t iCoordN = iChanN + fiStripsOffsetN2;
   Int_t iCoordP = iChanP + fiStripsOffsetP2;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      iCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      iCoordP *= -1;

//   if( iCoordP < - iCoordN - fdStripRangeNP / 2 || - iCoordN + fdStripRangeNP / 2 < iCoordP  )
   if( iCoordP < -iCoordN - fdStripRangeNP || -iCoordN < iCoordP  )
      return kFALSE;
      else return kTRUE;
}

Bool_t CbmCosy2018MonitorEfficiency::CheckPointInsideSensor1( Double_t dPosX, Double_t dPosY )
{
   // Input Position in mm inside sensor frame
   Double_t dCoordN = dPosX / kdPitchMm;
   Double_t dCoordP = ( dPosY - kdSensorsSzY / 2.0 ) * fdStereoAngleTan / kdPitchMm - dPosX / kdPitchMm;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      dCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      dCoordP *= -1;

   Int_t iChanN = dCoordN - fiStripsOffsetN1;
   Int_t iChanP = dCoordP - fiStripsOffsetP1;

   if( iChanN < 0 || 127 < iChanN || iChanP < 0 || 127 < iChanP )
      return kFALSE;

   return CheckPhysPairSensor1( iChanN, iChanP );
}

Bool_t CbmCosy2018MonitorEfficiency::CheckPointInsideSensor2( Double_t dPosX, Double_t dPosY )
{
   // Input Position in mm inside sensor frame
   Double_t dCoordN = dPosX / kdPitchMm;
   Double_t dCoordP = ( dPosY - kdSensorsSzY / 2.0 ) * fdStereoAngleTan / kdPitchMm - dPosX / kdPitchMm;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      dCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      dCoordP *= -1;

   Int_t iChanN = dCoordN - fiStripsOffsetN2;
   Int_t iChanP = dCoordP - fiStripsOffsetP2;

   if( iChanN < 0 || 127 < iChanN || iChanP < 0 || 127 < iChanP )
      return kFALSE;

   return CheckPhysPairSensor2( iChanN, iChanP );
}

Bool_t CbmCosy2018MonitorEfficiency::CheckPointInsideSensor1NoDead( Double_t dPosX, Double_t dPosY )
{
   // Input Position in mm inside sensor frame

   Double_t dCoordN = dPosX / kdPitchMm;
   Double_t dCoordP = ( dPosY - kdSensorsSzY / 2.0 ) * fdStereoAngleTan / kdPitchMm - dPosX / kdPitchMm;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      dCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      dCoordP *= -1;

   Int_t iChanN = dCoordN - fiStripsOffsetN1;
   Int_t iChanP = dCoordP - fiStripsOffsetP1;

   if( iChanN < 0 || 127 < iChanN || iChanP < 0 || 127 < iChanP )
      return kFALSE;

   for( UInt_t uDeadN = 0; uDeadN < kuNbDeadStripsS1N; ++uDeadN )
      if( kuDeadStripsS1N[uDeadN] == static_cast<UInt_t>( iChanN ) )
         return kFALSE;
   for( UInt_t uDeadP = 0; uDeadP < kuNbDeadStripsS1P; ++uDeadP )
      if( kuDeadStripsS1P[uDeadP] == static_cast<UInt_t>( iChanP ) )
         return kFALSE;

   return CheckPhysPairSensor1( iChanN, iChanP );
}

Bool_t CbmCosy2018MonitorEfficiency::CheckPointInsideSensor2NoDead( Double_t dPosX, Double_t dPosY )
{
   // Position in mm inside sensor frame

   Double_t dCoordN = dPosX / kdPitchMm;
   Double_t dCoordP = ( dPosY - kdSensorsSzY / 2.0 ) * fdStereoAngleTan / kdPitchMm - dPosX / kdPitchMm;

   // TODO: Add inversion to STS param
//   if( fUnpackParSts->IsXInvertedS2() )
      dCoordN *= -1;
//   if( fUnpackParSts->IsYInvertedS2() )
      dCoordP *= -1;

   Int_t iChanN = dCoordN - fiStripsOffsetN2;
   Int_t iChanP = dCoordP - fiStripsOffsetP2;

   if( iChanN < 0 || 127 < iChanN || iChanP < 0 || 127 < iChanP )
      return kFALSE;

   for( UInt_t uDeadN = 0; uDeadN < kuNbDeadStripsS2N; ++uDeadN )
      if( kuDeadStripsS2N[uDeadN] == static_cast<UInt_t>( iChanN ) )
         return kFALSE;
   for( UInt_t uDeadP = 0; uDeadP < kuNbDeadStripsS2P; ++uDeadP )
      if( kuDeadStripsS2P[uDeadP] == static_cast<UInt_t>( iChanP ) )
         return kFALSE;

   return CheckPhysPairSensor2( iChanN, iChanP );
}

ClassImp(CbmCosy2018MonitorEfficiency)
