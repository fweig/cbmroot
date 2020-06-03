// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorTof                            -----
// -----               Created 27.10.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTSMonitorTof.h"
#include "CbmTofUnpackPar.h"

#include "CbmFlibCern2016Source.h"
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

#include <iostream>
#include <stdint.h>
#include <iomanip>

static Int_t iMess = 0;
Bool_t bResetTofHistos = kFALSE;


CbmTSMonitorTof::CbmTSMonitorTof() :
    CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuOverlapMsNb(0),
    fuMinNbGdpb(),
    fuCurrNbGdpb(0),
    fNrOfGdpbs(0),
    fNrOfFebsPerGdpb(0),
    fNrOfGet4PerFeb(0),
    fNrOfChannelsPerGet4(0),
    fNrOfGet4(0),
    fNrOfGet4PerGdpb(0),
    fDiamondGdpb(0),
    fDiamondFeet(0),
    fDiamondChanA(0),
    fDiamondChanB(0),
    fDiamondChanC(0),
    fDiamondChanD(0),
    fDiamondTimeLastReset(-1),
    fiDiamCountsLastTs(0),
    fiDiamSpillOnThr(10),
    fiDiamSpillOffThr(3),
    fiTsUnderOff(0),
    fiTsUnderOffThr(10),
    fdDiamondLastTime(-1.),
    fdDiamondTimeLastTs(-1.),
    fbSpillOn(kFALSE),
    fSpillIdx(0),
    fbBeamTuningMode( kFALSE),
    fbEpochSuppModeOn(kFALSE),
    fvmEpSupprBuffer(),
    fGdpbId(0),
    fGdpbNr(0),
    fGet4Id(0),
    fGet4Nr(0),
    fMsgCounter(11, 0), // length of enum MessageTypes initialized with 0
    fGdpbIdIndexMap(),
    fHM(new CbmHistManager()),
    fCurrentEpoch(NULL),
    fNofEpochs(0),
    fCurrentEpochTime(0.),
    fdStartTime(-1.),
    fdStartTimeMsSz(-1.),
    fcMsSizeAll(NULL),
    fTsLastHit(),
    fEquipmentId(0),
    fUnpackPar(NULL),
    fHistMessType(NULL),
    fHistSysMessType(NULL),
    fHistGet4MessType(NULL),
    fHistGet4ChanErrors(NULL),
    fHistGet4EpochFlags(NULL),
    fHistDiamond(NULL),
    fHistDiamondSpill(NULL),
    fHistDiamondSpillLength(NULL),
    fHistDiamondSpillCount(NULL),
    fHistDiamondSpillQA(NULL),
    fRaw_Tot_gDPB(),
    fChCount_gDPB(),
    fChannelRate_gDPB(),
    fFeetRate_gDPB(),
    fFeetRateDate_gDPB(),
    fiRunStartDateTimeSec( -1 ),
    fiBinSizeDatePlots( -1 )
{
}

CbmTSMonitorTof::~CbmTSMonitorTof()
{
  delete fHM; //TODO: Who deletes the histograms stored in the CbmHistManager???
  delete[] fCurrentEpoch;
}

Bool_t CbmTSMonitorTof::Init()
{
  LOG(info) << "Initializing Get4 monitor";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmTSMonitorTof::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar =
      (CbmTofUnpackPar*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
          "CbmTofUnpackPar"));

}

Bool_t CbmTSMonitorTof::InitContainers()
{
  LOG(info) << "Init parameter containers for " << GetName();
  Bool_t initOK = ReInitContainers();

  CreateHistograms();

  fCurrentEpoch = new Int_t[fNrOfGdpbs * fNrOfGet4PerGdpb];
  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    for (UInt_t j = 0; j < fNrOfGet4PerGdpb; ++j) {
      fCurrentEpoch[GetArrayIndex(i, j)] = -111;
    }
  }

  return initOK;
}

