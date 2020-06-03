// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTofStarEventBuilder2018                 -----
// -----               Created 08.12.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTofStarEventBuilder2018.h"
#include "CbmTofStar2018Par.h"
//#include "CbmTbEvent.h"

#include "CbmFormatDecHexPrintout.h"

#include "CbmTbDaqBuffer.h"

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
#include "TProfile.h"
#include <TFile.h>

#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <fstream>

static Int_t iMess=0;
// const Int_t DetMask = 0x0001FFFF; (VF) unused


CbmTofStarEventBuilder2018::CbmTofStarEventBuilder2018( UInt_t uNbGdpb )
  : CbmTSUnpack(),
    fuMsAcceptsPercent(100),
    fuTotalMsNb(0),
    fuOverlapMsNb(0),
    fuCoreMs(0),
    fdMsSizeInNs(0.0),
    fdTsCoreSizeInNs(0.0),
    fuMinNbGdpb( uNbGdpb ),
    fuCurrNbGdpb( 0 ),
    fuNrOfGdpbs(0),
    fuNrOfFebsPerGdpb(0),
    fuNrOfGet4PerFeb(0),
    fuNrOfChannelsPerGet4(0),
    fuNrOfChannelsPerFeet(0),
    fuNrOfGet4(0),
    fuNrOfGet4PerGdpb(0),
    fuNrOfChannelsPerGdpb(0),
    fMsgCounter(11,0), // length of enum MessageTypes initialized with 0
    fGdpbIdIndexMap(),
    fdStarTriggerDeadtime(),
    fdStarTriggerDelay(),
    fdStarTriggerWinSize(),
    fdTsDeadtimePeriod(0.0),
    fulCurrentTsIndex(0),
    fuCurrentMs(0),
    fuGdpbId(0),
    fuGdpbNr(0),
    fuGet4Id(0),
    fuGet4Nr(0),
    fvulCurrentEpoch(),
    fvbFirstEpochSeen(),
    fNofEpochs(0),
    fulCurrentEpochTime(0.),
    fdStartTime(-1.),
    fdStartTimeLong(-1.),
    fdStartTimeMsSz(-1.),
    fcMsSizeAll(NULL),
    fEquipmentId(0),
    fdMsIndex(0.),
    fUnpackPar(NULL),
    fuHistoryHistoSize( 1800 ),
    fuHistoryHistoSizeLong( 600 ),
    fdMoniOutIntervalSec( 30.0 ),
    fuPrintToSaveRatio( 4 ),
    fuPrintToSaveCount( 0 ),
    fulGdpbTsMsb(),
    fulGdpbTsLsb(),
    fulStarTsMsb(),
    fulStarTsMid(),
    fulGdpbTsFullLast(),
    fulStarTsFullLast(),
    fuStarTokenLast(),
    fuStarDaqCmdLast(),
    fuStarTrigCmdLast(),
    fulGdpbTsFullLastCore(),
    fulStarTsFullLastCore(),
    fuStarTokenLastCore(),
    fuStarDaqCmdLastCore(),
    fuStarTrigCmdLastCore(),
    fhTokenMsgType(),
    fhTriggerRate(),
    fhTriggerRateLong(),
    fhCmdDaqVsTrig(),
    fhStarTokenEvo(),
    fhStarTrigGdpbTsEvo(),
    fhStarTrigStarTsEvo(),
    fbEventBuilding( kFALSE ),
    fbTimeSortOutput( kFALSE ),
    fStarSubEvent(),
    fulNbBuiltSubEvent( 0 ),
    fulNbStarSubEvent( 0 ),
    fulNbBuiltSubEventLastPrintout( 0 ),
    fulNbStarSubEventLastPrintout( 0 ),
    fTimeLastPrintoutNbStarEvent(),
    fdCurrentMsStartTime( 0.0 ),
    fdCurrentMsEndTime( 0.0 ),
    fvmCurrentLinkBuffer(),
    fvtCurrentLinkBuffer(),
    fdCurrentTsStartTime( 0.0 ),
    fdCurrentTsCoreEndTime( 0.0 ),
    fvmTsLinksBuffer(),
    fvtTsLinksBuffer(),
    fvmTsOverLinksBuffer(),
    fvtTsOverLinksBuffer(),
    fhStarHitToTrigAll_gDPB(),
    fhStarHitToTrigWin_gDPB(),
    fhStarHitToTrigAllTime_gDPB(),
    fhStarHitToTrigWinTime_gDPB(),
    fhStarHitToTrigAllTimeLong_gDPB(),
    fhStarHitToTrigWinTimeLong_gDPB(),
    fhStarEventSize_gDPB(),
    fhStarEventSizeTime_gDPB(),
    fhStarEventSizeTimeLong_gDPB(),
    fhStarTrigTimeToMeanTrig_gDPB(),
    fhStarEventSize( NULL ),
    fhStarEventSizeTime( NULL ),
    fhStarEventSizeTimeLong( NULL ),
    fStartTimeProcessingLastTs(),
    fhStarTsProcessTime( NULL ),
    fhStarTsProcessTimeShort( NULL ),
    fhStarTsInterprocessTime( NULL ),
    fvmEpSupprBuffer(),
    fbEventDumpEna( kFALSE ),
    fpBinDumpFile( NULL )
{
}

CbmTofStarEventBuilder2018::~CbmTofStarEventBuilder2018()
{
}

Bool_t CbmTofStarEventBuilder2018::Init()
{
   LOG(info) << "CbmTofStarEventBuilder2018::Init => modif of 2018/02/14 18:52 CET";
   LOG(info) << "Initializing STAR eTOF 2018 Event Builder";

   FairRootManager* ioman = FairRootManager::Instance();
   if( NULL == ioman )
   {
      LOG(fatal) << "No FairRootManager instance";
   }

   fUnpackPar = (CbmTofStar2018Par*)(FairRun::Instance());

   return kTRUE;
}

