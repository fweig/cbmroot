// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorTofLab                        -----
// -----               Created 15.12.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTSMonitorTofLab.h"
#include "CbmTofUnpackPar.h"

//#include "CbmFlibCern2016Source.h"
#include "CbmHistManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TString.h"
#include "THttpServer.h"
#include "Rtypes.h"
#include "TH1.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TDatime.h"
#include <TFile.h>

#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <ctime>

namespace MonitorTofLab {
   static Int_t iMess = 0;
//   static Int_t iMessB = 0;
   Bool_t bResetTofLabHistos = kFALSE;
   Bool_t bTofLabCyclePulserFee  = kFALSE;

   // Default value for nb bins in Pulser time difference histos
   //const UInt_t kuNbBinsDt    = 5000;
   const UInt_t kuNbBinsDt    = 150;
   Double_t dMinDt     = -1.*(kuNbBinsDt*get4v1x::kdBinSize/2.) - get4v1x::kdBinSize/2.;
   Double_t dMaxDt     =  1.*(kuNbBinsDt*get4v1x::kdBinSize/2.) + get4v1x::kdBinSize/2.;
   //const UInt_t kuNbBinsDt    = 299;
   //Double_t dMinDt     = -1.*((kuNbBinsDt+1)*get4v1x::kdBinSize/2.);
   //Double_t dMaxDt     =  1.*((kuNbBinsDt+1)*get4v1x::kdBinSize/2.);

   // Default number of FEET per channels histograms
   UInt_t uNbFeetPlot = 2;
}

using namespace MonitorTofLab;

CbmTSMonitorTofLab::CbmTSMonitorTofLab() :
    CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuOverlapMsNb(0),
    fuMinNbGdpb(),
    fuCurrNbGdpb(0),
    fNrOfGdpbs(0),
    fNrOfFebsPerGdpb(0),
    fNrOfGet4PerFeb(0),
    fNrOfChannelsPerGet4(0),
    fNrOfChannelsPerFeet(0),
    fNrOfGet4(0),
    fNrOfGet4PerGdpb(0),
    fiCountsLastTs(0),
    fiSpillOnThr(10),
    fiSpillOffThr(3),
    fiTsUnderOff(0),
    fiTsUnderOffThr(10),
    fdDetLastTime(-1.),
    fdDetTimeLastTs(-1.),
    fbSpillOn(kFALSE),
    fSpillIdx(0),
    fbEpochSuppModeOn(kFALSE),
    fvmEpSupprBuffer(),
    fvmEpSupprBufferErrors(),
    fGdpbId(0),
    fGdpbNr(0),
    fGet4Id(0),
    fGet4Nr(0),
    fMsgCounter(11, 0), // length of enum MessageTypes initialized with 0
    fGdpbIdIndexMap(),
    fHM(new CbmHistManager()),
    fCurrentEpoch(NULL),
    fuCurrentEpochCycle(NULL),
    fNofEpochs(0),
    fCurrentEpochTime(0.),
    fdStartTime(-1.),
    fdStartTimeMsSz(-1.),
    fcMsSizeAll(NULL),
    fTsLastHit(),
    fEquipmentId(0),
    fUnpackPar(NULL),
    fTimeLastHistoSave(),
    fHistMessType(NULL),
    fHistSysMessType(NULL),
    fHistGet4MessType(NULL),
    fHistGet4ChanErrors(NULL),
    fHistGet4EpochFlags(NULL),
    fHistSpill(NULL),
    fHistSpillLength(NULL),
    fHistSpillCount(NULL),
    fHistSpillQA(NULL),
    fRaw_Tot_gDPB(),
    fChCount_gDPB(),
    fChannelRate_gDPB(),
    fFeetRate_gDPB(),
    fFeetErrorRate_gDPB(),
    fFeetRateLong_gDPB(),
    fFeetErrorRateLong_gDPB(),
    fFeetRateDate_gDPB(),
    fiRunStartDateTimeSec( -1 ),
    fiBinSizeDatePlots( -1 ),
    fbEnableCoincMap(kFALSE),
    fulLastMsIdx(0),
    fbHitsInLastTs(kFALSE),
    fvulHitEpochBuffLastTs(),
    fvhCoincOffsetEpochGet4(),
    fulGdpbTsMsb(0),
    fulGdpbTsLsb(0),
    fulStarTsMsb(0),
    fulStarTsMid(0),
    fulGdpbTsFullLast(0),
    fulStarTsFullLast(0),
    fuStarTokenLast(0),
    fuStarDaqCmdLast(0),
    fuStarTrigCmdLast(0),
    fhTokenMsgType(NULL),
    fhTriggerRate(NULL),
    fhCmdDaqVsTrig(NULL),
    fhStarTokenEvo(NULL),
    fbGet4M24b( kFALSE ),
    fbGet4v20( kFALSE ),
    fbPulserMode( kFALSE ),
    fuPulserGdpb(0),
    fuPulserFee(0),
    fhTimeDiffPulserChosenFee(),
    fhTimeDiffPulserChosenChPairs(),
    fhTimeRmsPulserChosenFee(NULL),
    fhTimeRmsPulserChosenChPairs(NULL),
    fdLastRmsUpdateTime(-1),
    fhFtDistribPerCh(),
    fChCountFall_gDPB(),
    fhFtDistribPerChFall(),
    fviFtLastRise24b(),
    fviFtLastFall24b(),
    fvdTimeLastRise24b(),
    fvdTimeLastFall24b(),
    fuRiseFallChSel(0),
    fhFtLastRiseCurrFall(),
    fhFtCurrRiseLastFall(),
    fhFtLastRiseDistRise(),
    fhFtLastRiseDistFall()
{
}

CbmTSMonitorTofLab::~CbmTSMonitorTofLab()
{
  delete fHM; //TODO: Who deletes the histograms stored in the CbmHistManager???
  delete[] fCurrentEpoch;
  delete[] fuCurrentEpochCycle;
}

Bool_t CbmTSMonitorTofLab::Init()
{
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmTSMonitorTofLab::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar =
      (CbmTofUnpackPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
          "CbmTofUnpackPar"));

}

Bool_t CbmTSMonitorTofLab::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  fCurrentEpoch = new Long64_t[fNrOfGdpbs * fNrOfGet4PerGdpb];
  fuCurrentEpochCycle = new UInt_t[fNrOfGdpbs * fNrOfGet4PerGdpb];
  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    for (UInt_t j = 0; j < fNrOfGet4PerGdpb; ++j) {
      fCurrentEpoch[GetArrayIndex(i, j)] = -111;
      fuCurrentEpochCycle[GetArrayIndex(i, j)] = 0;
    }
  }

  return initOK;
}

Bool_t CbmTSMonitorTofLab::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for " << GetName();

  fNrOfGdpbs = fUnpackPar->GetNrOfRocs();
  LOG(info) << "Nr. of Tof GDPBs: " << fNrOfGdpbs;
  fuMinNbGdpb = fNrOfGdpbs;

  fNrOfFebsPerGdpb = fUnpackPar->GetNrOfFebsPerGdpb();
  LOG(info) << "Nr. of FEBS per Tof GDPB: " << fNrOfFebsPerGdpb;

  fNrOfGet4PerFeb = fUnpackPar->GetNrOfGet4PerFeb();
  LOG(info) << "Nr. of GET4 per Tof FEB: " << fNrOfGet4PerFeb;

  fNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
  LOG(info) << "Nr. of channels per GET4: " << fNrOfChannelsPerGet4;

  fNrOfChannelsPerFeet = fNrOfGet4PerFeb * fNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per FEET: " << fNrOfChannelsPerFeet;

  fNrOfGet4 = fNrOfGdpbs * fNrOfFebsPerGdpb * fNrOfGet4PerFeb;
  LOG(info) << "Nr. of GET4s: " << fNrOfGet4;

  fNrOfGet4PerGdpb = fNrOfFebsPerGdpb * fNrOfGet4PerFeb;
  LOG(info) << "Nr. of GET4s per GDPB: " << fNrOfGet4PerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
    LOG(info) << "GDPB Id of TOF  " << i << " : " << fUnpackPar->GetRocId(i);
  }
  Int_t NrOfChannels = fUnpackPar->GetNumberOfChannels();
  LOG(info) << "Nr. of mapped Tof channels: " << NrOfChannels;
  std::stringstream ss;
	for (Int_t i = 0; i< NrOfChannels; ++i) {
      if(i%8 == 0)
         ss << "\n";
      ss << Form(" 0x%08x",fUnpackPar->GetChannelToDetUIdMap(i));
	}
   LOG(info) << ss.str();

  LOG(info) << "Plot Channel Rate => "
               << (fUnpackPar->IsChannelRateEnabled() ? "ON" : "OFF");

   if( fbEpochSuppModeOn )
   {
      fvmEpSupprBuffer.resize( fNrOfGet4 );
      fvmEpSupprBufferErrors.resize( fNrOfGet4 );
   } // if( fbEpochSuppModeOn )

  return kTRUE;
}

void CbmTSMonitorTofLab::CreateHistograms()
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
//  server->SetJSROOT("https://root.cern.ch/js/latest");
  LOG(info) << "Using Http Server " << server
            << "with JSROOT = ";

  TString name { "" };
  TString title { "" };

  name = "hMessageType";
  title = "Nb of message for each type; Type";
  // Test Big Data readout with plotting
  TH1I* hMessageType = new TH1I(name, title, 16, 0., 16.);
  //  TH1I* hMessageType = new TH1I(name, title, 16, -0.5, 15.5);
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_NOP, "NOP");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_HIT, "HIT");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_EPOCH, "EPOCH");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_SYNC, "SYNC");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_AUX, "AUX");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_EPOCH2, "EPOCH2");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4, "GET4");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_SYS, "SYS");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_SLC,
      "MSG_GET4_SLC");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_32B,
      "MSG_GET4_32B");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_SYS,
      "MSG_GET4_SYS");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_STAR_TRI,
      "MSG_STAR_TRI");
  hMessageType->GetXaxis()->SetBinLabel(1 + 15, "GET4 Hack 32B");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_NOP, "NOP");
  fHM->Add(name.Data(), hMessageType);
  if (server)
    server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hSysMessType";
  title = "Nb of system message for each type; System Type";
  TH1I* hSysMessType = new TH1I(name, title, 17, 0., 17.);
//  TH1I* hSysMessType = new TH1I(name, title, 17, -0.5, 16.5);
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,
      "DAQ START");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,
      "DAQ FINISH");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_NX_PARITY,
      "NX PARITY");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_SYNC_PARITY,
      "SYNC PARITY");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_RESUME,
      "DAQ RESUME");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_FIFO_RESET,
      "FIFO RESET");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_USER, "USER");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_PCTIME, "PCTIME");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_ADC, "ADC");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_PACKETLOST,
      "PACKET LOST");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_GET4_EVENT,
      "GET4 ERROR");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_CLOSYSYNC_ERROR,
      "CLOSYSYNC ERROR");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_TS156_SYNC,
      "TS156 SYNC");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_GDPB_UNKWN,
      "UNKW GET4 MSG");
  hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
  fHM->Add(name.Data(), hSysMessType);
  if (server)
    server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hGet4MessType";
  title = "Nb of message for each type per GET4; GET4 chip # ; Type";
  TH2I* hGet4MessType = new TH2I(name, title, fNrOfGet4, 0., fNrOfGet4, 5, 0.,
      5.);
