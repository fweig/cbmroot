/* Copyright (C) 2020-2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Dominik Smith */

#include "CbmDevEventSink.h"

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

CbmDevEventSink::CbmDevEventSink() {}

void CbmDevEventSink::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDevEventSink.";

  fsOutputFileName = fConfig->GetValue<std::string>("OutFileName");

  fsChannelNameDataInput = fConfig->GetValue<std::string>("EvtNameIn");
  fsAllowedChannels[0]   = fsChannelNameDataInput;

  fbFillHistos             = fConfig->GetValue<bool>("FillHistos");
  fuPublishFreqTs          = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime         = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime         = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");

  /// Associate the MissedTs Channel to the corresponding handler
  OnData(fsChannelNameMissedTs, &CbmDevEventSink::HandleMissTsData);

  /// Associate the command Channel to the corresponding handler
  OnData(fsChannelNameCommands, &CbmDevEventSink::HandleCommand);

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
      OnData(entry.first, &CbmDevEventSink::HandleData);
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
  /// Register all input data members with the FairRoot manager
  /// TS MetaData
  fpFairRootMgr->Register("TimesliceMetaData", "TS Meta Data", fTimeSliceMetaDataArray, kTRUE);
  /// CbmEvent
  fpFairRootMgr->RegisterAny("DigiEvent", fEventsSel, kTRUE);
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

bool CbmDevEventSink::IsChannelNameAllowed(std::string channelName)
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

bool CbmDevEventSink::InitHistograms()
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
bool CbmDevEventSink::HandleMissTsData(FairMQMessagePtr& msg, int /*index*/)
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
bool CbmDevEventSink::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  /// Unpack the message
  CbmEventTimeslice unpTs(parts);

  /// FIXME: Need to check if TS arrived in order (probably not!!!) + buffer!!!
  LOG(debug) << "Next TS check " << fuPrevTsIndex << " " << fulTsCounter << " " << unpTs.fTsMetaData.GetIndex();
  if (fuPrevTsIndex + 1 == unpTs.fTsMetaData.GetIndex()
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

  /// Check TS queue and process it if needed (in case it filled a hole!)
  CheckTsQueues();
  LOG(debug) << "TS queues checked";

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
bool CbmDevEventSink::HandleCommand(FairMQMessagePtr& msg, int /*index*/)
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
      LOG(fatal) << "CbmDevEventSink::HandleCommand => "
                 << "Incomplete EOF command received: " << sCommand;
      return false;
    }  // if( std::string::npos == charPosDel )
       /// Last TS index
    charPosDel++;
    std::string sNext = sCommand.substr(charPosDel);
    charPosDel        = sNext.find(' ');

    if (std::string::npos == charPosDel) {
      LOG(fatal) << "CbmDevEventSink::HandleCommand => "
                 << "Incomplete EOF command received: " << sCommand;
      return false;
    }  // if( std::string::npos == charPosDel )
    fuLastTsIndex = std::stoul(sNext.substr(0, charPosDel));
    /// Total TS count
    charPosDel++;
    fuTotalTsCount = std::stoul(sNext.substr(charPosDel));

    LOG(info) << "CbmDevEventSink::HandleCommand => "
              << "Received EOF command with final TS index " << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
    /// End of data: clean save of data + close file + send last state of histos if enabled
    if (fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount) {
      LOG(info) << "CbmDevEventSink::HandleCommand => "
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
void CbmDevEventSink::CheckTsQueues()
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
      LOG(debug) << "CbmDevEventSink::CheckTsQueues => Full TS " << (*itFullTs).first << " VS " << (fuPrevTsIndex + 1);
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
      LOG(debug) << "CbmDevEventSink::CheckTsQueues => Empty TS " << (*itMissTs) << " VS " << (fuPrevTsIndex + 1);

    /// Should be reached only if both queues at the end or hole found in both
    bHoleFoundInBothQueues = true;
  }  // while( !bHoleFoundInBothQueues )

  LOG(debug) << "CbmDevEventSink::CheckTsQueues => buffered TS " << fmFullTsStorage.size() << " buffered empties "
             << fvulMissedTsIndices.size();
  for (auto it = fmFullTsStorage.begin(); it != fmFullTsStorage.end(); ++it) {
    LOG(debug) << "CbmDevEventSink::CheckTsQueues => buffered TS index " << (*it).first;
  }

  /// Delete the processed entries
  fmFullTsStorage.erase(fmFullTsStorage.begin(), itFullTs);
  fvulMissedTsIndices.erase(fvulMissedTsIndices.begin(), itMissTs);

  /// End of data: clean save of data + close file + send last state of histos if enabled
  if (fbReceivedEof && fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount) {
    LOG(info) << "CbmDevEventSink::CheckTsQueues => "
              << "Found final TS index " << fuLastTsIndex << " and total nb TS " << fuTotalTsCount;
    Finish();
  }  // if( fbReceivedEof && fuPrevTsIndex == fuLastTsIndex && fulTsCounter == fuTotalTsCount )
}
//--------------------------------------------------------------------//
void CbmDevEventSink::PrepareTreeEntry(CbmEventTimeslice unpTs)
{
  /// FIXME: poor man solution with lots of data copy until we undertsnad how to properly deal
  /// with FairMq messages ownership and memory managment

  /// FIXME: Not sure if this is the proper way to insert the data
  new ((*fTimeSliceMetaDataArray)[fTimeSliceMetaDataArray->GetEntriesFast()])
    TimesliceMetaData(std::move(unpTs.fTsMetaData));

  /// Extract CbmEvent TClonesArray from input message
  (*fEventsSel) = std::move(unpTs.GetSelectedData());
}

