// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTSMonitorTofLegacy                            -----
// -----               Created 27.10.2016 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTSMonitorTofLegacy.h"
#include "CbmTofUnpackPar.h"
//#include "CbmTofDigiExp.h"

//#include "CbmTbDaqBuffer.h"

//#include "CbmFiberHodoAddress.h"
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

#include <iostream>
#include <stdint.h>
#include <iomanip>

static Int_t iMess=0;

CbmTSMonitorTofLegacy::CbmTSMonitorTofLegacy()
  : CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuMinNbGdpb(),
    fuCurrNbGdpb( 0 ),
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
    fMsgCounter(11,0), // length of enum MessageTypes initialized with 0
    fGdpbIdIndexMap(),
    fHM(new CbmHistManager()),
    fCurrentEpoch(),
    fNofEpochs(0),
    fCurrentEpochTime(0.),
    fdStartTime(-1.),
    fTsLastHit(),
    fEquipmentId(0),
    fUnpackPar(NULL)
{
}

CbmTSMonitorTofLegacy::~CbmTSMonitorTofLegacy()
{
}

Bool_t CbmTSMonitorTofLegacy::Init()
{
   LOG(info) << "Initializing Get4 monitor";

   FairRootManager* ioman = FairRootManager::Instance();
   if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
   }

   return kTRUE;
}

void CbmTSMonitorTofLegacy::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackPar = (CbmTofUnpackPar*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmTofUnpackPar"));

}

Bool_t CbmTSMonitorTofLegacy::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();
   Bool_t initOK = ReInitContainers();

   CreateHistograms();

   return initOK;
}

Bool_t CbmTSMonitorTofLegacy::ReInitContainers()
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
   for (UInt_t i = 0; i< fNrOfGdpbs; ++i) {
     fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
     LOG(info) << "GDPB Id of TOF  " << i
               << " : " << fUnpackPar->GetRocId(i);
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

   LOG(info) << "Plot Channel Rate => " << ( fUnpackPar->IsChannelRateEnabled() ? "ON" : "OFF");

   return kTRUE;
}

void CbmTSMonitorTofLegacy::CreateHistograms()
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  TString name{""};
  TString title{""};

  name = "hMessageType";
  title = "Nb of message for each type; Type";
  // Test Big Data readout with plotting
  TH1I* hMessageType = new TH1I(name, title, 16, 0., 16.);
  //  TH1I* hMessageType = new TH1I(name, title, 16, -0.5, 15.5);
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_NOP,      "NOP");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_HIT,      "HIT");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_EPOCH,    "EPOCH");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_SYNC,     "SYNC");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_AUX,      "AUX");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_EPOCH2,   "EPOCH2");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4,     "GET4");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_SYS,      "SYS");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_SLC, "MSG_GET4_SLC");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_32B, "MSG_GET4_32B");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_SYS, "MSG_GET4_SYS");
  hMessageType->GetXaxis()->SetBinLabel(1 + 15, "GET4 Hack 32B");
  hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_NOP,      "NOP");
  fHM->Add(name.Data(), hMessageType);
  if (server) server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hSysMessType";
  title = "Nb of system message for each type; System Type";
  TH1I* hSysMessType = new TH1I(name, title, 17, 0., 17.);
//  TH1I* hSysMessType = new TH1I(name, title, 17, -0.5, 16.5);
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_NX_PARITY,       "NX PARITY");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_SYNC_PARITY,     "SYNC PARITY");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_RESUME,      "DAQ RESUME");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_FIFO_RESET,      "FIFO RESET");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_USER,            "USER");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_PCTIME,          "PCTIME");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_ADC,             "ADC");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_PACKETLOST,      "PACKET LOST");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_GET4_EVENT,      "GET4 ERROR");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_CLOSYSYNC_ERROR, "CLOSYSYNC ERROR");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_TS156_SYNC,        "TS156 SYNC");
  hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_GDPB_UNKWN,        "UNKW GET4 MSG");
  hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
  fHM->Add(name.Data(), hSysMessType);
  if (server) server->Register("/TofRaw", fHM->H1(name.Data()));

  name = "hGet4MessType";
  title = "Nb of message for each type per GET4; GET4 chip # ; Type";
  TH2I* hGet4MessType = new TH2I(name, title, fNrOfGet4, 0., fNrOfGet4, 5 , 0., 5.);
//  TH2I* hGet4MessType = new TH2I(name, title, uNbFeets*feetv1::kuChipPerFeet, -0.5, uNbFeets*feetv1::kuChipPerFeet -0.5, 5, -0.5, 4.5);
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_EPOCH, "EPOCH");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_SLCM,  "S.C. M");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_ERROR, "ERROR");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_DATA,  "DATA 32b");
  hGet4MessType->GetYaxis()->SetBinLabel(1 + ngdpb::GET4_32B_DATA+1,"DATA 24b");
  fHM->Add(name.Data(), hGet4MessType);
  if (server) server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hGet4ChanErrors";
  title = "Error messages per GET4 channel; GET4 channel # ; Error";
  TH2I* hGet4ChanErrors    = new TH2I(name, title, fNrOfGet4*fNrOfChannelsPerGet4 , 0., fNrOfGet4*fNrOfChannelsPerGet4, 32, 0., 32.);