//  TH2I* hGet4MessType = new TH2I(name, title, uNbFeets*feetv1::kuChipPerFeet, -0.5, uNbFeets*feetv1::kuChipPerFeet -0.5, 5, -0.5, 4.5);
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_EPOCH, "EPOCH");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_SLCM, "S.C. M");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_ERROR, "ERROR");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_DATA, "DATA 32b");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_DATA + 1,
      "DATA 24b");
  fHM->Add(name.Data(), hGet4MessType);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hGet4ChanErrors";
  title = "Error messages per GET4 channel; GET4 channel # ; Error";
  TH2I* hGet4ChanErrors = NULL;
  if( kTRUE == fbGet4v20 )
  {
     /// Possible errors changed after introduction of Bubbles correction/detection
     hGet4ChanErrors = new TH2I(name, title,
      fNrOfGet4 * fNrOfChannelsPerGet4, 0., fNrOfGet4 * fNrOfChannelsPerGet4,
      21, 0., 21.);
//      32, 0., 32.);
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 1, "0x00: Readout Init    ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 2, "0x01: Sync            ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 3, "0x02: Epoch count sync");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 4, "0x03: Epoch           ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 5, "0x04: FIFO Write      ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 6, "0x05: Lost event      ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 7, "0x06: Channel state   ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 8, "0x07: Token Ring state");
     hGet4ChanErrors->GetYaxis()->SetBinLabel( 9, "0x08: Token           ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
     hGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc."); // <- From GET4 v1.2
     hGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x14: Add. Rising edge"); // <- From GET4 v1.3
     hGet4ChanErrors->GetYaxis()->SetBinLabel(18, "0x15: Unpaired Falling"); // <- From GET4 v1.3
     hGet4ChanErrors->GetYaxis()->SetBinLabel(19, "0x16: Sequence error  "); // <- From GET4 v1.3
     hGet4ChanErrors->GetYaxis()->SetBinLabel(20, "0x7f: Unknown         ");
     hGet4ChanErrors->GetYaxis()->SetBinLabel(21, "Corrupt/unsuprtd error");
  } // if( kTRUE == fbGet4v20 )
     else
     {
        hGet4ChanErrors = new TH2I(name, title,
         fNrOfGet4 * fNrOfChannelsPerGet4, 0., fNrOfGet4 * fNrOfChannelsPerGet4,
         18, 0., 18.);
   //      32, 0., 32.);
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 1, "0x00: Readout Init    ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 2, "0x01: Sync            ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 3, "0x02: Epoch count sync");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 4, "0x03: Epoch           ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 5, "0x04: FIFO Write      ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 6, "0x05: Lost event      ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 7, "0x06: Channel state   ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 8, "0x07: Token Ring state");
        hGet4ChanErrors->GetYaxis()->SetBinLabel( 9, "0x08: Token           ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
        hGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc."); // <- From GET4 v1.2
        hGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
        hGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
        hGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x7f: Unknown         ");
     } // else of if( kTRUE == fbGet4v20 )
  fHM->Add(name.Data(), hGet4ChanErrors);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hGet4EpochFlags";
  title = "Epoch flags per GET4; GET4 chip # ; Type";
  TH2I* hGet4EpochFlags = new TH2I(name, title, fNrOfGet4, 0., fNrOfGet4, 4, 0.,
      4.);
