// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmTofStarMonitorShift2018                     -----
// -----               Created 19.01.2018 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTofStarMonitorShift2018.h"
#include "CbmTofStar2018Par.h"

//#include "CbmFlibCern2016Source.h"
#include "CbmHistManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"

#include "TROOT.h"
#include "TClonesArray.h"
#include "TString.h"
#include "THttpServer.h"
#include "Rtypes.h"
#include "TProfile.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TMath.h"
#include <TFile.h>

#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <ctime>

Bool_t bResetTofStarMoniShiftHistos = kFALSE;
Bool_t bSaveTofStarMoniShiftHistos  = kFALSE;
Bool_t bTofUpdateZoomedFitMoniShift = kFALSE;

CbmTofStarMonitorShift2018::CbmTofStarMonitorShift2018() :
    CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuTotalMsNb(0),
    fuOverlapMsNb(0),
    fuCoreMs(0),
    fdMsSizeInNs(0.0),
    fdTsCoreSizeInNs(0.0),
    fuMinNbGdpb( 0 ),
    fuCurrNbGdpb( 0 ),
    fUnpackPar(),
    fuNrOfGdpbs(0),
    fuNrOfFeetPerGdpb(0),
    fuNrOfGet4PerFeb(0),
    fuNrOfChannelsPerGet4(0),
    fuNrOfChannelsPerFeet(0),
    fuNrOfGet4(0),
    fuNrOfGet4PerGdpb(0),
    fuNrOfChannelsPerGdpb(0),
    fulCurrentTsIndex(0),
    fuCurrentMs(0),
    fdMsIndex(0),
    fuGdpbId(0),
    fuGdpbNr(0),
    fuGet4Id(0),
    fuGet4Nr(0),
    fiEquipmentId(0),
    fviMsgCounter(11, 0), // length of enum MessageTypes initialized with 0
    fvulGdpbTsMsb(),
    fvulGdpbTsLsb(),
    fvulStarTsMsb(),
    fvulStarTsMid(),
    fvulGdpbTsFullLast(),
    fvulStarTsFullLast(),
    fvuStarTokenLast(),
    fvuStarDaqCmdLast(),
    fvuStarTrigCmdLast(),
    fvulCurrentEpoch(),
    fvbFirstEpochSeen(),
    fulCurrentEpochTime(0),
    fGdpbIdIndexMap(),
    fvmEpSupprBuffer(),
    fdTsLastPulserHit(),
    dMinDt(-1.*(kuNbBinsDt*get4v2x::kdBinSize/2.) - get4v2x::kdBinSize/2.),
    dMaxDt(1.*(kuNbBinsDt*get4v2x::kdBinSize/2.) + get4v2x::kdBinSize/2.),
    fuNbFeetPlot(2),
    fuNbFeetPlotsPerGdpb(0),
    fdStartTime(-1.),
    fdStartTimeLong(-1.),
    fdStartTimeMsSz(-1.),
    fuHistoryHistoSize( 1800 ),
    fuHistoryHistoSizeLong( 600 ),
    fdLastRmsUpdateTime(0.0),
    fdFitZoomWidthPs(0.0),
    fcMsSizeAll(NULL),
    fvhMsSzPerLink(12, NULL),
    fvhMsSzTimePerLink(12, NULL),
    fhMessType(NULL),
    fhSysMessType(NULL),
    fhGet4MessType(NULL),
    fhGet4ChanScm(NULL),
    fhGet4ChanErrors(NULL),
    fhGet4EpochFlags(NULL),
    fhScmScalerCounters(NULL),
    fhScmDeadtimeCounters(NULL),
    fhScmSeuCounters(NULL),
    fhScmSeuCountersEvo(NULL),
    fvhRawFt_gDPB(),
    fvhRawTot_gDPB(),
    fvhChCount_gDPB(),
    fvhChannelRate_gDPB(),
    fvhRemapTot_gDPB(),
    fvhRemapChCount_gDPB(),
    fvhRemapChRate_gDPB(),
    fvhFeetRate_gDPB(),
    fvhFeetErrorRate_gDPB(),
    fvhFeetErrorRatio_gDPB(),
    fvhFeetRateLong_gDPB(),
    fvhFeetErrorRateLong_gDPB(),
    fvhFeetErrorRatioLong_gDPB(),
    fvhTokenMsgType(),
    fvhTriggerRate(),
    fvhCmdDaqVsTrig(),
    fvhStarTokenEvo(),
    fvhStarTrigGdpbTsEvo(),
    fvhStarTrigStarTsEvo(),
    fvhTimeDiffPulser(),
    fhTimeRmsPulser(NULL),
    fhTimeRmsZoomFitPuls(NULL),
    fhTimeResFitPuls(NULL),
    fvuPadiToGet4(),
    fvuGet4ToPadi(),
    fTimeLastHistoSaving()
{
}

CbmTofStarMonitorShift2018::~CbmTofStarMonitorShift2018()
{
}

Bool_t CbmTofStarMonitorShift2018::Init()
{
   LOG(info) << "Initializing Get4 monitor";

   FairRootManager* ioman = FairRootManager::Instance();
   if( ioman == NULL )
   {
      LOG(fatal) << "No FairRootManager instance";
   } // if( ioman == NULL )

   return kTRUE;
}

void CbmTofStarMonitorShift2018::SetParContainers()
{
   LOG(info) << "Setting parameter containers for " << GetName();
   fUnpackPar = (CbmTofStar2018Par*) (FairRun::Instance()->GetRuntimeDb()->getContainer(
          "CbmTofStar2018Par") );

}

Bool_t CbmTofStarMonitorShift2018::InitContainers()
{
   LOG(info) << "Init parameter containers for " << GetName();
   Bool_t initOK = ReInitContainers();

   CreateHistograms();

   fvulCurrentEpoch.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
   fvbFirstEpochSeen.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
      {
         fvulCurrentEpoch[GetArrayIndex(i, j)] = 0;
      } // for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  return initOK;
}

Bool_t CbmTofStarMonitorShift2018::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();

   fuNrOfGdpbs = fUnpackPar->GetNrOfRocs();
   LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
   fuMinNbGdpb = fuNrOfGdpbs;

   fuNrOfFeetPerGdpb = fUnpackPar->GetNrOfFebsPerGdpb();
   LOG(info) << "Nr. of FEBS per Tof GDPB: " << fuNrOfFeetPerGdpb;

   fuNrOfGet4PerFeb = fUnpackPar->GetNrOfGet4PerFeb();
   LOG(info) << "Nr. of GET4 per Tof FEB: " << fuNrOfGet4PerFeb;

   fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
   LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

   fuNrOfChannelsPerFeet = fuNrOfGet4PerFeb * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per FEET: " << fuNrOfChannelsPerFeet;

   fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFeetPerGdpb * fuNrOfGet4PerFeb;
   LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

   fuNrOfGet4PerGdpb = fuNrOfFeetPerGdpb * fuNrOfGet4PerFeb;
   LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

   fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

   fGdpbIdIndexMap.clear();
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      fGdpbIdIndexMap[fUnpackPar->GetRocId(i)] = i;
      LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackPar->GetRocId(i)
                 << std::dec;
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   UInt_t uNrOfChannels = fUnpackPar->GetNumberOfChannels();
   LOG(info) << "Nr. of mapped Tof channels: " << uNrOfChannels;
   std::stringstream ss;
   for( UInt_t i = 0; i < uNrOfChannels; ++i)
   {
      if(i%8 == 0)
         ss << "\n";
      ss << Form(" 0x%08x",fUnpackPar->GetChannelToDetUIdMap(i));
   } // for( UInt_t i = 0; i < uNrOfChannels; ++i)
   LOG(info) << ss.str();

   fuTotalMsNb   = fUnpackPar->GetNbMsTot();
   fuOverlapMsNb = fUnpackPar->GetNbMsOverlap();
   fuCoreMs      = fuTotalMsNb - fuOverlapMsNb;
   fdMsSizeInNs  = fUnpackPar->GetSizeMsInNs();
   fdTsCoreSizeInNs = fdMsSizeInNs * fuCoreMs;
   LOG(info) << "Timeslice parameters: "
             << fuTotalMsNb << " MS per link, of which "
             << fuOverlapMsNb << " overlap MS, each MS is "
             << fdMsSizeInNs << " ns";

   /// STAR Trigger decoding and monitoring
   fvulGdpbTsMsb.resize(  fuNrOfGdpbs );
   fvulGdpbTsLsb.resize(  fuNrOfGdpbs );
   fvulStarTsMsb.resize(  fuNrOfGdpbs );
   fvulStarTsMid.resize(  fuNrOfGdpbs );
   fvulGdpbTsFullLast.resize(  fuNrOfGdpbs );
   fvulStarTsFullLast.resize(  fuNrOfGdpbs );
   fvuStarTokenLast.resize(  fuNrOfGdpbs );
   fvuStarDaqCmdLast.resize(  fuNrOfGdpbs );
   fvuStarTrigCmdLast.resize(  fuNrOfGdpbs );
   for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
   {
      fvulGdpbTsMsb[ uGdpb ] = 0;
      fvulGdpbTsLsb[ uGdpb ] = 0;
      fvulStarTsMsb[ uGdpb ] = 0;
      fvulStarTsMid[ uGdpb ] = 0;
      fvulGdpbTsFullLast[ uGdpb ] = 0;
      fvulStarTsFullLast[ uGdpb ] = 0;
      fvuStarTokenLast[ uGdpb ]   = 0;
      fvuStarDaqCmdLast[ uGdpb ]  = 0;
      fvuStarTrigCmdLast[ uGdpb ] = 0;
   } // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

   fvmEpSupprBuffer.resize( fuNrOfGet4 );

   ///* Pulser monitoring *///
   fdTsLastPulserHit.resize( fuNrOfFeetPerGdpb * fuNrOfGdpbs, 0.0 );

/// TODO: move these constants somewhere shared, e.g the parameter file
   fvuPadiToGet4.resize( fuNrOfChannelsPerFeet );
   fvuGet4ToPadi.resize( fuNrOfChannelsPerFeet );
   UInt_t uGet4topadi[32] = {
        4,  3,  2,  1,  // provided by Jochen
      24, 23, 22, 21,
       8,  7,  6,  5,
      28, 27, 26, 25,
      12, 11, 10,  9,
      32, 31, 30, 29,
      16, 15, 14, 13,
      20, 19, 18, 17 };

  UInt_t uPaditoget4[32] = {
       4,  3,  2,  1,  // provided by Jochen
      12, 11, 10,  9,
      20, 19, 18, 17,
      28, 27, 26, 25,
      32, 31, 30, 29,
       8,  7,  6,  5,
      16, 15, 14, 13,
      24, 23, 22, 21 };

   for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFeet; ++uChan )
   {
      fvuPadiToGet4[ uChan ] = uPaditoget4[ uChan ] - 1;
      fvuGet4ToPadi[ uChan ] = uGet4topadi[ uChan ] - 1;
   } // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFeet; ++uChan )

	return kTRUE;
}

