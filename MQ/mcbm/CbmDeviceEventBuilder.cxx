/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau[committer], Dominik Smith */

#include "CbmDeviceEventBuilder.h"

/// CBM headers
#include "CbmEvent.h"
#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"
#include "CbmMatch.h"
#include "CbmMvdDigi.h"
#include "CbmTsEventHeader.h"

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
#include "TimesliceMetaData.h"

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"

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

CbmDeviceEventBuilder::CbmDeviceEventBuilder() {}

void CbmDeviceEventBuilder::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceEventBuilder.";
  //fbFillHistos      = fConfig->GetValue<bool>("FillHistos");
  //fbIgnoreTsOverlap = fConfig->GetValue<bool>("IgnOverMs");

  fsOutputFileName = fConfig->GetValue<std::string>("OutFileName");  //For storage of events

  // Event builder algorithm params
  const std::vector<std::string> vsAddDet        = fConfig->GetValue<std::vector<std::string>>("AddDet");
  const std::vector<std::string> vsSetEvbuildWin = fConfig->GetValue<std::vector<std::string>>("SetEvbuildWin");

  // Trigger algorithm params
  const std::string sTriggerDet = fConfig->GetValue<std::string>("TriggerDet");
  fTriggerWindow                = fConfig->GetValue<double>("TriggerWin");
  fMinNumDigis                  = fConfig->GetValue<int32_t>("TriggerMinDigis");
  fDeadTime                     = fConfig->GetValue<double>("TriggerDeadTime");

  fsChannelNameDataInput   = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput  = fConfig->GetValue<std::string>("EvtNameOut");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");
  fsAllowedChannels[0]     = fsChannelNameDataInput;

  fuPublishFreqTs  = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime = fConfig->GetValue<double_t>("PubTimeMax");

  /// Prepare root output
  if ("" != fsOutputFileName) {
    fpRun         = new FairRunOnline();
    fpFairRootMgr = FairRootManager::Instance();
    fpFairRootMgr->SetSink(new FairRootFileSink(fsOutputFileName));
    if (nullptr == fpFairRootMgr->GetOutFile()) { throw InitTaskError("Could not open root file"); }
    LOG(info) << "Init Root Output to " << fsOutputFileName;
    fpFairRootMgr->InitSink();

    /// Create storage objects
    fCbmTsEventHeaderOut = new CbmTsEventHeader();
    fpFairRootMgr->Register("EventHeader.", "Event", fCbmTsEventHeaderOut, kTRUE);

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
      OnData(entry.first, &CbmDeviceEventBuilder::HandleData);
    }
  }

  /// Extract refdet
  fTriggerDet = GetDetectorId(sTriggerDet);

  if (ECbmModuleId::kNotExist == fTriggerDet) {
    LOG(info) << "CbmDeviceEventBuilder::InitTask => Trying to change "
                 "reference to unsupported detector, ignored! "
              << sTriggerDet;
  }

  /// Extract detector to add if any
  /*
  for (std::vector<std::string>::const_iterator itStrAdd = vsAddDet.begin(); itStrAdd != vsAddDet.end(); ++itStrAdd) {
    const ECbmModuleId addDet = GetDetectorId(*itStrAdd);
    if (ECbmModuleId::kNotExist != addDet) { fEvbuildAlgo.AddSystem(addDet); }
    else {
      LOG(info) << "CbmDeviceEventBuilder::InitTask => Trying to add "
                   "unsupported detector, ignored! "
                << (*itStrAdd);
      continue;
    }
  }
  */

  /// Extract event builder window to add if any
  for (std::vector<std::string>::const_iterator itStrEvbuildWin = vsSetEvbuildWin.begin();
       itStrEvbuildWin != vsSetEvbuildWin.end(); ++itStrEvbuildWin) {
    size_t charPosDel = (*itStrEvbuildWin).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDeviceEventBuilder::InitTask => "
                << "Trying to set event builder window with invalid option pattern, ignored! "
                << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found " << (*itStrEvbuildWin) << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet       = (*itStrEvbuildWin).substr(0, charPosDel);
    const ECbmModuleId selDet = GetDetectorId(sSelDet);

    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info) << "CbmDeviceEventBuilder::InitTask => "
                << "Trying to set trigger window for unsupported detector, ignored! " << sSelDet;
      continue;
    }

    /// Window beginning
    charPosDel++;
    std::string sNext = (*itStrEvbuildWin).substr(charPosDel);
    charPosDel        = sNext.find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDeviceEventBuilder::InitTask => "
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