void CbmTofStarEventBuilder2018::SetParContainers()
{
  LOG(info) << "Setting parameter containers for " << GetName();
  fUnpackPar = (CbmTofStar2018Par*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmTofStar2018Par"));

}

Bool_t CbmTofStarEventBuilder2018::InitContainers()
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

Bool_t CbmTofStarEventBuilder2018::ReInitContainers()
{
   LOG(info) << "ReInit parameter containers for " << GetName();

   fuNrOfGdpbs = fUnpackPar->GetNrOfRocs();
   LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
   fuMinNbGdpb = fuNrOfGdpbs;

   fuNrOfFebsPerGdpb = fUnpackPar->GetNrOfFebsPerGdpb();
   LOG(info) << "Nr. of FEBS per Tof GDPB: " << fuNrOfFebsPerGdpb;

   fuNrOfGet4PerFeb = fUnpackPar->GetNrOfGet4PerFeb();
   LOG(info) << "Nr. of GET4 per Tof FEB: " << fuNrOfGet4PerFeb;

   fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
   LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

   fuNrOfChannelsPerFeet = fuNrOfGet4PerFeb * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of channels per FEET: " << fuNrOfChannelsPerFeet;

   fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFebsPerGdpb * fuNrOfGet4PerFeb;
   LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

   fuNrOfGet4PerGdpb = fuNrOfFebsPerGdpb * fuNrOfGet4PerFeb;
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
             << fdMsSizeInNs << " ns, gives TS of "
             << fdTsCoreSizeInNs << " ns";

   fdStarTriggerDeadtime.resize( fuNrOfGdpbs );
   fdStarTriggerDelay.resize(    fuNrOfGdpbs );
   fdStarTriggerWinSize.resize(  fuNrOfGdpbs );
   for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
   {
      fdStarTriggerDeadtime[ uGdpb ] = fUnpackPar->GetStarTriggDeadtime( uGdpb );
      fdStarTriggerDelay[ uGdpb ]    = fUnpackPar->GetStarTriggDelay( uGdpb );
      fdStarTriggerWinSize[ uGdpb ]  = fUnpackPar->GetStarTriggWinSize( uGdpb );
      LOG(info) << "STAR trigger par of gDPB  " << uGdpb << " are: "
                << " Deadtime "  <<  fdStarTriggerDeadtime[ uGdpb ]
                << " Delay "     << -fdStarTriggerDelay[ uGdpb ]
                << " and WinSz " <<  fdStarTriggerWinSize[ uGdpb ];
      if( fdStarTriggerDelay[ uGdpb ] < fdStarTriggerWinSize[ uGdpb ] )
         LOG(fatal) << "STAR trigger window should end at latest on token itself!"
                    << " => Delay has to be greater or equal with window size!";
   } // for (Int_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
   fdTsDeadtimePeriod  = fUnpackPar->GetTsDeadtimePeriod();
   LOG(info) << "Timeslice deadtime period at beginning: "
             << fdTsDeadtimePeriod << "(dealt with using the overlap MS)";

   /// STAR Trigger decoding and monitoring
   fulGdpbTsMsb.resize(  fuNrOfGdpbs );
   fulGdpbTsLsb.resize(  fuNrOfGdpbs );
   fulStarTsMsb.resize(  fuNrOfGdpbs );
   fulStarTsMid.resize(  fuNrOfGdpbs );
   fulGdpbTsFullLast.resize(  fuNrOfGdpbs );
   fulStarTsFullLast.resize(  fuNrOfGdpbs );
   fuStarTokenLast.resize(  fuNrOfGdpbs );
   fuStarDaqCmdLast.resize(  fuNrOfGdpbs );
   fuStarTrigCmdLast.resize(  fuNrOfGdpbs );
   fulGdpbTsFullLastCore.resize(  fuNrOfGdpbs );
   fulStarTsFullLastCore.resize(  fuNrOfGdpbs );
   fuStarTokenLastCore.resize(  fuNrOfGdpbs );
   fuStarDaqCmdLastCore.resize(  fuNrOfGdpbs );
   fuStarTrigCmdLastCore.resize(  fuNrOfGdpbs );
   fhTokenMsgType.resize(  fuNrOfGdpbs );
   fhTriggerRate.resize(  fuNrOfGdpbs );
   fhTriggerRateLong.resize(  fuNrOfGdpbs );
   fhCmdDaqVsTrig.resize(  fuNrOfGdpbs );
   fhStarTokenEvo.resize(  fuNrOfGdpbs );
   fhStarTrigGdpbTsEvo.resize(  fuNrOfGdpbs );
   fhStarTrigStarTsEvo.resize(  fuNrOfGdpbs );
   for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
   {
      fulGdpbTsMsb[ uGdpb ] = 0;
      fulGdpbTsLsb[ uGdpb ] = 0;
      fulStarTsMsb[ uGdpb ] = 0;
      fulStarTsMid[ uGdpb ] = 0;
      fulGdpbTsFullLast[ uGdpb ] = 0;
      fulStarTsFullLast[ uGdpb ] = 0;
      fuStarTokenLast[ uGdpb ]   = 0;
      fuStarDaqCmdLast[ uGdpb ]  = 0;
      fuStarTrigCmdLast[ uGdpb ] = 0;
      fulGdpbTsFullLastCore[ uGdpb ] = 0;
      fulStarTsFullLastCore[ uGdpb ] = 0;
      fuStarTokenLastCore[ uGdpb ]   = 0;
      fuStarDaqCmdLastCore[ uGdpb ]  = 0;
      fuStarTrigCmdLastCore[ uGdpb ] = 0;
      fhTokenMsgType[ uGdpb ] = NULL;
      fhTriggerRate[ uGdpb ]  = NULL;
      fhTriggerRateLong[ uGdpb ]  = NULL;
      fhCmdDaqVsTrig[ uGdpb ] = NULL;
      fhStarTokenEvo[ uGdpb ] = NULL;
      fhStarTrigGdpbTsEvo[ uGdpb ] = NULL;
      fhStarTrigStarTsEvo[ uGdpb ] = NULL;
   } // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

   /// STAR subevent building
   if( kTRUE == fbEventBuilding )
   {
      fvmTsLinksBuffer.resize(  fuNrOfGdpbs );
      fvtTsLinksBuffer.resize(  fuNrOfGdpbs );
      fvmTsOverLinksBuffer.resize(  fuNrOfGdpbs );
      fvtTsOverLinksBuffer.resize(  fuNrOfGdpbs );
   } // if( kTRUE == fbEventBuilding )

   fvmEpSupprBuffer.resize( fuNrOfGet4 );

	return kTRUE;
}

void CbmTofStarEventBuilder2018::SetEventBuildingMode( Bool_t bEventBuildingMode )
{
   fbEventBuilding = bEventBuildingMode;
   if( !fbEventBuilding )
      LOG(info) << "Event building mode = single link per subevent: " << "\n"
                << "test mode in 2018 S1, sector mode in 2018 S2";
      else LOG(info) << "Event building mode = all links in same subevent: " << "\n"
                     << "Sector mode in 2018 S1, Full eTOF mode in 2018 S2";
}
void CbmTofStarEventBuilder2018::SetTimeSortOutput( Bool_t bTimeSort )
{
   fbTimeSortOutput = bTimeSort;
   if( fbTimeSortOutput )
      LOG(info) << "Output buffer will be time sorted before being sent to STAR DAQ " << "\n"
                << "=> This will have an effect (build time) only in event building mode with all links in one subevent "
                << " as data for single links are already time sorted";
      else LOG(info) << "Output buffer will NOT be time sorted before being sent to STAR DAQ " << "\n"
                     << "=> This will have an effect (build time) only in event building mode with all links in one subevent "
                     << " as data for single links are already time sorted";
}
void CbmTofStarEventBuilder2018::SetEventDumpEnable( Bool_t bDumpEna )
{
   if( fbEventDumpEna != bDumpEna )
   {
      if( bDumpEna )
      {
         LOG(info) << "Enabling event dump to binary file which was disabled. File will be opened.";

         std::time_t cTimeCurrent = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
         char tempBuff[80];
         std::strftime( tempBuff, 80, "%Y_%m_%d_%H_%M_%S", localtime (&cTimeCurrent) );
         TString sFileName = Form("event_dump_%s.bin", tempBuff);
         fpBinDumpFile = new std::fstream( sFileName, std::ios::out | std::ios::binary);

         if( NULL == fpBinDumpFile )
         {
            LOG(fatal) << "Failed to open new binary file for event dump at "
                       << sFileName;
         } // if( NULL == fpBinDumpFile )
            else LOG(info) << "Opened binary dump file at "
                           << sFileName;

      } // if( bDumpEna )
         else
         {
            LOG(info) << "Disabling event dump to binary file which was enabled. File will be closed.";

            if( NULL != fpBinDumpFile )
               fpBinDumpFile->close();
         } // else of if( bDumpEna )
   } // if( fbEventDumpEna != bDumpEna )

   fbEventDumpEna = bDumpEna;
   if( fbEventDumpEna )
      LOG(info) << "Event dump to binary file is now ENABLED";
      else LOG(info) << "Event dump to binary file is now DISABLED";
}

void CbmTofStarEventBuilder2018::CbmTofStarEventBuilder2018::CreateHistograms()
{
   LOG(info) << "create Histos for " << fuNrOfGdpbs <<" gDPBs ";

   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   TString name { "" };
   TString title { "" };

   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      /// Event building
      name = Form("StarHitToTrigAll_gDPB_%02u", uGdpb);
      title = Form("Time to trigger for all hits gDPB %02u; t(Hit) - t(Trigg) [ns]", uGdpb);
      fhStarHitToTrigAll_gDPB.push_back(
         new TH1I( name.Data(), title.Data(),
                   5000, -100000.0, 50000.0) ); // TODO make offset parameter
//                   4000, -800000.0, 0.0) ); // TODO make offset parameter
      if (server)
         server->Register("/StarRaw", fhStarHitToTrigAll_gDPB[ uGdpb ] );

      name = Form("StarHitToTrigWin_gDPB_%02u", uGdpb);
      title = Form("Time to trigger for hits in trigger window gDPB %02u; t(Hit) - t(Trigg) [ns]", uGdpb);
      UInt_t uNbBins = static_cast< UInt_t >( fdStarTriggerWinSize[uGdpb] / 10.0 );
      Double_t dLowBin  = -fdStarTriggerDelay[uGdpb];
      Double_t dHighBin = -fdStarTriggerDelay[uGdpb] + fdStarTriggerWinSize[uGdpb];
      fhStarHitToTrigWin_gDPB.push_back(
         new TH1I( name.Data(), title.Data(),
                   uNbBins, dLowBin, dHighBin) ); // TODO make size parameter
      if (server)
         server->Register("/StarRaw", fhStarHitToTrigWin_gDPB[ uGdpb ] );

      name = Form("StarHitToTrigAllTime_gDPB_%02u", uGdpb);
      title = Form("Time to trigger for all hits gDPB %02u; Time in run [s]; t(Hit) - t(Trigg) [ns]", uGdpb);
      fhStarHitToTrigAllTime_gDPB.push_back(
         new TH2I( name.Data(), title.Data(),
                   fuHistoryHistoSize, 0, fuHistoryHistoSize,
                   1500, -100000.0, 50000.0) ); // TODO make offset parameter

      name = Form("StarHitToTrigWinTime_gDPB_%02u", uGdpb);
      title = Form("Time to trigger for hits in trigger window gDPB %02u; Time in run [s]; t(Hit) - t(Trigg) [ns]", uGdpb);
      fhStarHitToTrigWinTime_gDPB.push_back(
         new TH2I( name.Data(), title.Data(),
                   fuHistoryHistoSize, 0, fuHistoryHistoSize,
                   uNbBins/10.0, dLowBin, dHighBin) ); // TODO make size parameter

      name = Form("StarHitToTrigAllTimeLong_gDPB_%02u", uGdpb);
      title = Form("Time to trigger for all hits gDPB %02u; Time in run [s]; t(Hit) - t(Trigg) [ns]", uGdpb);
      fhStarHitToTrigAllTimeLong_gDPB.push_back(
         new TH2I( name.Data(), title.Data(),
                   fuHistoryHistoSizeLong, 0, fuHistoryHistoSizeLong,
                   1500, -100000.0, 50000.0) ); // TODO make offset parameter

      name = Form("StarHitToTrigWinTimeLong_gDPB_%02u", uGdpb);
      title = Form("Time to trigger for hits in trigger window gDPB %02u; Time in run [s]; t(Hit) - t(Trigg) [ns]", uGdpb);
      fhStarHitToTrigWinTimeLong_gDPB.push_back(
         new TH2I( name.Data(), title.Data(),
                   fuHistoryHistoSizeLong, 0, fuHistoryHistoSizeLong,
                   uNbBins/10.0, dLowBin, dHighBin) ); // TODO make size parameter

      if( server )
      {
         server->Register("/StarRaw", fhStarHitToTrigAllTime_gDPB[ uGdpb ] );
         server->Register("/StarRaw", fhStarHitToTrigWinTime_gDPB[ uGdpb ] );
         server->Register("/StarRaw", fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ] );
         server->Register("/StarRaw", fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ] );
      } //

      /// STAR Trigger decoding and monitoring
      name = Form( "hTokenMsgType_gDPB_%02u", uGdpb);
      title = Form( "STAR trigger Messages type gDPB %02u; Type ; Counts", uGdpb);
      fhTokenMsgType[ uGdpb ] =  new TH1F(name, title, 4, 0, 4);
      fhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 1, "A"); // gDPB TS high
      fhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 2, "B"); // gDPB TS low, STAR TS high
      fhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 3, "C"); // STAR TS mid
      fhTokenMsgType[ uGdpb ]->GetXaxis()->SetBinLabel( 4, "D"); // STAR TS low, token, CMDs
      if (server)
         server->Register("/StarRaw", fhTokenMsgType[ uGdpb ] );

      name = Form( "hTriggerRate_gDPB_%02u", uGdpb);
      title = Form( "STAR trigger signals per second gDPB %02u; Time[s] ; Counts", uGdpb);
      fhTriggerRate[ uGdpb ] =  new TH1F(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize);
      if (server)
         server->Register("/StarRaw", fhTriggerRate[ uGdpb ] );

      name = Form( "hTriggerRateLong_gDPB_%02u", uGdpb);
      title = Form( "STAR trigger signals per minute gDPB %02u; Time[min] ; Counts", uGdpb);
      fhTriggerRateLong[ uGdpb ] =  new TH1F(name, title, fuHistoryHistoSizeLong, 0, fuHistoryHistoSizeLong);
      if (server)
         server->Register("/StarRaw", fhTriggerRateLong[ uGdpb ] );

      name = Form( "hCmdDaqVsTrig_gDPB_%02u", uGdpb);
      title = Form( "STAR daq command VS STAR trigger command gDPB %02u; DAQ ; TRIGGER", uGdpb);
      fhCmdDaqVsTrig[ uGdpb ] =  new TH2I(name, title, 16, 0, 16, 16, 0, 16 );
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 1, "0x0: no-trig "); // idle link
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 2, "0x1: clear   "); // clears redundancy counters on the readout boards
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 3, "0x2: mast-rst"); // general reset of the whole front-end logic
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 4, "0x3: spare   "); // reserved
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 5, "0x4: trigg. 0"); // Default physics readout, all det support required
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 6, "0x5: trigg. 1"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 7, "0x6: trigg. 2"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 8, "0x7: trigg. 3"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel( 9, "0x8: puls.  0"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(10, "0x9: puls.  1"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(11, "0xA: puls.  2"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(12, "0xB: puls.  3"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(13, "0xC: config  "); // housekeeping trigger: return geographic info of FE
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(14, "0xD: abort   "); // aborts and clears an active event
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(15, "0xE: L1accept"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetXaxis()->SetBinLabel(16, "0xF: L2accept"); //
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 1, "0x0:  0"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 2, "0x1:  1"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 3, "0x2:  2"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 4, "0x3:  3"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 5, "0x4:  4"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 6, "0x5:  5"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 7, "0x6:  6"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 8, "0x7:  7"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel( 9, "0x8:  8"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(10, "0x9:  9"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(11, "0xA: 10"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(12, "0xB: 11"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(13, "0xC: 12"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(14, "0xD: 13"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(15, "0xE: 14"); // To be filled at STAR
      fhCmdDaqVsTrig[ uGdpb ]->GetYaxis()->SetBinLabel(16, "0xF: 15"); // To be filled at STAR
      if (server)
         server->Register("/StarRaw", fhCmdDaqVsTrig[ uGdpb ] );

      name = Form( "hStarTokenEvo_gDPB_%02u", uGdpb);
      title = Form( "STAR token value VS time gDPB %02u; Time in Run [s] ; STAR Token; Counts", uGdpb);
      fhStarTokenEvo[ uGdpb ] =  new TH2I(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize, 410, 0, 4100 ); // 4096
      if (server)
         server->Register("/StarRaw", fhStarTokenEvo[ uGdpb ] );

      name = Form( "hStarTrigGdpbTsEvo_gDPB_%02u", uGdpb);
      title = Form( "gDPB TS in STAR triger tokens for gDPB %02u; Time in Run [s] ; gDPB TS;", uGdpb);
      fhStarTrigGdpbTsEvo[ uGdpb ] =  new TProfile(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize );
      if (server)
         server->Register("/StarRaw", fhStarTrigGdpbTsEvo[ uGdpb ] );

      name = Form( "hStarTrigStarTsEvo_gDPB_%02u", uGdpb);
      title = Form( "STAR TS in STAR triger tokens for gDPB %02u; Time in Run [s] ; STAR TS;", uGdpb);
      fhStarTrigStarTsEvo[ uGdpb ] =  new TProfile(name, title, fuHistoryHistoSize, 0, fuHistoryHistoSize );
      if (server)
         server->Register("/StarRaw", fhStarTrigStarTsEvo[ uGdpb ] );

      /// Check if we are in "single link per sub-event" building mode
      if( kFALSE == fbEventBuilding )
      {
         name = Form("StarEventSize_gDPB_%02u", uGdpb);
         title = Form("STAR SubEvent size gDPB %02u; SubEvent size [bytes]", uGdpb);
         uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize() / 8 ); // 1 bin = 1 long 64b uint
         fhStarEventSize_gDPB.push_back(
            new TH1I( name.Data(), title.Data(),
                      uNbBins, 0.0, CbmTofStarSubevent::GetMaxOutputSize() ) );
         if (server)
            server->Register("/StarRaw", fhStarEventSize_gDPB[ uGdpb ] );

         name = Form("StarEventSizeTime_gDPB_%02u", uGdpb);
         title = Form("STAR SubEvent size gDPB %02u; run time [s]; SubEvent size [bytes]", uGdpb);
         uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize()
                                               / (sizeof( ngdpb::Message )) );
         fhStarEventSizeTime_gDPB.push_back(
            new TH2I( name.Data(), title.Data(),
                      fuHistoryHistoSize, 0.0, fuHistoryHistoSize,
                      uNbBins, 0.0, CbmTofStarSubevent::GetMaxOutputSize() ) );
         if (server)
            server->Register("/StarRaw", fhStarEventSizeTime_gDPB[ uGdpb ] );

         name = Form("StarEventSizeTimeLong_gDPB_%02u", uGdpb);
         title = Form("STAR SubEvent size gDPB %02u; run time [min]; SubEvent size [bytes]", uGdpb);
         uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize()
                                               / (sizeof( ngdpb::Message )) );
         fhStarEventSizeTimeLong_gDPB.push_back(
            new TH2I( name.Data(), title.Data(),
                      fuHistoryHistoSizeLong, 0.0, fuHistoryHistoSizeLong,
                      uNbBins, 0.0, CbmTofStarSubevent::GetMaxOutputSize() ) );
         if (server)
            server->Register("/StarRaw", fhStarEventSizeTimeLong_gDPB[ uGdpb ] );
      } // if( kFALSE == fbEventBuilding )
         else
         {

            name = Form("StarTrigTimeToMeanTrig_gDPB_%02u", uGdpb);
            title = Form("Time difference between gDPB %02u trigger and mean trigger; dT [ns]; counts []", uGdpb);
            uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize()
                                                  / (sizeof( ngdpb::Message )) );
            fhStarTrigTimeToMeanTrig_gDPB.push_back(
               new TH1I( name.Data(), title.Data(),
                         625, -312.5, 312.5 ) );
            if (server)
               server->Register("/StarRaw", fhStarTrigTimeToMeanTrig_gDPB[ uGdpb ] );
         } // else of if( kFALSE == fbEventBuilding )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

   /// Check if we are in "single sub-event for all links" building mode
   if( kTRUE == fbEventBuilding )
   {
      name = "StarEventSize_gDPB";
      title = "STAR SubEvent size for all gDPBs; SubEvent size [bytes]";
      UInt_t uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize() / 8 ); // 1 bin = 1 long 64b uint
      fhStarEventSize = new TH1I( name.Data(), title.Data(), uNbBins, 0.0, CbmTofStarSubevent::GetMaxOutputSize() );
      if (server)
         server->Register("/StarRaw", fhStarEventSize );

      name = "StarEventSizeTime";
      title = "STAR SubEvent size for all gDPBS; run time [s]; SubEvent size [bytes]";
      uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize()
                                            / (sizeof( gdpb::FullMessage )) );
      fhStarEventSizeTime = new TH2I( name.Data(), title.Data(),
                   fuHistoryHistoSize, 0.0, fuHistoryHistoSize,
                   uNbBins, 0.0, CbmTofStarSubevent::GetMaxOutputSize() );
      if (server)
         server->Register("/StarRaw", fhStarEventSizeTime );

      name = "StarEventSizeTimeLong";
      title = "STAR SubEvent size for all gDPB; run time [min]; SubEvent size [bytes]";
      uNbBins = static_cast< UInt_t >( CbmTofStarSubevent::GetMaxOutputSize()
                                            / (sizeof( gdpb::FullMessage )) );
      fhStarEventSizeTimeLong = new TH2I( name.Data(), title.Data(),
                   fuHistoryHistoSizeLong, 0.0, fuHistoryHistoSizeLong,
                   uNbBins, 0.0, CbmTofStarSubevent::GetMaxOutputSize() );
      if (server)
         server->Register("/StarRaw", fhStarEventSizeTimeLong );
   } // if( kFALSE == fbEventBuilding )

   /// Monitoring of the processing time
   name = "fhStarTsProcessTime";
   title = "Ratio of processing time per TS to TS size VS TS index; TS index []; Ratio  []";
   Double_t dTsEvoLongMax = fuHistoryHistoSizeLong * 60.0 / (fdTsCoreSizeInNs * 1e-9);
   fhStarTsProcessTime = new TH2I( name.Data(), title.Data(),
                fuHistoryHistoSizeLong, 0.0, dTsEvoLongMax,
                250, 0.0, 5.0 );
   if (server)
      server->Register("/StarRaw", fhStarTsProcessTime );
   name = "fhStarTsProcessTimeShort";
   title = "Ratio of processing time per TS to TS size VS TS index; TS index []; Ratio  []";
   Double_t dTsEvoShortMax = 500 / (fdTsCoreSizeInNs * 1e-9);
   fhStarTsProcessTimeShort = new TH2I( name.Data(), title.Data(),
                500, 0.0, dTsEvoShortMax,
                250, 0.0, 5.0 );
   if (server)
      server->Register("/StarRaw", fhStarTsProcessTimeShort );
   name = "fhStarTsInterprocessTime";
   title = "Ratio of time between TS processing to TS size VS TS index; Ratio tInterprocess / tTS  []";
   fhStarTsInterprocessTime = new TH1D( name.Data(), title.Data(),
                2000, 0.0, 200.0 );
   if (server)
      server->Register("/StarRaw", fhStarTsInterprocessTime );

   /** Create summary Canvases for STAR 2017 **/
   Double_t w = 10;
   Double_t h = 10;
   /*****************************/


   /** Create STAR token Canvas for STAR 2017 **/
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      TCanvas* cStarToken = new TCanvas( Form("cStarToken_g%02u", uGdpb),
                                           Form("STAR token detection info for gDPB %02u", uGdpb),
                                           w, h);
      cStarToken->Divide( 2, 2 );

      cStarToken->cd(1);
      fhTriggerRate[uGdpb]->Draw();

      cStarToken->cd(2);
      fhCmdDaqVsTrig[uGdpb]->Draw( "colz" );

      cStarToken->cd(3);
      fhStarTokenEvo[uGdpb]->Draw();

      cStarToken->cd(4);
      fhStarTrigStarTsEvo[uGdpb]->Draw( "hist" );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   /*****************************/

   /** Create Event building mode Canvas(es) for STAR 2017 **/
   if( kTRUE == fbEventBuilding )
   {
      TCanvas* cStarEvtBuild = new TCanvas( "cStarEvtAll",
                                           "STAR SubEvent Building for all gDPBs",
                                           w, h);
      cStarEvtBuild->Divide( 2, 2 );

      cStarEvtBuild->cd(1);
      gPad->SetLogx();
      gPad->SetLogy();
      fhStarEventSize->Draw();

      cStarEvtBuild->cd(2);
      gPad->SetLogy();
      gPad->SetLogz();
      fhStarEventSizeTime->Draw( "colz" );

      cStarEvtBuild->cd(3);
      gPad->SetLogy();
      gPad->SetLogz();
      fhStarEventSizeTimeLong->Draw( "colz" );
   } // if( kTRUE == fbEventBuilding )

   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      TCanvas* cStarEvtBuild = new TCanvas( Form("cStarEvt_g%02u", uGdpb),
                                           Form("STAR SubEvent Building for gDPB %02u", uGdpb),
                                           w, h);
      if( kTRUE == fbEventBuilding )
         cStarEvtBuild->Divide( 1, 3 );
         else cStarEvtBuild->Divide( 2, 2 );

      cStarEvtBuild->cd(1);
      fhStarHitToTrigAll_gDPB[uGdpb]->Draw();

      cStarEvtBuild->cd(2);
      fhStarHitToTrigWin_gDPB[uGdpb]->Draw();

      if( kFALSE == fbEventBuilding )
      {
         cStarEvtBuild->cd(3);
         gPad->SetLogx();
         gPad->SetLogy();
         fhStarEventSize_gDPB[uGdpb]->Draw();

         cStarEvtBuild->cd(4);
         gPad->SetLogy();
         gPad->SetLogz();
         fhStarEventSizeTime_gDPB[uGdpb]->Draw( "colz" );
      } // if( kFALSE == fbEventBuilding )
         else
         {
            cStarEvtBuild->cd(3);
            gPad->SetLogy();
            fhStarTrigTimeToMeanTrig_gDPB[uGdpb]->Draw();
         } // else of if( kFALSE == fbEventBuilding )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   /*****************************/

}

Bool_t CbmTofStarEventBuilder2018::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   uint64_t ulTsIndex = ts.index();
   if( fulCurrentTsIndex < ulTsIndex )
   {
      /// Check if we are in "single sub-event for all links" building mode
      if( kTRUE == fbEventBuilding )
      {
         BuildStarEventsAllLinks();
      } // if( kTRUE == fbEventBuilding )

      std::chrono::time_point<std::chrono::system_clock> timeCurrent = std::chrono::system_clock::now();
      if( 0 != fStartTimeProcessingLastTs.time_since_epoch().count() &&
          0 < fulCurrentTsIndex )
      {
         std::chrono::duration<double> elapsed_seconds = timeCurrent - fStartTimeProcessingLastTs;
         Double_t dTimeRatio = elapsed_seconds.count() / (fdTsCoreSizeInNs * 1e-9);
         fhStarTsInterprocessTime->Fill( dTimeRatio );
      } // if( 0 != fStartTimeProcessingLastTs.time_since_epoch().count() && 0 < fulCurrentTsIndex )
      fStartTimeProcessingLastTs = timeCurrent;
   } // if( fCurrentTsIndex < ts.index() )
      else if( ulTsIndex < fulCurrentTsIndex )
      {
         /// Either TS index made a loop or the timestamps were reinitialised
         /// Check if we are in "single sub-event for all links" building mode
         if( kTRUE == fbEventBuilding )
         {
            /// Clear buffers, eventual border crossing should be taken care thanks to the overlap MS
            for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
            {
               fvtTsLinksBuffer[uGdpb].clear();
               fvmTsLinksBuffer[uGdpb].clear();
               fvmTsOverLinksBuffer[uGdpb].clear();
               fvtTsOverLinksBuffer[uGdpb].clear();
            } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
         } // if( kTRUE == fbEventBuilding )
      } // if( tsIndex < fulCurrentTsIndex )
   fulCurrentTsIndex = ulTsIndex;

   LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
              << " microslices of component " << component;

   /// Printout of nb star events log
   std::chrono::time_point<std::chrono::system_clock> timeCurrent = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = timeCurrent - fTimeLastPrintoutNbStarEvent;
   if( 0 == fTimeLastPrintoutNbStarEvent.time_since_epoch().count() )
   {
      fTimeLastPrintoutNbStarEvent = timeCurrent;
      fulNbBuiltSubEventLastPrintout = fulNbBuiltSubEvent;
      fulNbStarSubEventLastPrintout  = fulNbStarSubEvent;
   } // if( 0 == fTimeLastPrintoutNbStarEvent.time_since_epoch().count() )
   else if( fdMoniOutIntervalSec < elapsed_seconds.count() )
   {
      std::time_t cTimeCurrent = std::chrono::system_clock::to_time_t( timeCurrent );
      char tempBuff[80];
      std::strftime( tempBuff, 80, "%F %T", localtime (&cTimeCurrent) );

      LOG(info) << "CbmTofStarEventBuilder2018::DoUnpack => " << tempBuff
               << " Total number of Built events: " << std::setw(9) << fulNbBuiltSubEvent
               << ", " << std::setw(9) << (fulNbBuiltSubEvent - fulNbBuiltSubEventLastPrintout)
               << " events in last " << std::setw(4) << elapsed_seconds.count() << " s";
      fTimeLastPrintoutNbStarEvent = timeCurrent;
      fulNbBuiltSubEventLastPrintout   = fulNbBuiltSubEvent;

      LOG(info) << "CbmTofStarEventBuilder2018::DoUnpack => " << tempBuff
               << " Total number of events sent to STAR: " << std::setw(9) << fulNbStarSubEvent
               << ", " << std::setw(9) << (fulNbStarSubEvent - fulNbStarSubEventLastPrintout)
               << " events in last " << std::setw(4) << elapsed_seconds.count() << " s";
      fTimeLastPrintoutNbStarEvent = timeCurrent;
      fulNbStarSubEventLastPrintout   = fulNbStarSubEvent;

      if( kTRUE == fbEventBuilding )
      {
      } // if( kTRUE == fbEventBuilding )
         else LOG(info) << "CbmTofStarEventBuilder2018::DoUnpack => " << tempBuff
                        << " Buffers size: messages " << std::setw(9) << fvmCurrentLinkBuffer.size()
                        << " triggers " << std::setw(9) << fvtCurrentLinkBuffer.size();

      fuPrintToSaveCount ++;
      if( fuPrintToSaveCount == fuPrintToSaveRatio )
      {
         SaveAllHistos( "data/histos_event_build.root" );
         fuPrintToSaveCount = 0;
      } // if( fuPrintToSaveCount == fuPrintToSaveRatio )
   } // else if( 300 < elapsed_seconds.count() )

   // Loop over microslices
   Int_t iMessageType = -111;
   size_t numCompMsInTs = ts.num_microslices(component);
   for( fuCurrentMs = 0; fuCurrentMs < numCompMsInTs; ++fuCurrentMs )
   {
      // Jump some microslices if needed
//      if( fuMsAcceptsPercent < m)
//         continue;

      // Ignore overlap ms if number defined by user
      // => Should be dealt with in the methods for each message type
//      if( numCompMsInTs - fuOverlapMsNb <= fuCurrentMs )
//         continue;

      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, fuCurrentMs);
      fEquipmentId = msDescriptor.eq_id;
      fdMsIndex = static_cast<double>(msDescriptor.idx);
      const uint8_t* msContent = reinterpret_cast< const uint8_t* >( ts.content(component, fuCurrentMs) );

      uint32_t size = msDescriptor.size;
      if( 0 < size )
         LOG(debug1) << "Microslice "<< fuCurrentMs <<": " << fdMsIndex
                     << " has size: " << size;

      if( fdStartTimeMsSz < 0 )
         fdStartTimeMsSz = (1e-9) * fdMsIndex;

      /// Save the starting time of the TS or of the MS
      if( kTRUE == fbEventBuilding )
      {
         if( 0 == fuCurrentMs )
         {
            fdCurrentTsStartTime = fdMsIndex;
            fdCurrentTsCoreEndTime = fdCurrentTsStartTime + fdTsCoreSizeInNs;
         } // if( 0 == fuCurrentMs )
      } // if( kTRUE == fbEventBuilding )
         else
         {
            fdCurrentMsStartTime = fdMsIndex;
            fdCurrentMsEndTime   = fdCurrentMsStartTime + fdMsSizeInNs;
         } // else of if( kTRUE == fbEventBuilding )

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

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
         gdpb::Message mess( ulData );

         if( gLogger->IsLogNeeded( fair::Severity::debug2 ) )
         {
            mess.printDataCout();
         } // if( gLogger->IsLogNeeded( fair::Severity::debug2 ) )

         // Increment counter for different message types
         iMessageType = mess.getMessageType();
         fMsgCounter[ iMessageType ]++;

         if( 0 == uIdx )
         {
            fuGdpbId = mess.getRocNumber();

            /// if unmapped gDPB, just ignore this block
            if( fGdpbIdIndexMap.end() == fGdpbIdIndexMap.find( fuGdpbId ) )
               break;

            fuGdpbNr = fGdpbIdIndexMap[ fuGdpbId ];

            /// Store the last STAR trigger values for the core MS when reaching the first overlap MS
            if( fuCoreMs == fuCurrentMs )
            {
               fulGdpbTsFullLastCore[ fuGdpbNr ] = fulGdpbTsFullLast[ fuGdpbNr ];
               fulStarTsFullLastCore[ fuGdpbNr ] = fulStarTsFullLast[ fuGdpbNr ];
               fuStarTokenLastCore[ fuGdpbNr ]   = fuStarTokenLast[ fuGdpbNr ];
               fuStarDaqCmdLastCore[ fuGdpbNr ]  = fuStarDaqCmdLast[ fuGdpbNr ];
               fuStarTrigCmdLastCore[ fuGdpbNr ] = fuStarTrigCmdLast[ fuGdpbNr ];
            } // if( fuCoreMs == fuCurrentMs )

            /// Restore the last STAR trigger values for the core MS when reaching the first core MS
            if( 0 == fuCurrentMs )
            {
               fulGdpbTsFullLast[ fuGdpbNr ] = fulGdpbTsFullLastCore[ fuGdpbNr ];
               fulStarTsFullLast[ fuGdpbNr ] = fulStarTsFullLastCore[ fuGdpbNr ];
               fuStarTokenLast[ fuGdpbNr ]   = fuStarTokenLastCore[ fuGdpbNr ];
               fuStarDaqCmdLast[ fuGdpbNr ]  = fuStarDaqCmdLastCore[ fuGdpbNr ];
               fuStarTrigCmdLast[ fuGdpbNr ] = fuStarTrigCmdLastCore[ fuGdpbNr ];
            } // if( 0 == fuCurrentMs )
         } // if( 0 = uIdx )

         fuGet4Id = mess.getGdpbGenChipId();
         fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + fuGet4Id;

         if( fuNrOfGet4PerGdpb <= fuGet4Id &&
             gdpb::MSG_STAR_TRI != iMessageType &&
             get4v2x::kuChipIdMergedEpoch != fuGet4Id )
            LOG(warning) << "Message with Get4 ID too high: " << fuGet4Id
                         << " VS " << fuNrOfGet4PerGdpb << " set in parameters.";

         /// First message in MS should ALWAYS be a merged epoch
/*
         if( 0 == uIdx )
         {
            if( gdpb::MSG_EPOCH2 != iMessageType || get4v2x::kuChipIdMergedEpoch != fuGet4Id )
               LOG(warning) << "First message in MS is not a merged epoch!!!!!"
                            << " TS = " << fulCurrentTsIndex << " MS = " << fuCurrentMs
                            << " Type = " << iMessageType << " Get4Id = " << fuGet4Id
                            << " Is epoch? " << (gdpb::MSG_EPOCH2 == iMessageType);
         } // if( 0 == uIdx )
         if( uNbMessages - 1 == uIdx )
         {
            if( gdpb::MSG_EPOCH2 != iMessageType || get4v2x::kuChipIdMergedEpoch != fuGet4Id )
               LOG(warning) << "Last message in MS is not a merged epoch!!!!!"
                            << " TS = " << fulCurrentTsIndex << " MS = " << fuCurrentMs
                            << " Type = " << iMessageType << " Get4Id = " << fuGet4Id
                            << " Is epoch? " << (gdpb::MSG_EPOCH2 == iMessageType);
         } // if( uNbMessages - 1 == uIdx )
*/
         switch( iMessageType )
         {
            case gdpb::MSG_GET4:
            {
               //           FillEpochInfo(mess);
               LOG(error) << "Message type " << iMessageType
                          << " not included in unpacker.";
               break;
            } // case old non tof messages
            case gdpb::MSG_EPOCH2:
            {
               if( get4v2x::kuChipIdMergedEpoch == fuGet4Id )
               {
                  /// Propagate for all ASICs corresponding to this merged epoch
                  for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGet4Index ++ )
                  {
                     gdpb::Message tmpMess( mess );
                     tmpMess.setGdpbGenChipId( uGet4Index );
                     fuGet4Nr = (fuGdpbNr * fuNrOfGet4PerGdpb) + uGet4Index;

                     FillEpochInfo( tmpMess );
                  } // for( uint32_t uGet4Index = 0; uGet4Index < fuNrOfGet4PerGdpb; uGetIndex ++ )

                  /// Keep track of extended epoch index for each gDPB
                  // TO BE DONE
               } // if this epoch message is a merged one valiud for all chips
                  else
                  {
//                     fHistGet4MessType->Fill( fuGet4Nr, gdpb::GET4_32B_EPOCH );
                     LOG(warning) << "Extra non merged epoch for GDPB#" << Form("%2u", fuGdpbNr)
                                  << " G4 #" << Form("%2u", fuGet4Id)
                                  << " => ASIC " << Form("%3u", fuGet4Nr);
                     FillEpochInfo(mess);
                  } // if single chip epoch message

               /// Out of FillEpochInfo so that we do not expand the Merged epoch!!
               gdpb::FullMessage fullMess( mess, mess.getGdpbEpEpochNb() );
               if( kTRUE == fbEventBuilding )
               {
                  if( fuCurrentMs < fuCoreMs )
                     fvmTsLinksBuffer[fuGdpbNr].push_back( fullMess );
                     else fvmTsOverLinksBuffer[fuGdpbNr].push_back( fullMess );
               } // if( kTRUE == fbEventBuilding )
                  else
                     fvmCurrentLinkBuffer.push_back( fullMess );

               break;
            } // case gdpb::MSG_EPOCH2:
            case gdpb::MSG_GET4_32B:
            {
               FillHitInfo( mess );
//               fvmEpSupprBuffer[ fuGet4Nr ].push_back( mess );
               break;
            } // case gdpb::MSG_GET4_32B:
            case gdpb::MSG_GET4_SLC:
            {
               PrintSlcInfo(mess);
               break;
            } // case gdpb::MSG_GET4_SLC:
            case gdpb::MSG_GET4_SYS:
            {
               PrintSysInfo(mess);
               break;
            } // case gdpb::MSG_GET4_SYS:
            case gdpb::MSG_STAR_TRI:
            {
               FillStarTrigInfo(mess);
               break;
            } // case gdpb::MSG_STAR_TRI:
            default:
            {
               if(100 > iMess++)
                  LOG(error) << "Message ("<<iMess<<") type " << std::hex << std::setw(2)
                             << static_cast< uint16_t >( iMessageType )
                             << " not included in Get4 unpacker.";
               if(100 == iMess)
                  LOG(error) << "Stop reporting MSG errors... ";
            } // default:
         } // switch( iMessageType )

      } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)

      /// Check if we are in "single link per sub-event" building mode
      if( kFALSE == fbEventBuilding )
      {
         /// Make sure that the current MS is not one of the overlap ones
         /// and build sub-events in it if that is the case
         if( fuCurrentMs < fuCoreMs )
            BuildStarEventsSingleLink();
            else
            {
               /// clear message buffer
               fvmCurrentLinkBuffer.clear();

               /// Clear trigger buffer
               fvtCurrentLinkBuffer.clear();
            } // else of if( fuCurrentMs < fuCoreMs )
      } // if( kFALSE == fbEventBuilding )

   } // for( fuCurrentMs = 0; fuCurrentMs < numCompMsInTs; ++fuCurrentMs )

   /// Plot processing time
   timeCurrent = std::chrono::system_clock::now();
   if( 0 != fStartTimeProcessingLastTs.time_since_epoch().count() &&
       0 < fulCurrentTsIndex )
   {
      elapsed_seconds = timeCurrent - fStartTimeProcessingLastTs;
      Double_t dTimeRatio = elapsed_seconds.count() / (fdTsCoreSizeInNs * 1e-9);
      if( dTimeRatio < 5 )
      {
         fhStarTsProcessTime->Fill(fulCurrentTsIndex, dTimeRatio );
         fhStarTsProcessTimeShort->Fill(fulCurrentTsIndex, dTimeRatio );
      } // if( dTimeRatio < 5 )
         else
         {
            fhStarTsProcessTime->Fill(fulCurrentTsIndex, 4.999 );
            fhStarTsProcessTimeShort->Fill(fulCurrentTsIndex, 4.999 );
         } // else of if( dTimeRatio < 5 )
   } // if( 0 != fStartTimeProcessingLastTs.time_since_epoch().count() && 0 < fulCurrentTsIndex )
   fStartTimeProcessingLastTs = timeCurrent;

  return kTRUE;
}

