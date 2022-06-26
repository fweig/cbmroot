/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Pierre-Alain Loizeau, Volker Friese */

#include "CbmDevEventSink.h"

// CBM headers
#include "CbmMQDefs.h"

#include "TimesliceMetaData.h"

// FAIRROOT headers
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "BoostSerializer.h"

#include "RootSerializer.h"

// External packages
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

/// C++ headers
#include <thread>  // this_thread::sleep_for

#include <stdexcept>
#include <string>


using std::istringstream;
using std::string;
using std::vector;


struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};


// -----   Destructor   -------------------------------------------------------
CbmDevEventSink::~CbmDevEventSink()
{

  // Close things properly if not already done
  if (!fFinishDone) Finish();

  // Clear and delete members
  if (fTsMetaData) delete fTsMetaData;
  if (fEventVec != nullptr) {
    fEventVec->clear();
    delete fEventVec;
  }
  if (fFairRun) delete fFairRun;
}
// ----------------------------------------------------------------------------


// -----   Initialize   -------------------------------------------------------
void CbmDevEventSink::InitTask()
try {

  // Read options from executable
  LOG(info) << "Init options for CbmDevEventSink";
  string outputFileName       = fConfig->GetValue<std::string>("OutFileName");
  string channelNameDataInput = fConfig->GetValue<std::string>("ChannelNameDataInput");
  string channelNameCommands  = fConfig->GetValue<std::string>("ChannelNameCommands");

  // --- Hook action on input channels
  OnData(channelNameDataInput, &CbmDevEventSink::HandleData);
  OnData(channelNameCommands, &CbmDevEventSink::HandleCommand);

  // --- Prepare ROOT output
  // TODO: WE use FairRunOnline and FairRootManager to manage the output. There might be a more
  // elegant way.
  fTsMetaData = new TimesliceMetaData();
  fEventVec   = new vector<CbmDigiEvent>();
  if ("" != outputFileName) {
    fFairRun     = new FairRunOnline();
    fFairRootMgr = FairRootManager::Instance();
    fFairRootMgr->SetSink(new FairRootFileSink(outputFileName));
    if (nullptr == fFairRootMgr->GetOutFile()) throw InitTaskError("Could not open ROOT file");
  }
  else {
    throw InitTaskError("Empty output filename!");
  }
  fFairRootMgr->InitSink();
  fFairRootMgr->RegisterAny("TimesliceMetaData.", fTsMetaData, kTRUE);
  fFairRootMgr->RegisterAny("DigiEvent", fEventVec, kTRUE);
  fFairRootMgr->WriteFolder();
  LOG(info) << "Init ROOT Output to " << outputFileName;
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}
// ----------------------------------------------------------------------------


// -----   Finish execution   -------------------------------------------------
void CbmDevEventSink::Finish()
{
  fFairRootMgr->Write();
  fFairRootMgr->CloseSink();
  LOG(info) << "File closed after " << fNumMessages << " and saving " << fNumTs << " TS";
  LOG(info) << "Index of last processed timeslice: " << fPrevTsIndex, ChangeState(fair::mq::Transition::Stop);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  ChangeState(fair::mq::Transition::End);
  fFinishDone = true;
}
// ----------------------------------------------------------------------------


// -----   Handle command message   -------------------------------------------
bool CbmDevEventSink::HandleCommand(FairMQMessagePtr& msg, int)
{
  // Deserialize command string
  string command;
  string msgStrCmd(static_cast<char*>(msg->GetData()), msg->GetSize());
  istringstream issCmd(msgStrCmd);
  boost::archive::binary_iarchive inputArchiveCmd(issCmd);
  inputArchiveCmd >> command;

  // Command tag is up to the first blank
  size_t charPosDel = command.find(' ');
  string type       = command.substr(0, charPosDel);

  // EOF command
  if (type == "EOF") {

    // The second substring should be the last timeslice index
    if (charPosDel == string::npos) {
      LOG(error) << "HandleCommand: Incomplete EOF command " << command;
      return false;
    }
    charPosDel++;
    string rest = command.substr(charPosDel);
    charPosDel  = rest.find(' ');
    if (charPosDel == string::npos) {
      LOG(error) << "HandleCommand: Incomplete EOF command " << command;
      return false;
    }
    uint64_t lastTsIndex = std::stoul(rest.substr(0, charPosDel));

    // The third substring should be the timeslice count
    charPosDel++;
    uint64_t numTs = std::stoul(rest.substr(charPosDel));

    // Log
    LOG(info) << "HandleCommand: Received EOF command with final TS index " << lastTsIndex << " and total number of TS "
              << numTs;
    Finish();
  }  //? EOF

  // STOP command
  else if (type == "STOP") {
    LOG(info) << "HandleCommand: Received STOP command";
    Finish();
  }

  // Unknown command
  else {
    LOG(warning) << "HandleCommand: Unknown command " << type << " => will be ignored!";
  }

  return true;
}
// ----------------------------------------------------------------------------


// -----   Handle data in input channel   -------------------------------------
bool CbmDevEventSink::HandleData(FairMQParts& parts, int)
{
  fNumMessages++;
  LOG(debug) << "Received message number " << fNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();
  if (0 == fNumMessages % 10000) LOG(info) << "Received " << fNumMessages << " messages";

  // --- Extract TimesliceMetaData (part 0)    TObject* tempObjectPointer = nullptr;
  TObject* tempObjectPointer = nullptr;
  RootSerializer().Deserialize(*parts.At(0), tempObjectPointer);
  if (tempObjectPointer && TString(tempObjectPointer->ClassName()).EqualTo("TimesliceMetaData")) {
    (*fTsMetaData) = *(static_cast<TimesliceMetaData*>(tempObjectPointer));
  }
  else {
    LOG(fatal) << "Failed to deserialize the TS metadata";
  }

  // --- Extract event vector (part 1)
  std::string msgStrEvt(static_cast<char*>(parts.At(1)->GetData()), (parts.At(1))->GetSize());
  std::istringstream issEvt(msgStrEvt);
  boost::archive::binary_iarchive inputArchiveEvt(issEvt);
  inputArchiveEvt >> (*fEventVec);

  // --- Dump tree entry for this timeslice
  fFairRootMgr->StoreWriteoutBufferData(fFairRootMgr->GetEventTime());
  fFairRootMgr->Fill();
  fFairRootMgr->DeleteOldWriteoutBufferData();
  fEventVec->clear();

  // --- Timeslice log
  LOG(info) << "Processed TS " << fTsMetaData->GetIndex() << " with " << fEventVec->size() << " events";

  return true;
}
// ----------------------------------------------------------------------------
