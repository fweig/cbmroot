// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmCosy2018MonitorHodo                        -----
// -----                Created 15/02/18  by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmCosy2018MonitorHodo.h"

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

Bool_t bCosy2018ResetHodoHistos = kFALSE;
Bool_t bCosy2018WriteHodoHistos = kFALSE;

CbmCosy2018MonitorHodo::CbmCosy2018MonitorHodo() :
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
   fdCoincBorder( 50.0 ),
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
   fhHodoSortedCntEvoX1Y1(NULL),
   fhHodoSortedCntEvoX2Y2(NULL),
   fhHodoSortedCntEvoX1X2(NULL),
   fhHodoSortedCntEvoY1Y2(NULL),
   fhHodoSortedCntEvoX1Y2(NULL),
   fhHodoSortedCntEvoY1X2(NULL),
//   fhHodoFebTsMsb(NULL),
   fcMsSizeAll(NULL)
{
}

CbmCosy2018MonitorHodo::~CbmCosy2018MonitorHodo()
{
}

Bool_t CbmCosy2018MonitorHodo::Init()
{
  LOG(info) << "Initializing flib StsXyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmCosy2018MonitorHodo::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackPar = (CbmCern2017UnpackParHodo*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmCern2017UnpackParHodo"));
}


Bool_t CbmCosy2018MonitorHodo::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmCosy2018MonitorHodo::ReInitContainers()
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

      std::stringstream ss;
      for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      {
         if( 0 == uElink % 10 )
            ss << "\n------> ";

         ss << std::setw( 5 ) << fvuElinkToAsic[uDpb][uElink] << " ";
      } // for( UInt_t uElink = 0; uElink < fuNbElinksPerDpb; ++uElink )
      LOG(info) << ss.str();
   } // for( UInt_t uDpb = 0; uDpb < fuNrOfDpbs; ++uDpb )

   LOG(info) << "ASIC Idx for Hodo 1: " << fUnpackPar->GetAsicIndexHodo1();
   LOG(info) << "ASIC Idx for Hodo 2: " << fUnpackPar->GetAsicIndexHodo2();

   LOG(info) << "XY swapped in Hodo 1: " << fUnpackPar->IsXySwappedHodo1();
   LOG(info) << "XY swapped in Hodo 2: " << fUnpackPar->IsXySwappedHodo2();

   LOG(info) << "X axis inverted in Hodo 1: " << fUnpackPar->IsXInvertedHodo1();
   LOG(info) << "X axis inverted in Hodo 1: " << fUnpackPar->IsYInvertedHodo1();
   LOG(info) << "X axis inverted in Hodo 2: " << fUnpackPar->IsXInvertedHodo2();
   LOG(info) << "X axis inverted in Hodo 2: " << fUnpackPar->IsYInvertedHodo2();

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
   LOG(info) << "CbmCosy2018MonitorHodo::ReInitContainers => Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
   LOG(info) << "CbmCosy2018MonitorHodo::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
   LOG(info) << "CbmCosy2018MonitorHodo::ReInitContainers =>  Changed fvuChanNbHitsInMs size "
                << fvuChanNbHitsInMs[ 0 ][ 0 ].size() << " VS " << fuMaxNbMicroslices;
   return kTRUE;
}

void CbmCosy2018MonitorHodo::CreateHistograms()
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
   fHM->Add(sHistName.Data(), fhHodoMessType);

   sHistName = "hHodoSysMessType";
   title = "Nb of system message for each type; System Type";
   fhHodoSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
