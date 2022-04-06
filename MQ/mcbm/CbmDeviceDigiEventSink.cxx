/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceDigiEventSink.cxx
 *
 * @since 2020-05-24
 * @author P.-A. Loizeau
 */

#include "CbmDeviceDigiEventSink.h"


/// CBM headers
#include "CbmEvent.h"
#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"

#include "TimesliceMetaData.h"

/// FAIRROOT headers
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "BoostSerializer.h"

#include "RootSerializer.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

/// C/C++ headers
#include <thread>  // this_thread::sleep_for

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

//Bool_t bMcbm2018MonitorTaskT0ResetHistos = kFALSE;

CbmDeviceDigiEventSink::CbmDeviceDigiEventSink() {}

void CbmDeviceDigiEventSink::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceDigiEventSink.";

  fbStoreFullTs    = fConfig->GetValue<bool>("StoreFullTs");
  fsOutputFileName = fConfig->GetValue<std::string>("OutFileName");

  fsChannelNameDataInput = fConfig->GetValue<std::string>("EvtNameIn");
  fsAllowedChannels[0]   = fsChannelNameDataInput;

  fbBypassConsecutiveTs = fConfig->GetValue<bool>("BypassConsecutiveTs");

  fbFillHistos             = fConfig->GetValue<bool>("FillHistos");
  fuPublishFreqTs          = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime         = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime         = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");

  /// Associate the MissedTs Channel to the corresponding handler
  OnData(fsChannelNameMissedTs, &CbmDeviceDigiEventSink::HandleMissTsData);

  /// Associate the command Channel to the corresponding handler
  OnData(fsChannelNameCommands, &CbmDeviceDigiEventSink::HandleCommand);

  /// Associate the Event + Unp data Channel to the corresponding handler
  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  //logger::SetLogLevel("INFO");
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined channels: " << noChannel;
  for (auto const& entry : fChannels) {
    LOG(info) << "Channel name: " << entry.first;
    if (std::string::npos != entry.first.find(fsChannelNameDataInput)) {
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
      OnData(entry.first, &CbmDeviceDigiEventSink::HandleData);
    }  // if( entry.first.find( "ts" )
  }    // for( auto const &entry : fChannels )

  //   InitContainers();

  /// Prepare storage TClonesArrays
  /// TS MetaData storage
  fTimeSliceMetaDataArray = new TClonesArray("TimesliceMetaData", 1);
  if (NULL == fTimeSliceMetaDataArray) {
    throw InitTaskError("Failed creating the TS meta data TClonesarray ");
  }  // if( NULL == fTimeSliceMetaDataArray )
     /// Events storage
  /// TODO: remove TObject from CbmEvent and switch to vectors!
  fEventsSel = new std::vector<CbmDigiEvent>();

  /// Prepare root output
  if ("" != fsOutputFileName) {
    fpRun         = new FairRunOnline();
    fpFairRootMgr = FairRootManager::Instance();
    fpFairRootMgr->SetSink(new FairRootFileSink(fsOutputFileName));
    if (nullptr == fpFairRootMgr->GetOutFile()) {
      throw InitTaskError("Could not open root file");
    }  // if( nullptr == fpFairRootMgr->GetOutFile() )
  }    // if( "" != fsOutputFileName )
  else {
    throw InitTaskError("Empty output filename!");
  }  // else of if( "" != fsOutputFileName )

  LOG(info) << "Init Root Output to " << fsOutputFileName;

  fpFairRootMgr->InitSink();
  fEvtHeader = new CbmTsEventHeader();
  fpFairRootMgr->Register("EventHeader.", "Event", fEvtHeader, kTRUE);

  /// Register all input data members with the FairRoot manager
  /// TS MetaData
  fpFairRootMgr->Register("TimesliceMetaData", "TS Meta Data", fTimeSliceMetaDataArray, kTRUE);
  /// CbmEvent
  fpFairRootMgr->RegisterAny("DigiEvent", fEventsSel, kTRUE);

  /// Full TS Digis storage (optional usage, controlled by fbStoreFullTs!)
  if (fbStoreFullTs) {
    fvDigiT0   = new std::vector<CbmTofDigi>();
    fvDigiSts  = new std::vector<CbmStsDigi>();
    fvDigiMuch = new std::vector<CbmMuchDigi>();
    fvDigiTrd  = new std::vector<CbmTrdDigi>();
    fvDigiTof  = new std::vector<CbmTofDigi>();
    fvDigiRich = new std::vector<CbmRichDigi>();
    fvDigiPsd  = new std::vector<CbmPsdDigi>();

    fpFairRootMgr->RegisterAny("T0Digi", fvDigiT0, kTRUE);
    fpFairRootMgr->RegisterAny("StsDigi", fvDigiSts, kTRUE);
    fpFairRootMgr->RegisterAny("MuchDigi", fvDigiMuch, kTRUE);
    fpFairRootMgr->RegisterAny("TrdDigi", fvDigiTrd, kTRUE);
    fpFairRootMgr->RegisterAny("TofDigi", fvDigiTof, kTRUE);
    fpFairRootMgr->RegisterAny("RichDigi", fvDigiRich, kTRUE);
    fpFairRootMgr->RegisterAny("PsdDigi", fvDigiPsd, kTRUE);
  }

  fpFairRootMgr->WriteFolder();

  LOG(info) << "Initialized outTree with rootMgr at " << fpFairRootMgr;

  /// Histograms management
  if (kTRUE == fbFillHistos) {
    /// Comment to prevent clang format single lining
    if (kFALSE == InitHistograms()) { throw InitTaskError("Failed to initialize the histograms."); }
  }  // if( kTRUE == fbFillHistos )
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceDigiEventSink::IsChannelNameAllowed(std::string channelName)
{
  for (auto const& entry : fsAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fsAllowedChannels.begin(), fsAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos - fsAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx;
      return true;
    }  // if (pos1!=std::string::npos)
  }    // for(auto const &entry : fsAllowedChannels)
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

