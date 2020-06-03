// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorSts                         -----
// -----                Created 15/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCosy2018MonitorSts.h"

// Data

// CbmRoot
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

Bool_t bCosy2018ResetStsHistos = kFALSE;
Bool_t bCosy2018WriteStsHistos = kFALSE;

CbmCosy2018MonitorSts::CbmCosy2018MonitorSts() :
   CbmTSUnpack(),
   fuOverlapMsNb(0),
   fUnpackPar(NULL),
   fuNrOfDpbs(0),
   fDpbIdIndexMap(),
   fuNbElinksPerDpb(0),
   fuNbStsXyters(0),
   fuNbChanPerAsic(0),
   fvuElinkToAsic(),
   fbPrintMessages( kFALSE ),
   fPrintMessCtrl( stsxyter::MessagePrintMask::msg_print_Human ),
   fbChanHitDtEna( kFALSE ),
   fbDualStsEna( kFALSE ),
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
   fLastSortedHit1N(),
   fLastSortedHit1P(),
   fLastSortedHit2N(),
   fLastSortedHit2P(),
   fuMaxNbMicroslices(100),
   fHM(new CbmHistManager()),
   fhStsMessType(NULL),
   fhStsSysMessType(NULL),
   fhStsMessTypePerDpb(NULL),
   fhStsSysMessTypePerDpb(NULL),
   fhStsDpbRawTsMsb(NULL),
   fhStsDpbRawTsMsbSx(NULL),
   fhStsDpbRawTsMsbDpb(NULL),
   fhStsMessTypePerElink(NULL),
   fhStsSysMessTypePerElink(NULL),
   fhStsChanCounts(),
   fhStsChanRawAdc(),
   fhStsChanRawAdcProf(),
   fhStsChanRawTs(),
   fhStsChanMissEvt(),
   fhStsChanHitRateEvo(),
   fhStsXyterRateEvo(),
   fhStsChanHitRateEvoLong(),
   fhStsXyterRateEvoLong(),
   fhStsChanHitDt(),
   fhStsChanHitDtNeg(),
   fhStsChanHitsPerMs(),
   fhStsAsicTsMsb(NULL),
   fbLongHistoEnable( kFALSE ),
   fuLongHistoNbSeconds( 0 ),
   fuLongHistoBinSizeSec( 0 ),
   fuLongHistoBinNb( 0 ),
   fhFebRateEvoLong(),
   fhFebChRateEvoLong(),
   fhStsSortedDtN1P1(NULL),
   fhStsSortedDtN2P2(NULL),
   fhStsSortedDtN1N2(NULL),
   fhStsSortedDtP1P2(NULL),
   fhStsSortedDtN1P2(NULL),
   fhStsSortedDtP1N2(NULL),
   fhStsSortedMapN1P1(NULL),
   fhStsSortedMapN2P2(NULL),
   fhStsSortedMapN1N2(NULL),
   fhStsSortedMapP1P2(NULL),
   fhStsSortedMapN1P2(NULL),
   fhStsSortedMapP1N2(NULL),
   fhStsSortedCntEvoN1P1(NULL),
   fhStsSortedCntEvoN2P2(NULL),
   fhStsSortedCntEvoN1N2(NULL),
   fhStsSortedCntEvoP1P2(NULL),
   fhStsSortedCntEvoN1P2(NULL),
   fhStsSortedCntEvoP1N2(NULL),
   fhStsSortedDtN1P1N2P2(NULL),
   fhStsSortedMapN1P1Coinc(NULL),
   fhStsSortedMapN2P2Coinc(NULL),
   fhStsSortedDtVsDiffN(NULL),
   fhStsSortedDtVsDiffP(NULL),
   fdStereoAngleTan( TMath::Tan( kdStereoAngle * TMath::DegToRad() ) ),
   fiStripsOffsetN1(  69 ),
   fiStripsOffsetP1( -69 ),
   fiStripsOffsetN2(  69 ),
   fiStripsOffsetP2( -69 ),
   fhStsSortedMapX1Y1(NULL),
   fhStsSortedMapX2Y2(NULL),
   fhStsSortedMapX1Y1Coinc(NULL),
   fhStsSortedMapX2Y2Coinc(NULL),
   fcMsSizeAll(NULL)
{
}

CbmCosy2018MonitorSts::~CbmCosy2018MonitorSts()
{
}

Bool_t CbmCosy2018MonitorSts::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmCosy2018MonitorSts::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackPar = (CbmCern2017UnpackParSts*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParSts"));
}