/*
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhHodoSysMessType);

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
   fHM->Add(sHistName.Data(), fhHodoMessTypePerDpb);

   sHistName = "hHodoSysMessTypePerDpb";
   title = "Nb of system message of each type for each DPB; DPB; System Type";
   fhHodoSysMessTypePerDpb = new TH2I(sHistName, title, fuNrOfDpbs, 0, fuNrOfDpbs, 17, 0., 17.);
/*
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhHodoSysMessTypePerDpb);

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
   fHM->Add(sHistName.Data(), fhHodoMessTypePerElink);

   sHistName = "hHodoSysMessTypePerElink";
   title = "Nb of system message of each type for each eLink; eLink; System Type";
   fhHodoSysMessTypePerElink = new TH2I(sHistName, title, fuNrOfDpbs * fuNbElinksPerDpb, 0, fuNrOfDpbs * fuNbElinksPerDpb, 17, 0., 17.);
/*
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhHodoSysMessTypePerElink->GetYaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
*/
   fHM->Add(sHistName.Data(), fhHodoSysMessTypePerElink);

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

      // Raw Adc Distribution
      sHistName = Form( "hHodoChanAdcRaw_%03u", uXyterIdx );
      title = Form( "Raw Adc distribution per channel, StsXyter #%03u; Channel []; Adc []; Hits []", uXyterIdx );
      fhHodoChanAdcRaw.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanAdcRaw[ uXyterIdx ] );

      // Raw Adc Distribution profile
      sHistName = Form( "hHodoChanAdcRawProfc_%03u", uXyterIdx );
      title = Form( "Raw Adc prodile per channel, StsXyter #%03u; Channel []; Adc []", uXyterIdx );
      fhHodoChanAdcRawProf.push_back( new TProfile(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanAdcRawProf[ uXyterIdx ] );

      // Raw Ts Distribution
      sHistName = Form( "hHodoChanRawTs_%03u", uXyterIdx );
      title = Form( "Raw Timestamp distribution per channel, StsXyter #%03u; Channel []; Ts []; Hits []", uXyterIdx );
      fhHodoChanRawTs.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 stsxyter::kuHitNbTsBins, -0.5, stsxyter::kuHitNbTsBins -0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanRawTs[ uXyterIdx ] );

      // Missed event flag
      sHistName = Form( "hHodoChanMissEvt_%03u", uXyterIdx );
      title = Form( "Missed Event flags per channel, StsXyter #%03u; Channel []; Miss Evt []; Hits []", uXyterIdx );
      fhHodoChanMissEvt.push_back( new TH2I(sHistName, title,
                                 fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5,
                                 2, -0.5, 1.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanMissEvt[ uXyterIdx ] );

      // Hit rates evo per channel
      sHistName = Form( "hHodoChanRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [s]; Channel []; Hits []", uXyterIdx );
      fhHodoChanHitRateEvo.push_back( new TH2I( sHistName, title,
                                                1800, 0, 1800,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo[ uXyterIdx ] );

      // Hit rates evo per StsXyter
      sHistName = Form( "hHodoFebRateEvo_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [s]; Hits []", uXyterIdx );
      fhHodoFebRateEvo.push_back( new TH1I(sHistName, title, 1800, 0, 1800 ) );
      fHM->Add(sHistName.Data(), fhHodoFebRateEvo[ uXyterIdx ] );

      // Hit rates evo per channel, 1 minute bins, 24h
      sHistName = Form( "hHodoChanRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second & channel in StsXyter #%03u; Time [min]; Channel []; Hits []", uXyterIdx );
      fhHodoChanHitRateEvoLong.push_back( new TH2D( sHistName, title,
                                                1440, 0, 1440,
                                                fuNbChanPerAsic, -0.5, fuNbChanPerAsic - 0.5 ) );
      fHM->Add(sHistName.Data(), fhHodoChanHitRateEvoLong[ uXyterIdx ] );

      // Hit rates evo per StsXyter, 1 minute bins, 24h
      sHistName = Form( "hHodoFebRateEvoLong_%03u", uXyterIdx );
      title = Form( "Hits per second in StsXyter #%03u; Time [min]; Hits []", uXyterIdx );
      fhHodoFebRateEvoLong.push_back( new TH1D(sHistName, title, 1440, 0, 1440 ) );
      fHM->Add(sHistName.Data(), fhHodoFebRateEvoLong[ uXyterIdx ] );

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

   sHistName = "hHodoChanCounts1Y";
   title = "Channel counts for hodoscope 1 axis Y; Y channel []; Hits []";
   fhHodoChanCounts1Y = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts1Y );

   sHistName = "hHodoChanCounts2X";
   title = "Channel counts for hodoscope 2 axis X; X channel []; Hits []";
   fhHodoChanCounts2X = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts2X );

   sHistName = "hHodoChanCounts2Y";
   title = "Channel counts for hodoscope 2 axis Y; Y channel []; Hits []";
   fhHodoChanCounts2Y = new TH1I( sHistName, title,  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanCounts2Y );

   // Raw ADC distributions for each of the hodoscope planes
   sHistName = "hHodoChanAdcRaw1X";
   title = "Raw ADC distributions for hodoscope 1 axis X; X channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw1X = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw1X );

   sHistName = "hHodoChanAdcRaw1Y";
   title = "Raw ADC distributions for hodoscope 1 axis Y; Y channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw1Y = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw1Y );

   sHistName = "hHodoChanAdcRaw2X";
   title = "Raw ADC distributions for hodoscope 2 axis X; X channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw2X = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw2X );

   sHistName = "hHodoChanAdcRaw2Y";
   title = "Raw ADC distributions for hodoscope 2 axis Y; Y channel []; ADC [bin]; Hits []";
   fhHodoChanAdcRaw2Y = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  stsxyter::kuHitNbAdcBins, -0.5, stsxyter::kuHitNbAdcBins -0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanAdcRaw2Y );

   // Hit rate Evo for each channel of each of the hodoscope planes
   sHistName = "hHodoChanHitRateEvo1X";
   title = "Hits per second & channel in Hodo 1 X axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo1X = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo1X );

   sHistName = "hHodoChanHitRateEvo1Y";
   title = "Hits per second & channel in Hodo 1 Y axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo1Y = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo1Y );

   sHistName = "hHodoChanHitRateEvo2X";
   title = "Hits per second & channel in Hodo 2 X axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo2X = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo2X );

   sHistName = "hHodoChanHitRateEvo2Y";
   title = "Hits per second & channel in Hodo 2 Y axis; Time [s]; Channel []; Hits []";
   fhHodoChanHitRateEvo2Y = new TH2I( sHistName, title,
                                      1800, 0, 1800,
                                      fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoChanHitRateEvo2Y );

   // Hit rate Evo for each channel of each of the hodoscope planes
   sHistName = "fhHodoRateEvo1X";
   title = "Hits per second in Hodoscope 1 X axis; Time [s]; Hits []";
   fhHodoRateEvo1X = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo1X );

   sHistName = "hHodoRateEvo1Y";
   title = "Hits per second in Hodoscope 1 Y axis; Time [s]; Hits []";
   fhHodoRateEvo1Y = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo1Y );

   sHistName = "fhHodoRateEvo2X";
   title = "Hits per second in Hodoscope 2 X axis; Time [s]; Hits []";
   fhHodoRateEvo2X = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo2X );

   sHistName = "hHodoRateEvo2Y";
   title = "Hits per second in Hodoscope 2 Y axis; Time [s]; Hits []";
   fhHodoRateEvo2Y = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoRateEvo2Y );