Bool_t CbmTSMonitorTof::ReInitContainers()
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

  fNrOfGet4 = fNrOfGdpbs * fNrOfFebsPerGdpb * fNrOfGet4PerFeb;
  LOG(info) << "Nr. of GET4s: " << fNrOfGet4;

  fNrOfGet4PerGdpb = fNrOfFebsPerGdpb * fNrOfGet4PerFeb;
  LOG(info) << "Nr. of GET4s per GDPB: " << fNrOfGet4PerGdpb;

  fGdpbIdIndexMap.clear();
  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
    LOG(info) << "GDPB Id of TOF  " << i << " : " << fUnpackPar->GetRocId(i);
  }
  UInt_t NrOfChannels = fUnpackPar->GetNumberOfChannels();
  LOG(info) << "Nr. of mapped Tof channels: " << NrOfChannels;
  std::stringstream ss;
	for (UInt_t i = 0; i< NrOfChannels; ++i) {
      if(i%8 == 0)
         ss << "\n";
      ss << Form(" 0x%08x",fUnpackPar->GetChannelToDetUIdMap(i));
	}
	LOG(info) << ss.str();

  LOG(info) << "Plot Channel Rate => "
               << (fUnpackPar->IsChannelRateEnabled() ? "ON" : "OFF");

  if( fbEpochSuppModeOn )
      fvmEpSupprBuffer.resize( fNrOfGet4 );

  return kTRUE;
}

void CbmTSMonitorTof::CreateHistograms()
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
//  server->SetJSROOT("https://root.cern.ch/js/latest");

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
  TH2I* hGet4ChanErrors = new TH2I(name, title,
      fNrOfGet4 * fNrOfChannelsPerGet4, 0., fNrOfGet4 * fNrOfChannelsPerGet4,
      32, 0., 32.);
