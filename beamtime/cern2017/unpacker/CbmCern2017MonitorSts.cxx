// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCern2017MonitorSts                         -----
// -----                Created 24/11/17  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCern2017MonitorSts.h"

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
#include <TFile.h>

// C++11

// C/C++
#include <iostream>
#include <stdint.h>
#include <iomanip>

Bool_t bCern2017ResetStsHistos = kFALSE;
Bool_t bCern2017WriteStsHistos = kFALSE;

CbmCern2017MonitorSts::CbmCern2017MonitorSts() :
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
   fvuChanNbHitsInMs(),
   fvdChanLastHitTimeInMs(),
   fvusChanLastHitAdcInMs(),
   fvmChanHitsInTs(),
   fdStartTime(-1.0),
   fdStartTimeMsSz(-1.0),
   ftStartTimeUnix( std::chrono::steady_clock::now() ),
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
   fhStsChanSameMs(),
   fpStsChanSameMsTimeDiff(),
   fhStsChanSameMsTimeDiff(),
   fbPulserTimeDiffOn(kFALSE),
   fuPulserMaxNbMicroslices(100),
   fvuPulserAsic(),
   fvuPulserChan(),
   fhStsPulserChansTimeDiff(),
   fhStsPulserChansTimeDiffEvo(),
   fhStsPulserChansTimeDiffAdc(),
   fhStsAsicTsMsb(NULL),
   fhStsAsicTsMsbMaj(NULL),
   fhStsElinkTsMsbCrc(),
   fhStsElinkTsMsbMaj(),
   fbLongHistoEnable( kFALSE ),
   fuLongHistoNbSeconds( 0 ),
   fuLongHistoBinSizeSec( 0 ),
   fuLongHistoBinNb( 0 ),
   fhFebRateEvoLong(),
   fhFebChRateEvoLong(),
   fcMsSizeAll(NULL)
{
}

CbmCern2017MonitorSts::~CbmCern2017MonitorSts()
{
}

Bool_t CbmCern2017MonitorSts::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmCern2017MonitorSts::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackPar = (CbmCern2017UnpackParSts*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParSts"));
}


Bool_t CbmCern2017MonitorSts::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmCern2017MonitorSts::ReInitContainers()
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

      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      {
         if( 0 == uElink % 10 )
            LOG(info) << "\n"
                      << "------> ";

         LOG(info) << std::setw( 5 ) << fvuElinkToAsic[uDpb][uElink] << " ";
      } // for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      LOG(info) << "";
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   LOG(info) << "Counter size for TS:     " << stsxyter::kuHitNbTsBins;
   LOG(info) << "Counter size for TS_MSB: " << stsxyter::kuTsMsbNbTsBins;
   LOG(info) << "Counter size for cycles: " << stsxyter::kulTsCycleNbBins;

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
   fvmChanHitsInTs.resize( fuNbStsXyters );
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      fvulChanLastHitTime[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvdChanLastHitTime[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvuChanNbHitsInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvdChanLastHitTimeInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvusChanLastHitAdcInMs[ uXyterIdx ].resize( fuNbChanPerAsic );
      fvmChanHitsInTs[ uXyterIdx ].resize( fuNbChanPerAsic );
      for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
      {
         fvulChanLastHitTime[ uXyterIdx ][ uChan ] = 0;
         fvdChanLastHitTime[ uXyterIdx ][ uChan ] = -1.0;

         fvuChanNbHitsInMs[ uXyterIdx ][ uChan ].resize( fuPulserMaxNbMicroslices );
         fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ].resize( fuPulserMaxNbMicroslices );
         fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ].resize( fuPulserMaxNbMicroslices );
         fvmChanHitsInTs[ uXyterIdx ][ uChan ].clear();
         for( UInt_t uMsIdx = 0; uMsIdx < fuPulserMaxNbMicroslices; ++uMsIdx )
         {
            fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
            fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
         } // for( UInt_t uMsIdx = 0; uMsIdx < fuPulserMaxNbMicroslices; ++uMsIdx )
      } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   LOG(info) << "CbmCern2017MonitorSts::ReInitContainers => Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
   LOG(info) << "CbmCern2017MonitorSts::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
   LOG(info) << "CbmCern2017MonitorSts::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuPulserMaxNbMicroslices;
   return kTRUE;
}