void CbmTofStarEventBuilder2018::FillHitInfo( gdpb::Message mess )
{
   // --- Get absolute time, GET4 ID and channel number
   UInt_t uGet4Id     = mess.getGdpbGenChipId();
   UInt_t uChannel    = mess.getGdpbHitChanId();
//   UInt_t uTot        = mess.getGdpbHit32Tot();
   // In 32b mode the coarse counter is already computed back to 112 FTS bins
   // => need to hide its contribution from the Finetime
   // => FTS = Fullt TS modulo 112
//   UInt_t uFts        = mess.getGdpbHitFullTs() % 112;

   ULong64_t  ulCurEpochGdpbGet4 = fvulCurrentEpoch[ fuGet4Nr ];

   if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
   {

/// TODO clean epoch suppression in STAR 2018!
/*
      // In Ep. Suppr. Mode, receive following epoch instead of previous
      if( 0 < ulCurEpochGdpbGet4 )
         ulCurEpochGdpbGet4 --;
         else ulCurEpochGdpbGet4 = get4v2x::kuEpochCounterSz; // Catch epoch cycle!
*/
//      ULong_t  ulhitTime = mess.getMsgFullTime(  ulCurEpochGdpbGet4 );
      Double_t dHitTime  = mess.getMsgFullTimeD( ulCurEpochGdpbGet4 );
//      Double_t dHitTot   = uTot;     // in bins


//      UInt_t uFebIdx     = (uGet4Id / fuNrOfGet4PerFeb);
//      UInt_t uFullFebIdx = (fuGdpbNr * fuNrOfFebsPerGdpb) + uFebIdx;

      UInt_t uChanInGdpb = uGet4Id * fuNrOfChannelsPerGet4 + uChannel;
      UInt_t uChanInSyst = fuGdpbNr * fuNrOfChannelsPerGdpb + uChanInGdpb;
      if( fUnpackPar->GetNumberOfChannels() < 1 ||
          static_cast< UInt_t>( fUnpackPar->GetNumberOfChannels() ) < uChanInSyst )
      {
         LOG(error) << "Invalid mapping index " << uChanInSyst
                    << " VS " << fUnpackPar->GetNumberOfChannels()
                    <<", from " << fuGdpbNr
                    <<", " << uGet4Id
                    <<", " << uChannel;
         return;
      } // if( fUnpackPar->GetNumberOfChannels() < uChanUId )

      /// Histograms filling
         /// In Run rate evolution
      if (fdStartTime < 0)
         fdStartTime = dHitTime;
      if (fdStartTimeLong < 0)
         fdStartTimeLong = dHitTime;
         /// Reset the evolution Histogram and the start time when we reach the end of the range
      if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) )
      {
         ResetEvolutionHistograms();
         fdStartTime = dHitTime;
      } // if( fuHistoryHistoSize < 1e-9 * (dHitTime - fdStartTime) )
          /// Reset the long evolution Histogram and the start time when we reach the end of the range
      if( fuHistoryHistoSizeLong < 1e-9 * (dHitTime - fdStartTimeLong) / 60.0 )
      {
         ResetLongEvolutionHistograms();
         fdStartTimeLong = dHitTime;
      } // if( fuHistoryHistoSizeLong < 1e-9 * (dHitTime - fdStartTimeLong) / 60.0 )

      gdpb::FullMessage fullMess( mess, ulCurEpochGdpbGet4 );
      if( kTRUE == fbEventBuilding )
      {
         if( fuCurrentMs < fuCoreMs )
            fvmTsLinksBuffer[fuGdpbNr].push_back( fullMess );
            else fvmTsOverLinksBuffer[fuGdpbNr].push_back( fullMess );
      } // if( kTRUE == fbEventBuilding )
         else
            fvmCurrentLinkBuffer.push_back( fullMess );

   } // if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
}