void CbmTofStarMonitorShift2018::CreateHistograms()
{
   LOG(info) << "create Histos for " << fuNrOfGdpbs <<" gDPBs ";

   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   TString name { "" };
   TString title { "" };

   // Full Fee time difference test
   UInt_t uNbBinsDt = kuNbBinsDt + 1; // To account for extra bin due to shift by 1/2 bin of both ranges

   fuNbFeetPlotsPerGdpb = fuNrOfFeetPerGdpb/fuNbFeetPlot + ( 0 != fuNrOfFeetPerGdpb%fuNbFeetPlot ? 1 : 0 );
   Double_t dBinSzG4v2 = (6250. / 112.);
   dMinDt     = -1.*(kuNbBinsDt*dBinSzG4v2/2.) - dBinSzG4v2/2.;
   dMaxDt     =  1.*(kuNbBinsDt*dBinSzG4v2/2.) + dBinSzG4v2/2.;

   /*******************************************************************/
   name = "hMessageType";
   title = "Nb of message for each type; Type";
   // Test Big Data readout with plotting
   fhMessType = new TH1I(name, title, 16, 0., 16.);
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_NOP,      "NOP");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_HIT,      "HIT");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_EPOCH,    "EPOCH");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_SYNC,     "SYNC");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_AUX,      "AUX");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_EPOCH2,   "EPOCH2");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_GET4,     "GET4");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_SYS,      "SYS");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_GET4_SLC, "MSG_GET4_SLC");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_GET4_32B, "MSG_GET4_32B");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_GET4_SYS, "MSG_GET4_SYS");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_STAR_TRI, "MSG_STAR_TRI");
   fhMessType->GetXaxis()->SetBinLabel(1 + 15,                 "GET4 Hack 32B");
   fhMessType->GetXaxis()->SetBinLabel(1 + gdpb::MSG_NOP,      "NOP");

   /*******************************************************************/
   name = "hSysMessType";
   title = "Nb of system message for each type; System Type";
   fhSysMessType = new TH1I(name, title, 17, 0., 17.);
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_DAQ_START,       "DAQ START");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_NX_PARITY,       "NX PARITY");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_SYNC_PARITY,     "SYNC PARITY");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_DAQ_RESUME,      "DAQ RESUME");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_FIFO_RESET,      "FIFO RESET");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_USER,            "USER");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_PCTIME,          "PCTIME");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_ADC,             "ADC");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_PACKETLOST,      "PACKET LOST");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_GET4_EVENT,      "GET4 ERROR");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_CLOSYSYNC_ERROR, "CLOSYSYNC ERROR");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_TS156_SYNC,      "TS156 SYNC");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + gdpb::SYSMSG_GDPB_UNKWN,      "UNKW GET4 MSG");
   fhSysMessType->GetXaxis()->SetBinLabel(1 + 16,                            "GET4 Hack 32B");

   /*******************************************************************/
   name = "hGet4MessType";
   title = "Nb of message for each type per GET4; GET4 chip # ; Type";
   fhGet4MessType = new TH2I(name, title, fuNrOfGet4, 0., fuNrOfGet4, 5, 0., 5.);
   fhGet4MessType->GetYaxis()->SetBinLabel(1 + gdpb::GET4_32B_EPOCH,    "EPOCH");
   fhGet4MessType->GetYaxis()->SetBinLabel(1 + gdpb::GET4_32B_SLCM,     "S.C. M");
   fhGet4MessType->GetYaxis()->SetBinLabel(1 + gdpb::GET4_32B_ERROR,    "ERROR");
   fhGet4MessType->GetYaxis()->SetBinLabel(1 + gdpb::GET4_32B_DATA,     "DATA 32b");
   fhGet4MessType->GetYaxis()->SetBinLabel(1 + gdpb::GET4_32B_DATA + 1, "DATA 24b");

   /*******************************************************************/
   name = "hGet4ChanScm";
   title = "SC messages per GET4 channel; GET4 channel # ; SC type";
   fhGet4ChanScm =  new TH2I(name, title,
         2 * fuNrOfGet4 * fuNrOfChannelsPerGet4, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4,
         5, 0., 5.);
   fhGet4ChanScm->GetYaxis()->SetBinLabel( 1, "Hit Scal" );
   fhGet4ChanScm->GetYaxis()->SetBinLabel( 2, "Deadtime" );
   fhGet4ChanScm->GetYaxis()->SetBinLabel( 3, "SPI" );
   fhGet4ChanScm->GetYaxis()->SetBinLabel( 4, "SEU Scal" );
   fhGet4ChanScm->GetYaxis()->SetBinLabel( 5, "START" );

   /*******************************************************************/
   name = "hGet4ChanErrors";
   title = "Error messages per GET4 channel; GET4 channel # ; Error";
   fhGet4ChanErrors = new TH2I(name, title,
            fuNrOfGet4 * fuNrOfChannelsPerGet4, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4,
            21, 0., 21.);
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 1, "0x00: Readout Init    ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 2, "0x01: Sync            ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 3, "0x02: Epoch count sync");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 4, "0x03: Epoch           ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 5, "0x04: FIFO Write      ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 6, "0x05: Lost event      ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 7, "0x06: Channel state   ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 8, "0x07: Token Ring state");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel( 9, "0x08: Token           ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(10, "0x09: Error Readout   ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(11, "0x0a: SPI             ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(12, "0x0b: DLL Lock error  "); // <- From GET4 v1.2
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(13, "0x0c: DLL Reset invoc."); // <- From GET4 v1.2
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(14, "0x11: Overwrite       ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(15, "0x12: ToT out of range");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(16, "0x13: Event Discarded ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(17, "0x14: Add. Rising edge"); // <- From GET4 v1.3
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(18, "0x15: Unpaired Falling"); // <- From GET4 v1.3
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(19, "0x16: Sequence error  "); // <- From GET4 v1.3
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(20, "0x7f: Unknown         ");
   fhGet4ChanErrors->GetYaxis()->SetBinLabel(21, "Corrupt/unsuprtd error");

   /*******************************************************************/
   name = "hGet4EpochFlags";
   title = "Epoch flags per GET4; GET4 chip # ; Type";
   fhGet4EpochFlags = new TH2I(name, title, fuNrOfGet4, 0., fuNrOfGet4, 4, 0., 4.);
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(1, "SYNC");
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(2, "Ep LOSS");
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(3, "Da LOSS");
   fhGet4EpochFlags->GetYaxis()->SetBinLabel(4, "MISSMAT");

   /*******************************************************************/
      // Slow control messages analysis
   name = "hScmScalerCounters";
   title = "Content of Scaler counter SC messages; Scaler counter [hit]; Channel []";
   fhScmScalerCounters = new TH2I(name, title, fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                               8192, 0., 8192.);

   name = "hScmDeadtimeCounters";
   title = "Content of Deadtime counter SC messages; Deadtime [Clk Cycles]; Channel []";
   fhScmDeadtimeCounters = new TH2I(name, title, fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                               8192, 0., 8192.);

   name = "hScmSeuCounters";
   title = "Content of SEU counter SC messages; SEU []; Channel []";
   fhScmSeuCounters = new TH2I(name, title, fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                               8192, 0., 8192.);

   name = "hScmSeuCountersEvo";
   title = "SEU counter rate from SC messages; Time in Run [s]; Channel []; SEU []";
   fhScmSeuCountersEvo = new TH2I(name, title, fuNrOfGet4 * fuNrOfChannelsPerGet4 * 2, 0., fuNrOfGet4 * fuNrOfChannelsPerGet4,
                                               fuHistoryHistoSize, 0., fuHistoryHistoSize);

   /*******************************************************************/
  for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
  {
      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
      name = Form("RawFt_gDPB_%02u", uGdpb);
      title = Form("Raw FineTime gDPB %02u Plot 0; channel; FineTime [bin]", uGdpb);
      fvhRawFt_gDPB.push_back(
         new TH2F(name.Data(), title.Data(),
            fuNrOfChannelsPerGdpb, 0, fuNrOfChannelsPerGdpb,
            128, 0, 128 ) );

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
      name = Form("RawTot_gDPB_%02u_0", uGdpb);
      title = Form("Raw TOT gDPB %02u Plot 0; channel; TOT [bin]", uGdpb);
      fvhRawTot_gDPB.push_back(
         new TH2F(name.Data(), title.Data(),
            fuNbFeetPlot*fuNrOfChannelsPerFeet, 0*fuNbFeetPlot*fuNrOfChannelsPerFeet, 1*fuNbFeetPlot*fuNrOfChannelsPerFeet,
            256, 0, 256 ) );

      if( fuNbFeetPlot < fuNrOfFeetPerGdpb )
      {
         name = Form("RawTot_gDPB_%02u_1", uGdpb);
         title = Form("Raw TOT gDPB %02u Plot 1; channel; TOT [bin]", uGdpb);
         fvhRawTot_gDPB.push_back(
            new TH2F(name.Data(), title.Data(),
               fuNbFeetPlot*fuNrOfChannelsPerFeet, 1*fuNbFeetPlot*fuNrOfChannelsPerFeet, 2*fuNbFeetPlot*fuNrOfChannelsPerFeet,
               256, 0, 256));
      } // if( fuNbFeetPlot < fuNrOfFeetPerGdpb )
      if( 2 * fuNbFeetPlot < fuNrOfFeetPerGdpb )
      {
         name = Form("RawTot_gDPB_%02u_2", uGdpb);
         title = Form("Raw TOT gDPB %02u Plot 2; channel; TOT [bin]", uGdpb);
         fvhRawTot_gDPB.push_back(
            new TH2F(name.Data(), title.Data(),
               fuNbFeetPlot*fuNrOfChannelsPerFeet, 2*fuNbFeetPlot*fuNrOfChannelsPerFeet, 3*fuNbFeetPlot*fuNrOfChannelsPerFeet,
               256, 0, 256));
      } // if( 2 * fuNbFeetPlot < fuNrOfFeetPerGdpb )

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
       name = Form("ChCount_gDPB_%02u", uGdpb);
       title = Form("Channel counts gDPB %02u; channel; Hits", uGdpb);
       fvhChCount_gDPB.push_back( new TH1I(name.Data(), title.Data(),
                fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet, 0, fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet) );

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
       name = Form("ChRate_gDPB_%02u", uGdpb);
       title = Form("Channel rate gDPB %02u; Time in run [s]; channel; Rate [1/s]", uGdpb);
       fvhChannelRate_gDPB.push_back( new TH2D(name.Data(), title.Data(),
                fuHistoryHistoSize, 0, fuHistoryHistoSize,
                fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet, 0, fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet ) );

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
      name = Form("RemapTot_gDPB_%02u_0", uGdpb);
      title = Form("Raw TOT gDPB %02u remapped Plot 0; PADI channel; TOT [bin]", uGdpb);
      fvhRemapTot_gDPB.push_back(
         new TH2F(name.Data(), title.Data(),
            fuNbFeetPlot*fuNrOfChannelsPerFeet, 0*fuNbFeetPlot*fuNrOfChannelsPerFeet, 1*fuNbFeetPlot*fuNrOfChannelsPerFeet,
            256, 0, 256 ) );

      if( fuNbFeetPlot < fuNrOfFeetPerGdpb )
      {
         name = Form("RemapTot_gDPB_%02u_1", uGdpb);
         title = Form("Raw TOT gDPB %02u remapped Plot 1; PADI channel; TOT [bin]", uGdpb);
         fvhRemapTot_gDPB.push_back(
            new TH2F(name.Data(), title.Data(),
               fuNbFeetPlot*fuNrOfChannelsPerFeet, 1*fuNbFeetPlot*fuNrOfChannelsPerFeet, 2*fuNbFeetPlot*fuNrOfChannelsPerFeet,
               256, 0, 256));
      } // if( fuNbFeetPlot < fuNrOfFeetPerGdpb )
      if( 2 * fuNbFeetPlot < fuNrOfFeetPerGdpb )
      {
         name = Form("RemapTot_gDPB_%02u_2", uGdpb);
         title = Form("Raw TOT gDPB %02u remapped Plot 2; PADI channel; TOT [bin]", uGdpb);
         fvhRemapTot_gDPB.push_back(
            new TH2F(name.Data(), title.Data(),
               fuNbFeetPlot*fuNrOfChannelsPerFeet, 2*fuNbFeetPlot*fuNrOfChannelsPerFeet, 3*fuNbFeetPlot*fuNrOfChannelsPerFeet,
               256, 0, 256));
      } // if( 2 * fuNbFeetPlot < fuNrOfFeetPerGdpb )

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
       name = Form("RemapChCount_gDPB_%02u", uGdpb);
       title = Form("Channel counts gDPB %02u remapped; PADI channel; Hits", uGdpb);
       fvhRemapChCount_gDPB.push_back( new TH1I(name.Data(), title.Data(),
                fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet, 0, fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet) );

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
       name = Form("RemapChRate_gDPB_%02u", uGdpb);
       title = Form("PADI channel rate gDPB %02u; Time in run [s]; PADI channel; Rate [1/s]", uGdpb);
       fvhRemapChRate_gDPB.push_back( new TH2D(name.Data(), title.Data(),
                fuHistoryHistoSize, 0, fuHistoryHistoSize,
                fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet, 0, fuNrOfFeetPerGdpb * fuNrOfChannelsPerFeet ) );

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)
      {
         name = Form("FeetRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
             "Counts per second in Feet %1u of gDPB %02u; Time[s] ; Counts", uFeet,
             uGdpb);
         fvhFeetRate_gDPB.push_back( new TH1D(name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize) );

         name = Form("FeetErrorRate_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
             "Error Counts per second in Feet %1u of gDPB %02u; Time[s] ; Error Counts", uFeet,
             uGdpb);
         fvhFeetErrorRate_gDPB.push_back( new TH1D(name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize) );

         name = Form("FeetErrorRatio_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
             "Error to data ratio per second in Feet %1u of gDPB %02u; Time[s] ; Error ratio[]", uFeet,
             uGdpb);
         fvhFeetErrorRatio_gDPB.push_back( new TProfile(name.Data(), title.Data(), fuHistoryHistoSize, 0, fuHistoryHistoSize) );

         name = Form("FeetRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
             "Counts per minutes in Feet %1u of gDPB %02u; Time[min] ; Counts", uFeet,
             uGdpb);
         fvhFeetRateLong_gDPB.push_back( new TH1D(name.Data(), title.Data(), fuHistoryHistoSizeLong, 0, fuHistoryHistoSizeLong) );

         name = Form("FeetErrorRateLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
             "Error Counts per minutes in Feet %1u of gDPB %02u; Time[min] ; Error Counts", uFeet,
             uGdpb);
         fvhFeetErrorRateLong_gDPB.push_back( new TH1D(name.Data(), title.Data(), fuHistoryHistoSizeLong, 0, fuHistoryHistoSizeLong) );

         name = Form("FeetErrorRatioLong_gDPB_g%02u_f%1u", uGdpb, uFeet);
         title = Form(
             "Error to data ratio per minutes in Feet %1u of gDPB %02u; Time[min] ; Error ratio[]", uFeet,
             uGdpb);
         fvhFeetErrorRatioLong_gDPB.push_back( new TProfile(name.Data(), title.Data(), fuHistoryHistoSizeLong, 0, fuHistoryHistoSizeLong) );
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; uFeet++)

      /**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
      /// STAR Trigger decoding and monitoring
      name = Form( "hTokenMsgType_gDPB_%02u", uGdpb);
      title = Form( "STAR trigger Messages type gDPB %02u; Type ; Counts", uGdpb);
      fvhTokenMsgType.push_back(  new TH1F(name, title, 4, 0, 4) );
      fvhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 1, "A"); // gDPB TS high
      fvhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 2, "B"); // gDPB TS low, STAR TS high
      fvhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 3, "C"); // STAR TS mid
      fvhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 4, "D"); // STAR TS low, token, CMDs
      if (server)
         server->Register("/StarRaw", fvhTokenMsgType[ uGdpb ] );

      name = Form( "hTriggerRate_gDPB_%02u", uGdpb);
      title = Form( "STAR trigger signals per second gDPB %02u; Time[s] ; Counts", uGdpb);
      fvhTriggerRate.push_back(  new TH1F(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize) );
      if (server)
         server->Register("/StarRaw", fvhTriggerRate[ uGdpb ] );

      name = Form( "hCmdDaqVsTrig_gDPB_%02u", uGdpb);
      title = Form( "STAR daq command VS STAR trigger command gDPB %02u; DAQ ; TRIGGER", uGdpb);
      fvhCmdDaqVsTrig.push_back( new TH2I(name, title, 16, 0, 16, 16, 0, 16 ) );
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 1, "0x0: no-trig "); // idle link
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 2, "0x1: clear   "); // clears redundancy counters on the readout boards
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 3, "0x2: mast-rst"); // general reset of the whole front-end logic
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 4, "0x3: spare   "); // reserved
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 5, "0x4: trigg. 0"); // Default physics readout, all det support required
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 6, "0x5: trigg. 1"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 7, "0x6: trigg. 2"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 8, "0x7: trigg. 3"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 9, "0x8: puls.  0"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(10, "0x9: puls.  1"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(11, "0xA: puls.  2"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(12, "0xB: puls.  3"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(13, "0xC: config  "); // housekeeping trigger: return geographic info of FE
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(14, "0xD: abort   "); // aborts and clears an active event
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(15, "0xE: L1accept"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(16, "0xF: L2accept"); //
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 1, "0x0:  0"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 2, "0x1:  1"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 3, "0x2:  2"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 4, "0x3:  3"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 5, "0x4:  4"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 6, "0x5:  5"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 7, "0x6:  6"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 8, "0x7:  7"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 9, "0x8:  8"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(10, "0x9:  9"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(11, "0xA: 10"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(12, "0xB: 11"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(13, "0xC: 12"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(14, "0xD: 13"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(15, "0xE: 14"); // To be filled at STAR
      fvhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(16, "0xF: 15"); // To be filled at STAR
      if (server)
         server->Register("/StarRaw", fvhCmdDaqVsTrig[ uGdpb ] );

      name = Form( "hStarTokenEvo_gDPB_%02u", uGdpb);
      title = Form( "STAR token value VS time gDPB %02u; Time in Run [s] ; STAR Token; Counts", uGdpb);
      fvhStarTokenEvo.push_back( new TH2I(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize, 410, 0, 4100 ) );


      name = Form( "hStarTrigGdpbTsEvo_gDPB_%02u", uGdpb);
      title = Form( "gDPB TS in STAR triger tokens for gDPB %02u; Time in Run [s] ; gDPB TS;", uGdpb);
      fvhStarTrigGdpbTsEvo.push_back( new TProfile(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize ) );

      name = Form( "hStarTrigStarTsEvo_gDPB_%02u", uGdpb);
      title = Form( "STAR TS in STAR triger tokens for gDPB %02u; Time in Run [s] ; STAR TS;", uGdpb);
      fvhStarTrigStarTsEvo.push_back( new TProfile(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize ) );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

   /*******************************************************************/
   /// FEET pulser test channels
   fvhTimeDiffPulser.resize( fuNrOfFeetPerGdpb * fuNrOfGdpbs );
   for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeA++)
   {
      fvhTimeDiffPulser[uFeeA].resize( fuNrOfFeetPerGdpb * fuNrOfGdpbs );
      for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
      {
         if( uFeeA < uFeeB )
         {
            UInt_t uGdpbA = uFeeA / ( fuNrOfFeetPerGdpb );
            UInt_t uFeetA = uFeeA - ( fuNrOfFeetPerGdpb * uGdpbA );
            UInt_t uGdpbB = uFeeB / ( fuNrOfFeetPerGdpb );
            UInt_t uFeetB = uFeeB - ( fuNrOfFeetPerGdpb * uGdpbB );
            fvhTimeDiffPulser[uFeeA][uFeeB] = new TH1I(
               Form("hTimeDiffPulser_g%02u_f%1u_g%02u_f%1u", uGdpbA, uFeetA, uGdpbB, uFeetB),
               Form("Time difference for pulser on gDPB %02u FEE %1u and gDPB %02u FEE %1u; DeltaT [ps]; Counts",
                     uGdpbA, uFeetA, uGdpbB, uFeetB ),
               uNbBinsDt, dMinDt, dMaxDt);
         } // if( uFeeA < uFeeB )
            else fvhTimeDiffPulser[uFeeA][uFeeB] = NULL;
      } // for( UInt_t uFeeB = uFeeA; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1; uFeeB++)
   } // for( UInt_t uFeeA = 0; uFeeA < kuNbChanTest - 1; uFeeA++)

   name = "hTimeRmsPulser";
   fhTimeRmsPulser = new TH2D( name.Data(),
         "Time difference RMS for each FEE pairs; FEE A; FEE B ; [ps]",
         fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1.5,
         fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1,  0.5, fuNrOfFeetPerGdpb * fuNrOfGdpbs - 0.5 );

   name = "hTimeRmsZoomFitPuls";
   fhTimeRmsZoomFitPuls = new TH2D( name.Data(),
         "Time difference RMS after zoom for each FEE pairs; FEE A; FEE B ; RMS [ps]",
         fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1.5,
         fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1,  0.5, fuNrOfFeetPerGdpb * fuNrOfGdpbs - 0.5 );

   name = "hTimeResFitPuls";
   fhTimeResFitPuls = new TH2D( name.Data(),
         "Time difference Res from fit for each FEE pairs; FEE A; FEE B ; Sigma [ps]",
         fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1, -0.5, fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1.5,
         fuNrOfFeetPerGdpb * fuNrOfGdpbs - 1,  0.5, fuNrOfFeetPerGdpb * fuNrOfGdpbs - 0.5 );

   if( server )
   {
      server->Register("/TofRaw", fhMessType );
      server->Register("/TofRaw", fhSysMessType );
      server->Register("/TofRaw", fhGet4MessType );
      server->Register("/TofRaw", fhGet4ChanScm );
      server->Register("/TofRaw", fhGet4ChanErrors );
      server->Register("/TofRaw", fhGet4EpochFlags );
      server->Register("/TofRaw", fhScmScalerCounters );
      server->Register("/TofRaw", fhScmDeadtimeCounters );
      server->Register("/TofRaw", fhScmSeuCounters );
      server->Register("/TofRaw", fhScmSeuCountersEvo );

      for( UInt_t uTotPlot = 0; uTotPlot < fvhRawTot_gDPB.size(); ++uTotPlot )
         server->Register("/TofRaw", fvhRawTot_gDPB[ uTotPlot ] );

      for( UInt_t uTotPlot = 0; uTotPlot < fvhRemapTot_gDPB.size(); ++uTotPlot )
         server->Register("/TofRaw", fvhRemapTot_gDPB[ uTotPlot ] );

      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
      {
         server->Register("/TofRaw", fvhRawFt_gDPB[ uGdpb ] );
         server->Register("/TofRaw", fvhChCount_gDPB[ uGdpb ] );
         server->Register("/TofRates", fvhChannelRate_gDPB[ uGdpb ] );
         server->Register("/TofRaw", fvhRemapChCount_gDPB[ uGdpb ] );
         server->Register("/TofRates", fvhRemapChRate_gDPB[ uGdpb ] );

         for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)
         {
            server->Register("/TofRates", fvhFeetRate_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ] );
            server->Register("/TofRates", fvhFeetErrorRate_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ] );
            server->Register("/TofRates", fvhFeetErrorRatio_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ] );
            server->Register("/TofRates", fvhFeetRateLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ] );
            server->Register("/TofRates", fvhFeetErrorRateLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ] );
            server->Register("/TofRates", fvhFeetErrorRatioLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ] );
         } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)

         server->Register("/StarRaw", fvhTokenMsgType[ uGdpb ] );
         server->Register("/StarRaw", fvhTriggerRate[ uGdpb ] );
         server->Register("/StarRaw", fvhCmdDaqVsTrig[ uGdpb ] );
         server->Register("/StarRaw", fvhStarTokenEvo[ uGdpb ] );
         server->Register("/StarRaw", fvhStarTrigGdpbTsEvo[ uGdpb ] );
         server->Register("/StarRaw", fvhStarTrigStarTsEvo[ uGdpb ] );
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

      for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeA++)
         for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
            if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
               server->Register("/TofDt", fvhTimeDiffPulser[uFeeA][uFeeB] );

      server->Register("/TofRaw", fhTimeRmsPulser );
      server->Register("/TofRaw", fhTimeRmsZoomFitPuls );
      server->Register("/TofRaw", fhTimeResFitPuls );


      server->RegisterCommand("/Reset_All_eTOF", "bResetTofStarMoniShiftHistos=kTRUE");
      server->RegisterCommand("/Save_All_eTof",  "bSaveTofStarMoniShiftHistos=kTRUE");
      server->RegisterCommand("/Update_PulsFit", "bTofUpdateZoomedFitMoniShift=kTRUE");

      server->Restrict("/Reset_All_eTof", "allow=admin");
      server->Restrict("/Save_All_eTof",  "allow=admin");
      server->Restrict("/Update_PulsFit", "allow=admin");
   } // if( server )

   /** Create summary Canvases for STAR 2018 **/
   Double_t w = 10;
   Double_t h = 10;
   TCanvas* cSummary = new TCanvas("cSummary", "gDPB Monitoring Summary", w, h);
   cSummary->Divide(2, 3);

   // 1st Column: Messages types
   cSummary->cd(1);
   gPad->SetLogy();
   fhMessType->Draw();

   cSummary->cd(2);
   gPad->SetLogy();
   fhSysMessType->Draw();

   cSummary->cd(3);
   gPad->SetLogz();
   fhGet4MessType->Draw("colz");

   // 2nd Column: GET4 Errors + Epoch flags + SCm
   cSummary->cd(4);
   gPad->SetLogz();
   fhGet4ChanErrors->Draw("colz");

   cSummary->cd(5);
   gPad->SetLogz();
   fhGet4EpochFlags->Draw("colz");

   cSummary->cd(6);
   fhGet4ChanScm->Draw("colz");
   /*****************************/

   /** Create FEET rates Canvas for STAR 2018 **/
   TCanvas* cFeeRates = new TCanvas("cFeeRates", "gDPB Monitoring FEET rates", w, h);
   cFeeRates->Divide(fuNrOfFeetPerGdpb, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
      {
         cFeeRates->cd( 1 + uGdpb * fuNrOfFeetPerGdpb + uFeet );
         gPad->SetLogy();
         fvhFeetRate_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->Draw("hist");

         fvhFeetErrorRate_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->SetLineColor( kRed );
         fvhFeetErrorRate_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->Draw("same hist");
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create FEET error ratio Canvas for STAR 2018 **/
   TCanvas* cFeeErrRatio = new TCanvas("cFeeErrRatio", "gDPB Monitoring FEET error ratios", w, h);
   cFeeErrRatio->Divide(fuNrOfFeetPerGdpb, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
      {
         cFeeErrRatio->cd( 1 + uGdpb * fuNrOfFeetPerGdpb + uFeet );
         gPad->SetLogy();
         fvhFeetErrorRatio_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->Draw( "hist le0");
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/


   /** Create FEET rates long Canvas for STAR 2018 **/
   TCanvas* cFeeRatesLong = new TCanvas("cFeeRatesLong", "gDPB Monitoring FEET rates", w, h);
   cFeeRatesLong->Divide(fuNrOfFeetPerGdpb, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
      {
         cFeeRatesLong->cd( 1 + uGdpb * fuNrOfFeetPerGdpb + uFeet );
         gPad->SetLogy();
         fvhFeetRateLong_gDPB[uGdpb]->Draw( "hist" );

         fvhFeetErrorRateLong_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->SetLineColor( kRed );
         fvhFeetErrorRateLong_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->Draw("same hist");
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create FEET error ratio long Canvas for STAR 2018 **/
   TCanvas* cFeeErrRatioLong = new TCanvas("cFeeErrRatioLong", "gDPB Monitoring FEET error ratios", w, h);
   cFeeErrRatioLong->Divide(fuNrOfFeetPerGdpb, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
      {
         cFeeErrRatioLong->cd( 1 + uGdpb * fuNrOfFeetPerGdpb + uFeet );
         gPad->SetLogy();
         fvhFeetErrorRatioLong_gDPB[uGdpb * fuNrOfFeetPerGdpb + uFeet]->Draw( "hist le0");
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++uFeet )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create channel count Canvas for STAR 2018 **/
   TCanvas* cGdpbChannelCount = new TCanvas("cGdpbChannelCount", "Integrated Get4 channel counts per gDPB", w, h);
   cGdpbChannelCount->Divide( 1, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      cGdpbChannelCount->cd( 1 + uGdpb );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhChCount_gDPB[ uGdpb ]->Draw();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create remapped channel count Canvas for STAR 2018 **/
   TCanvas* cGdpbRemapChCount = new TCanvas("cGdpbRemapChCount", "Integrated PADI channel counts per gDPB", w, h);
   cGdpbRemapChCount->Divide( 1, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      cGdpbRemapChCount->cd( 1 + uGdpb );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogy();
      fvhRemapChCount_gDPB[ uGdpb ]->Draw();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create channel rate Canvas for STAR 2018 **/
   TCanvas* cGdpbChannelRate = new TCanvas("cGdpbChannelRate", "Get4 channel rate per gDPB", w, h);
   cGdpbChannelRate->Divide( 1, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      cGdpbChannelRate->cd( 1 + uGdpb );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fvhChannelRate_gDPB[ uGdpb ]->Draw( "colz" );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create remapped rate count Canvas for STAR 2018 **/
   TCanvas* cGdpbRemapChRate = new TCanvas("cGdpbRemapChRate", "PADI channel rate per gDPB", w, h);
   cGdpbRemapChRate->Divide( 1, fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      cGdpbRemapChRate->cd( 1 + uGdpb );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogz();
      fvhRemapChRate_gDPB[ uGdpb ]->Draw( "colz" );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   /*****************************/

   /** Create TOT Canvas(es) for STAR 2018 **/
   TCanvas* cTotPnt = NULL;
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      cTotPnt = new TCanvas( Form("cTot_g%02u", uGdpb),
                             Form("gDPB %02u TOT distributions", uGdpb),
                             w, h);
      cTotPnt->Divide( fuNbFeetPlotsPerGdpb );

      for( UInt_t uFeetPlot = 0; uFeetPlot < fuNbFeetPlotsPerGdpb; ++uFeetPlot )
      {
         cTotPnt->cd( 1 + uFeetPlot );
         gPad->SetGridx();
         gPad->SetGridy();
         gPad->SetLogz();

         fvhRawTot_gDPB[ uGdpb * fuNbFeetPlotsPerGdpb + uFeetPlot ]->Draw( "colz" );
      } // for (UInt_t uFeet = 0; uFeet < fuNbFeetPlotsPerGdpb; ++uFeet )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   cTotPnt  = new TCanvas( "cTot_all", "TOT distributions", w, h);
   cTotPnt->Divide( fuNrOfGdpbs, fuNbFeetPlotsPerGdpb );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
      for( UInt_t uFeetPlot = 0; uFeetPlot < fuNbFeetPlotsPerGdpb; ++uFeetPlot )
      {
         cTotPnt->cd( 1 + uGdpb + fuNrOfGdpbs * uFeetPlot );
         gPad->SetGridx();
         gPad->SetGridy();
         gPad->SetLogz();

         fvhRawTot_gDPB[ uGdpb * fuNbFeetPlotsPerGdpb + uFeetPlot]->Draw( "colz" );
      } // for (UInt_t uFeet = 0; uFeet < fuNbFeetPlotsPerGdpb; ++uFeet )
   /**************************************************/

   /** Create PADI TOT Canvas(es) for STAR 2018 **/
   cTotPnt = NULL;
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      cTotPnt = new TCanvas( Form("cTotRemap_g%02u", uGdpb),
                             Form("PADI ch gDPB %02u TOT distributions", uGdpb),
                             w, h);
      cTotPnt->Divide( fuNbFeetPlotsPerGdpb );

      for( UInt_t uFeetPlot = 0; uFeetPlot < fuNbFeetPlotsPerGdpb; ++uFeetPlot )
      {
         cTotPnt->cd( 1 + uFeetPlot );
         gPad->SetGridx();
         gPad->SetGridy();
         gPad->SetLogz();

         fvhRemapTot_gDPB[ uGdpb * fuNbFeetPlotsPerGdpb + uFeetPlot ]->Draw( "colz" );
      } // for (UInt_t uFeet = 0; uFeet < fuNbFeetPlotsPerGdpb; ++uFeet )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   cTotPnt  = new TCanvas( "cTotRemap_all", "TOT distributions", w, h);
   cTotPnt->Divide( fuNrOfGdpbs, fuNbFeetPlotsPerGdpb );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
      for( UInt_t uFeetPlot = 0; uFeetPlot < fuNbFeetPlotsPerGdpb; ++uFeetPlot )
      {
         cTotPnt->cd( 1 + uGdpb + fuNrOfGdpbs * uFeetPlot );
         gPad->SetGridx();
         gPad->SetGridy();
         gPad->SetLogz();

         fvhRemapTot_gDPB[ uGdpb * fuNbFeetPlotsPerGdpb + uFeetPlot]->Draw( "colz" );
      } // for (UInt_t uFeet = 0; uFeet < fuNbFeetPlotsPerGdpb; ++uFeet )
   /**************************************************/

   /** Create STAR token Canvas for STAR 2018 **/
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      TCanvas* cStarToken = new TCanvas( Form("cStarToken_g%02u", uGdpb),
                                           Form("STAR token detection info for gDPB %02u", uGdpb),
                                           w, h);
      cStarToken->Divide( 2, 2 );

      cStarToken->cd(1);
      fvhTriggerRate[uGdpb]->Draw();

      cStarToken->cd(2);
      fvhCmdDaqVsTrig[uGdpb]->Draw( "colz" );

      cStarToken->cd(3);
      fvhStarTokenEvo[uGdpb]->Draw();

      cStarToken->cd(4);
      fvhStarTrigGdpbTsEvo[uGdpb]->Draw( "hist le0" );
      fvhStarTrigStarTsEvo[uGdpb]->SetLineColor( kRed );
      fvhStarTrigStarTsEvo[uGdpb]->Draw( "same hist le0" );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   /*****************************/

   /** Create Pulser check Canvas for STAR 2018 **/
   TCanvas* cPulser = new TCanvas("cPulser", "Time difference RMS for pulser channels when FEE pulser mode is ON", w, h);
   cPulser->Divide( 3 );

   cPulser->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   fhTimeRmsPulser->Draw( "colz" );

   cPulser->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   fhTimeRmsZoomFitPuls->Draw( "colz" );

   cPulser->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   fhTimeResFitPuls->Draw( "colz" );
   /*****************************/

   /** Recovers/Create Ms Size Canvas for STAR 2018 **/
   // Try to recover canvas in case it was created already by another monitor
   // If not existing, create it
   fcMsSizeAll = dynamic_cast<TCanvas *>( gROOT->FindObject( "cMsSizeAll" ) );
   if( NULL == fcMsSizeAll )
   {
      fcMsSizeAll = new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
      fcMsSizeAll->Divide( 4, 3 );
      LOG(info) << "Created MS size canvas in TOF monitor";
   } // if( NULL == fcMsSizeAll )
      else LOG(info) << "Recovered MS size canvas in TOF monitor";

  LOG(info) << "Leaving CreateHistograms";
}

Bool_t CbmTofStarMonitorShift2018::DoUnpack(const fles::Timeslice& ts,
    size_t component)
{
   if( bResetTofStarMoniShiftHistos )
   {
      LOG(info) << "Reset eTOF STAR histos ";
      ResetAllHistos();
      bResetTofStarMoniShiftHistos = kFALSE;
   } // if( bResetTofStarMoniShiftHistos )
   if( bSaveTofStarMoniShiftHistos )
   {
      LOG(info) << "Start saving eTOF STAR histos ";
      SaveAllHistos( "data/histos_Shift_StarTof.root" );
      bSaveTofStarMoniShiftHistos = kFALSE;
   } // if( bSaveStsHistos )
   if( bTofUpdateZoomedFitMoniShift )
   {
      UpdateZoomedFit();
      bTofUpdateZoomedFitMoniShift = kFALSE;
   } // if (bTofUpdateZoomedFitMoniShift)

  LOG(debug1) << "Timeslice contains " << ts.num_microslices(component)
                 << "microslices.";

   // MS size monitoring
   if( NULL == fvhMsSzPerLink[ component ] )
   {
      TString sMsSzName  = Form( "MsSz_link_%02lu", component );
      TString sMsSzTitle = Form( "Size of MS from link %02lu; Ms Size [bytes]", component );
      fvhMsSzPerLink[ component ] = new TH1F(sMsSzName.Data(), sMsSzTitle.Data(), 160000, 0., 20000. );

      sMsSzName = Form("MsSzTime_link_%02lu", component);
      sMsSzTitle = Form( "Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]", component);
      fvhMsSzTimePerLink[ component ] =  new TProfile( sMsSzName.Data(), sMsSzTitle.Data(),
                                                       100 * fuHistoryHistoSize, 0., 2 * fuHistoryHistoSize );
      THttpServer* server = FairRunOnline::Instance()->GetHttpServer();
      if( server )
      {
         server->Register("/FlibRaw", fvhMsSzPerLink[ component ]);
         server->Register("/FlibRaw", fvhMsSzTimePerLink[ component ]);
      } // if( server )
      if( NULL != fcMsSizeAll )
      {
         fcMsSizeAll->cd( 1 + component );
         gPad->SetLogy();
         fvhMsSzTimePerLink[ component ]->Draw("hist le0");
      } // if( NULL != fcMsSizeAll )
      LOG(info) << "Added MS size histo for component (link): " << component;
   } // if( NULL == fvhMsSzPerLink[ component ] )

   /// Periodically save the histograms
   std::chrono::time_point<std::chrono::system_clock> timeCurrent = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = timeCurrent - fTimeLastHistoSaving;
   if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
   {
      fTimeLastHistoSaving = timeCurrent;
//      fulNbBuiltSubEventLastPrintout = fulNbBuiltSubEvent;
//      fulNbStarSubEventLastPrintout  = fulNbStarSubEvent;
   } // if( 0 == fTimeLastHistoSaving.time_since_epoch().count() )
   else if( 300 < elapsed_seconds.count() )
   {
      std::time_t cTimeCurrent = std::chrono::system_clock::to_time_t( timeCurrent );
      char tempBuff[80];
      std::strftime( tempBuff, 80, "%F %T", localtime (&cTimeCurrent) );
/*
      LOG(info) << "CbmTofStarEventBuilder2018::DoUnpack => " << tempBuff
               << " Total number of Built events: " << std::setw(9) << fulNbBuiltSubEvent
               << ", " << std::setw(9) << (fulNbBuiltSubEvent - fulNbBuiltSubEventLastPrintout)
               << " events in last " << std::setw(4) << elapsed_seconds.count() << " s";
      fTimeLastPrintoutNbStarEvent = timeCurrent;
      fulNbBuiltSubEventLastPrintout   = fulNbBuiltSubEvent;
*/
      fTimeLastHistoSaving = timeCurrent;
      SaveAllHistos( "data/histos_shift.root" );
   } // else if( 300 < elapsed_seconds.count() )

   Int_t messageType = -111;
   Double_t dTsStartTime = -1;
   // Loop over microslices
   size_t numCompMsInTs = ts.num_microslices(component);
   for( size_t m = 0; m < numCompMsInTs; ++m )
   {
      if (fuMsAcceptsPercent < m)
         continue;

      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, m);
      fiEquipmentId = msDescriptor.eq_id;
      fdMsIndex = static_cast<double>(msDescriptor.idx);
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>( ts.content( component, m) );

      uint32_t size = msDescriptor.size;
//    fulLastMsIdx = msDescriptor.idx;
      if (size > 0)
         LOG(debug) << "Microslice: " << msDescriptor.idx << " has size: " << size;

      if( numCompMsInTs - fuOverlapMsNb <= m )
      {
//         LOG(info) << "Ignore overlap Microslice: " << msDescriptor.idx;
         continue;
      } // if( numCompMsInTs - fuOverlapMsNb <= m )

      if( 0 == m )
         dTsStartTime = (1e-9) * fdMsIndex;

      if( fdStartTimeMsSz < 0 )
         fdStartTimeMsSz = (1e-9) * fdMsIndex;
      fvhMsSzPerLink[ component ]->Fill(size);
      if( 2 * fuHistoryHistoSize < (1e-9) * fdMsIndex - fdStartTimeMsSz )
      {
         // Reset the evolution Histogram and the start time when we reach the end of the range
         fvhMsSzTimePerLink[ component ]->Reset();
         fdStartTimeMsSz = (1e-9) * fdMsIndex;
      } // if( 2 * fuHistoryHistoSize < (1e-9) * fdMsIndex - fdStartTimeMsSz )
      fvhMsSzTimePerLink[ component ]->Fill((1e-9) * fdMsIndex - fdStartTimeMsSz, size);

      // If not integer number of message in input buffer, print warning/error
      if (0 != (size % kuBytesPerMessage))
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage)) / kuBytesPerMessage;

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>(msContent);
      for( uint32_t uIdx = 0; uIdx < uNbMessages; uIdx++ )
      {
         // Fill message
         uint64_t ulData = static_cast<uint64_t>(pInBuff[uIdx]);
         gdpb::Message mess(ulData);

         if (gLogger->IsLogNeeded( fair::Severity::debug2 ))
         {
            mess.printDataCout();
         } // if (gLogger->IsLogNeeded( fair::Severity::debug2 ))


         // Increment counter for different message types
         // and fill the corresponding histogram
         messageType = mess.getMessageType();
         fviMsgCounter[messageType]++;
         fhMessType->Fill(messageType);

         fuGdpbId = mess.getRocNumber();
         fuGdpbNr = fGdpbIdIndexMap[fuGdpbId];

         fuGet4Id = mess.getGdpbGenChipId();
         fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

         if( fuNrOfGet4PerGdpb <= fuGet4Id &&
             gdpb::MSG_STAR_TRI != messageType &&
             ( get4v2x::kuChipIdMergedEpoch != fuGet4Id ) )
            LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                         << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

         switch (messageType)
         {
            case gdpb::MSG_HIT:
            case gdpb::MSG_EPOCH:
               LOG(error) << "Message type " << messageType
                          << " not included in unpacker.";
               break;
            case gdpb::MSG_SYS:
               fhSysMessType->Fill(mess.getSysMesType());
               break;
            case gdpb::MSG_EPOCH2:
            {
               if( get4v2x::kuChipIdMergedEpoch == fuGet4Id )
               {
                  for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
                  {
                     fuGet4Id = uGet4Index;
                     fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;
                     gdpb::Message tmpMess(mess);
                     tmpMess.setGdpbGenChipId( uGet4Index );

                     fhGet4MessType->Fill(fuGet4Nr, gdpb::GET4_32B_EPOCH);
                     FillEpochInfo(tmpMess);
                  } // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )
               } // if this epoch message is a merged one valiud for all chips
               else
               {
                  fhGet4MessType->Fill(fuGet4Nr, gdpb::GET4_32B_EPOCH);
                  FillEpochInfo(mess);
               } // if single chip epoch message
               break;
            }
            case gdpb::MSG_GET4:
               fhGet4MessType->Fill(fuGet4Nr, gdpb::GET4_32B_DATA + 1);
               PrintGenInfo(mess);
               break;
            case gdpb::MSG_GET4_32B:
               fhGet4MessType->Fill(fuGet4Nr, gdpb::GET4_32B_DATA);
               fvmEpSupprBuffer[fuGet4Nr].push_back( mess );
               break;
            case gdpb::MSG_GET4_SLC:
               fhGet4MessType->Fill(fuGet4Nr, gdpb::GET4_32B_SLCM);
               PrintSlcInfo(mess);
               break;
            case gdpb::MSG_GET4_SYS:
            {
               fhSysMessType->Fill(mess.getGdpbSysSubType());
               if( gdpb::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType() )
               {
                  fhGet4MessType->Fill(fuGet4Nr, gdpb::GET4_32B_ERROR);

                  UInt_t uFeetNr   = (fuGet4Id / fuNrOfGet4PerFeb);
                  if (0 <= fdStartTime)
                  {
                     fvhFeetErrorRate_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill(
                        1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime));
                     fvhFeetErrorRatio_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill(
                        1e-9 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTime), 1, 1 );
                  } // if (0 <= fdStartTime)
                  if (0 <= fdStartTimeLong)
                  {
                     fvhFeetErrorRateLong_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill(
                        1e-9 / 60.0 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTimeLong), 1 / 60.0);
                     fvhFeetErrorRatioLong_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill(
                        1e-9 / 60.0 * (mess.getMsgFullTimeD(fvulCurrentEpoch[fuGet4Nr]) - fdStartTimeLong), 1, 1 / 60.0);
                  } // if (0 <= fdStartTime)

                  Int_t dFullChId =  fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSysErrChanId();
                  switch( mess.getGdpbSysErrData() )
                  {
                     case gdpb::GET4_V2X_ERR_READ_INIT:
                        fhGet4ChanErrors->Fill(dFullChId, 0);
                        break;
                     case gdpb::GET4_V2X_ERR_SYNC:
                        fhGet4ChanErrors->Fill(dFullChId, 1);
                        break;
                     case gdpb::GET4_V2X_ERR_EP_CNT_SYNC:
                        fhGet4ChanErrors->Fill(dFullChId, 2);
                        break;
                     case gdpb::GET4_V2X_ERR_EP:
                        fhGet4ChanErrors->Fill(dFullChId, 3);
                        break;
                     case gdpb::GET4_V2X_ERR_FIFO_WRITE:
                        fhGet4ChanErrors->Fill(dFullChId, 4);
                        break;
                     case gdpb::GET4_V2X_ERR_LOST_EVT:
                        fhGet4ChanErrors->Fill(dFullChId, 5);
                        break;
                     case gdpb::GET4_V2X_ERR_CHAN_STATE:
                        fhGet4ChanErrors->Fill(dFullChId, 6);
                        break;
                     case gdpb::GET4_V2X_ERR_TOK_RING_ST:
                        fhGet4ChanErrors->Fill(dFullChId, 7);
                        break;
                     case gdpb::GET4_V2X_ERR_TOKEN:
                        fhGet4ChanErrors->Fill(dFullChId, 8);
                        break;
                     case gdpb::GET4_V2X_ERR_READOUT_ERR:
                        fhGet4ChanErrors->Fill(dFullChId, 9);
                        break;
                     case gdpb::GET4_V2X_ERR_SPI:
                        fhGet4ChanErrors->Fill(dFullChId, 10);
                        break;
                     case gdpb::GET4_V2X_ERR_DLL_LOCK:
                        fhGet4ChanErrors->Fill(dFullChId, 11);
                        break;
                     case gdpb::GET4_V2X_ERR_DLL_RESET:
                        fhGet4ChanErrors->Fill(dFullChId, 12);
                        break;
                     case gdpb::GET4_V2X_ERR_TOT_OVERWRT:
                        fhGet4ChanErrors->Fill(dFullChId, 13);
                        break;
                     case gdpb::GET4_V2X_ERR_TOT_RANGE:
                        fhGet4ChanErrors->Fill(dFullChId, 14);
                        break;
                     case gdpb::GET4_V2X_ERR_EVT_DISCARD:
                        fhGet4ChanErrors->Fill(dFullChId, 15);
                        break;
                     case gdpb::GET4_V2X_ERR_ADD_RIS_EDG:
                        fhGet4ChanErrors->Fill(dFullChId, 16);
                        break;
                     case gdpb::GET4_V2X_ERR_UNPAIR_FALL:
                        fhGet4ChanErrors->Fill(dFullChId, 17);
                        break;
                     case gdpb::GET4_V2X_ERR_SEQUENCE_ER:
                        fhGet4ChanErrors->Fill(dFullChId, 18);
                        break;
                     case gdpb::GET4_V2X_ERR_UNKNOWN:
                        fhGet4ChanErrors->Fill(dFullChId, 19);
                        break;
                     default: // Corrupt error or not yet supported error
                        fhGet4ChanErrors->Fill(dFullChId, 20);
                        break;
                  } // Switch( mess.getGdpbSysErrData() )
               } // if( gdpb::SYSMSG_GET4_EVENT == mess.getGdpbSysSubType() )
               PrintSysInfo(mess);
               break;
            } // case gdpb::MSG_GET4_SYS:
            case gdpb::MSG_STAR_TRI:
               fhGet4MessType->Fill(fuGet4Nr, gdpb::MSG_STAR_TRI);
               FillStarTrigInfo(mess);
               break;
            default:
               LOG(error) << "Message type " << std::hex
                          << std::setw(2) << static_cast<uint16_t>(messageType)
                          << " not included in Get4 unpacker.";
         } // switch( mess.getMessageType() )
      } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
   } // for (size_t m = 0; m < ts.num_microslices(component); ++m)

   // Update RMS plots only every 10s in data
   if( 10.0 < dTsStartTime - fdLastRmsUpdateTime )
   {
      // Reset summary histograms for safety
      fhTimeRmsPulser->Reset();

      for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeA++)
         for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
            if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
            {
               fhTimeRmsPulser->Fill( uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS() );
            } // for( UInt_t uChan = 0; uChan < kuNbChanTest - 1; uChan++)
      fdLastRmsUpdateTime = dTsStartTime;
   } // if( 10.0 < dTsStartTime - fdLastRmsUpdateTime )

  return kTRUE;
}

void CbmTofStarMonitorShift2018::FillHitInfo(gdpb::Message mess)
{
   UInt_t uChannel = mess.getGdpbHitChanId();
   UInt_t uTot     = mess.getGdpbHit32Tot();
   UInt_t uFts     = mess.getGdpbHitFineTs();

   ULong64_t ulCurEpochGdpbGet4 = fvulCurrentEpoch[ fuGet4Nr ];

   // In Ep. Suppr. Mode, receive following epoch instead of previous
   if( 0 < ulCurEpochGdpbGet4 )
      ulCurEpochGdpbGet4 --;
      else ulCurEpochGdpbGet4 = get4v2x::kuEpochCounterSz; // Catch epoch cycle!

   UInt_t uChannelNr = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
   UInt_t uChannelNrInFeet = (fuGet4Id % fuNrOfGet4PerFeb) * fuNrOfChannelsPerGet4 + uChannel;
   UInt_t uFeetNr   = (fuGet4Id / fuNrOfGet4PerFeb);
   UInt_t uFeetNrInSys = fuGdpbNr * fuNrOfFeetPerGdpb + uFeetNr;
   UInt_t uRemappedChannelNr = uFeetNr * fuNrOfChannelsPerFeet + fvuGet4ToPadi[ uChannelNrInFeet ];

//   ULong_t  ulHitTime = mess.getMsgFullTime(ulCurEpochGdpbGet4);
   Double_t dHitTime  = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);

   // In 32b mode the coarse counter is already computed back to 112 FTS bins
   // => need to hide its contribution from the Finetime
   // => FTS = Fullt TS modulo 112
   uFts = mess.getGdpbHitFullTs() % 112;

   fvhChCount_gDPB[fuGdpbNr]->Fill(uChannelNr);
   fvhRawFt_gDPB[fuGdpbNr]->Fill(uChannelNr, uFts);
   fvhRawTot_gDPB[ fuGdpbNr * fuNbFeetPlotsPerGdpb + uFeetNr/fuNbFeetPlot ]->Fill(uChannelNr, uTot);

   /// Remapped for PADI to GET4
   fvhRemapChCount_gDPB[fuGdpbNr]->Fill( uRemappedChannelNr );
   fvhRemapTot_gDPB[ fuGdpbNr * fuNbFeetPlotsPerGdpb + uFeetNr/fuNbFeetPlot ]->Fill(  uRemappedChannelNr , uTot);

   ///* Pulser monitoring *///
   /// Save last hist time if pulser channel
   /// Fill the corresponding histos if the time difference is reasonnable
   if( get4v2x::kuFeePulserChannel == uChannelNrInFeet )
   {
      fdTsLastPulserHit[ uFeetNrInSys ] = dHitTime;

      /// Update the difference to all other FEE with lower indices
      for( UInt_t uFeeB = 0; uFeeB < uFeetNrInSys; uFeeB++)
         if( NULL != fvhTimeDiffPulser[uFeeB][uFeetNrInSys] )
         {
            Double_t dTimeDiff = 1e3 * ( fdTsLastPulserHit[ uFeetNrInSys ] - fdTsLastPulserHit[ uFeeB ] );
            if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
               fvhTimeDiffPulser[uFeeB][uFeetNrInSys]->Fill( dTimeDiff );
         } // if( NULL != fvhTimeDiffPulser[uFeeB][uFeeB] )

      /// Update the difference to all other FEE with higher indices
      for( UInt_t uFeeB = uFeetNrInSys + 1; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
         if( NULL != fvhTimeDiffPulser[uFeetNrInSys][uFeeB] )
         {
            Double_t dTimeDiff = 1e3 * ( fdTsLastPulserHit[ uFeeB ] - fdTsLastPulserHit[ uFeetNrInSys ] );
            if( TMath::Abs( dTimeDiff ) < kdMaxDtPulserPs )
               fvhTimeDiffPulser[uFeetNrInSys][uFeeB]->Fill( dTimeDiff );
         } // if( NULL != fvhTimeDiffPulser[uFeetNrInSys][uFeeB] )
   } // if( get4v2x::kuFeePulserChannel == uChannelNrInFeet )

   // In Run rate evolution
   if (fdStartTime < 0)
      fdStartTime = dHitTime;

   // Reset the evolution Histogram and the start time when we reach the end of the range
   if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) )
   {
      ResetEvolutionHistograms();
      fdStartTime = dHitTime;
   } // if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) )

   // In Run rate evolution
   if (fdStartTimeLong < 0)
      fdStartTimeLong = dHitTime;

   // Reset the evolution Histogram and the start time when we reach the end of the range
   if( fuHistoryHistoSizeLong < 1e-9 * (dHitTime - fdStartTimeLong) / 60.0 )
   {
      ResetLongEvolutionHistograms();
      fdStartTimeLong = dHitTime;
   } // if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) / 60.0 )

   if (0 <= fdStartTime)
   {
      fvhChannelRate_gDPB[ fuGdpbNr ]->Fill( 1e-9 * (dHitTime - fdStartTime), uChannelNr );
      fvhRemapChRate_gDPB[ fuGdpbNr ]->Fill( 1e-9 * (dHitTime - fdStartTime), uRemappedChannelNr );
      fvhFeetRate_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill( 1e-9 * (dHitTime - fdStartTime));
      fvhFeetErrorRatio_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill( 1e-9 * (dHitTime - fdStartTime), 0, 1);
   } // if (0 <= fdStartTime)

   if (0 <= fdStartTimeLong)
   {
      fvhFeetRateLong_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill(
            1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 1 / 60.0 );
      fvhFeetErrorRatioLong_gDPB[(fuGdpbNr * fuNrOfFeetPerGdpb) + uFeetNr]->Fill(
            1e-9 / 60.0 * (dHitTime - fdStartTimeLong), 0, 1 / 60.0 );
   } // if (0 <= fdStartTimeLong)
}

void CbmTofStarMonitorShift2018::FillEpochInfo(gdpb::Message mess)
{
   ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();

   fvulCurrentEpoch[fuGet4Nr] = ulEpochNr;

   if (1 == mess.getGdpbEpSync())
      fhGet4EpochFlags->Fill(fuGet4Nr, 0);
   if (1 == mess.getGdpbEpDataLoss())
      fhGet4EpochFlags->Fill(fuGet4Nr, 1);
   if (1 == mess.getGdpbEpEpochLoss())
      fhGet4EpochFlags->Fill(fuGet4Nr, 2);
   if (1 == mess.getGdpbEpMissmatch())
      fhGet4EpochFlags->Fill(fuGet4Nr, 3);

   fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);

   /// Re-align the epoch number of the message in case it will be used later:
   /// We received the epoch after the data instead of the one before!
   if( 0 < ulEpochNr )
      mess.setEpoch2Number( ulEpochNr - 1 );
      else mess.setEpoch2Number( get4v2x::kuEpochCounterSz );

   Int_t iBufferSize = fvmEpSupprBuffer[fuGet4Nr].size();
   if( 0 < iBufferSize )
   {
      LOG(debug) << "Now processing stored messages for for get4 " << fuGet4Nr << " with epoch number "
                 << (fvulCurrentEpoch[fuGet4Nr] - 1);

      /// Data are sorted between epochs, not inside => Epoch level ordering
      /// Sorting at lower bin precision level
      std::stable_sort( fvmEpSupprBuffer[fuGet4Nr].begin(), fvmEpSupprBuffer[fuGet4Nr].begin() );

      for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
      {
         FillHitInfo( fvmEpSupprBuffer[fuGet4Nr][ iMsgIdx ] );
      } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

      fvmEpSupprBuffer[fuGet4Nr].clear();
   } // if( 0 < fvmEpSupprBuffer[fuGet4Nr] )
}