Bool_t CbmCosy2018MonitorSts::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmCosy2018MonitorSts::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();

   LOG(info) << "Nr. of Overlap MS:     " << fuOverlapMsNb;

   fuNrOfDpbs       = fUnpackPar->GetNrOfDpbs();
   fuNbElinksPerDpb = fUnpackPar->GetNbElinksPerDpb();
   fuNbStsXyters    = fUnpackPar->GetNbStsXyters();
   fuNbChanPerAsic  = fUnpackPar->GetNbChanPerAsic();


   LOG(info) << "Nr. of STS DPBs:       " << fuNrOfDpbs;

   fDpbIdIndexMap.clear();
   fvuElinkToAsic.resize( fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fDpbIdIndexMap[ fUnpackPar->GetDpbId( uDpb )  ] = uDpb;
      LOG(info) << "Eq. ID for DPB #" << std::setw(2) << uDpb << " = "
                << std::setw(4) << std::hex << fUnpackPar->GetDpbId( uDpb )
                << std::dec
                << " => " << fDpbIdIndexMap[ fUnpackPar->GetDpbId( uDpb )  ];

      fvuElinkToAsic[uDpb].resize( fuNbElinksPerDpb );
      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
         fvuElinkToAsic[uDpb][uElink] = fUnpackPar->GetElinkToAsicIdx( uDpb * fuNbElinksPerDpb + uElink );
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

   LOG(info) << "Counter size for TS:     " << stsxyter::kuHitNbTsBins;
   LOG(info) << "Counter size for TS_MSB: " << stsxyter::kuTsMsbNbTsBins;
   LOG(info) << "Counter size for cycles: " << stsxyter::kulTsCycleNbBins;

   LOG(info) << "ASIC Idx for STS 1 N: " << fUnpackPar->GetAsicIndexSts1N();
   LOG(info) << "ASIC Idx for STS 1 P: " << fUnpackPar->GetAsicIndexSts1P();
   LOG(info) << "ASIC Idx for STS 2 N: " << fUnpackPar->GetAsicIndexSts2N();
   LOG(info) << "ASIC Idx for STS 2 P: " << fUnpackPar->GetAsicIndexSts2P();

   // Internal status initialization
   fvulCurrentTsMsb.resize( fuNrOfDpbs );
   fvuCurrentTsMsbCycle.resize( fuNrOfDpbs );
   fvuElinkLastTsHit.resize( fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fvulCurrentTsMsb[uDpb]     = 0;
      fvuCurrentTsMsbCycle[uDpb] = 0;
      fvuElinkLastTsHit[uDpb]    = 0;
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   fvulChanLastHitTime.resize( fuNbStsXyters );
   fvdChanLastHitTime.resize( fuNbStsXyters );
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
   LOG(info) << "CbmCosy2018MonitorSts::ReInitContainers => Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
   LOG(info) << "CbmCosy2018MonitorSts::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
   LOG(info) << "CbmCosy2018MonitorSts::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuMaxNbMicroslices;
   return kTRUE;
}

void CbmCosy2018MonitorSts::CreateHistograms()
{

   TString sHistName{""};
   TString title{""};

   sHistName = "hStsMessageType";
   title = "Nb of message for each type; Type";
   fhStsMessType = new TH1I(sHistName, title, 5, 0., 5.);
   fhStsMessType->GetXaxis()->SetBinLabel( 1, "Dummy");
   fhStsMessType->GetXaxis()->SetBinLabel( 2, "Hit");
   fhStsMessType->GetXaxis()->SetBinLabel( 3, "TsMsb");
   fhStsMessType->GetXaxis()->SetBinLabel( 4, "Epoch");
   fhStsMessType->GetXaxis()->SetBinLabel( 5, "Empty");
/* *** Missing int + MessType OP!!!! ****
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhStsMessType->GetXaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

   sHistName = "hStsSysMessType";
   title = "Nb of system message for each type; System Type";
   fhStsSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
/*
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/

   sHistName = "hStsMessageTypePerDpb";
   title = "Nb of message of each type for each DPB; DPB; Type";
   fhStsMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 5, 0., 5.);
   fhStsMessTypePerDpb->GetYaxis()->SetBinLabel( 1, "Dummy");
   fhStsMessTypePerDpb->GetYaxis()->SetBinLabel( 2, "Hit");
   fhStsMessTypePerDpb->GetYaxis()->SetBinLabel( 3, "TsMsb");
   fhStsMessTypePerDpb->GetYaxis()->SetBinLabel( 4, "Epoch");
   fhStsMessTypePerDpb->GetYaxis()->SetBinLabel( 5, "Empty");
/* *** Missing int + MessType OP!!!! ****
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhStsMessType->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

   sHistName = "hStsSysMessTypePerDpb";
   title = "Nb of system message of each type for each DPB; DPB; System Type";
   fhStsSysMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);
/*
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/

   sHistName = "hStsDpbRawTsMsb";
   title = "Raw TsMsb distribution for each DPB; DPB; TS MSB [Bins]";
   fhStsDpbRawTsMsb = new TH2I( sHistName, title,
                                fuNrOfDpbs, 0, fuNrOfDpbs,
                                stsxyter::kuTsMsbNbTsBins, 0., stsxyter::kuTsMsbNbTsBins);

   sHistName = "hStsDpbRawTsMsbSx";
   title = "Raw TsMsb distribution for each DPB; DPB; TS MSB [Bins]";
   fhStsDpbRawTsMsbSx = new TH2I( sHistName, title,
                                  fuNrOfDpbs, 0, fuNrOfDpbs,
                                  0x1F , 0., 0x1F );

   sHistName = "hStsDpbRawTsMsbDpb";
   title = "Raw TsMsb distribution for each DPB; DPB; TS MSB [Bins]";
   fhStsDpbRawTsMsbDpb = new TH2I( sHistName, title,
                                   fuNrOfDpbs, 0, fuNrOfDpbs,
                                   stsxyter::kuTsMsbNbTsBins >> 5, 0., stsxyter::kuTsMsbNbTsBins >> 5 );

   sHistName = "hStsMessageTypePerElink";
   title = "Nb of message of each type for each eLink; eLink; Type";
   fhStsMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 5, 0., 5.);
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel( 1,       "Dummy");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel( 2,         "Hit");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel( 3,       "TsMsb");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel( 4, "ReadDataAck");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel( 5,         "Ack");
/* *** Missing int + MessType OP!!!! ****
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Dummy,       "Dummy");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Hit,         "Hit");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::TsMsb,       "TsMsb");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::ReadDataAck, "ReadDataAck");
   fhStsMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::MessType::Ack,         "Ack");
*/

   sHistName = "hStsSysMessTypePerElink";
   title = "Nb of system message of each type for each eLink; eLink; System Type";
   fhStsSysMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 17, 0., 17.);
/*
   fhStsSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   fhStsSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhStsSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
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

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      // Channel counts
      sHistName = Form( "hStsChanCounts_%03u", uXyterIdx );
      title = Form( "Hits Count per channel, StsXyter #%03u; Channel; Hits []", uXyterIdx );
      fhStsChanCounts.push_back( new TH1I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Raw Adc Distribution
      sHistName = Form( "hStsChanRawAdc_%03u", uXyterIdx );
      title = Form( "Raw Adc distribution per channel, StsXyter #%03u; Channel []; Adc []; Hits []", uXyterIdx );
      fhStsChanRawAdc.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 ) );

      // Raw Adc Distribution profile
      sHistName = Form( "hStsChanRawAdcProfc_%03u", uXyterIdx );
      title = Form( "Raw Adc prodile per channel, StsXyter #%03u; Channel []; Adc []", uXyterIdx );
      fhStsChanRawAdcProf.push_back( new TProfile(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Raw Ts Distribution
      sHistName = Form( "hStsChanRawTs_%03u", uXyterIdx );
      title = Form( "Raw Timestamp distribution per channel, StsXyter #%03u; Channel []; Ts []; Hits []", uXyterIdx );
      fhStsChanRawTs.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins -0.5 ) );

      // Missed event flag
      sHistName = Form( "hStsChanMissEvt_%03u", uXyterIdx );
      title = Form( "Missed Event flags per channel, StsXyter #%03u; Channel []; Miss Evt []; Hits []", uXyterIdx );
      fhStsChanMissEvt.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 2, -0.5, 1.5 ) );

      // Hit rates evo per channel
      sHistName = Form( "hStsChanRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [s]; Channel []; Hits []", uXyterIdx );
      fhStsChanHitRateEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Hit rates evo per StsXyter
      sHistName = Form( "hStsXyterRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [s]; Hits []", uXyterIdx );
      fhStsXyterRateEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );

      // Hit rates evo per channel, 1 minute bins, 24h
      sHistName = Form( "hStsChanRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [min]; Channel []; Hits []", uXyterIdx );
      fhStsChanHitRateEvoLong.push_back( new TH2D( sHistName, title,
                                                1440, 0, 1440,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      // Hit rates evo per StsXyter, 1 minute bins, 24h
      sHistName = Form( "hStsXyterRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [min]; Hits []", uXyterIdx );
      fhStsXyterRateEvoLong.push_back( new TH1D(sHistName, title, 1440, 0, 1440 ) );

      // Hit distance in time for each channel
      if( fbChanHitDtEna )
      {
         sHistName = Form( "hStsChanHitDt_%03u", uXyterIdx );
         title = Form( "Time diff between hits on same channel in StsXyter #%03u; t_hit - t_prev [ns]; Channel []; Hits []", uXyterIdx );
         fhStsChanHitDt.push_back( new TH2I( sHistName, title,
                                                   iNbBinsRate - 1, dBinsRate,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

         sHistName = Form( "hStsChanHitDtNeg_%03u", uXyterIdx );
         title = Form( "Time diff between hits on same channel in StsXyter #%03u; t_prev - t_hit [ns]; Channel []; Hits []", uXyterIdx );
         fhStsChanHitDtNeg.push_back( new TH2I( sHistName, title,
                                                   iNbBinsRate - 1, dBinsRate,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      } // if( fbChanHitDtEna )

      sHistName = Form( "hStsChanHitsPerMs_%03u", uXyterIdx );
      title = Form( "Nb of hits per channel in each MS in StsXyter #%03u; Nb Hits in MS []; Channel []; MS []", uXyterIdx );
      fhStsChanHitsPerMs.push_back( new TH2I( sHistName, title,
                                                100, -0.5, 99.5,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      if( kTRUE == fbLongHistoEnable )
      {
         UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
         fuLongHistoBinNb = uAlignedLimit / fuLongHistoBinSizeSec;

         sHistName = Form( "hFebRateEvoLong_%03u", uXyterIdx );
         title = Form( "Mean rate VS run time in same MS in StsXyter #%03u; Time in run [s]; Rate [1/s]", uXyterIdx );
         fhFebRateEvoLong.push_back( new TH1D( sHistName, title,
                                                   fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5) );

         sHistName = Form( "hFebChRateEvoLong_%03u", uXyterIdx );
         title = Form( "Mean rate per channel VS run time in StsXyter #%03u; Time in run [s]; Channel []; Rare [1/s]", uXyterIdx );
         fhFebChRateEvoLong.push_back( new TH2D( sHistName, title,
                                                   fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      } // if( kTRUE == fbLongHistoEnable )

   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   UInt_t uNbBinEvo = 32768 + 1;
   Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

   sHistName = "fhStsSortedDtN1P1";
   title =  "Time diff for hits Sts 1 N and Sts 1 P; tP1 - tN1 [ns]; Counts";
   fhStsSortedDtN1P1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

   if( kTRUE == fbDualStsEna )
   {
      sHistName = "fhStsSortedDtN2P2";
      title =  "Time diff for hits Sts 2 N and Sts 2 P; tP2 - tN2 [ns]; Counts";
      fhStsSortedDtN2P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

      sHistName = "fhStsSortedDtN1N2";
      title =  "Time diff for hits Sts 1 N and Sts 2 N; tN2 - tN1 [ns]; Counts";
      fhStsSortedDtN1N2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

      sHistName = "fhStsSortedDtP1P2";
      title =  "Time diff for hits Sts 1 P and Sts 2 P; tP2 - tP1 [ns]; Counts";
      fhStsSortedDtP1P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

      sHistName = "fhStsSortedDtN1P2";
      title =  "Time diff for hits Sts 1 N and Sts 2 P; tP2 - tN1 [ns]; Counts";
      fhStsSortedDtN1P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

      sHistName = "fhStsSortedDtP1N2";
      title =  "Time diff for hits Sts 1 P and Sts 2 N; tP2 - tP1 [ns]; Counts";
      fhStsSortedDtP1N2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   } // if( kTRUE == fbDualStsEna )

   sHistName = "fhStsSortedMapN1P1";
   title = "Sorted hits in coincidence for Sts 1 axis N and P; N channel Sts 1 []; P channel Sts 1 []; Hits []";
   fhStsSortedMapN1P1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                  fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

   if( kTRUE == fbDualStsEna )
   {
      sHistName = "fhStsSortedMapN2P2";
      title = "Sorted hits in coincidence for Sts 2 axis N and P; N channel Sts 2 []; P channel Sts 2 []; Hits []";
      fhStsSortedMapN2P2 = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

      sHistName = "fhStsSortedMapN1N2";
      title = "Sorted hits in coincidence for Sts 1 axis N and 2 axis N; N channel Sts 1 []; N channel Sts 2 []; Hits []";
      fhStsSortedMapN1N2 = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

      sHistName = "fhStsSortedMapP1P2";
      title = "Sorted hits in coincidence for Sts 1 axis P and 2 axis P; P channel Sts 1 []; P channel Sts 2 []; Hits []";
      fhStsSortedMapP1P2 = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

      sHistName = "fhStsSortedMapN1P2";
      title = "Sorted hits in coincidence for Sts 1 axis N and 2 axis P; N channel Sts 1 []; P channel Sts 2 []; Hits []";
      fhStsSortedMapN1P2 = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

      sHistName = "fhStsSortedMapP1N2";
      title = "Sorted hits in coincidence for Sts 1 axis P and 2 axis N; P channel Sts 1 []; N channel Sts 2 []; Hits []";
      fhStsSortedMapP1N2 = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );
   } // if( kTRUE == fbDualStsEna )

   // Coincidence counts evolution between some axis of the sensors
   sHistName = "hStsSortedCntEvoN1P1";
   title = "Nb of coincidences in both N1 and P1 per s; Time [s]; N1-P1 coincidences []";
   fhStsSortedCntEvoN1P1 = new TH1I(sHistName, title, 1800, 0, 1800 );

   if( kTRUE == fbDualStsEna )
   {
      sHistName = "hStsSortedCntEvoN2P2";
      title = "Nb of coincidences in both N2 and P2 per s; Time [s]; N2-P2 coincidences []";
      fhStsSortedCntEvoN2P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

      sHistName = "fhStsSortedCntEvoN1N2";
      title = "Nb of coincidences in both N1 and N2 per s; Time [s]; N1-N2 coincidences []";
      fhStsSortedCntEvoN1N2 = new TH1I(sHistName, title, 1800, 0, 1800 );

      sHistName = "fhStsSortedCntEvoP1P2";
      title = "Nb of coincidences in both P1 and P2 per s; Time [s]; P1-P2 coincidences []";
      fhStsSortedCntEvoP1P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

      sHistName = "hStsSortedCntEvoN1P2";
      title = "Nb of coincidences in both N1 and P2 per s; Time [s]; N1-P2 coincidences []";
      fhStsSortedCntEvoN1P2 = new TH1I(sHistName, title, 1800, 0, 1800 );

      sHistName = "hStsSortedCntEvoP1N2";
      title = "Nb of coincidences in both P1 and N2 per s; Time [s]; P1-N2 coincidences []";
      fhStsSortedCntEvoP1N2 = new TH1I(sHistName, title, 1800, 0, 1800 );

      /// STS1 STS2 correlations ---------------------------------------
      sHistName = "fhStsSortedDtN1P1N2P2";
      title =  "Time diff for hits Sts 1 <N,P> and Sts 2 <N,P>; t<N2,P2> - t<N1,P1> [ns]; Counts";
      fhStsSortedDtN1P1N2P2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

      sHistName = "fhStsSortedMapN1P1Coinc";
      title = "Sorted hits in coincidence for Sts 1 axis N and P if Sts2 in coinc; N channel Sts 1 []; P channel Sts 1 []; Hits []";
      fhStsSortedMapN1P1Coinc = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

      sHistName = "fhStsSortedMapN2P2Coinc";
      title = "Sorted hits in coincidence for Sts 2 axis N and P if Sts1 in coinc; N channel Sts 2 []; P channel Sts 2 []; Hits []";
      fhStsSortedMapN2P2Coinc = new TH2I( sHistName, title,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                     fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 );

      UInt_t uNbBinDtDual = 250;
      Double_t dMaxDtDual = 250;
      UInt_t uNbBinChanDiffDual = 2*fuNbChanPerAsic + 1;
      Double_t dMaxChanDiffDual = fuNbChanPerAsic + 0.5;

      sHistName = "fhStsSortedDtVsDiffN";
      title = "Channel diff in N vs Time diff between STS 1 and STS 2; t<N2,P2> - t<N1,P1> [ns]; N2 - N1 []; Hits []";
      fhStsSortedDtVsDiffN = new TH2I( sHistName, title,
                                       uNbBinDtDual, -dMaxDtDual, dMaxDtDual,
                                       uNbBinChanDiffDual, -dMaxChanDiffDual, dMaxChanDiffDual );

      sHistName = "fhStsSortedDtVsDiffP";
      title = "Channel diff in P vs Time diff between STS 1 and STS 2; t<N2,P2> - t<N1,P1> [ns]; P2 - P1 []; Hits []";
      fhStsSortedDtVsDiffP = new TH2D( sHistName, title,
                                       uNbBinDtDual, -dMaxDtDual, dMaxDtDual,
                                       uNbBinChanDiffDual, -dMaxChanDiffDual, dMaxChanDiffDual );
   } // if( kTRUE == fbDualStsEna )

   Double_t dMapSizeMmX = kdSensorsSzX + 10;
   Double_t dMapSizeMmY = kdSensorsSzY + 10;
   Int_t iNbBinsX = dMapSizeMmX / kdPitchMm; // 58 um bin
   Int_t iNbBinsY = dMapSizeMmY / kdPitchMm / 8; // 116 um bin
   sHistName = "fhStsSortedMapX1Y1";
   title = "Sorted hits in coincidence for Sts 1 axis X and Y; X1 [mm]; Y1 [mm]; Hits []";
   fhStsSortedMapX1Y1 = new TH2I( sHistName, title,
                                  iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                  iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

   if( kTRUE == fbDualStsEna )
   {
      sHistName = "fhStsSortedMapX2Y2";
      title = "Sorted hits in coincidence for Sts 2 axis X and Y; X2 [mm]; Y2 [mm]; Hits []";
      fhStsSortedMapX2Y2 = new TH2I( sHistName, title,
                                     iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                     iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

      sHistName = "fhStsSortedMapX1Y1Coinc";
      title = "Sorted hits in coincidence for Sts 1 axis X and Y if Sts2 in coinc; X1 [mm]; Y1 [mm]; Hits []";
      fhStsSortedMapX1Y1Coinc = new TH2I( sHistName, title,
                                     iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                     iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );

      sHistName = "fhStsSortedMapX2Y2Coinc";
      title = "Sorted hits in coincidence for Sts 2 axis X and Y if Sts1 in coinc; X2 [mm]; Y2 [mm]; Hits []";
      fhStsSortedMapX2Y2Coinc = new TH2I( sHistName, title,
                                     iNbBinsX, -dMapSizeMmX / 2.0, dMapSizeMmX / 2.0,
                                     iNbBinsY, -dMapSizeMmY / 2.0, dMapSizeMmY / 2.0 );
   } // if( kTRUE == fbDualStsEna )

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   // Distribution of the TS_MSB per StsXyter
   sHistName = "hStsAsicTsMsb";
   title = "Raw Timestamp Msb distribution per StsXyter; Ts MSB []; StsXyter []; Hits []";
   fhStsAsicTsMsb = new TH2I( sHistName, title, stsxyter::kuTsMsbNbTsBins, -0.5,   stsxyter::kuTsMsbNbTsBins - 0.5,
                                                fuNbStsXyters, -0.5, fuNbStsXyters - 0.5 );

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
      server->Register("/StsRaw", fhStsMessType );
      server->Register("/StsRaw", fhStsSysMessType );
      server->Register("/StsRaw", fhStsMessTypePerDpb );
      server->Register("/StsRaw", fhStsSysMessTypePerDpb );
      server->Register("/StsRaw", fhStsDpbRawTsMsb );
      server->Register("/StsRaw", fhStsDpbRawTsMsbSx );
      server->Register("/StsRaw", fhStsDpbRawTsMsbDpb );
      server->Register("/StsRaw", fhStsMessTypePerElink );
      server->Register("/StsRaw", fhStsSysMessTypePerElink );
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {
         server->Register("/StsRaw", fhStsChanCounts[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsChanRawAdc[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsChanRawAdcProf[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsChanRawTs[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsChanMissEvt[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsChanHitRateEvo[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsXyterRateEvo[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsChanHitRateEvoLong[ uXyterIdx ] );
         server->Register("/StsRaw", fhStsXyterRateEvoLong[ uXyterIdx ] );
         if( fbChanHitDtEna )
         {
            server->Register("/StsRaw", fhStsChanHitDt[ uXyterIdx ] );
            server->Register("/StsRaw", fhStsChanHitDtNeg[ uXyterIdx ] );
         } // if( fbChanHitDtEna )
         server->Register("/StsRaw", fhStsChanHitsPerMs[ uXyterIdx ] );
         if( kTRUE == fbLongHistoEnable )
         {
            server->Register("/StsRaw", fhFebRateEvoLong[ uXyterIdx ] );
            server->Register("/StsRaw", fhFebChRateEvoLong[ uXyterIdx ] );
         } // if( kTRUE == fbLongHistoEnable )
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

      server->Register("/StsRaw", fhStsSortedDtN1P1 );
      if( kTRUE == fbDualStsEna )
      {
         server->Register("/StsRaw", fhStsSortedDtN2P2 );
         server->Register("/StsRaw", fhStsSortedDtN1N2 );
         server->Register("/StsRaw", fhStsSortedDtP1P2 );
         server->Register("/StsRaw", fhStsSortedDtN1P2 );
         server->Register("/StsRaw", fhStsSortedDtP1N2 );
      } // if( kTRUE == fbDualStsEna )

      server->Register("/StsRaw", fhStsSortedMapN1P1 );
      if( kTRUE == fbDualStsEna )
      {
         server->Register("/StsRaw", fhStsSortedMapN2P2 );
         server->Register("/StsRaw", fhStsSortedMapN1N2 );
         server->Register("/StsRaw", fhStsSortedMapP1P2 );
         server->Register("/StsRaw", fhStsSortedMapN1P2 );
         server->Register("/StsRaw", fhStsSortedMapP1N2 );
      } // if( kTRUE == fbDualStsEna )

      server->Register("/StsRaw", fhStsSortedCntEvoN1P1 );
      if( kTRUE == fbDualStsEna )
      {
         server->Register("/StsRaw", fhStsSortedCntEvoN2P2 );
         server->Register("/StsRaw", fhStsSortedCntEvoN1N2 );
         server->Register("/StsRaw", fhStsSortedCntEvoP1P2 );
         server->Register("/StsRaw", fhStsSortedCntEvoN1P2 );
         server->Register("/StsRaw", fhStsSortedCntEvoP1N2 );

         server->Register("/StsDual", fhStsSortedDtN1P1N2P2 );
         server->Register("/StsDual", fhStsSortedMapN1P1Coinc );
         server->Register("/StsDual", fhStsSortedMapN2P2Coinc );
         server->Register("/StsDual", fhStsSortedDtVsDiffN );
         server->Register("/StsDual", fhStsSortedDtVsDiffP );
      } // if( kTRUE == fbDualStsEna )

      server->Register("/StsRaw", fhStsSortedMapX1Y1 );
      if( kTRUE == fbDualStsEna )
      {
         server->Register("/StsRaw", fhStsSortedMapX2Y2 );
         server->Register("/StsDual", fhStsSortedMapX1Y1Coinc );
         server->Register("/StsDual", fhStsSortedMapX2Y2Coinc );
      } // if( kTRUE == fbDualStsEna )


      server->Register("/StsRaw", fhStsAsicTsMsb );
      server->RegisterCommand("/Reset_All_Sts", "bCosy2018ResetStsHistos=kTRUE");
      server->RegisterCommand("/Write_All_Sts", "bCosy2018WriteStsHistos=kTRUE");

      server->Restrict("/Reset_All_Sts", "allow=admin");
      server->Restrict("/Write_All_Sts", "allow=admin");
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
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fhStsChanCounts[ uXyterIdx ]->Draw();

      cStsSumm->cd(2);
      gPad->SetLogz();
      fhStsChanRawAdc[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsChanHitRateEvo[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(4);
      gPad->SetGridy();
//      gPad->SetLogy();
      fhStsChanRawAdcProf[ uXyterIdx ]->Draw();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

      // Long duration rate monitoring
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


//====================================================================//
   TCanvas* cStsMaps = new TCanvas( "cStsMaps",
                                    "Sts coincidence maps",
                                    w, h);
   if( kTRUE == fbDualStsEna )
   {
      cStsMaps->Divide( 2, 3 );
   } // if( kTRUE == fbDualStsEna )

   cStsMaps->cd(1);
   gPad->SetLogz();
   fhStsSortedMapN1P1->Draw( "colz" );

   if( kTRUE == fbDualStsEna )
   {
      cStsMaps->cd(2);
      gPad->SetLogz();
      fhStsSortedMapN2P2->Draw( "colz" );

      cStsMaps->cd(3);
      gPad->SetLogz();
      fhStsSortedMapN1N2->Draw( "colz" );

      cStsMaps->cd(4);
      gPad->SetLogz();
      fhStsSortedMapP1P2->Draw( "colz" );

      cStsMaps->cd(5);
      gPad->SetLogz();
      fhStsSortedMapN1P2->Draw( "colz" );

      cStsMaps->cd(6);
      gPad->SetLogz();
      fhStsSortedMapP1N2->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   TCanvas* cStsCoincEvo = new TCanvas( "cStsCoincEvo",
                                    "Sts coincidence rate evolution",
                                    w, h);
   if( kTRUE == fbDualStsEna )
   {
      cStsCoincEvo->Divide( 2, 3 );
   } // if( kTRUE == fbDualStsEna )

   cStsCoincEvo->cd(1);
   gPad->SetLogz();
   fhStsSortedCntEvoN1P1->Draw( "colz" );

   if( kTRUE == fbDualStsEna )
   {
      cStsCoincEvo->cd(2);
      gPad->SetLogz();
      fhStsSortedCntEvoN2P2->Draw( "colz" );

      cStsCoincEvo->cd(3);
      gPad->SetLogz();
      fhStsSortedCntEvoN1N2->Draw( "colz" );

      cStsCoincEvo->cd(4);
      gPad->SetLogz();
      fhStsSortedCntEvoP1P2->Draw( "colz" );

      cStsCoincEvo->cd(5);
      gPad->SetLogz();
      fhStsSortedCntEvoN1P2->Draw( "colz" );

      cStsCoincEvo->cd(6);
      gPad->SetLogz();
      fhStsSortedCntEvoP1N2->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
//====================================================================//

//====================================================================//
   if( kTRUE == fbDualStsEna )
   {
      TCanvas* cStsDualDt = new TCanvas( "cStsDualDt",
                                 "Sts dual sensor time diffs",
                                 w, h);
      cStsDualDt->Divide( 2, 2 );

      cStsDualDt->cd(1);
      gPad->SetGridx();
      gPad->SetLogy();
      fhStsSortedDtN1P1N2P2->Draw( "" );

      cStsDualDt->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedDtVsDiffN->Draw( "colz" );

      cStsDualDt->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedDtVsDiffP->Draw( "colz" );

   } // if( kTRUE == fbDualStsEna )
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
   fhStsSortedMapX1Y1->Draw( "colz" );

   if( kTRUE == fbDualStsEna )
   {
      cStsMapXY->cd(2);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedMapX2Y2->Draw( "colz" );

      cStsMapXY->cd(3);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedMapX1Y1Coinc->Draw( "colz" );

      cStsMapXY->cd(4);
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fhStsSortedMapX2Y2Coinc->Draw( "colz" );
   } // if( kTRUE == fbDualStsEna )
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

Bool_t CbmCosy2018MonitorSts::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bCosy2018ResetStsHistos )
   {
      ResetAllHistos();
      bCosy2018ResetStsHistos = kFALSE;
   } // if( bCosy2018ResetStsHistos )
   if( bCosy2018WriteStsHistos )
   {
      SaveAllHistos( "data/StsHistos.root" );
      bCosy2018WriteStsHistos = kFALSE;
   } // if( bCosy2018WriteStsHistos )

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
      LOG(info) << "CbmCosy2018MonitorSts::DoUnpack => Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
      LOG(info) << "CbmCosy2018MonitorSts::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
      LOG(info) << "CbmCosy2018MonitorSts::DoUnpack =>  Changed fvuChanNbHitsInMs size "
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
      LOG(debug) << "Microslice: " << fulCurrentMsIdx
                 << " from EqId " << std::hex << fuCurrentEquipmentId << std::dec
                 << " has size: " << size;

      fuCurrDpbId  = static_cast< uint32_t >( fuCurrentEquipmentId & 0xFFFF );
      fuCurrDpbIdx = fDpbIdIndexMap[ fuCurrDpbId ];

      if( component < kiMaxNbFlibLinks )
      {
         if( fdStartTimeMsSz < 0 )
            fdStartTimeMsSz = (1e-9) * static_cast<double>(fulCurrentMsIdx);
         fhMsSz[ component ]->Fill( size );
         fhMsSzTime[ component ]->Fill( (1e-9) * static_cast<double>( fulCurrentMsIdx) - fdStartTimeMsSz, size);
      } // if( component < kiMaxNbFlibLinks )

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
         fhStsMessType->Fill( static_cast< uint16_t > (typeMess) );
         fhStsMessTypePerDpb->Fill( fuCurrDpbIdx, static_cast< uint16_t > (typeMess) );

         switch( typeMess )
         {
            case stsxyter::MessType::Hit :
            {
               // Extract the eLink and Asic indices => Should GO IN the fill method now that obly hits are link/asic specific!
               UShort_t usElinkIdx = mess.GetLinkIndex();
               if( fuNbElinksPerDpb <= usElinkIdx )
               {
                  LOG(fatal) << "CbmCosy2018MonitorSts::DoUnpack => "
                             << "eLink index out of bounds!"
                             << usElinkIdx << " VS " << fuNbElinksPerDpb;
               } // if( fuNbElinksPerDpb <= usElinkIdx )
               fhStsMessTypePerElink->Fill( fuCurrDpbIdx * fuNbElinksPerDpb + usElinkIdx,
                                            static_cast< uint16_t > (typeMess) );

               UInt_t   uAsicIdx   = fvuElinkToAsic[fuCurrDpbIdx][usElinkIdx];
               if( ! ( fUnpackPar->GetAsicIndexSts1N() == uAsicIdx ||
                       fUnpackPar->GetAsicIndexSts1P() == uAsicIdx ) )
               {
                  if( kFALSE == fbDualStsEna )
                     continue;

                  if( ! ( fUnpackPar->GetAsicIndexSts2N() == uAsicIdx ||
                          fUnpackPar->GetAsicIndexSts2P() == uAsicIdx ) )
                     continue;
               } // If neither P nor n side of STS sensor 1

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
                  LOG(info) << "CbmCosy2018MonitorSts::DoUnpack => "
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
               LOG(fatal) << "CbmCosy2018MonitorSts::DoUnpack => "
                          << "Unknown message type, should never happen, stopping here!";
            }
         } // switch( mess.GetMessType() )

      } // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )
/*
      LOG(info) << " Finished MS " << std::setw( 12 ) << fulCurrentMsIdx
                << " for DPB " << std::setw( 2 ) << fuCurrDpbIdx;
*/
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


      // Time differences plotting using the fully time sorted hits
      Double_t dLastTimeN1 = -1e12;
      Double_t dLastTimeP1 = -1e12;
      Double_t dLastTimeN2 = -1e12;
      Double_t dLastTimeP2 = -1e12;
      if( 0 < fvmHitsInTs.size() )
      {
         ULong64_t ulLastHitTime = ( *( fvmHitsInTs.rbegin() ) ).GetTs();
         std::multiset< stsxyter::FinalHit >::iterator it;

//         std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
//         Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();


         for( it  = fvmHitsInTs.begin();
              it != fvmHitsInTs.end() && (*it).GetTs() < ulLastHitTime - 320; // 32 * 3.125 ns = 1000 ns
              ++it )
         {
            UShort_t usAsicIdx = (*it).GetAsic();
            UShort_t usChanIdx = (*it).GetChan();
            ULong64_t ulHitTs  = (*it).GetTs();

            Double_t dTimeSinceStartSec = (ulHitTs * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;

            if( fUnpackPar->GetAsicIndexSts1N() == usAsicIdx )
            {
               fLastSortedHit1N = (*it);
               dLastTimeN1      = ulHitTs;

               Double_t dDtN1P1 = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
               fhStsSortedDtN1P1->Fill( dDtN1P1 );
               if( TMath::Abs( dDtN1P1 ) < fdCoincBorder )
               {
                  fhStsSortedMapN1P1->Fill( usChanIdx, fLastSortedHit1P.GetChan() );
                  fhStsSortedCntEvoN1P1->Fill( dTimeSinceStartSec );

                  Double_t dX, dY;
                  ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX, dY );
                  fhStsSortedMapX1Y1->Fill( dX, dY );
               } // if( TMath::Abs( dDtN1P1 ) < fdCoincBorder )

               if( kTRUE == fbDualStsEna )
               {
                  Double_t dDtN1N2 = ( dLastTimeN2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1P2 = ( dLastTimeP2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtP1P2 = ( dLastTimeP2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtP1N2 = ( dLastTimeN2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtN2P2 = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1P1N2P2 = ( dLastTimeN2 + dLastTimeP2 - ( dLastTimeN1 + dLastTimeP1 ) ) * stsxyter::kdClockCycleNs / 2.0;

                  fhStsSortedDtN1N2->Fill( dDtN1N2 );
                  fhStsSortedDtN1P2->Fill( dDtN1P2 );
                  fhStsSortedDtN1P1N2P2->Fill( dDtN1P1N2P2 );
                  Int_t iChanDiffN =  static_cast< Int_t >( fLastSortedHit2N.GetChan() )
                                    - static_cast< Int_t >( fLastSortedHit1N.GetChan() );
                  fhStsSortedDtVsDiffN->Fill( dDtN1P1N2P2, iChanDiffN );

                  if( TMath::Abs( dDtN1N2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapN1N2->Fill( usChanIdx, fLastSortedHit2N.GetChan() );
                     fhStsSortedCntEvoN1N2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtN1N2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapN1P2->Fill( usChanIdx, fLastSortedHit2P.GetChan() );
                     fhStsSortedCntEvoN1P2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )

                  if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder )
                  {
                     fhStsSortedMapN1P1Coinc->Fill( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan() );
                     fhStsSortedMapN2P2Coinc->Fill( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan() );

                     Double_t dX1, dY1, dX2, dY2;
                     ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapX1Y1Coinc->Fill( dX1, dY1 );
                     fhStsSortedMapX2Y2Coinc->Fill( dX2, dY2 );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
               } // if( kTRUE == fbDualStsEna )
            } // if( fUnpackPar->GetAsicIndexSts1N() == usAsicIdx )
            else if( fUnpackPar->GetAsicIndexSts1P() == usAsicIdx )
            {
               fLastSortedHit1P = (*it);
               dLastTimeP1      = ulHitTs;

               Double_t dDtN1P1 = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
               fhStsSortedDtN1P1->Fill( dDtN1P1 );
               if( TMath::Abs( dDtN1P1 ) < fdCoincBorder )
               {
                  fhStsSortedMapN1P1->Fill( fLastSortedHit1N.GetChan(), usChanIdx );
                  fhStsSortedCntEvoN1P1->Fill( dTimeSinceStartSec );

                  Double_t dX, dY;
                  ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX, dY );
                  fhStsSortedMapX1Y1->Fill( dX, dY );
               }

               if( kTRUE == fbDualStsEna )
               {
//                  Double_t dDtN1N2 = ( dLastTimeN2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtN1P2 = ( dLastTimeP2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtP1P2 = ( dLastTimeP2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtP1N2 = ( dLastTimeN2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtN2P2 = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1P1N2P2 = ( dLastTimeN2 + dLastTimeP2 - ( dLastTimeN1 + dLastTimeP1 ) ) * stsxyter::kdClockCycleNs / 2.0;

                  fhStsSortedDtP1P2->Fill( dDtP1P2 );
                  fhStsSortedDtP1N2->Fill( dDtP1N2 );
                  fhStsSortedDtN1P1N2P2->Fill( dDtN1P1N2P2 );
                  Int_t iChanDiffP =  static_cast< Int_t >( fLastSortedHit2P.GetChan() )
                                    - static_cast< Int_t >( fLastSortedHit1P.GetChan() );
                  fhStsSortedDtVsDiffP->Fill( dDtN1P1N2P2, iChanDiffP );

                  if( TMath::Abs( dDtP1P2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapP1P2->Fill( usChanIdx, fLastSortedHit2P.GetChan() );
                     fhStsSortedCntEvoP1P2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtP1P2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtP1N2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapP1N2->Fill( usChanIdx, fLastSortedHit2N.GetChan() );
                     fhStsSortedCntEvoP1N2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtP1N2 ) < fdCoincBorder )

                  if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder )
                  {
                     fhStsSortedMapN1P1Coinc->Fill( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan() );
                     fhStsSortedMapN2P2Coinc->Fill( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan() );

                     Double_t dX1, dY1, dX2, dY2;
                     ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapX1Y1Coinc->Fill( dX1, dY1 );
                     fhStsSortedMapX2Y2Coinc->Fill( dX2, dY2 );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
               } // if( kTRUE == fbDualStsEna )
            } // else if( fUnpackPar->GetAsicIndexSts1P() == usAsicIdx )
            else if( kTRUE == fbDualStsEna )
            {
               if( fUnpackPar->GetAsicIndexSts2N() == usAsicIdx )
               {
                  fLastSortedHit2N = (*it);
                  dLastTimeN2      = ulHitTs;

//                  Double_t dDtN1P1 = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1N2 = ( dLastTimeN2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtN1P2 = ( dLastTimeP2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtP1P2 = ( dLastTimeP2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtP1N2 = ( dLastTimeN2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN2P2 = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1P1N2P2 = ( dLastTimeN2 + dLastTimeP2 - ( dLastTimeN1 + dLastTimeP1 ) ) * stsxyter::kdClockCycleNs / 2.0;

                  fhStsSortedDtN2P2->Fill( dDtN2P2 );
                  fhStsSortedDtN1N2->Fill( dDtN1N2 );
                  fhStsSortedDtP1N2->Fill( dDtP1N2 );
                  fhStsSortedDtN1P1N2P2->Fill( dDtN1P1N2P2 );
                  Int_t iChanDiffN =  static_cast< Int_t >( fLastSortedHit2N.GetChan() )
                                    - static_cast< Int_t >( fLastSortedHit1N.GetChan() );
                  fhStsSortedDtVsDiffN->Fill( dDtN1P1N2P2, iChanDiffN );

                  if( TMath::Abs( dDtN2P2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapN2P2->Fill( usChanIdx, fLastSortedHit2P.GetChan() );
                     fhStsSortedCntEvoN2P2->Fill( dTimeSinceStartSec );

                     Double_t dX, dY;
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX, dY );
                     fhStsSortedMapX2Y2->Fill( dX, dY );
                  } // if( TMath::Abs( dDtN2P2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtN1N2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapN1N2->Fill( fLastSortedHit1N.GetChan(), usChanIdx );
                     fhStsSortedCntEvoN1N2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtN1N2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtP1N2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapP1N2->Fill( fLastSortedHit1P.GetChan(), usChanIdx );
                     fhStsSortedCntEvoP1N2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtP1N2 ) < fdCoincBorder )

                  if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder )
                  {
                     fhStsSortedMapN1P1Coinc->Fill( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan() );
                     fhStsSortedMapN2P2Coinc->Fill( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan() );

                     Double_t dX1, dY1, dX2, dY2;
                     ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapX1Y1Coinc->Fill( dX1, dY1 );
                     fhStsSortedMapX2Y2Coinc->Fill( dX2, dY2 );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
               } // if( fUnpackPar->GetAsicIndexSts2N() == usAsicIdx )
               else if( fUnpackPar->GetAsicIndexSts2P() == usAsicIdx )
               {
                  fLastSortedHit2P = (*it);
                  dLastTimeP2      = ulHitTs;

//                  Double_t dDtN1P1 = ( dLastTimeP1 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtN1N2 = ( dLastTimeN2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1P2 = ( dLastTimeP2 - dLastTimeN1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtP1P2 = ( dLastTimeP2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
//                  Double_t dDtP1N2 = ( dLastTimeN2 - dLastTimeP1 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN2P2 = ( dLastTimeP2 - dLastTimeN2 ) * stsxyter::kdClockCycleNs;
                  Double_t dDtN1P1N2P2 = ( dLastTimeN2 + dLastTimeP2 - ( dLastTimeN1 + dLastTimeP1 ) ) * stsxyter::kdClockCycleNs / 2.0;

                  fhStsSortedDtN2P2->Fill( dDtN2P2 );
                  fhStsSortedDtP1P2->Fill( dDtP1P2 );
                  fhStsSortedDtN1P2->Fill( dDtN1P2 );
                  fhStsSortedDtN1P1N2P2->Fill( dDtN1P1N2P2 );
                  Int_t iChanDiffP =  static_cast< Int_t >( fLastSortedHit2P.GetChan() )
                                    - static_cast< Int_t >( fLastSortedHit1P.GetChan() );
                  fhStsSortedDtVsDiffP->Fill( dDtN1P1N2P2, iChanDiffP );

                  if( TMath::Abs( dDtN2P2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapN2P2->Fill( fLastSortedHit2N.GetChan(), usChanIdx );
                     fhStsSortedCntEvoN2P2->Fill( dTimeSinceStartSec );

                     Double_t dX, dY;
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX, dY );
                     fhStsSortedMapX2Y2->Fill( dX, dY );
                  } // if( TMath::Abs( dDtN2P2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtP1P2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapP1P2->Fill( fLastSortedHit1P.GetChan(), usChanIdx );
                     fhStsSortedCntEvoP1P2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtP1P2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
                  {
                     fhStsSortedMapN1P2->Fill( fLastSortedHit1N.GetChan(), usChanIdx );
                     fhStsSortedCntEvoN1P2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )

                  if( TMath::Abs( dDtN1P1N2P2 ) < 2*fdCoincBorder )
                  {
                     fhStsSortedMapN1P1Coinc->Fill( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan() );
                     fhStsSortedMapN2P2Coinc->Fill( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan() );

                     Double_t dX1, dY1, dX2, dY2;
                     ComputeCoordinatesSensor1( fLastSortedHit1N.GetChan(), fLastSortedHit1P.GetChan(), dX1, dY1 );
                     ComputeCoordinatesSensor2( fLastSortedHit2N.GetChan(), fLastSortedHit2P.GetChan(), dX2, dY2 );
                     fhStsSortedMapX1Y1Coinc->Fill( dX1, dY1 );
                     fhStsSortedMapX2Y2Coinc->Fill( dX2, dY2 );
                  } // if( TMath::Abs( dDtN1P2 ) < fdCoincBorder )
               } // else if( fUnpackPar->GetAsicIndexSts2P() == usAsicIdx )
            } // else if( kTRUE == fbDualStsEna )
         } // loop on hits untils hits within 100 ns of last one or last one itself are reached

         // Remove all hits which were already used
         fvmHitsInTs.erase( fvmHitsInTs.begin(), it );
      } // if( 0 < fvmHitsInTs.size() )

      // Remove all hits from this TS
/*
      for( UInt_t uAsicIdx = 0; uAsicIdx < fuNbStsXyters; ++uAsicIdx )
         for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
            fvmChanHitsInTs[ uAsicIdx ][ uChan ].clear();
*/
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

void CbmCosy2018MonitorSts::FillHitInfo( stsxyter::Message mess, const UShort_t & /*usElinkIdx*/, const UInt_t & uAsicIdx, const UInt_t & uMsIdx )
{
   UShort_t usChan     = mess.GetHitChannel();
   UShort_t usRawAdc   = mess.GetHitAdc();
//   UShort_t usFullTs = mess.GetHitTimeFull();
//   Bool_t   bMissedEvt = mess.IsHitMissedEvts();
   UShort_t usRawTs    = mess.GetHitTime();

   fhStsChanCounts[  uAsicIdx ]->Fill( usChan );
   fhStsChanRawAdc[  uAsicIdx ]->Fill( usChan, usRawAdc );
   fhStsChanRawAdcProf[  uAsicIdx ]->Fill( usChan, usRawAdc );
   fhStsChanRawTs[   uAsicIdx ]->Fill( usChan, usRawTs );
   fhStsChanMissEvt[ uAsicIdx ]->Fill( usChan, mess.IsHitMissedEvts() );

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

   fvuElinkLastTsHit[fuCurrDpbIdx] = usRawTs;

   // Convert the Hit time in bins to Hit time in ns
   Double_t dHitTimeNs = fvulChanLastHitTime[ uAsicIdx ][ usChan ] * stsxyter::kdClockCycleNs;

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
//   fvmChanHitsInTs[        uAsicIdx ][ usChan ].insert( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ], usRawAdc, uAsicIdx, usChan ) );
   fvmHitsInTs.insert( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ], usRawAdc, uAsicIdx, usChan ) );

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
   fhStsChanHitRateEvo[ uAsicIdx ]->Fill( dTimeSinceStartSec , usChan );
   fhStsXyterRateEvo[ uAsicIdx ]->Fill(   dTimeSinceStartSec );
   fhStsChanHitRateEvoLong[ uAsicIdx ]->Fill( dTimeSinceStartMin, usChan, 1.0/60.0 );
   fhStsXyterRateEvoLong[ uAsicIdx ]->Fill(   dTimeSinceStartMin, 1.0/60.0 );

   if( kTRUE == fbLongHistoEnable )
   {
      std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
      Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();
      fhFebRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , 1.0 / fuLongHistoBinSizeSec );
      fhFebChRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , usChan, 1.0 / fuLongHistoBinSizeSec );
   } // if( kTRUE == fbLongHistoEnable )

}

