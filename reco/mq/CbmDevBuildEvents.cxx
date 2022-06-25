/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau[committer], Dominik Smith */

#include "CbmDevBuildEvents.h"

/// CBM headers
#include "CbmMQDefs.h"

/// FAIRROOT headers
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairRootFileSink.h"
#include "FairRootManager.h"
#include "FairRunOnline.h"

#include "BoostSerializer.h"

#include "RootSerializer.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "TimesliceMetaData.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

/// C/C++ headers
#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

CbmDevBuildEvents::CbmDevBuildEvents() {}

void CbmDevBuildEvents::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDevBuildEvents.";

  fsOutputFileName = fConfig->GetValue<std::string>("OutFileName");  //For storage of events

  // Event builder algorithm params
  const std::vector<std::string> vsSetEvbuildWin = fConfig->GetValue<std::vector<std::string>>("SetEvbuildWin");

  fsChannelNameDataInput  = fConfig->GetValue<std::string>("TrigNameIn");
  fsChannelNameDataOutput = fConfig->GetValue<std::string>("EvtNameOut");
  fsAllowedChannels[0]    = fsChannelNameDataInput;

  /// Prepare root output
  if ("" != fsOutputFileName) {
    fpRun         = new FairRunOnline();
    fpFairRootMgr = FairRootManager::Instance();
    fpFairRootMgr->SetSink(new FairRootFileSink(fsOutputFileName));
    if (nullptr == fpFairRootMgr->GetOutFile()) { throw InitTaskError("Could not open root file"); }
    LOG(info) << "Init Root Output to " << fsOutputFileName;
    fpFairRootMgr->InitSink();

    /// Create storage objects
    fEventsSelOut = new std::vector<CbmDigiEvent>();
    fpFairRootMgr->RegisterAny("DigiEvent", fEventsSelOut, kTRUE);

    fTimeSliceMetaDataArrayOut = new TClonesArray("TimesliceMetaData", 1);
    fpFairRootMgr->Register("TimesliceMetaData", "TS Meta Data", fTimeSliceMetaDataArrayOut, kTRUE);

    fpFairRootMgr->WriteFolder();
  }  // if( "" != fsOutputFileName )

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
      OnData(entry.first, &CbmDevBuildEvents::HandleData);
    }
  }

  /// Extract event builder window to add if any
  for (std::vector<std::string>::const_iterator itStrEvbuildWin = vsSetEvbuildWin.begin();
       itStrEvbuildWin != vsSetEvbuildWin.end(); ++itStrEvbuildWin) {
    size_t charPosDel = (*itStrEvbuildWin).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDevBuildEvents::InitTask => "
                << "Trying to set event builder window with invalid option pattern, ignored! "
                << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found " << (*itStrEvbuildWin) << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet       = (*itStrEvbuildWin).substr(0, charPosDel);
    const ECbmModuleId selDet = GetDetectorId(sSelDet);

    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info) << "CbmDevBuildEvents::InitTask => "
                << "Trying to set trigger window for unsupported detector, ignored! " << sSelDet;
      continue;
    }

    /// Window beginning
    charPosDel++;
    std::string sNext = (*itStrEvbuildWin).substr(charPosDel);
    charPosDel        = sNext.find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDevBuildEvents::InitTask => "
                << "Trying to set event builder window with invalid option pattern, ignored! "
                << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found " << (*itStrEvbuildWin) << " )";
      continue;
    }
    double dWinBeg = std::stod(sNext.substr(0, charPosDel));

    /// Window end
    charPosDel++;
    double dWinEnd = std::stod(sNext.substr(charPosDel));

    fEvbuildAlgo.SetEventWindow(selDet, dWinBeg, dWinEnd);
  }
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

ECbmModuleId CbmDevBuildEvents::GetDetectorId(std::string detName)
{
  /// FIXME: Disable clang formatting for now as it corrupts all alignment
  /* clang-format off */
  ECbmModuleId detId = ("kT0"   == detName ? ECbmModuleId::kT0
                       : ("kSts"  == detName ? ECbmModuleId::kSts
                       : ("kMuch" == detName ? ECbmModuleId::kMuch
                       : ("kTrd"  == detName ? ECbmModuleId::kTrd
                       : ("kTof"  == detName ? ECbmModuleId::kTof
                       : ("kRich" == detName ? ECbmModuleId::kRich
                       : ("kPsd"  == detName ? ECbmModuleId::kPsd
                                             : ECbmModuleId::kNotExist)))))));
  return detId; 
  /// FIXME: Re-enable clang formatting after formatted lines
  /* clang-format on */
}