void CbmTofStarMonitorShift2018::PrintSlcInfo(gdpb::Message mess)
{
   if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId))
   {
//      UInt_t uChip = mess.getGdpbGenChipId();
//      UInt_t uChan = mess.getGdpbSlcChan();
//      UInt_t uEdge = mess.getGdpbSlcEdge();
      UInt_t uData = mess.getGdpbSlcData();
//      UInt_t uCRC  = mess.getGdpbSlcCrc();
      UInt_t uType = mess.getGdpbSlcType();
      Double_t dFullChId =  fuGet4Nr * fuNrOfChannelsPerGet4 + mess.getGdpbSlcChan() + 0.5 * mess.getGdpbSlcEdge();
      Double_t dMessTime = static_cast< Double_t>( fulCurrentEpochTime ) * 1.e-9;

      switch( uType )
      {
         case 0: // Scaler counter
         {
            fhGet4ChanScm->Fill(dFullChId, uType );
            fhScmScalerCounters->Fill( uData, dFullChId);
            break;
         }
         case 1: // Deadtime counter
         {
            fhGet4ChanScm->Fill(dFullChId, uType );
            fhScmDeadtimeCounters->Fill( uData, dFullChId);
            break;
         }
         case 2: // SPI message
         {
/*
            LOG(info) << "GET4 Slow Control message, epoch "
                       << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                       << std::fixed << dMessTime << " s "
                       << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                       << std::dec << "\n"
                       << " +++++++ > Chip = "
                       << std::setw(2) << uChip << ", Chan = "
                       << std::setw(1) << uChan << ", Edge = "
                       << std::setw(1) << uEdge << ", Type = "
                       << std::setw(1) << mess.getGdpbSlcType() << ", Data = 0x"
//                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                       << Form( "%06x", uData )
                       << std::dec << ", CRC = " << uCRC
//                 << " RAW: " << Form( "%08x", mess.getGdpbSlcMess() );
*/
            fhGet4ChanScm->Fill(dFullChId, uType );
            break;
         }
         case 3: // Start message or SEU counter
         {
            if( 0 == mess.getGdpbSlcChan() && 0 == mess.getGdpbSlcEdge() ) // START message
            {
/*
               LOG(info) << std::setprecision(9)
                             << std::fixed << dMessTime << " s ";
            LOG(info) << "GET4 Slow Control message, epoch "
                    << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                    << std::fixed << dMessTime << " s "
                    << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                    << std::dec << "\n"
                    << " +++++++ > Chip = "
                    << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = "
                    << std::setw(1) << mess.getGdpbSlcChan() << ", Edge = "
                    << std::setw(1) << mess.getGdpbSlcEdge() << ", Type = "
                    << std::setw(1) << mess.getGdpbSlcType() << ", Data = 0x"
   //                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                    << Form( "%06x", mess.getGdpbSlcData() )
                    << std::dec << ", CRC = " << mess.getGdpbSlcCrc();
*/
               fhGet4ChanScm->Fill(dFullChId, uType + 1);
            } // if( 0 == mess.getGdpbSlcChan() && 0 == mess.getGdpbSlcEdge() )
            else if( 0 == mess.getGdpbSlcChan() && 1 == mess.getGdpbSlcEdge() ) // SEU counter message
            {
/*
         LOG(info) << "GET4 Slow Control message, epoch "
                 << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                 << std::fixed << dMessTime << " s "
                 << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                 << std::dec << "\n"
                 << " +++++++ > Chip = "
                 << std::setw(2) << mess.getGdpbGenChipId() << ", Chan = "
                 << std::setw(1) << mess.getGdpbSlcChan() << ", Edge = "
                 << std::setw(1) << mess.getGdpbSlcEdge() << ", Type = "
                 << std::setw(1) << mess.getGdpbSlcType() << ", Data = 0x"
//                 << std::hex << std::setw(6) << mess.getGdpbSlcData()
                 << Form( "%06x", mess.getGdpbSlcData() )
                 << std::dec << ", CRC = " << mess.getGdpbSlcCrc();
*/
               fhGet4ChanScm->Fill(dFullChId, uType );
               fhScmSeuCounters->Fill( uData, dFullChId);
               fhScmSeuCountersEvo->Fill( dMessTime - fdStartTime* 1.e-9, uData, dFullChId);
             } // else if( 0 == mess.getGdpbSlcChan() && 1 == mess.getGdpbSlcEdge() )
            break;
         }
         default: // Should never happen
            break;
      } // switch( mess.getGdpbSlcType() )
   }
}