void CbmCern2017MonitorSts::CreateHistograms()
{
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

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
   fHM->Add(sHistName.Data(), fhStsMessType);
   if( server ) server->Register("/StsRaw", fhStsMessType );

   sHistName = "hStsSysMessType";
   title = "Nb of system message for each type; System Type";
   fhStsSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
/*
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhStsSysMessType);
   if( server ) server->Register("/StsRaw", fhStsSysMessType );

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
   fHM->Add(sHistName.Data(), fhStsMessTypePerDpb);
   if( server ) server->Register("/StsRaw", fhStsMessTypePerDpb );

   sHistName = "hStsSysMessTypePerDpb";
   title = "Nb of system message of each type for each DPB; DPB; System Type";
   fhStsSysMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);
/*
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhStsSysMessTypePerDpb);
   if( server ) server->Register("/StsRaw", fhStsSysMessTypePerDpb );

   sHistName = "hStsDpbRawTsMsb";
   title = "Raw TsMsb distribution for each DPB; DPB; TS MSB [Bins]";
   fhStsDpbRawTsMsb = new TH2I( sHistName, title,
                                fuNrOfDpbs, 0, fuNrOfDpbs,
                                stsxyter::kuTsMsbNbTsBins, 0., stsxyter::kuTsMsbNbTsBins);
   if( server ) server->Register("/StsRaw", fhStsDpbRawTsMsb );

   sHistName = "hStsDpbRawTsMsbSx";
   title = "Raw TsMsb distribution for each DPB; DPB; TS MSB [Bins]";
   fhStsDpbRawTsMsbSx = new TH2I( sHistName, title,
                                  fuNrOfDpbs, 0, fuNrOfDpbs,
                                  0x1F , 0., 0x1F );
   if( server ) server->Register("/StsRaw", fhStsDpbRawTsMsbSx );

   sHistName = "hStsDpbRawTsMsbDpb";
   title = "Raw TsMsb distribution for each DPB; DPB; TS MSB [Bins]";
   fhStsDpbRawTsMsbDpb = new TH2I( sHistName, title,
                                   fuNrOfDpbs, 0, fuNrOfDpbs,
                                   stsxyter::kuTsMsbNbTsBins >> 5, 0., stsxyter::kuTsMsbNbTsBins >> 5 );
   if( server ) server->Register("/StsRaw", fhStsDpbRawTsMsbDpb );

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
   fHM->Add(sHistName.Data(), fhStsMessTypePerElink);
   if( server ) server->Register("/StsRaw", fhStsMessTypePerElink );

   sHistName = "hStsSysMessTypePerElink";
   title = "Nb of system message of each type for each eLink; eLink; System Type";
   fhStsSysMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 17, 0., 17.);
/*
   fhStsSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   fhStsSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhStsSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhStsSysMessTypePerElink);
   if( server ) server->Register("/StsRaw", fhStsSysMessTypePerElink );

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
      fHM->Add(sHistName.Data(), fhStsChanCounts[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanCounts[ uXyterIdx ] );

      // Raw Adc Distribution
      sHistName = Form( "hStsChanRawAdc_%03u", uXyterIdx );
      title = Form( "Raw Adc distribution per channel, StsXyter #%03u; Channel []; Adc []; Hits []", uXyterIdx );
      fhStsChanRawAdc.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanRawAdc[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanRawAdc[ uXyterIdx ] );

      // Raw Adc Distribution profile
      sHistName = Form( "hStsChanRawAdcProfc_%03u", uXyterIdx );
      title = Form( "Raw Adc prodile per channel, StsXyter #%03u; Channel []; Adc []", uXyterIdx );
      fhStsChanRawAdcProf.push_back( new TProfile(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanRawAdcProf[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanRawAdcProf[ uXyterIdx ] );

      // Raw Ts Distribution
      sHistName = Form( "hStsChanRawTs_%03u", uXyterIdx );
      title = Form( "Raw Timestamp distribution per channel, StsXyter #%03u; Channel []; Ts []; Hits []", uXyterIdx );
      fhStsChanRawTs.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins -0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanRawTs[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanRawTs[ uXyterIdx ] );

      // Missed event flag
      sHistName = Form( "hStsChanMissEvt_%03u", uXyterIdx );
      title = Form( "Missed Event flags per channel, StsXyter #%03u; Channel []; Miss Evt []; Hits []", uXyterIdx );
      fhStsChanMissEvt.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 2, -0.5, 1.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanMissEvt[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanMissEvt[ uXyterIdx ] );

      // Hit rates evo per channel
      sHistName = Form( "hStsChanRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [s]; Channel []; Hits []", uXyterIdx );
      fhStsChanHitRateEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanHitRateEvo[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanHitRateEvo[ uXyterIdx ] );

      // Hit rates evo per StsXyter
      sHistName = Form( "hStsXyterRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [s]; Hits []", uXyterIdx );
      fhStsXyterRateEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );
      fHM->Add(sHistName.Data(), fhStsXyterRateEvo[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsXyterRateEvo[ uXyterIdx ] );

      // Hit rates evo per channel, 1 minute bins, 24h
      sHistName = Form( "hStsChanRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [min]; Channel []; Hits []", uXyterIdx );
      fhStsChanHitRateEvoLong.push_back( new TH2D( sHistName, title,
                                                1440, 0, 1440,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanHitRateEvoLong[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanHitRateEvoLong[ uXyterIdx ] );

      // Hit rates evo per StsXyter, 1 minute bins, 24h
      sHistName = Form( "hStsXyterRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [min]; Hits []", uXyterIdx );
      fhStsXyterRateEvoLong.push_back( new TH1D(sHistName, title, 1440, 0, 1440 ) );
      fHM->Add(sHistName.Data(), fhStsXyterRateEvoLong[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsXyterRateEvoLong[ uXyterIdx ] );

      // Hit distance in time for each channel
      if( fbChanHitDtEna )
      {
         sHistName = Form( "hStsChanHitDt_%03u", uXyterIdx );
         title = Form( "Time diff between hits on same channel in StsXyter #%03u; t_hit - t_prev [ns]; Channel []; Hits []", uXyterIdx );
         fhStsChanHitDt.push_back( new TH2I( sHistName, title,
                                                   iNbBinsRate - 1, dBinsRate,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
         fHM->Add(sHistName.Data(), fhStsChanHitDt[ uXyterIdx ] );
         if( server ) server->Register("/StsRaw", fhStsChanHitDt[ uXyterIdx ] );
         sHistName = Form( "hStsChanHitDtNeg_%03u", uXyterIdx );
         title = Form( "Time diff between hits on same channel in StsXyter #%03u; t_prev - t_hit [ns]; Channel []; Hits []", uXyterIdx );
         fhStsChanHitDtNeg.push_back( new TH2I( sHistName, title,
                                                   iNbBinsRate - 1, dBinsRate,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
         fHM->Add(sHistName.Data(), fhStsChanHitDtNeg[ uXyterIdx ] );
         if( server ) server->Register("/StsRaw", fhStsChanHitDtNeg[ uXyterIdx ] );

      } // if( fbChanHitDtEna )

      sHistName = Form( "hStsChanHitsPerMs_%03u", uXyterIdx );
      title = Form( "Nb of hits per channel in each MS in StsXyter #%03u; Nb Hits in MS []; Channel []; MS []", uXyterIdx );
      fhStsChanHitsPerMs.push_back( new TH2I( sHistName, title,
                                                100, -0.5, 99.5,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanHitsPerMs[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanHitsPerMs[ uXyterIdx ] );

      sHistName = Form( "hStsChanSameMs_%03u", uXyterIdx );
      title = Form( "Nb of MS with hits in both channels in StsXyter #%03u; Channel A []; Channel B []; Coinc. MS []", uXyterIdx );
      fhStsChanSameMs.push_back( new TH2I( sHistName, title,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanHitsPerMs[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanSameMs[ uXyterIdx ] );

      sHistName = Form( "pStsChanSameMsTimeDiff_%03u", uXyterIdx );
      title = Form( "Mean Time difference of channels when hits in same MS in StsXyter #%03u; Channel A []; Channel B []; Mean time diff [bins]", uXyterIdx );
      fpStsChanSameMsTimeDiff.push_back( new TProfile2D( sHistName, title,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );

      sHistName = Form( "hStsChanSameMsTimeDiff_%03u", uXyterIdx );
      title = Form( "Mean Time difference of channels when hits in same MS in StsXyter #%03u; Channel A []; Channel B []; Mean time diff [bins]", uXyterIdx );
      fhStsChanSameMsTimeDiff.push_back( new TH2D( sHistName, title,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhStsChanSameMsTimeDiff[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhStsChanSameMsTimeDiff[ uXyterIdx ] );

      if( kTRUE == fbLongHistoEnable )
      {
         UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
         fuLongHistoBinNb = uAlignedLimit / fuLongHistoBinSizeSec;

         sHistName = Form( "hFebRateEvoLong_%03u", uXyterIdx );
         title = Form( "Mean rate VS run time in same MS in StsXyter #%03u; Time in run [s]; Rate [1/s]", uXyterIdx );
         fhFebRateEvoLong.push_back( new TH1D( sHistName, title,
                                                   fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5) );
         fHM->Add(sHistName.Data(), fhFebRateEvoLong[ uXyterIdx ] );
         if( server ) server->Register("/StsRaw", fhFebRateEvoLong[ uXyterIdx ] );

         sHistName = Form( "hFebChRateEvoLong_%03u", uXyterIdx );
         title = Form( "Mean rate per channel VS run time in StsXyter #%03u; Time in run [s]; Channel []; Rare [1/s]", uXyterIdx );
         fhFebChRateEvoLong.push_back( new TH2D( sHistName, title,
                                                   fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                                   fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
         fHM->Add(sHistName.Data(), fhFebChRateEvoLong[ uXyterIdx ] );
         if( server ) server->Register("/StsRaw", fhFebChRateEvoLong[ uXyterIdx ] );
      } // if( kTRUE == fbLongHistoEnable )

   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   if( kTRUE == fbPulserTimeDiffOn )
   {
      UInt_t uNbPulserChans = fvuPulserAsic.size();
      UInt_t uNbBinEvo = 32768 + 1;
      Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
                            * static_cast< Double_t >( uNbBinEvo ) / 2.0;
      Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;
      for( UInt_t uChA = 0; uChA < uNbPulserChans; ++uChA )
         for( UInt_t uChB = uChA + 1; uChB < uNbPulserChans; ++uChB )
         {
            sHistName = Form( "hStsPulserTimeDiff_%02u_%03u_%02u_%03u",
                              fvuPulserAsic[uChA], fvuPulserChan[uChA],
                              fvuPulserAsic[uChB], fvuPulserChan[uChB] );
            title = Form( "Time diff for hits in same MS from ASIC %02d ch %03d and ASIC %02d ch %03d; tB - tA [ns]; Counts",
                              fvuPulserAsic[uChA], fvuPulserChan[uChA],
                              fvuPulserAsic[uChB], fvuPulserChan[uChB] );
//            TH1 * pHist = new TH1I(sHistName, title, 16385, -51203.125, 51203.125);
            TH1 * pHist = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

            fHM->Add(sHistName.Data(), pHist);
            if( server ) server->Register("/StsRaw", pHist );
            fhStsPulserChansTimeDiff.push_back( pHist );

            if( kTRUE == fbLongHistoEnable )
            {
               UInt_t uAlignedLimit = fuLongHistoNbSeconds - (fuLongHistoNbSeconds % fuLongHistoBinSizeSec);
               sHistName = Form( "hStsPulserChansTimeDiffEvo_%02u_%03u_%02u_%03u",
                              fvuPulserAsic[uChA], fvuPulserChan[uChA],
                              fvuPulserAsic[uChB], fvuPulserChan[uChB] );
               title = Form( "Time diff for hits in same MS from ASIC %02d ch %03d and ASIC %02d ch %03d, vs time in run; Time in run [min]; tB - tA [ns]; Counts",
                                 fvuPulserAsic[uChA], fvuPulserChan[uChA],
                                 fvuPulserAsic[uChB], fvuPulserChan[uChB] );
               TH2 * pHistEvo = new TH2I( sHistName, title,
                                       fuLongHistoBinNb, -0.5, uAlignedLimit - 0.5,
                                       uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );

               fHM->Add(sHistName.Data(), pHistEvo);
               if( server ) server->Register("/StsRaw", pHistEvo );
               fhStsPulserChansTimeDiffEvo.push_back( pHistEvo );
            } // if( kTRUE == fbLongHistoEnable )

            sHistName = Form( "hStsPulserChansTimeDiffAdc_%02u_%03u_%02u_%03u",
                              fvuPulserAsic[uChA], fvuPulserChan[uChA],
                              fvuPulserAsic[uChB], fvuPulserChan[uChB] );
            title = Form( "Time diff vs Adc Diff for hits in same MS from ASIC %02d ch %03d and ASIC %02d ch %03d; tB - tA [ns];  Adc diff [a.u.]; Counts",
                              fvuPulserAsic[uChA], fvuPulserChan[uChA],
                              fvuPulserAsic[uChB], fvuPulserChan[uChB] );
            TH2 * pHistAdc = new TH2I( sHistName, title,
                                        uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo,
                                        2*stsxyter::kuHitNbAdcBins + 1, -0.5 - stsxyter::kuHitNbAdcBins, stsxyter::kuHitNbAdcBins + 0.5 );
            fhStsPulserChansTimeDiffAdc.push_back( pHistAdc );
            if( server ) server->Register("/StsRaw", pHistAdc );
         } // Loop on pairs of channels
   } // if( kTRUE == fbPulserTimeDiffOn )

   // Distribution of the TS_MSB per StsXyter
   sHistName = "hStsAsicTsMsb";
   title = "Raw Timestamp Msb distribution per StsXyter; Ts MSB []; StsXyter []; Hits []";
   fhStsAsicTsMsb = new TH2I( sHistName, title, stsxyter::kuTsMsbNbTsBins, -0.5,   stsxyter::kuTsMsbNbTsBins - 0.5,
                                                fuNbStsXyters, -0.5, fuNbStsXyters - 0.5 );
   fHM->Add(sHistName.Data(), fhStsAsicTsMsb );
   if( server ) server->Register("/StsRaw", fhStsAsicTsMsb );

   // Nb values in agreement in TS MSB messages, per StsXyter
   sHistName = "hStsAsicTsMsbMaj";
   title = "Nb values in agreement in TS MSB messages, per StsXyter; StsXyter []; Majority? []; Mess. []";
   fhStsAsicTsMsbMaj = new TH2I( sHistName, title, fuNbStsXyters, -0.5, fuNbStsXyters - 0.5,
                                                   3, 0.5, 3.5);
   fHM->Add(sHistName.Data(), fhStsAsicTsMsbMaj );
   if( server ) server->Register("/StsRaw", fhStsAsicTsMsbMaj );

   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      // CRC good or bad in TS MSB messages, per (DPB, eLink) pairs
      sHistName = Form( "hStsElinkTsMsbCrc_d%02u", uDpb );
      title = Form( "CRC test in TS MSB, per eLink, DPB #%02u; eLink []; CRC Match? []; Mess. []", uDpb);
      fhStsElinkTsMsbCrc.push_back( new TH2I( sHistName, title, fuNbElinksPerDpb, -0.5, fuNbElinksPerDpb - 0.5,
                                                                2, -0.5, 1.5) );
      fHM->Add(sHistName.Data(), fhStsElinkTsMsbCrc[ uDpb ] );
      if( server ) server->Register("/StsRaw", fhStsElinkTsMsbCrc[ uDpb ] );

      // Nb values in agreement in TS MSB messages, per (DPB, eLink) pairs
      sHistName = Form( "hStsElinkTsMsbMaj_d%02u", uDpb );
      title = Form( "Nb values in agreement in TS MSB, per eLink, DPB #%02u; eLink []; Majority? []; Mess. []", uDpb);
      fhStsElinkTsMsbMaj.push_back( new TH2I( sHistName, title, fuNbElinksPerDpb, -0.5, fuNbElinksPerDpb - 0.5,
                                                                3, 0.5, 3.5) );
      fHM->Add(sHistName.Data(), fhStsElinkTsMsbMaj[ uDpb ] );
      if( server ) server->Register("/StsRaw", fhStsElinkTsMsbMaj[ uDpb ] );
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   // Miscroslice properties histos
   for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )
   {
      fhMsSz[ component ] = NULL;
      fhMsSzTime[ component ] = NULL;
   } // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )

   // Online histo browser commands
   if( server )
   {
      server->RegisterCommand("/Reset_All_Sts", "bCern2017ResetStsHistos=kTRUE");
      server->RegisterCommand("/Write_All_Sts", "bCern2017WriteStsHistos=kTRUE");

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
      gPad->SetLogy();
      fhStsChanCounts[ uXyterIdx ]->Draw();

      cStsSumm->cd(2);
      gPad->SetLogz();
      fhStsChanRawAdc[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(3);
      gPad->SetLogz();
      fhStsChanRawTs[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(4);
      gPad->SetLogy();
      fhStsXyterRateEvo[ uXyterIdx ]->Draw();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

      // Pulser testing
   if( kTRUE == fbPulserTimeDiffOn )
   {
      UInt_t uNbPulserPlots = fhStsPulserChansTimeDiff.size();
      TCanvas* cStsPulser = new TCanvas( "cStsPulser" , "Summary plots for StsXyter pulser testing",
                                         w, h);
      cStsPulser->Divide( 2, uNbPulserPlots/2 + uNbPulserPlots%2 );

      for( UInt_t uPulserPlot = 0; uPulserPlot < uNbPulserPlots; ++uPulserPlot)
      {
         cStsPulser->cd( 1 + uPulserPlot );
         gPad->SetLogy();
         gPad->SetGridx();
         gPad->SetGridy();
         fhStsPulserChansTimeDiff[ uPulserPlot ]->Draw();
         gStyle->SetOptStat("emruo");
         fhStsPulserChansTimeDiff[ uPulserPlot ]->SetStats(1);
      } // for( UInt_t uPulserPlot = 0; uPulserPlot < uNbPulserPlots; ++uPulserPlot)

      if( kTRUE == fbLongHistoEnable )
      {
         TCanvas* cStsPulserEvo = new TCanvas( "cStsPulserEvo" , "Summary plots for StsXyter pulser testing vs time",
                                            w, h);
         cStsPulserEvo->Divide( 2, uNbPulserPlots/2 + uNbPulserPlots%2 );

         for( UInt_t uPulserPlot = 0; uPulserPlot < uNbPulserPlots; ++uPulserPlot)
         {
            cStsPulserEvo->cd( 1 + uPulserPlot );
            gPad->SetGridx();
            gPad->SetGridy();
            fhStsPulserChansTimeDiffEvo[ uPulserPlot ]->Draw( "colz" );
            gStyle->SetOptStat("emruo");
            fhStsPulserChansTimeDiffEvo[ uPulserPlot ]->SetStats(1);
         } // for( UInt_t uPulserPlot = 0; uPulserPlot < uNbPulserPlots; ++uPulserPlot)
      } // if( kTRUE == fbLongHistoEnable )
   } // if( kTRUE == fbPulserTimeDiffOn )

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

/*
  Int_t iNbPadsPerDpb = fuNbElinksPerDpb/2 + fuNbElinksPerDpb%2;
  TCanvas* cMuchChCounts = new TCanvas("cMuchChCounts", "MUCH Channels counts", w, h);
  cMuchChCounts->Divide( fNrOfNdpbs/2 + fNrOfNdpbs%2, fNrOfFebsPerNdpb );

  TCanvas* cMuchFebRate = new TCanvas("cMuchFebRate", "MUCH FEB rate", w, h);
  cMuchFebRate->Divide( fNrOfNdpbs/2 + fNrOfNdpbs%2, fNrOfFebsPerNdpb );

   TH1* histPnt = NULL;
   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
      {
         sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdA(dpbId) );
      } // if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
         else
         {
            sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdB(dpbId - fNrOfNdpbsA) );
         } // else of if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )

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

Bool_t CbmCern2017MonitorSts::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bCern2017ResetStsHistos )
   {
      ResetAllHistos();
      bCern2017ResetStsHistos = kFALSE;
   } // if( bCern2017ResetStsHistos )
   if( bCern2017WriteStsHistos )
   {
      SaveAllHistos( "data/StsHistos.root" );
      bCern2017WriteStsHistos = kFALSE;
   } // if( bCern2017WriteStsHistos )

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

   if( fuPulserMaxNbMicroslices < numCompMsInTs )
   {
      fuPulserMaxNbMicroslices = numCompMsInTs;

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
            fvuChanNbHitsInMs[ uXyterIdx ][ uChan ].resize( fuPulserMaxNbMicroslices );
            fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ].resize( fuPulserMaxNbMicroslices );
            fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ].resize( fuPulserMaxNbMicroslices );
            for( UInt_t uMsIdx = 0; uMsIdx < fuPulserMaxNbMicroslices; ++uMsIdx )
            {
               fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
               fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
               fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            } // for( UInt_t uMsIdx = 0; uMsIdx < fuPulserMaxNbMicroslices; ++uMsIdx )
         } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      LOG(info) << "CbmCern2017MonitorSts::DoUnpack => Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
      LOG(info) << "CbmCern2017MonitorSts::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
      LOG(info) << "CbmCern2017MonitorSts::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuPulserMaxNbMicroslices;
   } // if( fuPulserMaxNbMicroslices < numCompMsInTs )

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

      if( 0 == component && 0 == ( ( fulCurrentMsIdx / 1000000000 ) % 10 )
                         && 0 == ( ( fulCurrentMsIdx / 100000 ) ) %50000 )
      {
         for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
         {
            fhStsChanSameMsTimeDiff[uXyterIdx]->Reset();
            TH2* tempProj = fpStsChanSameMsTimeDiff[uXyterIdx]->ProjectionXY( );
            fhStsChanSameMsTimeDiff[uXyterIdx]->Add( tempProj );
            delete tempProj;
/*
            for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
               for( UInt_t uChanB = uChan + 1; uChanB < fuNbChanPerAsic; ++uChanB )
                  if( 0 < fpStsChanSameMsTimeDiff[uXyterIdx]->GetBinEntries( fpStsChanSameMsTimeDiff[uXyterIdx]->GetBin( uChan, uChanB ) ) )
                  {
                     LOG(debug) << "Microslice Index =  " << (fulCurrentMsIdx / 1000000000)
                                << "s, ASIC " << uXyterIdx << " Chan " << uChan << " VS " << uChanB
                                << " Entries " << fpStsChanSameMsTimeDiff[uXyterIdx]->GetBinEntries( fpStsChanSameMsTimeDiff[uXyterIdx]->GetBin( uChan, uChanB ) )
                                << " Mean time diff "<< fpStsChanSameMsTimeDiff[uXyterIdx]->GetBinContent( uChan, uChanB )
                                << " Error " << fpStsChanSameMsTimeDiff[uXyterIdx]->GetBinError( uChan, uChanB );
                  } // if( 0 < fpStsChanSameMsTimeDiff[uXyterIdx]->GetBinEntries( uChan, uChanB ) )
*/
         }  // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      } // if( 0 == ( ( fulCurrentMsIdx / 1e-9 ) % 10 )  )

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
                  LOG(fatal) << "CbmCern2017MonitorSts::DoUnpack => "
                             << "eLink index out of bounds!"
                             << usElinkIdx << " VS " << fuNbElinksPerDpb;
               } // if( fuNbElinksPerDpb <= usElinkIdx )
               fhStsMessTypePerElink->Fill( fuCurrDpbIdx * fuNbElinksPerDpb + usElinkIdx,
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
                  LOG(info) << "CbmCern2017MonitorSts::DoUnpack => "
                            << "EPOCH message at unexpected position in MS: message "
                            << uIdx << " VS message 0 expected!";
               break;
            } // case stsxyter::MessType::TsMsb :
            case stsxyter::MessType::Empty :
            {
//               FillTsMsbInfo( mess );
               break;
            } // case stsxyter::MessType::TsMsb :
            case stsxyter::MessType::Dummy :
            {
               break;
            } // case stsxyter::MessType::Dummy / ReadDataAck / Ack :
            default:
            {
               LOG(fatal) << "CbmCern2017MonitorSts::DoUnpack => "
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
      for( UInt_t uMsIdx = 0; uMsIdx < fuPulserMaxNbMicroslices; ++uMsIdx )
      {
/*
         if( kTRUE == fbPulserTimeDiffOn )
         {
            UInt_t uNbPulserChans = fvuPulserAsic.size();
            UInt_t uHistoIdx = 0;
            for( UInt_t uChA = 0; uChA < uNbPulserChans; ++uChA )
               for( UInt_t uChB = uChA + 1; uChB < uNbPulserChans; ++uChB )
               {
                  if( 0 < fvuChanNbHitsInMs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ][ uMsIdx ] &&
                      0 < fvuChanNbHitsInMs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ][ uMsIdx ] )
                  {
                     Double_t dTimeDiff = fvdChanLastHitTimeInMs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ][ uMsIdx ]
                                        - fvdChanLastHitTimeInMs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ][ uMsIdx ];
                     Short_t sAdcDiff = static_cast< Short_t >( fvusChanLastHitAdcInMs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ][ uMsIdx ] )
                                      - static_cast< Short_t >( fvusChanLastHitAdcInMs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ][ uMsIdx ] );

                     if( dTimeDiff < -102406.25 || 102406.25 < dTimeDiff )
                        LOG(info) << "CbmCern2017MonitorSts::DoUnpack =>"
                                  << " ASIC A" << fvuPulserAsic[uChA] << " chan A " << fvuPulserChan[uChA]
                                  << " tA " << fvdChanLastHitTimeInMs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ][ uMsIdx ]
                                  << " ASIC B" << fvuPulserAsic[uChB] << " chan B " << fvuPulserChan[uChB]
                                  << " tB " << fvdChanLastHitTimeInMs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ][ uMsIdx ]
                                  << " dt " << dTimeDiff;

                     if( dTimeDiff < -102406.25 )
                        dTimeDiff += 102406.25;
                     else if( 102406.25 < dTimeDiff )
                        dTimeDiff -= 102406.25;

                     fhStsPulserChansTimeDiff[ uHistoIdx ]->Fill( dTimeDiff );
                  } // Both pulser channels got data in last MS
                  uHistoIdx ++;
               } // Loop on channel pairs
         } // if( kTRUE == fbPulserTimeDiffOn )
*/
         for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
         {
            for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
            {
               fhStsChanHitsPerMs[uXyterIdx]->Fill( fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ], uChan );

               // Coincidences between
               if( fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] )
                  for( UInt_t uChanB = uChan + 1; uChanB < fuNbChanPerAsic; ++uChanB )
                     if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ][ uMsIdx ] )
                     {
                        fhStsChanSameMs[ uXyterIdx ]->Fill( uChan, uChanB );
                        Double_t dTimeDiff = fvdChanLastHitTimeInMs[ uXyterIdx ][ uChanB ][ uMsIdx ]
                                           - fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ];

                        fpStsChanSameMsTimeDiff[ uXyterIdx ]->Fill( uChan, uChanB, dTimeDiff );
                     } // if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ] )