void CbmTofStarEventBuilder2018::FillEpochInfo( gdpb::Message mess )
{
   ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();

   fvulCurrentEpoch[ fuGet4Nr ] = ulEpochNr;
/*
   if (1 == mess.getGdpbEpDataLoss())
      LOG(info) << "Data loss flag set GDPB #" << Form( "%2u", fuGdpbNr) << " G4 #" << Form( "%3u", fuGet4Nr);
   if (1 == mess.getGdpbEpEpochLoss())
      LOG(info) << "Epoch loss flag set GDPB #" << Form( "%2u", fuGdpbNr) << " G4 #" << Form( "%3u", fuGet4Nr);
//   if (1 == mess.getGdpbEpMissmatch())
//      LOG(info) << "Epoch missmatch flag set GDPB #" << Form( "%2u", fuGdpbNr) << " G4 #" << Form( "%3u", fuGet4Nr);
*/

   if( kFALSE == fvbFirstEpochSeen[ fuGet4Nr ] )
      fvbFirstEpochSeen[ fuGet4Nr ] = kTRUE;

   fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);
   fNofEpochs++;
/*
   /// In Ep. Suppr. Mode, receive following epoch instead of previous
   /// Re-align the epoch number of the message in case it will be used later:
   /// We received the epoch after the data instead of the one before!
   /// TODO FIXME: commented out for the time being so that the order in the STAR
   ///             date is the same as in the CBM data
   if( 0 < ulEpochNr )
      mess.setEpoch2Number( ulEpochNr - 1 );
   else mess.setEpoch2Number( get4v2x::kuEpochCounterSz );

   Int_t iBufferSize = fvmEpSupprBuffer[ fuGet4Nr ].size();
   if( 0 < iBufferSize )
   {
      LOG(debug) << "Now processing stored messages for for get4 " << fuGet4Nr << " with epoch number "
                 << (fvulCurrentEpoch[ fuGet4Nr ] - 1);

      /// Data are sorted between epochs, not inside => Epoch level ordering
      /// Sorting at lower bin precision level
      std::stable_sort( fvmEpSupprBuffer[fuGet4Nr].begin(), fvmEpSupprBuffer[fuGet4Nr].begin() );

      for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
      {
         FillHitInfo( fvmEpSupprBuffer[ fuGet4Nr ][ iMsgIdx ] );
      } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )

      fvmEpSupprBuffer[fuGet4Nr].clear();
   } // if( 0 < fvmEpSupprBuffer[fGet4Nr] )
*/
}

void CbmTofStarEventBuilder2018::PrintSlcInfo(gdpb::Message mess)
{
   /// Nothing to do, maybe later use it to trakc parameter changes like treshold?
/*
  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
     LOG(info) << "GET4 Slow Control message, epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
                << "\n"
                << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                << ", Chan = " << std::setw(1) << mess.getGdpbSlcChan()
                << ", Edge = " << std::setw(1) << mess.getGdpbSlcEdge()
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType()
                << ", Data = " << std::hex << std::setw(6) << mess.getGdpbSlcData() << std::dec
                << ", Type = " << mess.getGdpbSlcCrc();
*/
   gdpb::FullMessage fullMess( mess, fvulCurrentEpoch[ fuGet4Nr ] );
   if( kTRUE == fbEventBuilding )
   {
      if( fuCurrentMs < fuCoreMs )
         fvmTsLinksBuffer[fuGdpbNr].push_back( fullMess );
         else fvmTsOverLinksBuffer[fuGdpbNr].push_back( fullMess );
   } // if( kTRUE == fbEventBuilding )
      else
         fvmCurrentLinkBuffer.push_back( fullMess );

   /// TODO FIXME: Add the error messages to the buffer for inclusion in the STAR event
   ///             NEEDS the proper "<" operator in FullMessage or Message to allow time sorting
}

void CbmTofStarEventBuilder2018::PrintGenInfo(gdpb::Message mess)
{
   Int_t mType        = mess.getMessageType();
   Int_t rocId          = mess.getRocNumber();
   Int_t get4Id     = mess.getGdpbGenChipId();
   Int_t channel    = mess.getGdpbHitChanId();
   uint64_t            uData = mess.getData();
   if(100 > iMess++)
      LOG(info) << "Get4 MSG type "<<mType<<" from rocId "<<rocId<<", getId "<<get4Id
                << ", (hit channel) "<<channel
//                <<Form(" hex data %0llx ",uData);
                << " hex data " << FormatHexPrintout( uData, 0, '0' );
}