void CbmTofStarMonitorShift2018::PrintGenInfo(gdpb::Message mess)
{
   Int_t mType = mess.getMessageType();
   Int_t channel = mess.getGdpbHitChanId();
   uint64_t uData = mess.getData();

   LOG(debug) << "Get4 MSG type " << mType << " from gdpbId " << fuGdpbId
              << ", getId " << fuGet4Id << ", (hit channel) " << channel
              << " data " << std::hex << uData;
}

void CbmTofStarMonitorShift2018::PrintSysInfo(gdpb::Message mess)
{
   if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId))
      LOG(debug) << "GET4 System message,       epoch "
                 << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                 << std::fixed << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                 << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                 << std::dec;

   switch( mess.getGdpbSysSubType() )
   {
      case gdpb::SYSMSG_GET4_EVENT:
      {
         uint32_t uData = mess.getGdpbSysErrData();
         if( gdpb::GET4_V2X_ERR_TOT_OVERWRT == uData
          || gdpb::GET4_V2X_ERR_TOT_RANGE   == uData
          || gdpb::GET4_V2X_ERR_EVT_DISCARD == uData
          || gdpb::GET4_V2X_ERR_ADD_RIS_EDG == uData
          || gdpb::GET4_V2X_ERR_UNPAIR_FALL == uData
          || gdpb::GET4_V2X_ERR_SEQUENCE_ER == uData
           )
            LOG(debug) << " +++++++ > gDPB: " << std::hex << std::setw(4) << fuGdpbId
                       << std::dec << ", Chip = " << std::setw(2)
                       << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                       << mess.getGdpbSysErrChanId() << ", Edge = "
                       << std::setw(1) << mess.getGdpbSysErrEdge() << ", Empt = "
                       << std::setw(1) << mess.getGdpbSysErrUnused()
                       << ", Data = " << std::hex << std::setw(2) << uData
                       << std::dec << " -- GET4 V1 Error Event";
            else LOG(debug) << " +++++++ >gDPB: " << std::hex << std::setw(4) << fuGdpbId
                            << std::dec << ", Chip = " << std::setw(2)
                            << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                            << mess.getGdpbSysErrChanId() << ", Edge = "
                            << std::setw(1) << mess.getGdpbSysErrEdge() << ", Empt = "
                            << std::setw(1) << mess.getGdpbSysErrUnused()
                            << ", Data = " << std::hex << std::setw(2) << uData
                            << std::dec << " -- GET4 V1 Error Event ";
         break;
      } // case gdpb::SYSMSG_GET4_EVENT
      case gdpb::SYSMSG_CLOSYSYNC_ERROR:
         LOG(debug) << "Closy synchronization error";
         break;
      case gdpb::SYSMSG_TS156_SYNC:
         LOG(debug) << "160.00 MHz timestamp reset";
         break;
      case gdpb::SYSMSG_GDPB_UNKWN:
         LOG(debug) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                    << mess.getGdpbSysUnkwData() << std::dec
                    <<" Full message: " << std::hex << std::setw(16)
                    << mess.getData() << std::dec;
         break;
   } // switch( getGdpbSysSubType() )
}