//  TH2I* hGet4ChanErrors    = new TH2I(name, title, uNbFeets*feetv1::kuChanPerFeet*2, -0.5, uNbFeets*feetv1::kuChanPerFeet -0.5, 32, -0.5, 31.5);
  hGet4ChanErrors->GetYaxis()->SetBinLabel(1, "0x00: Readout Init    ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(2, "0x01: Sync            ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(3, "0x02: Epoch count sync");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(4, "0x03: Epoch           ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(5, "0x04: FIFO Write      ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(6, "0x05: Lost event      ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(7, "0x06: Channel state   ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(8, "0x07: Token Ring state");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(9, "0x08: Token           ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
  hGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc."); // <- From GET4 v1.2
  hGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x7f: Unknown         ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(18,
      "Corrupt error or unsupported yet");
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
      name = Form("ChannelRate_gDPB_%02u", uGdpb);
      title = Form("Channel instant rate gDPB %02u; Dist[ns] ; Channel", uGdpb);
      fHM->Add(name.Data(),
          new TH2F(name.Data(), title.Data(), iNbBinsRate - 1, dBinsRate, 96, 0,
              96));
      if (server)
        server->Register("/TofRaw", fHM->H2(name.Data()));
      LOG(info) << "Adding the rate histos";
    } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  }

  for (UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb++) {
    name = Form("Raw_Tot_gDPB_%02u", uGdpb);
    title = Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb);
    fHM->Add(name.Data(),
        new TH2F(name.Data(), title.Data(), 96, 0, 96, 256, 0, 256));
    if (server)
      server->Register("/TofRaw", fHM->H2(name.Data()));

    name = Form("ChCount_gDPB_%02u", uGdpb);
    title = Form("Channel counts gDPB %02u; channel; Hits", uGdpb);
    fHM->Add(name.Data(), new TH1I(name.Data(), title.Data(), 96, 0, 96));

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
    } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
  } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)

  name = "hDiamond";
  title = "Counts per diamond in last 10s; X [pad]; Y [pad]; Counts";
  TH2I* hDiamond = new TH2I(name, title, 2, 0., 2, 2, 0., 2.);
  fHM->Add(name.Data(), hDiamond);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hDiamondSpill";
  title = "Counts per diamond in Current Spill; X [pad]; Y [pad]; Counts";
  TH2I* hDiamondSpill = new TH2I(name, title, 2, 0., 2, 2, 0., 2.);
  fHM->Add(name.Data(), hDiamondSpill);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hDiamondSpillLength";
  title = "Length of spill interval as found from diamond; Length [s]; Counts";
  TH1* hDiamondSpillLength = new TH1F(name, title, 3000, 0., 300.);
  fHM->Add(name.Data(), hDiamondSpillLength);
  if (server)
    server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hDiamondSpillCount";
  title = "Total counts in diamond in each spill; Spill; Counts";
  TH1* hDiamondSpillCount = new TH1F(name, title, 300, 0., 300.);
  fHM->Add(name.Data(), hDiamondSpillCount);
  if (server)
    server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hDiamondSpillQA";
  title = "Total counts in diamond per spill VS Spill length; Length [s]; Counts";
  TH2* hDiamondSpillQA = new TH2F(name, title, 120, 0., 120., 150, 0., 150000.);
  fHM->Add(name.Data(), hDiamondSpillQA);
  if (server)
    server->Register("/TofRaw", fHM->H2(name.Data()));

  if (server)
    server->RegisterCommand("/Reset_All_TOF", "bResetTofHistos=kTRUE");
  if (server)
    server->Restrict("/Reset_All_TOF", "allow=admin");

  /** Create summary Canvases for CERN 2016 **/
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
  hDiamondSpill->Draw("col text");


  /** Create FEET rates Canvase for CERN 2016 **/
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
    } // for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; ++uFeet )
  } // for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; ++uGdpb )

  /** Recovers/Create Ms Size Canvase for CERN 2016 **/
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

  /** Create beam tuning Canvases for CERN 2016 **/
  TCanvas* cBeamTunning = new TCanvas("cBeamTunning", "USTC D1 rate + DIam Rate + Diam centering", w, h);
  cBeamTunning->Divide(2, 2);

  gStyle->Reset();

  cBeamTunning->cd(1);
  histPnt = fHM->H1( "FeetRate_gDPB_g00_f0" );
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  histPnt->Draw();

  cBeamTunning->cd(2);
  histPnt = fHM->H1( "FeetRate_gDPB_g00_f2" );
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  histPnt->Draw();

  cBeamTunning->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  hDiamondSpillCount->Draw("text90");

  cBeamTunning->cd(4);
  hDiamondSpill->Draw("col text");
  /*****************************/


  fHistMessType = fHM->H1("hMessageType");
  fHistSysMessType = fHM->H1("hSysMessType");
  fHistGet4MessType = fHM->H2("hGet4MessType");
  fHistGet4ChanErrors = fHM->H2("hGet4ChanErrors");
  fHistGet4EpochFlags = fHM->H2("hGet4EpochFlags");
  fHistDiamond = fHM->H2("hDiamond");
  fHistDiamondSpill = fHM->H2("hDiamondSpill");
  fHistDiamondSpillLength = fHM->H1("hDiamondSpillLength");
  fHistDiamondSpillCount = fHM->H1("hDiamondSpillCount");
  fHistDiamondSpillQA = fHM->H2("hDiamondSpillQA");

  for (UInt_t i = 0; i < fNrOfGdpbs; ++i) {
    name = Form("Raw_Tot_gDPB_%02u", i);
    fRaw_Tot_gDPB.push_back(fHM->H2(name.Data()));
    name = Form("ChCount_gDPB_%02u", i);
    fChCount_gDPB.push_back(fHM->H1(name.Data()));
    name = Form("ChannelRate_gDPB_%02u", i);
    if (fUnpackPar->IsChannelRateEnabled()) {
      fChannelRate_gDPB.push_back(fHM->H2(name.Data()));
    }
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      name = Form("FeetRate_gDPB_g%02u_f%1u", i, uFeet);
      fFeetRate_gDPB.push_back(fHM->H1(name.Data()));
      if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
        name = Form("FeetRateDate_gDPB_g%02u_f%1u", i, uFeet);
        fFeetRateDate_gDPB.push_back(fHM->H1(name.Data()));
      } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
    } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
  }
  /*****************************/

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmTSMonitorTof::DoUnpack(const fles::Timeslice& ts,
    size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  if (bResetTofHistos) {
    ResetAllHistos();
    bResetTofHistos = kFALSE;
  }

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
        new TProfile(sMsSzName.Data(), sMsSzTitle.Data(), 15000, 0., 300.));
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
  fiDiamCountsLastTs = 0;

  Int_t messageType = -111;
  // Loop over microslices
  size_t numCompMsInTs = ts.num_microslices(component);
  for (size_t m = 0; m < numCompMsInTs; ++m) {
    if (fuMsAcceptsPercent < m)
      continue;

    // Ignore overlap ms if number defined by user
    if( numCompMsInTs - fuOverlapMsNb <= m )
      continue;

    constexpr uint32_t kuBytesPerMessage = 8;

    auto msDescriptor = ts.descriptor(component, m);
    fEquipmentId = msDescriptor.eq_id;
    const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(
        component, m));

    uint32_t size = msDescriptor.size;
    if (size > 0)
      LOG(debug) << "Microslice: " << msDescriptor.idx << " has size: " << size;

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

      if (gLogger->IsLogNeeded( fair::Severity::debug2 )) {
        mess.printDataCout();
      }

      // Increment counter for different message types
      // and fill the corresponding histogram
      messageType = mess.getMessageType();
      fMsgCounter[messageType]++;
      fHistMessType->Fill(messageType);

      fGdpbId = mess.getRocNumber();
      fGdpbNr = fGdpbIdIndexMap[fGdpbId];

      fGet4Id = mess.getGdpbGenChipId();
      fGet4Nr = (fGdpbNr * fNrOfGet4PerGdpb) + fGet4Id;

      // Jump most data in Beam tuning mode
      if( fbBeamTuningMode )
      {
          if( fGdpbNr != fDiamondGdpb )
            break;
//          if( fGet4Id / fNrOfGet4PerFeb != fDiamondFeet )
//            continue;
      } // if( fbBeamTuningMode )

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
          PrintGenInfo(mess);
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
        case ngdpb::MSG_GET4_SYS: {
          fHistSysMessType->Fill(mess.getGdpbSysSubType());
          if (ngdpb::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType()) {
            fHistGet4MessType->Fill(fGet4Nr, ngdpb::GET4_32B_ERROR);
            Int_t dFullChId =  fGet4Nr * fNrOfChannelsPerGet4 + mess.getGdpbHitChanId();
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
              case ngdpb::GET4_V1X_ERR_UNKNOWN:
                fHistGet4ChanErrors->Fill(dFullChId, 16);
                break;
              default: // Corrupt error or not yet supported error
                fHistGet4ChanErrors->Fill(dFullChId, 17);
                break;
            } // Switch( mess.getGdpbSysErrData() )

          }
          if (100 > iMess++)
            PrintSysInfo(mess);
          break;
        }
        default:
          if (100 > iMess++)
            LOG(error) << "Message (" << iMess << ") type " << std::hex
                          << std::setw(2) << static_cast<uint16_t>(messageType)
                          << " not yet included in Get4 unpacker.";
          if (100 == iMess)
            LOG(error) << "Stop reporting MSG errors... ";
      } // switch( mess.getMessageType() )
    } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
  } // for (size_t m = 0; m < ts.num_microslices(component); ++m)

  // Spill detection using Diamond
  if( !fbSpillOn && (fiDiamSpillOnThr < fiDiamCountsLastTs) )
  {
    fbSpillOn = kTRUE;
    fiTsUnderOff = 0;

    if( 0 < fdDiamondTimeLastTs )
    {
      fHistDiamondSpillLength->Fill( fdDiamondLastTime - fdDiamondTimeLastTs );
      fHistDiamondSpillCount->Fill( fSpillIdx, fHistDiamondSpill->GetEntries() );
      fHistDiamondSpillQA->Fill( fdDiamondLastTime - fdDiamondTimeLastTs,
                                 fHistDiamondSpill->GetEntries() );
    } // if( 0 < fdDiamondTimeLastTs )
    fdDiamondTimeLastTs = fdDiamondLastTime;
    fSpillIdx++;
    fHistDiamondSpill->Reset();
  } // if( !fbSpillOn && (fiDiamSpillOnThr < fiDiamCountsLastTs) )
  else if( fbSpillOn  )
  {
    if( fiDiamCountsLastTs < fiDiamSpillOffThr )
    {
      fiTsUnderOff ++;
      if( fiTsUnderOffThr < fiTsUnderOff )
        fbSpillOn = kFALSE;
    } //  if( fiDiamCountsLastTs < fiDiamSpillOffThr )
      else fiTsUnderOff = 0;
  } // else if( fbSpillOn  )

  return kTRUE;
}