void CbmTofStarEventBuilder2018::PrintSysInfo(gdpb::Message mess)
{
/*
      LOG(info) << "GET4 System message,       epoch " << (fvulCurrentEpoch[ fuGet4Nr])
                   << ", time " << std::setprecision(9) << std::fixed
                   << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                   << " for board ID " << std::hex << std::setw(4) << fuGdpbId << std::dec;
*/

   switch( mess.getGdpbSysSubType() )
   {
      case gdpb::SYSMSG_GET4_EVENT:
      {
/*
            LOG(info) << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                      << ", Chan = " << std::setw(1) << mess.getGdpbSysErrChanId()
                      << ", Edge = " << std::setw(1) << mess.getGdpbSysErrEdge()
                      << ", Empt = " << std::setw(1) << mess.getGdpbSysErrUnused()
                      << ", Data = " << std::hex << std::setw(2) << mess.getGdpbSysErrData() << std::dec
                      << " -- GET4 V1 Error Event";
*/
         /// TODO FIXME: Add the error messages to the buffer for inclusion in the STAR event
         ///             NEEDS the proper "<" operator in FullMessage or Message to allow time sorting
         gdpb::FullMessage fullMess( mess, fvulCurrentEpoch[ fuGet4Nr ] );
         if( kTRUE == fbEventBuilding )
         {
            if( fuCurrentMs < fuCoreMs )
               fvmTsLinksBuffer[fuGdpbNr].push_back( fullMess );
               else fvmTsOverLinksBuffer[fuGdpbNr].push_back( fullMess );
         } // if( kTRUE == fbEventBuilding )
            else
               fvmCurrentLinkBuffer.push_back( fullMess );
         break;
      } //
      case gdpb::SYSMSG_CLOSYSYNC_ERROR:
         if(100 > iMess++)
            LOG(info) << "Closy synchronization error";
         break;
      case gdpb::SYSMSG_TS156_SYNC:
         if(100 > iMess++)
            LOG(info) << "156.25MHz timestamp reset";
         break;
      case gdpb::SYSMSG_GDPB_UNKWN:
         if(100 > iMess++)
            LOG(info) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                      << mess.getGdpbSysUnkwData() << std::dec;
         break;
   } // switch( getGdpbSysSubType() )
}

void CbmTofStarEventBuilder2018::FillStarTrigInfo(gdpb::Message mess)
{
   Int_t iMsgIndex = mess.getStarTrigMsgIndex();

   switch( iMsgIndex )
   {
      case 0:
         fhTokenMsgType[fuGdpbNr]->Fill(0);
         fulGdpbTsMsb[fuGdpbNr] = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fhTokenMsgType[fuGdpbNr]->Fill(1);
         fulGdpbTsLsb[fuGdpbNr] = mess.getGdpbTsLsbStarB();
         fulStarTsMsb[fuGdpbNr] = mess.getStarTsMsbStarB();
         break;
      case 2:
         fhTokenMsgType[fuGdpbNr]->Fill(2);
         fulStarTsMid[fuGdpbNr] = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         fhTokenMsgType[fuGdpbNr]->Fill(3);

         ULong64_t ulNewGdpbTsFull = ( fulGdpbTsMsb[fuGdpbNr] << 24 )
                                   + ( fulGdpbTsLsb[fuGdpbNr]       );
         ULong64_t ulNewStarTsFull = ( fulStarTsMsb[fuGdpbNr] << 48 )
                                   + ( fulStarTsMid[fuGdpbNr] <<  8 )
                                   + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();

/*
         UInt_t uNewTrigWord =  ( (uNewTrigCmd & 0x00F) << 16 )
                  + ( (uNewDaqCmd   & 0x00F) << 12 )
                  + ( (uNewToken    & 0xFFF)       );
         LOG(info) << "New STAR trigger "
                   << " TS " << fulCurrentTsIndex
                   << " gDBB #" << fuGdpbNr << " "
                   << Form("token = %5u ", uNewToken )
                   << Form("gDPB ts  = %12llu ", ulNewGdpbTsFull )
                   << Form("STAR ts = %12llu ", ulNewStarTsFull )
                   << Form("DAQ cmd = %2u ", uNewDaqCmd )
                   << Form("TRG cmd = %2u ", uNewTrigCmd )
                   << Form("TRG Wrd = %5x ", uNewTrigWord );
*/

         if( ( uNewToken == fuStarTokenLast[fuGdpbNr] ) && ( ulNewGdpbTsFull == fulGdpbTsFullLast[fuGdpbNr] ) &&
             ( ulNewStarTsFull == fulStarTsFullLast[fuGdpbNr] ) && ( uNewDaqCmd == fuStarDaqCmdLast[fuGdpbNr] ) &&
             ( uNewTrigCmd == fuStarTrigCmdLast[fuGdpbNr] ) )
         {
            UInt_t uTrigWord =  ( (fuStarTrigCmdLast[fuGdpbNr] & 0x00F) << 16 )
                     + ( (fuStarDaqCmdLast[fuGdpbNr]   & 0x00F) << 12 )
                     + ( (fuStarTokenLast[fuGdpbNr]    & 0xFFF)       );
            LOG(warning) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << " TS " << fulCurrentTsIndex
                         << " gDBB #" << fuGdpbNr << " "
                         << Form("token = %5u ", fuStarTokenLast[fuGdpbNr] )
                         << Form("gDPB ts  = %12llu ", fulGdpbTsFullLast[fuGdpbNr] )
                         << Form("STAR ts = %12llu ", fulStarTsFullLast[fuGdpbNr] )
                         << Form("DAQ cmd = %2u ", fuStarDaqCmdLast[fuGdpbNr] )
                         << Form("TRG cmd = %2u ", fuStarTrigCmdLast[fuGdpbNr] )
                         << Form("TRG Wrd = %5x ", uTrigWord );
            return;
         } // if exactly same message repeated
/*
         if( (uNewToken != fuStarTokenLast[fuGdpbNr] + 1) &&
             0 < fulGdpbTsFullLast[fuGdpbNr] && 0 < fulStarTsFullLast[fuGdpbNr] &&
             ( 4095 != fuStarTokenLast[fuGdpbNr] || 1 != uNewToken)  )
            LOG(warning) << "Possible error: STAR token did not increase by exactly 1! "
                         << " gDBB #" << fuGdpbNr << " "
                         << Form("old = %5u vs new = %5u ", fuStarTokenLast[fuGdpbNr],   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fulGdpbTsFullLast[fuGdpbNr], ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fulStarTsFullLast[fuGdpbNr], ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fuStarDaqCmdLast[fuGdpbNr],  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fuStarTrigCmdLast[fuGdpbNr], uNewTrigCmd);
*/
         // GDPB TS counter reset detection
         if( ulNewGdpbTsFull < fulGdpbTsFullLast[fuGdpbNr] )
            LOG(debug) << "Probable reset of the GDPB TS: old = " << Form("%16llu", fulGdpbTsFullLast[fuGdpbNr])
                       << " new = " << Form("%16llu", ulNewGdpbTsFull)
                       << " Diff = -" << Form("%8llu", fulGdpbTsFullLast[fuGdpbNr] - ulNewGdpbTsFull)
                       << " GDPB #" << Form( "%2u", fuGdpbNr);

         // STAR TS counter reset detection
         if( ulNewStarTsFull < fulStarTsFullLast[fuGdpbNr] )
            LOG(debug) << "Probable reset of the STAR TS: old = " << Form("%16llu", fulStarTsFullLast[fuGdpbNr])
                       << " new = " << Form("%16llu", ulNewStarTsFull)
                       << " Diff = -" << Form("%8llu", fulStarTsFullLast[fuGdpbNr] - ulNewStarTsFull)
                       << " GDPB #" << Form( "%2u", fuGdpbNr);

/*
         LOG(info) << "Updating  trigger token for " << fuGdpbNr
                   << " " << fuStarTokenLast[fuGdpbNr] << " " << uNewToken;
*/
//         ULong64_t ulGdpbTsDiff = ulNewGdpbTsFull - fulGdpbTsFullLast[fuGdpbNr];
         fulGdpbTsFullLast[fuGdpbNr] = ulNewGdpbTsFull;
         fulStarTsFullLast[fuGdpbNr] = ulNewStarTsFull;
         fuStarTokenLast[fuGdpbNr]   = uNewToken;
         fuStarDaqCmdLast[fuGdpbNr]  = uNewDaqCmd;
         fuStarTrigCmdLast[fuGdpbNr] = uNewTrigCmd;

         /// Histograms filling only in core MS
         if( fuCurrentMs < fuCoreMs  )
         {
            /// In Run rate evolution
            if( 0 <= fdStartTime )
            {
               /// Reset the evolution Histogram and the start time when we reach the end of the range
               if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime) )
               {
                  ResetEvolutionHistograms();
                  fdStartTime = fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs;
               } // if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast * get4v2x::kdClockCycleSizeNs - fdStartTime) )

               fhTriggerRate[fuGdpbNr]->Fill( 1e-9 * ( fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ) );
               fhStarTokenEvo[fuGdpbNr]->Fill( 1e-9 * ( fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ),
                                               fuStarTokenLast[fuGdpbNr] );
               fhStarTrigGdpbTsEvo[fuGdpbNr]->Fill( 1e-9 * ( fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ),
                                               fulGdpbTsFullLast[fuGdpbNr] );
               fhStarTrigStarTsEvo[fuGdpbNr]->Fill( 1e-9 * ( fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime ),
                                               fulStarTsFullLast[fuGdpbNr] );
            } // if( 0 < fdStartTime )
               else fdStartTime = fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs;

            /// In Run rate long evolution
            if( 0 <= fdStartTimeLong )
            {
               /// Reset the evolution Histogram and the start time when we reach the end of the range
               if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTime) / 60.0 )
               {
                  ResetLongEvolutionHistograms();
                  fdStartTimeLong = fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs;
               } // if( fuHistoryHistoSize < 1e-9 * (fulGdpbTsFullLast * get4v2x::kdClockCycleSizeNs - fdStartTime) / 60.0 )
               fhTriggerRateLong[fuGdpbNr]->Fill( 1e-9 * ( fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs - fdStartTimeLong ) / 60.0,
                                                  1 / 60.0 );
            } // if( 0 < fdStartTimeLong )
               else fdStartTimeLong = fulGdpbTsFullLast[fuGdpbNr] * get4v2x::kdClockCycleSizeNs;

            fhCmdDaqVsTrig[fuGdpbNr]->Fill( fuStarDaqCmdLast[fuGdpbNr], fuStarTrigCmdLast[fuGdpbNr] );
         } // if( fuCurrentMs < fuCoreMs  )

         /// Generate Trigger object and store it for event building ///
         CbmTofStarTrigger2018 newTrig( fulGdpbTsFullLast[fuGdpbNr], fulStarTsFullLast[fuGdpbNr], fuStarTokenLast[fuGdpbNr],
                                        fuStarDaqCmdLast[fuGdpbNr], fuStarTrigCmdLast[fuGdpbNr] );
         if( kTRUE == fbEventBuilding )
         {
            if( fuCurrentMs < fuCoreMs )
               fvtTsLinksBuffer[fuGdpbNr].push_back( newTrig );
               else fvtTsOverLinksBuffer[fuGdpbNr].push_back( newTrig );
         } // if( kTRUE == fbEventBuilding )
            else
               fvtCurrentLinkBuffer.push_back( newTrig );
         ///---------------------------------------------------------///
         break;
      } // case 3
      default:
         LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
   } // switch( iMsgIndex )
}

void CbmTofStarEventBuilder2018::Reset()
{
}

void CbmTofStarEventBuilder2018::FillOutput(boost::any)
{
}

void CbmTofStarEventBuilder2018::Finish()
{
   TString message_type;

   for( UInt_t uType = 0; uType < fMsgCounter.size(); ++uType)
   {
      switch(uType)
      {
         case 0:   message_type ="NOP"; break;
         case 1:   message_type ="HIT"; break;
         case 2:   message_type ="EPOCH"; break;
         case 3:   message_type ="SYNC"; break;
         case 4:   message_type ="AUX"; break;
         case 5:   message_type ="EPOCH2"; break;
         case 6:   message_type ="GET4"; break;
         case 7:   message_type ="SYS"; break;
         case 8:   message_type ="GET4_SLC"; break;
         case 9:   message_type ="GET4_32B"; break;
         case 10:  message_type ="GET4_SYS"; break;
         default:  message_type ="UNKNOWN"; break;
      } // switch(uType)
      LOG(info) << message_type << " messages: "
                << fMsgCounter[uType];
   } // for( UInt_t uType = 0; uType < fMsgCounter.size(); ++uType)

   LOG(info) << "-------------------------------------";
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
      for( UInt_t uGet4 = 0; uGet4 < fuNrOfGet4PerGdpb; ++uGet4 )
         LOG(info) << "Last epoch for gDPB: "<< std::setw(4) << uGdpb
                   << " , GET4  " << std::setw(4) << uGet4
                   << " => " << fvulCurrentEpoch[GetArrayIndex(uGdpb, uGet4)];
   LOG(info) << "-------------------------------------";

   if( NULL != fpBinDumpFile )
   {
      LOG(info) << "Closing binary file used for event dump.";
      fpBinDumpFile->close();
   } // if( NULL != fpBinDumpFile )

  SaveAllHistos();

}

void CbmTofStarEventBuilder2018::SaveAllHistos( TString sFileName )
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
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      /// Token detection
      fhTokenMsgType[ uGdpb ]->Write();
      fhTriggerRate[ uGdpb ]->Write();
      fhTriggerRateLong[ uGdpb ]->Write();
      fhCmdDaqVsTrig[ uGdpb ]->Write();
      fhStarTokenEvo[ uGdpb ]->Write();
      fhStarTrigGdpbTsEvo[ uGdpb ]->Write();
      fhStarTrigStarTsEvo[ uGdpb ]->Write();

      /// Event building
      fhStarHitToTrigAll_gDPB[ uGdpb ]->Write();
      fhStarHitToTrigWin_gDPB[ uGdpb ]->Write();
      fhStarHitToTrigAllTime_gDPB[ uGdpb ]->Write();
      fhStarHitToTrigWinTime_gDPB[ uGdpb ]->Write();
      fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ]->Write();
      fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ]->Write();

      if( kFALSE == fbEventBuilding )
      {
         fhStarEventSize_gDPB[ uGdpb ]->Write();
         fhStarEventSizeTime_gDPB[ uGdpb ]->Write();
         fhStarEventSizeTimeLong_gDPB[ uGdpb ]->Write();
      } // if( kFALSE == fbEventBuilding )
         else fhStarTrigTimeToMeanTrig_gDPB[ uGdpb ]->Write();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

   if( kTRUE == fbEventBuilding )
   {
      fhStarEventSize->Write();
      fhStarEventSizeTime->Write();
      fhStarEventSizeTimeLong->Write();
   } // if( kTRUE == fbEventBuilding )

   fhStarTsProcessTime->Write();
   fhStarTsProcessTimeShort->Write();
   fhStarTsInterprocessTime->Write();
   gDirectory->cd("..");

   // Plots monitoring the TS losses
   gDirectory->mkdir("Flib_Raw");
   gDirectory->cd("Flib_Raw");
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

