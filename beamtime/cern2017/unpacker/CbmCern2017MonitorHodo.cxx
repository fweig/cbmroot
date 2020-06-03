// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCern2017MonitorHodo                        -----
// -----                Created 14/12/17  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCern2017MonitorHodo.h"

// Data

// CbmRoot
#include "CbmCern2017UnpackParHodo.h"
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

Bool_t bCern2017ResetHodoHistos = kFALSE;
Bool_t bCern2017WriteHodoHistos = kFALSE;

CbmCern2017MonitorHodo::CbmCern2017MonitorHodo() :
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
   fPrintMessCtrl( stsxyter::BetaMessagePrintMask::msg_print_Human ),
   fmMsgCounter(),
   fuCurrentEquipmentId(0),
   fuCurrDpbId(0),
   fuCurrDpbIdx(0),
   fiRunStartDateTimeSec(-1),
   fiBinSizeDatePlots(-1),
   fvuCurrentTsMsb(),
   fvuCurrentTsMsbCycle(),
   fvuCurrentTsMsbOver(),
   fvulChanLastHitTime(),
   fvdChanLastHitTime(),
   fvdMsTime(),
   fvuChanNbHitsInMs(),
   fvdChanLastHitTimeInMs(),
   fvusChanLastHitAdcInMs(),
   fvmChanHitsInTs(),
   fdStartTime(-1.0),
   fdStartTimeMsSz(-1.0),
   ftStartTimeUnix( std::chrono::steady_clock::now() ),
   fbBetaFormat( kFALSE ),
   fvuElinkLastTsHit(),
   fvmHitsInTs(),
   fLastSortedHit1X(),
   fLastSortedHit1Y(),
   fLastSortedHit2X(),
   fLastSortedHit2Y(),
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
   fhHodoChanAdcRaw(),
   fhHodoChanAdcRawProf(),
   fhHodoChanRawTs(),
   fhHodoChanMissEvt(),
   fhHodoChanHitRateEvo(),
   fhHodoFebRateEvo(),
   fhHodoChanHitRateEvoLong(),
   fhHodoFebRateEvoLong(),
   fhHodoChanCounts1X(NULL),
   fhHodoChanCounts1Y(NULL),
   fhHodoChanCounts2X(NULL),
   fhHodoChanCounts2Y(NULL),
   fhHodoChanAdcRaw1X(NULL),
   fhHodoChanAdcRaw1Y(NULL),
   fhHodoChanAdcRaw2X(NULL),
   fhHodoChanAdcRaw2Y(NULL),
   fhHodoChanHitRateEvo1X(NULL),
   fhHodoChanHitRateEvo1Y(NULL),
   fhHodoChanHitRateEvo2X(NULL),
   fhHodoChanHitRateEvo2Y(NULL),
   fhHodoRateEvo1X(NULL),
   fhHodoRateEvo1Y(NULL),
   fhHodoRateEvo2X(NULL),
   fhHodoRateEvo2Y(NULL),
   fhHodoSameMs1XY(NULL),
   fhHodoSameMs2XY(NULL),
   fhHodoSameMsX1X2(NULL),
   fhHodoSameMsY1Y2(NULL),
   fhHodoSameMsX1Y2(NULL),
   fhHodoSameMsY1X2(NULL),
   fhHodoSameMsCntEvoX1Y1(NULL),
   fhHodoSameMsCntEvoX2Y2(NULL),
   fhHodoSameMsCntEvoX1X2(NULL),
   fhHodoSameMsCntEvoY1Y2(NULL),
   fhHodoSameMsCntEvoX1Y2(NULL),
   fhHodoSameMsCntEvoY1X2(NULL),
   fhHodoSameMsCntEvoX1Y1X2Y2(NULL),
   fhHodoSortedDtX1Y1(NULL),
   fhHodoSortedDtX2Y2(NULL),
   fhHodoSortedDtX1X2(NULL),
   fhHodoSortedDtY1Y2(NULL),
   fhHodoSortedDtX1Y2(NULL),
   fhHodoSortedDtY1X2(NULL),
   fhHodoSortedMapX1Y1(NULL),
   fhHodoSortedMapX2Y2(NULL),
   fhHodoSortedMapX1X2(NULL),
   fhHodoSortedMapY1Y2(NULL),
   fhHodoSortedMapX1Y2(NULL),
   fhHodoSortedMapY1X2(NULL),
   fhHodoFebTsMsb(NULL),
   fcMsSizeAll(NULL)
{
}

CbmCern2017MonitorHodo::~CbmCern2017MonitorHodo()
{
}

Bool_t CbmCern2017MonitorHodo::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmCern2017MonitorHodo::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackPar = (CbmCern2017UnpackParHodo*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParHodo"));
}


Bool_t CbmCern2017MonitorHodo::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmCern2017MonitorHodo::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();

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

   LOG(info) << "ASCIC Idx for Hodo 1: " << fUnpackPar->GetAsicIndexHodo1();
   LOG(info) << "ASCIC Idx for Hodo 2: " << fUnpackPar->GetAsicIndexHodo2();

   LOG(info) << "XY swapped in Hodo 1: " << fUnpackPar->IsXySwappedHodo1();
   LOG(info) << "XY swapped in Hodo 2: " << fUnpackPar->IsXySwappedHodo2();

   LOG(info) << "X axis inverted in Hodo 1: " << fUnpackPar->IsXInvertedHodo1();
   LOG(info) << "X axis inverted in Hodo 1: " << fUnpackPar->IsYInvertedHodo1();
   LOG(info) << "X axis inverted in Hodo 2: " << fUnpackPar->IsXInvertedHodo2();
   LOG(info) << "X axis inverted in Hodo 2: " << fUnpackPar->IsYInvertedHodo2();

   // Internal status initialization
   fvuCurrentTsMsb.resize( fuNrOfDpbs );
   fvuCurrentTsMsbCycle.resize( fuNrOfDpbs );
   fvuCurrentTsMsbOver.resize( fuNrOfDpbs );
   fvuElinkLastTsHit.resize( fuNrOfDpbs );
   for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
   {
      fvuCurrentTsMsb[uDpb].resize( fuNbElinksPerDpb );
      fvuCurrentTsMsbCycle[uDpb].resize( fuNbElinksPerDpb );
      fvuCurrentTsMsbOver[uDpb].resize( fuNbElinksPerDpb );
      fvuElinkLastTsHit[uDpb].resize( fuNbElinksPerDpb );
      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      {
         fvuCurrentTsMsb[uDpb][uElink]      = 0;
         fvuCurrentTsMsbCycle[uDpb][uElink] = 0;
         fvuCurrentTsMsbOver[uDpb][uElink]  = 0;
         fvuElinkLastTsHit[uDpb][uElink]  = 0;
      } // for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   fvulChanLastHitTime.resize( fuNbStsXyters );
   fvdChanLastHitTime.resize( fuNbStsXyters );
   fvdMsTime.resize( fuMaxNbMicroslices );
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

         fvuChanNbHitsInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
         fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
         fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ].resize( fuMaxNbMicroslices );
         fvmChanHitsInTs[ uXyterIdx ][ uChan ].clear();
         for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
         {
            fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
            fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
         } // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
      } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   LOG(info) << "CbmCern2017MonitorHodo::ReInitContainers => Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
   LOG(info) << "CbmCern2017MonitorHodo::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
   LOG(info) << "CbmCern2017MonitorHodo::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuMaxNbMicroslices;
   return kTRUE;
}