bool CbmDeviceDigiEventSink::InitHistograms()
{
  /// Histos creation and obtain pointer on them
  /// Trigger histo creation, filling vHistos and vCanvases
  // bool initOK =CreateHistograms();
  bool initOK = true;

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  // ALGO: std::vector<std::pair<TNamed*, std::string>> vHistos = fMonitorAlgo->GetHistoVector();
  std::vector<std::pair<TNamed*, std::string>> vHistos = {};
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  // ALGO: std::vector<std::pair<TCanvas*, std::string>> vCanvases = fMonitorAlgo->GetCanvasVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = {};

  /// Add pointers to each histo in the histo array
  /// Create histo config vector
  /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Histo name, Folder >
  ///      and send it through a separate channel using the BoostSerializer
  for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
    //         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
    //                   << " in " << vHistos[ uHisto ].second.data()
    //                   ;
    fArrayHisto.Add(vHistos[uHisto].first);
    std::pair<std::string, std::string> psHistoConfig(vHistos[uHisto].first->GetName(), vHistos[uHisto].second);
    fvpsHistosFolder.push_back(psHistoConfig);

    LOG(info) << "Config of hist  " << psHistoConfig.first.data() << " in folder " << psHistoConfig.second.data();
  }  // for( UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto )

  /// Create canvas config vector
  /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Canvas name, config >
  ///      and send it through a separate channel using the BoostSerializer
  for (UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv) {
    //         LOG(info) << "Registering  " << vCanvases[ uCanv ].first->GetName()
    //                   << " in " << vCanvases[ uCanv ].second.data();
    std::string sCanvName = (vCanvases[uCanv].first)->GetName();
    std::string sCanvConf = GenerateCanvasConfigString(vCanvases[uCanv].first);

    std::pair<std::string, std::string> psCanvConfig(sCanvName, sCanvConf);

    fvpsCanvasConfig.push_back(psCanvConfig);

    LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data() << " is " << psCanvConfig.second.data();
  }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

  return initOK;
}