void CbmTSMonitorTof::FillHitInfo(ngdpb::Message mess)
{
  Int_t channel = mess.getGdpbHitChanId();
  Int_t tot = mess.getGdpbHit32Tot();
  ULong_t hitTime = mess.getMsgFullTime(0);

  Int_t curEpochGdpbGet4 = fCurrentEpoch[fGet4Nr];

  if (curEpochGdpbGet4 != -111) {

    if( fbEpochSuppModeOn )
      curEpochGdpbGet4 --; // In Ep. Suppr. Mode, receive following epoch instead of previous

    Int_t channelNr = fGet4Id * fNrOfChannelsPerGet4 + channel;
    fRaw_Tot_gDPB[fGdpbNr]->Fill(channelNr, tot);
    fChCount_gDPB[fGdpbNr]->Fill(channelNr);

    if (fUnpackPar->IsChannelRateEnabled()) {
      // Check if at least one hit before in this gDPB
      if (fTsLastHit.end() != fTsLastHit.find(fGdpbNr)) {
        // Check if at least one hit before in this Get4
        if (fTsLastHit[fGdpbNr].end() != fTsLastHit[fGdpbNr].find(fGet4Id)) {
          // Check if at least one hit before in this channel
          if (fTsLastHit[fGdpbNr][fGet4Id].end()
              != fTsLastHit[fGdpbNr][fGet4Id].find(channel)) {
            fChannelRate_gDPB[fGdpbNr]->Fill(
                1e9
                    / (mess.getMsgFullTimeD(curEpochGdpbGet4)
                        - fTsLastHit[fGdpbNr][fGet4Id][channel]),
                fGet4Id * fNrOfChannelsPerGet4 + channel);
          } // if( fTsLastHit[gdpbId][get4Id].end() != fTsLastHit[gdpbId][get4Id].find( channel ) )
        } // if( fTsLastHit[gdpbId].end() != fTsLastHit[gdpbId].find( get4Id ) )
      } // if( fTsLastHit.end() != fTsLastHit.find( gdpbId ) )

      fTsLastHit[fGdpbNr][fGet4Id][channel] = mess.getMsgFullTimeD(
          curEpochGdpbGet4);
    } // if( fUnpackPar->IsChannelRateEnabled() )

    // In Run rate evolution
    if (fdStartTime < 0)
      fdStartTime = mess.getMsgFullTimeD(curEpochGdpbGet4);

    if (0 < fdStartTime)
    {
      fFeetRate_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + (fGet4Id / fNrOfGet4PerFeb)]->Fill(
          1e-9 * (mess.getMsgFullTimeD(curEpochGdpbGet4) - fdStartTime));

       // General Time (date + time) rate evolution
       // Add offset of -1H as the filenames were using some times offset by 1 hour (Summer time?!?)
       if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
         fFeetRateDate_gDPB[(fGdpbNr * fNrOfFebsPerGdpb) + (fGet4Id / fNrOfGet4PerFeb)]->Fill(
             1e-9 * (mess.getMsgFullTimeD(curEpochGdpbGet4) - fdStartTime)  );
       } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
    }

     if (fDiamondGdpb == fGdpbNr
         && (fGet4Id / fNrOfGet4PerFeb == fDiamondFeet)) {
       UInt_t uChanInGdpb = fGet4Id * fNrOfChannelsPerGet4 + channel;
       Int_t increment = static_cast<CbmFlibCern2016Source*>(FairRunOnline::Instance()->GetSource())->GetNofTSSinceLastTS();
       if (fDiamondChanA == uChanInGdpb)
       {
         fHistDiamond->Fill(0., 0., increment);
         fHistDiamondSpill->Fill(0., 0., increment);
       } // if (fDiamondChanA == uChanInGdpb)
       else if (fDiamondChanB == uChanInGdpb)
       {
         fHistDiamond->Fill(1., 0., increment);
         fHistDiamondSpill->Fill(1., 0., increment);
       } // else if (fDiamondChanB == uChanInGdpb)
       else if (fDiamondChanC == uChanInGdpb)
       {
         fHistDiamond->Fill(0., 1., increment);
         fHistDiamondSpill->Fill(0., 1., increment);
       } // else if (fDiamondChanC == uChanInGdpb)
       else if (fDiamondChanD == uChanInGdpb)
       {
         fHistDiamond->Fill(1., 1., increment);
         fHistDiamondSpill->Fill(1., 1., increment);
       } // else if (fDiamondChanD == uChanInGdpb)

       if( fDiamondChanA == uChanInGdpb || fDiamondChanB == uChanInGdpb ||
           fDiamondChanC == uChanInGdpb || fDiamondChanD == uChanInGdpb )
       {
         fiDiamCountsLastTs ++;
         fdDiamondLastTime = 1e-9 * mess.getMsgFullTimeD(curEpochGdpbGet4);
       } // if channel match one of the diamond ones

     } // if( fDiamondGdpb == gdpbNr && ( get4Id/fNrOfGet4PerFeb == fDiamondFeet ) )

     hitTime = mess.getMsgFullTime(curEpochGdpbGet4);
     Int_t Ft = mess.getGdpbHitFineTs();

     if (100 > iMess++)
       LOG(debug) << "Hit: " << Form("0x%08x ", fGdpbId) << ", " << fGet4Id
                     << ", " << channel << ", " << tot << ", epoch "
                     << curEpochGdpbGet4 << ", FullTime "
                     << hitTime << ", FineTime " << Ft;
  } // if( fCurrentEpoch[rocId].end() != fCurrentEpoch[rocId].find( get4Id ) )
    //} // if( fCurrentEpoch.end() != fCurrentEpoch.find( rocId ) )
  //} // if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
  else
    LOG(warning) << "Found hit in gdpbId w/o epoch yet: "
                    << Form("0x%08x g4 %02u", fGdpbId, fGet4Nr);
}