void CbmCern2017MonitorHodo::CreateHistograms()
{
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   TString sHistName{""};
   TString title{""};

   sHistName = "hStsMessageType";
   title = "Nb of message for each type; Type";
   fhHodoMessType = new TH1I(sHistName, title, 5, 0., 5.);
   fhHodoMessType->GetXaxis()->SetBinLabel( 1,       "Dummy");
   fhHodoMessType->GetXaxis()->SetBinLabel( 2,         "Hit");
   fhHodoMessType->GetXaxis()->SetBinLabel( 3,       "TsMsb");
   fhHodoMessType->GetXaxis()->SetBinLabel( 4, "ReadDataAck");
   fhHodoMessType->GetXaxis()->SetBinLabel( 5,         "Ack");
/* *** Missing int + MessType OP!!!! ****
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Dummy,       "Dummy");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Hit,         "Hit");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::BetaMessType::TsMsb,       "TsMsb");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::BetaMessType::ReadDataAck, "ReadDataAck");
   fhHodoMessType->GetXaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Ack,         "Ack");
*/
   fHM->Add(sHistName.Data(), fhHodoMessType);
   if( server ) server->Register("/StsRaw", fhHodoMessType );

   sHistName = "hStsSysMessType";
   title = "Nb of system message for each type; System Type";
   fhHodoSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
/*
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhHodoSysMessType);
   if( server ) server->Register("/StsRaw", fhHodoSysMessType );

   sHistName = "hStsMessageTypePerDpb";
   title = "Nb of message of each type for each DPB; DPB; Type";
   fhHodoMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 5, 0., 5.);
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 1,       "Dummy");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 2,         "Hit");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 3,       "TsMsb");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 4, "ReadDataAck");
   fhHodoMessTypePerDpb->GetYaxis()->SetBinLabel( 5,         "Ack");
/* *** Missing int + MessType OP!!!! ****
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Dummy,       "Dummy");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Hit,         "Hit");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::TsMsb,       "TsMsb");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::ReadDataAck, "ReadDataAck");
   fhHodoMessType->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Ack,         "Ack");
*/
   fHM->Add(sHistName.Data(), fhHodoMessTypePerDpb);
   if( server ) server->Register("/StsRaw", fhHodoMessTypePerDpb );

   sHistName = "hStsSysMessTypePerDpb";
   title = "Nb of system message of each type for each DPB; DPB; System Type";
   fhHodoSysMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);
/*
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhHodoSysMessTypePerDpb);
   if( server ) server->Register("/StsRaw", fhHodoSysMessTypePerDpb );

   sHistName = "hStsMessageTypePerElink";
   title = "Nb of message of each type for each eLink; eLink; Type";
   fhHodoMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 5, 0., 5.);
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 1,       "Dummy");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 2,         "Hit");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 3,       "TsMsb");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 4, "ReadDataAck");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel( 5,         "Ack");
/* *** Missing int + MessType OP!!!! ****
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Dummy,       "Dummy");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Hit,         "Hit");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::TsMsb,       "TsMsb");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::ReadDataAck, "ReadDataAck");
   fhHodoMessTypePerElink->GetYaxis()->SetBinLabel(1 + stsxyter::BetaMessType::Ack,         "Ack");
*/
   fHM->Add(sHistName.Data(), fhHodoMessTypePerElink);
   if( server ) server->Register("/StsRaw", fhHodoMessTypePerElink );

   sHistName = "hStsSysMessTypePerElink";
   title = "Nb of system message of each type for each eLink; eLink; System Type";
   fhHodoSysMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 17, 0., 17.);