//--------------------------------------------------------------------//
// handler is called whenever a message arrives on fsChannelNameMissedTs, with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceDigiEventSink::HandleMissTsData(FairMQMessagePtr& msg, int /*index*/)
{
  std::vector<uint64_t> vIndices;
  std::string msgStrMissTs(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream issMissTs(msgStrMissTs);
  boost::archive::binary_iarchive inputArchiveMissTs(issMissTs);
  inputArchiveMissTs >> vIndices;

  fvulMissedTsIndices.insert(fvulMissedTsIndices.end(), vIndices.begin(), vIndices.end());

  /// Check TS queue and process it if needed (in case it filled a hole!)
  CheckTsQueues();

  return true;
}
//--------------------------------------------------------------------//
// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceDigiEventSink::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  /// Unpack the message
  CbmEventTimeslice unpTs(parts);

  /// FIXME: Need to check if TS arrived in order (probably not!!!) + buffer!!!
  LOG(debug) << "Next TS check " << fuPrevTsIndex << " " << fulTsCounter << " " << unpTs.fTsMetaData.GetIndex()
             << " Storage size: " << fmFullTsStorage.size();
  if (fbBypassConsecutiveTs || (fuPrevTsIndex + 1 == unpTs.fTsMetaData.GetIndex())
      || (0 == fuPrevTsIndex && 0 == fulTsCounter && 0 == unpTs.fTsMetaData.GetIndex())) {
    LOG(debug) << "TS direct to dump";
    /// Fill all storage variables registers for data output
    PrepareTreeEntry(unpTs);
    /// Trigger FairRoot manager to dump Tree entry
    DumpTreeEntry();
    /// Update counters
    fuPrevTsIndex = unpTs.fTsMetaData.GetIndex();
    fulTsCounter++;
  }
  else {
    LOG(debug) << "TS direct to storage";
    /// If not consecutive to last TS sent,
    fmFullTsStorage.emplace_hint(fmFullTsStorage.end(),
                                 std::pair<uint64_t, CbmEventTimeslice>(unpTs.fTsMetaData.GetIndex(), unpTs));
  }
  LOG(debug) << "TS metadata checked";

  /// Clear metadata => crashes, maybe not needed as due to move the pointer is invalidated?
  //   delete fTsMetaData;

  if (fbBypassConsecutiveTs) {
    /// Skip checking the TS buffer as writing straight to file
    /// => Just check if we are done and can close the file or not
    if (fbReceivedEof) {
      /// In this case we cannot check if the last TS received/processed is the final one due to lack of order
      /// => use instead the fact that we received all expected TS
      if ((fulTsCounter + fvulMissedTsIndices.size()) == fuTotalTsCount) {
        LOG(info) << "CbmDeviceDigiEventSink::HandleData => "
                  << "Found all expected TS (" << fulTsCounter << ") and total nb of TS " << fuTotalTsCount
                  << " after accounting for the ones reported as missing by the source (" << fvulMissedTsIndices.size()
                  << ")";
        Finish();
      }  // if ((fulTsCounter + fvulMissedTsIndices.size()) == fuTotalTsCount)
    }
  }
  else {
    /// Check TS queue and process it if needed (in case it filled a hole!)
    CheckTsQueues();
    LOG(debug) << "TS queues checked";
  }

  /// Histograms management
  if (kTRUE == fbFillHistos) {
    /// Send histograms each 100 time slices. Should be each ~1s
    /// Use also runtime checker to trigger sending after M s if
    /// processing too slow or delay sending if processing too fast
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double_t> elapsedSeconds    = currentTime - fLastPublishTime;
    if ((fdMaxPublishTime < elapsedSeconds.count())
        || (0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count())) {
      if (!fbConfigSent) {
        // Send the configuration only once per run!
        fbConfigSent = SendHistoConfAndData();
      }  // if( !fbConfigSent )
      else
        SendHistograms();

      fLastPublishTime = std::chrono::system_clock::now();
    }  // if( ( fdMaxPublishTime < elapsedSeconds.count() ) || ( 0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )
  }    // if( kTRUE == fbFillHistos )

  LOG(debug) << "Processed TS with saving " << (fulTsCounter + fulMissedTsCounter) << " TS (" << fulTsCounter
             << " full ones and " << fulMissedTsCounter << " missed/empty ones)";
  LOG(debug) << "Buffers are " << fmFullTsStorage.size() << " full TS and " << fvulMissedTsIndices.size()
             << " missed/empty ones)";
  LOG(debug) << "Buffers are " << fmFullTsStorage.size() << " full TS and " << fvulMissedTsIndices.size()
             << " missed/empty ones)";

  return true;
}
//--------------------------------------------------------------------//
bool CbmDeviceDigiEventSink::HandleCommand(FairMQMessagePtr& msg, int /*index*/)
{
  /*
   std::string sCommand( static_cast< char * >( msg->GetData() ),
                          msg->GetSize() );
*/
  std::string sCommand;
  std::string msgStrCmd(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream issCmd(msgStrCmd);
  boost::archive::binary_iarchive inputArchiveCmd(issCmd);
  inputArchiveCmd >> sCommand;

  std::string sCmdTag = sCommand;
  size_t charPosDel   = sCommand.find(' ');
  if (std::string::npos != charPosDel) {
    sCmdTag = sCommand.substr(0, charPosDel);
  }  // if( std::string::npos != charPosDel )

  if ("EOF" == sCmdTag) {
    fbReceivedEof = true;

    /// Extract the last TS index and global full TS count
    if (std::string::npos == charPosDel) {
      LOG(fatal) << "CbmDeviceDigiEventSink::HandleCommand => "
                 << "Incomplete EOF command received: " << sCommand;
      return false;
    }  // if( std::string::npos == charPosDel )
       /// Last TS index
    charPosDel++;
    std::string sNext = sCommand.substr(charPosDel);
    charPosDel        = sNext.find(' ');

    if (std::string::npos == charPosDel) {
      LOG(fatal) << "CbmDeviceDigiEventSink::HandleCommand => "
                 << "Incomplete EOF command received: " << sCommand;
      return false;
    }  // if( std::string::npos == charPosDel )
    fuLastTsIndex = std::stoul(sNext.substr(0, charPosDel));
    /// Total TS count
    charPosDel++;
    fuTotalTsCount = std::stoul(sNext.substr(charPosDel));

    LOG(info) << "CbmDeviceDigiEventSink::HandleCommand => "
              << "Received EOF command with final TS index " << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
    /// End of data: clean save of data + close file + send last state of histos if enabled
    if (fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount) {
      LOG(info) << "CbmDeviceDigiEventSink::HandleCommand => "
                << "Found final TS index " << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
      Finish();
    }  // if( fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
  }    // if( "EOF" == sCmdTag )
  else if ("STOP" == sCmdTag) {
    /// TODO: different treatment in case of "BAD" ending compared to EOF?
    /// Source failure: clean save of received data + close file + send last state of histos if enabled
    Finish();
  }  // else if( "STOP" == sCmdTag )
  else {
    LOG(warning) << "Unknown command received: " << sCmdTag << " => will be ignored!";
  }  // else if command not recognized

  return true;
}
//--------------------------------------------------------------------//
void CbmDeviceDigiEventSink::CheckTsQueues()
{
  bool bHoleFoundInBothQueues = false;

  std::map<uint64_t, CbmEventTimeslice>::iterator itFullTs = fmFullTsStorage.begin();
  std::vector<uint64_t>::iterator itMissTs                 = fvulMissedTsIndices.begin();

  while (!bHoleFoundInBothQueues) {
    /// Check if the first TS in the full TS queue is the next one
    if (fmFullTsStorage.end() != itFullTs && fuPrevTsIndex + 1 == (*itFullTs).first) {
      /// Fill all storage variables registers for data output
      PrepareTreeEntry((*itFullTs).second);
      /// Trigger FairRoot manager to dump Tree entry
      DumpTreeEntry();

      /// Update counters
      fuPrevTsIndex = (*itFullTs).first;
      fulTsCounter++;

      /// Increment iterator
      ++itFullTs;
      continue;
    }  // if( fmFullTsStorage.end() != itFullTs && fuPrevTsIndex + 1 == (*itFullTs).first() )
    if (fmFullTsStorage.end() != itFullTs)
      LOG(debug) << "CbmDeviceDigiEventSink::CheckTsQueues => Full TS " << (*itFullTs).first << " VS "
                 << (fuPrevTsIndex + 1);
    /// Check if the first TS in the missed TS queue is the next one
    if (fvulMissedTsIndices.end() != itMissTs
        && ((0 == fuPrevTsIndex && fuPrevTsIndex == (*itMissTs))
            || ((0 < fulTsCounter || 0 < fulMissedTsCounter) && fuPrevTsIndex + 1 == (*itMissTs)))) {

      /// Prepare entry with only dummy TS metadata and empty storage variables
      new ((*fTimeSliceMetaDataArray)[fTimeSliceMetaDataArray->GetEntriesFast()])
        TimesliceMetaData(0, 0, 0, (*itMissTs));

      /// Trigger FairRoot manager to dump Tree entry
      DumpTreeEntry();

      /// Update counters
      fuPrevTsIndex = (*itMissTs);
      fulMissedTsCounter++;

      /// Increment iterator
      ++itMissTs;
      continue;
    }  // if( fvulMissedTsIndices.end() != itMissTs && fuPrevTsIndex + 1 == (*itMissTs ) )
    if (fvulMissedTsIndices.end() != itMissTs)
      LOG(debug) << "CbmDeviceDigiEventSink::CheckTsQueues => Empty TS " << (*itMissTs) << " VS "
                 << (fuPrevTsIndex + 1);

    /// Should be reached only if both queues at the end or hole found in both
    bHoleFoundInBothQueues = true;
  }  // while( !bHoleFoundInBothQueues )

  LOG(debug) << "CbmDeviceDigiEventSink::CheckTsQueues => buffered TS " << fmFullTsStorage.size()
             << " buffered empties " << fvulMissedTsIndices.size();
  for (auto it = fmFullTsStorage.begin(); it != fmFullTsStorage.end(); ++it) {
    LOG(debug) << "CbmDeviceDigiEventSink::CheckTsQueues => buffered TS index " << (*it).first;
  }

  /// Delete the processed entries
  fmFullTsStorage.erase(fmFullTsStorage.begin(), itFullTs);
  fvulMissedTsIndices.erase(fvulMissedTsIndices.begin(), itMissTs);

  /// End of data: clean save of data + close file + send last state of histos if enabled
  if (fbReceivedEof && fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount) {
    LOG(info) << "CbmDeviceDigiEventSink::CheckTsQueues => "
              << "Found final TS index " << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
    Finish();
  }  // if( fbReceivedEof && fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
}
//--------------------------------------------------------------------//
void CbmDeviceDigiEventSink::PrepareTreeEntry(CbmEventTimeslice unpTs)
{
  /// FIXME: poor man solution with lots of data copy until we undertsnad how to properly deal
  /// with FairMq messages ownership and memory managment

  (*fEvtHeader) = std::move(unpTs.fCbmTsEventHeader);

  /// FIXME: Not sure if this is the proper way to insert the data
  new ((*fTimeSliceMetaDataArray)[fTimeSliceMetaDataArray->GetEntriesFast()])
    TimesliceMetaData(std::move(unpTs.fTsMetaData));

  /// Extract CbmEvent TClonesArray from input message
  (*fEventsSel) = std::move(unpTs.GetSelectedData());

  /// Full TS Digis storage (optional usage, controlled by fbStoreFullTs!)
  if (fbStoreFullTs) {
    if (0 < unpTs.fvDigiT0.size()) fvDigiT0->assign(unpTs.fvDigiT0.begin(), unpTs.fvDigiT0.end());
    if (0 < unpTs.fvDigiSts.size()) fvDigiSts->assign(unpTs.fvDigiSts.begin(), unpTs.fvDigiSts.end());
    if (0 < unpTs.fvDigiMuch.size()) fvDigiMuch->assign(unpTs.fvDigiMuch.begin(), unpTs.fvDigiMuch.end());
    if (0 < unpTs.fvDigiTrd.size()) fvDigiTrd->assign(unpTs.fvDigiTrd.begin(), unpTs.fvDigiTrd.end());
    if (0 < unpTs.fvDigiTof.size()) fvDigiTof->assign(unpTs.fvDigiTof.begin(), unpTs.fvDigiTof.end());
    if (0 < unpTs.fvDigiRich.size()) fvDigiRich->assign(unpTs.fvDigiRich.begin(), unpTs.fvDigiRich.end());
    if (0 < unpTs.fvDigiPsd.size()) fvDigiPsd->assign(unpTs.fvDigiPsd.begin(), unpTs.fvDigiPsd.end());
  }
}
void CbmDeviceDigiEventSink::DumpTreeEntry()
{
  // Unpacked digis + CbmEvent output to root file
  /*
 * NH style
//      fpFairRootMgr->FillEventHeader(fEvtHeader);
//      LOG(info) << "Fill WriteOutBuffer with FairRootManager at " << fpFairRootMgr;
//      fpOutRootFile->cd();
      fpFairRootMgr->Fill();
      fpFairRootMgr->StoreWriteoutBufferData( fpFairRootMgr->GetEventTime() );
      //fpFairRootMgr->StoreAllWriteoutBufferData();
      fpFairRootMgr->DeleteOldWriteoutBufferData();
*/
  /// FairRunOnline style
  fpFairRootMgr->StoreWriteoutBufferData(fpFairRootMgr->GetEventTime());
  fpFairRootMgr->FillEventHeader(fEvtHeader);
  fpFairRootMgr->Fill();
  fpFairRootMgr->DeleteOldWriteoutBufferData();
  //  fpFairRootMgr->Write();

  /// Clear metadata array
  fTimeSliceMetaDataArray->Clear();

  /// Clear event vector
  fEventsSel->clear();
  /// Full TS Digis storage (optional usage, controlled by fbStoreFullTs!)
  if (fbStoreFullTs) {
    fvDigiT0->clear();
    fvDigiSts->clear();
    fvDigiMuch->clear();
    fvDigiTrd->clear();
    fvDigiTof->clear();
    fvDigiRich->clear();
    fvDigiPsd->clear();
  }
}

//--------------------------------------------------------------------//

bool CbmDeviceDigiEventSink::SendHistoConfAndData()
{
  /// Prepare multiparts message and header
  std::pair<uint32_t, uint32_t> pairHeader(fvpsHistosFolder.size(), fvpsCanvasConfig.size());
  FairMQMessagePtr messageHeader(NewMessage());
  //  Serialize<BoostSerializer<std::pair<uint32_t, uint32_t>>>(*messageHeader, pairHeader);
  BoostSerializer<std::pair<uint32_t, uint32_t>>().Serialize(*messageHeader, pairHeader);

  FairMQParts partsOut;
  partsOut.AddPart(std::move(messageHeader));

  for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto) {
    /// Serialize the vector of histo config into a single MQ message
    FairMQMessagePtr messageHist(NewMessage());
    //    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageHist, fvpsHistosFolder[uHisto]);
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageHist, fvpsHistosFolder[uHisto]);

    partsOut.AddPart(std::move(messageHist));
  }  // for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto)

  /// Catch case where no histos are registered!
  /// => Add empty message
  if (0 == fvpsHistosFolder.size()) {
    FairMQMessagePtr messageHist(NewMessage());
    partsOut.AddPart(std::move(messageHist));
  }

  for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv) {
    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    //    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageCan, fvpsCanvasConfig[uCanv]);
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageCan, fvpsCanvasConfig[uCanv]);

    partsOut.AddPart(std::move(messageCan));
  }  // for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv)

  /// Catch case where no Canvases are registered!
  /// => Add empty message
  if (0 == fvpsCanvasConfig.size()) {
    FairMQMessagePtr messageHist(NewMessage());
    partsOut.AddPart(std::move(messageHist));
  }

  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr msgHistos(NewMessage());
  //  Serialize<RootSerializer>(*msgHistos, &fArrayHisto);
  RootSerializer().Serialize(*msgHistos, &fArrayHisto);

  partsOut.AddPart(std::move(msgHistos));

  /// Send the multi-parts message to the common histogram messages queue
  if (Send(partsOut, fsChannelNameHistosInput) < 0) {
    LOG(error) << "CbmTsConsumerReqDevExample::SendHistoConfAndData => Problem sending data";
    return false;
  }  // if( Send( partsOut, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  // ResetHistograms(kFALSE);

  return true;
}