//  hEpochFlags = new TH2I(name, title, uNbFeets*feetv1::kuChipPerFeet, -0.5, uNbFeets*feetv1::kuChipPerFeet -0.5, 4, -0.5, 3.5);
  hGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
  hGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
  hGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
  hGet4EpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");
  fHM->Add(name.Data(), hGet4EpochFlags);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

   // Prepare storing of hit time info
   if( fUnpackPar->IsChannelRateEnabled() || fbPulserMode )
   {
      fTsLastHit.resize( fNrOfGdpbs );
      for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb++)
      {
         fTsLastHit[ uGdpb ].resize( fNrOfGet4PerGdpb );
         for( UInt_t uGet4 = 0; uGet4 < fNrOfGet4PerGdpb; uGet4++)
         {
            fTsLastHit[ uGdpb ][ uGet4 ].resize( fNrOfChannelsPerGet4 );
            for( UInt_t uCh = 0; uCh < fNrOfChannelsPerGet4; uCh++)
               fTsLastHit[ uGdpb ][ uGet4 ][ uCh ] = -1;
         } // for( UInt_t uGet4 = 0; uGet4 < fNrOfGet4PerGdpb; uGet4++)
      } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb++)
   } // if( fUnpackPar->IsChannelRateEnabled() || fbPulserMode )

  if (fUnpackPar->IsChannelRateEnabled()) {
    for (UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb++) {
      const Int_t iNbBinsRate = 82;
      Double_t dBinsRate[iNbBinsRate] = { 1e0, 2e0, 3e0, 4e0, 5e0, 6e0, 7e0,
          8e0, 9e0, 1e1, 2e1, 3e1, 4e1, 5e1, 6e1, 7e1, 8e1, 9e1, 1e2, 2e2, 3e2,
          4e2, 5e2, 6e2, 7e2, 8e2, 9e2, 1e3, 2e3, 3e3, 4e3, 5e3, 6e3, 7e3, 8e3,
          9e3, 1e4, 2e4, 3e4, 4e4, 5e4, 6e4, 7e4, 8e4, 9e4, 1e5, 2e5, 3e5, 4e5,
          5e5, 6e5, 7e5, 8e5, 9e5, 1e6, 2e6, 3e6, 4e6, 5e6, 6e6, 7e6, 8e6, 9e6,
          1e7, 2e7, 3e7, 4e7, 5e7, 6e7, 7e7, 8e7, 9e7, 1e8, 2e8, 3e8, 4e8, 5e8,
          6e8, 7e8, 8e8, 9e8, 1e9 };
      name = Form("ChannelRate_gDPB_%02u_0", uGdpb);
      title = Form("Channel instant rate gDPB %02u; Dist[ns] ; Channel", uGdpb);
      fHM->Add(name.Data(),
          new TH2F( name.Data(), title.Data(), iNbBinsRate - 1, dBinsRate,
                    uNbFeetPlot*fNrOfChannelsPerFeet, 0, uNbFeetPlot*fNrOfChannelsPerFeet));
      if (server)
        server->Register("/TofRaw", fHM->H2(name.Data()));
      if( uNbFeetPlot < fNrOfFebsPerGdpb  )
      {
         name = Form("ChannelRate_gDPB_%02u_1", uGdpb);
         title = Form("Channel instant rate gDPB %02u; Dist[ns] ; Channel", uGdpb);
         fHM->Add(name.Data(),
             new TH2F( name.Data(), title.Data(), iNbBinsRate - 1, dBinsRate,
                        uNbFeetPlot*fNrOfChannelsPerFeet,
                        uNbFeetPlot*fNrOfChannelsPerFeet,
                        2*uNbFeetPlot*fNrOfChannelsPerFeet));
         if (server)
           server->Register("/TofRaw", fHM->H2(name.Data()));
      } // if( uNbFeetPlot < fNrOfFebsPerGdpb  )
      if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )
      {
         name = Form("ChannelRate_gDPB_%02u_2", uGdpb);
         title = Form("Channel instant rate gDPB %02u; Dist[ns] ; Channel", uGdpb);
         fHM->Add(name.Data(),
             new TH2F( name.Data(), title.Data(), iNbBinsRate - 1, dBinsRate,
                        uNbFeetPlot*fNrOfChannelsPerFeet,
                        2*uNbFeetPlot*fNrOfChannelsPerFeet,
                        3*uNbFeetPlot*fNrOfChannelsPerFeet));
         if (server)
           server->Register("/TofRaw", fHM->H2(name.Data()));
      } // if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )

      LOG(info) << "Adding the rate histos";
    } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  }

  if( fbPulserMode )
  {
      // Full Fee time difference test
      UInt_t uNbBinsDt = kuNbBinsDt + 1; // To account for extra bin due to shift by 1/2 bin of both ranges
      for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)
      {
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
         {
            fhTimeDiffPulserChosenFee.push_back(
               new TH1I(
                  Form("hTimeDiffPulserChosenFee_%03u_%03u", uChanFeeA, uChanFeeB),
                  Form("Time difference for channels %03u and %03u in chosen Fee; DeltaT [ps]; Counts",
                        uChanFeeA, uChanFeeB),
                  uNbBinsDt, dMinDt, dMaxDt) );
         } // for any unique pair of channel in chosen Fee
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)

      // Selected channels test
      fhTimeDiffPulserChosenChPairs.resize( kuNbChanTest - 1 );
      for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
      {
         fhTimeDiffPulserChosenChPairs[uChan]  = new TH1I(
               Form("hTimeDiffPulserChosenChPairs_%03u_%03u", fuPulserChan[uChan], fuPulserChan[uChan+1]),
               Form("Time difference for selected channels %03u and %03u in the first gDPB; DeltaT [ps]; Counts",
                     fuPulserChan[uChan], fuPulserChan[uChan+1]),
               uNbBinsDt, dMinDt, dMaxDt);
         if (server)
           server->Register("/TofRaw", fhTimeDiffPulserChosenChPairs[uChan] );
      } // for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)

      name = "hTimeRmsPulserChosenFee";
      fhTimeRmsPulserChosenFee = new TH2D( name.Data(),
            "Time difference RMS for any channels pair in chosen Fee; Ch A; Ch B; RMS [ps]",
            fNrOfChannelsPerFeet - 1, -0.5, fNrOfChannelsPerFeet - 1.5,
            fNrOfChannelsPerFeet - 1,  0.5, fNrOfChannelsPerFeet - 0.5);
      fHM->Add( name.Data(), fhTimeRmsPulserChosenFee);
      if (server)
        server->Register("/TofRaw", fHM->H2( name.Data() ) );

      name = "hTimeRmsPulserChosenChPairs";
      fhTimeRmsPulserChosenChPairs = new TH1D( name.Data(),
            "Time difference RMS for chosen channels pairs; Pair # ; [ps]",
            kuNbChanTest - 1, -0.5, kuNbChanTest - 1.5);
      fHM->Add( name.Data(), fhTimeRmsPulserChosenChPairs);
      if (server)
        server->Register("/TofRaw", fHM->H1( name.Data() ) );
  } // if( fbPulserMode )

  if( fbGet4M24b )
  {
     fviFtLastRise24b.resize( fNrOfGdpbs * fNrOfFebsPerGdpb );
     fviFtLastFall24b.resize( fNrOfGdpbs * fNrOfFebsPerGdpb );
     fvdTimeLastRise24b.resize( fNrOfGdpbs * fNrOfFebsPerGdpb );
     fvdTimeLastFall24b.resize( fNrOfGdpbs * fNrOfFebsPerGdpb );
  } // if( fbGet4M24b )

  for (UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb++) {
    name = Form("Raw_Tot_gDPB_%02u_0", uGdpb);
    title = Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb);
    fHM->Add(name.Data(),
        new TH2F(name.Data(), title.Data(),
                 uNbFeetPlot*fNrOfChannelsPerFeet, 0, uNbFeetPlot*fNrOfChannelsPerFeet,
                 256, 0, 256));
    if (server)
      server->Register("/TofRaw", fHM->H2(name.Data()));
    if( uNbFeetPlot < fNrOfFebsPerGdpb )
    {
      name = Form("Raw_Tot_gDPB_%02u_1", uGdpb);
      title = Form("Raw TOT gDPB %02u RPC 1; channel; TOT [bin]", uGdpb);
      fHM->Add(name.Data(),
               new TH2F(name.Data(), title.Data(),
                  uNbFeetPlot*fNrOfChannelsPerFeet,
                  uNbFeetPlot*fNrOfChannelsPerFeet,
                  2*uNbFeetPlot*fNrOfChannelsPerFeet,
                  256, 0, 256));
      if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));
    } // if( uNbFeetPlot < fNrOfFebsPerGdpb  )
    if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )
    {
      name = Form("Raw_Tot_gDPB_%02u_2", uGdpb);
      title = Form("Raw TOT gDPB %02u RPC 2; channel; TOT [bin]", uGdpb);
      fHM->Add(name.Data(),
               new TH2F(name.Data(), title.Data(),
                  uNbFeetPlot*fNrOfChannelsPerFeet,
                  2*uNbFeetPlot*fNrOfChannelsPerFeet,
                  3*uNbFeetPlot*fNrOfChannelsPerFeet,
                  256, 0, 256));
      if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));
    } // if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )

    name = Form("ChCount_gDPB_%02u", uGdpb);
    title = Form("Channel counts gDPB %02u; channel; Hits", uGdpb);
    fHM->Add(name.Data(), new TH1I(name.Data(), title.Data(),
             fNrOfFebsPerGdpb*fNrOfChannelsPerFeet, 0, fNrOfFebsPerGdpb*fNrOfChannelsPerFeet));

    if (server)
      server->Register("/TofRaw", fHM->H1(name.Data()));

    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      name = Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
      title = Form(
          "Counts per second in Feet %1u of gDPB %02u; Time[s] ; Counts", uFeet,
          uGdpb);
      fHM->Add(name.Data(), new TH1F(name.Data(), title.Data(), 1800, 0, 1800));
      if (server)
        server->Register("/TofRaw", fHM->H1(name.Data()));

      name = Form("FeetErrorRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
      title = Form(
          "Error Counts per second in Feet %1u of gDPB %02u; Time[s] ; Error Counts", uFeet,
          uGdpb);
      fHM->Add(name.Data(), new TH1F(name.Data(), title.Data(), 1800, 0, 1800));
      if (server)
        server->Register("/TofRaw", fHM->H1(name.Data()));
      /// Long duration rate plots: 1 minute binning, 5 days range
      name = Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
      title = Form(
          "Counts per minute in Feet %1u of gDPB %02u; Time[min] ; Counts", uFeet,
          uGdpb);
      fHM->Add(name.Data(), new TH1F(name.Data(), title.Data(), 5*24*60, 0, 5*24*60));
      if (server)
        server->Register("/TofRaw", fHM->H1(name.Data()));

      name = Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
      title = Form(
          "Error Counts per minute in Feet %1u of gDPB %02u; Time[min] ; Error Counts", uFeet,
          uGdpb);
      fHM->Add(name.Data(), new TH1F(name.Data(), title.Data(), 5*24*60, 0, 5*24*60));
      if (server)
        server->Register("/TofRaw", fHM->H1(name.Data()));

      if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
        name = Form("FeetRateDate_gDPB_g%02u_f%1u", uGdpb, uFeet);
        title = Form(
          "Counts per second in Feet %1u of gDPB %02u; Time[s] ; Counts", uFeet,
          uGdpb);
        fHM->Add(name.Data(), new TH1F(name.Data(), title.Data(), (5400 / fiBinSizeDatePlots), -10, 5400 - 10));
        ( fHM->H1(name.Data()) )->GetXaxis()->SetTimeDisplay(1);
        ( fHM->H1(name.Data()) )->GetXaxis()->SetTimeOffset( fiRunStartDateTimeSec );
        if (server)
          server->Register("/TofRaw", fHM->H1(name.Data()));
      } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )

      name = Form("FtDistribPerCh_gDPB_g%02u_f%1u", uGdpb, uFeet);
      title = Form(
          "FT distribution per channel in Feet %1u of gDPB %02u; Channel [] ; FT [bin]; Counts []", uFeet,
          uGdpb);
      fHM->Add(name.Data(), new TH2I( name.Data(), title.Data(),
                                      fNrOfChannelsPerFeet, 0, fNrOfChannelsPerFeet,
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize));
      if (server)
        server->Register("/TofRaw", fHM->H2(name.Data()));

      if( fbGet4M24b )
      {
          name = Form("ChCountFall_gDPB_%02u", uGdpb);
          title = Form("Channel falling edge counts gDPB %02u; channel; Hits", uGdpb);
          fHM->Add(name.Data(), new TH1I(name.Data(), title.Data(),
                   fNrOfFebsPerGdpb*fNrOfChannelsPerFeet, 0, fNrOfFebsPerGdpb*fNrOfChannelsPerFeet));
          if (server)
            server->Register("/TofRaw", fHM->H1(name.Data()));

         name = Form("FtDistribPerChFall_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
          "FT distribution per channel, falling edge, in Feet %1u of gDPB %02u; Channel [] ; FT Fall [bin]; Counts []", uFeet,
          uGdpb);
         fHM->Add(name.Data(), new TH2I( name.Data(), title.Data(),
                                      fNrOfChannelsPerFeet, 0, fNrOfChannelsPerFeet,
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize));
         if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));

        fviFtLastRise24b[ uGdpb * fNrOfFebsPerGdpb + uFeet ].resize( fNrOfChannelsPerFeet, -1 );
        fviFtLastFall24b[ uGdpb * fNrOfFebsPerGdpb + uFeet ].resize( fNrOfChannelsPerFeet, -1 );
        fvdTimeLastRise24b[ uGdpb * fNrOfFebsPerGdpb + uFeet ].resize( fNrOfChannelsPerFeet, -1 );
        fvdTimeLastFall24b[ uGdpb * fNrOfFebsPerGdpb + uFeet ].resize( fNrOfChannelsPerFeet, -1 );

         name = Form("FtLastRiseCurrFall_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
          "FT distribution last rising vs falling edge, in Feet %1u of gDPB %02u; FT last Rise [bin] ; FT Fall [bin]; Counts []", uFeet,
          uGdpb);
         fHM->Add(name.Data(), new TH2I( name.Data(), title.Data(),
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize,
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize));
         if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));

         name = Form("FtCurrRiseLastFall_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
          "FT distribution rising vs last falling edge, in Feet %1u of gDPB %02u; FT Rise [bin] ; FT last Fall [bin]; Counts []", uFeet,
          uGdpb);
         fHM->Add(name.Data(), new TH2I( name.Data(), title.Data(),
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize,
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize));
         if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));

         name = Form("FtLastRiseDistRise_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
          "Time between rising edges vs rising edge FT, in Feet %1u of gDPB %02u; FT last Rise [] ; Trise - TriseLast[ns]; Counts []", uFeet,
          uGdpb);
         fHM->Add(name.Data(), new TH2I( name.Data(), title.Data(),
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize,
                                      500, 0.0, 25.0 ));
         if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));

         name = Form("FtLastRiseDistFall_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
          "Time between falling edges vs rising edge FT, in Feet %1u of gDPB %02u; FT last Rise [] ; Tfall - TfallLast[ns]; Counts []", uFeet,
          uGdpb);
         fHM->Add(name.Data(), new TH2I( name.Data(), title.Data(),
                                      get4v1x::kuFineCounterSize, 0, get4v1x::kuFineCounterSize,
                                      500, 0.0, 25.0 ));
         if (server)
         server->Register("/TofRaw", fHM->H2(name.Data()));

      } // if( fbGet4M24b )
    } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
  } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)

  ///* ASIC coincidences & offsets mapping *///
  if( fbEnableCoincMap )
  {
     fvulHitEpochBuffLastTs.resize(fNrOfGet4);  //! Dims: [gDPB][hits]
     for( UInt_t uAsicA = 0; uAsicA < fNrOfGet4 - 1; uAsicA ++ )
     {
         name = Form("fvhCoincOffsetEpochGet4__g%03u", uAsicA);
         title = Form(
             "Distance in epoch between hits in Get4 %03u and hits in another Get4 ASIC in same TS; GET4 B index [] ; Hit B - Hit A [Epoch]; Counts []",
             uAsicA );
         TH2* ph2 = new TH2I( name.Data(), title.Data(),
                  fNrOfGet4 - uAsicA - 1, uAsicA + 1, fNrOfGet4,
                  20000, -10000, 10000
                  );
         fvhCoincOffsetEpochGet4.push_back( ph2 );

         fHM->Add( name.Data(), ph2 );
         if (server)
            server->Register("/TofCoinc", fHM->H2(name.Data()));
     } // for( UInt_t uAsicA = 0; uAsicA < fNrOfGet4 - 1; uAsicA ++ )
  } // if( fbEnableCoincMap )

  name = "hSpill";
  title = "Counts per channel in Current Spill; X [Strip]; Y [End]; Counts";
  TH2I* hSpill = new TH2I(name, title, fNrOfChannelsPerFeet, 0., fNrOfChannelsPerFeet, 2, 0., 2.);
  fHM->Add(name.Data(), hSpill);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hSpillLength";
  title = "Length of spill interval as found from detectors; Length [s]; Counts";
  TH1* hSpillLength = new TH1F(name, title, 3000, 0., 300.);
  fHM->Add(name.Data(), hSpillLength);
  if (server)
    server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hSpillCount";
  title = "Total counts in detector in each spill; Spill; Counts";
  TH1* hSpillCount = new TH1F(name, title, 300, 0., 300.);
  fHM->Add(name.Data(), hSpillCount);
  if (server)
    server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hSpillQA";
  title = "Total counts in detector per spill VS Spill length; Length [s]; Counts";
  TH2* hSpillQA = new TH2F(name, title, 120, 0., 120., 150, 0., 150000.);
  fHM->Add(name.Data(), hSpillQA);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hTokenMsgType";
  title = "STAR trigger Messages type; Type ; Counts";
  fhTokenMsgType = new TH1F(name, title, 4, 0, 4);
  fhTokenMsgType->GetXaxis()->SetBinLabel( 1, "A"); // gDPB TS high
  fhTokenMsgType->GetXaxis()->SetBinLabel( 2, "B"); // gDPB TS low, STAR TS high
  fhTokenMsgType->GetXaxis()->SetBinLabel( 3, "C"); // STAR TS mid
  fhTokenMsgType->GetXaxis()->SetBinLabel( 4, "D"); // STAR TS low, token, CMDs
  fHM->Add(name.Data(), fhTokenMsgType);
  if (server)
    server->Register("/StarRaw", fHM->H1(name.Data()));

  name = "hTriggerRate";
  title = "STAR trigger signals per second; Time[s] ; Counts";
  fhTriggerRate = new TH1F(name, title, 1800, 0, 1800);
  fHM->Add(name.Data(), fhTriggerRate);
  if (server)
    server->Register("/StarRaw", fHM->H1(name.Data()));

  name = "hCmdDaqVsTrig";
  title = "STAR daq command VS STAR trigger command; DAQ ; TRIGGER";
  fhCmdDaqVsTrig = new TH2I(name, title, 16, 0, 16, 16, 0, 16 );
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 1, "0x0: no-trig "); // idle link
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 2, "0x1: clear   "); // clears redundancy counters on the readout boards
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 3, "0x2: mast-rst"); // general reset of the whole front-end logic
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 4, "0x3: spare   "); // reserved
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 5, "0x4: trigg. 0"); // Default physics readout, all det support required
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 6, "0x5: trigg. 1"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 7, "0x6: trigg. 2"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 8, "0x7: trigg. 3"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel( 9, "0x8: puls.  0"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(10, "0x9: puls.  1"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(11, "0xA: puls.  2"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(12, "0xB: puls.  3"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(13, "0xC: config  "); // housekeeping trigger: return geographic info of FE
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(14, "0xD: abort   "); // aborts and clears an active event
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(15, "0xE: L1accept"); //
  fhCmdDaqVsTrig->GetXaxis()->SetBinLabel(16, "0xF: L2accept"); //
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 1, "0x0:  0"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 2, "0x1:  1"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 3, "0x2:  2"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 4, "0x3:  3"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 5, "0x4:  4"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 6, "0x5:  5"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 7, "0x6:  6"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 8, "0x7:  7"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel( 9, "0x8:  8"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(10, "0x9:  9"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(11, "0xA: 10"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(12, "0xB: 11"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(13, "0xC: 12"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(14, "0xD: 13"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(15, "0xE: 14"); // To be filled at STAR
  fhCmdDaqVsTrig->GetYaxis()->SetBinLabel(16, "0xF: 15"); // To be filled at STAR
  fHM->Add(name.Data(), fhCmdDaqVsTrig);
  if (server)
    server->Register("/StarRaw", fHM->H2(name.Data()));

  name = "hStarTokenEvo";
  title = "STAR token value VS time; Time in Run [s] ; STAR Token; Counts";
  fhStarTokenEvo = new TH2I(name, title, 1800, 0, 1800, 410, 0, 4100 ); // 4096
  fHM->Add(name.Data(), fhStarTokenEvo);
  if (server)
    server->Register("/StarRaw", fHM->H2(name.Data()));

  if (server)
    server->RegisterCommand("/Reset_All_TOF", "bResetTofLabHistos=kTRUE");
  if (server)
    server->RegisterCommand("/Cycle_Pulser_FEE", "bTofLabCyclePulserFee=kTRUE");
  if (server)
    server->Restrict("/Reset_All_TOF", "allow=admin");

   LOG(info) << "Done with histo creation, now doing canvases";

  /** Create summary Canvases for STAR 2017 **/
  Double_t w = 10;
  Double_t h = 10;
  TCanvas* cSummary = new TCanvas("cSummary", "gDPB Monitoring Summary", w, h);
  cSummary->Divide(2, 3);

  // 1st Column: Messages types
  cSummary->cd(1);
  gPad->SetLogy();
  hMessageType->Draw();

  cSummary->cd(2);
  gPad->SetLogy();
  hSysMessType->Draw();

  cSummary->cd(3);
  gPad->SetLogz();
  hGet4MessType->Draw("colz");

  // 2nd Column: GET4 Errors + Epoch flags +
  cSummary->cd(4);
  gPad->SetLogz();
  hGet4ChanErrors->Draw("colz");

  cSummary->cd(5);
  gPad->SetLogz();
  hGet4EpochFlags->Draw("colz");

  cSummary->cd(6);
  hSpill->Draw("col text");
  /*****************************/


  /** Create FEET rates Canvas for STAR 2017 **/
  TCanvas* cFeeRates = new TCanvas("cFeeRates", "gDPB Monitoring FEET rates", w, h);
  cFeeRates->Divide(fNrOfFebsPerGdpb, fNrOfGdpbs );

  TH1* histPnt = NULL;
  for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb ) {
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet ) {
      name = Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
      histPnt = fHM->H1(name.Data());

      cFeeRates->cd( 1 + uGdpb * fNrOfFebsPerGdpb + uFeet );
      gPad->SetLogy();

      histPnt->Draw();

      name = Form("FeetErrorRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
      histPnt = fHM->H1(name.Data());
      histPnt->SetLineColor( kRed );
      histPnt->Draw("same");
    } // for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet )
  } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb )
  /*****************************/

  /** Create FEET rates long duration Canvas for STAR 2017 **/
  TCanvas* cFeeRatesLong = new TCanvas("cFeeRatesLong", "gDPB Monitoring FEET rates", w, h);
  cFeeRatesLong->Divide(fNrOfFebsPerGdpb, fNrOfGdpbs );
  THStack * stackLong[fNrOfGdpbs][fNrOfFebsPerGdpb];
  for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb ) {
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet ) {
      stackLong[uGdpb][uFeet] = new THStack(
            Form("hsRateLong_g%02u_f%1u", uGdpb, uFeet),
            Form("Rate of hits and errors (red), long duration, for g%02u f%1u", uGdpb, uFeet) );

      name = Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
      histPnt = fHM->H1(name.Data());

      cFeeRatesLong->cd( 1 + uGdpb * fNrOfFebsPerGdpb + uFeet );
      gPad->SetLogy();

      stackLong[uGdpb][uFeet]->Add(histPnt);
      histPnt->Draw();

      name = Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
      histPnt = fHM->H1(name.Data());
      histPnt->SetLineColor( kRed );
      histPnt->Draw("same");
      stackLong[uGdpb][uFeet]->Add(histPnt);
//      stackLong[uGdpb][uFeet]->Draw("nostack");

       if (server)
         server->Register("/TofStacks", stackLong[uGdpb][uFeet]);
    } // for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet )
  } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb )
  /*****************************/

  /** Create TOT Canvas(es) for STAR 2017 **/
  TCanvas* cTotPnt = NULL;
  for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb ) {

    cTotPnt = new TCanvas( Form("cTotPnt_g%02u", uGdpb),
                           Form("gDPB %02u TOT distributions", uGdpb),
                           w, h);
    cTotPnt->Divide( fNrOfFebsPerGdpb/uNbFeetPlot );
    TH2* histPntTot = NULL;
    for (UInt_t uFeetPlot = 0; uFeetPlot < fNrOfFebsPerGdpb/uNbFeetPlot; ++uFeetPlot ) {
      name = Form("Raw_Tot_gDPB_%02u_%1u", uGdpb, uFeetPlot);
      histPntTot = fHM->H2(name.Data());

      cTotPnt->cd( 1 + uFeetPlot );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();

      histPntTot->Draw( "colz" );
    } // for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet )
  } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb )
  /*****************************/

  /** Create FT Canvas(es) for STAR 2017 **/
  TCanvas* cFtPnt = NULL;
  for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb ) {

    cFtPnt = new TCanvas( Form("cFtPnt_g%02u", uGdpb),
                           Form("gDPB %02u FineTime distributions", uGdpb),
                           w, h);
    cFtPnt->Divide( fNrOfFebsPerGdpb );
    TH2* histPntFt = NULL;
    for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++) {
      name = Form("FtDistribPerCh_gDPB_g%02u_f%1u", uGdpb, uFeet);
      histPntFt = fHM->H2(name.Data());

      cFtPnt->cd( 1 + uFeet );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();

      histPntFt->Draw( "colz" );
    } // for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet )
  } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb )
  /*****************************/


  /** Create 24b mode Canvas(es) for STAR 2017 **/
  if( fbGet4M24b )
  {
     TCanvas* cFt24b = NULL;
     for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb ) {

       cFt24b = new TCanvas( Form("cFt24b_g%02u", uGdpb),
                              Form("gDPB %02u 24b mode FineTime distributions", uGdpb),
                              w, h);
       cFt24b->Divide( 2, 2 );
       TH2* histPntFt = NULL;

       cFt24b->cd( 1  );
       gPad->SetGridx();
       gPad->SetGridy();
       gPad->SetLogz();
       name = Form("FtDistribPerCh_gDPB_g%02u_f%1u", uGdpb, 0);
       histPntFt = fHM->H2(name.Data());
       histPntFt->Draw( "colz" );

       cFt24b->cd( 2  );
       gPad->SetGridx();
       gPad->SetGridy();
       gPad->SetLogz();
       name = Form("FtDistribPerChFall_gDPB_g%02u_f%1u", uGdpb, 0);
       histPntFt = fHM->H2(name.Data());
       histPntFt->Draw( "colz" );

       cFt24b->cd( 3 );
       gPad->SetGridx();
       gPad->SetGridy();
       gPad->SetLogz();
       name = Form("ChCount_gDPB_%02u", uGdpb);
       fHM->H1(name.Data())->Draw();

       cFt24b->cd( 4 );
       gPad->SetGridx();
       gPad->SetGridy();
       gPad->SetLogz();
       name = Form("ChCountFall_gDPB_%02u", uGdpb);
       fHM->H1(name.Data())->Draw();
     } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb )
  } // if( fbGet4M24b )
  /*****************************/

  /** Create Pulser mode Canvas for STAR 2017 **/
  if( fbPulserMode )
  {
     TCanvas* cPulserRms = new TCanvas("cPulserRms", "Time difference RMS for chosen FEE and channels in pulser mode", w, h);
     cPulserRms->Divide( 2 );

     cPulserRms->cd(1);
     fhTimeRmsPulserChosenFee->Draw( "colz" );

     cPulserRms->cd(2);
     fhTimeRmsPulserChosenChPairs->Draw( "colz" );
  } // if( fbPulserMode )
  /*****************************/

  /** Create STAR token Canvas for STAR 2017 **/
  TCanvas* cStarToken = new TCanvas("cStarToken", "STAR token detection info", w, h);
  cStarToken->Divide( 2, 2 );

  cStarToken->cd(1);
  fhTriggerRate->Draw();

  cStarToken->cd(2);
  fhCmdDaqVsTrig->Draw( "colz" );

  cStarToken->cd(3);
  fhStarTokenEvo->Draw();
  /*****************************/

  /** Recovers/Create Ms Size Canvas for STAR 2017 **/
  // Try to recover canvas in case it was created already by another monitor
  // If not existing, create it
  fcMsSizeAll = dynamic_cast<TCanvas *>( gROOT->FindObject( "cMsSizeAll" ) );
  if( NULL == fcMsSizeAll )
  {
     fcMsSizeAll = new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
     fcMsSizeAll->Divide( 4, 4 );
      LOG(info) << "Created MS size canvas in TOF monitor";
  } // if( NULL == fcMsSizeAll )
      else LOG(info) << "Recovered MS size canvas in TOF monitor";
  /*****************************/

   LOG(info) << "Done with canvases creation";

  /** Store pointers to histograms for STAR 2017 **/
  fHistMessType = fHM->H1("hMessageType");
  fHistSysMessType = fHM->H1("hSysMessType");
  fHistGet4MessType = fHM->H2("hGet4MessType");
  fHistGet4ChanErrors = fHM->H2("hGet4ChanErrors");
  fHistGet4EpochFlags = fHM->H2("hGet4EpochFlags");
  fHistSpill = fHM->H2("hSpill");
  fHistSpillLength = fHM->H1("hSpillLength");
  fHistSpillCount = fHM->H1("hSpillCount");
  fHistSpillQA = fHM->H2("hSpillQA");

  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    name = Form("Raw_Tot_gDPB_%02u_0", i);
    fRaw_Tot_gDPB.push_back(fHM->H2(name.Data()));
    if( uNbFeetPlot < fNrOfFebsPerGdpb )
    {
       name = Form("Raw_Tot_gDPB_%02u_1", i);
       fRaw_Tot_gDPB.push_back(fHM->H2(name.Data()));
    } // if( uNbFeetPlot < fNrOfFebsPerGdpb )
    if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )
    {
       name = Form("Raw_Tot_gDPB_%02u_2", i);
       fRaw_Tot_gDPB.push_back(fHM->H2(name.Data()));
    } // if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )

    name = Form("ChCount_gDPB_%02u", i);
    fChCount_gDPB.push_back(fHM->H1(name.Data()));

    if (fUnpackPar->IsChannelRateEnabled()) {
      name = Form("ChannelRate_gDPB_%02u_0", i);
      fChannelRate_gDPB.push_back(fHM->H2(name.Data()));
      if( uNbFeetPlot < fNrOfFebsPerGdpb )
      {
        name = Form("ChannelRate_gDPB_%02u_1", i);
        fChannelRate_gDPB.push_back(fHM->H2(name.Data()));
      } // if( uNbFeetPlot < fNrOfFebsPerGdpb )
      if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )
      {
        name = Form("ChannelRate_gDPB_%02u_2", i);
        fChannelRate_gDPB.push_back(fHM->H2(name.Data()));
      } // if( 2 * uNbFeetPlot < fNrOfFebsPerGdpb )
    } // if (fUnpackPar->IsChannelRateEnabled())

    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      name = Form("FeetRate_gDPB_g%02u_f%1u", i, uFeet);
      fFeetRate_gDPB.push_back(fHM->H1(name.Data()));
      name = Form("FeetErrorRate_gDPB_g%02u_f%1u", i, uFeet);
      fFeetErrorRate_gDPB.push_back(fHM->H1(name.Data()));
      name = Form("FeetRateLong_gDPB_g%02u_f%1u", i, uFeet);
      fFeetRateLong_gDPB.push_back(fHM->H1(name.Data()));
      name = Form("FeetErrorRateLong_gDPB_g%02u_f%1u", i, uFeet);
      fFeetErrorRateLong_gDPB.push_back(fHM->H1(name.Data()));
      if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
        name = Form("FeetRateDate_gDPB_g%02u_f%1u", i, uFeet);
        fFeetRateDate_gDPB.push_back(fHM->H1(name.Data()));
      } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
      name = Form("FtDistribPerCh_gDPB_g%02u_f%1u", i, uFeet);
      fhFtDistribPerCh.push_back(fHM->H2(name.Data()));
      if( fbGet4M24b )
      {
         name = Form("ChCountFall_gDPB_%02u", i);
         fChCountFall_gDPB.push_back(fHM->H1(name.Data()));
         name = Form("FtDistribPerChFall_gDPB_g%02u_f%1u", i, uFeet);
         fhFtDistribPerChFall.push_back(fHM->H2(name.Data()));

         name = Form("FtLastRiseCurrFall_gDPB_g%02u_f%1u", i, uFeet);
         fhFtLastRiseCurrFall.push_back(fHM->H2(name.Data()));
         name = Form("FtCurrRiseLastFall_gDPB_g%02u_f%1u", i, uFeet);
         fhFtCurrRiseLastFall.push_back(fHM->H2(name.Data()));

         name = Form("FtLastRiseDistRise_gDPB_g%02u_f%1u", i, uFeet);
         fhFtLastRiseDistRise.push_back(fHM->H2(name.Data()));
         name = Form("FtLastRiseDistFall_gDPB_g%02u_f%1u", i, uFeet);
         fhFtLastRiseDistFall.push_back(fHM->H2(name.Data()));
      } // if( fbGet4M24b )
    } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
  }
  /*****************************/

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmTSMonitorTofLab::DoUnpack(const fles::Timeslice& ts,
    size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  if (bResetTofLabHistos) {
    ResetAllHistos();
    bResetTofLabHistos = kFALSE;
  }
  if (bTofLabCyclePulserFee) {
     CyclePulserFee();
     bTofLabCyclePulserFee = kFALSE;
  }

  ///* ASIC coincidences & offsets mapping *///
  if( fbEnableCoincMap )
  {
     if( 0 == component && fbHitsInLastTs )
     {
         for( UInt_t uAsicA = 0; uAsicA < fNrOfGet4 - 1; uAsicA ++ )
         {
            for( UInt_t uHitA = 0; uHitA < fvulHitEpochBuffLastTs[uAsicA].size(); uHitA ++ )
            {
               for( UInt_t uAsicB = uAsicA; uAsicB < fNrOfGet4; uAsicB ++ )
               {
                  for( UInt_t uHitB = 0; uHitB < fvulHitEpochBuffLastTs[uAsicB].size(); uHitB ++ )
                  {
                     fvhCoincOffsetEpochGet4[uAsicA]->Fill( uAsicB,
                              fvulHitEpochBuffLastTs[uAsicB][uHitB]
                            - fvulHitEpochBuffLastTs[uAsicA][uHitA] );
                  } // for( UInt_t uHitB = 0; uHitB < fvulHitEpochBuffLastTs[uAsicB].size(); uHitB ++ )
               } // for( UInt_t uAsicA = 0; uAsicA < fNrOfGet4 - 1; uAsicA ++ )
            } // for( UInt_t uHitA = 0; uHitA < fvulHitEpochBuffLastTs[uAsicA].size(); uHitA ++ )
            fvulHitEpochBuffLastTs[uAsicA].clear();
         } // for( UInt_t uAsicA = 0; uAsicA < fNrOfGet4 - 1; uAsicA ++ )
         fvulHitEpochBuffLastTs[fNrOfGet4 - 1].clear();
         fbHitsInLastTs = kFALSE;
     } // if( 0 == component && fbHitsInLastTs )
  } // if( fbEnableCoincMap )

  // Periodic histo saving & Printout of nb events log
  std::chrono::time_point<std::chrono::system_clock> timeCurrent = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = timeCurrent - fTimeLastHistoSave;
  if( 0 == fTimeLastHistoSave.time_since_epoch().count() )
  {
     fTimeLastHistoSave = timeCurrent;
  } // if( 0 == fTimeLastHistoSave.time_since_epoch().count() )
  else if( 300 < elapsed_seconds.count() )
  {
     std::time_t cTimeCurrent = std::chrono::system_clock::to_time_t( timeCurrent );
     char tempBuff[80];
     std::strftime( tempBuff, 80, "%F %T", localtime (&cTimeCurrent) );
//     LOG(info) << "CbmTSMonitorTofLab::DoUnpack => " << tempBuff
//               << " events in last " << std::setw(4) << elapsed_seconds.count() << " s";
     fTimeLastHistoSave = timeCurrent;

     SaveAllHistos( "data/histos_test.root" );
  } // else if( 300 < elapsed_seconds.count() )

  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
                 << "microslices.";

  // Getting the pointer to the correct histogram needs a lot more time then
  // the actual filling procedure. If one gets the pointer in a loop this kills
  // the performance. A test shows that extracting the pointer from the CbmHistManger
  // is slower by a factor of 20-100 (depending on the number of histos managed by the
  // CbmHistManager) compared to using the pointer directly
  // So get the pointer once outside the loop and use it in the loop

  TString sMsSzName = Form("MsSz_link_%02lu", component);
  TH1* hMsSz = NULL;
  TProfile* hMsSzTime = NULL;
  if (fHM->Exists(sMsSzName.Data())) {
    hMsSz = fHM->H1(sMsSzName.Data());
    sMsSzName = Form("MsSzTime_link_%02lu", component);
    hMsSzTime = fHM->P1(sMsSzName.Data());
  } // if( fHM->Exists(sMsSzName.Data() ) )
  else {
    TString sMsSzTitle = Form(
        "Size of MS for gDPB of link %02lu; Ms Size [bytes]", component);
    fHM->Add(sMsSzName.Data(),
        new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000.));
    hMsSz = fHM->H1(sMsSzName.Data());
    if (server)
      server->Register("/FlibRaw", hMsSz);
    sMsSzName = Form("MsSzTime_link_%02lu", component);
    sMsSzTitle = Form(
        "Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]",
        component);
    fHM->Add(sMsSzName.Data(),
        new TProfile(sMsSzName.Data(), sMsSzTitle.Data(), 180000, 0., 3600.));
    hMsSzTime = fHM->P1(sMsSzName.Data());
    if (server)
      server->Register("/FlibRaw", hMsSzTime);
    if( NULL != fcMsSizeAll )
    {
      fcMsSizeAll->cd( 1 + component );
      gPad->SetLogy();
      hMsSzTime->Draw("hist le0");
    } // if( NULL != fcMsSizeAll )
    LOG(info) << "Added MS size histo for component: " << component << " (gDPB)";
  } // else of if( fHM->Exists(sMsSzName.Data() ) )

  // Initialize spill detection
  fiCountsLastTs = 0;

  Int_t messageType = -111;
  Double_t dTsStartTime = -1;
  // Loop over microslices
  size_t numCompMsInTs = ts.num_microslices(component);
  for (size_t m = 0; m < numCompMsInTs; ++m) {
    if (fuMsAcceptsPercent < m)
      continue;

    constexpr uint32_t kuBytesPerMessage = 8;

    auto msDescriptor = ts.descriptor(component, m);
    fEquipmentId = msDescriptor.eq_id;
    const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(
        component, m));

    uint32_t size = msDescriptor.size;
    fulLastMsIdx = msDescriptor.idx;
    if (size > 0)
      LOG(debug) << "Microslice: " << msDescriptor.idx << " has size: " << size;

    if( numCompMsInTs - fuOverlapMsNb <= m )
    {
//      LOG(info) << "Ignore overlap Microslice: " << msDescriptor.idx;
      continue;
    } // if( numCompMsInTs - fuOverlapMsNb <= m )

    if( 0 == m )
      dTsStartTime = (1e-9) * static_cast<double>(msDescriptor.idx);

    if( fdStartTimeMsSz < 0 )
      fdStartTimeMsSz = (1e-9) * static_cast<double>(msDescriptor.idx);
    hMsSz->Fill(size);
    hMsSzTime->Fill((1e-9) * static_cast<double>(msDescriptor.idx) - fdStartTimeMsSz, size);

    // If not integer number of message in input buffer, print warning/error
    if (0 != (size % kuBytesPerMessage))
      LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

    // Compute the number of complete messages in the input microslice buffer
    uint32_t uNbMessages = (size - (size % kuBytesPerMessage))
        / kuBytesPerMessage;

    // Prepare variables for the loop on contents
    const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
    for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++) {
      // Fill message
      uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
      ngdpb::Message mess(ulData);

      if (gLogger->IsLogNeeded( fair::Severity::debug2 ))
      {
        mess.printDataCout();
      } // if (gLogger->IsLogNeeded( fair::Severity::debug2 ))


      // Increment counter for different message types
      // and fill the corresponding histogram
      messageType = mess.getMessageType();
      fMsgCounter[messageType]++;
      fHistMessType->Fill(messageType);

      fGdpbId = mess.getRocNumber();
      fGdpbNr = fGdpbIdIndexMap[fGdpbId];

      fGet4Id = mess.getGdpbGenChipId();
      fGet4Nr = (fGdpbNr * fNrOfGet4PerGdpb) + fGet4Id;

      switch (messageType) {
        case ngdpb::MSG_HIT:
          //           FillHitInfo(mess);
          LOG(error) << "Message type " << messageType
                        << " not yet included in unpacker.";
          break;
        case ngdpb::MSG_EPOCH:
          //           FillEpochInfo(mess);
          LOG(error) << "Message type " << messageType
                        << " not yet included in unpacker.";
          break;
        case ngdpb::MSG_SYS:
          fHistSysMessType->Fill(mess.getSysMesType());
          break;
        case ngdpb::MSG_EPOCH2:
          fHistGet4MessType->Fill(fGet4Nr, ngdpb::GET4_32B_EPOCH);
          FillEpochInfo(mess);
          break;
        case ngdpb::MSG_GET4:
          fHistGet4MessType->Fill(fGet4Nr, ngdpb::GET4_32B_DATA + 1);
          if( fbGet4M24b )
            FillHitInfo(mess);
            else PrintGenInfo(mess);
          break;
        case ngdpb::MSG_GET4_32B:
          fHistGet4MessType->Fill(fGet4Nr, ngdpb::GET4_32B_DATA);
          if( fbEpochSuppModeOn )
            fvmEpSupprBuffer[fGet4Nr].push_back( mess );
            else FillHitInfo(mess);
          break;
        case ngdpb::MSG_GET4_SLC:
          fHistGet4MessType->Fill(fGet4Nr, ngdpb::GET4_32B_SLCM);
          PrintSlcInfo(mess);
          break;
        case ngdpb::MSG_GET4_SYS:
        {
          fHistSysMessType->Fill(mess.getGdpbSysSubType());
          if (ngdpb::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType())
          {
            if( fbEpochSuppModeOn )
               fvmEpSupprBufferErrors[fGet4Nr].push_back( mess );
               else FillGet4ErrorInfo(mess);
          } // if (ngdpb::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType())

          if (100 > iMess)
          {
            PrintSysInfo(mess);
            iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
          } // if (100 > iMess)
          break;
        }
        case ngdpb::MSG_STAR_TRI:
          fHistGet4MessType->Fill(fGet4Nr, ngdpb::MSG_STAR_TRI);
          FillStarTrigInfo(mess);
          break;
        default:
          if (100 > iMess)
          {
            LOG(error) << "Message (" << iMess << ") type " << std::hex
                          << std::setw(2) << static_cast<uint16_t>(messageType)
                          << " not yet included in Get4 unpacker.";
            iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
          } // if (100 > iMess++)
      } // switch( mess.getMessageType() )
    } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
  } // for (size_t m = 0; m < ts.num_microslices(component); ++m)
  if (100 == iMess)
  {
    LOG(error) << "Stop reporting MSG errors... ";
    iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
  } // if (100 == iMess)

  // Spill detection using Detectors
  if( !fbSpillOn && (fiSpillOnThr < fiCountsLastTs) )
  {
    fbSpillOn = kTRUE;
    fiTsUnderOff = 0;

    if( 0 < fdDetTimeLastTs )
    {
      fHistSpillLength->Fill( fdDetLastTime - fdDetTimeLastTs );
      fHistSpillCount->Fill( fSpillIdx, fHistSpill->GetEntries() );
      fHistSpillQA->Fill( fdDetLastTime - fdDetTimeLastTs,
                                 fHistSpill->GetEntries() );
    } // if( 0 < fdTimeLastTs )
    fdDetTimeLastTs = fdDetLastTime;
    fSpillIdx++;
    fHistSpill->Reset();
  } // if( !fbSpillOn && (fiSpillOnThr < fiCountsLastTs) )
  else if( fbSpillOn  )
  {
    if( fiCountsLastTs < fiSpillOffThr )
    {
      fiTsUnderOff ++;
      if( fiTsUnderOffThr < fiTsUnderOff )
        fbSpillOn = kFALSE;
    } //  if( fiCountsLastTs < fiSpillOffThr )
      else fiTsUnderOff = 0;
  } // else if( fbSpillOn  )

   if( kTRUE == fbPulserMode )
   {
      // Update RMS plots only every 10s in data
      if( 10.0 < dTsStartTime - fdLastRmsUpdateTime )
      {
         // Reset summary histograms for safety
         fhTimeRmsPulserChosenFee->Reset();
         fhTimeRmsPulserChosenChPairs->Reset();

         UInt_t uHistoFeeIdx = 0;
         for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)
         {
            for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
            {
               fhTimeRmsPulserChosenFee->Fill(uChanFeeA, uChanFeeB, fhTimeDiffPulserChosenFee[uHistoFeeIdx]->GetRMS() );
               uHistoFeeIdx++;
            } // for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
         } // for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)
         for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
         {
            fhTimeRmsPulserChosenChPairs->Fill( uChan, fhTimeDiffPulserChosenChPairs[uChan]->GetRMS() );
         } // for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
         fdLastRmsUpdateTime = dTsStartTime;
      } // if( 10.0 < dTsStartTime - fdLastRmsUpdateTime )
   } // if( kTRUE == fbPulserMode )

  return kTRUE;
}