/*
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhHodoSysMessTypePerElink);
   if( server ) server->Register("/StsRaw", fhHodoSysMessTypePerElink );

/*
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
*/
   for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
   {
      // Channel counts
      sHistName = Form( "hHodoChanCntRaw_%03u", uXyterIdx );
      title = Form( "Hits Count per channel, StsXyter #%03u; Channel; Hits []", uXyterIdx );
      fhHodoChanCntRaw.push_back( new TH1I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanCntRaw[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanCntRaw[ uXyterIdx ] );

      // Raw Adc Distribution
      sHistName = Form( "hHodoChanAdcRaw_%03u", uXyterIdx );
      title = Form( "Raw Adc distribution per channel, StsXyter #%03u; Channel []; Adc []; Hits []", uXyterIdx );
      fhHodoChanAdcRaw.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuBetaHitNbAdcBins, -0.5, stsxyter::kuBetaHitNbAdcBins -0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanAdcRaw[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanAdcRaw[ uXyterIdx ] );

      // Raw Adc Distribution profile
      sHistName = Form( "hHodoChanAdcRawProfc_%03u", uXyterIdx );
      title = Form( "Raw Adc prodile per channel, StsXyter #%03u; Channel []; Adc []", uXyterIdx );
      fhHodoChanAdcRawProf.push_back( new TProfile(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanAdcRawProf[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanAdcRawProf[ uXyterIdx ] );

      // Raw Ts Distribution
      sHistName = Form( "hHodoChanRawTs_%03u", uXyterIdx );
      title = Form( "Raw Timestamp distribution per channel, StsXyter #%03u; Channel []; Ts []; Hits []", uXyterIdx );
      fhHodoChanRawTs.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuBetaHitNbTsBins, -0.5, stsxyter::kuBetaHitNbTsBins -0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanRawTs[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanRawTs[ uXyterIdx ] );

      // Missed event flag
      sHistName = Form( "hHodoChanMissEvt_%03u", uXyterIdx );
      title = Form( "Missed Event flags per channel, StsXyter #%03u; Channel []; Miss Evt []; Hits []", uXyterIdx );
      fhHodoChanMissEvt.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 2, -0.5, 1.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanMissEvt[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanMissEvt[ uXyterIdx ] );

      // Hit rates evo per channel
      sHistName = Form( "hStsChanRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [s]; Channel []; Hits []", uXyterIdx );
      fhHodoChanHitRateEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanHitRateEvo[ uXyterIdx ] );

      // Hit rates evo per StsXyter
      sHistName = Form( "hHodoFebRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [s]; Hits []", uXyterIdx );
      fhHodoFebRateEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );
      fHM->Add(sHistName.Data(), fhHodoFebRateEvo[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoFebRateEvo[ uXyterIdx ] );

      // Hit rates evo per channel, 1 minute bins, 24h
      sHistName = Form( "hStsChanRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [min]; Channel []; Hits []", uXyterIdx );
      fhHodoChanHitRateEvoLong.push_back( new TH2D( sHistName, title,
                                                1440, 0, 1440,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanHitRateEvoLong[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoChanHitRateEvoLong[ uXyterIdx ] );

      // Hit rates evo per StsXyter, 1 minute bins, 24h
      sHistName = Form( "hHodoFebRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [min]; Hits []", uXyterIdx );
      fhHodoFebRateEvoLong.push_back( new TH1D(sHistName, title, 1440, 0, 1440 ) );
      fHM->Add(sHistName.Data(), fhHodoFebRateEvoLong[ uXyterIdx ] );
      if( server ) server->Register("/StsRaw", fhHodoFebRateEvoLong[ uXyterIdx ] );
/*
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
*/
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

   // Channel counts for each of the hodoscope planes
   sHistName = "hHodoChanCounts1X";
   title = "Channel counts for hodoscope 1 axis X; X channel []; Hits []";
   fhHodoChanCounts1X = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts1X );
   if( server ) server->Register("/StsRaw", fhHodoChanCounts1X );
   sHistName = "hHodoChanCounts1Y";
   title = "Channel counts for hodoscope 1 axis Y; Y channel []; Hits []";
   fhHodoChanCounts1Y = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts1Y );
   if( server ) server->Register("/StsRaw", fhHodoChanCounts1Y );
   sHistName = "hHodoChanCounts2X";
   title = "Channel counts for hodoscope 2 axis X; X channel []; Hits []";
   fhHodoChanCounts2X = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts2X );
   if( server ) server->Register("/StsRaw", fhHodoChanCounts2X );
   sHistName = "hHodoChanCounts2Y";
   title = "Channel counts for hodoscope 2 axis Y; Y channel []; Hits []";
   fhHodoChanCounts2Y = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts2Y );
   if( server ) server->Register("/StsRaw", fhHodoChanCounts2Y );

   // Raw ADC distributions for each of the hodoscope planes
   sHistName = "hHodoChanAdcRaw1X";
   title = "Raw ADC distributions for hodoscope 1 axis X; X channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw1X = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuBetaHitNbAdcBins, -0.5, stsxyter::kuBetaHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw1X );
   if( server ) server->Register("/StsRaw", fhHodoChanAdcRaw1X );
   sHistName = "hHodoChanAdcRaw1Y";
   title = "Raw ADC distributions for hodoscope 1 axis Y; Y channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw1Y = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuBetaHitNbAdcBins, -0.5, stsxyter::kuBetaHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw1Y );
   if( server ) server->Register("/StsRaw", fhHodoChanAdcRaw1Y );
   sHistName = "hHodoChanAdcRaw2X";
   title = "Raw ADC distributions for hodoscope 2 axis X; X channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw2X = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuBetaHitNbAdcBins, -0.5, stsxyter::kuBetaHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw2X );
   if( server ) server->Register("/StsRaw", fhHodoChanAdcRaw2X );
   sHistName = "hHodoChanAdcRaw2Y";
   title = "Raw ADC distributions for hodoscope 2 axis Y; Y channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw2Y = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuBetaHitNbAdcBins, -0.5, stsxyter::kuBetaHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw2Y );
   if( server ) server->Register("/StsRaw", fhHodoChanAdcRaw2Y );

   // Hit rate Evo for each channel of each of the hodoscope planes
   sHistName = "hHodoChanHitRateEvo1X";
   title = "Hits per second & channel in Hodo 1 X axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo1X = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo1X );
   if( server ) server->Register("/StsRaw", fhHodoChanHitRateEvo1X );
   sHistName = "hHodoChanHitRateEvo1Y";
   title = "Hits per second & channel in Hodo 1 Y axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo1Y = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo1Y );
   if( server ) server->Register("/StsRaw", fhHodoChanHitRateEvo1Y );
   sHistName = "hHodoChanHitRateEvo2X";
   title = "Hits per second & channel in Hodo 2 X axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo2X = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo2X );
   if( server ) server->Register("/StsRaw", fhHodoChanHitRateEvo2X );
   sHistName = "hHodoChanHitRateEvo2Y";
   title = "Hits per second & channel in Hodo 2 Y axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo2Y = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo2Y );
   if( server ) server->Register("/StsRaw", fhHodoChanHitRateEvo2Y );

   // Hit rate Evo for each channel of each of the hodoscope planes
   sHistName = "fhHodoRateEvo1X";
   title = "Hits per second in Hodoscope 1 X axis; Time [s]; Hits []";
   fhHodoRateEvo1X = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo1X );
   if( server ) server->Register("/StsRaw", fhHodoRateEvo1X );
   sHistName = "hHodoRateEvo1Y";
   title = "Hits per second in Hodoscope 1 Y axis; Time [s]; Hits []";
   fhHodoRateEvo1Y = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo1Y );
   if( server ) server->Register("/StsRaw", fhHodoRateEvo1Y );
   sHistName = "fhHodoRateEvo2X";
   title = "Hits per second in Hodoscope 2 X axis; Time [s]; Hits []";
   fhHodoRateEvo2X = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo2X );
   if( server ) server->Register("/StsRaw", fhHodoRateEvo2X );
   sHistName = "hHodoRateEvo2Y";
   title = "Hits per second in Hodoscope 2 Y axis; Time [s]; Hits []";
   fhHodoRateEvo2Y = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo2Y );
   if( server ) server->Register("/StsRaw", fhHodoRateEvo2Y );

   // Coincidence map for each hodoscope
   sHistName = "hHodoSameMs1XY";
   title = "MS with hits in both channels for hodoscope 1 axis X and Y; X channel []; Y channel []; MS []";
   fhHodoSameMs1XY = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSameMs1XY );
   if( server ) server->Register("/StsRaw", fhHodoSameMs1XY );
   sHistName = "hHodoSameMs2XY";
   title = "MS with hits in both channels for hodoscope 2 axis X and Y; X channel []; Y channel []; MS []";
   fhHodoSameMs2XY = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSameMs2XY );
   if( server ) server->Register("/StsRaw", fhHodoSameMs2XY );

   // Coincidence map between some axis of the hodoscopes
   sHistName = "hHodoSameMsX1X2";
   title = "MS with hits in both channels for hodoscope 1 and 2 axis X; X channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSameMsX1X2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSameMsX1X2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsX1X2 );
   sHistName = "fhHodoSameMsY1Y2";
   title = "MS with hits in both channels for hodoscope 1 and 2 axis Y; Y channel Hodo 1 []; Y channel Hodo 2 []; MS []";
   fhHodoSameMsY1Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSameMsY1Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsY1Y2 );
   sHistName = "hHodoSameMsX1Y2";
   title = "MS with hits in both channels for hodoscope 1 axis X and 2 axis Y; X channel Hodo 1 []; Y channel Hodo 2 []; MS []";
   fhHodoSameMsX1Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSameMsX1Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsX1Y2 );
   sHistName = "fhHodoSameMsY1X2";
   title = "MS with hits in both channels for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSameMsY1X2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSameMsY1X2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsY1X2 );

   // Coincidence counts evolution between some axis of the hodoscopes
   sHistName = "hHodoSameMsCntEvoX1Y1";
   title = "Nb of MS with hits in both X1 and Y1 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoX1Y1 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoX1Y1 );

   sHistName = "hHodoSameMsCntEvoX2Y2";
   title = "Nb of MS with hits in both X2 and Y2 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoX2Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoX2Y2 );

   sHistName = "fhHodoSameMsCntEvoX1X2";
   title = "Nb of MS with hits in both X1 and X2 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoX1X2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoX1X2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoX1X2 );

   sHistName = "fhHodoSameMsCntEvoY1Y2";
   title = "Nb of MS with hits in both Y1 and Y2 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoY1Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoY1Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoY1Y2 );

   sHistName = "hHodoSameMsCntEvoX1Y2";
   title = "Nb of MS with hits in both X1 and Y2 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoX1Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoX1Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoX1Y2 );

   sHistName = "hHodoSameMsCntEvoY1X2";
   title = "Nb of MS with hits in both Y1 and X2 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoY1X2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoY1X2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoY1X2 );

   sHistName = "hHodoSameMsCntEvoX1Y1X2Y2";
   title = "Nb of MS with hits in both X1, Y1, X2 and Y2 per s; Time [s]; MS with both []";
   fhHodoSameMsCntEvoX1Y1X2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSameMsCntEvoX1Y1X2Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSameMsCntEvoX1Y1X2Y2 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   UInt_t uNbBinEvo = 32768 + 1;
   Double_t dMaxEdgeEvo = stsxyter::kdBetaClockCycleNs
                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

   sHistName = "fhHodoSortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhHodoSortedDtX1Y1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX1Y1);
   if( server ) server->Register("/StsRaw", fhHodoSortedDtX1Y1 );

   sHistName = "fhHodoSortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhHodoSortedDtX2Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX2Y2);
   if( server ) server->Register("/StsRaw", fhHodoSortedDtX2Y2 );

   sHistName = "fhHodoSortedDtX1X2";
   title =  "Time diff for hits Hodo 1 X and Hodo 2 X; tX2 - tX1 [ns]; Counts";
   fhHodoSortedDtX1X2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX1X2);
   if( server ) server->Register("/StsRaw", fhHodoSortedDtX1X2 );

   sHistName = "fhHodoSortedDtY1Y2";
   title =  "Time diff for hits Hodo 1 Y and Hodo 2 Y; tY2 - tY1 [ns]; Counts";
   fhHodoSortedDtY1Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtY1Y2);
   if( server ) server->Register("/StsRaw", fhHodoSortedDtY1Y2 );

   sHistName = "fhHodoSortedDtX1Y2";
   title =  "Time diff for hits Hodo 1 X and Hodo 2 Y; tY2 - tX1 [ns]; Counts";
   fhHodoSortedDtX1Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX1Y2);
   if( server ) server->Register("/StsRaw", fhHodoSortedDtX1Y2 );

   sHistName = "fhHodoSortedDtY1X2";
   title =  "Time diff for hits Hodo 1 Y and Hodo 2 X; tY2 - tY1 [ns]; Counts";
   fhHodoSortedDtY1X2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtY1X2);
   if( server ) server->Register("/StsRaw", fhHodoSortedDtY1X2 );

   sHistName = "fhHodoSortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX1Y1 );
   if( server ) server->Register("/StsRaw", fhHodoSortedMapX1Y1 );

   sHistName = "fhHodoSortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX2Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSortedMapX2Y2 );

   sHistName = "fhHodoSortedMapX1X2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapX1X2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX1X2 );
   if( server ) server->Register("/StsRaw", fhHodoSortedMapX1X2 );

   sHistName = "fhHodoSortedMapY1Y2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapY1Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapY1Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSortedMapY1Y2 );

   sHistName = "fhHodoSortedMapX1Y2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapX1Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX1Y2 );
   if( server ) server->Register("/StsRaw", fhHodoSortedMapX1Y2 );

   sHistName = "fhHodoSortedMapY1X2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapY1X2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapY1X2 );
   if( server ) server->Register("/StsRaw", fhHodoSortedMapY1X2 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   // Distribution of the TS_MSB per StsXyter
   sHistName = "hHodoFebTsMsb";
   title = "Raw Timestamp Msb distribution per StsXyter; Ts MSB []; StsXyter []; Hits []";
   fhHodoFebTsMsb = new TH2I( sHistName, title, stsxyter::kuBetaTsMsbNbTsBins, -0.5,   stsxyter::kuBetaTsMsbNbTsBins - 0.5,
                                                fuNbStsXyters, -0.5, fuNbStsXyters - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoFebTsMsb );
   if( server ) server->Register("/StsRaw", fhHodoFebTsMsb );

   // Miscroslice properties histos
   for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )
   {
      fhMsSz[ component ] = NULL;
      fhMsSzTime[ component ] = NULL;
   } // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )

   // Online histo browser commands
   if( server )
   {
      server->RegisterCommand("/Reset_All_Hodo", "bCern2017ResetHodoHistos=kTRUE");
      server->RegisterCommand("/Write_All_Hodo", "bCern2017WriteHodoHistos=kTRUE");

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
      fhHodoChanRawTs[ uXyterIdx ]->Draw( "colz" );

      cStsSumm->cd(4);
      gPad->SetLogy();
      fhHodoFebRateEvo[ uXyterIdx ]->Draw();
   } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