void CbmDevEventSink::DumpTreeEntry()
{
  // Unpacked digis + CbmEvent output to root file
  /// FairRunOnline style
  fpFairRootMgr->StoreWriteoutBufferData(fpFairRootMgr->GetEventTime());
  fpFairRootMgr->Fill();
  fpFairRootMgr->DeleteOldWriteoutBufferData();

  /// Clear metadata array
  fTimeSliceMetaDataArray->Clear();

  /// Clear event vector
  fEventsSel->clear();
}

//--------------------------------------------------------------------//

bool CbmDevEventSink::SendHistoConfAndData()
{
  /// Prepare multiparts message and header
  std::pair<uint32_t, uint32_t> pairHeader(fvpsHistosFolder.size(), fvpsCanvasConfig.size());
  FairMQMessagePtr messageHeader(NewMessage());
  BoostSerializer<std::pair<uint32_t, uint32_t>>().Serialize(*messageHeader, pairHeader);

  FairMQParts partsOut;
  partsOut.AddPart(std::move(messageHeader));

  for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto) {
    /// Serialize the vector of histo config into a single MQ message
    FairMQMessagePtr messageHist(NewMessage());
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageHist, fvpsHistosFolder[uHisto]);

    partsOut.AddPart(std::move(messageHist));
  }  // for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto)

  for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv) {
    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageCan, fvpsCanvasConfig[uCanv]);

    partsOut.AddPart(std::move(messageCan));
  }  // for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv)

  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr msgHistos(NewMessage());
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

bool CbmDevEventSink::SendHistograms()
{
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
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
CbmDevEventSink::~CbmDevEventSink()
{
  /// FIXME: Add pointers check before delete

  /// Close things properly if not alredy done
  if (!fbFinishDone) Finish();

  /// Clear events vector
  fEventsSel->clear();
  delete fEventsSel;

  delete fpRun;
}

void CbmDevEventSink::Finish()
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

  /// TS metadata
  TObject* tempObjectPointer = nullptr;
  RootSerializer().Deserialize(*parts.At(uPartIdx), tempObjectPointer);

  if (tempObjectPointer && TString(tempObjectPointer->ClassName()).EqualTo("TimesliceMetaData")) {
    fTsMetaData = *(static_cast<TimesliceMetaData*>(tempObjectPointer));
  }
  else {
    LOG(fatal) << "Failed to deserialize the TS metadata";
  }
  ++uPartIdx;

  /// Events
  std::string msgStrEvt(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issEvt(msgStrEvt);
  boost::archive::binary_iarchive inputArchiveEvt(issEvt);
  inputArchiveEvt >> fvEvents;
  ++uPartIdx;

  LOG(debug) << "Input event array " << fvEvents.size();
}

CbmEventTimeslice::~CbmEventTimeslice() { fvEvents.clear(); }

std::vector<CbmDigiEvent> CbmEventTimeslice::GetSelectedData() { return fvEvents; }