/*
               // Pulser test: Coincidences between different FEEs (maybe on different DPBs)
               for( UInt_t uXyterIdxB = uXyterIdx; uXyterIdxB < fuNbStsXyters; ++uXyterIdxB )
               {
                  for( UInt_t uChanB = 0; uChanB < fuNbChanPerAsic; ++uChanB )
                  {
                  } // for( UInt_t uChanB = 0; uChanB < fuNbChanPerAsic; ++uChanB )
               } // for( UInt_t uXyterIdxB = uXyterIdx; uXyterIdxB < fuNbStsXyters; ++uXyterIdxB )
*/
               // Ok to reset as we only compare to channels with higher indices (or channels in ASICs with higher indices)
               fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
               fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
               fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
         } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      } // for( UInt_t uMsIdx = 0; uMsIdx < fuPulserMaxNbMicroslices; ++uMsIdx )

      if( kTRUE == fbPulserTimeDiffOn )
      {
         UInt_t uNbPulserChans = fvuPulserAsic.size();
         UInt_t uHistoIdx = 0;
         std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
         Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();
         for( UInt_t uChA = 0; uChA < uNbPulserChans; ++uChA )
            for( UInt_t uChB = uChA + 1; uChB < uNbPulserChans; ++uChB )
            {
               if( 0 < fvmChanHitsInTs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ].size() &&
                   0 < fvmChanHitsInTs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ].size() )
               {
                  std::multiset< stsxyter::FinalHit >::iterator itClosestHitB = fvmChanHitsInTs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ].begin();
                  for( std::multiset< stsxyter::FinalHit >::iterator itHitA = fvmChanHitsInTs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ].begin();
                       itHitA != fvmChanHitsInTs[ fvuPulserAsic[uChA] ][ fvuPulserChan[uChA] ].end();
                       ++itHitA )
                  {
                     Double_t dClosestTimeDiff = 1e9;
                     Bool_t bFoundClosest = kFALSE;

                     for( std::multiset< stsxyter::FinalHit >::iterator itHitB = itClosestHitB ;
                          itHitB != fvmChanHitsInTs[ fvuPulserAsic[uChB] ][ fvuPulserChan[uChB] ].end();
                          ++itHitB )
                     {
                        Double_t dTimeDiff = static_cast< Double_t >( (*itHitB).GetTs() )
                                           - static_cast< Double_t >( (*itHitA).GetTs() );

                        if( TMath::Abs( dTimeDiff ) <  TMath::Abs( dClosestTimeDiff ) )
                        {
                           dClosestTimeDiff = dTimeDiff;
                           itClosestHitB = itHitB;
                           bFoundClosest = kTRUE;
                        } // if( TMath::Abs( dTimeDiff ) <  TMath::Abs( dClosestTimeDiff ) )
                     } // Loop on hits in channel B, starting from match to previous channel A hit

                     if( kFALSE == bFoundClosest )
                        continue;

                     Double_t dTimeDiff = dClosestTimeDiff * stsxyter::kdClockCycleNs;
                     Short_t sAdcDiff = static_cast< Short_t >( (*itClosestHitB).GetAdc() )
                                      - static_cast< Short_t >( (*itHitA).GetAdc() );
/*
                     LOG(info) << "(  " << std::setw( 2 ) << fvuPulserAsic[uChA]
                               << ", " << std::setw( 3 ) << fvuPulserChan[uChA]
                               << ")-(  " << std::setw( 2 ) << fvuPulserAsic[uChB]
                               << ", " << std::setw( 3 ) << fvuPulserChan[uChB]
                               << ") ADC " << std::setw( 3 ) << sAdcDiff
                               << " dT " << std::setw( 7 )  << dTimeDiff
                               << " T_A " << std::setw( 12 ) << (*itHitA).GetTs()
                               << " T_B " << std::setw( 12 ) << (*itClosestHitB).GetTs();
*/
                     fhStsPulserChansTimeDiff[ uHistoIdx ]->Fill( dTimeDiff );
                     fhStsPulserChansTimeDiffAdc[ uHistoIdx ]->Fill( dTimeDiff, sAdcDiff );
                     if( fbLongHistoEnable )
                     {
//                        Double_t dTimeSinceStartSec = ( (*itHitA).GetTs() * stsxyter::kdClockCycleNs - fdStartTime )* 1e-9;
                        fhStsPulserChansTimeDiffEvo[ uHistoIdx ]->Fill( dUnixTimeInRun, dTimeDiff );
                     } // if( fbLongHistoEnable )
                  } // Loop on hits in channel A
               } // Both pulser channels got data in last TS
               uHistoIdx ++;
            } // Loop on channel pairs

         // Clean up the buffers now that we used the data
         for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
            for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
               fvmChanHitsInTs[ uXyterIdx ][ uChan ].clear();
      } // if( kTRUE == fbPulserTimeDiffOn )

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