//====================================================================//
   TCanvas* cHodoCounts = new TCanvas( "cHodoCounts",
                                    "Hodoscopes counts per axis",
                                    w, h);
   cHodoCounts->Divide( 2, 2 );

   cHodoCounts->cd(1);
   gPad->SetLogz();
   fhHodoChanCounts1X->Draw( "" );

   cHodoCounts->cd(2);
   gPad->SetLogz();
   fhHodoChanCounts1Y->Draw( "" );

   cHodoCounts->cd(3);
   gPad->SetLogz();
   fhHodoChanCounts2X->Draw( "" );

   cHodoCounts->cd(4);
   gPad->SetLogz();
   fhHodoChanCounts2Y->Draw( "" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoAdc = new TCanvas( "cHodoAdc",
                                    "Hodoscopes ADC distributions per axis",
                                    w, h);
   cHodoAdc->Divide( 2, 2 );

   cHodoAdc->cd(1);
   gPad->SetLogz();
   fhHodoChanAdcRaw1X->Draw( "colz" );

   cHodoAdc->cd(2);
   gPad->SetLogz();
   fhHodoChanAdcRaw1Y->Draw( "colz" );

   cHodoAdc->cd(3);
   gPad->SetLogz();
   fhHodoChanAdcRaw2X->Draw( "colz" );

   cHodoAdc->cd(4);
   gPad->SetLogz();
   fhHodoChanAdcRaw2Y->Draw( "colz" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoRates = new TCanvas( "cHodoRates",
                                    "Hodoscopes rates per axis",
                                    w, h);
   cHodoRates->Divide( 2, 2 );

   cHodoRates->cd(1);
   gPad->SetLogz();
   fhHodoRateEvo1X->Draw( "" );

   cHodoRates->cd(2);
   gPad->SetLogz();
   fhHodoRateEvo1Y->Draw( "" );

   cHodoRates->cd(3);
   gPad->SetLogz();
   fhHodoRateEvo2X->Draw( "" );

   cHodoRates->cd(4);
   gPad->SetLogz();
   fhHodoRateEvo2Y->Draw( "" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoRatesEvo = new TCanvas( "cHodoRatesEvo",
                                    "Hodoscopes rates evolutionper axis",
                                    w, h);
   cHodoRatesEvo->Divide( 2, 2 );

   cHodoRatesEvo->cd(1);
   gPad->SetLogz();
   fhHodoChanHitRateEvo1X->Draw( "colz" );

   cHodoRatesEvo->cd(2);
   gPad->SetLogz();
   fhHodoChanHitRateEvo1Y->Draw( "colz" );

   cHodoRatesEvo->cd(3);
   gPad->SetLogz();
   fhHodoChanHitRateEvo2X->Draw( "colz" );

   cHodoRatesEvo->cd(4);
   gPad->SetLogz();
   fhHodoChanHitRateEvo2Y->Draw( "colz" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoMaps = new TCanvas( "cHodoMaps",
                                    "Hodoscopes coincidence maps",
                                    w, h);
   cHodoMaps->Divide( 2, 3 );

   cHodoMaps->cd(1);
   gPad->SetLogz();
   fhHodoSameMs1XY->Draw( "colz" );

   cHodoMaps->cd(2);
   gPad->SetLogz();
   fhHodoSameMs2XY->Draw( "colz" );

   cHodoMaps->cd(3);
   gPad->SetLogz();
   fhHodoSameMsX1X2->Draw( "colz" );

   cHodoMaps->cd(4);
   gPad->SetLogz();
   fhHodoSameMsY1Y2->Draw( "colz" );

   cHodoMaps->cd(5);
   gPad->SetLogz();
   fhHodoSameMsX1Y2->Draw( "colz" );

   cHodoMaps->cd(6);
   gPad->SetLogz();
   fhHodoSameMsY1X2->Draw( "colz" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoCoincEvo = new TCanvas( "cHodoCoincEvo",
                                    "Hodoscopes coincidence rate evolution",
                                    w, h);
   cHodoCoincEvo->Divide( 2, 4 );

   cHodoCoincEvo->cd(1);
   gPad->SetLogz();
   fhHodoSameMsCntEvoX1Y1->Draw( "colz" );

   cHodoCoincEvo->cd(2);
   gPad->SetLogz();
   fhHodoSameMsCntEvoX2Y2->Draw( "colz" );

   cHodoCoincEvo->cd(3);
   gPad->SetLogz();
   fhHodoSameMsCntEvoX1X2->Draw( "colz" );

   cHodoCoincEvo->cd(4);
   gPad->SetLogz();
   fhHodoSameMsCntEvoY1Y2->Draw( "colz" );

   cHodoCoincEvo->cd(5);
   gPad->SetLogz();
   fhHodoSameMsCntEvoX1Y2->Draw( "colz" );

   cHodoCoincEvo->cd(6);
   gPad->SetLogz();
   fhHodoSameMsCntEvoY1X2->Draw( "colz" );

   cHodoCoincEvo->cd(7);
   gPad->SetLogz();
   fhHodoSameMsCntEvoX1Y1X2Y2->Draw( "colz" );
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

Bool_t CbmCern2017MonitorHodo::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bCern2017ResetHodoHistos )
   {
      ResetAllHistos();
      bCern2017ResetHodoHistos = kFALSE;
   } // if( bCern2017ResetHodoHistos )
   if( bCern2017WriteHodoHistos )
   {
      SaveAllHistos( "data/HodoHistos.root" );
      bCern2017WriteHodoHistos = kFALSE;
   } // if( bCern2017WriteHodoHistos )

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
      LOG(info) << "CbmCern2017MonitorHodo::DoUnpack => Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
      LOG(info) << "CbmCern2017MonitorHodo::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
      LOG(info) << "CbmCern2017MonitorHodo::DoUnpack =>  Changed fvuChanNbHitsInMs size "
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
      Double_t dMsTime = (1e-9) * static_cast<double>(msDescriptor.idx);

      uint32_t size = msDescriptor.size;
      LOG(debug) << "Microslice: " << msDescriptor.idx
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

         stsxyter::BetaMessage mess( static_cast< uint32_t >( ulData & 0xFFFFFFFF ) );

         // Print message if requested
         if( fbPrintMessages )
            mess.PrintMess( std::cout, fPrintMessCtrl );

         // Extract the eLink and Asic indices
         UShort_t usElinkIdx = mess.GetLinkIndex();
         if( fuNbElinksPerDpb <= usElinkIdx )
         {
            LOG(fatal) << "CbmCern2017MonitorHodo::DoUnpack => "
                       << "eLink index out of bounds!";
         } // if( fuNbElinksPerDpb <= usElinkIdx )
         UInt_t   uAsicIdx   = fvuElinkToAsic[fuCurrDpbIdx][usElinkIdx];

         stsxyter::BetaMessType typeMess = mess.GetMessType();
         fmMsgCounter[ typeMess ] ++;
         fhHodoMessType->Fill( static_cast< uint16_t > (typeMess) );
         fhHodoMessTypePerDpb->Fill( fuCurrDpbIdx, static_cast< uint16_t > (typeMess) );
         fhHodoMessTypePerElink->Fill( fuCurrDpbIdx * fuNbElinksPerDpb + usElinkIdx,
                                      static_cast< uint16_t > (typeMess) );

         switch( typeMess )
         {
            case stsxyter::BetaMessType::Hit :
            {
               FillHitInfo( mess, usElinkIdx, uAsicIdx, m );
               break;
            } // case stsxyter::BetaMessType::Hit :
            case stsxyter::BetaMessType::TsMsb :
            {
               FillTsMsbInfo( mess, usElinkIdx, uAsicIdx );
               break;
            } // case stsxyter::BetaMessType::TsMsb :
            case stsxyter::BetaMessType::Dummy :
            {
               if( kTRUE == fbBetaFormat )
               {
                  // In beta data format, a dummy hit indicates a TS overflow => TS_MSB increase by 1
                  fvuCurrentTsMsb[fuCurrDpbIdx][0] ++;
                  fvuElinkLastTsHit[fuCurrDpbIdx][0] = 0;
               } // if( kTRUE == fbBetaFormat )
               break;
            } // case stsxyter::BetaMessType::Dummy / ReadDataAck / Ack :
            default:
            {
               LOG(fatal) << "CbmCern2017MonitorHodo::DoUnpack => "
                          << "Unknown message type, should never happen, stopping here!";
            }
         } // switch( mess.GetMessType() )
      } // for( uint32_t uIdx = 0; uIdx < uNbMessages; ++uIdx )
   } // for( size_t m = 0; m < numCompMsInTs; ++m )


   // End of TS, check if stuff to do with the hits inside each MS
   // Usefull for low rate pulser tests
      // Need to do it only when last DPB is processed, as they are done one by one
   if( fuCurrDpbIdx == fuNrOfDpbs - 1 )
   {
      for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )
      {
         std::vector< Bool_t> bHodo1X( fuNbChanPerAsic/2, kFALSE);
         std::vector< Bool_t> bHodo1Y( fuNbChanPerAsic/2, kFALSE);
         std::vector< Bool_t> bHodo2X( fuNbChanPerAsic/2, kFALSE);
         std::vector< Bool_t> bHodo2Y( fuNbChanPerAsic/2, kFALSE);
         for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
         {
            if( fUnpackPar->GetAsicIndexHodo1() == uXyterIdx )
            {
               // Hodo 1
                  // Loop on one Axis
               for( UInt_t uChan = 0; uChan < fuNbChanPerAsic/2; ++uChan )
                  if( fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] )
                     // Loop on Second axis
                     for( UInt_t uChanB = fuNbChanPerAsic/2; uChanB < fuNbChanPerAsic; ++uChanB )
                        if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ][ uMsIdx ] )
                        {
                           if( fUnpackPar->IsXySwappedHodo1() )
                           {
                              // Maybe add cross check for invalide mapping index!
                              bHodo1X[ fUnpackPar->GetChannelToFiberMap( uChanB ) ] = kTRUE;
                              bHodo1Y[ fUnpackPar->GetChannelToFiberMap( uChan  ) ] = kTRUE;
                           } // if( fUnpackPar->IsXySwappedHodo1() )
                              else
                              {
                                 // Maybe add cross check for invalide mapping index!
                                 bHodo1X[ fUnpackPar->GetChannelToFiberMap( uChan ) ] = kTRUE;
                                 bHodo1Y[ fUnpackPar->GetChannelToFiberMap( uChanB  ) ] = kTRUE;
                              } // else of if( fUnpackPar->IsXySwappedHodo1() )
                        } // if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ][ uMsIdx ] )
            } // if( fUnpackPar->GetAsicIndexHodo1() == uXyterIdx )
            else if( fUnpackPar->GetAsicIndexHodo2() == uXyterIdx )
            {
               // Hodo 2
                  // Loop on one Axis
               for( UInt_t uChan = 0; uChan < fuNbChanPerAsic/2; ++uChan )
                  if( fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] )
                     // Loop on Second axis
                     for( UInt_t uChanB = fuNbChanPerAsic/2; uChanB < fuNbChanPerAsic; ++uChanB )
                        if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ][ uMsIdx ] )
                        {
                           if( fUnpackPar->IsXySwappedHodo2() )
                           {
                              // Maybe add cross check for invalide mapping index!
                              bHodo2X[ fUnpackPar->GetChannelToFiberMap( uChanB ) ] = kTRUE;
                              bHodo2Y[ fUnpackPar->GetChannelToFiberMap( uChan  ) ] = kTRUE;
                           } // if( fUnpackPar->IsXySwappedHodo2() )
                              else
                              {
                              // Maybe add cross check for invalide mapping index!
                                 bHodo2X[ fUnpackPar->GetChannelToFiberMap( uChan  ) ] = kTRUE;
                                 bHodo2Y[ fUnpackPar->GetChannelToFiberMap( uChanB ) ] = kTRUE;
                              } // else of if( fUnpackPar->IsXySwappedHodo2() )
                        } // if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ][ uMsIdx ] )
            } // else if( fUnpackPar->GetAsicIndexHodo2() == uXyterIdx )