void CbmTofStarMonitorShift2018::FillStarTrigInfo(gdpb::Message mess)
{
   Int_t iMsgIndex = mess.getStarTrigMsgIndex();

   switch( iMsgIndex )
   {
      case 0:
         fvhTokenMsgType[fuGdpbNr]->Fill(0);
         fvulGdpbTsMsb[fuGdpbNr] = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fvhTokenMsgType[fuGdpbNr]->Fill(1);
         fvulGdpbTsLsb[fuGdpbNr] = mess.getGdpbTsLsbStarB();
         fvulStarTsMsb[fuGdpbNr] = mess.getStarTsMsbStarB();
         break;
      case 2:
         fvhTokenMsgType[fuGdpbNr]->Fill(2);
         fvulStarTsMid[fuGdpbNr] = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         fvhTokenMsgType[fuGdpbNr]->Fill(3);

         ULong64_t ulNewGdpbTsFull = ( fvulGdpbTsMsb[fuGdpbNr] << 24 )
                                   + ( fvulGdpbTsLsb[fuGdpbNr]       );
         ULong64_t ulNewStarTsFull = ( fvulStarTsMsb[fuGdpbNr] << 48 )
                                   + ( fvulStarTsMid[fuGdpbNr] <<  8 )
                                   + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

         if( ( uNewToken == fvuStarTokenLast[fuGdpbNr] ) && ( ulNewGdpbTsFull == fvulGdpbTsFullLast[fuGdpbNr] ) &&
             ( ulNewStarTsFull == fvulStarTsFullLast[fuGdpbNr] ) && ( uNewDaqCmd == fvuStarDaqCmdLast[fuGdpbNr] ) &&
             ( uNewTrigCmd == fvuStarTrigCmdLast[fuGdpbNr] ) )
         {
            UInt_t uTrigWord =  ( (fvuStarTrigCmdLast[fuGdpbNr] & 0x00F) << 16 )
                     + ( (fvuStarDaqCmdLast[fuGdpbNr]   & 0x00F) << 12 )
                     + ( (fvuStarTokenLast[fuGdpbNr]    & 0xFFF)       );
            LOG(warning) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << " TS " << fulCurrentTsIndex
                         << " gDBB #" << fuGdpbNr << " "
                         << Form("token = %5u ", fvuStarTokenLast[fuGdpbNr] )
                         << Form("gDPB ts  = %12llu ", fvulGdpbTsFullLast[fuGdpbNr] )
                         << Form("STAR ts = %12llu ", fvulStarTsFullLast[fuGdpbNr] )
                         << Form("DAQ cmd = %2u ", fvuStarDaqCmdLast[fuGdpbNr] )
                         << Form("TRG cmd = %2u ", fvuStarTrigCmdLast[fuGdpbNr] )
                         << Form("TRG Wrd = %5x ", uTrigWord );
            return;
         } // if exactly same message repeated
/*
         if( (uNewToken != fuStarTokenLast[fuGdpbNr] + 1) &&
             0 < fvulGdpbTsFullLast[fuGdpbNr] && 0 < fvulStarTsFullLast[fuGdpbNr] &&
             ( 4095 != fvuStarTokenLast[fuGdpbNr] || 1 != uNewToken)  )
            LOG(warning) << "Possible error: STAR token did not increase by exactly 1! "
                         << " gDBB #" << fuGdpbNr << " "
                         << Form("old = %5u vs new = %5u ", fvuStarTokenLast[fuGdpbNr],   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fvulGdpbTsFullLast[fuGdpbNr], ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fvulStarTsFullLast[fuGdpbNr], ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fvuStarDaqCmdLast[fuGdpbNr],  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fvuStarTrigCmdLast[fuGdpbNr], uNewTrigCmd);
*/
         // STAR TS counter reset detection
         if( ulNewStarTsFull < fvulStarTsFullLast[fuGdpbNr] )
            LOG(debug) << "Probable reset of the STAR TS: old = " << Form("%16llu", fvulStarTsFullLast[fuGdpbNr])
                       << " new = " << Form("%16llu", ulNewStarTsFull)
                       << " Diff = -" << Form("%8llu", fvulStarTsFullLast[fuGdpbNr] - ulNewStarTsFull);

/*
         LOG(info) << "Updating  trigger token for " << fuGdpbNr
                   << " " << fuStarTokenLast[fuGdpbNr] << " " << uNewToken;
*/
//         ULong64_t ulGdpbTsDiff = ulNewGdpbTsFull - fvulGdpbTsFullLast[fuGdpbNr];
         fvulGdpbTsFullLast[fuGdpbNr] = ulNewGdpbTsFull;
         fvulStarTsFullLast[fuGdpbNr] = ulNewStarTsFull;
         fvuStarTokenLast[fuGdpbNr]   = uNewToken;
         fvuStarDaqCmdLast[fuGdpbNr]  = uNewDaqCmd;
         fvuStarTrigCmdLast[fuGdpbNr] = uNewTrigCmd;

         /// Histograms filling only in core MS
         if( fuCurrentMs < fuCoreMs  )
         {
            /// In Run rate evolution
            if( 0 <= fdStartTime )
            {
               /// Reset the evolution Histogram and the start time when we reach the end of the range
               if( fuHistoryHistoSize < 1e-9 * (fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime) )
               {
                  ResetEvolutionHistograms();
                  fdStartTime = fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs;
               } // if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast * get4v2x::kdClockCycleSizeNs - fdStartTime) )

               fvhTriggerRate[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ) );
               fvhStarTokenEvo[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ),
                                               fvuStarTokenLast[fuGdpbNr] );
               fvhStarTrigGdpbTsEvo[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ),
                                                     fvulGdpbTsFullLast[fuGdpbNr] );
               fvhStarTrigStarTsEvo[fuGdpbNr]->Fill( 1e-9 * ( fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ),
                                                     fvulStarTsFullLast[fuGdpbNr] );
            } // if( 0 < fdStartTime )
               else fdStartTime = fvulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs;
            fvhCmdDaqVsTrig[fuGdpbNr]->Fill( fvuStarDaqCmdLast[fuGdpbNr], fvuStarTrigCmdLast[fuGdpbNr] );
         } // if( fuCurrentMs < fuCoreMs  )

         break;
      } // case 3
      default:
         LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
   } // switch( iMsgIndex )
}