void CbmCosy2018MonitorSts::FillTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx, UInt_t /*uMsIdx*/ )
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

   if( 1 < uMessIdx )
   {
      fhStsDpbRawTsMsb->Fill( fuCurrDpbIdx,      fvulCurrentTsMsb[fuCurrDpbIdx] );
      fhStsDpbRawTsMsbSx->Fill( fuCurrDpbIdx,  ( fvulCurrentTsMsb[fuCurrDpbIdx] & 0x1F ) );
      fhStsDpbRawTsMsbDpb->Fill( fuCurrDpbIdx, ( fvulCurrentTsMsb[fuCurrDpbIdx] >> 5 ) );
   } // if( 0 < uMessIdx )

//   fhStsAsicTsMsb->Fill( fvulCurrentTsMsb[fuCurrDpbIdx], uAsicIdx );
}

void CbmCosy2018MonitorSts::FillEpochInfo( stsxyter::Message /*mess*/ )
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

void CbmCosy2018MonitorSts::Reset()
{
}

void CbmCosy2018MonitorSts::Finish()
{

   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmCosy2018MonitorSts statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::MessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::MessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::MessType::Dummy ] << "\n"
             << " Epoch    messages: " << fmMsgCounter[ stsxyter::MessType::Epoch ] << "\n"
             << " Empty    messages: " << fmMsgCounter[ stsxyter::MessType::Empty ];

   LOG(info) << "-------------------------------------";

   SaveAllHistos( "data/StsHistos.root" );
   SaveAllHistos();

}