void CbmTSMonitorTof::FillEpochInfo(ngdpb::Message mess)
{
  Int_t epochNr = mess.getEpoch2Number();
  fCurrentEpoch[fGet4Nr] = epochNr;

  if (100 > iMess++)
      LOG(debug) << "Epoch message for get4 " << fGet4Nr << " with epoch number "
                 << fCurrentEpoch[fGet4Nr];


  if( fbEpochSuppModeOn )
  {
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
    } // if( 0 < fvmEpSupprBuffer[fGet4Nr] )
  } // if( fbEpochSuppModeOn )

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


  if (fDiamondGdpb == fGdpbNr
      && (fGet4Id / fNrOfGet4PerFeb == fDiamondFeet)) {
    if (10
        < ((mess.getMsgFullTimeD(epochNr) / 1e9)
            - fDiamondTimeLastReset)) {
      fHistDiamond->Reset();
      fDiamondTimeLastReset = mess.getMsgFullTimeD(epochNr) / 1e9;
    } // if( 1e10 < ( mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] ) - fDiamondTimeLastReset )
  } // if( fDiamondGdpb == gdpbNr && ( get4Id/fNrOfGet4PerFeb == fDiamondFeet ) )

  /*
   LOG(debug) << "Epoch message "
   << fNofEpochs << ", epoch " << static_cast<Int_t>(fCurrentEpoch[gdpbId][get4Id])
   << ", time " << std::setprecision(9) << std::fixed
   << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
   << " for board ID " << std::hex << std::setw(4) << gdpbId << std::dec
   << " and chip " << mess.getEpoch2ChipNumber();
   */
}