void CbmTSMonitorTofLab::FillHitInfo(ngdpb::Message mess)
{
  Int_t channel = mess.getGdpbHitChanId();
  Int_t tot = mess.getGdpbHit32Tot();
  ULong_t hitTime = mess.getMsgFullTime(0);

  Long64_t curEpochGdpbGet4 = fCurrentEpoch[fGet4Nr];

  if (curEpochGdpbGet4 != -111) {
    UInt_t channelNr = fGet4Id * fNrOfChannelsPerGet4 + channel;
    UInt_t uFeetNr   = (fGet4Id / fNrOfGet4PerFeb);

    if( !fbGet4M24b )
    {
       fRaw_Tot_gDPB[fGdpbNr + uFeetNr/uNbFeetPlot]->Fill(channelNr, tot);
       fChCount_gDPB[fGdpbNr]->Fill(channelNr);

      /// Finetime monitoring
      fhFtDistribPerCh[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
            fGet4Id * fNrOfChannelsPerGet4 + channel,
            mess.getGdpbHitFineTs()
         );
    } // if( !fbGet4M24b )
      else
      {
/*
         fGet4Id = mess.getGet4Number();
         channel = mess.getGet4ChNum();
         UInt_t Fts  = mess.getGet4FineTs();
         UInt_t edge = mess.getGet4Edge();
*/
         Int_t Fts  = mess.getGdpbHitFineTs();
         Int_t edge = mess.getGdpbHit24Edge();

         channelNr = fGet4Id * fNrOfChannelsPerGet4 + channel;
         uFeetNr   = (fGet4Id / fNrOfGet4PerFeb);
         Int_t iFullFebIdx = (fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr;

         Double_t dHitTime = mess.getMsgFullTimeD(curEpochGdpbGet4);

         if( edge )
         {
            fChCountFall_gDPB[fGdpbNr]->Fill(channelNr);


            /// Finetime monitoring
            fhFtDistribPerChFall[iFullFebIdx]->Fill( channelNr, Fts );
            if( 0 <= fviFtLastRise24b[iFullFebIdx][channelNr] &&
                fuRiseFallChSel == channelNr)
               fhFtLastRiseCurrFall[iFullFebIdx]->Fill(
                     fviFtLastRise24b[iFullFebIdx][channelNr],
                     Fts);
            if( 0 <= fviFtLastRise24b[iFullFebIdx][channelNr] &&
                0 <= fvdTimeLastFall24b[iFullFebIdx][channelNr] &&
                fuRiseFallChSel == channelNr)
               fhFtLastRiseDistFall[iFullFebIdx]->Fill(
                     fviFtLastRise24b[iFullFebIdx][channelNr],
                     dHitTime - fvdTimeLastFall24b[iFullFebIdx][channelNr]);

            fviFtLastFall24b[iFullFebIdx][channelNr] = Fts;
            fvdTimeLastFall24b[iFullFebIdx][channelNr] = dHitTime;
         } // if( edge )
            else
            {
               fChCount_gDPB[fGdpbNr]->Fill(channelNr);

               /// Finetime monitoring
               fhFtDistribPerCh[iFullFebIdx]->Fill( channelNr, Fts );
               if( 0 <= fviFtLastFall24b[iFullFebIdx][channelNr] &&
                  fuRiseFallChSel == channelNr)
                  fhFtCurrRiseLastFall[iFullFebIdx]->Fill(
                        Fts,
                        fviFtLastFall24b[iFullFebIdx][channelNr] );
              if( 0 <= fviFtLastRise24b[iFullFebIdx][channelNr] &&
                  0 <= fvdTimeLastRise24b[iFullFebIdx][channelNr] &&
                  fuRiseFallChSel == channelNr)
                 fhFtLastRiseDistRise[iFullFebIdx]->Fill(
                       fviFtLastRise24b[iFullFebIdx][channelNr],
                       dHitTime - fvdTimeLastRise24b[iFullFebIdx][channelNr]);

               fviFtLastRise24b[iFullFebIdx][channelNr] = Fts;
               fvdTimeLastRise24b[iFullFebIdx][channelNr] = dHitTime;
            } // else of if( edge )
      } // else of if( !fbGet4M24b )

    if (fUnpackPar->IsChannelRateEnabled()) {
      // Check if at least one hit before in this channel
      if( -1 < fTsLastHit[fGdpbNr][fGet4Id][channel] )
      {
         fChannelRate_gDPB[fGdpbNr + uFeetNr/uNbFeetPlot]->Fill(
             1e9 / (mess.getMsgFullTimeD(curEpochGdpbGet4)
                     - fTsLastHit[fGdpbNr][fGet4Id][channel]),
             fGet4Id * fNrOfChannelsPerGet4 + channel);
      } // if( -1 < fTsLastHit[fGdpbNr][fGet4Id][channel] )
    } // if( fUnpackPar->IsChannelRateEnabled() )

    // Save last hist time if channel rate histos or pulser mode enabled
    if( fUnpackPar->IsChannelRateEnabled() || fbPulserMode )
      fTsLastHit[fGdpbNr][fGet4Id][channel] = mess.getMsgFullTimeD(
          curEpochGdpbGet4);

    // In Run rate evolution
    if (fdStartTime < 0)
      fdStartTime = mess.getMsgFullTimeD(curEpochGdpbGet4);

    if (0 <= fdStartTime)
    {
      fFeetRate_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
          1e-9 * (mess.getMsgFullTimeD(curEpochGdpbGet4) - fdStartTime));
//      fFeetRateLong_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
//          1e-9 * (mess.getMsgFullTimeD(curEpochGdpbGet4) - fdStartTime) / 60 );

       // General Time (date + time) rate evolution
       // Add offset of -1H as the filenames were using some times offset by 1 hour (Summer time?!?)
       if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
         fFeetRateDate_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
             1e-9 * (mess.getMsgFullTimeD(curEpochGdpbGet4) - fdStartTime)  );
       } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
    }
   fFeetRateLong_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
       1e-9 * (mess.getMsgFullTimeD(curEpochGdpbGet4) ) / 60.0
       + static_cast< double >( fuCurrentEpochCycle[fGet4Nr] ) * get4v1x::kdEpochCycleInS / 60.0 );