void CbmTofStarMonitorShift2018::Reset()
{
}

void CbmTofStarMonitorShift2018::Finish()
{
   // Printout some stats on what was unpacked
   TString message_type;
   for( unsigned int i = 0; i < fviMsgCounter.size(); ++i)
   {
      switch (i)
      {
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
      LOG(info) << message_type << " messages: " << fviMsgCounter[i];
   } // for (unsigned int i=0; i< fviMsgCounter.size(); ++i)

   LOG(info) << "-------------------------------------";
   for (UInt_t i = 0; i < fuNrOfGdpbs; ++i)
   {
      for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j)
      {
         LOG(info) << "Last epoch for gDPB: " << std::hex << std::setw(4) << i
                   << std::dec << " , GET4  " << std::setw(4) << j << " => "
                   << fvulCurrentEpoch[GetArrayIndex(i, j)];
      } // for (UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j)
   } // for (UInt_t i = 0; i < fuNrOfGdpbs; ++i)
   LOG(info) << "-------------------------------------";

   SaveAllHistos();
}

void CbmTofStarMonitorShift2018::FillOutput(boost::any)
{
}

void CbmTofStarMonitorShift2018::SaveAllHistos( TString sFileName )
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

   fhMessType->Write();
   fhSysMessType->Write();
   fhGet4MessType->Write();
   fhGet4ChanScm->Write();
   fhGet4ChanErrors->Write();
   fhGet4EpochFlags->Write();
   fhScmScalerCounters->Write();
   fhScmDeadtimeCounters->Write();
   fhScmSeuCounters->Write();
   fhScmSeuCountersEvo->Write();

   for( UInt_t uTotPlot = 0; uTotPlot < fvhRawTot_gDPB.size(); ++uTotPlot )
      fvhRawTot_gDPB[ uTotPlot ]->Write();

   for( UInt_t uTotPlot = 0; uTotPlot < fvhRemapTot_gDPB.size(); ++uTotPlot )
      fvhRemapTot_gDPB[ uTotPlot ]->Write();

   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fvhRawFt_gDPB[ uGdpb ]->Write();
      fvhChCount_gDPB[ uGdpb ]->Write();
      fvhChannelRate_gDPB[ uGdpb ]->Write();
      fvhRemapChCount_gDPB[ uGdpb ]->Write();
      fvhRemapChRate_gDPB[ uGdpb ]->Write();

      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)
      {
         fvhFeetRate_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Write();
         fvhFeetErrorRate_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Write();
         fvhFeetErrorRatio_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Write();
         fvhFeetRateLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Write();
         fvhFeetErrorRateLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Write();
         fvhFeetErrorRatioLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Write();
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)

   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   fhTimeRmsPulser->Write();
   fhTimeRmsZoomFitPuls->Write();
   fhTimeResFitPuls->Write();
   gDirectory->cd("..");

   ///* STAR event building/cutting *///
   gDirectory->mkdir("Star_Raw");
   gDirectory->cd("Star_Raw");
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fvhTokenMsgType[ uGdpb ]->Write();
      fvhTriggerRate[ uGdpb ]->Write();
      fvhCmdDaqVsTrig[ uGdpb ]->Write();
      fvhStarTokenEvo[ uGdpb ]->Write();
      fvhStarTrigGdpbTsEvo[ uGdpb ]->Write();
      fvhStarTrigStarTsEvo[ uGdpb ]->Write();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   gDirectory->cd("..");

   ///* Pulser monitoring *///
   gDirectory->mkdir("TofDt");
   gDirectory->cd("TofDt");
   for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeA++)
      for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
         if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
            fvhTimeDiffPulser[uFeeA][uFeeB]->Write();
   gDirectory->cd("..");


   gDirectory->mkdir("Flib_Raw");
   gDirectory->cd("Flib_Raw");
   for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )
   {
      if( NULL == fvhMsSzPerLink[ uLinks ] )
         continue;

      fvhMsSzPerLink[ uLinks ]->Write();
      fvhMsSzTimePerLink[ uLinks ]->Write();
   } // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

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
  if( "" != sFileName )
  {
     // Restore original directory position
     histoFile->Close();
     oldDir->cd();
  } // if( "" != sFileName )

}