void CbmCern2017MonitorSts::FillHitInfo( stsxyter::Message mess, const UShort_t & /*usElinkIdx*/, const UInt_t & uAsicIdx, const UInt_t & uMsIdx )
{
   UShort_t usChan   = mess.GetHitChannel();
   UShort_t usRawAdc = mess.GetHitAdc();
//   UShort_t usFullTs = mess.GetHitTimeFull();
//   UShort_t usTsOver = mess.GetHitTimeOver();
   UShort_t usRawTs  = mess.GetHitTime();

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

//         if( usRawTs < fvuElinkLastTsHit[fuCurrDpbIdx] )
//            fvulCurrentTsMsb[fuCurrDpbIdx]++;
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
   fvmChanHitsInTs[        uAsicIdx ][ usChan ].insert( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ], usRawAdc, uAsicIdx, usChan ) );

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

void CbmCern2017MonitorSts::FillTsMsbInfo( stsxyter::Message mess, UInt_t uMessIdx, UInt_t /*uMsIdx*/ )
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

void CbmCern2017MonitorSts::FillEpochInfo( stsxyter::Message /*mess*/ )
{
//   UShort_t usVal    = mess.GetTsMsbVal();
/*
   // Update Status counters
   if( usVal < fvulCurrentTsMsb[fuCurrDpbIdx] )
      fvuCurrentTsMsbCycle[fuCurrDpbIdx] ++;
   fvulCurrentTsMsb[fuCurrDpbIdx] = usVal;

//   fhStsAsicTsMsb->Fill( fvulCurrentTsMsb[fuCurrDpbIdx], uAsicIdx );
*/
}