/*
    Int_t iChanInGdpb = fGet4Id * fNrOfChannelsPerGet4 + channel;
    Int_t increment = static_cast<CbmFlibCern2016Source*>(FairRunOnline::Instance()->GetSource())->GetNofTSSinceLastTS();
    // if condition to find the right strip/end index
      fHistSpill->Fill(0., 0., increment);
*/
    fiCountsLastTs ++;
    fdDetLastTime = 1e-9 * mess.getMsgFullTimeD(curEpochGdpbGet4);

    hitTime = mess.getMsgFullTime(curEpochGdpbGet4);
    Int_t Ft = mess.getGdpbHitFineTs();

    if (100 > iMess)
    {
       LOG(debug) << "Hit: " << Form("0x%08x ", fGdpbId) << ", " << fGet4Id
                     << ", " << channel << ", " << tot << ", epoch "
                     << curEpochGdpbGet4 << ", FullTime "
                     << hitTime << ", FineTime " << Ft;
       iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
    } // if (100 > iMess)

    ///* ASIC coincidences & offsets mapping *///
    if( fbEnableCoincMap )
    {
       fbHitsInLastTs = kTRUE;
       fvulHitEpochBuffLastTs[fGet4Nr].push_back( curEpochGdpbGet4 );
    } // if( fbEnableCoincMap )
  } // if( fCurrentEpoch[rocId].end() != fCurrentEpoch[rocId].find( get4Id ) )
  else
    LOG(warning) << "Found hit in gdpbId w/o epoch yet: "
                    << Form("0x%08x g4 %02u", fGdpbId, fGet4Nr);
}