/*
            for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
            {
               fhHodoChanHitsPerMs[uXyterIdx]->Fill( fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ], uChan );

               // Coincidences between
               if( fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] )
                  for( UInt_t uChanB = uChan + 1; uChanB < fuNbChanPerAsic; ++uChanB )
                     if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ][ uMsIdx ] )
                     {
                        fhHodoChanSameMs[ uXyterIdx ]->Fill( uChan, uChanB );
                        Double_t dTimeDiff = fvdChanLastHitTimeInMs[ uXyterIdx ][ uChanB ][ uMsIdx ]
                                           - fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ];

                        fpStsChanSameMsTimeDiff[ uXyterIdx ]->Fill( uChan, uChanB, dTimeDiff );
                     } // if( fvuChanNbHitsInMs[ uXyterIdx ][ uChanB ] )
               // Ok to reset as we only compare to channels with higher indices (or channels in ASICs with higher indices)
               fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
               fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
               fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
*/
            for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
            {
               // Reset counters
               fvuChanNbHitsInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
               fvdChanLastHitTimeInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = -1.0;
               fvusChanLastHitAdcInMs[ uXyterIdx ][ uChan ][ uMsIdx ] = 0;
            } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic; ++uChan )
         } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )

         Bool_t bHitInHodo1X = kFALSE;
         Bool_t bHitInHodo1Y = kFALSE;
         Bool_t bHitInHodo2X = kFALSE;
         Bool_t bHitInHodo2Y = kFALSE;
         for( UInt_t uChan = 0; uChan < fuNbChanPerAsic/2; ++uChan )
         {
            if( bHodo1X[ uChan ] )
               bHitInHodo1X = kTRUE;

            if( bHodo1Y[ uChan ] )
               bHitInHodo1Y = kTRUE;

            if( bHodo2X[ uChan ] )
               bHitInHodo2X = kTRUE;

            if( bHodo2Y[ uChan ] )
               bHitInHodo2Y = kTRUE;

            for( UInt_t uChanB = 0; uChanB < fuNbChanPerAsic/2; ++uChanB )
            {
               if( bHodo1X[ uChan ] && bHodo1Y[ uChanB ] )
                  fhHodoSameMs1XY->Fill( uChan, uChanB);

               if( bHodo2X[ uChan ] && bHodo2Y[ uChanB ] )
                  fhHodoSameMs2XY->Fill( uChan, uChanB );

               if( bHodo1X[ uChan ] && bHodo2X[ uChanB ] )
                  fhHodoSameMsX1X2->Fill( uChan, uChanB );

               if( bHodo1Y[ uChan ] && bHodo2Y[ uChanB ] )
                  fhHodoSameMsY1Y2->Fill( uChan, uChanB );

               if( bHodo1X[ uChan ] && bHodo2Y[ uChanB ] )
                  fhHodoSameMsX1Y2->Fill( uChan, uChanB );

               if( bHodo1Y[ uChan ] && bHodo2X[ uChanB ] )
                  fhHodoSameMsY1X2->Fill( uChan, uChanB );
            } // for( UInt_t uChanB = 0; uChanB < fuNbChanPerAsic/2; ++uChanB )
         } // for( UInt_t uChan = 0; uChan < fuNbChanPerAsic/2; ++uChan )

         if( bHitInHodo1X && bHitInHodo1Y )
            fhHodoSameMsCntEvoX1Y1->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         if( bHitInHodo2X && bHitInHodo2Y )
            fhHodoSameMsCntEvoX2Y2->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         if( bHitInHodo1X && bHitInHodo2X )
            fhHodoSameMsCntEvoX1X2->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         if( bHitInHodo1Y && bHitInHodo2Y )
            fhHodoSameMsCntEvoY1Y2->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         if( bHitInHodo1X && bHitInHodo2Y )
            fhHodoSameMsCntEvoX1Y2->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         if( bHitInHodo1Y && bHitInHodo2X )
            fhHodoSameMsCntEvoY1X2->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         if( bHitInHodo1X && bHitInHodo1Y && bHitInHodo2X && bHitInHodo2Y)
            fhHodoSameMsCntEvoX1Y1X2Y2->Fill( fvdMsTime[ uMsIdx ] - fdStartTimeMsSz );

         fvdMsTime[ uMsIdx ] = 0.0;
      } // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )


      // Time differences plotting using the fully time sorted hits
      if( 0 < fvmHitsInTs.size() )
      {
         ULong64_t ulLastHitTime = ( *( fvmHitsInTs.rbegin() ) ).GetTs();
         std::multiset< stsxyter::BetaHit >::iterator it;

//         std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
//         Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();

         Double_t dCoincBorder = 50.0; // ns, +/-

         for( it  = fvmHitsInTs.begin();
              it != fvmHitsInTs.end() && (*it).GetTs() < ulLastHitTime - 320; // 32 * 3.125 ns = 1000 ns
              ++it )
         {
            UShort_t usAsicIdx = (*it).GetAsic();
            UShort_t usChanIdx = (*it).GetChan();
            ULong64_t ulHitTs  = (*it).GetTs();

            Bool_t bHitInX = usChanIdx < fuNbChanPerAsic/2;
            UInt_t uFiberIdx = fUnpackPar->GetChannelToFiberMap( usChanIdx  );

            if( fUnpackPar->GetAsicIndexHodo1() == usAsicIdx )
            {
               if( fUnpackPar->IsXySwappedHodo1() )
                  bHitInX = !bHitInX;

               if( bHitInX )
               {
                  fLastSortedHit1X = (*it);

                  Double_t dDtX1Y1 = ( fLastSortedHit1Y.GetTs() - ulHitTs ) * stsxyter::kdBetaClockCycleNs;
                  Double_t dDtX1X2 = ( fLastSortedHit2X.GetTs() - ulHitTs ) * stsxyter::kdBetaClockCycleNs;
                  Double_t dDtX1Y2 = ( fLastSortedHit2Y.GetTs() - ulHitTs ) * stsxyter::kdBetaClockCycleNs;

                  fhHodoSortedDtX1Y1->Fill( dDtX1Y1 );
                  fhHodoSortedDtX1X2->Fill( dDtX1X2 );
                  fhHodoSortedDtX1Y2->Fill( dDtX1Y2 );

                  if( TMath::Abs( dDtX1Y1 ) < dCoincBorder )
                     fhHodoSortedMapX1Y1->Fill( uFiberIdx, fUnpackPar->GetChannelToFiberMap( fLastSortedHit1Y.GetChan() ) );
                  if( TMath::Abs( dDtX1X2 ) < dCoincBorder )
                     fhHodoSortedMapX1X2->Fill( uFiberIdx, fUnpackPar->GetChannelToFiberMap( fLastSortedHit2X.GetChan() ) );
                  if( TMath::Abs( dDtX1Y2 ) < dCoincBorder )
                     fhHodoSortedMapX1Y2->Fill( uFiberIdx, fUnpackPar->GetChannelToFiberMap( fLastSortedHit2Y.GetChan() ) );
               } // if( bHitInX )
                  else
                  {
                     fLastSortedHit1Y = (*it);

                     Double_t dDtX1Y1 = ( ulHitTs - fLastSortedHit1X.GetTs() ) * stsxyter::kdBetaClockCycleNs;
                     Double_t dDtY1Y2 = ( fLastSortedHit2Y.GetTs() - ulHitTs ) * stsxyter::kdBetaClockCycleNs;
                     Double_t dDtY1X2 = ( fLastSortedHit2X.GetTs() - ulHitTs ) * stsxyter::kdBetaClockCycleNs;

                     fhHodoSortedDtX1Y1->Fill( dDtX1Y1 );
                     fhHodoSortedDtY1Y2->Fill( dDtY1Y2 );
                     fhHodoSortedDtY1X2->Fill( dDtY1X2 );

                     if( TMath::Abs( dDtX1Y1 ) < dCoincBorder )
                        fhHodoSortedMapX1Y1->Fill( fUnpackPar->GetChannelToFiberMap( fLastSortedHit1X.GetChan() ), uFiberIdx );
                     if( TMath::Abs( dDtY1Y2 ) < dCoincBorder )
                        fhHodoSortedMapY1Y2->Fill( uFiberIdx, fUnpackPar->GetChannelToFiberMap( fLastSortedHit2Y.GetChan() ) );
                     if( TMath::Abs( dDtY1X2 ) < dCoincBorder )
                        fhHodoSortedMapY1X2->Fill( uFiberIdx, fUnpackPar->GetChannelToFiberMap( fLastSortedHit2X.GetChan() ) );
                  } // else of if( bHitInX )
            } // if( fUnpackPar->GetAsicIndexHodo1() == usAsicIdx )
            else if( fUnpackPar->GetAsicIndexHodo2() == usAsicIdx )
            {
               if( fUnpackPar->IsXySwappedHodo2() )
                  bHitInX = !bHitInX;

               if( bHitInX )
               {
                  fLastSortedHit2X = (*it);

                  Double_t dDtX2Y2 = ( fLastSortedHit2Y.GetTs() - ulHitTs ) * stsxyter::kdBetaClockCycleNs;
                  Double_t dDtX1X2 = ( ulHitTs - fLastSortedHit1X.GetTs() ) * stsxyter::kdBetaClockCycleNs;
                  Double_t dDtY1X2 = ( ulHitTs - fLastSortedHit1Y.GetTs() ) * stsxyter::kdBetaClockCycleNs;

                  fhHodoSortedDtX2Y2->Fill( dDtX2Y2 );
                  fhHodoSortedDtX1X2->Fill( dDtX1X2 );
                  fhHodoSortedDtY1X2->Fill( dDtY1X2 );

                  if( TMath::Abs( dDtX2Y2 ) < dCoincBorder )
                     fhHodoSortedMapX2Y2->Fill( uFiberIdx, fUnpackPar->GetChannelToFiberMap( fLastSortedHit2Y.GetChan() ) );
                  if( TMath::Abs( dDtX1X2 ) < dCoincBorder )
                     fhHodoSortedMapX1X2->Fill( fUnpackPar->GetChannelToFiberMap( fLastSortedHit1X.GetChan() ), uFiberIdx );
                  if( TMath::Abs( dDtY1X2 ) < dCoincBorder )
                     fhHodoSortedMapY1X2->Fill( fUnpackPar->GetChannelToFiberMap( fLastSortedHit1Y.GetChan() ), uFiberIdx );
               } // if( bHitInX )
                  else
                  {
                     fLastSortedHit2Y = (*it);

                     Double_t dDtX2Y2 = ( ulHitTs - fLastSortedHit1X.GetTs() ) * stsxyter::kdBetaClockCycleNs;
                     Double_t dDtY1Y2 = ( ulHitTs - fLastSortedHit1Y.GetTs() ) * stsxyter::kdBetaClockCycleNs;
                     Double_t dDtX1Y2 = ( ulHitTs - fLastSortedHit2X.GetTs() ) * stsxyter::kdBetaClockCycleNs;

                     fhHodoSortedDtX2Y2->Fill( dDtX2Y2 );
                     fhHodoSortedDtY1Y2->Fill( dDtY1Y2 );
                     fhHodoSortedDtX1Y2->Fill( dDtX1Y2 );

                     if( TMath::Abs( dDtX2Y2 ) < dCoincBorder )
                        fhHodoSortedMapX2Y2->Fill( fUnpackPar->GetChannelToFiberMap( fLastSortedHit2X.GetChan() ), uFiberIdx );
                     if( TMath::Abs( dDtY1Y2 ) < dCoincBorder )
                        fhHodoSortedMapY1Y2->Fill( fUnpackPar->GetChannelToFiberMap( fLastSortedHit1Y.GetChan() ), uFiberIdx );
                     if( TMath::Abs( dDtX1Y2 ) < dCoincBorder )
                        fhHodoSortedMapX1Y2->Fill( fUnpackPar->GetChannelToFiberMap( fLastSortedHit1X.GetChan() ), uFiberIdx );
                  } // else of if( bHitInX )
            } // else if( fUnpackPar->GetAsicIndexHodo2() == usAsicIdx )
         } // loop on hits untils hits within 100 ns of last one or last one itself are reached

         // Remove all hits which were already used
         fvmHitsInTs.erase( fvmHitsInTs.begin(), it );
      } // if( 0 < fvmHitsInTs.size() )
   } // if( fuCurrDpbIdx == fuNrOfDpbs - 1 )

   if( kTRUE == fbBetaFormat && 0 == ts.index() % 1000 && fuCurrDpbIdx == fuNrOfDpbs - 1)
   {
      for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )
         LOG(info) << "eDPB "   << std::setw(2) << uDpb
                   << " eLink " << std::setw(2) << 0
                   << " current TS cycle counter is " << std::setw(12) << fvuCurrentTsMsb[uDpb][0]
                      << "\n"
                   << " current sorted buffer size is " << fvmHitsInTs.size()
                   << " First hit TS is " << ( *( fvmHitsInTs.begin() ) ).GetTs()
                   << " Last hit TS is " << ( *( fvmHitsInTs.rbegin() ) ).GetTs();

   } // if( kTRUE == fbBetaFormat && 0 == ts.descriptor.index % 10000 )

  return kTRUE;
}