void CbmTSMonitorTof::PrintSlcInfo(ngdpb::Message mess)
{
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

void CbmTSMonitorTof::PrintGenInfo(ngdpb::Message mess)
{
  Int_t mType = mess.getMessageType();
  Int_t channel = mess.getGdpbHitChanId();
  uint64_t uData = mess.getData();

  if (100 > iMess++)
    LOG(info) << "Get4 MSG type " << mType << " from gdpbId " << fGdpbId
                 << ", getId " << fGet4Id << ", (hit channel) " << channel
                 << " data " << std::hex << uData;
//Form(" data 0x%01Fx ",uData)
}

void CbmTSMonitorTof::PrintSysInfo(ngdpb::Message mess)
{
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
          || ngdpb::GET4_V1X_ERR_EVT_DISCARD == uData)
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
                     << std::dec << " -- GET4 V1 Error Event";
      break;
    } // case ngdpb::SYSMSG_GET4_EVENT
    case ngdpb::SYSMSG_CLOSYSYNC_ERROR:
      LOG(info) << "Closy synchronization error";
      break;
    case ngdpb::SYSMSG_TS156_SYNC:
      LOG(debug) << "156.25MHz timestamp reset";
      break;
    case ngdpb::SYSMSG_GDPB_UNKWN:
      LOG(info) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                   << mess.getGdpbSysUnkwData() << std::dec;
      break;
  } // switch( getGdpbSysSubType() )
}