void CbmTSMonitorTofLab::FillEpochInfo(ngdpb::Message mess)
{
  Long64_t epochNr = mess.getGdpbEpEpochNb();

   if( fbEpochSuppModeOn )
   {
      // If we are exactly on the epoch cycle border
      // or if epoch counter made a loop in suppressed epochs (assume at least 1 hit in ~5h!)
      if( ( 1 == epochNr && 1 <= fCurrentEpoch[fGet4Nr] ) ||
          ( epochNr + get4v1x::kuEpochCounterSz/2 < fCurrentEpoch[fGet4Nr] )
         )
      {
         fuCurrentEpochCycle[fGet4Nr] ++;
         LOG(info) << "Epoch cycle detected for gdpb "
                   << Form("0x%08x g4 %02u", fGdpbId, fGet4Nr)
                   << " : prev " << fCurrentEpoch[fGet4Nr]
                   << " new " << epochNr
                   << " cycle " << fuCurrentEpochCycle[fGet4Nr];
      } // if epoch cycle happened

      // In Ep. Suppr. Mode, receive following epoch instead of previous
      if( 0 < epochNr )
         fCurrentEpoch[fGet4Nr] = epochNr - 1;
         else fCurrentEpoch[fGet4Nr] = get4v1x::kuEpochCounterSz; // Catch end of epoch cycle!
   } // if( fbEpochSuppModeOn )
      else
      {
         // If we are exactly on the epoch cycle border
         if( 0 == epochNr && 0 < fCurrentEpoch[fGet4Nr] )
            fuCurrentEpochCycle[fGet4Nr] ++;
         // if epoch counter made a loop in suppressed epochs (assume at least 1 hit in ~5h!)
         else if( epochNr + get4v1x::kuEpochCounterSz/2 < fCurrentEpoch[fGet4Nr] )
            fuCurrentEpochCycle[fGet4Nr] ++;

         fCurrentEpoch[fGet4Nr] = epochNr;
      } // else of if( fbEpochSuppModeOn )


  if (100 > iMess)
  {
      LOG(debug) << "Epoch message for get4 " << fGet4Nr << " with epoch number "
                 << fCurrentEpoch[fGet4Nr];
      iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
  } // if (100 > iMess)

  if (1 == mess.getGdpbEpSync())
    fHistGet4EpochFlags->Fill(fGet4Nr, 0);
  if (1 == mess.getGdpbEpDataLoss())
    fHistGet4EpochFlags->Fill(fGet4Nr, 1);
  if (1 == mess.getGdpbEpEpochLoss())
    fHistGet4EpochFlags->Fill(fGet4Nr, 2);
  if (1 == mess.getGdpbEpMissmatch())
    fHistGet4EpochFlags->Fill(fGet4Nr, 3);

  fCurrentEpochTime = mess.getMsgFullTime(epochNr);
  fNofEpochs++;


   if( fbEpochSuppModeOn )
   {
      /// Re-align the epoch number of the message in case it will be used later:
      /// We received the epoch after the data instead of the one before!
      if( 0 < epochNr )
         mess.setEpoch2Number( epochNr - 1 );
         else mess.setEpoch2Number( get4v1x::kuEpochCounterSz );


      Int_t iBufferSize = fvmEpSupprBuffer[fGet4Nr].size();
      if( 0 < iBufferSize )
      {
         LOG(debug) << "Now processing stored messages for for get4 " << fGet4Nr << " with epoch number "
                    << (fCurrentEpoch[fGet4Nr] - 1);

         for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
         {
            FillHitInfo( fvmEpSupprBuffer[fGet4Nr][ iMsgIdx ] );
         } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

         fvmEpSupprBuffer[fGet4Nr].clear();
      } // if( 0 < iBufferSize )

      iBufferSize = fvmEpSupprBufferErrors[fGet4Nr].size();
      if( 0 < iBufferSize )
      {
         LOG(debug) << "Now processing stored error messages for for get4 " << fGet4Nr << " with epoch number "
                    << (fCurrentEpoch[fGet4Nr] - 1);

         for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
         {
            FillGet4ErrorInfo( fvmEpSupprBufferErrors[fGet4Nr][ iMsgIdx ] );
         } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

         fvmEpSupprBufferErrors[fGet4Nr].clear();
      } // if( 0 < iBufferSize )
   } // if( fbEpochSuppModeOn )

   // Fill Pulser test histos if needed
   if( fbPulserMode && 0 == (fGet4Id % fNrOfGet4PerFeb) // GET4 index in FEET
         && fuPulserFee == (fGet4Nr / fNrOfGet4PerFeb)) // FEET index in full system
   {
      // Fill the time difference for all channels pairs in
      // the chosen Fee
      UInt_t uHistoFeeIdx = 0;

      UInt_t uOffsetChip = fuPulserFee * fNrOfGet4PerFeb;
      for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)
      {
         UInt_t uChipA = uChanFeeA / fNrOfChannelsPerGet4 + uOffsetChip;
         UInt_t uChanA = uChanFeeA % fNrOfChannelsPerGet4;

         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
         {
            UInt_t uChipB = uChanFeeB / fNrOfChannelsPerGet4 + uOffsetChip;
            UInt_t uChanB = uChanFeeB % fNrOfChannelsPerGet4;

            Double_t dTimeDiff =
                  fTsLastHit[ fuPulserGdpb ][ uChipB ][ uChanB ]
                - fTsLastHit[ fuPulserGdpb ][ uChipA ][ uChanA ];
            dTimeDiff *= 1e3;  // ns -> ps
            if( ( 10.0 * dMinDt < dTimeDiff ) && ( dTimeDiff < 10.0 * dMaxDt ) &&
                ( 0 < fTsLastHit[ fuPulserGdpb ][ uChipA ][ uChanA ] ) &&
                ( 0 < fTsLastHit[ fuPulserGdpb ][ uChipB ][ uChanB ] ) )
            {
               fhTimeDiffPulserChosenFee[uHistoFeeIdx]->Fill( dTimeDiff );
            } // if both channels have already 1 hit and these are not too far away

            uHistoFeeIdx++;
         } // for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)

      // Fill the time difference for the chosen channel pairs
      for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
      {
         UInt_t uChipA = fuPulserChan[ uChan     ] / fNrOfChannelsPerGet4;
         UInt_t uChanA = fuPulserChan[ uChan     ] % fNrOfChannelsPerGet4;
         UInt_t uChipB = fuPulserChan[ uChan + 1 ] / fNrOfChannelsPerGet4;
         UInt_t uChanB = fuPulserChan[ uChan + 1 ] % fNrOfChannelsPerGet4;

         Double_t dTimeDiff =
               fTsLastHit[0][uChipB][ uChanB ] - fTsLastHit[0][uChipA][ uChanA ];
            dTimeDiff *= 1e3;  // ns -> ps
         if( ( 10.0 * dMinDt < dTimeDiff ) && ( dTimeDiff < 10.0 * dMaxDt ) &&
             ( 0 < fTsLastHit[0][uChipA][ uChanA ] ) &&
             ( 0 < fTsLastHit[0][uChipB][ uChanB ] ) )
         {
            fhTimeDiffPulserChosenChPairs[uChan]->Fill( dTimeDiff );
         } // if both channels have already 1 hit and these are not too far away
      } // for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
   } // if( fbPulserMode && First GET4 on chosen FEET )
}