void CbmTofStarMonitorShift2018::ResetAllHistos()
{
   LOG(info) << "Reseting all TOF histograms.";

   fhMessType->Reset();
   fhSysMessType->Reset();
   fhGet4MessType->Reset();
   fhGet4ChanScm->Reset();
   fhGet4ChanErrors->Reset();
   fhGet4EpochFlags->Reset();
   fhScmScalerCounters->Reset();
   fhScmDeadtimeCounters->Reset();
   fhScmSeuCounters->Reset();
   fhScmSeuCountersEvo->Reset();

   for( UInt_t uTotPlot = 0; uTotPlot < fvhRawTot_gDPB.size(); ++uTotPlot )
      fvhRawTot_gDPB[ uTotPlot ]->Reset();

   for( UInt_t uTotPlot = 0; uTotPlot < fvhRemapTot_gDPB.size(); ++uTotPlot )
      fvhRemapTot_gDPB[ uTotPlot ]->Reset();

   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fvhRawFt_gDPB[ uGdpb ]->Reset();
      fvhChCount_gDPB[ uGdpb ]->Reset();
      fvhChannelRate_gDPB[ uGdpb ]->Reset();
      fvhRemapChCount_gDPB[ uGdpb ]->Reset();
      fvhRemapChRate_gDPB[ uGdpb ]->Reset();

      for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)
      {
         fvhFeetRate_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Reset();
         fvhFeetErrorRate_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Reset();
         fvhFeetErrorRatio_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Reset();
         fvhFeetRateLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Reset();
         fvhFeetErrorRateLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Reset();
         fvhFeetErrorRatioLong_gDPB[ uGdpb * fuNrOfFeetPerGdpb + uFeet ]->Reset();
      } // for (UInt_t uFeet = 0; uFeet < fuNrOfFeetPerGdpb; ++ uFeet)

   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   fhTimeRmsPulser->Reset();
   fhTimeRmsZoomFitPuls->Reset();
   fhTimeResFitPuls->Reset();

   ///* STAR event building/cutting *///
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fvhTokenMsgType[ uGdpb ]->Reset();
      fvhTriggerRate[ uGdpb ]->Reset();
      fvhCmdDaqVsTrig[ uGdpb ]->Reset();
      fvhStarTokenEvo[ uGdpb ]->Reset();
      fvhStarTrigGdpbTsEvo[ uGdpb ]->Reset();
      fvhStarTrigStarTsEvo[ uGdpb ]->Reset();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

   ///* Pulser monitoring *///
   for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeA++)
      for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
         if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
            fvhTimeDiffPulser[uFeeA][uFeeB]->Reset();

   for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )
   {
      if( NULL == fvhMsSzPerLink[ uLinks ] )
         continue;

      fvhMsSzPerLink[ uLinks ]->Reset();
      fvhMsSzTimePerLink[ uLinks ]->Reset();
   } // for( UInt_t uLinks = 0; uLinks < fvhMsSzPerLink.size(); uLinks++ )

   fdStartTime = -1;
   fdStartTimeLong = -1;
   fdStartTimeMsSz = -1;
}
void CbmTofStarMonitorShift2018::ResetEvolutionHistograms()
{
   for( UInt_t uGdpbLoop = 0; uGdpbLoop < fuNrOfGdpbs; ++uGdpbLoop )
   {
      fvhChannelRate_gDPB[ uGdpbLoop ]->Reset();
      fvhRemapChRate_gDPB[ uGdpbLoop ]->Reset();
      for( UInt_t uFeetLoop = 0; uFeetLoop < fuNrOfFeetPerGdpb; ++uFeetLoop )
      {
         fvhFeetRate_gDPB[(uGdpbLoop * fuNrOfFeetPerGdpb) + uFeetLoop]->Reset();
         fvhFeetErrorRate_gDPB[(uGdpbLoop * fuNrOfFeetPerGdpb) + uFeetLoop]->Reset();
         fvhFeetErrorRatio_gDPB[(uGdpbLoop * fuNrOfFeetPerGdpb) + uFeetLoop]->Reset();
      } // for( UInt_t uFeetLoop = 0; uFeetLoop < fuNrOfFeetPerGdpb; ++uFeetLoop )
      fvhTriggerRate[ uGdpbLoop ]->Reset();
      fvhStarTokenEvo[ uGdpbLoop ]->Reset();
      fvhStarTrigGdpbTsEvo[ uGdpbLoop ]->Reset();
      fvhStarTrigStarTsEvo[ uGdpbLoop ]->Reset();
   } // for( UInt_t uGdpbLoop = 0; uGdpbLoop < fuNrOfGdpbs; ++uGdpbLoop )

   fdStartTime = -1;
}
void CbmTofStarMonitorShift2018::ResetLongEvolutionHistograms()
{
   for (UInt_t uGdpbLoop = 0; uGdpbLoop < fuNrOfGdpbs; uGdpbLoop++)
   {
      for (UInt_t uFeetLoop = 0; uFeetLoop < fuNrOfFeetPerGdpb; uFeetLoop++)
      {
         fvhFeetRateLong_gDPB[(uGdpbLoop * fuNrOfFeetPerGdpb) + uFeetLoop]->Reset();
         fvhFeetErrorRateLong_gDPB[(uGdpbLoop * fuNrOfFeetPerGdpb) + uFeetLoop]->Reset();
         fvhFeetErrorRatioLong_gDPB[(uGdpbLoop * fuNrOfFeetPerGdpb) + uFeetLoop]->Reset();
      } // for (UInt_t uFeetLoop = 0; uFeetLoop < fuNrOfFeetPerGdpb; uFeetLoop++)
   } // for (UInt_t uFeetLoop = 0; uFeetLoop < fuNrOfFeetPerGdpb; uFeetLoop++)

   fdStartTimeLong = -1;
}