void CbmCern2017MonitorHodo::FillHitInfo( stsxyter::BetaMessage mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx, const UInt_t & uMsIdx )
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

      // Use TS w/o overlap bits as they will anyway come from the TS_MSB
   fvulChanLastHitTime[ uAsicIdx ][ usChan ] = usRawTs;


//         if( usRawTs < fvuElinkLastTsHit[fuCurrDpbIdx][usElinkIdx] )
//            fvuCurrentTsMsb[fuCurrDpbIdx][usElinkIdx]++;

   fvulChanLastHitTime[ uAsicIdx ][ usChan ] +=
               static_cast<ULong64_t>( stsxyter::kuBetaHitNbTsBins )
             * static_cast<ULong64_t>( fvuCurrentTsMsb[fuCurrDpbIdx][0]);

   fvuElinkLastTsHit[fuCurrDpbIdx][usElinkIdx] = usRawTs;

   // Convert the Hit time in bins to Hit time in ns
   Double_t dHitTimeNs = fvulChanLastHitTime[ uAsicIdx ][ usChan ] * stsxyter::kdBetaClockCycleNs;

   // Store new value of Hit time in ns
   fvdChanLastHitTime[ uAsicIdx ][ usChan ] = fvulChanLastHitTime[ uAsicIdx ][ usChan ];

   // Pulser and MS
   fvuChanNbHitsInMs[      uAsicIdx ][ usChan ][ uMsIdx ] ++;
   fvdChanLastHitTimeInMs[ uAsicIdx ][ usChan ][ uMsIdx ] = dHitTimeNs;
   fvusChanLastHitAdcInMs[ uAsicIdx ][ usChan ][ uMsIdx ] = usRawAdc;
   fvmChanHitsInTs[        uAsicIdx ][ usChan ].insert( stsxyter::BetaHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ], usRawAdc ) );
   fvmHitsInTs.insert( stsxyter::BetaHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ], usRawAdc, uAsicIdx, usChan ) );

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
/*
   if( kTRUE == fbLongHistoEnable )
   {
      std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
      Double_t dUnixTimeInRun = std::chrono::duration_cast< std::chrono::seconds >(tNow - ftStartTimeUnix).count();
      fhFebRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , 1.0 / fuLongHistoBinSizeSec );
      fhFebChRateEvoLong[ uAsicIdx ]->Fill( dUnixTimeInRun , usChan, 1.0 / fuLongHistoBinSizeSec );
   } // if( kTRUE == fbLongHistoEnable )
*/
   // Fill histos for single Hodos
   UInt_t uFiberInHodo = fUnpackPar->GetChannelToFiberMap( usChan );
   if( fUnpackPar->GetAsicIndexHodo1() == uAsicIdx )
   {
      // Hodo 1
      if( fUnpackPar->GetChannelToPlaneMapHodo1( usChan ) )
      {
         // Y
         fhHodoChanCounts1Y->Fill( uFiberInHodo );
         fhHodoChanAdcRaw1Y->Fill( uFiberInHodo, usRawAdc );
         fhHodoChanHitRateEvo1Y->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
         fhHodoRateEvo1Y->Fill(   dTimeSinceStartMin, 1.0/60.0 );
      } // if( fUnpackPar->GetChannelToPlaneMapHodo1( usChan ) )
         else
         {
            // X
            fhHodoChanCounts1X->Fill( uFiberInHodo );
            fhHodoChanAdcRaw1X->Fill( uFiberInHodo, usRawAdc );
            fhHodoChanHitRateEvo1X->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
            fhHodoRateEvo1X->Fill(   dTimeSinceStartMin, 1.0/60.0 );
         } // else of if( fUnpackPar->GetChannelToPlaneMapHodo1( usChan ) )
   } // if( fUnpackPar->GetAsicIndexHodo1() == uAsicIdx )
   else if( fUnpackPar->GetAsicIndexHodo2() == uAsicIdx )
   {
      // Hodo 2
      if( fUnpackPar->GetChannelToPlaneMapHodo2( usChan ) )
      {
         // Y
         fhHodoChanCounts2Y->Fill( uFiberInHodo );
         fhHodoChanAdcRaw2Y->Fill( uFiberInHodo, usRawAdc );
         fhHodoChanHitRateEvo2Y->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
         fhHodoRateEvo2Y->Fill(   dTimeSinceStartMin, 1.0/60.0 );
      } // if( fUnpackPar->GetChannelToPlaneMapHodo2( usChan ) )
         else
         {
            // X
            fhHodoChanCounts2X->Fill( uFiberInHodo );
            fhHodoChanAdcRaw2X->Fill( uFiberInHodo, usRawAdc );
            fhHodoChanHitRateEvo2X->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
            fhHodoRateEvo2X->Fill(   dTimeSinceStartMin, 1.0/60.0 );
         } // else of if( fUnpackPar->GetChannelToPlaneMapHodo2( usChan ) )
   } // else if( fUnpackPar->GetAsicIndexHodo2() == uAsicIdx )

}