void CbmCern2017MonitorSts::Reset()
{
}

void CbmCern2017MonitorSts::Finish()
{

   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmCern2017MonitorSts statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::MessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::MessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::MessType::Dummy ] << "\n"
             << " Epoch    messages: " << fmMsgCounter[ stsxyter::MessType::Epoch ] << "\n"
             << " Empty    messages: " << fmMsgCounter[ stsxyter::MessType::Empty ];

   LOG(info) << "-------------------------------------";

   SaveAllHistos();

}


void CbmCern2017MonitorSts::FillOutput(boost::any)
{
}

void CbmCern2017MonitorSts::SaveAllHistos( TString sFileName )
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
      fhStsChanSameMs[ uXyterIdx ]->Write();
      fpStsChanSameMsTimeDiff[ uXyterIdx ]->Write();
      fhStsChanSameMsTimeDiff[ uXyterIdx ]->Write();
      if( kTRUE == fbLongHistoEnable )
      {
         fhFebRateEvoLong[ uXyterIdx ]->Write();
         fhFebChRateEvoLong[ uXyterIdx ]->Write();
      } // if( kTRUE == fbLongHistoEnable )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   if( kTRUE == fbPulserTimeDiffOn )
   {
      UInt_t uNbPulserChans = fvuPulserAsic.size();
      UInt_t uHistoIdx = 0;
      for( UInt_t uChA = 0; uChA < uNbPulserChans; ++uChA )
         for( UInt_t uChB = uChA + 1; uChB < uNbPulserChans; ++uChB )
         {
            fhStsPulserChansTimeDiff[ uHistoIdx ]->Write();
            uHistoIdx ++;
         } // Loop on channel pairs
   } // if( kTRUE == fbPulserTimeDiffOn )

   fhStsAsicTsMsb->Write();
   fhStsAsicTsMsbMaj->Write();

   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fhStsElinkTsMsbCrc[ uDpb ]->Write();
      fhStsElinkTsMsbMaj[ uDpb ]->Write();
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

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
void CbmCern2017MonitorSts::ResetAllHistos()
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
      fhStsChanSameMs[ uXyterIdx ]->Reset();
      fpStsChanSameMsTimeDiff[ uXyterIdx ]->Reset();
      fhStsChanSameMsTimeDiff[ uXyterIdx ]->Reset();
      if( kTRUE == fbLongHistoEnable )
      {
         ftStartTimeUnix = std::chrono::steady_clock::now();
         fhFebRateEvoLong[ uXyterIdx ]->Reset();
         fhFebChRateEvoLong[ uXyterIdx ]->Reset();
      } // if( kTRUE == fbLongHistoEnable )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   if( kTRUE == fbPulserTimeDiffOn )
   {
      UInt_t uNbPulserChans = fvuPulserAsic.size();
      UInt_t uHistoIdx = 0;
      for( UInt_t uChA = 0; uChA < uNbPulserChans; ++uChA )
         for( UInt_t uChB = uChA + 1; uChB < uNbPulserChans; ++uChB )
         {
            fhStsPulserChansTimeDiff[ uHistoIdx ]->Reset();
            uHistoIdx ++;
         } // Loop on channel pairs
   } // if( kTRUE == fbPulserTimeDiffOn )

   fhStsAsicTsMsb->Reset();
   fhStsAsicTsMsbMaj->Reset();

   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fhStsElinkTsMsbCrc[ uDpb ]->Reset();
      fhStsElinkTsMsbMaj[ uDpb ]->Reset();
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

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