void CbmTSMonitorTofLab::FillGet4ErrorInfo(ngdpb::Message mess)
{

   fHistGet4MessType->Fill(fGet4Nr, ngdpb::GET4_32B_ERROR);

   UInt_t uFeetNr   = (fGet4Id / fNrOfGet4PerFeb);
   if (0 <= fdStartTime)
   {
      fFeetErrorRate_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
         1e-9 * (mess.getMsgFullTimeD(fCurrentEpoch[fGet4Nr]) - fdStartTime));
//      fFeetErrorRateLong_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
//         1e-9 * (mess.getMsgFullTimeD(fCurrentEpoch[fGet4Nr]) - fdStartTime) / 60 );
   } // if (0 <= fdStartTime)
   fFeetErrorRateLong_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + uFeetNr]->Fill(
      1e-9 * (mess.getMsgFullTimeD(fCurrentEpoch[fGet4Nr])) / 60.0
      + static_cast< double >( fuCurrentEpochCycle[fGet4Nr] ) * get4v1x::kdEpochCycleInS / 60.0 );

   Int_t dFullChId =  fGet4Nr * fNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
   switch (mess.getGdpbSysErrData()) {
     case ngdpb::GET4_V1X_ERR_READ_INIT:
       fHistGet4ChanErrors->Fill(dFullChId, 0);
       break;
     case ngdpb::GET4_V1X_ERR_SYNC:
       fHistGet4ChanErrors->Fill(dFullChId, 1);
       break;
     case ngdpb::GET4_V1X_ERR_EP_CNT_SYNC:
       fHistGet4ChanErrors->Fill(dFullChId, 2);
       break;
     case ngdpb::GET4_V1X_ERR_EP:
       fHistGet4ChanErrors->Fill(dFullChId, 3);
       break;
     case ngdpb::GET4_V1X_ERR_FIFO_WRITE:
       fHistGet4ChanErrors->Fill(dFullChId, 4);
       break;
     case ngdpb::GET4_V1X_ERR_LOST_EVT:
       fHistGet4ChanErrors->Fill(dFullChId, 5);
       break;
     case ngdpb::GET4_V1X_ERR_CHAN_STATE:
       fHistGet4ChanErrors->Fill(dFullChId, 6);
       break;
     case ngdpb::GET4_V1X_ERR_TOK_RING_ST:
       fHistGet4ChanErrors->Fill(dFullChId, 7);
       break;
     case ngdpb::GET4_V1X_ERR_TOKEN:
       fHistGet4ChanErrors->Fill(dFullChId, 8);
       break;
     case ngdpb::GET4_V1X_ERR_READOUT_ERR:
       fHistGet4ChanErrors->Fill(dFullChId, 9);
       break;
     case ngdpb::GET4_V1X_ERR_SPI:
       fHistGet4ChanErrors->Fill(dFullChId, 10);
       break;
     case ngdpb::GET4_V1X_ERR_DLL_LOCK:
       fHistGet4ChanErrors->Fill(dFullChId, 11);
       break;
     case ngdpb::GET4_V1X_ERR_DLL_RESET:
       fHistGet4ChanErrors->Fill(dFullChId, 12);
       break;
     case ngdpb::GET4_V1X_ERR_TOT_OVERWRT:
       fHistGet4ChanErrors->Fill(dFullChId, 13);
       break;
     case ngdpb::GET4_V1X_ERR_TOT_RANGE:
       fHistGet4ChanErrors->Fill(dFullChId, 14);
       break;
     case ngdpb::GET4_V1X_ERR_EVT_DISCARD:
       fHistGet4ChanErrors->Fill(dFullChId, 15);
       break;
     case ngdpb::GET4_V1X_ERR_ADD_RIS_EDG:
       if( kTRUE == fbGet4v20 )
         fHistGet4ChanErrors->Fill(dFullChId, 16);
         else fHistGet4ChanErrors->Fill(dFullChId, 17);
       break;
     case ngdpb::GET4_V1X_ERR_UNPAIR_FALL:
       if( kTRUE == fbGet4v20 )
         fHistGet4ChanErrors->Fill(dFullChId, 17);
         else fHistGet4ChanErrors->Fill(dFullChId, 17);
       break;
     case ngdpb::GET4_V1X_ERR_SEQUENCE_ER:
       if( kTRUE == fbGet4v20 )
         fHistGet4ChanErrors->Fill(dFullChId, 18);
         else fHistGet4ChanErrors->Fill(dFullChId, 17);
       break;
     case ngdpb::GET4_V1X_ERR_UNKNOWN:
       if( kTRUE == fbGet4v20 )
         fHistGet4ChanErrors->Fill(dFullChId, 19);
         else fHistGet4ChanErrors->Fill(dFullChId, 16);
       break;
     default: // Corrupt error or not yet supported error
       if( kTRUE == fbGet4v20 )
         fHistGet4ChanErrors->Fill(dFullChId, 20);
         else fHistGet4ChanErrors->Fill(dFullChId, 17);
       break;
   } // Switch( mess.getGdpbSysErrData() )
}

void CbmTSMonitorTofLab::PrintSlcInfo(ngdpb::Message mess)
{
  if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fGdpbId))
    LOG(info) << "GET4 Slow Control message, epoch "
                 << static_cast<Int_t>(fCurrentEpoch[fGet4Nr]) << ", time " << std::setprecision(9)
                 << std::fixed << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
                 << " for board ID " << std::hex << std::setw(4) << fGdpbId
                 << std::dec << "\n"
                 << " +++++++ > Chip = "
                 << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = "
                 << std::setw(1) << mess.getGdpbSlcChan() << ", Edge = "
                 << std::setw(1) << mess.getGdpbSlcEdge() << ", Type = "
                 << std::setw(1) << mess.getGdpbSlcType() << ", Data = "
                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                 << std::dec << ", Type = " << mess.getGdpbSlcCrc();

}

void CbmTSMonitorTofLab::PrintGenInfo(ngdpb::Message mess)
{
  Int_t mType = mess.getMessageType();
  Int_t channel = mess.getGdpbHitChanId();
  uint64_t uData = mess.getData();

  if (100 > iMess)
  {
    LOG(info) << "Get4 MSG type " << mType << " from gdpbId " << fGdpbId
                 << ", getId " << fGet4Id << ", (hit channel) " << channel
                 << " data " << std::hex << uData;
    iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
  } // if (100 > iMess)
}

void CbmTSMonitorTofLab::PrintSysInfo(ngdpb::Message mess)
{
  if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fGdpbId))
    LOG(debug) << "GET4 System message,       epoch "
                  << static_cast<Int_t>(fCurrentEpoch[fGet4Nr]) << ", time " << std::setprecision(9)
                  << std::fixed << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
                  << " for board ID " << std::hex << std::setw(4) << fGdpbId
                  << std::dec;

  switch (mess.getGdpbSysSubType()) {
    case ngdpb::SYSMSG_GET4_EVENT: {
      uint32_t uData = mess.getGdpbSysErrData();
      if (ngdpb::GET4_V1X_ERR_TOT_OVERWRT == uData
          || ngdpb::GET4_V1X_ERR_TOT_RANGE == uData
          || ngdpb::GET4_V1X_ERR_EVT_DISCARD == uData
          || ngdpb::GET4_V1X_ERR_ADD_RIS_EDG == uData
          || ngdpb::GET4_V1X_ERR_UNPAIR_FALL == uData
          || ngdpb::GET4_V1X_ERR_SEQUENCE_ER == uData
          )
        LOG(debug) << " +++++++ > gDPB: " << std::hex << std::setw(4) << fGdpbId
                      << std::dec << ", Chip = " << std::setw(2)
                      << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                      << mess.getGdpbSysErrChanId() << ", Edge = "
                      << std::setw(1) << mess.getGdpbSysErrEdge() << ", Empt = "
                      << std::setw(1) << mess.getGdpbSysErrUnused()
                      << ", Data = " << std::hex << std::setw(2) << uData
                      << std::dec << " -- GET4 V1 Error Event";
      else
        LOG(info) << " +++++++ >gDPB: " << std::hex << std::setw(4) << fGdpbId
                     << std::dec << ", Chip = " << std::setw(2)
                     << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                     << mess.getGdpbSysErrChanId() << ", Edge = "
                     << std::setw(1) << mess.getGdpbSysErrEdge() << ", Empt = "
                     << std::setw(1) << mess.getGdpbSysErrUnused()
                     << ", Data = " << std::hex << std::setw(2) << uData
                     << std::dec << " -- GET4 V1 Error Event " << iMess;
      break;
    } // case ngdpb::SYSMSG_GET4_EVENT
    case ngdpb::SYSMSG_CLOSYSYNC_ERROR:
      LOG(info) << "Closy synchronization error";
      break;
    case ngdpb::SYSMSG_TS156_SYNC:
      LOG(debug) << "160.00 MHz timestamp reset";
      break;
    case ngdpb::SYSMSG_GDPB_UNKWN:
      LOG(info) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                   << mess.getGdpbSysUnkwData() << std::dec
                   <<" Full message: " << std::hex << std::setw(16)
                   << mess.getData() << std::dec;
      break;
  } // switch( getGdpbSysSubType() )
}

void CbmTSMonitorTofLab::FillStarTrigInfo(ngdpb::Message mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

//  mess.printDataCout();

  switch( iMsgIndex )
  {
      case 0:
         fhTokenMsgType->Fill(0);
         fulGdpbTsMsb = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fhTokenMsgType->Fill(1);
         fulGdpbTsLsb = mess.getGdpbTsLsbStarB();
         fulStarTsMsb = mess.getStarTsMsbStarB();
         break;
      case 2:
         fhTokenMsgType->Fill(2);
         fulStarTsMid = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         fhTokenMsgType->Fill(3);
         ULong64_t ulNewGdpbTsFull = ( fulGdpbTsMsb << 24 )
                           + ( fulGdpbTsLsb       );
         ULong64_t ulNewStarTsFull = ( fulStarTsMsb << 48 )
                           + ( fulStarTsMid <<  8 )
                           + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

         if( ( uNewToken == fuStarTokenLast ) && ( ulNewGdpbTsFull == fulGdpbTsFullLast ) &&
             ( ulNewStarTsFull == fulStarTsFullLast ) && ( uNewDaqCmd == fuStarDaqCmdLast ) &&
             ( uNewTrigCmd == fuStarTrigCmdLast ) )
         {
            LOG(debug) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << Form("token = %5u ", fuStarTokenLast )
                         << Form("gDPB ts  = %12llu ", fulGdpbTsFullLast )
                         << Form("STAR ts = %12llu ", fulStarTsFullLast )
                         << Form("DAQ cmd = %2u ", fuStarDaqCmdLast )
                         << Form("TRG cmd = %2u ", fuStarTrigCmdLast );
            return;
         } // if exactly same message repeated

         // STAR TS counter reset detection
         if( ulNewStarTsFull < fulStarTsFullLast )
            LOG(debug) << "Probable reset of the STAR TS: old = " << Form("%16llu", fulStarTsFullLast)
                       << " new = " << Form("%16llu", ulNewStarTsFull)
                       << " Diff = -" << Form("%8llu", fulStarTsFullLast - ulNewStarTsFull);

//         ULong64_t ulGdpbTsDiff = ulNewGdpbTsFull - fulGdpbTsFullLast;
         fulGdpbTsFullLast = ulNewGdpbTsFull;
         fulStarTsFullLast = ulNewStarTsFull;
         fuStarTokenLast   = uNewToken;
         fuStarDaqCmdLast  = uNewDaqCmd;
         fuStarTrigCmdLast = uNewTrigCmd;

         if( 0 <= fdStartTime )
         {
            fhTriggerRate->Fill(
               1e-9 * ( fulGdpbTsFullLast * 6.25 - fdStartTime ) );
            fhStarTokenEvo->Fill(
               1e-9 * ( fulGdpbTsFullLast * 6.25 - fdStartTime ),
               fuStarTokenLast );
		   } // if( 0 < fdStartTime )
            else fdStartTime = fulGdpbTsFullLast * 6.25;
         fhCmdDaqVsTrig->Fill( fuStarDaqCmdLast, fuStarTrigCmdLast );

         break;
	  } // case 3
      default:
         LOG(fatal) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  } // switch( iMsgIndex )
}
void CbmTSMonitorTofLab::FillTrigEpochInfo(ngdpb::Message mess)
{
  Int_t epochNr = mess.getGdpbEpEpochNb();
  fGet4Id = mess.getGdpbGenChipId();
  fGet4Nr = (fGdpbNr * fNrOfGet4PerGdpb) + fGet4Id;

  if (100 > iMess)
  {
      LOG(debug) << "Epoch message for get4 " << fGet4Id
                 << " in gDPB " << fGdpbNr
                 <<" with epoch number " << epochNr;
    iMess++; // Separate increment from condition, otherwise increase until wrap to -1664185233
  } // if (100 > iMess)

  fCurrentEpochTime = mess.getMsgFullTime(epochNr);
  fNofEpochs++;

   if( fbEpochSuppModeOn )
   {
      /// Re-align the epoch number of the message in case it will be used later:
      /// We received the epoch after the data instead of the one before!
      //// The epoch index from the token message should not need to be re-aligned
      //// ====> to be checked with pulser or data-trigger correlation
//      if( 0 < epochNr )
//         mess.setEpoch2Number( epochNr - 1 );

   } // if( fbEpochSuppModeOn )
}