void CbmCern2017MonitorHodo::FillTsMsbInfo( stsxyter::BetaMessage mess, const UShort_t & usElinkIdx, const UInt_t & uAsicIdx )
{
   UShort_t usVal    = mess.GetTsMsbVal();

   // Update Status counters
   if( usVal < fvuCurrentTsMsb[fuCurrDpbIdx][usElinkIdx] )
      fvuCurrentTsMsbCycle[fuCurrDpbIdx][usElinkIdx] ++;
   fvuCurrentTsMsb[fuCurrDpbIdx][usElinkIdx] = usVal;

   fhHodoFebTsMsb->Fill( fvuCurrentTsMsb[fuCurrDpbIdx][usElinkIdx], uAsicIdx );

   // Update the overlap bits for this eLink
   fvuCurrentTsMsbOver[fuCurrDpbIdx][usElinkIdx] =  fvuCurrentTsMsb[fuCurrDpbIdx][usElinkIdx]
                                                  & stsxyter::kusBetaMaskTsMsbOver;
}

void CbmCern2017MonitorHodo::Reset()
{
}

void CbmCern2017MonitorHodo::Finish()
{

   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmCern2017MonitorHodo statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::BetaMessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::BetaMessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::BetaMessType::Dummy ];

   LOG(info) << "-------------------------------------";

   SaveAllHistos();

}


