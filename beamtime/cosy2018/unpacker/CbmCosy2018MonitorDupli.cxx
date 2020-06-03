// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorDupli                        -----
// -----                Created 27/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCosy2018MonitorDupli.h"

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
#include "THStack.h"
#include <TFile.h>

// C++11

// C/C++
#include <iostream>
#include <stdint.h>
#include <iomanip>

Bool_t bCosy2018ResetDupliHistos = kFALSE;
Bool_t bCosy2018WriteDupliHistos = kFALSE;
Bool_t bCosy2018PrintMessDupliHistos = kFALSE;

CbmCosy2018MonitorDupli::CbmCosy2018MonitorDupli() :
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
   fkuNbPrintMessages(5000),
   fuPrintMessagesIdx(fkuNbPrintMessages),
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
   fhStsRawAdcCombine(NULL),
   fhStsRawAdc(),
   fhHodoChanCntRaw(),
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
   fdCoincBorderHodo(  50.0 ),
   fdCoincBorderSts(   75.0 ),
   fdCoincBorder(     150.0 ),
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
   fhBothHodoSortedDtX1Y1(NULL),
   fhBothHodoSortedDtX2Y2(NULL),
   fhBothHodoSortedDtX1Y1X2Y2N1P1(NULL),
   fhBothHodoSortedDtX1Y1X2Y2N2P2(NULL),
   fhBothHodoSortedDtH1H2S1S2(NULL),
   fhBothHodoSortedMapX1Y1(NULL),
   fhBothHodoSortedMapX2Y2(NULL),
   fhBothHodoSortedCntEvoX1Y1(NULL),
   fhBothHodoSortedCntEvoX2Y2(NULL),
   fhH1H2S1SortedDtX1Y1(NULL),
   fhH1H2S1SortedDtX2Y2(NULL),
   fhH1H2S1SortedDtN1P1(NULL),
   fhH1H2S1SortedDtN2P2(NULL),
   fhH1H2S1SortedMapX1Y1(NULL),
   fhH1H2S1SortedMapX2Y2(NULL),
   fhH1H2S1SortedMapN1P1(NULL),
   fhH1H2S1SortedMapN2P2(NULL),
   fhH1H2S1SortedCntEvoX1Y1(NULL),
   fhH1H2S1SortedCntEvoX2Y2(NULL),
   fhH1H2S1SortedCntEvoN1P1(NULL),
   fhH1H2S1SortedCntEvoN2P2(NULL),
   fhH1H2S2SortedDtX1Y1(NULL),
   fhH1H2S2SortedDtX2Y2(NULL),
   fhH1H2S2SortedDtN1P1(NULL),
   fhH1H2S2SortedDtN2P2(NULL),
   fhH1H2S2SortedMapX1Y1(NULL),
   fhH1H2S2SortedMapX2Y2(NULL),
   fhH1H2S2SortedMapN1P1(NULL),
   fhH1H2S2SortedMapN2P2(NULL),
   fhH1H2S2SortedCntEvoX1Y1(NULL),
   fhH1H2S2SortedCntEvoX2Y2(NULL),
   fhH1H2S2SortedCntEvoN1P1(NULL),
   fhH1H2S2SortedCntEvoN2P2(NULL),
   fhSystSortedDtX1Y1(NULL),
   fhSystSortedDtX2Y2(NULL),
   fhSystSortedDtN1P1(NULL),
   fhSystSortedDtN2P2(NULL),
   fhSystSortedDtX1Y1X2Y2(NULL),
   fhSystSortedMapX1Y1(NULL),
   fhSystSortedMapX2Y2(NULL),
   fhSystSortedMapN1P1(NULL),
   fhSystSortedMapN2P2(NULL),
   fhSystSortedCntEvoX1Y1(NULL),
   fhSystSortedCntEvoX2Y2(NULL),
   fhSystSortedCntEvoN1P1(NULL),
   fhSystSortedCntEvoN2P2(NULL),
   fhSystSortedDtN1X1vsN1X2(NULL),
   fhSystSortedDtP1X1vsP1X2(NULL),
   fhSystSortedDtN1X1vsP1X1(NULL),
   fhSystSortedDtSts1Hodo1vsSts1Hodo2(NULL),
   fhSystSortedDtSts2Hodo1vsSts2Hodo2(NULL),
   fhSystSortedDtAllVsMapX1(NULL),
   fhSystSortedDtAllVsMapY1(NULL),
   fhSystSortedDtAllVsMapX2(NULL),
   fhSystSortedDtAllVsMapY2(NULL),
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
   fhSetupSortedTsFirstDuplicateChan(),
   fhSetupSortedTsGoodChan(),
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
   fhRatioSameTsSpillEvo(),
   fhRatioSameTsAdcSpillEvo(),
   fhRatioSameAdcSameTsSpillEvo(),
   fhFractionHitsAsicSpillEvo(),
   fhFractionGoodHitsSpillEvo(),
   fhFractionDupliHitsSpillEvo(),
   fhFractionTsMsbSpillEvo(),
   fhFractionEpochSpillEvo(),
   fhFractionEmptySpillEvo(),
   fhFractionHitsAsicEvo(),
   fhFractionGoodHitsEvo(),
   fhFractionDupliHitsEvo(),
   fhFractionTsMsbEvo(),
   fhFractionEpochEvo(),
   fhFractionEmptyEvo(),
   fhFractionAsics( NULL ),
   fhFractionTypes( NULL ),
   fvuNbSameFullHitAsic(),
   fvuNbDiffFullHitAsic(),
   fvuLastHitBufferIdx(),
   fvmLastHitsAsic(),
   fhAsicDuplicDtLastHits(),
   fhAsicDuplicCompTs(),
   fhAsicDuplicTsLsb(),
   fhAsicDuplicTsMsbAsic(),
   fhAsicDuplicCompTsBitThere(),
   fhAsicDuplicTsLsbBitThere(),
   fhAsicDuplicTsMsbBitThere(),
   fhAsicDuplicTs(),
   fhAsicGoodTs(),
   fhAsicDuplicTsMsb(),
   fhAsicGoodTsMsb(),
   fhAsicDuplicTsFull(),
   fhAsicGoodTsFull(),
   fhAsicDuplicTsBitPattern(),
   fhAsicGoodTsBitPattern(),
   fhAsicDuplicTsEvoAsic0Chan01( NULL ),
   fhAsicGoodTsEvoAsic0Chan01( NULL ),
   fhAsicGoodTsMsbEvoAsic0Chan01( NULL ),
   fhAsicGoodTsFullEvoAsic0Chan01( NULL ),
   fhAsicGoodTsFullEvoProfAsic0Chan01( NULL ),
   fhAsicDuplicTsEvoAsic0Chan09( NULL ),
   fhAsicGoodTsEvoAsic0Chan09( NULL ),
   fhAsicGoodTsFullEvoProfAsic0Chan09( NULL ),
   fhAsicDuplicTsEvoAsic0Chan16( NULL ),
   fhAsicGoodTsEvoAsic0Chan16( NULL ),
   fhAsicGoodTsFullEvoProfAsic0Chan16( NULL ),
   fuPulseIdx( 0 ),
   fuPulseIdxMax( 32000 ),
   fhAsicMissedChanIdVsPulseIdx( NULL ),
   fhAsicMissedChanGroupVsPulseIdx( NULL ),
   fhHodoChanCntGood(),
   fhHodoChanGoodHitRateEvo(),
   fhHodoX1SpillEvo(NULL),
   fhHodoY1SpillEvo(NULL),
   fhHodoX2SpillEvo(NULL),
   fhHodoY2SpillEvo(NULL),
   fdSpillEvoLength( 700.0 ),
   fhHodoX1SpillEvoProf(NULL),
   fhHodoY1SpillEvoProf(NULL),
   fhHodoX2SpillEvoProf(NULL),
   fhHodoY2SpillEvoProf(NULL),
   fcMsSizeAll(NULL),
   fvuAsicTimeLastPulse(),
   fvbPulseThereChan(),
   fhPulseChanCountEvo(),
   fhHodoChanHitRateEvoZoom(NULL),
   fuNbTsMsbSinceLastHit(0),
   fuNbHitsLastTsMsb(0)
{
}

CbmCosy2018MonitorDupli::~CbmCosy2018MonitorDupli()
{
}

Bool_t CbmCosy2018MonitorDupli::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmCosy2018MonitorDupli::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackParHodo = (CbmCern2017UnpackParHodo*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParHodo"));
   fUnpackParSts  = (CbmCern2017UnpackParSts*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParSts"));
}


Bool_t CbmCosy2018MonitorDupli::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmCosy2018MonitorDupli::ReInitContainers()
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
   LOG(info) << "CbmCosy2018MonitorDupli::ReInitContainers => Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
   LOG(info) << "CbmCosy2018MonitorDupli::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
   LOG(info) << "CbmCosy2018MonitorDupli::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
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
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fvuAsicTimeLastPulse.resize( fuNbStsXyters );
   fvbPulseThereChan.resize( fuNbStsXyters );
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fvuAsicTimeLastPulse[ uXyterIdx ] = 0;
      fvbPulseThereChan[ uXyterIdx ].resize( fuNbChanPerAsic );
      for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
      {
         fvbPulseThereChan[ uXyterIdx ][ uChan ] = kFALSE;
      } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fvuNbSameFullHitAsic.resize( fuNbStsXyters );
   fvuNbDiffFullHitAsic.resize( fuNbStsXyters );
   fvuLastHitBufferIdx.resize( fuNbStsXyters );
   fvmLastHitsAsic.resize( fuNbStsXyters );
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fvuNbSameFullHitAsic[ uXyterIdx ] = 0;
      fvuNbDiffFullHitAsic[ uXyterIdx ] = 0;
      fvuLastHitBufferIdx[ uXyterIdx ] = 0;
      fvmLastHitsAsic[ uXyterIdx ].resize( fuNbChanPerAsic );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   LOG(info) << "CbmCosy2018MonitorDupli::ReInitContainers =>  Dual STS mode:  " << fbDualStsEna;
   return kTRUE;
}