void CbmCosy2018MonitorSts::FillOutput(boost::any)
{
}

void CbmCosy2018MonitorSts::SaveAllHistos( TString sFileName )
{
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

   fhStsMessType->Write();
   fhStsSysMessType->Write();
   fhStsMessTypePerDpb->Write();
   fhStsSysMessTypePerDpb->Write();
   fhStsDpbRawTsMsb->Write();
   fhStsDpbRawTsMsbSx->Write();
   fhStsDpbRawTsMsbDpb->Write();
   fhStsMessTypePerElink->Write();
   fhStsSysMessTypePerElink->Write();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhStsChanCounts[ uXyterIdx ]->Write();
      fhStsChanRawAdc[ uXyterIdx ]->Write();
      fhStsChanRawAdcProf[ uXyterIdx ]->Write();
      fhStsChanRawTs[ uXyterIdx ]->Write();
      fhStsChanMissEvt[ uXyterIdx ]->Write();
      fhStsChanHitRateEvo[ uXyterIdx ]->Write();
      fhStsXyterRateEvo[ uXyterIdx ]->Write();
      fhStsChanHitRateEvoLong[ uXyterIdx ]->Write();
      fhStsXyterRateEvoLong[ uXyterIdx ]->Write();
      if( fbChanHitDtEna )
      {
         fhStsChanHitDt[ uXyterIdx ]->Write();
         fhStsChanHitDtNeg[ uXyterIdx ]->Write();
      } // if( fbChanHitDtEna )
      fhStsChanHitsPerMs[ uXyterIdx ]->Write();
      if( kTRUE == fbLongHistoEnable )
      {
         fhFebRateEvoLong[ uXyterIdx ]->Write();
         fhFebChRateEvoLong[ uXyterIdx ]->Write();
      } // if( kTRUE == fbLongHistoEnable )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   fhStsAsicTsMsb->Write();

   gDirectory->cd("..");

   gDirectory->mkdir("Sts_Map");
   gDirectory->cd("Sts_Map");
   // Coincidences in same MS (unsorted hits)
      // Coincidences in sorted hits
   fhStsSortedDtN1P1->Write();
   fhStsSortedMapN1P1->Write();
   fhStsSortedMapX1Y1->Write();
   if( kTRUE == fbDualStsEna )
   {
      fhStsSortedDtN2P2->Write();
      fhStsSortedDtN1N2->Write();
      fhStsSortedDtP1P2->Write();
      fhStsSortedDtN1P2->Write();
      fhStsSortedDtP1N2->Write();
      fhStsSortedMapN2P2->Write();
      fhStsSortedMapN1N2->Write();
      fhStsSortedMapP1P2->Write();
      fhStsSortedMapN1P2->Write();
      fhStsSortedMapP1N2->Write();

      fhStsSortedDtN1P1N2P2->Write();
      fhStsSortedMapN1P1Coinc->Write();
      fhStsSortedMapN2P2Coinc->Write();
      fhStsSortedDtVsDiffN->Write();
      fhStsSortedDtVsDiffP->Write();

      fhStsSortedMapX2Y2->Write();
      fhStsSortedMapX1Y1Coinc->Write();
      fhStsSortedMapX2Y2Coinc->Write();
   } // if( kTRUE == fbDualStsEna )
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
   gDirectory->cd("..");

   if( "" != sFileName )
   {
      // Restore original directory position
      histoFile->Close();
      oldDir->cd();
   } // if( "" != sFileName )

}
void CbmCosy2018MonitorSts::ResetAllHistos()
{
   LOG(info) << "Reseting all STS histograms.";

   fhStsMessType->Reset();
   fhStsSysMessType->Reset();
   fhStsMessTypePerDpb->Reset();
   fhStsSysMessTypePerDpb->Reset();
   fhStsDpbRawTsMsb->Reset();
   fhStsDpbRawTsMsbSx->Reset();
   fhStsDpbRawTsMsbDpb->Reset();
   fhStsMessTypePerElink->Reset();
   fhStsSysMessTypePerElink->Reset();

   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fhStsChanCounts[ uXyterIdx ]->Reset();
      fhStsChanRawAdc[ uXyterIdx ]->Reset();
      fhStsChanRawAdcProf[ uXyterIdx ]->Reset();
      fhStsChanRawTs[ uXyterIdx ]->Reset();
      fhStsChanMissEvt[ uXyterIdx ]->Reset();
      fhStsChanHitRateEvo[ uXyterIdx ]->Reset();
      fhStsXyterRateEvo[ uXyterIdx ]->Reset();
      fhStsChanHitRateEvoLong[ uXyterIdx ]->Reset();
      fhStsXyterRateEvoLong[ uXyterIdx ]->Reset();
      if( fbChanHitDtEna )
      {
         fhStsChanHitDt[ uXyterIdx ]->Reset();
         fhStsChanHitDtNeg[ uXyterIdx ]->Reset();
      } // if( fbChanHitDtEna )
      fhStsChanHitsPerMs[ uXyterIdx ]->Reset();
      if( kTRUE == fbLongHistoEnable )
      {
         ftStartTimeUnix = std::chrono::steady_clock::now();
         fhFebRateEvoLong[ uXyterIdx ]->Reset();
         fhFebChRateEvoLong[ uXyterIdx ]->Reset();
      } // if( kTRUE == fbLongHistoEnable )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   fhStsAsicTsMsb->Reset();

   // Coincidences in same MS (unsorted hits)
      // Coincidences in sorted hits
   fhStsSortedDtN1P1->Reset();
   fhStsSortedMapN1P1->Reset();
   fhStsSortedMapX1Y1->Reset();

   if( kTRUE == fbDualStsEna )
   {
      fhStsSortedDtN2P2->Reset();
      fhStsSortedDtN1N2->Reset();
      fhStsSortedDtP1P2->Reset();
      fhStsSortedDtN1P2->Reset();
      fhStsSortedDtP1N2->Reset();
      fhStsSortedMapN2P2->Reset();
      fhStsSortedMapN1N2->Reset();
      fhStsSortedMapP1P2->Reset();
      fhStsSortedMapN1P2->Reset();
      fhStsSortedMapP1N2->Reset();

      fhStsSortedDtN1P1N2P2->Reset();
      fhStsSortedMapN1P1Coinc->Reset();
      fhStsSortedMapN2P2Coinc->Reset();
      fhStsSortedDtVsDiffN->Reset();
      fhStsSortedDtVsDiffP->Reset();

      fhStsSortedMapX2Y2->Reset();
      fhStsSortedMapX1Y1Coinc->Reset();
      fhStsSortedMapX2Y2Coinc->Reset();
   } // if( kTRUE == fbDualStsEna )

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

void CbmCosy2018MonitorSts::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}