void CbmCern2017MonitorSts::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}
void CbmCern2017MonitorSts::SetPulserChannels( UInt_t uAsicA, UInt_t uChanA, UInt_t uAsicB, UInt_t uChanB,
                                               UInt_t uAsicC, UInt_t uChanC, UInt_t uAsicD, UInt_t uChanD,
                                               UInt_t uMaxNbMicroslices )
{
/*
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
   TString sHistName{""};
   TString title{""};
*/
   fuPulserMaxNbMicroslices = uMaxNbMicroslices;

   UInt_t uNbChans = 4;

   fvuPulserAsic.resize( uNbChans );
   fvuPulserChan.resize( uNbChans );
   fvuPulserAsic[0] = uAsicA;
   fvuPulserChan[0] = uChanA;
   fvuPulserAsic[1] = uAsicB;
   fvuPulserChan[1] = uChanB;
   fvuPulserAsic[2] = uAsicC;
   fvuPulserChan[2] = uChanC;
   fvuPulserAsic[3] = uAsicD;
   fvuPulserChan[3] = uChanD;
/*
   // Remove old histos
   if( kTRUE == fbPulserTimeDiffOn )
   {
      for( Int_t iIdx = 0; iIdx < fhStsPulserChansTimeDiff.size(); ++iIdx )
         delete fhStsPulserChansTimeDiff[ iIdx ];
   } // if( kTRUE == fbPulserTimeDiffOn )
   fhStsPulserChansTimeDiff.clear();

   for( UInt_t uChA = 0; uChA < uNbChans; ++uChA )
      for( UInt_t uChB = uChA + 1; uChB < uNbChans; ++uChB )
      {
         sHistName = Form( "hStsPulserTimeDiff_%02u_%03u_%02u_%03u",
                           fvuPulserAsic[uChA], fvuPulserChan[uChA],
                           fvuPulserAsic[uChB], fvuPulserChan[uChB] );
         title = "Time diff for hits in same MS from ASIC %02d ch %03d and ASIC %02d ch %03d; tB - tA [ns]; Counts";
         TH1 * pHist = new TH1I(sHistName, title, 16385, -51203.125, 51203.125);

         fHM->Add(sHistName.Data(), pHist);
         if( server ) server->Register("/StsRaw", pHist );
         fhStsPulserChansTimeDiff.push_back( pHist );
      } // Loop on pairs of channels
*/
   LOG(info) << "Set pulser channels to: "
             << "( " << fvuPulserAsic[0] << "' " << fvuPulserChan[0] <<") "
             << "( " << fvuPulserAsic[1] << "' " << fvuPulserChan[1] <<") "
             << "( " << fvuPulserAsic[2] << "' " << fvuPulserChan[2] <<") "
             << "( " << fvuPulserAsic[3] << "' " << fvuPulserChan[3] <<") ";

   fbPulserTimeDiffOn = kTRUE;
}

void CbmCern2017MonitorSts::SetLongDurationLimits( UInt_t uDurationSeconds, UInt_t uBinSize )
{
   fbLongHistoEnable     = kTRUE;
   fuLongHistoNbSeconds  = uDurationSeconds;
   fuLongHistoBinSizeSec = uBinSize;
}

ClassImp(CbmCern2017MonitorSts)