//  TH2I* hGet4ChanErrors    = new TH2I(name, title, uNbFeets*feetv1::kuChanPerFeet*2, -0.5, uNbFeets*feetv1::kuChanPerFeet -0.5, 32, -0.5, 31.5);
  hGet4ChanErrors->GetYaxis()->SetBinLabel(1,  "0x00: Readout Init    ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(2,  "0x01: Sync            ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(3,  "0x02: Epoch count sync");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(4,  "0x03: Epoch           ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(5,  "0x04: FIFO Write      ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(6,  "0x05: Lost event      ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(7,  "0x06: Channel state   ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(8,  "0x07: Token Ring state");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(9,  "0x08: Token           ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
  hGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc."); // <- From GET4 v1.2
  hGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x7f: Unknown         ");
  hGet4ChanErrors->GetYaxis()->SetBinLabel(18, "Corrupt error or unsupported yet");
  fHM->Add(name.Data(), hGet4ChanErrors);
  if (server) server->Register("/TofRaw", fHM->H2(name.Data()));

  name = "hGet4EpochFlags";
  title = "Epoch flags per GET4; GET4 chip # ; Type";
  TH2I* hGet4EpochFlags = new TH2I(name, title, fNrOfGet4, 0., fNrOfGet4, 4 , 0., 4.);
//  hEpochFlags = new TH2I(name, title, uNbFeets*feetv1::kuChipPerFeet, -0.5, uNbFeets*feetv1::kuChipPerFeet -0.5, 4, -0.5, 3.5);
  hGet4EpochFlags->GetYaxis()->SetBinLabel(1,       "SYNC");
  hGet4EpochFlags->GetYaxis()->SetBinLabel(2,       "Ep LOSS");
  hGet4EpochFlags->GetYaxis()->SetBinLabel(3,       "Da LOSS");
  hGet4EpochFlags->GetYaxis()->SetBinLabel(4,       "MISSMAT");
  fHM->Add(name.Data(), hGet4EpochFlags);
  if (server) server->Register("/TofRaw", fHM->H2(name.Data()));

  Double_t w = 10;
  Double_t h = 10;

  TCanvas* c1 = new TCanvas("c1", "Test canvas c1", w, h);
  c1->Divide( 2, 2 );

  c1->cd(1);
  gPad->SetLogy();
  hMessageType->Draw();

  c1->cd(2);
  hSysMessType->Draw();

  c1->cd(3);
  hGet4MessType->Draw("colz");
  gPad->SetLogz();


  c1->cd(4);
  hGet4ChanErrors->Draw("colz");

 /*

  hDataAvail = new TH1I( "hDataAvail",
		  "Data available before readout; # Data Avail [msg]; Checks []",
		  32769, -0.5, 32769 - 0.5);

  hChannelCounts = new TH1I("hChannelCounts",
		  "Hit counts per channel; Channel # []; Counts []",
		  uNbFeets*feetv1::kuChanPerFeet, -0.5, uNbFeets*feetv1::kuChanPerFeet - 0.5);

  hEpochCounts = new TH1I("hEpochCounts",
		  "Epoch counts per channel (chips); Channel # []; Counts []",
		  uNbFeets*feetv1::kuChipPerFeet, -0.5, uNbFeets*feetv1::kuChanPerFeet - 0.5);

  hTotDistribution = new TH2I("hTotDistribution",
		  "Distribution of TOT counts per channel; Channel # []; TOT [bin]",
		  uNbFeets*feetv1::kuChanPerFeet, -0.5, uNbFeets*feetv1::kuChanPerFeet - 0.5,
		  feetv1::kuTotBinsNb, -0.5, feetv1::kuTotBinsNb - 0.5);

  hTsDistribution = new TH2I("hTsDistribution",
		  "Distribution of TS counts per channel; Channel # []; Ts [bin]",
		  uNbFeets*feetv1::kuChanPerFeet, -0.5, uNbFeets*feetv1::kuChanPerFeet - 0.5,
		  feetv1::kuTsBinsNb/16, -0.5, feetv1::kuTsBinsNb - 0.5);

      hEpochDistribution = new TH2I("hEpochDistribution",
            "Distribution of Epochs per chip; Chip # []; Epoch [bin]",
            uNbFeets*feetv1::kuChipPerFeet, -0.5, uNbFeets*feetv1::kuChipPerFeet - 0.5,
            4096, -0.5, 0x7FFFFFFF - 0.5);
 */
  /*
      hChanCoinc = new TH2I("hChanCoinc",
            "Channels coincidences; Channel A # []; Channel B # []",
            uNbFeets*feetv1::kuChanPerFeet, -0.5, uNbFeets*feetv1::kuChanPerFeet - 0.5,
            uNbFeets*feetv1::kuChanPerFeet, -0.5, uNbFeets*feetv1::kuChanPerFeet - 0.5);
   */
/*
       hTimeDiffNext = new TH2I("hTimeDiffNext",
            "Time difference to next channel; First Channel # []; TS(Ch + 1) - TS (Ch) [Bin]",
            uNbFeets*feetv1::kuChanPerFeet - 1, -0.5, uNbFeets*feetv1::kuChanPerFeet - 1.5,
*/
  //         1025, -512.5, +512.5);
   //         10250, -5120.5, +5120.5);
   //         10251, -10250.5, +10250.5);

  if( fUnpackPar->IsChannelRateEnabled() ) {
	for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb ++)
	{
	  const Int_t iNbBinsRate = 82;
	  Double_t dBinsRate[iNbBinsRate] =
	        { 1e0, 2e0, 3e0, 4e0, 5e0, 6e0, 7e0, 8e0, 9e0,
			  1e1, 2e1, 3e1, 4e1, 5e1, 6e1, 7e1, 8e1, 9e1,
			  1e2, 2e2, 3e2, 4e2, 5e2, 6e2, 7e2, 8e2, 9e2,
			  1e3, 2e3, 3e3, 4e3, 5e3, 6e3, 7e3, 8e3, 9e3,
			  1e4, 2e4, 3e4, 4e4, 5e4, 6e4, 7e4, 8e4, 9e4,
			  1e5, 2e5, 3e5, 4e5, 5e5, 6e5, 7e5, 8e5, 9e5,
			  1e6, 2e6, 3e6, 4e6, 5e6, 6e6, 7e6, 8e6, 9e6,
			  1e7, 2e7, 3e7, 4e7, 5e7, 6e7, 7e7, 8e7, 9e7,
			  1e8, 2e8, 3e8, 4e8, 5e8, 6e8, 7e8, 8e8, 9e8,
			  1e9 };
	  name = Form("ChannelRate_gDPB_%02u", uGdpb);
	  title = Form("Channel instant rate gDPB %02u; Dist[ns] ; Channel", uGdpb);
	  fHM->Add(name.Data(), new TH2F(name.Data(), title.Data(),
			   iNbBinsRate-1, dBinsRate, 96, 0, 96) );
	  if (server) server->Register("/TofRaw", fHM->H2(name.Data()));
	  LOG(info) << "Adding the rate histos";
	} // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  }

  for( UInt_t uGdpb = 0; uGdpb < fNrOfGdpbs; uGdpb ++)
  {
    name = Form("Raw_Tot_gDPB_%02u", uGdpb);
    title = Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb);
    fHM->Add(name.Data(), new TH2F(name.Data(), title.Data(), 96, 0, 96, 256, 0, 256) );
    if (server) server->Register("/TofRaw", fHM->H2(name.Data()));

    name = Form("ChCount_gDPB_%02u", uGdpb);
    title = Form("Channel counts gDPB %02u; channel; Hits", uGdpb);
    fHM->Add(name.Data(), new TH1I(name.Data(), title.Data(), 96, 0, 96 ) );

    if (server) server->Register("/TofRaw", fHM->H1(name.Data()));

    for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
    {
       name = Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
       title = Form("Counts per second in Feet %1u of gDPB %02u; Time[s] ; Counts", uFeet, uGdpb);
       fHM->Add(name.Data(), new TH1F( name.Data(), title.Data(),
                                       1800, 0, 1800 ) );
      if (server) server->Register("/TofRaw", fHM->H1(name.Data()));
    } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
  } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)

      if (server) server->RegisterCommand( "/Reset_ChCount_gDPB_00", "/TofRaw/ChCount_gDPB_00/->Reset()" );
      if (server) server->Restrict("/Reset_ChCount_gDPB_00","allow=admin");
      if (server) server->RegisterCommand( "/Reset_All_TOF", "ResetAllHistos()" );
      if (server) server->Restrict("/Reset_All_TOF","allow=admin");


   /** Create summary Canvases for CERN 2016 **/
   name = "stackRate_g00";
   THStack * stackRateA = new THStack( name, "Sum of counts vs Time per FEET for gDPB 1");
   fHM->H1( "FeetRate_gDPB_g00_f0" )->SetLineColor( kBlack ); // => Make stack + color!
   stackRateA->Add( fHM->H1( "FeetRate_gDPB_g00_f0" ) );
   fHM->H1( "FeetRate_gDPB_g00_f1" )->SetLineColor( kRed ); // => Make stack + color!
   stackRateA->Add( fHM->H1( "FeetRate_gDPB_g00_f1" ) );
   fHM->H1( "FeetRate_gDPB_g00_f2" )->SetLineColor( kBlue ); // => Make stack + color!
   stackRateA->Add( fHM->H1( "FeetRate_gDPB_g00_f2" ) );
  fHM->Add(name.Data(), stackRateA);
   if (server) server->Register("/TofRaw", stackRateA);

   name = "stackRate_g01";
   THStack * stackRateB = new THStack( name, "Sum of counts vs Time per FEET for gDPB 2");
   fHM->H1( "FeetRate_gDPB_g01_f0" )->SetLineColor( kBlack ); // => Make stack + color!
   stackRateB->Add( fHM->H1( "FeetRate_gDPB_g01_f0" ) );
   fHM->H1( "FeetRate_gDPB_g01_f1" )->SetLineColor( kRed ); // => Make stack + color!
   stackRateB->Add( fHM->H1( "FeetRate_gDPB_g01_f1" ) );
   fHM->H1( "FeetRate_gDPB_g01_f2" )->SetLineColor( kBlue ); // => Make stack + color!
   stackRateB->Add( fHM->H1( "FeetRate_gDPB_g01_f2" ) );
  fHM->Add(name.Data(), stackRateB);
   if (server) server->Register("/TofRaw", stackRateB);

  TCanvas* cSummary = new TCanvas("cSummary", "gDPB Monitoring Summary", w, h);
  cSummary->Divide( 4, 3 );

   // 1st Column: Messages types
  cSummary->cd(1);
  gPad->SetLogy();
  hMessageType->Draw();

  cSummary->cd(5);
  gPad->SetLogy();
  hSysMessType->Draw();

  cSummary->cd(9);
  gPad->SetLogz();
  hGet4MessType->Draw("colz");

   // 2nd Column: GET4 Errors + Epoch flags +
  cSummary->cd(2);
  gPad->SetLogz();
  hGet4ChanErrors->Draw("colz");

  cSummary->cd(6);
  gPad->SetLogz();
  hGet4EpochFlags->Draw("colz");

  cSummary->cd(10);
//  gPad->SetLogz();
//  hGet4MessType->Draw("colz");

  cSummary->cd(4);
  hGet4ChanErrors->Draw("colz");

   // 3rd & 4th Column: Sum of counts vs Time per FEET
  cSummary->cd(3);
  gPad->SetLogy();
   stackRateA->Draw("nostack");

  cSummary->cd(4);
  gPad->SetLogy();
   stackRateB->Draw("nostack");

  cSummary->cd(7);
  gPad->SetLogy();
   name = "stackRate_g02";
   THStack * stackRateC = new THStack( name, "Sum of counts vs Time per FEET for gDPB 3");
   fHM->H1( "FeetRate_gDPB_g02_f0" )->SetLineColor( kBlack );// => Make stack + color!
   stackRateC->Add( fHM->H1( "FeetRate_gDPB_g02_f0" ) );
   fHM->H1( "FeetRate_gDPB_g02_f1" )->SetLineColor( kRed );  // => Make stack + color!
   stackRateC->Add( fHM->H1( "FeetRate_gDPB_g02_f1" ) );
   fHM->H1( "FeetRate_gDPB_g02_f2" )->SetLineColor( kBlue ); // => Make stack + color!
   stackRateC->Add( fHM->H1( "FeetRate_gDPB_g02_f2" ) );
   stackRateC->Draw("nostack");
  fHM->Add(name.Data(), stackRateC);
   if (server) server->Register("/TofRaw", stackRateC);

  cSummary->cd(8);
  gPad->SetLogy();
   name = "stackRate_g03";
   THStack * stackRateD = new THStack( name, "Sum of counts vs Time per FEET for gDPB 4");
   fHM->H1( "FeetRate_gDPB_g03_f0" )->SetLineColor( kBlack );// => Make stack + color!
   stackRateD->Add( fHM->H1( "FeetRate_gDPB_g03_f0" ) );
   fHM->H1( "FeetRate_gDPB_g03_f1" )->SetLineColor( kRed );  // => Make stack + color!
   stackRateD->Add( fHM->H1( "FeetRate_gDPB_g03_f1" ) );
   fHM->H1( "FeetRate_gDPB_g03_f2" )->SetLineColor( kBlue ); // => Make stack + color!
   stackRateD->Add( fHM->H1( "FeetRate_gDPB_g03_f2" ) );
   stackRateD->Draw("nostack");
  fHM->Add(name.Data(), stackRateD);
   if (server) server->Register("/TofRaw", stackRateD);

  cSummary->cd(11);
  gPad->SetLogy();
   name = "stackRate_g04";
   THStack * stackRateE = new THStack( name, "Sum of counts vs Time per FEET for gDPB 5");
   fHM->H1( "FeetRate_gDPB_g04_f0" )->SetLineColor( kBlack ); // => Make stack + color!
   stackRateE->Add( fHM->H1( "FeetRate_gDPB_g04_f0" ) );
   fHM->H1( "FeetRate_gDPB_g04_f1" )->SetLineColor( kRed ); // => Make stack + color!
   stackRateE->Add( fHM->H1( "FeetRate_gDPB_g04_f1" ) );
   fHM->H1( "FeetRate_gDPB_g04_f2" )->SetLineColor( kBlue ); // => Make stack + color!
   stackRateE->Add( fHM->H1( "FeetRate_gDPB_g04_f2" ) );
   stackRateE->Draw("nostack");
  fHM->Add(name.Data(), stackRateE);
   if (server) server->Register("/TofRaw", stackRateE);

  cSummary->cd(12);
  gPad->SetLogy();
   name = "stackRate_g05";
   THStack * stackRateF = new THStack( name, "Sum of counts vs Time per FEET for gDPB 6");
   fHM->H1( "FeetRate_gDPB_g05_f0" )->SetLineColor( kBlack ); // => Make stack + color!
   stackRateF->Add( fHM->H1( "FeetRate_gDPB_g05_f0" ) );
   fHM->H1( "FeetRate_gDPB_g05_f1" )->SetLineColor( kRed ); // => Make stack + color!
   stackRateF->Add( fHM->H1( "FeetRate_gDPB_g05_f1" ) );
   fHM->H1( "FeetRate_gDPB_g05_f2" )->SetLineColor( kBlue ); // => Make stack + color!
   stackRateF->Add( fHM->H1( "FeetRate_gDPB_g05_f2" ) );
   stackRateF->Draw("nostack");
  fHM->Add(name.Data(), stackRateF);
   if (server) server->Register("/TofRaw", stackRateF);

  name = "hDiamond";
  title = "Counts per diamond in last 10s; X [pad]; Y [pad]; Counts";
  TH2I* hDiamond = new TH2I(name, title, 2, 0., 2, 2 , 0., 2.);
  fHM->Add(name.Data(), hDiamond);
  if (server) server->Register("/TofRaw", fHM->H2(name.Data()));

   /*****************************/

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmTSMonitorTofLegacy::DoUnpack(const fles::Timeslice& ts, size_t component)
{
  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
               << "microslices.";


   // Getting the pointer to the correct histogram needs a lot more time then
   // the actual filling procedure. If one gets the pointer in a loop this kills
   // the performance. A test shows that extracting the pointer from the CbmHistManger
   // is slower by a factor of 20-100 (depending on the number of histos managed by the
   // CbmHistManager) compared to using the pointer directly
   // So get the pointer once outside the loop and use it in the loop

   TH1* histMessType = fHM->H1("hMessageType");
   TH1* histSysMessType = fHM->H1("hSysMessType");
   TH2* histGet4MessType = fHM->H2("hGet4MessType");
   TH2* histGet4ChanErrors = fHM->H2("hGet4ChanErrors");
   TH2* histGet4EpochFlags = fHM->H2("hGet4EpochFlags");

   std::vector<TH2*> Raw_Tot_gDPB;
   std::vector<TH1*> ChCount_gDPB;
   std::vector<TH2*> ChannelRate_gDPB;
   std::vector<TH1*> FeetRate_gDPB;

   for(UInt_t i=0; i<fNrOfGdpbs; ++i) {
     TString name = Form("Raw_Tot_gDPB_%02u", i);
     Raw_Tot_gDPB.push_back(fHM->H2(name.Data()));
     name = Form("ChCount_gDPB_%02u", i);
     ChCount_gDPB.push_back(fHM->H1(name.Data()));
  	  name = Form("ChannelRate_gDPB_%02u", i);
     if( fUnpackPar->IsChannelRateEnabled() ) {
       ChannelRate_gDPB.push_back(fHM->H2(name.Data()));
     }
     for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
     {
       name = Form("FeetRate_gDPB_g%02u_f%1u", i, uFeet);
       FeetRate_gDPB.push_back(fHM->H1(name.Data()));
     } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
   }
   TH2* histDiamond = fHM->H2("hDiamond");


   TString sMsSzName = Form("MsSz_link_%02lu", component);
   TH1* hMsSz = NULL;
   TProfile* hMsSzTime = NULL;
   if( fHM->Exists(sMsSzName.Data() ) )
   {
      hMsSz = fHM->H1(sMsSzName.Data());
      sMsSzName = Form("MsSzTime_link_%02lu", component);
      hMsSzTime = fHM->P1(sMsSzName.Data());
   } // if( fHM->Exists(sMsSzName.Data() ) )
      else
      {
         TString sMsSzTitle = Form("Size of MS for gDPB of link %02lu; Ms Size [bytes]", component);
         fHM->Add(sMsSzName.Data(), new TH1F( sMsSzName.Data(), sMsSzTitle.Data(),
                                       160000, 0., 20000. ) );
         hMsSz = fHM->H1(sMsSzName.Data());
         if (server) server->Register("/FlibRaw", hMsSz );
         sMsSzName = Form("MsSzTime_link_%02lu", component);
         sMsSzTitle = Form("Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]", component);
         fHM->Add(sMsSzName.Data(), new TProfile( sMsSzName.Data(), sMsSzTitle.Data(),
                                       15000, 0., 300. ) );
         hMsSzTime = fHM->P1(sMsSzName.Data());
         if (server) server->Register("/FlibRaw", hMsSzTime );
         LOG(info) << "Added MS size histo for component: " << component
                << " (gDPB)";
      } // else of if( fHM->Exists(sMsSzName.Data() ) )

   Int_t messageType = -111;
   // Loop over microslices
   for (size_t m = 0; m < ts.num_microslices(component); ++m)
   {
      if( fuMsAcceptsPercent < m )
      continue;

      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, m);
      fEquipmentId = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(component, m));

      uint32_t size = msDescriptor.size;
      if(size>0)
      LOG(debug) << "Microslice: " << msDescriptor.idx
      << " has size: " << size;

      hMsSz->Fill( size );
      hMsSzTime->Fill( (1e-9) * static_cast<double>( msDescriptor.idx) , size);
//      LOG(info) << "Test: " << ((1e-9) * static_cast<double>( msDescriptor.idx))
//      << " s for size: " << size;

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
      LOG(error) << "The input microslice buffer does NOT "
      << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage) )
      / kuBytesPerMessage;

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
      {
         // Fill message
         uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
         ngdpb::Message mess( ulData );

         if(gLogger->IsLogNeeded( fair::Severity::debug )) {
            mess.printDataCout();
         }

         // Increment counter for different message types
         // and fill the corresponding histogram
         messageType = mess.getMessageType();
         fMsgCounter[messageType]++;
         histMessType->Fill(messageType);

         Int_t rocId      = fGdpbIdIndexMap[mess.getRocNumber()];
         Int_t get4Id     = mess.getGdpbGenChipId();
         Int_t get4Nr    = (rocId*fNrOfGet4PerGdpb) + get4Id;

         switch( messageType )
         {
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
               histSysMessType->Fill(mess.getSysMesType());
               break;
            case ngdpb::MSG_EPOCH2:
           	   histGet4MessType->Fill(get4Nr, ngdpb::GET4_32B_EPOCH);
               FillEpochInfo(mess, histGet4EpochFlags);
               break;
            case ngdpb::MSG_GET4:
           	   histGet4MessType->Fill(get4Nr, ngdpb::GET4_32B_DATA + 1);
               PrintGenInfo(mess);
               break;
            case ngdpb::MSG_GET4_32B:
               histGet4MessType->Fill(get4Nr, ngdpb::GET4_32B_DATA);
               FillHitInfo(mess, Raw_Tot_gDPB, ChCount_gDPB, ChannelRate_gDPB, FeetRate_gDPB, histDiamond);
               break;
            case ngdpb::MSG_GET4_SLC:
           	   histGet4MessType->Fill(get4Nr, ngdpb::GET4_32B_SLCM);
               PrintSlcInfo(mess);
               break;
            case ngdpb::MSG_GET4_SYS:
               {
                 histSysMessType->Fill( mess.getGdpbSysSubType() );
                 if( ngdpb::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType() ) {
                   histGet4MessType->Fill(get4Nr, ngdpb::GET4_32B_ERROR);
                   Int_t dFullChId  = (rocId*fNrOfGet4PerGdpb
                				      + get4Id)*fNrOfChannelsPerGet4
									  + mess.getGdpbHitChanId();
                   switch( mess.getGdpbSysErrData() )
                   {
                      case ngdpb::GET4_V1X_ERR_READ_INIT:
                         histGet4ChanErrors->Fill( dFullChId, 0);
                         break;
                      case ngdpb::GET4_V1X_ERR_SYNC:
                         histGet4ChanErrors->Fill( dFullChId, 1);
                         break;
                      case ngdpb::GET4_V1X_ERR_EP_CNT_SYNC:
                         histGet4ChanErrors->Fill( dFullChId, 2);
                         break;
                      case ngdpb::GET4_V1X_ERR_EP:
                         histGet4ChanErrors->Fill( dFullChId, 3);
                         break;
                      case ngdpb::GET4_V1X_ERR_FIFO_WRITE:
                         histGet4ChanErrors->Fill( dFullChId, 4);
                         break;
                      case ngdpb::GET4_V1X_ERR_LOST_EVT:
                         histGet4ChanErrors->Fill( dFullChId, 5);
                         break;
                      case ngdpb::GET4_V1X_ERR_CHAN_STATE:
                         histGet4ChanErrors->Fill( dFullChId, 6);
                         break;
                      case ngdpb::GET4_V1X_ERR_TOK_RING_ST:
                         histGet4ChanErrors->Fill( dFullChId, 7);
                         break;
                      case ngdpb::GET4_V1X_ERR_TOKEN:
                         histGet4ChanErrors->Fill( dFullChId, 8);
                         break;
                      case ngdpb::GET4_V1X_ERR_READOUT_ERR:
                         histGet4ChanErrors->Fill( dFullChId, 9);
                         break;
                      case ngdpb::GET4_V1X_ERR_SPI:
                         histGet4ChanErrors->Fill( dFullChId, 10);
                         break;
                      case ngdpb::GET4_V1X_ERR_DLL_LOCK:
                         histGet4ChanErrors->Fill( dFullChId, 11);
                         break;
                      case ngdpb::GET4_V1X_ERR_DLL_RESET:
                         histGet4ChanErrors->Fill( dFullChId, 12);
                         break;
                      case ngdpb::GET4_V1X_ERR_TOT_OVERWRT:
                         histGet4ChanErrors->Fill( dFullChId, 13);
                         break;
                      case ngdpb::GET4_V1X_ERR_TOT_RANGE:
                         histGet4ChanErrors->Fill( dFullChId, 14);
                         break;
                      case ngdpb::GET4_V1X_ERR_EVT_DISCARD:
                         histGet4ChanErrors->Fill( dFullChId, 15);
                         break;
                      case ngdpb::GET4_V1X_ERR_UNKNOWN:
                         histGet4ChanErrors->Fill( dFullChId, 16);
                         break;
                      default: // Corrupt error or not yet supported error
                         histGet4ChanErrors->Fill( dFullChId, 17);
                         break;
                   } // Switch( mess.getGdpbSysErrData() )

                 }
                 if(100 > iMess++)
                 PrintSysInfo(mess);
                 break;
               }
               default:
               if(100 > iMess++)
                  LOG(error) << "Message ("<<iMess<<") type " << std::hex << std::setw(2)
                             << static_cast< uint16_t >( messageType )
                             << " not yet included in Get4 unpacker.";
               if(100 == iMess)
                  LOG(error) << "Stop reporting MSG errors... ";
         } // switch( mess.getMessageType() )
      } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
   } // for (size_t m = 0; m < ts.num_microslices(component); ++m)

   return kTRUE;
}