void CbmTofStarMonitorShift2018::UpdateZoomedFit()
{
   // Only do something is the user defined the width he want for the zoom
   if( 0.0 < fdFitZoomWidthPs )
   {
      // Reset summary histograms for safety
      fhTimeRmsZoomFitPuls->Reset();
      fhTimeResFitPuls->Reset();

      Double_t dRes = 0;
      TF1 *fitFuncPairs[ fuNrOfFeetPerGdpb * fuNrOfGdpbs ][ fuNrOfFeetPerGdpb * fuNrOfGdpbs ];

      for( UInt_t uFeeA = 0; uFeeA < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeA++)
         for( UInt_t uFeeB = 0; uFeeB < fuNrOfFeetPerGdpb * fuNrOfGdpbs; uFeeB++)
            if( NULL != fvhTimeDiffPulser[uFeeA][uFeeB] )
      {
         // Check that we have at least 1 entry
         if( 0 == fvhTimeDiffPulser[uFeeA][uFeeB]->GetEntries() )
         {
            fhTimeRmsZoomFitPuls->Fill( uFeeA, uFeeB, 0.0 );
            fhTimeResFitPuls->Fill( uFeeA, uFeeB, 0.0 );
            LOG(info) << "CbmTofStarMonitorShift2018::UpdateZoomedFit => Empty input "
                         << "for FEE pair " << uFeeA << " and " << uFeeB << " !!! ";
            continue;
         } // if( 0 == fvhTimeDiffPulser[uFeeA][uFeeB]->GetEntries() )

         // Read the peak position (bin with max counts) + total nb of entries
         Int_t    iBinWithMax = fvhTimeDiffPulser[uFeeA][uFeeB]->GetMaximumBin();
         Double_t dNbCounts   = fvhTimeDiffPulser[uFeeA][uFeeB]->Integral();

         // Zoom the X axis to +/- ZoomWidth around the peak position
         Double_t dPeakPos = fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->GetBinCenter( iBinWithMax );
         fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->SetRangeUser( dPeakPos - fdFitZoomWidthPs,
                                                                    dPeakPos + fdFitZoomWidthPs );

         // Read integral and check how much we lost due to the zoom (% loss allowed)
         Double_t dZoomCounts = fvhTimeDiffPulser[uFeeA][uFeeB]->Integral();
         if( ( dZoomCounts / dNbCounts ) < 0.99 )
         {
            fhTimeRmsZoomFitPuls->Fill( uFeeA, uFeeB, 0.0 );
            fhTimeResFitPuls->Fill( uFeeA, uFeeB, 0.0 );
            LOG(warning) << "CbmTofStarMonitorShift2018::UpdateZoomedFit => Zoom too strong, "
                         << "more than 1% loss for FEE pair " << uFeeA << " and " << uFeeB << " !!! ";
            continue;
         } // if( ( dZoomCounts / dNbCounts ) < 0.99 )

         // Fill new RMS after zoom into summary histo
         fhTimeRmsZoomFitPuls->Fill( uFeeA, uFeeB, fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS() );


         // Fit using zoomed boundaries + starting gaussian width, store into summary histo
         dRes = 0;
         fitFuncPairs[uFeeA][uFeeB] = new TF1( Form("fPair_%02d_%02d", uFeeA, uFeeB ), "gaus",
                                        dPeakPos - fdFitZoomWidthPs ,
                                        dPeakPos + fdFitZoomWidthPs);
         // Fix the Mean fit value around the Histogram Mean
         fitFuncPairs[uFeeA][uFeeB]->SetParameter( 0, dZoomCounts );
         fitFuncPairs[uFeeA][uFeeB]->SetParameter( 1, dPeakPos );
         fitFuncPairs[uFeeA][uFeeB]->SetParameter( 2, 200.0 ); // Hardcode start with ~4*BinWidth, do better later
         // Using integral instead of bin center seems to lead to unrealistic values => no "I"
         fvhTimeDiffPulser[uFeeA][uFeeB]->Fit( Form("fPair_%02d_%02d", uFeeA, uFeeB ), "QRM0");
         // Get Sigma
         dRes = fitFuncPairs[uFeeA][uFeeB]->GetParameter(2);
         // Cleanup memory
         delete fitFuncPairs[uFeeA][uFeeB];
         // Fill summary
         fhTimeResFitPuls->Fill( uFeeA, uFeeB,  dRes / TMath::Sqrt2() );


         LOG(info) << "CbmTofStarMonitorShift2018::UpdateZoomedFit => "
                      << "For FEE pair " << uFeeA << " and " << uFeeB
                      << " we have zoomed RMS = " << fvhTimeDiffPulser[uFeeA][uFeeB]->GetRMS()
                      << " and a resolution of " << dRes / TMath::Sqrt2();

         // Restore original axis state?
         fvhTimeDiffPulser[uFeeA][uFeeB]->GetXaxis()->UnZoom();
      } // loop on uFeeA and uFeeB + check if corresponding fvhTimeDiffPulser exists
   } // if( 0.0 < fdFitZoomWidthPs )
      else
      {
         LOG(error) << "CbmTofStarMonitorShift2018::UpdateZoomedFit => Zoom width not defined, "
                    << "please use SetFitZoomWidthPs, e.g. in macro, before trying this update !!!";
      } // else of if( 0.0 < fdFitZoomWidthPs )
}

ClassImp(CbmTofStarMonitorShift2018)