///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///

   UInt_t uNbBinEvo = 32768 + 1;
   Double_t dMaxEdgeEvo = stsxyter::kdClockCycleNs
                         * static_cast< Double_t >( uNbBinEvo ) / 2.0;
   Double_t dMinEdgeEvo = dMaxEdgeEvo * -1.0;

   sHistName = "fhHodoSortedDtX1Y1";
   title =  "Time diff for hits Hodo 1 X and Hodo 1 Y; tY1 - tX1 [ns]; Counts";
   fhHodoSortedDtX1Y1 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX1Y1);

   sHistName = "fhHodoSortedDtX2Y2";
   title =  "Time diff for hits Hodo 2 X and Hodo 2 Y; tY2 - tX2 [ns]; Counts";
   fhHodoSortedDtX2Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX2Y2);

   sHistName = "fhHodoSortedDtX1X2";
   title =  "Time diff for hits Hodo 1 X and Hodo 2 X; tX2 - tX1 [ns]; Counts";
   fhHodoSortedDtX1X2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX1X2);

   sHistName = "fhHodoSortedDtY1Y2";
   title =  "Time diff for hits Hodo 1 Y and Hodo 2 Y; tY2 - tY1 [ns]; Counts";
   fhHodoSortedDtY1Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtY1Y2);

   sHistName = "fhHodoSortedDtX1Y2";
   title =  "Time diff for hits Hodo 1 X and Hodo 2 Y; tY2 - tX1 [ns]; Counts";
   fhHodoSortedDtX1Y2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtX1Y2);

   sHistName = "fhHodoSortedDtY1X2";
   title =  "Time diff for hits Hodo 1 Y and Hodo 2 X; tY2 - tY1 [ns]; Counts";
   fhHodoSortedDtY1X2 = new TH1I(sHistName, title, uNbBinEvo, dMinEdgeEvo, dMaxEdgeEvo );
   fHM->Add(sHistName.Data(), fhHodoSortedDtY1X2);

   sHistName = "fhHodoSortedMapX1Y1";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and Y; X channel Hodo 1 []; Y channel Hodo 1 []; MS []";
   fhHodoSortedMapX1Y1 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX1Y1 );

   sHistName = "fhHodoSortedMapX2Y2";
   title = "Sorted hits in coincidence for hodoscope 2 axis X and Y; X channel Hodo 2 []; Y channel Hodo 2 []; MS []";
   fhHodoSortedMapX2Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX2Y2 );

   sHistName = "fhHodoSortedMapX1X2";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and 2 axis X; X channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapX1X2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX1X2 );

   sHistName = "fhHodoSortedMapY1Y2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis Y; Y channel Hodo 1 []; Y channel Hodo 2 []; MS []";
   fhHodoSortedMapY1Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapY1Y2 );

   sHistName = "fhHodoSortedMapX1Y2";
   title = "Sorted hits in coincidence for hodoscope 1 axis X and 2 axis Y; X channel Hodo 1 []; Y channel Hodo 2 []; MS []";
   fhHodoSortedMapX1Y2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapX1Y2 );

   sHistName = "fhHodoSortedMapY1X2";
   title = "Sorted hits in coincidence for hodoscope 1 axis Y and 2 axis X; Y channel Hodo 1 []; X channel Hodo 2 []; MS []";
   fhHodoSortedMapY1X2 = new TH2I( sHistName, title,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5,
                                  fuNbChanPerAsic/2, -0.5, fuNbChanPerAsic/2 - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoSortedMapY1X2 );

   // Coincidence counts evolution between some axis of the hodoscopes
   sHistName = "hHodoSortedCntEvoX1Y1";
   title = "Nb of coincidences in both X1 and Y1 per s; Time [s]; X1-Y1 coincidences []";
   fhHodoSortedCntEvoX1Y1 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSortedCntEvoX1Y1 );

   sHistName = "hHodoSortedCntEvoX2Y2";
   title = "Nb of coincidences in both X2 and Y2 per s; Time [s]; X2-Y2 coincidences []";
   fhHodoSortedCntEvoX2Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSortedCntEvoX2Y2 );

   sHistName = "fhHodoSortedCntEvoX1X2";
   title = "Nb of coincidences in both X1 and X2 per s; Time [s]; X1-X2 coincidences []";
   fhHodoSortedCntEvoX1X2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSortedCntEvoX1X2 );

   sHistName = "fhHodoSortedCntEvoY1Y2";
   title = "Nb of coincidences in both Y1 and Y2 per s; Time [s]; Y1-Y2 coincidences []";
   fhHodoSortedCntEvoY1Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSortedCntEvoY1Y2 );

   sHistName = "hHodoSortedCntEvoX1Y2";
   title = "Nb of coincidences in both X1 and Y2 per s; Time [s]; X1-Y2 coincidences []";
   fhHodoSortedCntEvoX1Y2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSortedCntEvoX1Y2 );

   sHistName = "hHodoSortedCntEvoY1X2";
   title = "Nb of coincidences in both Y1 and X2 per s; Time [s]; Y1-X2 coincidences []";
   fhHodoSortedCntEvoY1X2 = new TH1I(sHistName, title, 1800, 0, 1800 );
   fHM->Add(sHistName.Data(), fhHodoSortedCntEvoY1X2 );

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++///
/*
   // Distribution of the TS_MSB per StsXyter
   sHistName = "hHodoFebTsMsb";
   title = "Raw Timestamp Msb distribution per StsXyter; Ts MSB []; StsXyter []; Hits []";
   fhHodoFebTsMsb = new TH2I( sHistName, title, stsxyter::kuTsMsbNbTsBins, -0.5,   stsxyter::kuTsMsbNbTsBins - 0.5,
                                                fuNbStsXyters, -0.5, fuNbStsXyters - 0.5 );
   fHM->Add(sHistName.Data(), fhHodoFebTsMsb );
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
      for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      {
         server->Register("/HodoRaw", fhHodoChanCntRaw[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanAdcRaw[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanAdcRawProf[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanRawTs[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanMissEvt[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanHitRateEvo[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoFebRateEvo[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoChanHitRateEvoLong[ uXyterIdx ] );
         server->Register("/HodoRaw", fhHodoFebRateEvoLong[ uXyterIdx ] );
      } // for( UInt_t uXyterIdx = 0; uXyterIdx < fuNbStsXyters; ++uXyterIdx )
      server->Register("/HodoRaw", fhHodoChanCounts1X );
      server->Register("/HodoRaw", fhHodoChanCounts1Y );
      server->Register("/HodoRaw", fhHodoChanCounts2X );
      server->Register("/HodoRaw", fhHodoChanCounts2Y );
      server->Register("/HodoRaw", fhHodoChanAdcRaw1X );
      server->Register("/HodoRaw", fhHodoChanAdcRaw1Y );
      server->Register("/HodoRaw", fhHodoChanAdcRaw2X );
      server->Register("/HodoRaw", fhHodoChanAdcRaw2Y );
      server->Register("/HodoRaw", fhHodoChanHitRateEvo1X );
      server->Register("/HodoRaw", fhHodoChanHitRateEvo1Y );
      server->Register("/HodoRaw", fhHodoChanHitRateEvo2X );
      server->Register("/HodoRaw", fhHodoChanHitRateEvo2Y );
      server->Register("/HodoRaw", fhHodoRateEvo1X );
      server->Register("/HodoRaw", fhHodoRateEvo1Y );
      server->Register("/HodoRaw", fhHodoRateEvo2X );
      server->Register("/HodoRaw", fhHodoRateEvo2Y );
      server->Register("/HodoRaw", fhHodoSortedDtX1Y1 );
      server->Register("/HodoRaw", fhHodoSortedDtX2Y2 );
      server->Register("/HodoRaw", fhHodoSortedDtX1X2 );
      server->Register("/HodoRaw", fhHodoSortedDtY1Y2 );
      server->Register("/HodoRaw", fhHodoSortedDtX1Y2 );
      server->Register("/HodoRaw", fhHodoSortedDtY1X2 );
      server->Register("/HodoRaw", fhHodoSortedMapX1Y1 );
      server->Register("/HodoRaw", fhHodoSortedMapX2Y2 );
      server->Register("/HodoRaw", fhHodoSortedMapX1X2 );
      server->Register("/HodoRaw", fhHodoSortedMapY1Y2 );
      server->Register("/HodoRaw", fhHodoSortedMapX1Y2 );
      server->Register("/HodoRaw", fhHodoSortedMapY1X2 );
      server->Register("/HodoRaw", fhHodoSortedCntEvoX1Y1 );
      server->Register("/HodoRaw", fhHodoSortedCntEvoX2Y2 );
      server->Register("/HodoRaw", fhHodoSortedCntEvoX1X2 );
      server->Register("/HodoRaw", fhHodoSortedCntEvoY1Y2 );
      server->Register("/HodoRaw", fhHodoSortedCntEvoX1Y2 );
      server->Register("/HodoRaw", fhHodoSortedCntEvoY1X2 );

      server->RegisterCommand("/Reset_All_Hodo", "bCosy2018ResetHodoHistos=kTRUE");
      server->RegisterCommand("/Write_All_Hodo", "bCosy2018WriteHodoHistos=kTRUE");

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
   TCanvas* cHodoSortedDt = new TCanvas( "cHodoSortedDt",
                                    "Hodoscopes coincidence maps",
                                    w, h);
   cHodoSortedDt->Divide( 2, 3 );

   cHodoSortedDt->cd(1);
   gPad->SetLogy();
   fhHodoSortedDtX1Y1->Draw( "" );

   cHodoSortedDt->cd(2);
   gPad->SetLogy();
   fhHodoSortedDtX2Y2->Draw( "" );

   cHodoSortedDt->cd(3);
   gPad->SetLogy();
   fhHodoSortedDtX1X2->Draw( "" );

   cHodoSortedDt->cd(4);
   gPad->SetLogy();
   fhHodoSortedDtY1Y2->Draw( "" );

   cHodoSortedDt->cd(5);
   gPad->SetLogy();
   fhHodoSortedDtX1Y2->Draw( "" );

   cHodoSortedDt->cd(6);
   gPad->SetLogy();
   fhHodoSortedDtY1X2->Draw( "" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoSortedMaps = new TCanvas( "cHodoSortedMaps",
                                    "Hodoscopes coincidence maps",
                                    w, h);
   cHodoSortedMaps->Divide( 2, 3 );

   cHodoSortedMaps->cd(1);
   gPad->SetLogz();
   fhHodoSortedMapX1Y1->Draw( "colz" );

   cHodoSortedMaps->cd(2);
   gPad->SetLogz();
   fhHodoSortedMapX2Y2->Draw( "colz" );

   cHodoSortedMaps->cd(3);
   gPad->SetLogz();
   fhHodoSortedMapX1X2->Draw( "colz" );

   cHodoSortedMaps->cd(4);
   gPad->SetLogz();
   fhHodoSortedMapY1Y2->Draw( "colz" );

   cHodoSortedMaps->cd(5);
   gPad->SetLogz();
   fhHodoSortedMapX1Y2->Draw( "colz" );

   cHodoSortedMaps->cd(6);
   gPad->SetLogz();
   fhHodoSortedMapY1X2->Draw( "colz" );
//====================================================================//

//====================================================================//
   TCanvas* cHodoSortedCoincEvo = new TCanvas( "cHodoSortedCoincEvo",
                                    "Hodoscopes coincidence rate evolution",
                                    w, h);
   cHodoSortedCoincEvo->Divide( 2, 3 );

   cHodoSortedCoincEvo->cd(1);
   gPad->SetGridy();
   gPad->SetLogy();
   fhHodoSortedCntEvoX1Y1->Draw( "colz" );

   cHodoSortedCoincEvo->cd(2);
   gPad->SetGridy();
   gPad->SetLogy();
   fhHodoSortedCntEvoX2Y2->Draw( "colz" );

   cHodoSortedCoincEvo->cd(3);
   gPad->SetGridy();
   gPad->SetLogy();
   fhHodoSortedCntEvoX1X2->Draw( "colz" );

   cHodoSortedCoincEvo->cd(4);
   gPad->SetGridy();
   gPad->SetLogy();
   fhHodoSortedCntEvoY1Y2->Draw( "colz" );

   cHodoSortedCoincEvo->cd(5);
   gPad->SetGridy();
   gPad->SetLogy();
   fhHodoSortedCntEvoX1Y2->Draw( "colz" );

   cHodoSortedCoincEvo->cd(6);
   gPad->SetGridy();
   gPad->SetLogy();
   fhHodoSortedCntEvoY1X2->Draw( "colz" );
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

Bool_t CbmCosy2018MonitorHodo::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bCosy2018ResetHodoHistos )
   {
      ResetAllHistos();
      bCosy2018ResetHodoHistos = kFALSE;
   } // if( bCosy2018ResetHodoHistos )
   if( bCosy2018WriteHodoHistos )
   {
      SaveAllHistos( "data/HodoHistos.root" );
      bCosy2018WriteHodoHistos = kFALSE;
   } // if( bCosy2018WriteHodoHistos )

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
      LOG(info) << "CbmCosy2018MonitorHodo::DoUnpack => Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs.size() << " VS " << fuNbStsXyters;
      LOG(info) << "CbmCosy2018MonitorHodo::DoUnpack =>  Changed fvuChanNbHitsInMs size "
                   << fvuChanNbHitsInMs[ 0 ].size() << " VS " << fuNbChanPerAsic;
      LOG(info) << "CbmCosy2018MonitorHodo::DoUnpack =>  Changed fvuChanNbHitsInMs size "
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
                  LOG(fatal) << "CbmCosy2018MonitorHodo::DoUnpack => "
                             << "eLink index out of bounds!"
                             << usElinkIdx << " VS " << fuNbElinksPerDpb;
               } // if( fuNbElinksPerDpb <= usElinkIdx )
               fhHodoMessTypePerElink->Fill( fuCurrDpbIdx * fuNbElinksPerDpb + usElinkIdx,
                                            static_cast< uint16_t > (typeMess) );

               UInt_t   uAsicIdx   = fvuElinkToAsic[fuCurrDpbIdx][usElinkIdx];
               if( ! ( fUnpackPar->GetAsicIndexHodo1() == uAsicIdx ||
                       fUnpackPar->GetAsicIndexHodo2() == uAsicIdx ) )
                  continue;

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
                  LOG(info) << "CbmCosy2018MonitorHodo::DoUnpack => "
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
               LOG(fatal) << "CbmCosy2018MonitorHodo::DoUnpack => "
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
         fvdMsTime[ uMsIdx ] = 0.0;
      } // for( UInt_t uMsIdx = 0; uMsIdx < fuMaxNbMicroslices; ++uMsIdx )


      // Time differences plotting using the fully time sorted hits
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

            Bool_t bHitInX = usChanIdx < fuNbChanPerAsic/2;
            UInt_t uFiberIdx = fUnpackPar->GetChannelToFiberMap( usChanIdx  );

            Double_t dTimeSinceStartSec = (ulHitTs * stsxyter::kdClockCycleNs - fdStartTime)* 1e-9;

            if( fUnpackPar->GetAsicIndexHodo1() == usAsicIdx )
            {
               if( fUnpackPar->IsXySwappedHodo1() )
                  bHitInX = !bHitInX;

               if( bHitInX )
               {
                  if( fUnpackPar->IsXInvertedHodo1() )
                     uFiberIdx = fuNbChanPerAsic/2 - 1 - uFiberIdx;

                  fLastSortedHit1X = (*it);

                  Double_t dDtX1Y1 = ( fLastSortedHit1Y.GetTs() * stsxyter::kdClockCycleNs - ulHitTs * stsxyter::kdClockCycleNs );
                  Double_t dDtX1X2 = ( fLastSortedHit2X.GetTs() * stsxyter::kdClockCycleNs - ulHitTs * stsxyter::kdClockCycleNs );
                  Double_t dDtX1Y2 = ( fLastSortedHit2Y.GetTs() * stsxyter::kdClockCycleNs - ulHitTs * stsxyter::kdClockCycleNs );

                  fhHodoSortedDtX1Y1->Fill( dDtX1Y1 );
                  fhHodoSortedDtX1X2->Fill( dDtX1X2 );
                  fhHodoSortedDtX1Y2->Fill( dDtX1Y2 );

                  if( TMath::Abs( dDtX1Y1 ) < fdCoincBorder )
                  {
                     UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit1Y.GetChan() );
                     if( fUnpackPar->IsYInvertedHodo1() )
                        uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                     fhHodoSortedMapX1Y1->Fill( uFiberIdx, uFiberIdxOther );
                     fhHodoSortedCntEvoX1Y1->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtX1Y1 ) < fdCoincBorder )
                  if( TMath::Abs( dDtX1X2 ) < fdCoincBorder )
                  {
                     UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit2X.GetChan() );
                     if( fUnpackPar->IsXInvertedHodo2() )
                        uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                     fhHodoSortedMapX1X2->Fill( uFiberIdx, uFiberIdxOther );
                     fhHodoSortedCntEvoX1X2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtX1X2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtX1Y2 ) < fdCoincBorder )
                  {
                     UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit2Y.GetChan() );
                     if( fUnpackPar->IsYInvertedHodo2() )
                        uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                     fhHodoSortedMapX1Y2->Fill( uFiberIdx, uFiberIdxOther );
                     fhHodoSortedCntEvoX1Y2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtX1Y2 ) < fdCoincBorder )
               } // if( bHitInX )
                  else
                  {
                     if( fUnpackPar->IsYInvertedHodo1() )
                        uFiberIdx = fuNbChanPerAsic/2 - 1 - uFiberIdx;
                     fLastSortedHit1Y = (*it);

                     Double_t dDtX1Y1 = ( ulHitTs * stsxyter::kdClockCycleNs - fLastSortedHit1X.GetTs() * stsxyter::kdClockCycleNs );
                     Double_t dDtY1Y2 = ( fLastSortedHit2Y.GetTs() * stsxyter::kdClockCycleNs - ulHitTs * stsxyter::kdClockCycleNs );
                     Double_t dDtY1X2 = ( fLastSortedHit2X.GetTs() * stsxyter::kdClockCycleNs - ulHitTs * stsxyter::kdClockCycleNs );

                     fhHodoSortedDtX1Y1->Fill( dDtX1Y1 );
                     fhHodoSortedDtY1Y2->Fill( dDtY1Y2 );
                     fhHodoSortedDtY1X2->Fill( dDtY1X2 );

                     if( TMath::Abs( dDtX1Y1 ) < fdCoincBorder )
                     {
                        UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit1X.GetChan() );
                        if( fUnpackPar->IsXInvertedHodo1() )
                           uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                        fhHodoSortedMapX1Y1->Fill( uFiberIdxOther, uFiberIdx );
                        fhHodoSortedCntEvoX1Y1->Fill( dTimeSinceStartSec );
                     } // if( TMath::Abs( dDtX1Y1 ) < fdCoincBorder )
                     if( TMath::Abs( dDtY1Y2 ) < fdCoincBorder )
                     {
                        UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit2Y.GetChan() );
                        if( fUnpackPar->IsYInvertedHodo2() )
                           uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                        fhHodoSortedMapY1Y2->Fill( uFiberIdx, uFiberIdxOther );
                        fhHodoSortedCntEvoY1Y2->Fill( dTimeSinceStartSec );
                     } // if( TMath::Abs( dDtY1Y2 ) < fdCoincBorder )
                     if( TMath::Abs( dDtY1X2 ) < fdCoincBorder )
                     {
                        UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit2X.GetChan() );
                        if( fUnpackPar->IsXInvertedHodo2() )
                           uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                        fhHodoSortedMapY1X2->Fill( uFiberIdx, uFiberIdxOther );
                        fhHodoSortedCntEvoY1X2->Fill( dTimeSinceStartSec );
                     } // if( TMath::Abs( dDtY1X2 ) < fdCoincBorder )
                  } // else of if( bHitInX )
            } // if( fUnpackPar->GetAsicIndexHodo1() == usAsicIdx )
            else if( fUnpackPar->GetAsicIndexHodo2() == usAsicIdx )
            {
               if( fUnpackPar->IsXySwappedHodo2() )
                  bHitInX = !bHitInX;

               if( bHitInX )
               {
                  if( fUnpackPar->IsXInvertedHodo2() )
                     uFiberIdx = fuNbChanPerAsic/2 - 1 - uFiberIdx;

                  fLastSortedHit2X = (*it);

                  Double_t dDtX2Y2 = ( fLastSortedHit2Y.GetTs() * stsxyter::kdClockCycleNs - ulHitTs * stsxyter::kdClockCycleNs );
                  Double_t dDtX1X2 = ( ulHitTs * stsxyter::kdClockCycleNs - fLastSortedHit1X.GetTs() * stsxyter::kdClockCycleNs );
                  Double_t dDtY1X2 = ( ulHitTs * stsxyter::kdClockCycleNs - fLastSortedHit1Y.GetTs() * stsxyter::kdClockCycleNs );

                  fhHodoSortedDtX2Y2->Fill( dDtX2Y2 );
                  fhHodoSortedDtX1X2->Fill( dDtX1X2 );
                  fhHodoSortedDtY1X2->Fill( dDtY1X2 );

                  if( TMath::Abs( dDtX2Y2 ) < fdCoincBorder )
                  {
                     UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit2Y.GetChan() );
                     if( fUnpackPar->IsYInvertedHodo2() )
                        uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                     fhHodoSortedMapX2Y2->Fill( uFiberIdx, uFiberIdxOther );
                     fhHodoSortedCntEvoX2Y2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtX2Y2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtX1X2 ) < fdCoincBorder )
                  {
                     UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit1X.GetChan() );
                     if( fUnpackPar->IsXInvertedHodo1() )
                        uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                     fhHodoSortedMapX1X2->Fill( uFiberIdxOther, uFiberIdx );
                     fhHodoSortedCntEvoX1X2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtX1X2 ) < fdCoincBorder )
                  if( TMath::Abs( dDtY1X2 ) < fdCoincBorder )
                  {
                     UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit1Y.GetChan() );
                     if( fUnpackPar->IsYInvertedHodo1() )
                        uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                     fhHodoSortedMapY1X2->Fill( uFiberIdxOther, uFiberIdx );
                     fhHodoSortedCntEvoY1X2->Fill( dTimeSinceStartSec );
                  } // if( TMath::Abs( dDtY1X2 ) < fdCoincBorder )
               } // if( bHitInX )
                  else
                  {
                     if( fUnpackPar->IsYInvertedHodo2() )
                        uFiberIdx = fuNbChanPerAsic/2 - 1 - uFiberIdx;

                     fLastSortedHit2Y = (*it);

                     Double_t dDtX2Y2 = ( ulHitTs * stsxyter::kdClockCycleNs - fLastSortedHit2X.GetTs() * stsxyter::kdClockCycleNs );
                     Double_t dDtY1Y2 = ( ulHitTs * stsxyter::kdClockCycleNs - fLastSortedHit1Y.GetTs() * stsxyter::kdClockCycleNs );
                     Double_t dDtX1Y2 = ( ulHitTs * stsxyter::kdClockCycleNs - fLastSortedHit1X.GetTs() * stsxyter::kdClockCycleNs );

                     fhHodoSortedDtX2Y2->Fill( dDtX2Y2 );
                     fhHodoSortedDtY1Y2->Fill( dDtY1Y2 );
                     fhHodoSortedDtX1Y2->Fill( dDtX1Y2 );

                     if( TMath::Abs( dDtX2Y2 ) < fdCoincBorder )
                     {
                        UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit2X.GetChan() );
                        if( fUnpackPar->IsXInvertedHodo2() )
                           uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                        fhHodoSortedMapX2Y2->Fill( uFiberIdxOther, uFiberIdx );
                        fhHodoSortedCntEvoX2Y2->Fill( dTimeSinceStartSec );
                     } // if( TMath::Abs( dDtX2Y2 ) < fdCoincBorder )
                     if( TMath::Abs( dDtY1Y2 ) < fdCoincBorder )
                     {
                        UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit1Y.GetChan() );
                        if( fUnpackPar->IsYInvertedHodo1() )
                           uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                        fhHodoSortedMapY1Y2->Fill( uFiberIdxOther, uFiberIdx );
                        fhHodoSortedCntEvoY1Y2->Fill( dTimeSinceStartSec );
                     } // if( TMath::Abs( dDtY1Y2 ) < fdCoincBorder )
                     if( TMath::Abs( dDtX1Y2 ) < fdCoincBorder )
                     {
                        UInt_t uFiberIdxOther = fUnpackPar->GetChannelToFiberMap( fLastSortedHit1X.GetChan() );
                        if( fUnpackPar->IsXInvertedHodo1() )
                           uFiberIdxOther = fuNbChanPerAsic/2 - 1 - uFiberIdxOther;
                        fhHodoSortedMapX1Y2->Fill( uFiberIdxOther, uFiberIdx );
                        fhHodoSortedCntEvoX1Y2->Fill( dTimeSinceStartSec );
                     } // if( TMath::Abs( dDtX1Y2 ) < fdCoincBorder )
                  } // else of if( bHitInX )
            } // else if( fUnpackPar->GetAsicIndexHodo2() == usAsicIdx )
         } // loop on hits untils hits within 100 ns of last one or last one itself are reached

         // Remove all hits which were already used
         fvmHitsInTs.erase( fvmHitsInTs.begin(), it );
      } // if( 0 < fvmHitsInTs.size() )
/*
      // Remove all hits from this TS
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

void CbmCosy2018MonitorHodo::FillHitInfo( stsxyter::Message mess, const UShort_t & /*usElinkIdx*/, const UInt_t & uAsicIdx, const UInt_t & uMsIdx )
{
   UShort_t usChan     = mess.GetHitChannel();
   UShort_t usRawAdc   = mess.GetHitAdc();
//   UShort_t usFullTs = mess.GetHitTimeFull();
//   Bool_t   bMissedEvt = mess.IsHitMissedEvts();
   UShort_t usRawTs    = mess.GetHitTime();

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
//   fvmChanHitsInTs[        uAsicIdx ][ usChan ].insert( stsxyter::FinalHit( fvulChanLastHitTime[ uAsicIdx ][ usChan ],
//                                                                            usRawAdc, uAsicIdx, usChan ) );
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
         fhHodoChanHitRateEvo1Y->Fill( dTimeSinceStartSec, uFiberInHodo );
         fhHodoRateEvo1Y->Fill(   dTimeSinceStartSec );
//         fhHodoChanHitRateEvo1Y->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
//         fhHodoRateEvo1Y->Fill(   dTimeSinceStartMin, 1.0/60.0 );
      } // if( fUnpackPar->GetChannelToPlaneMapHodo1( usChan ) )
         else
         {
            // X
            fhHodoChanCounts1X->Fill( uFiberInHodo );
            fhHodoChanAdcRaw1X->Fill( uFiberInHodo, usRawAdc );
            fhHodoChanHitRateEvo1X->Fill( dTimeSinceStartSec, uFiberInHodo );
            fhHodoRateEvo1X->Fill(   dTimeSinceStartSec );
//            fhHodoChanHitRateEvo1X->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
//            fhHodoRateEvo1X->Fill(   dTimeSinceStartMin, 1.0/60.0 );
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
         fhHodoChanHitRateEvo2Y->Fill( dTimeSinceStartSec, uFiberInHodo );
         fhHodoRateEvo2Y->Fill(   dTimeSinceStartSec );
//         fhHodoChanHitRateEvo2Y->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
//         fhHodoRateEvo2Y->Fill(   dTimeSinceStartMin, 1.0/60.0 );
      } // if( fUnpackPar->GetChannelToPlaneMapHodo2( usChan ) )
         else
         {
            // X
            fhHodoChanCounts2X->Fill( uFiberInHodo );
            fhHodoChanAdcRaw2X->Fill( uFiberInHodo, usRawAdc );
            fhHodoChanHitRateEvo2X->Fill( dTimeSinceStartSec, uFiberInHodo );
            fhHodoRateEvo2X->Fill(   dTimeSinceStartSec );
//            fhHodoChanHitRateEvo2X->Fill( dTimeSinceStartMin, uFiberInHodo, 1.0/60.0 );
//            fhHodoRateEvo2X->Fill(   dTimeSinceStartMin, 1.0/60.0 );
         } // else of if( fUnpackPar->GetChannelToPlaneMapHodo2( usChan ) )
   } // else if( fUnpackPar->GetAsicIndexHodo2() == uAsicIdx )

}