void CbmTSMonitorTof::Reset()
{
}

void CbmTSMonitorTof::Finish()
{
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
                   << fCurrentEpoch[GetArrayIndex(i, j)];
    }
  }
  LOG(info) << "-------------------------------------";

  gDirectory->mkdir("Tof_Raw_gDPB");
  gDirectory->cd("Tof_Raw_gDPB");
  for (UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb++) {
    fHM->H2(Form("Raw_Tot_gDPB_%02u", uGdpb))->Write();
    fHM->H1(Form("ChCount_gDPB_%02u", uGdpb))->Write();
    if (fUnpackPar->IsChannelRateEnabled())
      fHM->H2(Form("ChannelRate_gDPB_%02u", uGdpb))->Write();
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      fHM->H1(Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
        fHM->H1(Form("FeetRateDate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Write();
      }
    }
  } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  fHM->H1("hMessageType")->Write();
  fHM->H1("hSysMessType")->Write();
  fHM->H2("hGet4MessType")->Write();
  fHM->H2("hGet4ChanErrors")->Write();
  fHM->H2("hGet4EpochFlags")->Write();
  fHM->H1("hDiamondSpillLength")->Write();
  fHM->H1("hDiamondSpillCount")->Write();
  fHM->H1("hDiamondSpillQA")->Write();

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

}

void CbmTSMonitorTof::FillOutput(boost::any)
{
}

void CbmTSMonitorTof::SetDiamondChannels( UInt_t uGdpb, UInt_t uFeet, UInt_t uChannelA,
                            UInt_t uChannelB, UInt_t uChannelC, UInt_t uChannelD)
{
   fDiamondGdpb  = uGdpb;
   fDiamondFeet  = uFeet;
   fDiamondChanA = uChannelA;
   fDiamondChanB = uChannelB;
   fDiamondChanC = uChannelC;
   fDiamondChanD = uChannelD;
}


void CbmTSMonitorTof::ResetAllHistos()
{
  LOG(info) << "Reseting all TOF histograms.";
  fHM->H1("hMessageType")->Reset();
  fHM->H1("hSysMessType")->Reset();
  fHM->H2("hGet4MessType")->Reset();
  fHM->H2("hGet4ChanErrors")->Reset();
  fHM->H2("hGet4EpochFlags")->Reset();
  fHM->H1("hDiamondSpillLength")->Reset();
  fHM->H1("hDiamondSpillCount")->Reset();
  fHM->H2("hDiamondSpillQA")->Reset();
  fSpillIdx = 0;

  for (UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb++) {
    fHM->H2(Form("Raw_Tot_gDPB_%02u", uGdpb))->Reset();
    fHM->H1(Form("ChCount_gDPB_%02u", uGdpb))->Reset();
    if (fUnpackPar->IsChannelRateEnabled())
      fHM->H2(Form("ChannelRate_gDPB_%02u", uGdpb))->Reset();
    for (UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet++) {
      fHM->H1(Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet))->Reset();
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

  fdStartTime = -1;
  fdStartTimeMsSz = -1;
}

void CbmTSMonitorTof::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new TOF Run Start Date-Time: " << fRunStartDateTime->AsString();
}

ClassImp(CbmTSMonitorTof)