void CbmTofStarEventBuilder2018::ResetAllHistos()
{
   LOG(info) << "Reseting all Event building histograms.";

   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      /// Token detection
      fhTokenMsgType[ uGdpb ]->Reset();
      fhTriggerRate[ uGdpb ]->Reset();
      fhTriggerRateLong[ uGdpb ]->Reset();
      fhCmdDaqVsTrig[ uGdpb ]->Reset();
      fhStarTokenEvo[ uGdpb ]->Reset();
      fhStarTrigGdpbTsEvo[ uGdpb ]->Reset();
      fhStarTrigStarTsEvo[ uGdpb ]->Reset();

      /// Event building
      fhStarHitToTrigAll_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigWin_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigAllTime_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigWinTime_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ]->Reset();

      if( kFALSE == fbEventBuilding )
      {
         fhStarEventSize_gDPB[ uGdpb ]->Reset();
         fhStarEventSizeTime_gDPB[ uGdpb ]->Reset();
         fhStarEventSizeTimeLong_gDPB[ uGdpb ]->Reset();
      } // if( kFALSE == fbEventBuilding )
         else fhStarTrigTimeToMeanTrig_gDPB[ uGdpb ]->Reset();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

   if( kTRUE == fbEventBuilding )
   {
      fhStarEventSize->Reset();
      fhStarEventSizeTime->Reset();
      fhStarEventSizeTimeLong->Reset();
   } // if( kTRUE == fbEventBuilding )
   fhStarTsProcessTime->Reset();
   fhStarTsProcessTimeShort->Reset();
   fhStarTsInterprocessTime->Reset();

   fdStartTime = -1;
   fdStartTimeLong = -1;
   fdStartTimeMsSz = -1;
}
void CbmTofStarEventBuilder2018::ResetEvolutionHistograms()
{
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fhTriggerRate[ uGdpb ]->Reset();
      fhStarTokenEvo[ uGdpb ]->Reset();
      fhStarTrigGdpbTsEvo[ uGdpb ]->Reset();
      fhStarTrigStarTsEvo[ uGdpb ]->Reset();

      /// Event building
      fhStarHitToTrigAllTime_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigWinTime_gDPB[ uGdpb ]->Reset();

      if( kFALSE == fbEventBuilding )
      {
         fhStarEventSizeTime_gDPB[ uGdpb ]->Reset();
      } // if( kFALSE == fbEventBuilding )
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

   if( kTRUE == fbEventBuilding )
   {
      fhStarEventSizeTime->Reset();
   } // if( kTRUE == fbEventBuilding )

   fdStartTime = -1;
}
void CbmTofStarEventBuilder2018::ResetLongEvolutionHistograms()
{
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )
   {
      fhTriggerRate[ uGdpb ]->Reset();

      /// Event building
      fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ]->Reset();
      fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ]->Reset();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb )

   if( kFALSE == fbEventBuilding )
   {
      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      {
         fhStarEventSizeTimeLong_gDPB[ uGdpb ]->Reset();
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   } // if( kFALSE == fbEventBuilding )
      else
      {
         fhStarEventSizeTimeLong->Reset();
      } // else of if( kFALSE == fbEventBuilding )
   fdStartTimeLong = -1;
}