ECbmModuleId CbmDeviceEventBuilder::GetDetectorId(std::string detName)
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

bool CbmDeviceEventBuilder::IsChannelNameAllowed(std::string channelName)
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
bool CbmDeviceEventBuilder::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;
  LOG(info) << "Received message number " << fulNumMessages << " with " << parts.Size() << " parts"
            << ", size0: " << parts.At(0)->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  /// Extract unpacked data from input message
  uint32_t uPartIdx = 0;

  /// TS header
  CbmTsEventHeader* evtHeader = new CbmTsEventHeader();
  Deserialize<RootSerializer>(*parts.At(uPartIdx), evtHeader);
  ++uPartIdx;

  CbmDigiTimeslice ts;

  /// T0
  std::string msgStrT0(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issT0(msgStrT0);
  boost::archive::binary_iarchive inputArchiveT0(issT0);
  inputArchiveT0 >> ts.fData.fT0.fDigis;
  ++uPartIdx;

  /// STS
  std::string msgStrSts(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issSts(msgStrSts);
  boost::archive::binary_iarchive inputArchiveSts(issSts);
  inputArchiveSts >> ts.fData.fSts.fDigis;
  ++uPartIdx;

  /// MUCH
  std::string msgStrMuch(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issMuch(msgStrMuch);
  boost::archive::binary_iarchive inputArchiveMuch(issMuch);
  inputArchiveMuch >> ts.fData.fMuch.fDigis;
  ++uPartIdx;

  /// TRD
  std::string msgStrTrd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTrd(msgStrTrd);
  boost::archive::binary_iarchive inputArchiveTrd(issTrd);
  inputArchiveTrd >> ts.fData.fTrd.fDigis;
  ++uPartIdx;

  /// T0F
  std::string msgStrTof(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTof(msgStrTof);
  boost::archive::binary_iarchive inputArchiveTof(issTof);
  inputArchiveTof >> ts.fData.fTof.fDigis;
  ++uPartIdx;

  /// RICH
  std::string msgStrRich(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issRich(msgStrRich);
  boost::archive::binary_iarchive inputArchiveRich(issRich);
  inputArchiveRich >> ts.fData.fRich.fDigis;
  ++uPartIdx;

  /// PSD
  std::string msgStrPsd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issPsd(msgStrPsd);
  boost::archive::binary_iarchive inputArchivePsd(issPsd);
  inputArchivePsd >> ts.fData.fPsd.fDigis;
  ++uPartIdx;

  /// TS metadata
  TimesliceMetaData* tsMetaData = new TimesliceMetaData();
  Deserialize<RootSerializer>(*parts.At(uPartIdx), tsMetaData);
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

  const std::vector<double> triggers = GetTriggerTimes(ts);
  LOG(debug) << "triggers: " << triggers.size();

  /// Create events
  std::vector<CbmDigiEvent> vEvents = fEvbuildAlgo(ts, triggers);
  LOG(debug) << "vEvents size: " << vEvents.size();

  /// Send output message
  if (!SendEvents(vEvents, tsMetaData, evtHeader)) { return false; }

  /// Write events to file
  // FIXME: poor man solution with lots of data copy until we undertand how to properly deal
  /// with FairMq messages ownership and memory managment

  if ("" != fsOutputFileName) {
    (*fEventsSelOut) = std::move(vEvents);
    LOG(debug) << "fEventSel size: " << fEventsSelOut->size();

    (*fCbmTsEventHeaderOut) = std::move(*evtHeader);

    new ((*fTimeSliceMetaDataArrayOut)[fTimeSliceMetaDataArrayOut->GetEntriesFast()])
      TimesliceMetaData(std::move(*tsMetaData));

    DumpTreeEntry();

    fTimeSliceMetaDataArrayOut->Clear();
    fEventsSelOut->clear();
  }

  return true;
}

void CbmDeviceEventBuilder::DumpTreeEntry()
{
  // Unpacked digis + CbmEvent output to root file

  /// FairRunOnline style
  fpFairRootMgr->StoreWriteoutBufferData(fpFairRootMgr->GetEventTime());
  fpFairRootMgr->FillEventHeader(fCbmTsEventHeaderOut);
  fpFairRootMgr->Fill();
  fpFairRootMgr->DeleteOldWriteoutBufferData();
}

std::vector<double> CbmDeviceEventBuilder::GetTriggerTimes(const CbmDigiTimeslice& ts)
{
  std::vector<double> vDigiTimes;
  switch (fTriggerDet) {
    case ECbmModuleId::kMuch: {
      vDigiTimes = GetDigiTimes(ts.fData.fMuch.fDigis);
      break;
    }
    case ECbmModuleId::kSts: {
      vDigiTimes = GetDigiTimes(ts.fData.fSts.fDigis);
      break;
    }
    case ECbmModuleId::kTof: {
      vDigiTimes = GetDigiTimes(ts.fData.fTof.fDigis);
      break;
    }
    case ECbmModuleId::kTrd: {
      vDigiTimes = GetDigiTimes(ts.fData.fTrd.fDigis);
      break;
    }
    case ECbmModuleId::kRich: {
      vDigiTimes = GetDigiTimes(ts.fData.fRich.fDigis);
      break;
    }
    case ECbmModuleId::kPsd: {
      vDigiTimes = GetDigiTimes(ts.fData.fPsd.fDigis);
      break;
    }
    case ECbmModuleId::kT0: {
      vDigiTimes = GetDigiTimes(ts.fData.fT0.fDigis);
      break;
    }
    default: LOG(fatal) << "CbmDeviceEventBuilder::GetTriggerTimes(): Reading digis from unknown detector type!";
  }
  LOG(debug) << "CbmDeviceEventBuilder::GetTriggerTimes(): Building triggers from " << vDigiTimes.size() << " digis.";
  return fTriggerAlgo(vDigiTimes, fTriggerWindow, fMinNumDigis, fDeadTime);
}

bool CbmDeviceEventBuilder::SendEvents(const std::vector<CbmDigiEvent>& vEvents, const TimesliceMetaData* tsMetaData,
                                       const CbmTsEventHeader* eventHeader)
{
  LOG(debug) << "Vector size: " << vEvents.size();

  FairMQParts partsOut;

  /// Prepare serialized versions of the TS Event header
  FairMQMessagePtr messTsHeader(NewMessage());
  Serialize<RootSerializer>(*messTsHeader, eventHeader);
  partsOut.AddPart(std::move(messTsHeader));

  // Prepare TS meta data
  FairMQMessagePtr messTsMeta(NewMessage());
  Serialize<RootSerializer>(*messTsMeta, tsMetaData);
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

void CbmDeviceEventBuilder::Finish()
{
  if ("" != fsOutputFileName) {
    // Clean closure of output to root file
    fpFairRootMgr->Write();
    fpFairRootMgr->CloseSink();
  }
  fbFinishDone = kTRUE;
}

CbmDeviceEventBuilder::~CbmDeviceEventBuilder()
{
  /// Close things properly if not alredy done
  if (!fbFinishDone) Finish();
  if (fEventsSelOut) { delete fEventsSelOut; }
  if (fpRun) { delete fpRun; }
  if (fCbmTsEventHeaderOut) { delete fCbmTsEventHeaderOut; }
  if (fTimeSliceMetaDataArrayOut) { delete fTimeSliceMetaDataArrayOut; }
}