void CbmCosy2018MonitorDupli::CreateHistograms()
{
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
      fhHodoFebRateEvoLong.push_back( new TH1D(sHistName, title, fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5 ) );

/*
      if( kTRUE == fbLongHistoEnable )
      {

         sHistName = Form( "hFebRateEvoLong_%03u", uXyterIdx );
         title = Form( "Mean rate VS run time in same MS in StsXyter #%03u; Time in run [s]; Rate [1/s]", uXyterIdx );
         fhFebRateEvoLong.push_back( new TH1D( sHistName, title,
                                                   fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5) );
         if( server ) server->Register("/StsRaw", fhFebRateEvoLong[ uXyterIdx ] );

         sHistName = Form( "hFebChRateEvoLong_%03u", uXyterIdx );
         title = Form( "Mean rate per channel VS run time in StsXyter #%03u; Time in run [s]; Channel []; Rare [1/s]", uXyterIdx );
         fhFebChRateEvoLong.push_back( new TH2D( sHistName, title,
                                                   fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
         if( server ) server->Register("/StsRaw", fhFebChRateEvoLong[ uXyterIdx ] );
      } // if( kTRUE == fbLongHistoEnable )
*/
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   UInt_t uNbBinEvo = 32768 + 1;
   Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

   UInt_t uNbBinDtHodo     = static_cast<UInt_t>(   fdCoincBorderHodo/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtBothHodo = static_cast<UInt_t>( 2*fdCoincBorderHodo/stsxyter::kdClockCycleNs );
   UInt_t uNbBinDtSts      = static_cast<UInt_t>(   fdCoincBorderSts/stsxyter::kdClockCycleNs  );
//   UInt_t uNbBinDtSyst     = static_cast<UInt_t>(   fdCoincBorder/stsxyter::kdClockCycleNs  );

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

   /// Coincidences between hodoscopes ------------------------------///
   sHistName = "fhBothHodoSortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhBothHodoSortedDtX1Y1 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhBothHodoSortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhBothHodoSortedDtX2Y2 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhBothHodoSortedDtX1Y1X2Y2N1P1";
   title =  "Time diff for hits Hodo (X1,Y1,X2,Y2) vs Sts 1 (N,P); t<N1,P1> - t<X1,Y1,X2,Y2> [ns]; Counts";
   fhBothHodoSortedDtX1Y1X2Y2N1P1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhBothHodoSortedDtX1Y1X2Y2N2P2";
   title =  "Time diff for hits Hodo (X1,Y1,X2,Y2) vs Sts 2 (N,P); t<N2,P2> - t<X1,Y1,X2,Y2> [ns]; Counts";
   fhBothHodoSortedDtX1Y1X2Y2N2P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhBothHodoSortedDtH1H2S1S2";
   title =  "Time diff for hits Hodo (X1,Y1,X2,Y2) vs Sts (N1,P1,N2,P2); t<N1,P1,N2,P2> - t<X1,Y1,X2,Y2> [ns]; Counts";
   fhBothHodoSortedDtH1H2S1S2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   sHistName = "fhBothHodoSortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and Y; X channel Hodo 1 []; Y channel Hodo 1 []; hits []";
   fhBothHodoSortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhBothHodoSortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 2 axis X and Y; X channel Hodo 2 []; Y channel Hodo 2 []; hits []";
   fhBothHodoSortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   // Coincidence counts evolution between some axis of the hodoscopes
   sHistName = "hBothHodoSortedCntEvoX1Y1";
   title = "Nb of coincidences in both X1 and Y1 per s; Time [s]; X1-Y1 coincidences []";
   fhBothHodoSortedCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hBothHodoSortedCntEvoX2Y2";
   title = "Nb of coincidences in both X2 and Y2 per s; Time [s]; X2-Y2 coincidences []";
   fhBothHodoSortedCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   /// Coincidences between hodoscopes and STS 1 --------------------///
   sHistName = "fhH1H2S1SortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhH1H2S1SortedDtX1Y1 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhH1H2S1SortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhH1H2S1SortedDtX2Y2 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhH1H2S1SortedDtN1P1";
   title =  "Time diff for hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhH1H2S1SortedDtN1P1 = new TH1I(sHistName, title, uNbBinDtSts, -fdCoincBorderSts, fdCoincBorderSts );

   sHistName = "fhH1H2S1SortedDtN2P2";
   title =  "Time diff for hits Sts 2 N and Sts 1 P; tP2 - tN2 [ns]; Counts";
   fhH1H2S1SortedDtN2P2 = new TH1I(sHistName, title, uNbBinDtSts, -fdCoincBorderSts, fdCoincBorderSts );

   sHistName = "fhH1H2S1SortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and Y; X channel Hodo 1 []; Y channel Hodo 1 []; hits []";
   fhH1H2S1SortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhH1H2S1SortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 2 axis X and Y; X channel Hodo 2 []; Y channel Hodo 2 []; hits []";
   fhH1H2S1SortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhH1H2S1SortedMapN1P1";
   title = "Sorted hits in coincidence for Sts 1 sides N and P; N channel Sts 1 []; P channel Sts 1 []; hits []";
   fhH1H2S1SortedMapN1P1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhH1H2S1SortedMapN2P2";
   title = "Sorted hits in coincidence for Sts 2 sides N and P; N channel Sts 2 []; P channel Sts 2 []; hits []";
   fhH1H2S1SortedMapN2P2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "hH1H2S1SortedCntEvoX1Y1";
   title = "Nb of coincidences in both X1 and Y1 per s; Time [s]; X1-Y1 coincidences []";
   fhH1H2S1SortedCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hH1H2S1SortedCntEvoX2Y2";
   title = "Nb of coincidences in both X2 and Y2 per s; Time [s]; X2-Y2 coincidences []";
   fhH1H2S1SortedCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hH1H2S1SortedCntEvoN1P1";
   title = "Nb of coincidences in both N1 and P1 per s; Time [s]; N1-P1 coincidences []";
   fhH1H2S1SortedCntEvoN1P1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hH1H2S1SortedCntEvoN2P2";
   title = "Nb of coincidences in both N2 and P2 per s; Time [s]; N2-P2 coincidences []";
   fhH1H2S1SortedCntEvoN2P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   /// Coincidences between hodoscopes and STS 2 --------------------///
   sHistName = "fhH1H2S2SortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhH1H2S2SortedDtX1Y1 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhH1H2S2SortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhH1H2S2SortedDtX2Y2 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhH1H2S2SortedDtN1P1";
   title =  "Time diff for hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhH1H2S2SortedDtN1P1 = new TH1I(sHistName, title, uNbBinDtSts, -fdCoincBorderSts, fdCoincBorderSts );

   sHistName = "fhH1H2S2SortedDtN2P2";
   title =  "Time diff for hits Sts 2 N and Sts 1 P; tP2 - tN2 [ns]; Counts";
   fhH1H2S2SortedDtN2P2 = new TH1I(sHistName, title, uNbBinDtSts, -fdCoincBorderSts, fdCoincBorderSts );

   sHistName = "fhH1H2S2SortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and Y; X channel Hodo 1 []; Y channel Hodo 1 []; hits []";
   fhH1H2S2SortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhH1H2S2SortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 2 axis X and Y; X channel Hodo 2 []; Y channel Hodo 2 []; hits []";
   fhH1H2S2SortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhH1H2S2SortedMapN1P1";
   title = "Sorted hits in coincidence for Sts 1 sides N and P; N channel Sts 1 []; P channel Sts 1 []; hits []";
   fhH1H2S2SortedMapN1P1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhH1H2S2SortedMapN2P2";
   title = "Sorted hits in coincidence for Sts 2 sides N and P; N channel Sts 2 []; P channel Sts 2 []; hits []";
   fhH1H2S2SortedMapN2P2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "hH1H2S2SortedCntEvoX1Y1";
   title = "Nb of coincidences in both X1 and Y1 per s; Time [s]; X1-Y1 coincidences []";
   fhH1H2S2SortedCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hH1H2S2SortedCntEvoX2Y2";
   title = "Nb of coincidences in both X2 and Y2 per s; Time [s]; X2-Y2 coincidences []";
   fhH1H2S2SortedCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hH1H2S2SortedCntEvoN1P1";
   title = "Nb of coincidences in both N1 and P1 per s; Time [s]; N1-P1 coincidences []";
   fhH1H2S2SortedCntEvoN1P1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hH1H2S2SortedCntEvoN2P2";
   title = "Nb of coincidences in both N2 and P2 per s; Time [s]; N2-P2 coincidences []";
   fhH1H2S2SortedCntEvoN2P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   /// Coincidences between hodoscopes and both STS -----------------///
   sHistName = "fhSystSortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhSystSortedDtX1Y1 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhSystSortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhSystSortedDtX2Y2 = new TH1I(sHistName, title, uNbBinDtHodo, -fdCoincBorderHodo, fdCoincBorderHodo );

   sHistName = "fhSystSortedDtN1P1";
   title =  "Time diff for hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhSystSortedDtN1P1 = new TH1I(sHistName, title, uNbBinDtSts, -fdCoincBorderSts, fdCoincBorderSts );

   sHistName = "fhSystSortedDtN2P2";
   title =  "Time diff for hits Sts 2 N and Sts 1 P; tP2 - tN2 [ns]; Counts";
   fhSystSortedDtN2P2 = new TH1I(sHistName, title, uNbBinDtSts, -fdCoincBorderSts, fdCoincBorderSts );

   sHistName = "fhSystSortedDtX1Y1X2Y2";
   title =  "Time diff for hits Hodo 1 (X,Y) vs Hodo 2 (X,Y); t<X2,Y2> - t<X1,Y1> [ns]; Counts";
   fhSystSortedDtX1Y1X2Y2 = new TH1I(sHistName, title, uNbBinDtBothHodo, -2*fdCoincBorderHodo, 2*fdCoincBorderHodo );

   sHistName = "fhSystSortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and Y; X channel Hodo 1 []; Y channel Hodo 1 []; hits []";
   fhSystSortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSystSortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 2 axis X and Y; X channel Hodo 2 []; Y channel Hodo 2 []; hits []";
   fhSystSortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSystSortedMapN1P1";
   title = "Sorted hits in coincidence for Sts 1 sides N and P; N channel Sts 1 []; P channel Sts 1 []; hits []";
   fhSystSortedMapN1P1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhSystSortedMapN2P2";
   title = "Sorted hits in coincidence for Sts 2 sides N and P; N channel Sts 2 []; P channel Sts 2 []; hits []";
   fhSystSortedMapN2P2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   // Coincidence counts evolution between some axis of the hodoscopes
   sHistName = "hSystSortedCntEvoX1Y1";
   title = "Nb of coincidences in both X1 and Y1 per s; Time [s]; X1-Y1 coincidences []";
   fhSystSortedCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hSystSortedCntEvoX2Y2";
   title = "Nb of coincidences in both X2 and Y2 per s; Time [s]; X2-Y2 coincidences []";
   fhSystSortedCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hSystSortedCntEvoN1P1";
   title = "Nb of coincidences in both N1 and P1 per s; Time [s]; N1-P1 coincidences []";
   fhSystSortedCntEvoN1P1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "hSystSortedCntEvoN2P2";
   title = "Nb of coincidences in both N2 and P2 per s; Time [s]; N2-P2 coincidences []";
   fhSystSortedCntEvoN2P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

   sHistName = "fhSystSortedDtN1X1vsN1X2";
   title =  "Time diff (N1 vs X1) vs (N1 vs X2); tN1 - tX1 [ns]; tN1 - tX2 [ns]; Counts";
   fhSystSortedDtN1X1vsN1X2 = new TH2I(sHistName, title, 200, -400, 400, 200, -400, 400 );

   sHistName = "fhSystSortedDtP1X1vsP1X2";
   title =  "Time diff (P1 vs X1) vs (P1 vs X2); tP1 - tX1 [ns]; tP1 - tX2 [ns]; Counts";
   fhSystSortedDtP1X1vsP1X2 = new TH2I(sHistName, title, 200, -400, 400, 200, -400, 400 );

   sHistName = "fhSystSortedDtN1X1vsP1X1";
   title =  "Time diff (N1 vs X1) vs (P1 vs X1); tN1 - tX1 [ns]; tP1 - tX1 [ns]; Counts";
   fhSystSortedDtN1X1vsP1X1 = new TH2I(sHistName, title, 200, -400, 400, 200, -400, 400 );

   sHistName = "fhSystSortedDtSts1Hodo1vsSts1Hodo2";
   title =  "Time diff (Sts1 vs Hodo1) vs (Sts1 vs Hodo2); t<N1,P1> - t<X1,Y1> [ns]; t<N1,P1> - t<X2,Y2> [ns]; Counts";
   fhSystSortedDtSts1Hodo1vsSts1Hodo2 = new TH2I(sHistName, title, 200, -400, 400, 200, -400, 400 );

   sHistName = "fhSystSortedDtSts2Hodo1vsSts2Hodo2";
   title =  "Time diff (Sts2 vs Hodo1) vs (Sts2 vs Hodo2); t<N2,P2> - t<X1,Y1> [ns]; t<N2,P2> - t<X2,Y2> [ns]; Counts";
   fhSystSortedDtSts2Hodo1vsSts2Hodo2 = new TH2I(sHistName, title, 200, -400, 400, 200, -400, 400 );

   sHistName = "fhSystSortedDtAllVsMapX1";
   title = "Time diff Hodo (X1,Y1,X2,Y2) Sts 1 (N,P) vs Hodo 1 axis X; t<N1,P1> - t<X1,Y1,X2,Y2> [ns]; X channel Hodo 1 []; hits []";
   fhSystSortedDtAllVsMapX1 = new TH2I( sHistName, title,
                                  static_cast< Int_t >(2*fdCoincBorder), -fdCoincBorder, fdCoincBorder,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSystSortedDtAllVsMapY1";
   title = "Time diff Hodo (X1,Y1,X2,Y2) Sts 1 (N,P) vs Hodo 1 axis Y; t<N1,P1> - t<X1,Y1,X2,Y2> [ns]; Y channel Hodo 1 []; hits []";
   fhSystSortedDtAllVsMapY1 = new TH2I( sHistName, title,
                                  static_cast< Int_t >(2*fdCoincBorder), -fdCoincBorder, fdCoincBorder,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSystSortedDtAllVsMapX2";
   title = "Time diff Hodo (X1,Y1,X2,Y2) Sts 1 (N,P) vs Hodo 2 axis X; t<N1,P1> - t<X1,Y1,X2,Y2> [ns]; X channel Hodo 2 []; hits []";
   fhSystSortedDtAllVsMapX2 = new TH2I( sHistName, title,
                                  static_cast< Int_t >(2*fdCoincBorder), -fdCoincBorder, fdCoincBorder,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );

   sHistName = "fhSystSortedDtAllVsMapY2";
   title = "Time diff Hodo (X1,Y1,X2,Y2) Sts 1 (N,P) vs Hodo 2 axis Y; t<N1,P1> - t<X1,Y1,X2,Y2> [ns]; Y channel Hodo 2 []; hits []";
   fhSystSortedDtAllVsMapY2 = new TH2I( sHistName, title,
                                  static_cast< Int_t >(2*fdCoincBorder), -fdCoincBorder, fdCoincBorder,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
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
      sHistName = Form( "fhSetupSortedTsFirstDuplicateChan%02u", uXyterIdx);
      title =  Form( "TS of first duplicate hit in a burst per channel Asic %02u; Channel []; TS [bin]; Bursts []", uXyterIdx);
      fhSetupSortedTsFirstDuplicateChan.push_back( new TH2I( sHistName, title,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                                512, -0.5, 511.5 ) );

      sHistName = Form( "fhSetupSortedTsGoodChan%02u", uXyterIdx);
      title =  Form( "TS of non duplicated hits per channel Asic %02u; Channel []; TS [bin]; Bursts []", uXyterIdx);
      fhSetupSortedTsGoodChan.push_back( new TH2I( sHistName, title,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                                512, -0.5, 511.5 ) );

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
                                 256, -0.5, 255.5 ) );

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
      title = Form( "Ratio of hits with same ADC among same TS vs Hit flux Asic %02u; Hits flux [1/s]; Same TS and ADC / Same TS []; []", uXyterIdx);
      fhSetupSortedAsicRatioSameAdcSameTsVsFlux.push_back( new TH2I(sHistName, title,
                                 iNbBinsRate - 1, dBinsRate,
                                 101, -0.005, 1.005 ) );

      sHistName = Form( "fhRatioSameTsSpillEvo%02u", uXyterIdx);
      title = Form( "Ratio of hits with same TS Hits vs time Asic %02u; time [s]; channel []; Same Ts / All Hits []; []", uXyterIdx);
      fhRatioSameTsSpillEvo.push_back( new TProfile2D(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "fhRatioSameTsAdcSpillEvo%02u", uXyterIdx);
      title = Form( "Ratio of hits with same ADC and same TS hits vs time Asic %02u; time [s]; channel []; Same Ts and ADC / All Hits []; []", uXyterIdx);
      fhRatioSameTsAdcSpillEvo.push_back( new TProfile2D(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "fhRatioSameAdcSameTsSpillEvo%02u", uXyterIdx);
      title = Form( "Ratio of hits with same ADC among same TS vs time Asic %02u; time [s]; channel []; Same TS and ADC / Same TS []; []", uXyterIdx);
      fhRatioSameAdcSameTsSpillEvo.push_back( new TProfile2D(sHistName, title,
                              70000, 0, 70.0,
                              fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "fhFractionHitsAsicSpillEvo%02u", uXyterIdx);
      title = Form( "Fraction of all hits vs time for Asic %02u; time [s]; Hits in Asic %02u []", uXyterIdx, uXyterIdx);
      fhFractionHitsAsicSpillEvo.push_back( new TProfile(sHistName, title, 210000, 0, 210.0 ) );

      sHistName = Form( "fhFractionHitsAsicEvo%02u", uXyterIdx);
      title = Form( "Fraction of all hits vs time for Asic %02u; time [s]; Hits in Asic %02u []", uXyterIdx, uXyterIdx);
      fhFractionHitsAsicEvo.push_back( new TProfile(sHistName, title, 1800, 0, 1800 ) );

      // Channel counts
      sHistName = Form( "hHodoChanCntGood_%03u", uXyterIdx );
      title = Form( "Good Hits Count per channel, StsXyter #%03u; Channel; Good Hits []", uXyterIdx );
      fhHodoChanCntGood.push_back( new TH1I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      // Hit rates evo per channel
      sHistName = Form( "hHodoChanGoodRateEvo_%03u", uXyterIdx );
      title = Form( "Good Hits per second & channel in StsXyter #%03u; Time [s]; Channel []; Good Hits []", uXyterIdx );
      fhHodoChanGoodHitRateEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
   {
      sHistName = Form( "fhFractionGoodHitsSpillEvo%02u", uDpbIdx);
      title = Form( "Fraction of good hits vs time Dpb %02u; time [s]; Good Hits []", uDpbIdx);
      fhFractionGoodHitsSpillEvo.push_back( new TProfile(sHistName, title, 300000, 0, 300.0 ) );

      sHistName = Form( "fhFractionDupliHitsSpillEvo%02u", uDpbIdx);
      title = Form( "Fraction of fake hits vs time Dpb %02u; time [s]; Dupli Hits []", uDpbIdx);
      fhFractionDupliHitsSpillEvo.push_back( new TProfile(sHistName, title, 300000, 0, 300.0 ) );

      sHistName = Form( "fhFractionTsMsbSpillEvo%02u", uDpbIdx);
      title = Form( "Fraction of TS MSB vs time Dpb %02u; time [s]; TS MSB []", uDpbIdx);
      fhFractionTsMsbSpillEvo.push_back( new TProfile(sHistName, title, 300000, 0, 300.0 ) );

      sHistName = Form( "fhFractionEpochSpillEvo%02u", uDpbIdx);
      title = Form( "Fraction of Epoch vs time Dpb %02u; time [s]; Epoch []", uDpbIdx);
      fhFractionEpochSpillEvo.push_back( new TProfile(sHistName, title, 300000, 0, 300.0 ) );

      sHistName = Form( "fhFractionEmptySpillEvo%02u", uDpbIdx);
      title = Form( "Fraction of Empty vs time Dpb %02u; time [s]; Empty []", uDpbIdx);
      fhFractionEmptySpillEvo.push_back( new TProfile(sHistName, title, 300000, 0, 300.0 ) );

      sHistName = Form( "fhFractionGoodHitsEvo%02u", uDpbIdx);
      title = Form( "Fraction of good hits vs time Dpb %02u; time [s]; Good Hits []", uDpbIdx);
      fhFractionGoodHitsEvo.push_back( new TProfile(sHistName, title, 1800, 0, 1800 ) );

      sHistName = Form( "fhFractionDupliHitsEvo%02u", uDpbIdx);
      title = Form( "Fraction of fake hits vs time Dpb %02u; time [s]; Dupli Hits []", uDpbIdx);
      fhFractionDupliHitsEvo.push_back( new TProfile(sHistName, title, 1800, 0, 1800 ) );

      sHistName = Form( "fhFractionTsMsbEvo%02u", uDpbIdx);
      title = Form( "Fraction of TS MSB vs time Dpb %02u; time [s]; TS MSB []", uDpbIdx);
      fhFractionTsMsbEvo.push_back( new TProfile(sHistName, title, 1800, 0, 1800 ) );

      sHistName = Form( "fhFractionEpochEvo%02u", uDpbIdx);
      title = Form( "Fraction of Epoch vs time Dpb %02u; time [s]; Epoch []", uDpbIdx);
      fhFractionEpochEvo.push_back( new TProfile(sHistName, title, 1800, 0, 1800 ) );

      sHistName = Form( "fhFractionEmptyEvo%02u", uDpbIdx);
      title = Form( "Fraction of Empty vs time Dpb %02u; time [s]; Empty []", uDpbIdx);
      fhFractionEmptyEvo.push_back( new TProfile(sHistName, title, 1800, 0, 1800 ) );
   } // for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   sHistName = "fhFractionAsics";
   title = "Fraction of all hits per Asic; Asic []; Hits fraction []";
   fhFractionAsics = new TProfile(sHistName, title, fuNbStsXyters, 0, fuNbStsXyters );

   sHistName = "fhFractionTypes";
   title = "Fraction of data per Type; ; Msg fraction []";
   fhFractionTypes = new TProfile(sHistName, title, 5, 0, 5 );
   fhFractionTypes->GetXaxis()->SetBinLabel( 1, "Good Hit");
   fhFractionTypes->GetXaxis()->SetBinLabel( 2, "Dupli Hit");
   fhFractionTypes->GetXaxis()->SetBinLabel( 3, "TsMsb");
   fhFractionTypes->GetXaxis()->SetBinLabel( 4, "Epoch");
   fhFractionTypes->GetXaxis()->SetBinLabel( 5, "Empty");
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {

     if(uXyterIdx == 2)
       {
  	 for( UInt_t uChannelIdx = 0; uChannelIdx < fuNbChanPerAsic; ++uChannelIdx )
	   {
	     // Channel RawAdc
	     sHistName = Form( "hStsRawAdc_%03u", uChannelIdx );
	     title = Form( "Hits Adc per channel, StsXyter #%03u; Channel; Hits []", uChannelIdx );
	     fhStsRawAdc.push_back( new TH1I(sHistName, title,
					     stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 ) );
	   }
       }

      sHistName = Form( "fhAsicDuplicDtLastHits%02u", uXyterIdx);
      title =  "Time diff Between duplicated hit and previous hits; tDupli - tPrevN [bins]; Nth Previous hit [N]";
      fhAsicDuplicDtLastHits.push_back( new TH2I( sHistName, title,
                                       2048, -0.5, 2047.5,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "fhAsicDuplicCompTs%02u", uXyterIdx);
      title =  "ASIC Compared TS of duplicated hit and previous hits; Nth Previous hit [N]; Comp TS [bin]";
      fhAsicDuplicCompTs.push_back( new TH2I( sHistName, title,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                       128, -0.5, 127.5 ) );

      sHistName = Form( "fhAsicDuplicTsLsb%02u", uXyterIdx);
      title =  "TS LSB of duplicated hit and previous hits; Nth Previous hit [N]; TS LSB [bins]";
      fhAsicDuplicTsLsb.push_back( new TH2I( sHistName, title,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                       256, -0.5, 255.5 ) );

      sHistName = Form( "fhAsicDuplicTsMsbAsic%02u", uXyterIdx);
      title =  "ASIC TS MSB of duplicated hit and previous hits; Nth Previous hit [N]; TS MSB [bins]";
      fhAsicDuplicTsMsbAsic.push_back( new TH2I( sHistName, title,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                       64, -0.5, 63.5 ) );

      sHistName = Form( "fhAsicDuplicCompTsBitThere%02u", uXyterIdx);
      title =  "Bits present in ASIC Compared TS of duplicated hit and N previous hits; Ns Previous hit [N]; Bit";
      fhAsicDuplicCompTsBitThere.push_back( new TH2I( sHistName, title,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                       7, -0.5, 6.5 ) );

      sHistName = Form( "fhAsicDuplicTsLsbBitThere%02u", uXyterIdx);
      title =  "Bits present in TS LSB of duplicated hit and N previous hits; Ns Previous hit [N]; Bit";
      fhAsicDuplicTsLsbBitThere.push_back( new TH2I( sHistName, title,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                       8, -0.5, 7.5 ) );

      sHistName = Form( "fhAsicDuplicTsMsbBitThere%02u", uXyterIdx);
      title =  "Bits present in ASIC TS MSB of duplicated hit and N previous hits; Ns Previous hit [N]; Bit";
      fhAsicDuplicTsMsbBitThere.push_back( new TH2I( sHistName, title,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                       6, -0.5, 5.5 ) );

      sHistName = Form( "fhAsicDuplicTs%02u", uXyterIdx);
      title =  "TS of duplicated hit vs its channel index; Channel []; TS [bins]";
      fhAsicDuplicTs.push_back( new TH2I( sHistName, title,
                                          fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                          256, -0.5, 255.5 ) );

      sHistName = Form( "fhAsicGoodTs%02u", uXyterIdx);
      title =  "TS of good hit vs its channel index; Channel []; TS [bins]";
      fhAsicGoodTs.push_back( new TH2I( sHistName, title,
                                          fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                          256, -0.5, 255.5 ) );

      sHistName = Form( "fhAsicDuplicTsMsb%02u", uXyterIdx);
      title =  "TS MSB of duplicated hit vs its channel index; Channel []; TS_MSB [bins]";
      fhAsicDuplicTsMsb.push_back( new TH2I( sHistName, title,
                                          fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                          256, -0.5, 255.5 ) );

      sHistName = Form( "fhAsicGoodTsMsb%02u", uXyterIdx);
      title =  "TS MSB of good hit vs its channel index; Channel []; TS_MSB [bins]";
      fhAsicGoodTsMsb.push_back( new TH2I( sHistName, title,
                                          fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                          256, -0.5, 255.5 ) );

      sHistName = Form( "fhAsicDuplicTsFull%02u", uXyterIdx);
      title =  "Full TS of duplicated hit vs its channel index; Channel []; TS + TS_MSB [bins]";
      fhAsicDuplicTsFull.push_back( new TH2I( sHistName, title,
                                          fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                          16384, -0.5, 16383.5 ) );

      sHistName = Form( "fhAsicGoodTsFull%02u", uXyterIdx);
      title =  "Full TS of good hit vs its channel index; Channel []; TS + TS_MSB [bins]";
      fhAsicGoodTsFull.push_back( new TH2I( sHistName, title,
                                          fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                          16384, -0.5, 16383.5 ) );

      sHistName = Form( "fhAsicDuplicTsBitPattern%02u", uXyterIdx);
      title =  "Bits pattern in duplicated hit; TS Bit; ON/OFF?";
      fhAsicDuplicTsBitPattern.push_back( new TH2I( sHistName, title,
                                       14, -0.5, 13.5,
                                        2, -0.5,  1.5 ) );

      sHistName = Form( "fhAsicGoodTsBitPattern%02u", uXyterIdx);
      title =  "Bits pattern in good hit; TS Bit; ON/OFF?";
      fhAsicGoodTsBitPattern.push_back( new TH2I( sHistName, title,
                                       14, -0.5, 13.5,
                                        2, -0.5,  1.5 ) );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   sHistName = "hStsRawAdcCombine";
   title = "ADC histogram combined";
   fhStsRawAdcCombine = new TH1I(sHistName, title, stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5);

   sHistName = "fhAsicDuplicTsEvoAsic0Chan01";
   title =  "TS of duplicated hits vs time in run for ASIC 0 chan 1; time in run [s]; TS + TS_MSB [bins]";
   fhAsicDuplicTsEvoAsic0Chan01 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       2048, -0.5, 4095.5 );
   sHistName = "fhAsicGoodTsEvoAsic0Chan01";
   title =  "TS of good hits vs time in run for ASIC 0 chan 1; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsEvoAsic0Chan01 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       2048, -0.5, 4095.5 );
   sHistName = "fhAsicGoodTsMsbEvoAsic0Chan01";
   title =  "TS MSB of good hits vs time in run for ASIC 0 chan 1; time in run [s]; TS_MSB[0 - 7] [bins]";
   fhAsicGoodTsMsbEvoAsic0Chan01 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       256, -0.5, 255.5 );
   sHistName = "fhAsicGoodTsFullEvoAsic0Chan01";
   title =  "Full TS of good hits vs time in run for ASIC 0 chan 1; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsFullEvoAsic0Chan01 = new TH2I( sHistName, title,
                                       4*900, -0.5, 899.5,
                                       16384, -0.5, 16383.5 );
   sHistName = "fhAsicGoodTsFullEvoProfAsic0Chan01";
   title =  "Full TS of good hits vs time in run for ASIC 0 chan 1; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsFullEvoProfAsic0Chan01 = new TProfile( sHistName, title,
                                       4*3600, -0.5, 3599.5 );

   sHistName = "fhAsicDuplicTsEvoAsic0Chan09";
   title =  "TS of duplicated hits vs time in run for ASIC 0 chan 9; time in run [s]; TS + TS_MSB [bins]";
   fhAsicDuplicTsEvoAsic0Chan09 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       2048, -0.5, 4095.5 );
   sHistName = "fhAsicGoodTsEvoAsic0Chan09";
   title =  "TS of good hits vs time in run for ASIC 0 chan 9; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsEvoAsic0Chan09 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       2048, -0.5, 4095.5 );
   sHistName = "fhAsicGoodTsFullEvoProfAsic0Chan09";
   title =  "Full TS of good hits vs time in run for ASIC 0 chan 9; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsFullEvoProfAsic0Chan09 = new TProfile( sHistName, title,
                                       4*3600, -0.5, 3599.5 );

   sHistName = "fhAsicDuplicTsEvoAsic0Chan16";
   title =  "TS of duplicated hits vs time in run for ASIC 0 chan 16; time in run [s]; TS + TS_MSB [bins]";
   fhAsicDuplicTsEvoAsic0Chan16 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       2048, -0.5, 4095.5 );
   sHistName = "fhAsicGoodTsEvoAsic0Chan16";
   title =  "TS of good hits vs time in run for ASIC 0 chan 16; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsEvoAsic0Chan16 = new TH2I( sHistName, title,
                                       2*4096, -0.5, 4095.5,
                                       2048, -0.5, 4095.5 );
   sHistName = "fhAsicGoodTsFullEvoProfAsic0Chan16";
   title =  "Full TS of good hits vs time in run for ASIC 0 chan 16; time in run [s]; TS + TS_MSB [bins]";
   fhAsicGoodTsFullEvoProfAsic0Chan16 = new TProfile( sHistName, title,
                                       4*3600, -0.5, 3599.5 );


   sHistName = "fhAsicMissedChanIdVsPulseIdx";
   title =  "Channels present VS pulse Idx; Pulse Idx []; Channel Id []";
   fhAsicMissedChanIdVsPulseIdx = new TH2I( sHistName, title,
                                       fuPulseIdxMax, -0.5, fuPulseIdxMax - 0.5,
                                       fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   sHistName = "fhAsicMissedChanGroupVsPulseIdx";
   title =  "Channels group present VS pulse Idx; Pulse Idx []; Channel Grp []";
   fhAsicMissedChanGroupVsPulseIdx = new TProfile( sHistName, title,
                                       fuPulseIdxMax, -0.5, fuPulseIdxMax - 0.5 );


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
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      sHistName = Form( "fhPulseChanCountEvo%02u", uXyterIdx);
      title = Form( "Number of channels per pulse vs time for ASIC %02u; time [s]; Channels []; Pulses []", uXyterIdx);
      fhPulseChanCountEvo.push_back( new TH2D( sHistName, title,
                                              300, 0, 300.0,
                                              fuNbChanPerAsic, 0.5, fuNbChanPerAsic + 0.5 ) );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   sHistName = "fhHodoChanHitRateEvoZoom";
   title = "Hits per second & channel in StsXyter 000; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvoZoom = new TH2I( sHistName, title,
                                             2000, 0, 50,
                                             fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

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
      server->Register("/HodoRaw", fhHodoMessType );
      server->Register("/HodoRaw", fhHodoSysMessType );
      server->Register("/HodoRaw", fhHodoMessTypePerDpb );
      server->Register("/HodoRaw", fhHodoSysMessTypePerDpb );
      server->Register("/HodoRaw", fhHodoMessTypePerElink );
      server->Register("/HodoRaw", fhHodoSysMessTypePerElink );
      server->Register("/HodoRaw", fhStsRawAdcCombine );
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {

	if(uXyterIdx == 2)
	  {
	    for( UInt_t uChannelIdx = 0; uChannelIdx < fuNbChanPerAsic; ++uChannelIdx )
	      {
		if( server ) server->Register("/HodoRaw", fhStsRawAdc[ uChannelIdx ] );
	      }
	  }
         server->Register("/HodoRaw", fhHodoChanCntRaw[ uXyterIdx ] );
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

      server->Register("/HodoBoth", fhBothHodoSortedDtX1Y1 );
      server->Register("/HodoBoth", fhBothHodoSortedDtX2Y2 );
      server->Register("/HodoBoth", fhBothHodoSortedDtX1Y1X2Y2N1P1 );
      server->Register("/HodoBoth", fhBothHodoSortedDtX1Y1X2Y2N2P2 );
      server->Register("/HodoBoth", fhBothHodoSortedMapX1Y1 );
      server->Register("/HodoBoth", fhBothHodoSortedMapX2Y2 );
      server->Register("/HodoBoth", fhBothHodoSortedCntEvoX1Y1 );
      server->Register("/HodoBoth", fhBothHodoSortedCntEvoX2Y2 );

      server->Register("/H1H2S1", fhH1H2S1SortedDtX1Y1 );
      server->Register("/H1H2S1", fhH1H2S1SortedDtX2Y2 );
      server->Register("/H1H2S1", fhH1H2S1SortedDtN1P1 );
      server->Register("/H1H2S1", fhH1H2S1SortedDtN2P2 );
      server->Register("/H1H2S1", fhH1H2S1SortedMapX1Y1 );
      server->Register("/H1H2S1", fhH1H2S1SortedMapX2Y2 );
      server->Register("/H1H2S1", fhH1H2S1SortedMapN1P1 );
      server->Register("/H1H2S1", fhH1H2S1SortedMapN2P2 );
      server->Register("/H1H2S1", fhH1H2S1SortedCntEvoX1Y1 );
      server->Register("/H1H2S1", fhH1H2S1SortedCntEvoX2Y2 );
      server->Register("/H1H2S1", fhH1H2S1SortedCntEvoN1P1 );
      server->Register("/H1H2S1", fhH1H2S1SortedCntEvoN2P2 );

      server->Register("/H1H2S2", fhH1H2S2SortedDtX1Y1 );
      server->Register("/H1H2S2", fhH1H2S2SortedDtX2Y2 );
      server->Register("/H1H2S2", fhH1H2S2SortedDtN1P1 );
      server->Register("/H1H2S2", fhH1H2S2SortedDtN2P2 );
      server->Register("/H1H2S2", fhH1H2S2SortedMapX1Y1 );
      server->Register("/H1H2S2", fhH1H2S2SortedMapX2Y2 );
      server->Register("/H1H2S2", fhH1H2S2SortedMapN1P1 );
      server->Register("/H1H2S2", fhH1H2S2SortedMapN2P2 );
      server->Register("/H1H2S2", fhH1H2S2SortedCntEvoX1Y1 );
      server->Register("/H1H2S2", fhH1H2S2SortedCntEvoX2Y2 );
      server->Register("/H1H2S2", fhH1H2S2SortedCntEvoN1P1 );
      server->Register("/H1H2S2", fhH1H2S2SortedCntEvoN2P2 );

      server->Register("/Syst", fhSystSortedDtX1Y1 );
      server->Register("/Syst", fhSystSortedDtX2Y2 );
      server->Register("/Syst", fhSystSortedDtN1P1 );
      server->Register("/Syst", fhSystSortedDtN2P2 );
      server->Register("/Syst", fhSystSortedDtX1Y1X2Y2 );
      server->Register("/Syst", fhSystSortedMapX1Y1 );
      server->Register("/Syst", fhSystSortedMapX2Y2 );
      server->Register("/Syst", fhSystSortedMapN1P1 );
      server->Register("/Syst", fhSystSortedMapN2P2 );
      server->Register("/Syst", fhSystSortedCntEvoX1Y1 );
      server->Register("/Syst", fhSystSortedCntEvoX2Y2 );
      server->Register("/Syst", fhSystSortedCntEvoN1P1 );
      server->Register("/Syst", fhSystSortedCntEvoN2P2 );

      server->Register("/Syst", fhSystSortedDtN1X1vsN1X2 );
      server->Register("/Syst", fhSystSortedDtP1X1vsP1X2 );
      server->Register("/Syst", fhSystSortedDtN1X1vsP1X1 );
      server->Register("/Syst", fhSystSortedDtSts1Hodo1vsSts1Hodo2 );
      server->Register("/Syst", fhSystSortedDtSts2Hodo1vsSts2Hodo2 );

      server->Register("/Syst", fhSystSortedDtAllVsMapX1 );
      server->Register("/Syst", fhSystSortedDtAllVsMapY1 );
      server->Register("/Syst", fhSystSortedDtAllVsMapX2 );
      server->Register("/Syst", fhSystSortedDtAllVsMapY2 );

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

         server->Register("/FebDupli", fhRatioSameTsSpillEvo[ uXyterIdx ] );
         server->Register("/FebDupli", fhRatioSameTsAdcSpillEvo[ uXyterIdx ] );
         server->Register("/FebDupli", fhRatioSameAdcSameTsSpillEvo[ uXyterIdx ] );

         server->Register("/FebDupli", fhSetupSortedTsFirstDuplicateChan[ uXyterIdx ] );
         server->Register("/FebDupli", fhSetupSortedTsGoodChan[ uXyterIdx ] );

         server->Register("/FebDupli", fhAsicDuplicDtLastHits[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicCompTs[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsLsb[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsMsbAsic[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicCompTsBitThere[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsLsbBitThere[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsMsbBitThere[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTs[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicGoodTs[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsMsb[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicGoodTsMsb[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsFull[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicGoodTsFull[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicDuplicTsBitPattern[ uXyterIdx ] );
         server->Register("/FebDupli", fhAsicGoodTsBitPattern[ uXyterIdx ] );

         server->Register("/Data", fhFractionHitsAsicSpillEvo[ uXyterIdx ] );
         server->Register("/Data", fhFractionHitsAsicEvo[ uXyterIdx ] );
         server->Register("/Data", fhHodoChanCntGood[ uXyterIdx ] );
         server->Register("/Data", fhHodoChanGoodHitRateEvo[ uXyterIdx ] );

         server->Register("/Data", fhPulseChanCountEvo[ uXyterIdx ] );
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

      server->Register("/FebDupli", fhAsicDuplicTsEvoAsic0Chan01 );
      server->Register("/FebDupli", fhAsicGoodTsEvoAsic0Chan01 );
      server->Register("/FebDupli", fhAsicGoodTsMsbEvoAsic0Chan01 );
      server->Register("/FebDupli", fhAsicGoodTsFullEvoAsic0Chan01 );
      server->Register("/FebDupli", fhAsicGoodTsFullEvoProfAsic0Chan01 );
      server->Register("/FebDupli", fhAsicDuplicTsEvoAsic0Chan09 );
      server->Register("/FebDupli", fhAsicGoodTsEvoAsic0Chan09 );
      server->Register("/FebDupli", fhAsicGoodTsFullEvoProfAsic0Chan09 );
      server->Register("/FebDupli", fhAsicDuplicTsEvoAsic0Chan16 );
      server->Register("/FebDupli", fhAsicGoodTsEvoAsic0Chan16 );
      server->Register("/FebDupli", fhAsicGoodTsFullEvoProfAsic0Chan16 );

      server->Register("/FebDupli", fhAsicMissedChanIdVsPulseIdx );
      server->Register("/FebDupli", fhAsicMissedChanGroupVsPulseIdx );

      for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
      {
         server->Register("/Data", fhFractionGoodHitsSpillEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionDupliHitsSpillEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionTsMsbSpillEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionEpochSpillEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionEmptySpillEvo[ uDpbIdx ] );

         server->Register("/Data", fhFractionGoodHitsEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionDupliHitsEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionTsMsbEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionEpochEvo[ uDpbIdx ] );
         server->Register("/Data", fhFractionEmptyEvo[ uDpbIdx ] );
      } // for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
      server->Register("/Data", fhFractionAsics );
      server->Register("/Data", fhFractionTypes );

      server->Register("/Spill", fhHodoX1SpillEvo );
      server->Register("/Spill", fhHodoY1SpillEvo );
      server->Register("/Spill", fhHodoX2SpillEvo );
      server->Register("/Spill", fhHodoY2SpillEvo );
      server->Register("/Spill", fhHodoX1SpillEvoProf );
      server->Register("/Spill", fhHodoY1SpillEvoProf );
      server->Register("/Spill", fhHodoX2SpillEvoProf );
      server->Register("/Spill", fhHodoY2SpillEvoProf );

      server->Register("/FebDupli", fhHodoChanHitRateEvoZoom );


      server->RegisterCommand("/Reset_All_Hodo", "bCosy2018ResetDupliHistos=kTRUE");
      server->RegisterCommand("/Write_All_Hodo", "bCosy2018WriteDupliHistos=kTRUE");
      server->RegisterCommand("/Print_Messages", "bCosy2018PrintMessDupliHistos=kTRUE");

      server->Restrict("/Reset_All_Hodo", "allow=admin");
      server->Restrict("/Write_All_Hodo", "allow=admin");
   } // if( server )

   /** Create summary Canvases for CERN 2017 **/
   Double_t w = 10;
   Double_t h = 10;
   Double_t w1 = 10;
   Double_t h1 = 10;

      // Summary per StsXyter
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {

     // Summary per StsXyter
     if(fuNbStsXyters == 2)
       {
  	 for( UInt_t uChannelIdx = 0; uChannelIdx < fuNbChanPerAsic; ++uChannelIdx )
	   {
	     TCanvas* cStsSumm1 = new TCanvas( Form("cStsSum1_%03u", uChannelIdx ),
					       Form("Summary plots for StsXyter %03u", uChannelIdx ),
					       w1, h1);
	     cStsSumm1->cd();
	     fhStsRawAdc[ uChannelIdx ]->Draw();

	   }
       }


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
   if( kTRUE == fbDualStsEna )
      cSetupSortedDt->Divide( 4, 3 );
      else cSetupSortedDt->Divide( 3, 3 );

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
   fhBothHodoSortedDtX1Y1->Draw( "" );

   cSetupSortedDt->cd(5 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhBothHodoSortedDtX2Y2->Draw( "" );
/*
   cSetupSortedDt->cd(6 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSetupSortedDtN1P1->Draw( "" );
*/
   cSetupSortedDt->cd(7 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSystSortedDtX1Y1->Draw( "" );

   cSetupSortedDt->cd(8 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSystSortedDtX2Y2->Draw( "" );

   cSetupSortedDt->cd(9 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetGridx();
   gPad->SetLogy();
   fhSystSortedDtN1P1->Draw( "" );

   if( kTRUE == fbDualStsEna )
   {
      cSetupSortedDt->cd(4);
      gPad->SetGridx();
      gPad->SetLogy();
      fhSetupSortedDtN2P2->Draw( "colz" );
/*
      cSetupSortedDt->cd(8);
      gPad->SetGridx();
      gPad->SetLogy();
      fhSetupSortedDtN2P2->Draw( "colz" );
*/

      cSetupSortedDt->cd(12);
      gPad->SetGridx();
      gPad->SetLogy();
      fhSystSortedDtN2P2->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cSetupSortedMaps = new TCanvas( "cSetupSortedMaps",
                                    "Hodoscopes coincidence maps",
                                    w, h);

   if( kTRUE == fbDualStsEna )
      cSetupSortedMaps->Divide( 4, 3 );
      else cSetupSortedMaps->Divide( 3, 3 );

   cSetupSortedMaps->cd(1);
   gPad->SetLogz();
   fhSetupSortedMapX1Y1->Draw( "colz" );

   cSetupSortedMaps->cd(2);
   gPad->SetLogz();
   fhSetupSortedMapX2Y2->Draw( "colz" );

   cSetupSortedMaps->cd(3);
   gPad->SetLogz();
   fhSetupSortedMapN1P1->Draw( "colz" );

   cSetupSortedMaps->cd(4 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetLogz();
   fhBothHodoSortedMapX1Y1->Draw( "colz" );

   cSetupSortedMaps->cd(5 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetLogz();
   fhBothHodoSortedMapX2Y2->Draw( "colz" );
/*
   cSetupSortedMaps->cd(6 + ( kTRUE == fbDualStsEna ? 1 : 0 ) );
   gPad->SetLogz();
   fhBothHodoSortedMapN1P1->Draw( "colz" );
*/
   cSetupSortedMaps->cd(7 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetLogz();
   fhSystSortedMapX1Y1->Draw( "colz" );

   cSetupSortedMaps->cd(8 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetLogz();
   fhSystSortedMapX2Y2->Draw( "colz" );

   cSetupSortedMaps->cd(9 + ( kTRUE == fbDualStsEna ? 2 : 0 ) );
   gPad->SetLogz();
   fhSystSortedMapN1P1->Draw( "colz" );

   if( kTRUE == fbDualStsEna )
   {
      cSetupSortedMaps->cd(4);
      gPad->SetLogz();
      fhSetupSortedMapN2P2->Draw( "colz" );
/*
      cSetupSortedMaps->cd(8);
      gPad->SetLogz();
      fhBothHodoSortedMapN2P2->Draw( "colz" );
*/

      cSetupSortedMaps->cd(12);
      gPad->SetLogz();
      fhSystSortedMapN2P2->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cSetupSortedCoincEvo = new TCanvas( "cSetupSortedCoincEvo",
                                    "Hodoscopes coincidence rate evolution",
                                    w, h);
   cSetupSortedCoincEvo->Divide( 3, 3 );

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
   fhBothHodoSortedCntEvoX1Y1->Draw( "" );

   cSetupSortedCoincEvo->cd(5);
   gPad->SetGridy();
   gPad->SetLogy();
   fhBothHodoSortedCntEvoX2Y2->Draw( "" );
/*
   cSetupSortedCoincEvo->cd(6);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSetupSortedCntEvoN1P1->Draw( "" );
*/
   cSetupSortedCoincEvo->cd(7);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSystSortedCntEvoX1Y1->Draw( "" );

   cSetupSortedCoincEvo->cd(8);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSystSortedCntEvoX2Y2->Draw( "" );

   cSetupSortedCoincEvo->cd(9);
   gPad->SetGridy();
   gPad->SetLogy();
   fhSystSortedCntEvoN1P1->Draw( "" );
//====================================================================//

//====================================================================//
   TCanvas* cSetupCoinQaDt = new TCanvas( "cSetupCoinQaDt",
                                    "Hodoscopes coincidence quality",
                                    w, h);
   cSetupCoinQaDt->Divide( 2, 2 );

   cSetupCoinQaDt->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhSystSortedDtN1X1vsN1X2->Draw( "colz" );

   cSetupCoinQaDt->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhSystSortedDtP1X1vsP1X2->Draw( "colz" );

   cSetupCoinQaDt->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhSystSortedDtN1X1vsP1X1->Draw( "colz" );

   cSetupCoinQaDt->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhSystSortedDtSts1Hodo1vsSts1Hodo2->Draw( "colz" );

//====================================================================//
   TCanvas* cSetupCoinQa = new TCanvas( "cSetupCoinQa",
                                    "Hodoscopes coincidence quality",
                                    w, h);
   cSetupCoinQa->Divide( 2, 2 );

   cSetupCoinQa->cd(1);
   gPad->SetLogz();
   fhSystSortedDtAllVsMapX1->Draw( "colz" );

   cSetupCoinQa->cd(2);
   gPad->SetLogz();
   fhSystSortedDtAllVsMapY1->Draw( "colz" );

   cSetupCoinQa->cd(3);
   gPad->SetLogz();
   fhSystSortedDtAllVsMapX2->Draw( "colz" );

   cSetupCoinQa->cd(4);
   gPad->SetLogz();
   fhSystSortedDtAllVsMapY2->Draw( "colz" );
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
   TCanvas* cDataShares = new TCanvas( "cDataShares",
                                    "Data share per ASIC or type",
                                    w, h);

   cDataShares->Divide( 2 + fuNrOfDpbs, 2 );

   Color_t colorsList[10] = { kBlue, kRed, kGreen, kMagenta, kYellow,
                              kGray, kOrange, kCyan, kSpring - 6, kPink +9 };

   cDataShares->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   fhFractionAsics->Draw("hist");

   cDataShares->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   THStack * stackShareAsicSpill = new THStack( "hsShareAsicSpill", "Fraction of hits for each ASIC" );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhFractionHitsAsicSpillEvo[ uXyterIdx ]->SetLineColor( colorsList[uXyterIdx] );
      fhFractionHitsAsicSpillEvo[ uXyterIdx ]->SetFillColor( colorsList[uXyterIdx] );

      stackShareAsicSpill->Add( fhFractionHitsAsicSpillEvo[ uXyterIdx ] );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   stackShareAsicSpill->Draw( ",hist" );

   cDataShares->cd( 3 + fuNrOfDpbs );
   gPad->SetGridx();
   gPad->SetGridy();
   fhFractionTypes->Draw("hist");

   cDataShares->cd( 4 + fuNrOfDpbs );
   gPad->SetGridx();
   gPad->SetGridy();
   THStack * stackShareAsic = new THStack( "hsShareAsic", "Fraction of hits for each ASIC" );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhFractionHitsAsicEvo[ uXyterIdx ]->SetLineColor( colorsList[uXyterIdx] );
      fhFractionHitsAsicEvo[ uXyterIdx ]->SetFillColor( colorsList[uXyterIdx] );

      stackShareAsic->Add( fhFractionHitsAsicEvo[ uXyterIdx ] );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   stackShareAsic->Draw( ",hist" );

   THStack * stackShareTypesSpill[ fuNrOfDpbs ];
   THStack * stackShareTypes[ fuNrOfDpbs ];
   for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
   {
      cDataShares->cd( 3 + uDpbIdx );
      gPad->SetGridx();
      gPad->SetGridy();

      stackShareTypesSpill[ uDpbIdx ] = new THStack( Form( "hsShareTypes%02uSpill", uDpbIdx),
                                                Form( "Fraction of message types for DPB %02u", uDpbIdx ) );

      fhFractionGoodHitsSpillEvo[ uDpbIdx ]->SetLineColor( colorsList[ 0 ] );
      fhFractionGoodHitsSpillEvo[ uDpbIdx ]->SetFillColor( colorsList[ 0 ] );
      stackShareTypesSpill[ uDpbIdx ]->Add( fhFractionGoodHitsSpillEvo[ uDpbIdx ] );

      fhFractionDupliHitsSpillEvo[ uDpbIdx ]->SetLineColor( colorsList[ 1 ] );
      fhFractionDupliHitsSpillEvo[ uDpbIdx ]->SetFillColor( colorsList[ 1 ] );
      stackShareTypesSpill[ uDpbIdx ]->Add( fhFractionDupliHitsSpillEvo[ uDpbIdx ] );

      fhFractionTsMsbSpillEvo[ uDpbIdx ]->SetLineColor( colorsList[ 2 ] );
      fhFractionTsMsbSpillEvo[ uDpbIdx ]->SetFillColor( colorsList[ 2 ] );
      stackShareTypesSpill[ uDpbIdx ]->Add( fhFractionTsMsbSpillEvo[ uDpbIdx ] );

      fhFractionEpochSpillEvo[ uDpbIdx ]->SetLineColor( colorsList[ 3 ] );
      fhFractionEpochSpillEvo[ uDpbIdx ]->SetFillColor( colorsList[ 3 ] );
      stackShareTypesSpill[ uDpbIdx ]->Add( fhFractionEpochSpillEvo[ uDpbIdx ] );

      fhFractionEmptySpillEvo[ uDpbIdx ]->SetLineColor( colorsList[ 4 ] );
      fhFractionEmptySpillEvo[ uDpbIdx ]->SetFillColor( colorsList[ 4 ] );
      stackShareTypesSpill[ uDpbIdx ]->Add( fhFractionEmptySpillEvo[ uDpbIdx ] );

      stackShareTypesSpill[ uDpbIdx ]->Draw( ",hist" );

      cDataShares->cd( 5 + fuNrOfDpbs + uDpbIdx );
      gPad->SetGridx();
      gPad->SetGridy();

      stackShareTypes[ uDpbIdx ] = new THStack( Form( "hsShareTypes%02u", uDpbIdx),
                                                Form( "Fraction of message types for DPB %02u", uDpbIdx ) );

      fhFractionGoodHitsEvo[ uDpbIdx ]->SetLineColor( colorsList[ 0 ] );
      fhFractionGoodHitsEvo[ uDpbIdx ]->SetFillColor( colorsList[ 0 ] );
      stackShareTypes[ uDpbIdx ]->Add( dynamic_cast< TH1 * >( fhFractionGoodHitsEvo[ uDpbIdx ] ) );
      stackShareTypes[ uDpbIdx ]->Draw("hist");

      fhFractionDupliHitsEvo[ uDpbIdx ]->SetLineColor( colorsList[ 1 ] );
      fhFractionDupliHitsEvo[ uDpbIdx ]->SetFillColor( colorsList[ 1 ] );
      stackShareTypes[ uDpbIdx ]->Add( fhFractionDupliHitsEvo[ uDpbIdx ] );
      stackShareTypes[ uDpbIdx ]->Draw("hist");

      fhFractionTsMsbEvo[ uDpbIdx ]->SetLineColor( colorsList[ 2 ] );
      fhFractionTsMsbEvo[ uDpbIdx ]->SetFillColor( colorsList[ 2 ] );
      stackShareTypes[ uDpbIdx ]->Add( fhFractionTsMsbEvo[ uDpbIdx ] );
      stackShareTypes[ uDpbIdx ]->Draw("hist");

      fhFractionEpochEvo[ uDpbIdx ]->SetLineColor( colorsList[ 3 ] );
      fhFractionEpochEvo[ uDpbIdx ]->SetFillColor( colorsList[ 3 ] );
      stackShareTypes[ uDpbIdx ]->Add( fhFractionEpochEvo[ uDpbIdx ] );
      stackShareTypes[ uDpbIdx ]->Draw("hist");

      fhFractionEmptyEvo[ uDpbIdx ]->SetLineColor( colorsList[ 4 ] );
      fhFractionEmptyEvo[ uDpbIdx ]->SetFillColor( colorsList[ 4 ] );
      stackShareTypes[ uDpbIdx ]->Add( fhFractionEmptyEvo[ uDpbIdx ] );
      stackShareTypes[ uDpbIdx ]->Draw("hist");

      stackShareTypes[ uDpbIdx ]->Draw( ",hist" );
   } // for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )

//====================================================================//

//====================================================================//
   TCanvas* cFebDupliDtPrev = new TCanvas( "cFebDupliDtPrev",
                                    "Duplicate dt to previous hits",
                                    w, h);

   cFebDupliDtPrev->Divide( 2, 3 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebDupliDtPrev->cd( 1 + uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhAsicDuplicDtLastHits[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebDupliPrevCompTs = new TCanvas( "cFebDupliPrevCompTs",
                                    "Duplicate and previous hits TS used in comparator",
                                    w, h);

   cFebDupliPrevCompTs->Divide( 2, 3 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebDupliPrevCompTs->cd( 1 + uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhAsicDuplicCompTs[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebDupliPrevTsLsb = new TCanvas( "cFebDupliPrevTsLsb",
                                    "Duplicate and previous hits Ts LSB",
                                    w, h);

   cFebDupliPrevTsLsb->Divide( 2, 3 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebDupliPrevTsLsb->cd( 1 + uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhAsicDuplicTsLsb[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebDupliPrevTsMsb = new TCanvas( "cFebDupliPrevTsMsb",
                                    "Duplicate and previous hits Ts MSB",
                                    w, h);

   cFebDupliPrevTsMsb->Divide( 2, 3 );

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      cFebDupliPrevTsMsb->cd( 1 + uXyterIdx );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhAsicDuplicTsMsbAsic[ uXyterIdx ]->Draw( "colz" );
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
//====================================================================//

//====================================================================//
   TCanvas* cFebDupliTsScan = new TCanvas( "cFebDupliTsScan",
                                    "Duplicate TS scan",
                                    w, h);

   cFebDupliTsScan->Divide( 2, 3 );

   cFebDupliTsScan->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhAsicGoodTs[ 0 ]->Draw( "colz" );

   cFebDupliTsScan->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhAsicDuplicTs[ 0 ]->Draw( "colz" );

   cFebDupliTsScan->cd( 3 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhAsicGoodTsMsb[ 0 ]->Draw( "colz" );

   cFebDupliTsScan->cd( 4 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhAsicDuplicTsMsb[ 0 ]->Draw( "colz" );

   cFebDupliTsScan->cd( 5 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhAsicGoodTsFull[ 0 ]->Draw( "colz" );

   cFebDupliTsScan->cd( 6 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   fhAsicDuplicTsFull[ 0 ]->Draw( "colz" );
//====================================================================//

      // Long duration rate monitoring
/*
   if( kTRUE == fbLongHistoEnable )
   {
      TCanvas* cStsLongRate = new TCanvas( "cStsLongRate" , "Long duration rate plots for StsXyter",
                                           w, h);
      cStsLongRate->Divide( 2, fuNbStsXyters );

      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {
         cStsLongRate->cd( 1 + 2 * uXyterIdx );
         gPad->SetLogy();
         gPad->SetGridx();
         gPad->SetGridy();
         fhFebRateEvoLong[ uXyterIdx ]->Draw( "hist" );

         cStsLongRate->cd( 2 + 2 * uXyterIdx );
         gPad->SetLogz();
         gPad->SetGridx();
         gPad->SetGridy();
         fhFebChRateEvoLong[ uXyterIdx ]->Draw( "colz" );
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   } // if( kTRUE == fbLongHistoEnable )
*/
/*
  Int_t iNbPadsPerDpb = fuNbElinksPerDpb/2 + fuNbElinksPerDpb%2;
  TCanvas* cMuchChCounts = new TCanvas("cMuchChCounts", "MUCH Channels counts", w, h);
  cMuchChCounts->Divide( fNrOfNdpbs/2 + fNrOfNdpbs%2, fNrOfFebsPerNdpb );

  TCanvas* cMuchFebRate = new TCanvas("cMuchFebRate", "MUCH FEB rate", w, h);
  cMuchFebRate->Divide( fNrOfNdpbs/2 + fNrOfNdpbs%2, fNrOfFebsPerNdpb );

   TH1* histPnt = NULL;
   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      if( dpbId < fUnpackParHodo->GetNrOfnDpbsModA() )
      {
         sNdpbTag = Form("%04X", fUnpackParHodo->GetNdpbIdA(dpbId) );
      } // if( dpbId < fUnpackParHodo->GetNrOfnDpbsModA() )
         else
         {
            sNdpbTag = Form("%04X", fUnpackParHodo->GetNdpbIdB(dpbId - fNrOfNdpbsA) );
         } // else of if( dpbId < fUnpackParHodo->GetNrOfnDpbsModA() )

      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
         cMuchChCounts->cd( 1 + dpbId * iNbPadsPerDpb + febId/2 );
         gPad->SetLogy();
         sHistName = Form("Chan_Counts_Much_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());

         if( 0 == febId%2 )
         {
            histPnt->SetLineColor( kRed );  // => Change color for 1st of the 2/pad!
            histPnt->Draw();
         } // if( 0 == febId%2 )
            else
            {
               histPnt->SetLineColor( kBlue );  // => Change color for 1nd of the 2/pad!
               histPnt->Draw("same");
            } // if( 0 == febId%2 )

         cMuchFebRate->cd( 1 + dpbId * iNbPadsPerDpb + febId/2 );
         gPad->SetLogy();
         sHistName = Form("FebRate_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());

         if( 0 == febId%2 )
         {
            histPnt->SetLineColor( kRed );  // => Change color for 1st of the 2/pad!
            histPnt->Draw();
         } // if( 0 == febId%2 )
            else
            {
               histPnt->SetLineColor( kBlue );  // => Change color for 1nd of the 2/pad!
               histPnt->Draw("same");
            } // if( 0 == febId%2 )
      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
*/

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

Bool_t CbmCosy2018MonitorDupli::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bCosy2018ResetDupliHistos )
   {
      ResetAllHistos();
      bCosy2018ResetDupliHistos = kFALSE;
   } // if( bCosy2018ResetDupliHistos )
   if( bCosy2018WriteDupliHistos )
   {
      SaveAllHistos( fsHistoFileFullname );
      bCosy2018WriteDupliHistos = kFALSE;
   } // if( bCosy2018WriteDupliHistos )
   if( bCosy2018PrintMessDupliHistos )
   {
      fuPrintMessagesIdx = 0;
      bCosy2018PrintMessDupliHistos = kFALSE;
   } // if( bCosy2018PrintMessDupliHistos )

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
      LOG(info) << "CbmCosy2018MonitorDupli::DoUnpack => Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
      LOG(info) << "CbmCosy2018MonitorDupli::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
      LOG(info) << "CbmCosy2018MonitorDupli::DoUnpack =>  Changed fvuChanNbHitsInMs size "
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
                  LOG(fatal) << "CbmCosy2018MonitorDupli::DoUnpack => "
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

               Double_t dTsMsbTime = ( (  static_cast<ULong64_t>( stsxyter::kuHitNbTsBins )
                                        * static_cast<ULong64_t>( fvulCurrentTsMsb[fuCurrDpbIdx])
                                        + static_cast<ULong64_t>( stsxyter::kulTsCycleNbBins )
                                        * static_cast<ULong64_t>( fvuCurrentTsMsbCycle[fuCurrDpbIdx] )
                                       ) * stsxyter::kdClockCycleNs
                                       - fdStartTime
                                     )* 1e-9;

               fhFractionGoodHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionDupliHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionTsMsbSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 1.0 );
               fhFractionEpochSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEmptySpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );

               fhFractionGoodHitsEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionDupliHitsEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionTsMsbEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 1.0 );
               fhFractionEpochEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEmptyEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );

               fhFractionTypes->Fill( 0., 0.0 ); // Good Hit
               fhFractionTypes->Fill( 1., 0.0 ); // Dupli Hit
               fhFractionTypes->Fill( 2., 1.0 ); // Ts MSB
               fhFractionTypes->Fill( 3., 0.0 ); // Epoch
               fhFractionTypes->Fill( 4., 0.0 ); // Empty

               break;
            } // case stsxyter::MessType::TsMsb :
            case stsxyter::MessType::Epoch :
            {
               // The first message in the TS is a special ones: EPOCH
               FillEpochInfo( mess );

               Double_t dTsMsbTime = ( (  static_cast<ULong64_t>( stsxyter::kuHitNbTsBins )
                                        * static_cast<ULong64_t>( fvulCurrentTsMsb[fuCurrDpbIdx])
                                        + static_cast<ULong64_t>( stsxyter::kulTsCycleNbBins )
                                        * static_cast<ULong64_t>( fvuCurrentTsMsbCycle[fuCurrDpbIdx] )
                                       ) * stsxyter::kdClockCycleNs
                                       - fdStartTime
                                     )* 1e-9;

               fhFractionGoodHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionDupliHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionTsMsbSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEpochSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 1.0 );
               fhFractionEmptySpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );

               fhFractionGoodHitsEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionDupliHitsEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionTsMsbEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEpochEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 1.0 );
               fhFractionEmptyEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );

               fhFractionTypes->Fill( 0., 0.0 ); // Good Hit
               fhFractionTypes->Fill( 1., 0.0 ); // Dupli Hit
               fhFractionTypes->Fill( 2., 0.0 ); // Ts MSB
               fhFractionTypes->Fill( 3., 1.0 ); // Epoch
               fhFractionTypes->Fill( 4., 0.0 ); // Empty

               if( 0 < uIdx )
                  LOG(info) << "CbmCosy2018MonitorDupli::DoUnpack => "
                            << "EPOCH message at unexpected position in MS: message "
                            << uIdx << " VS message 0 expected!";
               break;
            } // case stsxyter::MessType::TsMsb :
            case stsxyter::MessType::Empty :
            {
//               FillTsMsbInfo( mess );

               Double_t dTsMsbTime = ( (  static_cast<ULong64_t>( stsxyter::kuHitNbTsBins )
                                        * static_cast<ULong64_t>( fvulCurrentTsMsb[fuCurrDpbIdx])
                                        + static_cast<ULong64_t>( stsxyter::kulTsCycleNbBins )
                                        * static_cast<ULong64_t>( fvuCurrentTsMsbCycle[fuCurrDpbIdx] )
                                       ) * stsxyter::kdClockCycleNs
                                       - fdStartTime
                                     )* 1e-9;

               fhFractionGoodHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionDupliHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionTsMsbSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEpochSpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEmptySpillEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 1.0 );

               fhFractionGoodHitsEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionDupliHitsEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionTsMsbEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEpochEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 0.0 );
               fhFractionEmptyEvo[ fuCurrDpbIdx ]->Fill( dTsMsbTime, 1.0 );

               fhFractionTypes->Fill( 0., 0.0 ); // Good Hit
               fhFractionTypes->Fill( 1., 0.0 ); // Dupli Hit
               fhFractionTypes->Fill( 2., 0.0 ); // Ts MSB
               fhFractionTypes->Fill( 3., 0.0 ); // Epoch
               fhFractionTypes->Fill( 4., 1.0 ); // Empty

               break;
            } // case stsxyter::MessType::Empty :
            case stsxyter::MessType::Dummy :
            {
               break;
            } // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
            default:
            {
               LOG(fatal) << "CbmCosy2018MonitorDupli::DoUnpack => "
                          << "Unknown message type, should never happen, stopping here!";
            }
         } // switch( mess.GetMessType() )
      } // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )

      // Sort the buffer of hits
      std::sort( fvmHitsInTs.begin(), fvmHitsInTs.end() );

      // Time differences plotting using the fully time sorted hits
/*
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
*/
      if( 0 < fvmHitsInTs.size() )
      {
//         ULong64_t ulLastHitTime = ( *( fvmHitsInTs.rbegin() ) ).GetTs();
         std::vector< stsxyter::FinalHit >::iterator it;

//         std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
//         Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();

         for( it  = fvmHitsInTs.begin();
              it != fvmHitsInTs.end();
//              it != fvmHitsInTs.end() && (*it).GetTs() < ulLastHitTime - 320; // 32 * 3.125 ns = 1000 ns
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
               fhFractionGoodHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
               fhFractionDupliHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 1.0 );
               fhFractionTsMsbSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
               fhFractionEpochSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
               fhFractionEmptySpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );

               fhFractionGoodHitsEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
               fhFractionDupliHitsEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 1.0 );
               fhFractionTsMsbEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
               fhFractionEpochEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
               fhFractionEmptyEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );

               fhFractionTypes->Fill( 0., 0.0 ); // Good Hit
               fhFractionTypes->Fill( 1., 1.0 ); // Dupli Hit
               fhFractionTypes->Fill( 2., 0.0 ); // Ts MSB
               fhFractionTypes->Fill( 3., 0.0 ); // Epoch
               fhFractionTypes->Fill( 4., 0.0 ); // Empty

               if( 0 == fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] - usHitAdc )
               {
                  fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] ++;
                  fvuNbHitSameTsAdcAsicLastS[ usAsicIdx ] ++;

                  fhRatioSameTsAdcSpillEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 1.0 );
                  fhRatioSameAdcSameTsSpillEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 1.0 );

                  if( 0 == fvuNbSameFullHitAsic[ usAsicIdx ] && 1 < fvuNbDiffFullHitAsic[ usAsicIdx ] )
                  {
                     UInt_t uBitPattComp = 0;
                     UInt_t uBitPattLsb  = 0;
                     UInt_t uBitPattMsb  = 0;
                     ULong64_t ulDuplHitTs = fvmLastHitsAsic[ usAsicIdx ][ fvuLastHitBufferIdx[ usAsicIdx ] ].GetTs();

                     fhAsicDuplicTs[ usAsicIdx ]->Fill( usChanIdx,     ( ulDuplHitTs & 0x000FF ) );
                     fhAsicDuplicTsMsb[ usAsicIdx ]->Fill( usChanIdx,  ( ulDuplHitTs & 0x0FF00 ) >> 8 );
                     fhAsicDuplicTsFull[ usAsicIdx ]->Fill( usChanIdx, ( ulDuplHitTs & 0x03FFF ) );

                     if( 0 == usAsicIdx )
                     {
                        if( 0 == usChanIdx )
                           fhAsicDuplicTsEvoAsic0Chan01->Fill( dTimeSinceStartSec, ( ulDuplHitTs & 0x00FFF ) );
                        else if( 20 == usChanIdx )
                           fhAsicDuplicTsEvoAsic0Chan09->Fill( dTimeSinceStartSec, ( ulDuplHitTs & 0x00FFF ) );
                        else if( 99 == usChanIdx )
                           fhAsicDuplicTsEvoAsic0Chan16->Fill( dTimeSinceStartSec, ( ulDuplHitTs & 0x00FFF ) );
                     } // if( 0 == usAsicIdx )

                     for( UInt_t uBit = 0; uBit < 14; ++uBit )
                     {
                        fhAsicDuplicTsBitPattern[ usAsicIdx ]->Fill( uBit, ( ( ulDuplHitTs >> uBit ) & 0x1) );
                     } // for( UInt_t uBit = 0; uBit < 14; ++uBit )

                     for( UInt_t uPrevHit = 0; uPrevHit < fuNbChanPerAsic; ++uPrevHit )
                     {
                        UInt_t uBufferIdx = ( fvuLastHitBufferIdx[ usAsicIdx ] < uPrevHit ) ?
                                            ( fvuLastHitBufferIdx[ usAsicIdx ] - uPrevHit + fuNbChanPerAsic ):
                                            ( fvuLastHitBufferIdx[ usAsicIdx ] - uPrevHit );

                        ULong64_t ulPrevHitTs = fvmLastHitsAsic[ usAsicIdx ][ uBufferIdx ].GetTs();
                        UInt_t uCompTs = ( ulPrevHitTs & 0x01FC0 ) >> 6;
                        UInt_t uTsLsb  = ( ulPrevHitTs & 0x000FF );
                        UInt_t uTsMsb  = ( ulPrevHitTs & 0x03F00 ) >> 8;

                        uBitPattComp |= uCompTs;
                        uBitPattLsb  |= uTsLsb;
                        uBitPattMsb  |= uTsMsb;

                        Double_t dDt = static_cast< Double_t >(ulDuplHitTs) - static_cast< Double_t >(ulPrevHitTs);

                        if( 2048 < dDt )
                           break;

                        fhAsicDuplicDtLastHits[ usAsicIdx ]->Fill( dDt, uPrevHit );
                        fhAsicDuplicCompTs[ usAsicIdx ]->Fill( uPrevHit, uCompTs );
                        fhAsicDuplicTsLsb[ usAsicIdx ]->Fill(  uPrevHit, uTsLsb );
                        fhAsicDuplicTsMsbAsic[ usAsicIdx ]->Fill(  uPrevHit, uTsMsb );

                        for( UInt_t uBit = 0; uBit < 8; ++uBit )
                        {
                           if( 1 == ( ( uBitPattComp >> uBit ) & 0x1) )
                              fhAsicDuplicCompTsBitThere[ usAsicIdx ]->Fill( uPrevHit, uBit );

                           if( 1 == ( ( uBitPattLsb >> uBit ) & 0x1) )
                              fhAsicDuplicTsLsbBitThere[ usAsicIdx ]->Fill( uPrevHit, uBit );

                           if( 1 == ( ( uBitPattMsb >> uBit ) & 0x1) )
                              fhAsicDuplicTsMsbBitThere[ usAsicIdx ]->Fill( uPrevHit, uBit );
                        } // for( UInt_t uBit = 0; uBit < 8; ++uBit )
                     } // for( UInt_t uPrevHit = 0; uPrevHit < fuNbChanPerAsic; ++uPrevHit )
                  } // if( 0 == fvuNbSameFullHitAsic[ usAsicIdx ] && 1 < fvuNbDiffFullHitAsic[ usAsicIdx ] )
                  fvuNbSameFullHitAsic[ usAsicIdx ] ++;
                  fvuNbDiffFullHitAsic[ usAsicIdx ] = 0;
               } // if( 0 == fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] - usHitAdc )
                  else
                  {
                     fhSetupSortedNbConsSameTsAdcChan[ usAsicIdx ]->Fill( usChanIdx, fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] );
                     fhSetupSortedNbConsSameTsVsTsAdc[ usAsicIdx ]->Fill( fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ],
                                                                          fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] );
                     fvuNbSameFullHitAsicChan[ usAsicIdx ][ usChanIdx ] = 0;

                     fhRatioSameAdcSameTsSpillEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 0.0 );
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

               fhRatioSameTsSpillEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 1.0 );

               if( 1 == fvuNbSameHitAsicChan[ usAsicIdx ][ usChanIdx ] )
                  fhSetupSortedTsFirstDuplicateChan[ usAsicIdx ]->Fill( usChanIdx, ulHitTs % 512 );
            } // if( 0 == ulHitTs - fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] )
               else
               {
                  fhFractionGoodHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 1.0 );
                  fhFractionDupliHitsSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
                  fhFractionTsMsbSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
                  fhFractionEpochSpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
                  fhFractionEmptySpillEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );

                  fhFractionGoodHitsEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 1.0 );
                  fhFractionDupliHitsEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
                  fhFractionTsMsbEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
                  fhFractionEpochEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );
                  fhFractionEmptyEvo[ fuCurrDpbIdx ]->Fill( dTimeSinceStartSec, 0.0 );

                  fhFractionTypes->Fill( 0., 1.0 ); // Good Hit
                  fhFractionTypes->Fill( 1., 0.0 ); // Dupli Hit
                  fhFractionTypes->Fill( 2., 0.0 ); // Ts MSB
                  fhFractionTypes->Fill( 3., 0.0 ); // Epoch
                  fhFractionTypes->Fill( 4., 0.0 ); // Empty
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

                  fhRatioSameTsSpillEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 0.0 );
                  fhRatioSameTsAdcSpillEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx, 0.0 );

                  fhHodoChanCntGood[ usAsicIdx ]->Fill( usChanIdx, usHitAdc );
                  fhHodoChanGoodHitRateEvo[ usAsicIdx ]->Fill( dTimeSinceStartSec, usChanIdx );

                  fvuNbHitDiffTsAsicLastS[ usAsicIdx ]++;

                  fhSetupSortedTsGoodChan[ usAsicIdx ]->Fill( usChanIdx, ulHitTs % 512 );

                  if( 0 < fvuNbDiffFullHitAsic[ usAsicIdx ] )
                  {
                     fhAsicGoodTs[ usAsicIdx ]->Fill( usChanIdx,     ( ulHitTs & 0x000FF ) );
                     fhAsicGoodTsMsb[ usAsicIdx ]->Fill( usChanIdx,  ( ulHitTs & 0x0FF00 ) >> 8 );
                     fhAsicGoodTsFull[ usAsicIdx ]->Fill( usChanIdx, ( ulHitTs & 0x03FFF ) );
                     if( 0 == usAsicIdx )
                     {
                        if( 0 == usChanIdx )
                        {
                           fhAsicGoodTsEvoAsic0Chan01->Fill( dTimeSinceStartSec,     ( ulHitTs & 0x00FFF ) );
                           fhAsicGoodTsMsbEvoAsic0Chan01->Fill( dTimeSinceStartSec,  ( ulHitTs & 0x0FF00 ) >> 8 );
                           fhAsicGoodTsFullEvoAsic0Chan01->Fill( dTimeSinceStartSec, ( ulHitTs & 0x03FFF ) );
                           fhAsicGoodTsFullEvoProfAsic0Chan01->Fill( dTimeSinceStartSec, ( ulHitTs & 0x03FFF ) );
                        } //if( 3 == usChanIdx )
                        else if( 20 == usChanIdx )
                        {
                           fhAsicGoodTsEvoAsic0Chan09->Fill( dTimeSinceStartSec, ( ulHitTs & 0x00FFF ) );
                           fhAsicGoodTsFullEvoProfAsic0Chan09->Fill( dTimeSinceStartSec, ( ulHitTs & 0x03FFF ) );
                        } // else if( 20 == usChanIdx )
                        else if( 99 == usChanIdx )
                        {
                           fhAsicGoodTsEvoAsic0Chan16->Fill( dTimeSinceStartSec, ( ulHitTs & 0x00FFF ) );
                           fhAsicGoodTsFullEvoProfAsic0Chan16->Fill( dTimeSinceStartSec, ( ulHitTs & 0x03FFF ) );
                        } // else if( 99 == usChanIdx )
                     } // if( 0 == usAsicIdx )

                     for( UInt_t uBit = 0; uBit < 14; ++uBit )
                     {
                        fhAsicGoodTsBitPattern[ usAsicIdx ]->Fill( uBit, ( ( ulHitTs >> uBit ) & 0x1) );
                     } // for( UInt_t uBit = 0; uBit < 14; ++uBit )
                  } // if( 0 < fvuNbDiffFullHitAsic[ usAsicIdx ] )

                  fvuNbSameFullHitAsic[ usAsicIdx ] = 0;
                  fvuNbDiffFullHitAsic[ usAsicIdx ] ++;
                  fvuLastHitBufferIdx[ usAsicIdx ] = ( fvuLastHitBufferIdx[ usAsicIdx ] + 1 ) % fuNbChanPerAsic;
                  fvmLastHitsAsic[ usAsicIdx ][ fvuLastHitBufferIdx[ usAsicIdx ] ] = (*it);
               } // else of if( 0 == ulHitTs - fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] )
            fvulTimeLastHitAsicChan[ usAsicIdx ][ usChanIdx ] = ulHitTs;
            fviAdcLastHitAsicChan[ usAsicIdx ][ usChanIdx ] = usHitAdc;

            if( 0 == usAsicIdx && fuPulseIdx < fuPulseIdxMax )
            {
               fhAsicMissedChanIdVsPulseIdx->Fill( fuPulseIdx, usChanIdx );
               switch( usChanIdx )
               {
                  case 31:
                     fhAsicMissedChanGroupVsPulseIdx->Fill( fuPulseIdx, 0 );
                     break;
                  case 35:
                     fhAsicMissedChanGroupVsPulseIdx->Fill( fuPulseIdx, 1 );
                     break;
                  case 39:
                     fhAsicMissedChanGroupVsPulseIdx->Fill( fuPulseIdx, 2 );
                     break;
                  default:
                  break;
               } // switch( usChanIdx )
            } // if( 0 == usAsicIdx && fuPulseIdx < fuPulseIdxMax )
         } // loop on hits untils hits within 100 ns of last one or last one itself are reached
         fuPulseIdx ++;

         // Remove all hits which were already used
         fvmHitsInTs.erase( fvmHitsInTs.begin(), it );
      } // if( 0 < fvmHitsInTs.size() )
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

void CbmCosy2018MonitorDupli::FillHitInfo( stsxyter::Message mess, const UShort_t & /*usElinkIdx*/, const UInt_t & uAsicIdx, const UInt_t & uMsIdx )
{
   UShort_t usChan   = mess.GetHitChannel();
   UShort_t usRawAdc = mess.GetHitAdc();
//   UShort_t usFullTs = mess.GetHitTimeFull();
//   UShort_t usTsOver = mess.GetHitTimeOver();
   UShort_t usRawTs  = mess.GetHitTime();
   // ajit
   if(uAsicIdx == 2)fhStsRawAdc[ usChan ]->Fill( usRawAdc );
   if(uAsicIdx == 2) fhStsRawAdcCombine->Fill(usRawAdc);

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


   if( fuPrintMessagesIdx < fkuNbPrintMessages )
//   if( ( 5949 == fulCurrentTsIdx ) )
   {
      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " MsInTs " << std::setw( 3 ) << uMsIdx
                << " eLINK " << std::setw( 3 ) << mess.GetLinkIndex()
                << " Asic " << std::setw( 2 ) << uAsicIdx
                << " Channel " << std::setw( 3 ) << usChan
                << " ADC " << std::setw( 3 ) << usRawAdc
                << " TS " << std::setw( 3 )  << usRawTs // 9 bits TS
                << " TsMsb " << std::setw( 7 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " MsbCy " << std::setw( 4 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " Time " << std::setw ( 12 ) << fvulChanLastHitTime[ uAsicIdx ][ usChan ];
      fuPrintMessagesIdx++;
   } // if( fuPrintMessagesIdx < kuNbPrintMessages )

   // Check Starting point of histos with time as X axis
   if( -1 == fdStartTime )
      fdStartTime = fvdChanLastHitTime[ uAsicIdx ][ usChan ];

   // Fill histos with time as X axis
   Double_t dTimeSinceStartSec = (fvdChanLastHitTime[ uAsicIdx ][ usChan ] - fdStartTime)* 1e-9;
   Double_t dTimeSinceStartMin = dTimeSinceStartSec / 60.0;
   fhHodoChanHitRateEvo[ uAsicIdx ]->Fill( dTimeSinceStartSec , usChan );
   if( 0 == uAsicIdx )
      fhHodoChanHitRateEvoZoom->Fill( dTimeSinceStartSec , usChan );
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
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      if( uXyterIdx == uAsicIdx )
      {
         fhFractionHitsAsicSpillEvo[ uXyterIdx ]->Fill( dTimeSinceStartSec, 1.0 );
         fhFractionHitsAsicEvo[ uXyterIdx ]->Fill( dTimeSinceStartSec, 1.0 );
         fhFractionAsics->Fill( uXyterIdx, 1.0);
      }
         else
         {
            fhFractionHitsAsicSpillEvo[ uXyterIdx ]->Fill( dTimeSinceStartSec, 0.0 );
            fhFractionHitsAsicEvo[ uXyterIdx ]->Fill( dTimeSinceStartSec, 0.0 );
            fhFractionAsics->Fill( uXyterIdx, 0.0 );
         }
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
/*
   if( 596.51 < dTimeSinceStartSec && dTimeSinceStartSec < 597.07 &&
       0 == uAsicIdx && 3 == usChan )
      LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " ADC " << std::setw( 3 ) << usRawAdc
                << " TS " << std::setw( 3 )  << usRawTs // 9 bits TS
                << " SX TsMsb " << std::setw( 2 ) << ( fvulCurrentTsMsb[fuCurrDpbIdx] & 0x1F ) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b TS_MSB after DPB
                << " DPB TsMsb " << std::setw( 6 ) << ( fvulCurrentTsMsb[fuCurrDpbIdx] >> 5 ) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b of TS_MSB after DPB
                << " TsMsb " << std::setw( 7 ) << fvulCurrentTsMsb[fuCurrDpbIdx]
                << " MsbCy " << std::setw( 4 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " Time " << std::setw ( 12 ) << fvulChanLastHitTime[ uAsicIdx ][ usChan ]
                << " Time " << std::setw ( 5 ) << dTimeSinceStartSec;
*/

   // Print TS jumps
//   if( (ulOldHitTime & 0x00003C00) != (fvulChanLastHitTime[ uAsicIdx ][ usChan ] & 0x00003C00) && ( 255 != (ulOldHitTime & 0xFF) ) && (255 != (usRawTs & 0xFF) ))
/*
   if( 0 == usChan &&
       ( ( (ulOldHitTime & 0x00003FFF) + 2 < (fvulChanLastHitTime[ uAsicIdx ][ usChan ] & 0x00003FFF ) ) ||
         ( (ulOldHitTime & 0x00003FFF) > (fvulChanLastHitTime[ uAsicIdx ][ usChan ] & 0x00003FFF ) + 2 ) ) &&
       ( ( 11620 > (fvulChanLastHitTime[ uAsicIdx ][ usChan ] & 0x00003FFF ) ) ||
         ( 11630 < (fvulChanLastHitTime[ uAsicIdx ][ usChan ] & 0x00003FFF ) ) )
      )
   {
      Long64_t ulNewHitTime = fvulChanLastHitTime[ uAsicIdx ][ usChan ];
      LOG(info) << " Old Hit chan " << std::setw( 3 ) << usChan
                << " TS " << std::setw( 3 )  << (ulOldHitTime & 0xFF) // 8 bits TS
                << " Full TS " << std::setw( 5 )  << (ulOldHitTime & 0x3FFF) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b of TS_MSB after DPB
                << " SX TsMsb " << std::setw( 2 ) << ( ( ulOldHitTime >>  8 ) & 0x3 ) // 2 lower bits of TS_MSB from overlap bits in hit frame or from DPB
                << " DPB TsMsb " << std::setw( 9 ) << ( (ulOldHitTime >> 10 ) & 0xF ) // 4 (+16) higher bits of TS_MSB from DPB
                << " TsMsb " << std::setw( 9 ) << (ulOldHitTime >> 8)
                << " MsbCy " << std::setw( 4 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " Time " << std::setw ( 12 ) << ulOldHitTime
                << " Time " << std::setw ( 5 ) << (ulOldHitTime * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;
      LOG(info) << " New Hit chan " << std::setw( 3 ) << usChan
                << " TS " << std::setw( 3 )  << (ulNewHitTime & 0xFF) // 8 bits TS
                << " Full TS " << std::setw( 5 )  << (ulNewHitTime & 0x3FFF) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b of TS_MSB after DPB
                << " SX TsMsb " << std::setw( 2 ) << ( ( ulNewHitTime >>  8 ) & 0x3 ) // 2 lower bits of TS_MSB from overlap bits in hit frame or from DPB
                << " DPB TsMsb " << std::setw( 9 ) << ( (ulNewHitTime >> 10 ) & 0xF ) // 4 (+16) higher bits of TS_MSB from DPB
                << " TsMsb " << std::setw( 9 ) << (ulNewHitTime >> 8)
                << " MsbCy " << std::setw( 4 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                << " Time " << std::setw ( 12 ) << fvulChanLastHitTime[ uAsicIdx ][ usChan ]
                << " Time " << std::setw ( 5 ) << dTimeSinceStartSec;
    } // if( ulOldHitTime & 0x00003C00 != fvulChanLastHitTime[ uAsicIdx ][ usChan ] & 0x00003C00 )
*/
   if( 0 == usChan )
   {
/*
      if( 22.2 < dTimeSinceStartSec && dTimeSinceStartSec < 22.6 )
//      if( (443.574 < dTimeSinceStartSec && dTimeSinceStartSec < 443.575) || 32 != fuNbTsMsbSinceLastHit )
//      if( 32 != fuNbTsMsbSinceLastHit )
      {
         Long64_t ulNewHitTime = fvulChanLastHitTime[ uAsicIdx ][ usChan ];
         LOG(info) << fuNbTsMsbSinceLastHit << " TS_MSB since last hit on channel 0 ";
         LOG(info) << " New Hit chan " << std::setw( 3 ) << usChan
                   << " TS " << std::setw( 3 )  << (ulNewHitTime & 0xFF) // 8 bits TS
                   << " Full TS " << std::setw( 5 )  << (ulNewHitTime & 0x3FFF) // Total StsXyter TS = 14 bits => 9b Hit TS + lower 5b of TS_MSB after DPB
                   << " SX TsMsb " << std::setw( 2 ) << ( ( ulNewHitTime >>  8 ) & 0x3 ) // 2 lower bits of TS_MSB from overlap bits in hit frame or from DPB
                   << " DPB TsMsb " << std::setw( 9 ) << ( (ulNewHitTime >> 10 ) & 0xF ) // 4 (+16) higher bits of TS_MSB from DPB
                   << " TsMsb " << std::setw( 9 ) << (ulNewHitTime >> 8)
                   << " MsbCy " << std::setw( 4 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                   << " Time " << std::setw ( 12 ) << fvulChanLastHitTime[ uAsicIdx ][ usChan ]
                   << " Time " << std::setw ( 5 ) << dTimeSinceStartSec;
       } // if( 443.573 < dTimeSinceStartSec && dTimeSinceStartSec < 443.672 )
*/
      fuNbTsMsbSinceLastHit = 0;
   } // if( 0 == usChan )
   fuNbHitsLastTsMsb++;

   /// Pulse counting for pulser
   if( 0 == fvuAsicTimeLastPulse[ uAsicIdx ] )
   {
      /// Update the time of last Pulse
      fvuAsicTimeLastPulse[ uAsicIdx ] = fvulChanLastHitTime[ uAsicIdx ][ usChan ];
      fvbPulseThereChan[ uAsicIdx ][ usChan ] = kTRUE;
   } // if( 0 == fvuAsicTimeLastPulse[ uAsicIdx ] )
   else if( fvulChanLastHitTime[ uAsicIdx ][ usChan ] - fvuAsicTimeLastPulse[ uAsicIdx ] < 10 )
   {
      fvbPulseThereChan[ uAsicIdx ][ usChan ] = kTRUE;
   } // if( fvulChanLastHitTime[ uAsicIdx ][ usChan ] - fvuAsicTimeLastPulse[ uAsicIdx ] < 10 )
      else
      {
         UInt_t uNbChanThere = 0;
         for( UInt_t uChanLoop = 0; uChanLoop < fuNbChanPerAsic; ++uChanLoop )
         {
            if( kTRUE == fvbPulseThereChan[ uAsicIdx ][ uChanLoop ] )
               uNbChanThere++;
            fvbPulseThereChan[ uAsicIdx ][ uChanLoop ] = kFALSE;
         } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )

         /// Fill the count histo
         fhPulseChanCountEvo[ uAsicIdx ]->Fill( dTimeSinceStartSec, uNbChanThere );

         /// Update the time of last Pulse
         fvuAsicTimeLastPulse[ uAsicIdx ] = fvulChanLastHitTime[ uAsicIdx ][ usChan ];
         fvbPulseThereChan[ uAsicIdx ][ usChan ] = kTRUE;
      } // else of if( fvulChanLastHitTime[ uAsicIdx ][ usChan ] - fvuAsicTimeLastPulse[ uAsicIdx ] < 10 )
}

void CbmCosy2018MonitorDupli::FillTsMsbInfo( stsxyter::Message mess, UInt_t /*uMessIdx*/, UInt_t /*uMsIdx*/ )
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
   if( uVal != fvulCurrentTsMsb[fuCurrDpbIdx] )
      fuNbTsMsbSinceLastHit++;

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

   // Check Starting point of histos with time as X axis
   if( 0 < fdStartTime )
   {
//      Double_t dTimeSinceStartSec = ( ulNewTsMsbTime * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;
/*
//      if( 443.573 < dTimeSinceStartSec && dTimeSinceStartSec < 443.672 )
      if( 443.574 < dTimeSinceStartSec && dTimeSinceStartSec < 443.575 )
         LOG(info) << " TS " << std::setw( 12 ) << fulCurrentTsIdx
                   << " MS " << std::setw( 12 ) << fulCurrentMsIdx
                   << " TsMsb " << std::setw( 9 ) << uVal
                   << " MsbCy " << std::setw( 5 ) << fvuCurrentTsMsbCycle[fuCurrDpbIdx]
                   << " Time " << std::setw ( 12 ) << ulNewTsMsbTime
                   << " Time " << std::setw ( 5 ) << dTimeSinceStartSec
                   << " Nb Hits last TS_MSB " << fuNbHitsLastTsMsb;
*/
      fuNbHitsLastTsMsb = 0;
   } // if( 0 < fdStartTime )


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

void CbmCosy2018MonitorDupli::FillEpochInfo( stsxyter::Message /*mess*/ )
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

void CbmCosy2018MonitorDupli::Reset()
{
}

void CbmCosy2018MonitorDupli::Finish()
{

   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmCosy2018MonitorDupli statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::MessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::MessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::MessType::Dummy ] << "\n"
             << " Epoch    messages: " << fmMsgCounter[ stsxyter::MessType::Epoch ] << "\n"
             << " Empty    messages: " << fmMsgCounter[ stsxyter::MessType::Empty ];

   LOG(info) << "-------------------------------------";

   SaveAllHistos( fsHistoFileFullname );
   LOG(info) << "CbmCosy2018MonitorDupli done saving histos ";
   LOG(info) << "-------------------------------------";

//   SaveAllHistos();

}


void CbmCosy2018MonitorDupli::FillOutput(boost::any)
{
}

void CbmCosy2018MonitorDupli::SaveAllHistos( TString sFileName )
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

   gDirectory->mkdir("Sts_Raw");
   gDirectory->cd("Sts_Raw");

   fhHodoMessType->Write();
   fhHodoSysMessType->Write();
   fhHodoMessTypePerDpb->Write();
   fhHodoSysMessTypePerDpb->Write();
   fhHodoMessTypePerElink->Write();
   fhHodoSysMessTypePerElink->Write();
   fhStsRawAdcCombine->Write();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
         if(uXyterIdx == 2)
    {
      for( UInt_t uChannelIdx = 0; uChannelIdx < fuNbChanPerAsic; ++uChannelIdx )
      {
      fhStsRawAdc[ uChannelIdx ]->Write();
      }
    }

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
   fhHodoChanHitRateEvoZoom->Write();

/*
   fhSetupSortedDtX1Y1->Write();
   fhSetupSortedDtX2Y2->Write();
   fhSetupSortedDtN1P1->Write();
   fhSetupSortedDtN2P2->Write();
   fhSetupSortedDtX1Y1X2Y2->Write();
   fhSetupSortedDtN1P1N2P2->Write();
   fhSetupSortedDtX1Y1X2Y2N1P1->Write();
   fhSetupSortedDtX1Y1X2Y2N2P2->Write();
   fhSetupSortedDtH1H2S1S2->Write();
   fhSetupSortedMapX1Y1->Write();
   fhSetupSortedMapX2Y2->Write();
   fhSetupSortedMapN1P1->Write();
   fhSetupSortedMapN2P2->Write();
   fhSetupSortedCntEvoX1Y1->Write();
   fhSetupSortedCntEvoX2Y2->Write();
   fhSetupSortedCntEvoN1P1->Write();
   fhSetupSortedCntEvoN2P2->Write();

   fhBothHodoSortedDtX1Y1->Write();
   fhBothHodoSortedDtX2Y2->Write();
   fhBothHodoSortedDtX1Y1X2Y2N1P1->Write();
   fhBothHodoSortedDtX1Y1X2Y2N2P2->Write();
   fhBothHodoSortedDtH1H2S1S2->Write();
   fhBothHodoSortedMapX1Y1->Write();
   fhBothHodoSortedMapX2Y2->Write();
   fhBothHodoSortedCntEvoX1Y1->Write();
   fhBothHodoSortedCntEvoX2Y2->Write();

   fhH1H2S1SortedDtX1Y1->Write();
   fhH1H2S1SortedDtX2Y2->Write();
   fhH1H2S1SortedDtN1P1->Write();
   fhH1H2S1SortedDtN2P2->Write();
   fhH1H2S1SortedMapX1Y1->Write();
   fhH1H2S1SortedMapX2Y2->Write();
   fhH1H2S1SortedMapN1P1->Write();
   fhH1H2S1SortedMapN2P2->Write();
   fhH1H2S1SortedCntEvoX1Y1->Write();
   fhH1H2S1SortedCntEvoX2Y2->Write();
   fhH1H2S1SortedCntEvoN1P1->Write();
   fhH1H2S1SortedCntEvoN2P2->Write();

   fhH1H2S2SortedDtX1Y1->Write();
   fhH1H2S2SortedDtX2Y2->Write();
   fhH1H2S2SortedDtN1P1->Write();
   fhH1H2S2SortedDtN2P2->Write();
   fhH1H2S2SortedMapX1Y1->Write();
   fhH1H2S2SortedMapX2Y2->Write();
   fhH1H2S2SortedMapN1P1->Write();
   fhH1H2S2SortedMapN2P2->Write();
   fhH1H2S2SortedCntEvoX1Y1->Write();
   fhH1H2S2SortedCntEvoX2Y2->Write();
   fhH1H2S2SortedCntEvoN1P1->Write();
   fhH1H2S2SortedCntEvoN2P2->Write();

   fhSystSortedDtX1Y1->Write();
   fhSystSortedDtX2Y2->Write();
   fhSystSortedDtN1P1->Write();
   fhSystSortedDtN2P2->Write();
   fhSystSortedDtX1Y1X2Y2 ->Write();
   fhSystSortedMapX1Y1->Write();
   fhSystSortedMapX2Y2->Write();
   fhSystSortedMapN1P1->Write();
   fhSystSortedMapN2P2->Write();
   fhSystSortedCntEvoX1Y1->Write();
   fhSystSortedCntEvoX2Y2->Write();
   fhSystSortedCntEvoN1P1->Write();
   fhSystSortedCntEvoN2P2->Write();

   fhSystSortedDtN1X1vsN1X2->Write();
   fhSystSortedDtP1X1vsP1X2->Write();
   fhSystSortedDtN1X1vsP1X1->Write();
   fhSystSortedDtSts1Hodo1vsSts1Hodo2->Write();
   fhSystSortedDtSts2Hodo1vsSts2Hodo2->Write();
   fhSystSortedDtAllVsMapX1->Write();
   fhSystSortedDtAllVsMapY1->Write();
   fhSystSortedDtAllVsMapX2->Write();
   fhSystSortedDtAllVsMapY2->Write();

   fhSetupSortedDtX1->Write();
   fhSetupSortedDtY1->Write();
   fhSetupSortedDtX2->Write();
   fhSetupSortedDtY2->Write();
   fhSetupSortedDtN1->Write();
   fhSetupSortedDtP1->Write();
   fhSetupSortedDtN2->Write();
   fhSetupSortedDtP2->Write();
*/

   gDirectory->cd("..");

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
/*
      fhSetupSortedAsicRatioSameTsVsFlux[ uXyterIdx ]->Write();
      fhSetupSortedAsicRatioSameTsAdcVsFlux[ uXyterIdx ]->Write();
      fhSetupSortedAsicRatioSameAdcSameTsVsFlux[ uXyterIdx ]->Write();
*/
      fhRatioSameTsSpillEvo[ uXyterIdx ]->Write();
      fhRatioSameTsAdcSpillEvo[ uXyterIdx ]->Write();
      fhRatioSameAdcSameTsSpillEvo[ uXyterIdx ]->Write();

      fhSetupSortedTsFirstDuplicateChan[ uXyterIdx ]->Write();
      fhSetupSortedTsGoodChan[ uXyterIdx ]->Write();

      fhFractionHitsAsicSpillEvo[ uXyterIdx ]->Write();
      fhFractionHitsAsicEvo[ uXyterIdx ]->Write();

      fhAsicDuplicDtLastHits[ uXyterIdx ]->Write();
      fhAsicDuplicCompTs[ uXyterIdx ]->Write();
      fhAsicDuplicTsLsb[ uXyterIdx ]->Write();
      fhAsicDuplicTsMsbAsic[ uXyterIdx ]->Write();

      fhAsicDuplicTs[ uXyterIdx ]->Write();
      fhAsicGoodTs[ uXyterIdx ]->Write();
      fhAsicDuplicTsMsb[ uXyterIdx ]->Write();
      fhAsicGoodTsMsb[ uXyterIdx ]->Write();
      fhAsicDuplicTsFull[ uXyterIdx ]->Write();
      fhAsicGoodTsFull[ uXyterIdx ]->Write();

      fhHodoChanCntGood[ uXyterIdx ]->Write();
      fhHodoChanGoodHitRateEvo[ uXyterIdx ]->Write();
      fhPulseChanCountEvo[ uXyterIdx ]->Write();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   fhAsicGoodTsEvoAsic0Chan01->Write();
   fhAsicGoodTsMsbEvoAsic0Chan01->Write();
   fhAsicGoodTsFullEvoAsic0Chan01->Write();
   fhAsicGoodTsFullEvoProfAsic0Chan01->Write();
   fhAsicDuplicTsEvoAsic0Chan01->Write();
   fhAsicGoodTsEvoAsic0Chan09->Write();
   fhAsicDuplicTsEvoAsic0Chan09->Write();
   fhAsicGoodTsFullEvoProfAsic0Chan09->Write();
   fhAsicGoodTsEvoAsic0Chan16->Write();
   fhAsicDuplicTsEvoAsic0Chan16->Write();
   fhAsicGoodTsFullEvoProfAsic0Chan16->Write();
   fhAsicMissedChanIdVsPulseIdx->Write();
   fhAsicMissedChanGroupVsPulseIdx->Write();

   for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
   {
      fhFractionGoodHitsSpillEvo[ uDpbIdx ]->Write();
      fhFractionDupliHitsSpillEvo[ uDpbIdx ]->Write();
      fhFractionTsMsbSpillEvo[ uDpbIdx ]->Write();
      fhFractionEpochSpillEvo[ uDpbIdx ]->Write();
      fhFractionEmptySpillEvo[ uDpbIdx ]->Write();

      fhFractionGoodHitsEvo[ uDpbIdx ]->Write();
      fhFractionDupliHitsEvo[ uDpbIdx ]->Write();
      fhFractionTsMsbEvo[ uDpbIdx ]->Write();
      fhFractionEpochEvo[ uDpbIdx ]->Write();
      fhFractionEmptyEvo[ uDpbIdx ]->Write();
   } // for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
   fhFractionAsics->Write();
   fhFractionTypes->Write();

   cFebRatioDupli->Write();
   delete cFebRatioDupli;

   gDirectory->cd("..");

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

   if( "" != sFileName )
   {
      // Restore original directory position
      histoFile->Close();
      oldDir->cd();
   } // if( "" != sFileName )

}
void CbmCosy2018MonitorDupli::ResetAllHistos()
{
   LOG(info) << "Reseting all STS histograms.";

   fhHodoMessType->Reset();
   fhHodoSysMessType->Reset();
   fhHodoMessTypePerDpb->Reset();
   fhHodoSysMessTypePerDpb->Reset();
   fhHodoMessTypePerElink->Reset();
   fhHodoSysMessTypePerElink->Reset();
   fhStsRawAdcCombine->Reset();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
     if(uXyterIdx == 2)
    {
      for( UInt_t uChannelIdx = 0; uChannelIdx < fuNbChanPerAsic; ++uChannelIdx )
      {
      fhStsRawAdc[ uChannelIdx ]->Reset();
      }
    }

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
   fhHodoChanHitRateEvoZoom->Reset();

   fhSetupSortedDtX1Y1->Reset();
   fhSetupSortedDtX2Y2->Reset();
   fhSetupSortedDtN1P1->Reset();
   fhSetupSortedDtN2P2->Reset();
   fhSetupSortedDtX1Y1X2Y2->Reset();
   fhSetupSortedDtN1P1N2P2->Reset();
   fhSetupSortedDtX1Y1X2Y2N1P1->Reset();
   fhSetupSortedDtX1Y1X2Y2N2P2->Reset();
   fhSetupSortedDtH1H2S1S2->Reset();
   fhSetupSortedMapX1Y1->Reset();
   fhSetupSortedMapX2Y2->Reset();
   fhSetupSortedMapN1P1->Reset();
   fhSetupSortedMapN2P2->Reset();
   fhSetupSortedCntEvoX1Y1->Reset();
   fhSetupSortedCntEvoX2Y2->Reset();
   fhSetupSortedCntEvoN1P1->Reset();
   fhSetupSortedCntEvoN2P2->Reset();

   fhBothHodoSortedDtX1Y1->Reset();
   fhBothHodoSortedDtX2Y2->Reset();
   fhBothHodoSortedDtX1Y1X2Y2N1P1->Reset();
   fhBothHodoSortedDtX1Y1X2Y2N2P2->Reset();
   fhBothHodoSortedDtH1H2S1S2->Reset();
   fhBothHodoSortedMapX1Y1->Reset();
   fhBothHodoSortedMapX2Y2->Reset();
   fhBothHodoSortedCntEvoX1Y1->Reset();
   fhBothHodoSortedCntEvoX2Y2->Reset();

   fhH1H2S1SortedDtX1Y1->Reset();
   fhH1H2S1SortedDtX2Y2->Reset();
   fhH1H2S1SortedDtN1P1->Reset();
   fhH1H2S1SortedDtN2P2->Reset();
   fhH1H2S1SortedMapX1Y1->Reset();
   fhH1H2S1SortedMapX2Y2->Reset();
   fhH1H2S1SortedMapN1P1->Reset();
   fhH1H2S1SortedMapN2P2->Reset();
   fhH1H2S1SortedCntEvoX1Y1->Reset();
   fhH1H2S1SortedCntEvoX2Y2->Reset();
   fhH1H2S1SortedCntEvoN1P1->Reset();
   fhH1H2S1SortedCntEvoN2P2->Reset();

   fhH1H2S2SortedDtX1Y1->Reset();
   fhH1H2S2SortedDtX2Y2->Reset();
   fhH1H2S2SortedDtN1P1->Reset();
   fhH1H2S2SortedDtN2P2->Reset();
   fhH1H2S2SortedMapX1Y1->Reset();
   fhH1H2S2SortedMapX2Y2->Reset();
   fhH1H2S2SortedMapN1P1->Reset();
   fhH1H2S2SortedMapN2P2->Reset();
   fhH1H2S2SortedCntEvoX1Y1->Reset();
   fhH1H2S2SortedCntEvoX2Y2->Reset();
   fhH1H2S2SortedCntEvoN1P1->Reset();
   fhH1H2S2SortedCntEvoN2P2->Reset();

   fhSystSortedDtX1Y1->Reset();
   fhSystSortedDtX2Y2->Reset();
   fhSystSortedDtN1P1->Reset();
   fhSystSortedDtN2P2->Reset();
   fhSystSortedDtX1Y1X2Y2 ->Reset();
   fhSystSortedMapX1Y1->Reset();
   fhSystSortedMapX2Y2->Reset();
   fhSystSortedMapN1P1->Reset();
   fhSystSortedMapN2P2->Reset();
   fhSystSortedCntEvoX1Y1->Reset();
   fhSystSortedCntEvoX2Y2->Reset();
   fhSystSortedCntEvoN1P1->Reset();
   fhSystSortedCntEvoN2P2->Reset();

   fhSystSortedDtN1X1vsN1X2->Reset();
   fhSystSortedDtP1X1vsP1X2->Reset();
   fhSystSortedDtN1X1vsP1X1->Reset();
   fhSystSortedDtSts1Hodo1vsSts1Hodo2->Reset();
   fhSystSortedDtSts2Hodo1vsSts2Hodo2->Reset();
   fhSystSortedDtAllVsMapX1->Reset();
   fhSystSortedDtAllVsMapY1->Reset();
   fhSystSortedDtAllVsMapX2->Reset();
   fhSystSortedDtAllVsMapY2->Reset();

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

      fhRatioSameTsSpillEvo[ uXyterIdx ]->Reset();
      fhRatioSameTsAdcSpillEvo[ uXyterIdx ]->Reset();
      fhRatioSameAdcSameTsSpillEvo[ uXyterIdx ]->Reset();

      fhSetupSortedTsFirstDuplicateChan[ uXyterIdx ]->Reset();
      fhSetupSortedTsGoodChan[ uXyterIdx ]->Reset();

      fhFractionHitsAsicSpillEvo[ uXyterIdx ]->Reset();
      fhFractionHitsAsicEvo[ uXyterIdx ]->Reset();

      fhAsicDuplicDtLastHits[ uXyterIdx ]->Reset();
      fhAsicDuplicCompTs[ uXyterIdx ]->Reset();
      fhAsicDuplicTsLsb[ uXyterIdx ]->Reset();
      fhAsicDuplicTsMsbAsic[ uXyterIdx ]->Reset();
      fhAsicDuplicCompTsBitThere[ uXyterIdx ]->Reset();
      fhAsicDuplicTsLsbBitThere[ uXyterIdx ]->Reset();
      fhAsicDuplicTsMsbBitThere[ uXyterIdx ]->Reset();
      fhAsicDuplicTs[ uXyterIdx ]->Reset();
      fhAsicGoodTs[ uXyterIdx ]->Reset();
      fhAsicDuplicTsMsb[ uXyterIdx ]->Reset();
      fhAsicGoodTsMsb[ uXyterIdx ]->Reset();
      fhAsicDuplicTsFull[ uXyterIdx ]->Reset();
      fhAsicGoodTsFull[ uXyterIdx ]->Reset();
      fhAsicDuplicTsBitPattern[ uXyterIdx ]->Reset();
      fhAsicGoodTsBitPattern[ uXyterIdx ]->Reset();

      fhHodoChanCntGood[ uXyterIdx ]->Reset();
      fhHodoChanGoodHitRateEvo[ uXyterIdx ]->Reset();
      fhPulseChanCountEvo[ uXyterIdx ]->Reset();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   fhAsicGoodTsEvoAsic0Chan01->Reset();
   fhAsicGoodTsMsbEvoAsic0Chan01->Reset();
   fhAsicGoodTsFullEvoAsic0Chan01->Reset();
   fhAsicGoodTsFullEvoProfAsic0Chan01->Reset();
   fhAsicDuplicTsEvoAsic0Chan01->Reset();
   fhAsicGoodTsEvoAsic0Chan09->Reset();
   fhAsicDuplicTsEvoAsic0Chan09->Reset();
   fhAsicGoodTsFullEvoProfAsic0Chan09->Reset();
   fhAsicGoodTsEvoAsic0Chan16->Reset();
   fhAsicDuplicTsEvoAsic0Chan16->Reset();
   fhAsicGoodTsFullEvoProfAsic0Chan16->Reset();
   fuPulseIdx = 0;
   fhAsicMissedChanIdVsPulseIdx->Reset();
   fhAsicMissedChanGroupVsPulseIdx->Reset();

   for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
   {
      fhFractionGoodHitsSpillEvo[ uDpbIdx ]->Reset();
      fhFractionDupliHitsSpillEvo[ uDpbIdx ]->Reset();
      fhFractionTsMsbSpillEvo[ uDpbIdx ]->Reset();
      fhFractionEpochSpillEvo[ uDpbIdx ]->Reset();
      fhFractionEmptySpillEvo[ uDpbIdx ]->Reset();

      fhFractionGoodHitsEvo[ uDpbIdx ]->Reset();
      fhFractionDupliHitsEvo[ uDpbIdx ]->Reset();
      fhFractionTsMsbEvo[ uDpbIdx ]->Reset();
      fhFractionEpochEvo[ uDpbIdx ]->Reset();
      fhFractionEmptyEvo[ uDpbIdx ]->Reset();
   } // for( UInt_t uDpbIdx = 0; uDpbIdx < fuNrOfDpbs; ++uDpbIdx )
   fhFractionAsics->Reset();
   fhFractionTypes->Reset();

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

void CbmCosy2018MonitorDupli::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}

void CbmCosy2018MonitorDupli::SetLongDurationLimits( UInt_t uDurationSeconds, UInt_t uBinSize )
{
   fbLongHistoEnable     = kTRUE;
   fuLongHistoNbSeconds  = uDurationSeconds;
   fuLongHistoBinSizeSec = uBinSize;
}

ClassImp(CbmCosy2018MonitorDupli)