bool CbmDevBuildEvents::IsChannelNameAllowed(std::string channelName)
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
    }
  }
  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDevBuildEvents::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;
  LOG(info) << "Received message number " << fulNumMessages << " with " << parts.Size() << " parts"
            << ", size0: " << parts.At(0)->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  /// Extract unpacked data from input message
  uint32_t uPartIdx = 0;

  /// TS
  CbmDigiTimeslice ts;
  std::string msgStrTS(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTS(msgStrTS);
  boost::archive::binary_iarchive inputArchiveTS(issTS);
  inputArchiveTS >> ts;
  ++uPartIdx;

  /// TS metadata
  TimesliceMetaData* tsMetaData = new TimesliceMetaData();
  RootSerializer().Deserialize(*parts.At(uPartIdx), tsMetaData);
  ++uPartIdx;

  /// Triggers
  std::vector<double> triggers;
  std::string msgStrTrig(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTrig(msgStrTrig);
  boost::archive::binary_iarchive inputArchiveTrig(issTrig);
  inputArchiveTrig >> triggers;
  ++uPartIdx;

  //if (1 == fulNumMessages) {
  /// First message received (do TS metadata stuff here)
  //fpAlgo->SetTsParameters(0, fTsMetaDataOut->GetDuration(), fTsMetaDataOut->GetOverlapDuration());
  //}

  LOG(debug) << "T0 Vector size: " << ts.fData.fT0.fDigis.size();
  LOG(debug) << "STS Vector size: " << ts.fData.fSts.fDigis.size();
  LOG(debug) << "MUCH Vector size: " << ts.fData.fMuch.fDigis.size();
  LOG(debug) << "TRD Vector size: " << ts.fData.fTrd.fDigis.size();
  LOG(debug) << "TOF Vector size: " << ts.fData.fTof.fDigis.size();
  LOG(debug) << "RICH Vector size: " << ts.fData.fRich.fDigis.size();
  LOG(debug) << "PSD Vector size: " << ts.fData.fPsd.fDigis.size();
  LOG(debug) << "triggers: " << triggers.size();

  /// Create events
  std::vector<CbmDigiEvent> vEvents = fEvbuildAlgo(ts, triggers);
  LOG(debug) << "vEvents size: " << vEvents.size();

  /// Send output message
  if (!SendEvents(vEvents, tsMetaData)) { return false; }

  /// Write events to file
  // FIXME: poor man solution with lots of data copy until we undertand how to properly deal
  /// with FairMq messages ownership and memory managment

  if ("" != fsOutputFileName) {
    (*fEventsSelOut) = std::move(vEvents);
    LOG(debug) << "fEventSel size: " << fEventsSelOut->size();

    new ((*fTimeSliceMetaDataArrayOut)[fTimeSliceMetaDataArrayOut->GetEntriesFast()])
      TimesliceMetaData(std::move(*tsMetaData));

    DumpTreeEntry();

    fTimeSliceMetaDataArrayOut->Clear();
    fEventsSelOut->clear();
  }

  return true;
}

void CbmDevBuildEvents::DumpTreeEntry()
{
  // Unpacked digis + CbmEvent output to root file

  /// FairRunOnline style
  fpFairRootMgr->StoreWriteoutBufferData(fpFairRootMgr->GetEventTime());
  fpFairRootMgr->Fill();
  fpFairRootMgr->DeleteOldWriteoutBufferData();
}

bool CbmDevBuildEvents::SendEvents(const std::vector<CbmDigiEvent>& vEvents, const TimesliceMetaData* tsMetaData)
{
  LOG(debug) << "Vector size: " << vEvents.size();

  FairMQParts partsOut;

  // Prepare TS meta data
  FairMQMessagePtr messTsMeta(NewMessage());
  RootSerializer().Serialize(*messTsMeta, tsMetaData);
  partsOut.AddPart(std::move(messTsMeta));

  // Prepare event vector.
  std::stringstream ossEvt;
  boost::archive::binary_oarchive oaEvt(ossEvt);
  oaEvt << vEvents;
  std::string* strMsgEvt = new std::string(ossEvt.str());

  partsOut.AddPart(NewMessage(
    const_cast<char*>(strMsgEvt->c_str()),  // data
    strMsgEvt->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgEvt));  // object that manages the data

  if (Send(partsOut, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }
  return true;
}

void CbmDevBuildEvents::Finish()
{
  if ("" != fsOutputFileName) {
    // Clean closure of output to root file
    fpFairRootMgr->Write();
    fpFairRootMgr->CloseSink();
  }
  fbFinishDone = kTRUE;
}

CbmDevBuildEvents::~CbmDevBuildEvents()
{
  /// Close things properly if not alredy done
  if (!fbFinishDone) Finish();
  if (fEventsSelOut) { delete fEventsSelOut; }
  if (fpRun) { delete fpRun; }
  if (fTimeSliceMetaDataArrayOut) { delete fTimeSliceMetaDataArrayOut; }
}