void CbmTSMonitorTofLab::Reset()
{
}

void CbmTSMonitorTofLab::Finish()
{
   // Printout some stats on what was unpacked
  TString message_type;
  for (unsigned int i = 0; i < fMsgCounter.size(); ++i) {
    switch (i) {
      case 0:
        message_type = "NOP";
        break;
      case 1:
        message_type = "HIT";
        break;
      case 2:
        message_type = "EPOCH";
        break;
      case 3:
        message_type = "SYNC";
        break;
      case 4:
        message_type = "AUX";
        break;
      case 5:
        message_type = "EPOCH2";
        break;
      case 6:
        message_type = "GET4";
        break;
      case 7:
        message_type = "SYS";
        break;
      case 8:
        message_type = "GET4_SLC";
        break;
      case 9:
        message_type = "GET4_32B";
        break;
      case 10:
        message_type = "GET4_SYS";
        break;
      default:
        message_type = "UNKNOWN";
        break;
    } // switch(i)
    LOG(info) << message_type << " messages: " << fMsgCounter[i];
  } // for (unsigned int i=0; i< fMsgCounter.size(); ++i)

  LOG(info) << "-------------------------------------";
  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    for (UInt_t j = 0; j < fNrOfGet4PerGdpb; ++j) {
      LOG(info) << "Last epoch for gDPB: " << std::hex << std::setw(4) << i
                   << std::dec << " , GET4  " << std::setw(4) << j << " => "
                   << fCurrentEpoch[GetArrayIndex(i, j)]
                   << " Cycle " << fuCurrentEpochCycle[GetArrayIndex(i, j)];
    }
  }
  LOG(info) << "-------------------------------------";

  SaveAllHistos();
}

void CbmTSMonitorTofLab::FillOutput(boost::any)
{
}

void CbmTSMonitorTofLab::SaveAllHistos( TString sFileName )
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

  gDirectory->mkdir("Tof_Raw_gDPB");
  gDirectory->cd("Tof_Raw_gDPB");
  for (UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb++) {

    for (UInt_t uFeetPlot = 0; uFeetPlot < fNrOfFebsPerGdpb/uNbFeetPlot; ++uFeetPlot )
      fHM->H2(Form("Raw_Tot_gDPB_%02u_%1u", uGdpb, uFeetPlot))->Write();

    fHM->H1(Form("ChCount_gDPB_%02u", uGdpb))->Write();
    if( fbGet4M24b )
      fHM->H1(Form("ChCountFall_gDPB_%02u", uGdpb))->Write();

//    if (fUnpackPar->IsChannelRateEnabled())
//      fHM->H2(Form("ChannelRate_gDPB_%02u", uGdpb))->Write();
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      fHM->H1(Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      fHM->H1(Form("FeetErrorRate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      fHM->H1(Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      fHM->H1(Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
        fHM->H1(Form("FeetRateDate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      }
      fHM->H2(Form("FtDistribPerCh_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      if( fbGet4M24b )
         fHM->H2(Form("FtDistribPerChFall_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
    }
  } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  fHM->H1("hMessageType")->Write();
  fHM->H1("hSysMessType")->Write();
  fHM->H2("hGet4MessType")->Write();
  fHM->H2("hGet4ChanErrors")->Write();
  fHM->H2("hGet4EpochFlags")->Write();
  fHM->H1("hSpillLength")->Write();
  fHM->H1("hSpillCount")->Write();
  fHM->H1("hSpillQA")->Write();

  gDirectory->cd("..");

  ///* ASIC coincidences & offsets mapping *///
  if( fbEnableCoincMap )
  {
     gDirectory->mkdir("TofCoinc");
     gDirectory->cd("TofCoinc");
     for( UInt_t uAsicA = 0; uAsicA < fvhCoincOffsetEpochGet4.size(); uAsicA ++ )
         fvhCoincOffsetEpochGet4[uAsicA]->Write();
     gDirectory->cd("..");
  } // if( fbEnableCoincMap )

  ///* STAR event building/cutting *///
  gDirectory->mkdir("Star_Raw");
  gDirectory->cd("Star_Raw");
  fhTokenMsgType->Write();
  fHM->H1("hTriggerRate")->Write();
  fHM->H1("hCmdDaqVsTrig")->Write();
  fHM->H1("hStarTokenEvo")->Write();
  gDirectory->cd("..");

  gDirectory->mkdir("Flib_Raw");
  gDirectory->cd("Flib_Raw");
  for (UInt_t uLinks = 0; uLinks < 16; uLinks++) {
    TString sMsSzName = Form("MsSz_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data()))
      fHM->H1(sMsSzName.Data())->Write();

    sMsSzName = Form("MsSzTime_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data()))
      fHM->P1(sMsSzName.Data())->Write();
  } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)
  gDirectory->cd("..");

  if( "" != sFileName )
  {
     // Restore original directory position
     histoFile->Close();
     oldDir->cd();
  } // if( "" != sFileName )

}

void CbmTSMonitorTofLab::ResetAllHistos()
{
  LOG(info) << "Reseting all TOF histograms.";
  fHM->H1("hMessageType")->Reset();
  fHM->H1("hSysMessType")->Reset();
  fHM->H2("hGet4MessType")->Reset();
  fHM->H2("hGet4ChanErrors")->Reset();
  fHM->H2("hGet4EpochFlags")->Reset();
  fHM->H1("hSpillLength")->Reset();
  fHM->H1("hSpillCount")->Reset();
  fHM->H2("hSpillQA")->Reset();
  fSpillIdx = 0;

  for (UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb++) {
    fHM->H1(Form("ChCount_gDPB_%02u", uGdpb))->Reset();
    if( fbGet4M24b )
      fHM->H1(Form("ChCountFall_gDPB_%02u", uGdpb))->Reset();

    for (UInt_t uFeetPlot = 0; uFeetPlot < fNrOfFebsPerGdpb/uNbFeetPlot; ++uFeetPlot )
      fHM->H2(Form("Raw_Tot_gDPB_%02u_%1u", uGdpb, uFeetPlot))->Reset();

//    if (fUnpackPar->IsChannelRateEnabled())
//      fHM->H2(Form("ChannelRate_gDPB_%02u", uGdpb))->Reset();
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      fHM->H1(Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
      fHM->H1(Form("FeetErrorRate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
      fHM->H1(Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
      fHM->H1(Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
      fHM->H2(Form("FtDistribPerCh_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
      if( fbGet4M24b )
      {
         fHM->H2(Form("FtDistribPerChFall_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
         fHM->H2(Form("FtLastRiseCurrFall_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
         fHM->H2(Form("FtCurrRiseLastFall_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
         fHM->H2(Form("FtLastRiseDistRise_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
         fHM->H2(Form("FtLastRiseDistFall_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
      } // if( fbGet4M24b )
    } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
  } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)

  for (UInt_t uLinks = 0; uLinks < 16; uLinks++) {
    TString sMsSzName = Form("MsSz_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data()))
      fHM->H1(sMsSzName.Data())->Reset();

    sMsSzName = Form("MsSzTime_link_%02u", uLinks);
    if (fHM->Exists(sMsSzName.Data()))
      fHM->P1(sMsSzName.Data())->Reset();
  } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)

  if( fbPulserMode )
  {
     fHM->H2("hTimeRmsPulserChosenFee")->Reset();
     fHM->H1("hTimeRmsPulserChosenChPairs")->Reset();
     fdLastRmsUpdateTime = -1;

      // Full Fee time difference test
      UInt_t uHistoFeeIdx = 0;
      for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)
      {
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
         {
            fhTimeDiffPulserChosenFee[uHistoFeeIdx]->Reset();
            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)

      // Selected channels test
      for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
         fhTimeDiffPulserChosenChPairs[uChan]->Reset();
  } // if( fbPulserMode )

  ///* STAR event building/cutting *///
  fhTokenMsgType->Reset();
  fHM->H1("hTriggerRate")->Reset();
  fHM->H1("hCmdDaqVsTrig")->Reset();
  fHM->H1("hStarTokenEvo")->Reset();

  fdStartTime = -1;
  fdStartTimeMsSz = -1;
}
void CbmTSMonitorTofLab::CyclePulserFee()
{
  if( fbPulserMode )
  {
     fHM->H2("hTimeRmsPulserChosenFee")->Reset();
     fdLastRmsUpdateTime = -1;

      // Full Fee time difference test
      UInt_t uHistoFeeIdx = 0;
      for( UInt_t uChanFeeA = 0; uChanFeeA < fNrOfChannelsPerFeet; uChanFeeA++)
      {
         for( UInt_t uChanFeeB = uChanFeeA + 1; uChanFeeB < fNrOfChannelsPerFeet; uChanFeeB++)
         {
            fhTimeDiffPulserChosenFee[uHistoFeeIdx]->Reset();
            uHistoFeeIdx++;
         } // for any unique pair of channel in chosen Fee
      } // for( UInt_t uChanFeeA = 0; uChanFeeA < kuNbChanFee; uChanFeeA++)

      // Cycle the chosen FEE index within the current chosen gDPB
      fuPulserFee = (fuPulserFee + 1)%fNrOfFebsPerGdpb;

      LOG(info) << "FEE index for pulser RMS histo changed to : " << fuPulserFee;
  } // if( fbPulserMode )

  if( fbGet4M24b )
  {
      // Full Fee time difference test
      UInt_t uHistoFeeIdx = 0;
      for (UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb++)
      {
         for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++)
         {
            fhFtLastRiseCurrFall[uHistoFeeIdx]->Reset();
            fhFtCurrRiseLastFall[uHistoFeeIdx]->Reset();
            fhFtLastRiseDistRise[uHistoFeeIdx]->Reset();
            fhFtLastRiseDistFall[uHistoFeeIdx]->Reset();
            uHistoFeeIdx++;
         } // for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++)
      } // for (UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb++)

      // Cycle the chosen FEE index within the current chosen gDPB
      fuRiseFallChSel = (fuRiseFallChSel + 1)%fNrOfChannelsPerFeet;

      LOG(info) << "Channel index for 24b FT Rise vs FT Fall histo changed to : " << fuRiseFallChSel;
  } //

}


void CbmTSMonitorTofLab::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new TOF Run Start Date-Time: " << fRunStartDateTime->AsString();
}

void CbmTSMonitorTofLab::SetPulserChans(
      UInt_t inPulserChanA, UInt_t inPulserChanB, UInt_t inPulserChanC, UInt_t inPulserChanD,
      UInt_t inPulserChanE, UInt_t inPulserChanF, UInt_t inPulserChanG, UInt_t inPulserChanH,
      UInt_t inPulserChanI, UInt_t inPulserChanJ, UInt_t inPulserChanK, UInt_t inPulserChanL,
      UInt_t inPulserChanM, UInt_t inPulserChanN, UInt_t inPulserChanO, UInt_t inPulserChanP )
{
   fuPulserChan[ 0] = inPulserChanA;
   fuPulserChan[ 1] = inPulserChanB;
   fuPulserChan[ 2] = inPulserChanC;
   fuPulserChan[ 3] = inPulserChanD;
   fuPulserChan[ 4] = inPulserChanE;
   fuPulserChan[ 5] = inPulserChanF;
   fuPulserChan[ 6] = inPulserChanG;
   fuPulserChan[ 7] = inPulserChanH;
   fuPulserChan[ 8] = inPulserChanI;
   fuPulserChan[ 9] = inPulserChanJ;
   fuPulserChan[10] = inPulserChanK;
   fuPulserChan[11] = inPulserChanL;
   fuPulserChan[12] = inPulserChanM;
   fuPulserChan[13] = inPulserChanN;
   fuPulserChan[14] = inPulserChanO;
   fuPulserChan[15] = inPulserChanP;
}

ClassImp(CbmTSMonitorTofLab)