void CbmTSMonitorTofLegacy::FillHitInfo(ngdpb::Message mess,
                                  std::vector<TH2*> Raw_Tot_gDPB,
                                  std::vector<TH1*> ChCount_gDPB,
                                  std::vector<TH2*> ChannelRate_gDPB,
                                  std::vector<TH1*> FeetRate_gDPB,
                                  TH2* histDiamond
)
   {
   // --- Get absolute time, NXYTER and channel number
   Int_t rocId      = mess.getRocNumber();
   Int_t get4Id     = mess.getGdpbGenChipId();
   Int_t channel    = mess.getGdpbHitChanId();
   Int_t tot        = mess.getGdpbHit32Tot();
   ULong_t hitTime  = mess.getMsgFullTime( 0 );

   if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
   {
      // Check if at least one epoch before in this gDPB
      if( fCurrentEpoch.end() != fCurrentEpoch.find( rocId ) )
      {
         UInt_t gdpbNr = fGdpbIdIndexMap[ rocId ];
         Raw_Tot_gDPB[gdpbNr]->Fill( get4Id*fNrOfChannelsPerGet4 + channel, tot);
         ChCount_gDPB[gdpbNr]->Fill( get4Id*fNrOfChannelsPerGet4 + channel );

         // Check if at least one epoch before in this GET4
         if( fCurrentEpoch[rocId].end() != fCurrentEpoch[rocId].find( get4Id ) )
         {
            if( fUnpackPar->IsChannelRateEnabled() )
            {
               // Check if at least one hit before in this gDPB
               if( fTsLastHit.end() != fTsLastHit.find( rocId ) )
               {
                  // Check if at least one hit before in this Get4
                  if( fTsLastHit[rocId].end() != fTsLastHit[rocId].find( get4Id ) )
                  {
                     // Check if at least one hit before in this channel
                     if( fTsLastHit[rocId][get4Id].end() != fTsLastHit[rocId][get4Id].find( channel ) )
                     {
                        ChannelRate_gDPB[gdpbNr]->Fill( ( mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] )
                                                        - fTsLastHit[rocId][get4Id][channel] ),
                                                     get4Id*fNrOfChannelsPerGet4 + channel);
                     } // if( fTsLastHit[rocId][get4Id].end() != fTsLastHit[rocId][get4Id].find( channel ) )
                  } // if( fTsLastHit[rocId].end() != fTsLastHit[rocId].find( get4Id ) )
               } // if( fTsLastHit.end() != fTsLastHit.find( rocId ) )

               fTsLastHit[rocId][get4Id][channel] = mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] );
            } // if( fUnpackPar->IsChannelRateEnabled() )

            if( fdStartTime < 0 )
               fdStartTime = mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] );

            if( 0 < fdStartTime )
               FeetRate_gDPB[ (gdpbNr*fNrOfFebsPerGdpb) + (get4Id / fNrOfGet4PerFeb) ]->Fill(
                           1e-9*( mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] )
                                 - fdStartTime) );

            if( fDiamondGdpb == gdpbNr && ( get4Id/fNrOfGet4PerFeb == fDiamondFeet ) )
            {
               UInt_t uChanInGdpb = get4Id * fNrOfChannelsPerGet4 + channel;
               if( fDiamondChanA == uChanInGdpb )
                  histDiamond->Fill( 0., 0.);
               else if( fDiamondChanB == uChanInGdpb )
                  histDiamond->Fill( 1., 0.);
               else if( fDiamondChanC == uChanInGdpb )
                  histDiamond->Fill( 0., 1.);
               else if( fDiamondChanD == uChanInGdpb )
                  histDiamond->Fill( 1., 1.);

               if( 10 < ( (mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] )/1e9) - fDiamondTimeLastReset ) )
               {
                  histDiamond->Reset();
                  fDiamondTimeLastReset = mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] )/1e9;
               } // if( 1e10 < ( mess.getMsgFullTimeD( fCurrentEpoch[rocId][get4Id] ) - fDiamondTimeLastReset )

            } // if( fDiamondGdpb == gdpbNr && ( get4Id/fNrOfGet4PerFeb == fDiamondFeet ) )

            hitTime  = mess.getMsgFullTime(fCurrentEpoch[rocId][get4Id]);
            Int_t Ft = mess.getGdpbHitFineTs();

            if(100 > iMess++)
               LOG(debug) << "Hit: " << Form("0x%08x ",rocId) << ", " << get4Id
                          << ", " << channel << ", " << tot
                          << ", epoch " << fCurrentEpoch[rocId][get4Id]
                          << ", FullTime " << hitTime
                          << ", FineTime " << Ft;
         } // if( fCurrentEpoch[rocId].end() != fCurrentEpoch[rocId].find( get4Id ) )
      } // if( fCurrentEpoch.end() != fCurrentEpoch.find( rocId ) )
   } // if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
   else LOG(warning) << "found unmapped rocId w/o epoch yet: " << Form("0x%08x ",rocId);
}