void CbmTofStarEventBuilder2018::BuildStarEventsSingleLink()
{
   /// Time sort the message buffer
   /// TODO: Check if using an epoch buffer per gDPB similar to the epoch suppression one does not make
   ///       a more efficient sorting (max disorder is within the epoch!)
   /// Data are sorted between epochs, not inside => Epoch level ordering
   /// Sorting at lower bin precision level
   std::stable_sort( fvmCurrentLinkBuffer.begin(),     fvmCurrentLinkBuffer.begin() );

   std::vector< gdpb::FullMessage >::iterator itFirstMessageNextEvent   = fvmCurrentLinkBuffer.begin();
   Double_t dPrevEventEnd = 0.0;

   for( UInt_t uTriggIdx = 0; uTriggIdx < fvtCurrentLinkBuffer.size(); uTriggIdx++ )
   {
      Double_t dTriggerTime   = get4v2x::kdClockCycleSizeNs * fvtCurrentLinkBuffer[uTriggIdx].GetFullGdpbTs();
      Double_t dTriggerWinBeg = dTriggerTime - fdStarTriggerDelay[ fuGdpbNr ];
      Double_t dTriggerWinEnd = dTriggerTime - fdStarTriggerDelay[ fuGdpbNr ] + fdStarTriggerWinSize[ fuGdpbNr ];
      Double_t dClosestTriggerWinStart = dTriggerTime - fdStarTriggerDelay[ fuGdpbNr ] + fdStarTriggerDeadtime[ fuGdpbNr ];
      Bool_t bFirstMessClosestEventFound = kFALSE;

      /// Associate this trigger to its subevent
      fStarSubEvent.SetTrigger( fvtCurrentLinkBuffer[uTriggIdx] );
      /// Set the source index of the subevent
      fStarSubEvent.SetSource( fuGdpbNr );

      /// Check if this trigger may correspond to an event with data in previous MS
      if( dTriggerWinBeg < fdCurrentMsStartTime )
         fStarSubEvent.SetStartBorderEventFlag();

      /// Check if this trigger may correspond to an event with data in next MS
      if( fdCurrentMsEndTime < dTriggerWinEnd )
         fStarSubEvent.SetEndBorderEventFlag();

      /// Check if this trigger leads to an event overlapping with the previous one
      if( 0 < dPrevEventEnd && dTriggerWinBeg < dPrevEventEnd )
         fStarSubEvent.SetOverlapEventFlag();

      for( std::vector< gdpb::FullMessage >::iterator itMess = itFirstMessageNextEvent;
           itMess != fvmCurrentLinkBuffer.end();
           ++ itMess )
      {
         Double_t dMessageFullTime = (*itMess).GetFullTimeNs();
         if( kFALSE == bFirstMessClosestEventFound &&
             dClosestTriggerWinStart < dMessageFullTime )
         {
            itFirstMessageNextEvent = itMess;
            bFirstMessClosestEventFound = kTRUE;
         } // If first possible hit of closest event not found and current hit fits

         /// Plotting of time to trigger for all hits
         Double_t dTimeToTrigg = dMessageFullTime - dTriggerTime;
         fhStarHitToTrigAll_gDPB[ fuGdpbNr ]->Fill( dTimeToTrigg );
         fhStarHitToTrigAllTime_gDPB[ fuGdpbNr ]->Fill( 1e-9 *(dTriggerTime - fdStartTime), dTimeToTrigg );
         fhStarHitToTrigAllTimeLong_gDPB[ fuGdpbNr ]->Fill( 1e-9 *(dTriggerTime - fdStartTimeLong) / 60.0, dTimeToTrigg );

         if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
         {
            /// Message belongs to this event
            fStarSubEvent.AddMsg( (*itMess) );

            /// Plotting of time to trigger for hits within the event window
            fhStarHitToTrigWin_gDPB[ fuGdpbNr ]->Fill( dTimeToTrigg );
            fhStarHitToTrigWinTime_gDPB[ fuGdpbNr ]->Fill( 1e-9 *(dTriggerTime - fdStartTime), dTimeToTrigg );
            fhStarHitToTrigWinTimeLong_gDPB[ fuGdpbNr ]->Fill( 1e-9 *(dTriggerTime - fdStartTimeLong) / 60.0, dTimeToTrigg );
         } // if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
            else if( dTriggerWinEnd < dMessageFullTime )
               /// First Message out of the window for this event => Stop there and go to the next
               break;
      } // Loop on message from first allowed by last event + deadtime to end

      /** TODO: clarify how we deal with multiple sub-events (eg one for each gDPB) **/
      /// Send the sub-event to the STAR systems
      Int_t  iBuffSzByte = 0;
      void * pDataBuff = fStarSubEvent.BuildOutput( iBuffSzByte );
      if( NULL != pDataBuff )
      {
         /// Valid output, do stuff with it!
         fulNbBuiltSubEvent++;

#ifdef STAR_SUBEVT_BUILDER
         /*
          ** Function to send sub-event block to the STAR DAQ system
          *       trg_word received is packed as:
          *
          *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
          */
         star_rhicf_write( fStarSubEvent.GetTrigger().GetStarTrigerWord(),
                           pDataBuff, iBuffSzByte );
         fulNbStarSubEvent++;
/*
         LOG(info) << "Sent STAR event with size " << iBuffSzByte << " Bytes"
                   << " and token " << fStarSubEvent.GetTrigger().GetStarToken();
*/
#endif // STAR_SUBEVT_BUILDER

         if( kTRUE == fbEventDumpEna )
         {
            fpBinDumpFile->write( reinterpret_cast< const char * >( &kuBinDumpBegWord ), sizeof( UInt_t ) );
            fpBinDumpFile->write( reinterpret_cast< const char * >( &iBuffSzByte ), sizeof( Int_t ) );
            fpBinDumpFile->write( reinterpret_cast< const char * >( pDataBuff ), iBuffSzByte );
            fpBinDumpFile->write( reinterpret_cast< const char * >( &kuBinDumpEndWord ), sizeof( UInt_t ) );
         } // if( kTRUE == fbEventDumpEna )
      } // if( NULL != pDataBuff )
         else LOG(error) << "Invalid STAR SubEvent Output, can only happen if trigger "
                         << " object was not set => Do Nothing more with it!!! ";

      /// Fill plot of event size
      fhStarEventSize_gDPB[ fuGdpbNr ]->Fill( iBuffSzByte );
      /// Fill plot of event size as function of trigger time
      if( 0 < fdStartTime )
         fhStarEventSizeTime_gDPB[ fuGdpbNr ]->Fill( 1e-9 *(dTriggerTime - fdStartTime), iBuffSzByte );
      if( 0 < fdStartTimeLong )
         fhStarEventSizeTimeLong_gDPB[ fuGdpbNr ]->Fill( 1e-9 *(dTriggerTime - fdStartTimeLong) / 60.0, iBuffSzByte );

      /// Now clear the sub-event
      fStarSubEvent.ClearSubEvent();
   } // for( UInt_t uTriggIdx = 0; uTriggIdx < fvtCurrentLinkBuffer.size(); uTriggIdx++ )

   /// catch case of MS without triggers
   if( 0 == fvtCurrentLinkBuffer.size() )
      itFirstMessageNextEvent   = fvmCurrentLinkBuffer.end();

   /// Remove message until the first which could fit in an event in next MS
   fvmCurrentLinkBuffer.erase( fvmCurrentLinkBuffer.begin(), itFirstMessageNextEvent);

   /// Clear trigger buffer
   fvtCurrentLinkBuffer.clear();

}
void CbmTofStarEventBuilder2018::BuildStarEventsAllLinks()
{
   /// TODO:
   /// 1) use overlap MS to fill events where the event window would go over the lower edge of the TS

   /// First check if any trigger was present in this TS
   Bool_t bEmptyTs = kTRUE;
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      if( 0 < fvtTsLinksBuffer[uGdpb].size() )
      {
         bEmptyTs = kFALSE;
         break;
      } // if( 0 < fvtTsLinksBuffer[0].size() )
   if( kTRUE == bEmptyTs )
   {
      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      {
         fvmTsLinksBuffer[uGdpb].clear();
         fvmTsOverLinksBuffer[uGdpb].clear();
         fvtTsOverLinksBuffer[uGdpb].clear();
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      return;
   } // if( kTRUE == bEmptyTS )
/*
   LOG(info) << "--------------------------------";
   LOG(info) << " TS " << fulCurrentTsIndex;

   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      for( UInt_t uTrig = 0; uTrig < fvtTsLinksBuffer[uGdpb].size(); uTrig ++)
         LOG(info) << "board " << uGdpb << " Trig " << std::setw(2) << uTrig
                   << " TS g " << std::setw(15) << fvtTsLinksBuffer[uGdpb][uTrig].GetFullGdpbTs()
                   << " TS s " << std::setw(15) << fvtTsLinksBuffer[uGdpb][uTrig].GetFullStarTs()
                   << " Trigger " << std::hex << std::setw(5) << fvtTsLinksBuffer[uGdpb][uTrig].GetStarTrigerWord()
                   << std::dec;
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
*/

   /// Time sort the message buffers
   /// TODO: Check if using an epoch buffer per gDPB similar to the epoch suppression one does not make
   ///       a more efficient sorting (max disorder is within the epoch!)
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      /// Data are sorted between epochs, not inside => Epoch level ordering
      /// Sorting at lower bin precision level
      std::stable_sort( fvmTsLinksBuffer[uGdpb].begin(),     fvmTsLinksBuffer[uGdpb].begin() );
      std::stable_sort( fvmTsOverLinksBuffer[uGdpb].begin(), fvmTsOverLinksBuffer[uGdpb].begin() );
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

   /// Initialize the iterator for first possible message in event to beginning of buffer
   std::vector< std::vector< gdpb::FullMessage >::iterator > itFirstMessageNextEvent( fuNrOfGdpbs );
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      itFirstMessageNextEvent[ uGdpb ] = fvmTsLinksBuffer[ uGdpb ].begin();

   /// Initialize the time of previous event end to 0
   std::vector< Double_t > dPrevEventEnd( fuNrOfGdpbs, 0.0 );

   /// Checking whether all Trigger buffers start with the same trigger
   Bool_t bFirstTriggerMatch = kTRUE;
   std::vector< UInt_t > vuIndexMatchingTrigger(fuNrOfGdpbs);
   if( 0 < fvtTsLinksBuffer[0].size() )
   {
      vuIndexMatchingTrigger[0] = 0;
//      ULong64_t ulFirstBoardFirstTriggerTime = fvtTsLinksBuffer[0][0].GetFullGdpbTs();
      UInt_t    uFirstBoardFirstTriggerWord  = fvtTsLinksBuffer[0][0].GetStarTrigerWord();

      for( UInt_t uGdpb = 1; uGdpb < fuNrOfGdpbs; uGdpb ++)
      {
         if( 0 < fvtTsLinksBuffer[uGdpb].size() )
         {
            vuIndexMatchingTrigger[uGdpb] = 0;
            if( fvtTsLinksBuffer[uGdpb][0].GetStarTrigerWord() != uFirstBoardFirstTriggerWord )
            {
//               LOG(warning) << "Trigger word not matching for board " << uGdpb << ": "
//                            << fvtTsLinksBuffer[uGdpb][0].GetStarTrigerWord() << " VS "
//                            << uFirstBoardFirstTriggerWord;
               bFirstTriggerMatch = kFALSE;
               break;
            } // if( fvtTsLinksBuffer[uGdpb][0].GetStarTrigerWord() != uFirstBoardFirstTriggerWord )
         } // if( 0 < fvtTsLinksBuffer[0].size() )
            else
            {
//               LOG(warning) << "No trigger in board " << uGdpb;
               bFirstTriggerMatch = kFALSE;
            }
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   } // if( 0 < fvtTsLinksBuffer[0].size() )
      else
      {
//         LOG(warning) << "No trigger in first board!";
         bFirstTriggerMatch = kFALSE;
      } // else of if( 0 < fvtTsLinksBuffer[0].size() )

   if( kFALSE == bFirstTriggerMatch )
   {
//      LOG(error) << "First trigger in TS not the same for all boards!";
      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      {
//         LOG(error) << "Board: " << uGdpb
//                    << " has " << fvtTsLinksBuffer[uGdpb].size()
//                    << " triggers";
         fvtTsLinksBuffer[uGdpb].clear();
         fvmTsLinksBuffer[uGdpb].clear();
         fvmTsOverLinksBuffer[uGdpb].clear();
         fvtTsOverLinksBuffer[uGdpb].clear();
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      return;
   } // if( kFALSE == bFirstTriggerMatch )

   /// We now have a matching trigger for starting
   Bool_t bMatchingTriggerFound = kTRUE;
   while( kTRUE == bMatchingTriggerFound )
   {
      Double_t dMeanTriggerGdpbTs = 0;
      Double_t dMeanTriggerStarTs = 0;
      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      {
         dMeanTriggerGdpbTs += get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs();
         dMeanTriggerStarTs += get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullStarTs();

         /// Recreate the corresponding messages and insert them into the event bufffer
         std::vector< gdpb::FullMessage > vTrigMess = fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetGdpbMessages( fUnpackPar->GetRocId( uGdpb ) );
         for( std::vector< gdpb::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
         {
            fStarSubEvent.AddMsg( (*itMess) );
//            std::cout << Form( "Event %6llu Trigger message for gDPB %02u is %016lx ", fulNbBuiltSubEvent, uGdpb, (*itMess).getData() ) << std::endl;
         } // for( std::vector< gdpb::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
/*
         ULong64_t testulGdpbTsMsb = vTrigMess[0].getGdpbTsMsbStarA();
         ULong64_t testulGdpbTsLsb = vTrigMess[1].getGdpbTsLsbStarB();
         ULong64_t testulStarTsMsb = vTrigMess[1].getStarTsMsbStarB();
         ULong64_t testulStarTsMid = vTrigMess[2].getStarTsMidStarC();
         ULong64_t ulNewGdpbTsFull = ( testulGdpbTsMsb << 24 )
                                   + ( testulGdpbTsLsb       );
         ULong64_t ulNewStarTsFull = ( testulStarTsMsb << 48 )
                                   + ( testulStarTsMid <<  8 )
                                   + vTrigMess[3].getStarTsLsbStarD();
         UInt_t uNewToken   = vTrigMess[3].getStarTokenStarD();
         UInt_t uNewDaqCmd  = vTrigMess[3].getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = vTrigMess[3].getStarTrigCmdStarD();

         std::cout << Form( "Event %6llu Trigger message for gDPB %02u check: %1u %1u %1u %1u %1u ", fulNbBuiltSubEvent, uGdpb,
                            ulNewGdpbTsFull == fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs(),
                            ulNewStarTsFull == fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullStarTs(),
                            uNewToken   == fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarToken(),
                            uNewDaqCmd  == fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarDaqCmd(),
                            uNewTrigCmd == fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarTrigCmd() )
                   << std::endl;
*/
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      dMeanTriggerGdpbTs /= fuNrOfGdpbs;
      dMeanTriggerStarTs /= fuNrOfGdpbs;
      Double_t dMeanTriggTimeToStartSec = 1e-9 *(dMeanTriggerGdpbTs - fdStartTime);
      Double_t dMeanTriggTimeToStartLongSec = 1e-9 *(dMeanTriggerGdpbTs - fdStartTimeLong) / 60.0;

      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
         fhStarTrigTimeToMeanTrig_gDPB[ uGdpb ]->Fill(
                     get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs()
                   - dMeanTriggerGdpbTs );

      CbmTofStarTrigger2018 meanTrigger( static_cast< ULong64_t >( dMeanTriggerGdpbTs ), static_cast< ULong64_t >( dMeanTriggerStarTs ),
                                         fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarToken(),
                                         fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarDaqCmd(),
                                         fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarTrigCmd() );

      /// Associate this trigger to its subevent
      fStarSubEvent.SetTrigger( meanTrigger );
      /// Set the source index of the subevent
      fStarSubEvent.SetSource( 16 );

      for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
      {

         Double_t dTriggerTime   = get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs();
         Double_t dTriggerWinBeg = dTriggerTime - fdStarTriggerDelay[ uGdpb ];
         Double_t dTriggerWinEnd = dTriggerTime - fdStarTriggerDelay[ uGdpb ] + fdStarTriggerWinSize[ uGdpb ];
         Double_t dClosestTriggerWinStart = dTriggerTime - fdStarTriggerDelay[ uGdpb ] + fdStarTriggerDeadtime[ uGdpb ];
         Bool_t bFirstMessClosestEventFound = kFALSE;

         /// Check if this trigger may correspond to an event with data in previous TS
         if( dTriggerWinBeg < fdCurrentTsStartTime )
            fStarSubEvent.SetStartBorderEventFlag();

         /// Check if this trigger may correspond to an event with data in next TS
         if( fdCurrentTsCoreEndTime < dTriggerWinEnd )
            fStarSubEvent.SetEndBorderEventFlag();

         /// Check if this trigger leads to an event overlapping with the previous one
         if( 0 < dPrevEventEnd[ uGdpb ] && dTriggerWinBeg < dPrevEventEnd[ uGdpb ] )
            fStarSubEvent.SetOverlapEventFlag();

         for( std::vector< gdpb::FullMessage >::iterator itMess = itFirstMessageNextEvent[ uGdpb ];
              itMess != fvmTsLinksBuffer[ uGdpb ].end();
              ++ itMess )
         {
            Double_t dMessageFullTime = (*itMess).GetFullTimeNs();
            if( kFALSE == bFirstMessClosestEventFound &&
                dClosestTriggerWinStart < dMessageFullTime )
            {
               itFirstMessageNextEvent[ uGdpb ] = itMess;
               bFirstMessClosestEventFound = kTRUE;
            } // If first possible hit of closest event not found and current hit fits

            /// Plotting of time to trigger for all hits
            Double_t dTimeToTrigg = dMessageFullTime - dTriggerTime;
            fhStarHitToTrigAll_gDPB[ uGdpb ]->Fill( dTimeToTrigg );
            fhStarHitToTrigAllTime_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartSec, dTimeToTrigg );
            fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartLongSec, dTimeToTrigg );

            if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
            {
               /// Message belongs to this event
               fStarSubEvent.AddMsg( (*itMess) );

               /// Plotting of time to trigger for hits within the event window
               fhStarHitToTrigWin_gDPB[ uGdpb ]->Fill( dTimeToTrigg );
               fhStarHitToTrigWinTime_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartSec, dTimeToTrigg );
               fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartLongSec, dTimeToTrigg );
            } // if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
               else if( dTriggerWinEnd < dMessageFullTime )
                  /// First Message out of the window for this event => Stop there and go to the next
                  break;
         } // Loop on message from first allowed by last event + deadtime to end
      } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

      /// Time sort the sub-event if required by the user
      if( kTRUE == fbTimeSortOutput )
         fStarSubEvent.SortMessages(  );

      /// Send the sub-event to the STAR systems
      Int_t  iBuffSzByte = 0;
      void * pDataBuff = fStarSubEvent.BuildOutput( iBuffSzByte );
      if( NULL != pDataBuff )
      {
         /// Valid output, do stuff with it!
         fulNbBuiltSubEvent++;

#ifdef STAR_SUBEVT_BUILDER
         /*
          ** Function to send sub-event block to the STAR DAQ system
          *       trg_word received is packed as:
          *
          *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
          */
         star_rhicf_write( fStarSubEvent.GetTrigger().GetStarTrigerWord(),
                           pDataBuff, iBuffSzByte );
         fulNbStarSubEvent++;
/*
         LOG(info) << "Sent STAR event with size " << iBuffSzByte << " Bytes"
                   << " and token " << fStarSubEvent.GetTrigger().GetStarToken();
*/
#endif // STAR_SUBEVT_BUILDER

         if( kTRUE == fbEventDumpEna )
         {
            fpBinDumpFile->write( reinterpret_cast< const char * >( &kuBinDumpBegWord ), sizeof( UInt_t ) );
            fpBinDumpFile->write( reinterpret_cast< const char * >( &iBuffSzByte ), sizeof( Int_t ) );
            fpBinDumpFile->write( reinterpret_cast< const char * >( pDataBuff ), iBuffSzByte );
            fpBinDumpFile->write( reinterpret_cast< const char * >( &kuBinDumpEndWord ), sizeof( UInt_t ) );
         } // if( kTRUE == fbEventDumpEna )
      } // if( NULL != pDataBuff )
         else LOG(error) << "Invalid STAR SubEvent Output, can only happen if trigger "
                         << " object was not set => Do Nothing more with it!!! ";
/*
      if( 1000 <  iBuffSzByte )
         fStarSubEvent.PrintSubEvent();
*/
      /// Fill plot of event size
      fhStarEventSize->Fill( iBuffSzByte );
      /// Fill plot of event size as function of trigger time
      if( 0 < fdStartTime )
         fhStarEventSizeTime->Fill( dMeanTriggTimeToStartSec, iBuffSzByte );
      if( 0 < fdStartTimeLong )
         fhStarEventSizeTimeLong->Fill( dMeanTriggTimeToStartLongSec, iBuffSzByte );

      /// Now clear the sub-event
      fStarSubEvent.ClearSubEvent();

      /// Check if we still can find a Matching trigger
      vuIndexMatchingTrigger[0] ++;
      bMatchingTriggerFound = kTRUE;
      if( vuIndexMatchingTrigger[0] < fvtTsLinksBuffer[0].size() )
      {
//         ULong64_t ulFirstBoardNextTriggerTime = fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetFullGdpbTs();
         UInt_t    uFirstBoardNextTriggerWord  = fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarTrigerWord();
         for( UInt_t uGdpb = 1; uGdpb < fuNrOfGdpbs; uGdpb ++)
         {
            vuIndexMatchingTrigger[uGdpb] ++;
            if( vuIndexMatchingTrigger[uGdpb] < fvtTsLinksBuffer[uGdpb].size() )
            {
               if( fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarTrigerWord() != uFirstBoardNextTriggerWord )
               {
                  bMatchingTriggerFound = kFALSE;
                  break;
               } // if( fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarTrigerWord() != uFirstBoardNextTriggerWord )
            } // if( vuIndexMatchingTrigger[uGdpb] < fvtTsLinksBuffer[uGdpb].size() )
               else bMatchingTriggerFound = kFALSE;
         } // for( UInt_t uGdpb = 1; uGdpb < fuNrOfGdpbs; uGdpb ++)
      } // if( vuIndexMatchingTrigger[0] < fvtTsLinksBuffer[0].size() )
         else
         {
            bMatchingTriggerFound = kFALSE;
         } // else of if( 0 < fvtTsLinksBuffer[0].size() )

      /// If no matching trigger, check if matching trigger building not possible
      /// using the deadtime period of the overlap MS
      if( kFALSE == bMatchingTriggerFound )
      {
         /// First find out for which gDPBs the trigger is potentially in the overlap MS
         std::vector< Bool_t > bTriggerInOverMs( fuNrOfGdpbs, kFALSE );
         for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
            if( fvtTsLinksBuffer[uGdpb].size() < vuIndexMatchingTrigger[uGdpb] &&
                0 < fvtTsOverLinksBuffer[uGdpb].size() )
               bTriggerInOverMs[uGdpb] = kTRUE;

//         ULong64_t ulFirstBoardNextTriggerTime = 0;
         UInt_t    uFirstBoardNextTriggerWord  = 0;
         bFirstTriggerMatch = kTRUE;
         if( vuIndexMatchingTrigger[0] < fvtTsLinksBuffer[0].size() )
         {
//            ulFirstBoardNextTriggerTime = fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetFullGdpbTs();
            uFirstBoardNextTriggerWord  = fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarTrigerWord();
         } // if( vuIndexMatchingTrigger[0] < fvtTsLinksBuffer[0].size() )
            else if( bTriggerInOverMs[0] )
            {
//               ulFirstBoardNextTriggerTime = fvtTsOverLinksBuffer[0][ 0 ].GetFullGdpbTs();
               uFirstBoardNextTriggerWord  = fvtTsOverLinksBuffer[0][ 0 ].GetStarTrigerWord();
            } // else if( bTriggerInOverMs[0] ) of if( vuIndexMatchingTrigger[0] < fvtTsLinksBuffer[0].size() )
            else bFirstTriggerMatch = kFALSE;

         for( UInt_t uGdpb = 1; uGdpb < fuNrOfGdpbs; uGdpb ++)
         {
            if( vuIndexMatchingTrigger[uGdpb] < fvtTsLinksBuffer[uGdpb].size() )
            {
               /// Check if triggger words are matching
               if( fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarTrigerWord() != uFirstBoardNextTriggerWord )
               {
                  bFirstTriggerMatch = kFALSE;
                  break;
               } // if( fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetStarTrigerWord() != uFirstBoardNextTriggerWord )
            } // if( vuIndexMatchingTrigger[uGdpb] < fvtTsLinksBuffer[uGdpb].size() )
               else if( bTriggerInOverMs[uGdpb] )
               {
                  /// Check if triggger words are matching
                  if( fvtTsOverLinksBuffer[uGdpb][ 0 ].GetStarTrigerWord() != uFirstBoardNextTriggerWord )
                  {
                     bFirstTriggerMatch = kFALSE;
                     break;
                  } // if( fvtTsOverLinksBuffer[uGdpb][ 0 ].GetStarTrigerWord() != uFirstBoardNextTriggerWord )
                  /// Check if this trigger is still without the period declared as TS deadtime
                  if( fdCurrentTsCoreEndTime + fdTsDeadtimePeriod < fvtTsOverLinksBuffer[uGdpb][ 0 ].GetFullGdpbTs() )
                  {
                     bFirstTriggerMatch = kFALSE;
                     break;
                  } // if( fdCurrentTsCoreEndTime + fdTsDeadtimePeriod < fvtTsOverLinksBuffer[uGdpb][ 0 ].GetFullGdpbTs() )
               } // else if( bTriggerInOverMs[uGdpb] ) of if( vuIndexMatchingTrigger[uGdpb] < fvtTsLinksBuffer[uGdpb].size() )
               else bFirstTriggerMatch = kFALSE;
         } // for( UInt_t uGdpb = 1; uGdpb < fuNrOfGdpbs; uGdpb ++)

         if( bFirstTriggerMatch )
         {
            Double_t dMeanTriggerGdpbTsOver = 0;
            Double_t dMeanTriggerStarTsOver = 0;
            for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
            {
               if( bTriggerInOverMs[uGdpb] )
               {
                  dMeanTriggerGdpbTsOver += get4v2x::kdClockCycleSizeNs * fvtTsOverLinksBuffer[uGdpb][ 0 ].GetFullGdpbTs();
                  dMeanTriggerStarTsOver += get4v2x::kdClockCycleSizeNs * fvtTsOverLinksBuffer[uGdpb][ 0 ].GetFullStarTs();

                  /// Recreate the corresponding messages and insert them into the event bufffer
                  std::vector< gdpb::FullMessage > vTrigMess = fvtTsLinksBuffer[uGdpb][ 0 ].GetGdpbMessages( fUnpackPar->GetRocId( uGdpb ) );
                  for( std::vector< gdpb::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
                  {
                     fStarSubEvent.AddMsg( (*itMess) );
//                     std::cout << Form( "Event %6llu Trigger message in Overlap for gDPB %02u is %016lx ", fulNbBuiltSubEvent, uGdpb, (*itMess).getData() ) << std::endl;
                  } // for( std::vector< gdpb::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
               } // if( bTriggerInOverMs[uGdpb] )
                  else
                  {
                     dMeanTriggerGdpbTsOver += get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs();
                     dMeanTriggerStarTsOver += get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullStarTs();

                     /// Recreate the corresponding messages and insert them into the event bufffer
                     std::vector< gdpb::FullMessage > vTrigMess = fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetGdpbMessages( fUnpackPar->GetRocId( uGdpb ) );
                     for( std::vector< gdpb::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
                     {
                        fStarSubEvent.AddMsg( (*itMess) );
//                        std::cout << Form( "Event %6llu Trigger message in Overlap for gDPB %02u is %016lx ", fulNbBuiltSubEvent, uGdpb, (*itMess).getData() ) << std::endl;
                     } // for( std::vector< gdpb::FullMessage >::iterator itMess = vTrigMess.begin(); itMess != vTrigMess.end(); ++ itMess )
                  } // else of if( bTriggerInOverMs[uGdpb] )
            } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
            dMeanTriggerGdpbTsOver /= fuNrOfGdpbs;
            dMeanTriggerStarTsOver /= fuNrOfGdpbs;
            dMeanTriggTimeToStartSec = 1e-9 *(dMeanTriggerGdpbTs - fdStartTime);
            dMeanTriggTimeToStartLongSec = 1e-9 *(dMeanTriggerGdpbTs - fdStartTimeLong) / 60.0;

            if( bTriggerInOverMs[0] )
            {
               CbmTofStarTrigger2018 meanTriggerOver( static_cast< ULong64_t >( dMeanTriggerGdpbTsOver ), static_cast< ULong64_t >( dMeanTriggerStarTsOver ),
                                                      fvtTsOverLinksBuffer[0][ 0 ].GetStarToken(),
                                                      fvtTsOverLinksBuffer[0][ 0 ].GetStarDaqCmd(),
                                                      fvtTsOverLinksBuffer[0][ 0 ].GetStarTrigCmd() );

               /// Associate this trigger to its subevent
               fStarSubEvent.SetTrigger( meanTriggerOver );
            } // if( bTriggerInOverMs[0] )
               else
               {
                  CbmTofStarTrigger2018 meanTriggerOver( static_cast< ULong64_t >( dMeanTriggerGdpbTsOver ), static_cast< ULong64_t >( dMeanTriggerStarTsOver ),
                                                         fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarToken(),
                                                         fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarDaqCmd(),
                                                         fvtTsLinksBuffer[0][ vuIndexMatchingTrigger[0] ].GetStarTrigCmd() );

                  /// Associate this trigger to its subevent
                  fStarSubEvent.SetTrigger( meanTriggerOver );
               } // else of if( bTriggerInOverMs[0] )
            /// Set the source index of the subevent
            fStarSubEvent.SetSource( 16 );

            for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
               if( bTriggerInOverMs[uGdpb] )
                fhStarTrigTimeToMeanTrig_gDPB[ uGdpb ]->Fill(
                              get4v2x::kdClockCycleSizeNs * fvtTsOverLinksBuffer[uGdpb][ 0 ].GetFullGdpbTs()
                            - dMeanTriggerGdpbTs );
                  else fhStarTrigTimeToMeanTrig_gDPB[ uGdpb ]->Fill(
                              get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs()
                            - dMeanTriggerGdpbTs );

            for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
            {
               Double_t dTriggerTime   = 0.0;
               if( bTriggerInOverMs[uGdpb] )
                  dTriggerTime = get4v2x::kdClockCycleSizeNs * fvtTsOverLinksBuffer[uGdpb][ 0 ].GetFullGdpbTs();
                  else dTriggerTime = get4v2x::kdClockCycleSizeNs * fvtTsLinksBuffer[uGdpb][ vuIndexMatchingTrigger[uGdpb] ].GetFullGdpbTs();
               Double_t dTriggerWinBeg = dTriggerTime - fdStarTriggerDelay[ uGdpb ];
               Double_t dTriggerWinEnd = dTriggerTime - fdStarTriggerDelay[ uGdpb ] + fdStarTriggerWinSize[ uGdpb ];
               Double_t dClosestTriggerWinStart = dTriggerTime - fdStarTriggerDelay[ uGdpb ] + fdStarTriggerDeadtime[ uGdpb ];
               Bool_t bFirstMessClosestEventFound = kFALSE;
               Bool_t bEventWindowInOverlap = ( fdCurrentTsCoreEndTime < dTriggerWinEnd ? kTRUE : kFALSE );

               /// Check if this trigger may correspond to an event with data in next TS
               if( kTRUE == bEventWindowInOverlap )
                  fStarSubEvent.SetEndBorderEventFlag();

               /// Check if this trigger leads to an event overlapping with the previous one
               if( 0 < dPrevEventEnd[ uGdpb ] && dTriggerWinBeg < dPrevEventEnd[ uGdpb ] )
                  fStarSubEvent.SetOverlapEventFlag();

               for( std::vector< gdpb::FullMessage >::iterator itMess = itFirstMessageNextEvent[ uGdpb ];
                    itMess != fvmTsLinksBuffer[ uGdpb ].end();
                    ++ itMess )
               {
                  Double_t dMessageFullTime = (*itMess).GetFullTimeNs();
                  if( kFALSE == bFirstMessClosestEventFound &&
                      dClosestTriggerWinStart < dMessageFullTime )
                  {
                     itFirstMessageNextEvent[ uGdpb ] = itMess;
                     bFirstMessClosestEventFound = kTRUE;
                  } // If first possible hit of closest event not found and current hit fits

                  /// Plotting of time to trigger for all hits
                  Double_t dTimeToTrigg = dMessageFullTime - dTriggerTime;
                  fhStarHitToTrigAll_gDPB[ uGdpb ]->Fill( dTimeToTrigg );
                  fhStarHitToTrigAllTime_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartSec, dTimeToTrigg );
                  fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartLongSec, dTimeToTrigg );

                  if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
                  {
                     /// Message belongs to this event
                     fStarSubEvent.AddMsg( (*itMess) );

                     /// Plotting of time to trigger for hits within the event window
                     fhStarHitToTrigWin_gDPB[ uGdpb ]->Fill( dTimeToTrigg );
                     fhStarHitToTrigWinTime_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartSec, dTimeToTrigg );
                     fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartLongSec, dTimeToTrigg );
                  } // if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
                     else if( dTriggerWinEnd < dMessageFullTime )
                        /// First Message out of the window for this event => Stop there and go to the next
                        break;
               } // Loop on message from first allowed by last event + deadtime to end

               /// If event window is going in overlap MS, check also these messages
               if( kTRUE == bEventWindowInOverlap )
               {
                  for( std::vector< gdpb::FullMessage >::iterator itMess = fvmTsOverLinksBuffer[ uGdpb ].begin();
                       itMess != fvmTsOverLinksBuffer[ uGdpb ].end();
                       ++ itMess )
                  {
                     Double_t dMessageFullTime = (*itMess).GetFullTimeNs();
                     if( kFALSE == bFirstMessClosestEventFound &&
                         dClosestTriggerWinStart < dMessageFullTime )
                     {
                        itFirstMessageNextEvent[ uGdpb ] = itMess;
                        bFirstMessClosestEventFound = kTRUE;
                     } // If first possible hit of closest event not found and current hit fits

                     /// Plotting of time to trigger for all hits
                     Double_t dTimeToTrigg = dMessageFullTime - dTriggerTime;
                     fhStarHitToTrigAll_gDPB[ uGdpb ]->Fill( dTimeToTrigg );
                     fhStarHitToTrigAllTime_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartSec, dTimeToTrigg );
                     fhStarHitToTrigAllTimeLong_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartLongSec, dTimeToTrigg );

                     if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
                     {
                        /// Message belongs to this event
                        fStarSubEvent.AddMsg( (*itMess) );

                        /// Plotting of time to trigger for hits within the event window
                        fhStarHitToTrigWin_gDPB[ uGdpb ]->Fill( dTimeToTrigg );
                        fhStarHitToTrigWinTime_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartSec, dTimeToTrigg );
                        fhStarHitToTrigWinTimeLong_gDPB[ uGdpb ]->Fill( dMeanTriggTimeToStartLongSec, dTimeToTrigg );
                     } // if( dTriggerWinBeg   <= dMessageFullTime && dMessageFullTime <= dTriggerWinEnd )
                        else if( dTriggerWinEnd < dMessageFullTime )
                           /// First Message out of the window for this event => Stop there and go to the next
                           break;
                  } // Loop on message from beginning to end
               } // if( kTRUE == bEventWindowInOverlap )
            } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)

            /// Time sort the sub-event if required by the user
            if( kTRUE == fbTimeSortOutput )
               fStarSubEvent.SortMessages(  );

            /// Send the sub-event to the STAR systems
            iBuffSzByte = 0;
            pDataBuff = fStarSubEvent.BuildOutput( iBuffSzByte );
            if( NULL != pDataBuff )
            {
               /// Valid output, do stuff with it!
               fulNbBuiltSubEvent++;

#ifdef STAR_SUBEVT_BUILDER
               /*
                ** Function to send sub-event block to the STAR DAQ system
                *       trg_word received is packed as:
                *
                *       trg_cmd|daq_cmd|tkn_hi|tkn_mid|tkn_lo
                */
               star_rhicf_write( fStarSubEvent.GetTrigger().GetStarTrigerWord(),
                                 pDataBuff, iBuffSzByte );
               fulNbStarSubEvent++;
/*
               LOG(info) << "Sent STAR event with size " << iBuffSzByte << " Bytes"
                         << " and token " << fStarSubEvent.GetTrigger().GetStarToken();
*/
#endif // STAR_SUBEVT_BUILDER

               if( kTRUE == fbEventDumpEna )
               {
                  fpBinDumpFile->write( reinterpret_cast< const char * >( &kuBinDumpBegWord ), sizeof( UInt_t ) );
                  fpBinDumpFile->write( reinterpret_cast< const char * >( &iBuffSzByte ), sizeof( Int_t ) );
                  fpBinDumpFile->write( reinterpret_cast< const char * >( pDataBuff ), iBuffSzByte );
                  fpBinDumpFile->write( reinterpret_cast< const char * >( &kuBinDumpEndWord ), sizeof( UInt_t ) );
               } // if( kTRUE == fbEventDumpEna )
            } // if( NULL != pDataBuff )
               else LOG(error) << "Invalid STAR SubEvent Output, can only happen if trigger "
                               << " object was not set => Do Nothing more with it!!! ";