void CbmCosy2018MonitorSts::SetLongDurationLimits( UInt_t uDurationSeconds, UInt_t uBinSize )
{
   fbLongHistoEnable     = kTRUE;
   fuLongHistoNbSeconds  = uDurationSeconds;
   fuLongHistoBinSizeSec = uBinSize;
}

void CbmCosy2018MonitorSts::ComputeCoordinatesSensor1( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY )
{
   dPosX = 0.0;
   dPosY = 0.0;

   Int_t iCoordN = iChanN + fiStripsOffsetN1;
   Int_t iCoordP = 127 - (iChanP + fiStripsOffsetP1);

   dPosX = kdPitchMm * iCoordN;
   dPosY = kdSensorsSzY / 2.0 - kdPitchMm * ( iCoordN - iCoordP ) / fdStereoAngleTan;
}

void CbmCosy2018MonitorSts::ComputeCoordinatesSensor2( Int_t iChanN, Int_t iChanP, Double_t & dPosX, Double_t & dPosY )
{
   dPosX = 0.0;
   dPosY = 0.0;

   Int_t iCoordN = iChanN + fiStripsOffsetN2;
   Int_t iCoordP = 127 - (iChanP + fiStripsOffsetP2);

   dPosX = kdPitchMm * iCoordN;
   dPosY = kdSensorsSzY / 2.0 - kdPitchMm * ( iCoordN - iCoordP ) / fdStereoAngleTan;
}

ClassImp(CbmCosy2018MonitorSts)