void CbmCosy2018MonitorHodo::FillTsMsbInfo( stsxyter::Message mess, UInt_t /*uMessIdx*/, UInt_t /*uMsIdx*/ )
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
}

void CbmCosy2018MonitorHodo::FillEpochInfo( stsxyter::Message /*mess*/ )
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

void CbmCosy2018MonitorHodo::Reset()
{
}

void CbmCosy2018MonitorHodo::Finish()
{

   LOG(info) << "-------------------------------------";
   LOG(info) << "CbmCosy2018MonitorHodo statistics are ";
   LOG(info) << " Hit      messages: " << fmMsgCounter[ stsxyter::MessType::Hit   ] << "\n"
             << " Ts MSB   messages: " << fmMsgCounter[ stsxyter::MessType::TsMsb ] << "\n"
             << " Dummy    messages: " << fmMsgCounter[ stsxyter::MessType::Dummy ] << "\n"
             << " Epoch    messages: " << fmMsgCounter[ stsxyter::MessType::Epoch ] << "\n"
             << " Empty    messages: " << fmMsgCounter[ stsxyter::MessType::Empty ];

   LOG(info) << "-------------------------------------";

   SaveAllHistos( "data/HodoHistos.root" );
   SaveAllHistos();

}


void CbmCosy2018MonitorHodo::FillOutput(boost::any)
{
}

void CbmCosy2018MonitorHodo::SaveAllHistos( TString sFileName )
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

//   fhHodoFebTsMsb->Write();

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
void CbmCosy2018MonitorHodo::ResetAllHistos()
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

//   fhHodoFebTsMsb->Reset();

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

void CbmCosy2018MonitorHodo::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new MUCH Run Start Date-Time: " << fRunStartDateTime->AsString();
}

void CbmCosy2018MonitorHodo::SetLongDurationLimits( UInt_t uDurationSeconds, UInt_t uBinSize )
{
   fbLongHistoEnable     = kTRUE;
   fuLongHistoNbSeconds  = uDurationSeconds;
   fuLongHistoBinSizeSec = uBinSize;
}

ClassImp(CbmCosy2018MonitorHodo)