void CbmTSMonitorTofLegacy::FillEpochInfo(ngdpb::Message mess, TH2* EpochFlags)
{
   Int_t rocId  = mess.getRocNumber();
   Int_t get4Id = mess.getGdpbGenChipId();

   Int_t gdpbIdx = fGdpbIdIndexMap[rocId];
   Int_t get4Nr  = (gdpbIdx*fNrOfGet4PerGdpb) + get4Id;

   fCurrentEpoch[rocId][get4Id] = mess.getEpoch2Number();

   //  LOG(info) << "Epoch message for ROC " << rocId << " with epoch number "
   //            << fCurrentEpoch[rocId];

   if( 1 == mess.getGdpbEpSync() )
      EpochFlags->Fill( get4Nr, 0 );
   if( 1 == mess.getGdpbEpDataLoss() )
      EpochFlags->Fill( get4Nr, 1 );
   if( 1 == mess.getGdpbEpEpochLoss() )
      EpochFlags->Fill( get4Nr, 2 );
   if( 1 == mess.getGdpbEpMissmatch() )
      EpochFlags->Fill( get4Nr, 3 );

/*
   if( 0 == gdpbIdx && 0 == mess.getEpoch2Number() % 20000 ) // Try to force update every 1s
   {
      dynamic_cast< TCanvas * >(gROOT->FindObjectAny( "cSummary" ) )->cd(3);
      gPad->Modified();
      gPad->Update();
   }
*/

   fCurrentEpochTime = mess.getMsgFullTime(fCurrentEpoch[rocId][get4Id]);
   fNofEpochs++;
/*
    LOG(debug) << "Epoch message "
              << fNofEpochs << ", epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
              << ", time " << std::setprecision(9) << std::fixed
              << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
              << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
              << " and chip " << mess.getEpoch2ChipNumber();
*/
}