void CbmCern2017MonitorHodo::FillOutput(boost::any)
{
}

void CbmCern2017MonitorHodo::SaveAllHistos( TString sFileName )
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

   fhHodoChanCounts1X->Write();
   fhHodoChanCounts1Y->Write();
   fhHodoChanCounts2X->Write();
   fhHodoChanCounts2Y->Write();
   fhHodoChanAdcRaw1X->Write();
   fhHodoChanAdcRaw1Y->Write();
   fhHodoChanAdcRaw2X->Write();
   fhHodoChanAdcRaw2Y->Write();
   fhHodoChanHitRateEvo1X->Write();
   fhHodoChanHitRateEvo1Y->Write();
   fhHodoChanHitRateEvo2X->Write();
   fhHodoChanHitRateEvo2Y->Write();
   fhHodoRateEvo1X->Write();
   fhHodoRateEvo1Y->Write();
   fhHodoRateEvo1X->Write();
   fhHodoRateEvo2Y->Write();
   fhHodoSameMs1XY->Write();
   fhHodoSameMs2XY->Write();
   fhHodoSameMsX1X2->Write();
   fhHodoSameMsY1Y2->Write();
   fhHodoSameMsX1Y2->Write();
   fhHodoSameMsY1X2->Write();

   fhHodoSameMsCntEvoX1Y1->Write();
   fhHodoSameMsCntEvoX2Y2->Write();
   fhHodoSameMsCntEvoX1X2->Write();
   fhHodoSameMsCntEvoY1Y2->Write();
   fhHodoSameMsCntEvoX1Y2->Write();
   fhHodoSameMsCntEvoY1X2->Write();
   fhHodoSameMsCntEvoX1Y1X2Y2->Write();

   fhHodoSortedDtX1Y1->Write();
   fhHodoSortedDtX2Y2->Write();
   fhHodoSortedDtX1X2->Write();
   fhHodoSortedDtY1Y2->Write();
   fhHodoSortedDtX1Y2->Write();
   fhHodoSortedDtY1X2->Write();
   fhHodoSortedMapX1Y1->Write();
   fhHodoSortedMapX2Y2->Write();
   fhHodoSortedMapX1X2->Write();
   fhHodoSortedMapY1Y2->Write();
   fhHodoSortedMapX1Y2->Write();
   fhHodoSortedMapY1X2->Write();

   fhHodoFebTsMsb->Write();

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
void CbmCern2017MonitorHodo::ResetAllHistos()
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

   fhHodoChanCounts1X->Reset();
   fhHodoChanCounts1Y->Reset();
   fhHodoChanCounts2X->Reset();
   fhHodoChanCounts2Y->Reset();
   fhHodoChanAdcRaw1X->Reset();
   fhHodoChanAdcRaw1Y->Reset();
   fhHodoChanAdcRaw2X->Reset();
   fhHodoChanAdcRaw2Y->Reset();
   fhHodoChanHitRateEvo1X->Reset();
   fhHodoChanHitRateEvo1Y->Reset();
   fhHodoChanHitRateEvo2X->Reset();
   fhHodoChanHitRateEvo2Y->Reset();
   fhHodoRateEvo1X->Reset();
   fhHodoRateEvo1Y->Reset();
   fhHodoRateEvo1X->Reset();
   fhHodoRateEvo2Y->Reset();
   fhHodoSameMs1XY->Reset();
   fhHodoSameMs2XY->Reset();
   fhHodoSameMsX1X2->Reset();
   fhHodoSameMsY1Y2->Reset();
   fhHodoSameMsX1Y2->Reset();
   fhHodoSameMsY1X2->Reset();

   fhHodoSameMsCntEvoX1Y1->Reset();
   fhHodoSameMsCntEvoX2Y2->Reset();
   fhHodoSameMsCntEvoX1X2->Reset();
   fhHodoSameMsCntEvoY1Y2->Reset();
   fhHodoSameMsCntEvoX1Y2->Reset();
   fhHodoSameMsCntEvoY1X2->Reset();
   fhHodoSameMsCntEvoX1Y1X2Y2->Reset();

   fhHodoSortedDtX1Y1->Reset();
   fhHodoSortedDtX2Y2->Reset();
   fhHodoSortedDtX1X2->Reset();
   fhHodoSortedDtY1Y2->Reset();
   fhHodoSortedDtX1Y2->Reset();
   fhHodoSortedDtY1X2->Reset();
   fhHodoSortedMapX1Y1->Reset();
   fhHodoSortedMapX2Y2->Reset();
   fhHodoSortedMapX1X2->Reset();
   fhHodoSortedMapY1Y2->Reset();
   fhHodoSortedMapX1Y2->Reset();
   fhHodoSortedMapY1X2->Reset();

   fhHodoFebTsMsb->Reset();

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

void CbmCern2017MonitorHodo::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}

void CbmCern2017MonitorHodo::SetLongDurationLimits( UInt_t uDurationSeconds, UInt_t uBinSize )
{
   fbLongHistoEnable     = kTRUE;
   fuLongHistoNbSeconds  = uDurationSeconds;
   fuLongHistoBinSizeSec = uBinSize;
}

ClassImp(CbmCern2017MonitorHodo)
