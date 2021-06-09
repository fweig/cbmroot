/* Copyright (C) 2020-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Dominik Smith [committer] */

/**
 * CbmDeviceBuildRawEvents.cxx
 *
 * @since 2020-05-24
 * @author P.-A. Loizeau
 */

#include "CbmDeviceBuildRawEvents.h"

/// CBM headers
#include "CbmEvent.h"
#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"
#include "CbmMatch.h"
#include "CbmMvdDigi.h"

#include "TimesliceMetaData.h"

/// FAIRROOT headers
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"
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
#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

CbmDeviceBuildRawEvents::CbmDeviceBuildRawEvents() { fpAlgo = new CbmAlgoBuildRawEvents(); }

void CbmDeviceBuildRawEvents::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceBuildRawEvents.";
  fbFillHistos      = fConfig->GetValue<bool>("FillHistos");
  fbIgnoreTsOverlap = fConfig->GetValue<bool>("IgnOverMs");

  fsEvtOverMode   = fConfig->GetValue<std::string>("EvtOverMode");
  fsRefDet        = fConfig->GetValue<std::string>("RefDet");
  fvsAddDet       = fConfig->GetValue<std::vector<std::string>>("AddDet");
  fvsDelDet       = fConfig->GetValue<std::vector<std::string>>("DelDet");
  fvsSetTrigWin   = fConfig->GetValue<std::vector<std::string>>("SetTrigWin");
  fvsSetTrigMinNb = fConfig->GetValue<std::vector<std::string>>("SetTrigMinNb");

  fsChannelNameDataInput    = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput   = fConfig->GetValue<std::string>("EvtNameOut");
  fsChannelNameHistosInput  = fConfig->GetValue<std::string>("ChNameIn");
  fsChannelNameHistosConfig = fConfig->GetValue<std::string>("ChNameHistCfg");
  fsChannelNameCanvasConfig = fConfig->GetValue<std::string>("ChNameCanvCfg");
  fsAllowedChannels[0]      = fsChannelNameDataInput;

  fuPublishFreqTs  = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime = fConfig->GetValue<double_t>("PubTimeMax");

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
      OnData(entry.first, &CbmDeviceBuildRawEvents::HandleData);
    }
  }

  /// FIXME: Disable clang formatting for now as it corrupts all alignment
  /* clang-format off */

  /// Initialize the Algorithm parameters
  fpAlgo->SetFillHistos(fbFillHistos);
  fpAlgo->SetIgnoreTsOverlap(fbIgnoreTsOverlap);
  /// Extract Event Overlap Mode
  EOverlapModeRaw mode =
     ("NoOverlap"    == fsEvtOverMode ? EOverlapModeRaw::NoOverlap
   : ("MergeOverlap" == fsEvtOverMode ? EOverlapModeRaw::MergeOverlap
   : ("AllowOverlap" == fsEvtOverMode ? EOverlapModeRaw::AllowOverlap
                                      : EOverlapModeRaw::NoOverlap)));
  fpAlgo->SetEventOverlapMode(mode);
  /// Extract refdet
  RawEventBuilderDetector refDet = ("kT0"   == fsRefDet ? kRawEventBuilderDetT0
                              : ("kSts"  == fsRefDet ? kRawEventBuilderDetMuch
                              : ("kMuch" == fsRefDet ? kRawEventBuilderDetTrd
                              : ("kTrd"  == fsRefDet ? kRawEventBuilderDetTrd
                              : ("kTof"  == fsRefDet ? kRawEventBuilderDetTof
                              : ("kRich" == fsRefDet ? kRawEventBuilderDetRich
                              : ("kPsd"  == fsRefDet ? kRawEventBuilderDetPsd
                                                     : kRawEventBuilderDetUndef)))))));
  if (kRawEventBuilderDetUndef != refDet) {
    fpAlgo->SetReferenceDetector(refDet);
  } 
  else {
    LOG(info) << "CbmDeviceBuildRawEvents::InitTask => Trying to change "
                 "reference to unsupported detector, ignored! "
              << fsRefDet;
  }

  /// Extract detector to add if any
  for (std::vector<std::string>::iterator itStrAdd = fvsAddDet.begin();
       itStrAdd != fvsAddDet.end();
       ++itStrAdd) {
    RawEventBuilderDetector addDet = ("kT0"   == *itStrAdd ? kRawEventBuilderDetT0
                                : ("kSts"  == *itStrAdd ? kRawEventBuilderDetSts
                                : ("kMuch" == *itStrAdd ? kRawEventBuilderDetMuch
                                : ("kTrd"  == *itStrAdd ? kRawEventBuilderDetTrd
                                : ("kTof"  == *itStrAdd ? kRawEventBuilderDetTof
                                : ("kRich" == *itStrAdd ? kRawEventBuilderDetRich
                                : ("kPsd"  == *itStrAdd ? kRawEventBuilderDetPsd
                                                        : kRawEventBuilderDetUndef)))))));
    if (kRawEventBuilderDetUndef != addDet) {
      fpAlgo->AddDetector(addDet);
    } 
    else {
      LOG(info) << "CbmDeviceBuildRawEvents::InitTask => Trying to add "
                   "unsupported detector, ignored! "
                << (*itStrAdd);
      continue;
    }
  } 

     /// Extract detector to remove if any
  for (std::vector<std::string>::iterator itStrRem = fvsDelDet.begin();
       itStrRem != fvsDelDet.end();
       ++itStrRem) {
    RawEventBuilderDetector remDet = ("kT0"   == *itStrRem ? kRawEventBuilderDetT0
                                : ("kSts"  == *itStrRem ? kRawEventBuilderDetSts
                                : ("kMuch" == *itStrRem ? kRawEventBuilderDetMuch
                                : ("kTrd"  == *itStrRem ? kRawEventBuilderDetTrd
                                : ("kTof"  == *itStrRem ? kRawEventBuilderDetTof
                                : ("kRich" == *itStrRem ? kRawEventBuilderDetRich
                                : ("kPsd"  == *itStrRem ? kRawEventBuilderDetPsd
                                                        : kRawEventBuilderDetUndef)))))));
    if (kRawEventBuilderDetUndef != remDet) {
      fpAlgo->RemoveDetector(remDet);
    } 
    else {
      LOG(info) << "CbmDeviceBuildRawEvents::InitTask => Trying to remove "
                   "unsupported detector, ignored! "
                << (*itStrRem);
      continue;
    } 
  }  
     /// Extract Trigger window to add if any
  for (std::vector<std::string>::iterator itStrTrigWin = fvsSetTrigWin.begin();
       itStrTrigWin != fvsSetTrigWin.end();
       ++itStrTrigWin) {
    size_t charPosDel = (*itStrTrigWin).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info)
        << "CbmDeviceBuildRawEvents::InitTask => "
        << "Trying to set trigger window with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found "
        << (*itStrTrigWin) << " )";
      continue;
    } 

    /// Detector Enum Tag
    std::string sSelDet = (*itStrTrigWin).substr(0, charPosDel);
    ECbmModuleId selDet = ("kT0"   == sSelDet ? ECbmModuleId::kT0
                        : ("kSts"  == sSelDet ? ECbmModuleId::kSts
                        : ("kMuch" == sSelDet ? ECbmModuleId::kMuch
                        : ("kTrd"  == sSelDet ? ECbmModuleId::kTrd
                        : ("kTof"  == sSelDet ? ECbmModuleId::kTof
                        : ("kRich" == sSelDet ? ECbmModuleId::kRich
                        : ("kPsd"  == sSelDet ? ECbmModuleId::kPsd
                                              : ECbmModuleId::kNotExist)))))));
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildRawEvents::InitTask => "
        << "Trying to set trigger window for unsupported detector, ignored! "
        << sSelDet;
      continue;
    } 

    /// Window beginning
    charPosDel++;
    std::string sNext = (*itStrTrigWin).substr(charPosDel);
    charPosDel        = sNext.find(',');
    if (std::string::npos == charPosDel) {
      LOG(info)
        << "CbmDeviceBuildRawEvents::InitTask => "
        << "Trying to set trigger window with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found "
        << (*itStrTrigWin) << " )";
      continue;
    } 
    Double_t dWinBeg = std::stod(sNext.substr(0, charPosDel));

    /// Window end
    charPosDel++;
    Double_t dWinEnd = std::stod(sNext.substr(charPosDel));

    fpAlgo->SetTriggerWindow(selDet, dWinBeg, dWinEnd);
  }  
     /// Extract MinNb for trigger if any
  for (std::vector<std::string>::iterator itStrMinNb = fvsSetTrigMinNb.begin();
       itStrMinNb != fvsSetTrigMinNb.end();
       ++itStrMinNb) {
    size_t charPosDel = (*itStrMinNb).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info)
        << "CbmDeviceBuildRawEvents::InitTask => "
        << "Trying to set trigger min Nb with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,uMinNb but instead found " << (*itStrMinNb)
        << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet = (*itStrMinNb).substr(0, charPosDel);
    ECbmModuleId selDet = ("kT0"   == sSelDet ? ECbmModuleId::kT0
                        : ("kSts"  == sSelDet ? ECbmModuleId::kSts
                        : ("kMuch" == sSelDet ? ECbmModuleId::kMuch
                        : ("kTrd"  == sSelDet ? ECbmModuleId::kTrd
                        : ("kTof"  == sSelDet ? ECbmModuleId::kTof
                        : ("kRich" == sSelDet ? ECbmModuleId::kRich
                        : ("kPsd"  == sSelDet ? ECbmModuleId::kPsd
                                              : ECbmModuleId::kNotExist)))))));
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildRawEvents::InitTask => "
        << "Trying to set trigger min Nb for unsupported detector, ignored! "
        << sSelDet;
      continue;
    }

    /// Min number
    charPosDel++;
    UInt_t uMinNb = std::stoul((*itStrMinNb).substr(charPosDel));

    fpAlgo->SetTriggerMinNumber(selDet, uMinNb);
  } 

  /// FIXME: Re-enable clang formatting after formatted lines
  /* clang-format on */

  /// Create input vectors
  fvDigiT0   = new std::vector<CbmTofDigi>();
  fvDigiSts  = new std::vector<CbmStsDigi>();
  fvDigiMuch = new std::vector<CbmMuchBeamTimeDigi>();
  fvDigiTrd  = new std::vector<CbmTrdDigi>();
  fvDigiTof  = new std::vector<CbmTofDigi>();
  fvDigiRich = new std::vector<CbmRichDigi>();
  fvDigiPsd  = new std::vector<CbmPsdDigi>();

  fTimeSliceMetaDataArray = new TClonesArray("TimesliceMetaData", 1);
  if (NULL == fTimeSliceMetaDataArray) { throw InitTaskError("Failed creating the TS meta data TClonesarray "); }
  fpAlgo->SetTimeSliceMetaDataArray(fTimeSliceMetaDataArray);

  /// Digis storage
  fpAlgo->SetT0Digis(fvDigiT0);
  fpAlgo->SetStsDigis(fvDigiSts);
  fpAlgo->SetMuchBeamTimeDigis(fvDigiMuch);
  fpAlgo->SetTrdDigis(fvDigiTrd);
  fpAlgo->SetTofDigis(fvDigiTof);
  fpAlgo->SetRichDigis(fvDigiRich);
  fpAlgo->SetPsdDigis(fvDigiPsd);

  // Mvd currently not implemented in event builder
  //std::vector<CbmMvdDigi>* pMvdDigi = new std::vector<CbmMvdDigi>();

  /// Create output TClonesArray
  fEvents = new TClonesArray("CbmEvent", 500);

  /// Now that everything is set, initialize the Algorithm
  if (kFALSE == fpAlgo->InitAlgo()) { throw InitTaskError("Failed to initilize the algorithm class."); }

  /// Histograms management
  if (kTRUE == fbFillHistos) {
    /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
    std::vector<std::pair<TNamed*, std::string>> vHistos = fpAlgo->GetHistoVector();
    /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
    std::vector<std::pair<TCanvas*, std::string>> vCanvases = fpAlgo->GetCanvasVector();

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

      /// Serialize the vector of histo config into a single MQ message
      FairMQMessagePtr messageHist(NewMessage());
      Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageHist, psHistoConfig);

      /// Send message to the common histogram config messages queue
      if (Send(messageHist, fsChannelNameHistosConfig) < 0) { throw InitTaskError("Problem sending histo config"); }
      LOG(info) << "Config of hist  " << psHistoConfig.first.data() << " in folder " << psHistoConfig.second.data();
    }

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

      /// Serialize the vector of canvas config into a single MQ message
      FairMQMessagePtr messageCan(NewMessage());
      Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageCan, psCanvConfig);

      /// Send message to the common canvas config messages queue
      if (Send(messageCan, fsChannelNameCanvasConfig) < 0) { throw InitTaskError("Problem sending canvas config"); }

      LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data() << " is " << psCanvConfig.second.data();
    }
  }
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceBuildRawEvents::IsChannelNameAllowed(std::string channelName)
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
bool CbmDeviceBuildRawEvents::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  /// Extract unpacked data from input message
  uint32_t uPartIdx = 0;

  /// TS metadata
  Deserialize<RootSerializer>(*parts.At(uPartIdx), fTsMetaData);
  new ((*fTimeSliceMetaDataArray)[fTimeSliceMetaDataArray->GetEntriesFast()])
    TimesliceMetaData(std::move(*fTsMetaData));
  ++uPartIdx;

  /// T0
  std::string msgStrT0(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issT0(msgStrT0);
  boost::archive::binary_iarchive inputArchiveT0(issT0);
  inputArchiveT0 >> *fvDigiT0;
  ++uPartIdx;

  /// STS
  std::string msgStrSts(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issSts(msgStrSts);
  boost::archive::binary_iarchive inputArchiveSts(issSts);
  inputArchiveSts >> *fvDigiSts;
  ++uPartIdx;

  /// MUCH
  std::string msgStrMuch(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issMuch(msgStrMuch);
  boost::archive::binary_iarchive inputArchiveMuch(issMuch);
  inputArchiveMuch >> *fvDigiMuch;
  ++uPartIdx;

  /// TRD
  std::string msgStrTrd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTrd(msgStrTrd);
  boost::archive::binary_iarchive inputArchiveTrd(issTrd);
  inputArchiveTrd >> *fvDigiTrd;
  ++uPartIdx;

  /// T0F
  std::string msgStrTof(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issTof(msgStrTof);
  boost::archive::binary_iarchive inputArchiveTof(issTof);
  inputArchiveTof >> *fvDigiTof;
  ++uPartIdx;

  /// RICH
  std::string msgStrRich(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issRich(msgStrRich);
  boost::archive::binary_iarchive inputArchiveRich(issRich);
  inputArchiveRich >> *fvDigiRich;
  ++uPartIdx;

  /// PSD
  std::string msgStrPsd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
  std::istringstream issPsd(msgStrPsd);
  boost::archive::binary_iarchive inputArchivePsd(issPsd);
  inputArchivePsd >> *fvDigiPsd;
  ++uPartIdx;

  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Send events vector to ouput
  if (!SendEvents(parts)) return false;

  /// Clear metadata
  fTimeSliceMetaDataArray->Clear();

  /// Clear vectors
  fvDigiT0->clear();
  fvDigiSts->clear();
  fvDigiMuch->clear();
  fvDigiTrd->clear();
  fvDigiTof->clear();
  fvDigiRich->clear();
  fvDigiPsd->clear();

  /// Clear event vector after usage
  fpAlgo->ClearEventVector();
  fEvents->Clear("C");

  /// Histograms management
  if (kTRUE == fbFillHistos) {
    /// Send histograms each 100 time slices. Should be each ~1s
    /// Use also runtime checker to trigger sending after M s if
    /// processing too slow or delay sending if processing too fast
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double_t> elapsedSeconds    = currentTime - fLastPublishTime;
    if ((fdMaxPublishTime < elapsedSeconds.count())
        || (0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count())) {
      SendHistograms();
      fLastPublishTime = std::chrono::system_clock::now();
    }
  }

  return true;
}

bool CbmDeviceBuildRawEvents::SendEvents(FairMQParts& partsIn)
{
  /// Clear events TClonesArray before usage.
  fEvents->Delete();

  /// Get vector reference from algo
  std::vector<CbmEvent*> vEvents = fpAlgo->GetEventVector();

  /// Move CbmEvent from temporary vector to TClonesArray
  for (CbmEvent* event : vEvents) {
    LOG(debug) << "Vector: " << event->ToString();
    new ((*fEvents)[fEvents->GetEntriesFast()]) CbmEvent(std::move(*event));
    LOG(debug) << "TClonesArray: " << static_cast<CbmEvent*>(fEvents->At(fEvents->GetEntriesFast() - 1))->ToString();
  }

  /// Serialize the array of events into a single MQ message
  FairMQMessagePtr message(NewMessage());
  Serialize<RootSerializer>(*message, fEvents);

  /// Add it at the end of the input composed message
  FairMQParts partsOut(std::move(partsIn));
  partsOut.AddPart(std::move(message));

  if (Send(partsOut, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  return true;
}

bool CbmDeviceBuildRawEvents::SendHistograms()
{
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
  Serialize<RootSerializer>(*message, &fArrayHisto);

  /// Send message to the common histogram messages queue
  if (Send(message, fsChannelNameHistosInput) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  /// Reset the histograms after sending them (but do not reset the time)
  fpAlgo->ResetHistograms(kFALSE);

  return true;
}

CbmDeviceBuildRawEvents::~CbmDeviceBuildRawEvents()
{
  /// Clear metadata
  fTimeSliceMetaDataArray->Clear();
  delete fTsMetaData;

  /// Clear vectors
  fvDigiT0->clear();
  fvDigiSts->clear();
  fvDigiMuch->clear();
  fvDigiTrd->clear();
  fvDigiTof->clear();
  fvDigiRich->clear();
  fvDigiPsd->clear();

  /// Clear events TClonesArray
  fEvents->Delete();

  delete fpRun;
  delete fTimeSliceMetaDataArray;
  delete fEvents;
  delete fpAlgo;
}

void CbmDeviceBuildRawEvents::Finish() {}