void CbmTSMonitorTofLegacy::PrintSlcInfo(ngdpb::Message mess)
{
   Int_t rocId      = mess.getRocNumber();
   Int_t get4Id     = mess.getGdpbGenChipId();

   if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
      LOG(info) << "GET4 Slow Control message, epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
                << "\n"
                << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                << ", Chan = " << std::setw(1) << mess.getGdpbSlcChan()
                << ", Edge = " << std::setw(1) << mess.getGdpbSlcEdge()
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType()
                << ", Data = " << std::hex << std::setw(6) << mess.getGdpbSlcData() << std::dec
                << ", Type = " << mess.getGdpbSlcCrc();

}

void CbmTSMonitorTofLegacy::PrintGenInfo(ngdpb::Message mess)
{
   Int_t mType    = mess.getMessageType();
   Int_t rocId    = mess.getRocNumber();
   Int_t get4Id   = mess.getGdpbGenChipId();
   Int_t channel  = mess.getGdpbHitChanId();
   uint64_t uData = mess.getData();

   if(100 > iMess++)
   LOG(info) << "Get4 MSG type "<<mType<<" from rocId "<<rocId<<", getId "<<get4Id
             << ", (hit channel) "<< channel << " data " << std::hex
             << uData;
}

void CbmTSMonitorTofLegacy::PrintSysInfo(ngdpb::Message mess)
{
   Int_t rocId      = mess.getRocNumber();
   Int_t get4Id     = mess.getGdpbGenChipId();

   if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
      LOG(debug) << "GET4 System message,       epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec;

   switch( mess.getGdpbSysSubType() )
   {
      case ngdpb::SYSMSG_GET4_EVENT:
      {
         uint32_t uData = mess.getGdpbSysErrData();
         if( ngdpb::GET4_V1X_ERR_TOT_OVERWRT == uData ||
             ngdpb::GET4_V1X_ERR_TOT_RANGE   == uData ||
             ngdpb::GET4_V1X_ERR_EVT_DISCARD == uData )
            LOG(debug) << " +++++++ > gDPB: "
                      << std::hex << std::setw(4) << rocId << std::dec
                      << ", Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                      << ", Chan = " << std::setw(1) << mess.getGdpbSysErrChanId()
                      << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                      << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused()
                      << ", Data = " << std::hex << std::setw(2) << uData << std::dec
                      << " -- GET4 V1 Error Event";
            else LOG(info) << " +++++++ >gDPB: "
                   << std::hex << std::setw(4) << rocId << std::dec
                   << ", Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                   << ", Chan = " << std::setw(1) << mess.getGdpbSysErrChanId()
                   << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                   << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused()
                   << ", Data = " << std::hex << std::setw(2) << uData << std::dec
                   << " -- GET4 V1 Error Event";
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

void CbmTSMonitorTofLegacy::Reset()
{
}

void CbmTSMonitorTofLegacy::Finish()
{
   TString message_type;

   for (unsigned int i=0; i< fMsgCounter.size(); ++i)
   {
      switch(i)
      {
         case 0: message_type ="NOP"; break;
         case 1: message_type ="HIT"; break;
         case 2: message_type ="EPOCH"; break;
         case 3: message_type ="SYNC"; break;
         case 4: message_type ="AUX"; break;
         case 5: message_type ="EPOCH2"; break;
         case 6: message_type ="GET4"; break;
         case 7: message_type ="SYS"; break;
         case 8: message_type ="GET4_SLC"; break;
         case 9: message_type ="GET4_32B"; break;
         case 10: message_type ="GET4_SYS"; break;
         default:  message_type ="UNKNOWN"; break;
      } // switch(i)
      LOG(info) << message_type << " messages: "
                << fMsgCounter[i];
   } // for (unsigned int i=0; i< fMsgCounter.size(); ++i)

   LOG(info) << "-------------------------------------";
   for( auto it = fCurrentEpoch.begin(); it != fCurrentEpoch.end(); ++it)
      for( auto itG = (it->second).begin(); itG != (it->second).end(); ++itG)
      LOG(info) << "Last epoch for gDPB: "
                << std::hex << std::setw(4) << it->first << std::dec
                << " , GET4  " << std::setw(4) << itG->first
                << " => " << itG->second;
   LOG(info) << "-------------------------------------";


   gDirectory->mkdir("Tof_Raw_gDPB");
   gDirectory->cd("Tof_Raw_gDPB");
   for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   {
      fHM->H2( Form("Raw_Tot_gDPB_%02u", uGdpb) )->Write();
      fHM->H1( Form("ChCount_gDPB_%02u", uGdpb) )->Write();
      if( fUnpackPar->IsChannelRateEnabled() )
         fHM->H2( Form("ChannelRate_gDPB_%02u", uGdpb) )->Write();
      for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
      {
        fHM->H1( Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet) )->Write();
      } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
   } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   gDirectory->cd("..");

   gDirectory->mkdir("Flib_Raw");
   gDirectory->cd("Flib_Raw");
   for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)
   {
      TString sMsSzName = Form("MsSz_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->H1( sMsSzName.Data() )->Write();

      sMsSzName = Form("MsSzTime_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->P1( sMsSzName.Data() )->Write();
   } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)
   gDirectory->cd("..");

}


void CbmTSMonitorTofLegacy::FillOutput(boost::any)
{
}



void CbmTSMonitorTofLegacy::SetDiamondChannels( UInt_t uGdpb, UInt_t uFeet, UInt_t uChannelA,
                            UInt_t uChannelB, UInt_t uChannelC, UInt_t uChannelD)
{
   fDiamondGdpb  = uGdpb;
   fDiamondFeet  = uFeet;
   fDiamondChanA = uChannelA;
   fDiamondChanB = uChannelB;
   fDiamondChanC = uChannelC;
   fDiamondChanD = uChannelD;
}

void CbmTSMonitorTofLegacy::ResetAllHistos()
{
   fHM->H1( "hMessageType" )->Reset();
   fHM->H1( "hSysMessType" )->Reset();
   fHM->H2( "hGet4MessType" )->Reset();
   fHM->H2( "hGet4ChanErrors" )->Reset();
   fHM->H2( "hGet4EpochFlags")->Reset();

   for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
   {
      fHM->H2( Form("Raw_Tot_gDPB_%02u", uGdpb) )->Reset();
      fHM->H1( Form("ChCount_gDPB_%02u", uGdpb) )->Reset();
      if( fUnpackPar->IsChannelRateEnabled() )
         fHM->H2( Form("ChannelRate_gDPB_%02u", uGdpb) )->Reset();
      for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
      {
        fHM->H1( Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet) )->Reset();
      } // for( UInt_t uFeet = 0; uFeet < fNrOfFebsPerGdpb; uFeet ++)
   } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)

   for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)
   {
      TString sMsSzName = Form("MsSz_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->H1( sMsSzName.Data() )->Reset();

      sMsSzName = Form("MsSzTime_link_%02u", uLinks);
      if( fHM->Exists(sMsSzName.Data() ) )
         fHM->P1( sMsSzName.Data() )->Reset();
   } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)
}

ClassImp(CbmTSMonitorTofLegacy)