bool CbmDeviceDigiEventSink::SendHistograms()
{
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
  //  Serialize<RootSerializer>(*message, &fArrayHisto);
  RootSerializer().Serialize(*message, &fArrayHisto);

  /// Send message to the common histogram messages queue
  if (Send(message, fsChannelNameHistosInput) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }  // if( Send( message, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  // ResetHistograms(kFALSE);

  return true;
}

//--------------------------------------------------------------------//
CbmDeviceDigiEventSink::~CbmDeviceDigiEventSink()
{
  /// FIXME: Add pointers check before delete

  /// Close things properly if not already done
  if (!fbFinishDone) Finish();

  /// Clear events vector
  fEventsSel->clear();
  delete fEventsSel;

  delete fpRun;
}

void CbmDeviceDigiEventSink::Finish()
{
  // Clean closure of output to root file
  fpFairRootMgr->Write();
  //   fpFairRootMgr->GetSource()->Close();
  fpFairRootMgr->CloseSink();
  LOG(info) << "File closed after saving " << (fulTsCounter + fulMissedTsCounter) << " TS (" << fulTsCounter
            << " full ones and " << fulMissedTsCounter << " missed/empty ones)";
  LOG(info) << "Still buffered TS " << fmFullTsStorage.size() << " and still buffered empties "
            << fvulMissedTsIndices.size();

  if (kTRUE == fbFillHistos) {
    SendHistograms();
    fLastPublishTime = std::chrono::system_clock::now();
  }  // if( kTRUE == fbFillHistos )

  ChangeState(fair::mq::Transition::Stop);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  ChangeState(fair::mq::Transition::End);

  fbFinishDone = kTRUE;
}