/*
            if( 1000 <  iBuffSzByte )
            {
               std::cout << "Big subevent in overlap" << std::endl;
               fStarSubEvent.PrintSubEvent();
            } // if( 100 <  iBuffSzByte )
*/
            /// Fill plot of event size
            fhStarEventSize->Fill( iBuffSzByte );
            /// Fill plot of event size as function of trigger time
            if( 0 < fdStartTime )
               fhStarEventSizeTime->Fill( 1e-9 *(dMeanTriggerGdpbTsOver - fdStartTime), iBuffSzByte );
            if( 0 < fdStartTimeLong )
               fhStarEventSizeTimeLong->Fill( 1e-9 *(dMeanTriggerGdpbTsOver - fdStartTimeLong) / 60.0, iBuffSzByte );

            /// Now clear the sub-event
            fStarSubEvent.ClearSubEvent();
         } // if( bFirstTriggerMatch )
      } // if( kFALSE == bMatchingTriggerFound )
   } // while( kTRUE == bMatchingTriggerFound )

   /// Clear buffers, eventual border crossing should be taken care thanks to the overlap MS
   for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
   {
      fvtTsLinksBuffer[uGdpb].clear();
      fvmTsLinksBuffer[uGdpb].clear();
      fvmTsOverLinksBuffer[uGdpb].clear();
      fvtTsOverLinksBuffer[uGdpb].clear();
   } // for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
}

ClassImp(CbmTofStarEventBuilder2018)