CbmEventTimeslice::CbmEventTimeslice(FairMQParts& parts)
{
  /// Extract unpacked data from input message
  uint32_t uPartIdx = 0;

  /// TODO: code order of vectors in the TS header!!

  /// TS header
  TObject* tempObjectPointer = nullptr;
  RootSerializer().Deserialize(*parts.At(uPartIdx), tempObjectPointer);
  if (tempObjectPointer && TString(tempObjectPointer->ClassName()).EqualTo("CbmTsEventHeader")) {
    fCbmTsEventHeader = *(static_cast<CbmTsEventHeader*>(tempObjectPointer));
  }
  else {
    LOG(fatal) << "Failed to deserialize the TS header";
  }
  ++uPartIdx;

  /// T0
  std::string msgStrT0(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issT0(msgStrT0);
  boost::archive::binary_iarchive inputArchiveT0(issT0);
  inputArchiveT0 >> fvDigiT0;
  ++uPartIdx;

  /// STS
  std::string msgStrSts(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issSts(msgStrSts);
  boost::archive::binary_iarchive inputArchiveSts(issSts);
  inputArchiveSts >> fvDigiSts;
  ++uPartIdx;

  /// MUCH
  std::string msgStrMuch(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issMuch(msgStrMuch);
  boost::archive::binary_iarchive inputArchiveMuch(issMuch);
  inputArchiveMuch >> fvDigiMuch;
  ++uPartIdx;

  /// TRD
  std::string msgStrTrd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTrd(msgStrTrd);
  boost::archive::binary_iarchive inputArchiveTrd(issTrd);
  inputArchiveTrd >> fvDigiTrd;
  ++uPartIdx;

  /// T0F
  std::string msgStrTof(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTof(msgStrTof);
  boost::archive::binary_iarchive inputArchiveTof(issTof);
  inputArchiveTof >> fvDigiTof;
  ++uPartIdx;

  /// RICH
  std::string msgStrRich(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issRich(msgStrRich);
  boost::archive::binary_iarchive inputArchiveRich(issRich);
  inputArchiveRich >> fvDigiRich;
  ++uPartIdx;

  /// PSD
  std::string msgStrPsd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issPsd(msgStrPsd);
  boost::archive::binary_iarchive inputArchivePsd(issPsd);
  inputArchivePsd >> fvDigiPsd;
  ++uPartIdx;

  /// TS metadata
  tempObjectPointer = nullptr;
  RootSerializer().Deserialize(*parts.At(uPartIdx), tempObjectPointer);

  if (tempObjectPointer && TString(tempObjectPointer->ClassName()).EqualTo("TimesliceMetaData")) {
    fTsMetaData = *(static_cast<TimesliceMetaData*>(tempObjectPointer));
  }
  else {
    LOG(fatal) << "Failed to deserialize the TS metadata";
  }
  ++uPartIdx;

  /// Events
  /// FIXME: Find out if possible to use only the boost serializer/deserializer
  /*
  std::string msgStrEvt(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issEvt(msgStrEvt);
  boost::archive::binary_iarchive inputArchiveEvt(issEvt);
  inputArchiveEvt >> fvEvents;
  ++uPartIdx;
  LOG(info) << "Input event array " << fvEvents.size();
*/
  std::vector<CbmEvent>* pvOutEvents = nullptr;
  RootSerializer().Deserialize(*parts.At(uPartIdx), pvOutEvents);
  fvEvents = std::move(*pvOutEvents);
  LOG(debug) << "Input event array " << fvEvents.size();
}

CbmEventTimeslice::~CbmEventTimeslice()
{
  fvDigiT0.clear();
  fvDigiSts.clear();
  fvDigiMuch.clear();
  fvDigiTrd.clear();
  fvDigiTof.clear();
  fvDigiRich.clear();
  fvDigiPsd.clear();
  fvEvents.clear();
}


std::vector<CbmDigiEvent> CbmEventTimeslice::GetSelectedData()
{
  std::vector<CbmDigiEvent> vEventsSel;
  vEventsSel.reserve(fvEvents.size());

  /// Loop on events in input vector
  for (CbmEvent event : fvEvents) {
    CbmDigiEvent selEvent;
    selEvent.fTime   = event.GetStartTime();
    selEvent.fNumber = event.GetNumber();

    /// FIXME: for pure digi based event, we select "continuous slices of digis"
    ///        => Copy block of [First Digi index, last digi index] with assign(it_start, it_stop)
    /// FIXME: Keep TRD1D + TRD2D support, may lead to holes in the digi sequence!
    ///        => Would need to keep the loop

    /// Get the proper order for block selection as TRD1D and TRD2D may insert indices in separate loops
    /// => Needed to ensure that the start and stop of the block copy do not trigger a vector size exception
    event.SortIndices();

    /// for each detector, find the data in the Digi vectors and copy them
    /// TODO: Template + loop on list of data types?
    /// ==> T0
    uint32_t uNbDigis = (0 < event.GetNofData(ECbmDataType::kT0Digi) ? event.GetNofData(ECbmDataType::kT0Digi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiT0.begin() + event.GetIndex(ECbmDataType::kT0Digi, 0);
      auto stopIt  = fvDigiT0.begin() + event.GetIndex(ECbmDataType::kT0Digi, uNbDigis - 1);
      selEvent.fData.fT0.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kT0Digi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fT0.fDigis.push_back(fvDigiT0[event.GetIndex(ECbmDataType::kT0Digi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kT0Digi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kT0Digi, uDigiIdx) - (uPrevIdx + 1);
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kT0Digi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the T0 block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    /// ==> STS
    uNbDigis = (0 < event.GetNofData(ECbmDataType::kStsDigi) ? event.GetNofData(ECbmDataType::kStsDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiSts.begin() + event.GetIndex(ECbmDataType::kStsDigi, 0);
      auto stopIt  = fvDigiSts.begin() + event.GetIndex(ECbmDataType::kStsDigi, uNbDigis - 1);
      selEvent.fData.fSts.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kStsDigi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fSts.fDigis.push_back(fvDigiSts[event.GetIndex(ECbmDataType::kStsDigi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kStsDigi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kStsDigi, uDigiIdx) - (uPrevIdx + 1);
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kStsDigi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the STS block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    /// ==> MUCH
    uNbDigis = (0 < event.GetNofData(ECbmDataType::kMuchDigi) ? event.GetNofData(ECbmDataType::kMuchDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiMuch.begin() + event.GetIndex(ECbmDataType::kMuchDigi, 0);
      auto stopIt  = fvDigiMuch.begin() + event.GetIndex(ECbmDataType::kMuchDigi, uNbDigis - 1);
      selEvent.fData.fMuch.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kMuchDigi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fMuch.fDigis.push_back(fvDigiMuch[event.GetIndex(ECbmDataType::kMuchDigi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kMuchDigi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kMuchDigi, uDigiIdx) - (uPrevIdx + 1);
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kMuchDigi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the MUCH block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    /// ==> TRD + TRD2D
    uNbDigis = (0 < event.GetNofData(ECbmDataType::kTrdDigi) ? event.GetNofData(ECbmDataType::kTrdDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiTrd.begin() + event.GetIndex(ECbmDataType::kTrdDigi, 0);
      auto stopIt  = fvDigiTrd.begin() + event.GetIndex(ECbmDataType::kTrdDigi, uNbDigis - 1);
      selEvent.fData.fTrd.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kTrdDigi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fTrd.fDigis.push_back(fvDigiTrd[event.GetIndex(ECbmDataType::kTrdDigi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kTrdDigi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kTrdDigi, uDigiIdx) - (uPrevIdx + 1);
          LOG(info) << "Extra TRD digi: prev is " << uPrevIdx << " vs new " << event.GetIndex(ECbmDataType::kTrdDigi, uDigiIdx)
                    << " index " << uDigiIdx << " out of " << uNbDigis;
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kTrdDigi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the TRD block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    /// ==> TOF
    uNbDigis = (0 < event.GetNofData(ECbmDataType::kTofDigi) ? event.GetNofData(ECbmDataType::kTofDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiTof.begin() + event.GetIndex(ECbmDataType::kTofDigi, 0);
      auto stopIt  = fvDigiTof.begin() + event.GetIndex(ECbmDataType::kTofDigi, uNbDigis - 1);
      selEvent.fData.fTof.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kTofDigi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fTof.fDigis.push_back(fvDigiTof[event.GetIndex(ECbmDataType::kTofDigi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kTofDigi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kTofDigi, uDigiIdx) - (uPrevIdx + 1);
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kTofDigi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the TOF block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    /// ==> RICH
    uNbDigis = (0 < event.GetNofData(ECbmDataType::kRichDigi) ? event.GetNofData(ECbmDataType::kRichDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiRich.begin() + event.GetIndex(ECbmDataType::kRichDigi, 0);
      auto stopIt  = fvDigiRich.begin() + event.GetIndex(ECbmDataType::kRichDigi, uNbDigis - 1);
      selEvent.fData.fRich.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kRichDigi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fRich.fDigis.push_back(fvDigiRich[event.GetIndex(ECbmDataType::kRichDigi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kRichDigi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kRichDigi, uDigiIdx) - (uPrevIdx + 1);
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kRichDigi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the RICH block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    /// ==> PSD
    uNbDigis = (0 < event.GetNofData(ECbmDataType::kPsdDigi) ? event.GetNofData(ECbmDataType::kPsdDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiPsd.begin() + event.GetIndex(ECbmDataType::kPsdDigi, 0);
      auto stopIt  = fvDigiPsd.begin() + event.GetIndex(ECbmDataType::kPsdDigi, uNbDigis - 1);
      selEvent.fData.fPsd.fDigis.assign(startIt, stopIt);

      /*
      uint32_t uPrevIdx = event.GetIndex(ECbmDataType::kPsdDigi, 0);
      uint32_t uNbExtra = 0;
      for (uint32_t uDigiIdx = 1; uDigiIdx < uNbDigis; ++uDigiIdx) {
        // selEvent.fData.fPsd.fDigis.push_back(fvDigiPsd[event.GetIndex(ECbmDataType::kPsdDigi, uDigiIdx)]);
        if (uPrevIdx + 1 != event.GetIndex(ECbmDataType::kPsdDigi, uDigiIdx)) {
          uNbExtra += event.GetIndex(ECbmDataType::kPsdDigi, uDigiIdx) - (uPrevIdx + 1);
        }
        uPrevIdx = event.GetIndex(ECbmDataType::kPsdDigi, uDigiIdx);
      }
      if (0 < uNbExtra) {
        LOG(info) << "In event " << event.GetNumber() << " the PSD block selection added " << uNbExtra
                  << " extra digis compared to the loop one";
      }
      */
    }

    vEventsSel.push_back(selEvent);
  }

  return vEventsSel;
}
