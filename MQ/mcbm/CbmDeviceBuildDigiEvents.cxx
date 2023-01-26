/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau[committer] */

/**
 * CbmDeviceBuildDigiEvents.cxx
 *
 * @since 2021-11-18
 * @author P.-A. Loizeau
 */

#include "CbmDeviceBuildDigiEvents.h"

/// CBM headers
#include "CbmDigiEvent.h"
#include "CbmEvent.h"
#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"
#include "CbmMatch.h"
#include "CbmMvdDigi.h"
#include "CbmTsEventHeader.h"

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

CbmDeviceBuildDigiEvents::CbmDeviceBuildDigiEvents() { fpAlgo = new CbmAlgoBuildRawEvents(); }

void CbmDeviceBuildDigiEvents::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceBuildDigiEvents.";
  fbFillHistos      = fConfig->GetValue<bool>("FillHistos");
  fbIgnoreTsOverlap = fConfig->GetValue<bool>("IgnOverMs");

  fsEvtOverMode         = fConfig->GetValue<std::string>("EvtOverMode");
  fsRefDet              = fConfig->GetValue<std::string>("RefDet");
  fvsAddDet             = fConfig->GetValue<std::vector<std::string>>("AddDet");
  fvsDelDet             = fConfig->GetValue<std::vector<std::string>>("DelDet");
  fvsSetTrigWin         = fConfig->GetValue<std::vector<std::string>>("SetTrigWin");
  fvsSetTrigMinNb       = fConfig->GetValue<std::vector<std::string>>("SetTrigMinNb");
  fvsSetTrigMaxNb       = fConfig->GetValue<std::vector<std::string>>("SetTrigMaxNb");
  fvsSetTrigMinLayersNb = fConfig->GetValue<std::vector<std::string>>("SetTrigMinLayersNb");
  fvsSetHistMaxDigiNb   = fConfig->GetValue<std::vector<std::string>>("SetHistMaxDigiNb");

  fbDoNotSend              = fConfig->GetValue<bool>("DoNotSend");
  fbDigiEventOutput        = fConfig->GetValue<bool>("DigiEventOutput");
  fsChannelNameDataInput   = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput  = fConfig->GetValue<std::string>("EvtNameOut");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");
  fsAllowedChannels[0]     = fsChannelNameDataInput;

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
      OnData(entry.first, &CbmDeviceBuildDigiEvents::HandleData);
    }
  }

  /// FIXME: Disable clang formatting for now as it corrupts all alignment
  /* clang-format off */

  /// Initialize the Algorithm parameters
  fpAlgo->SetFillHistos(fbFillHistos);
  fpAlgo->SetIgnoreTsOverlap(fbIgnoreTsOverlap);
  /// Extract Event Overlap Mode
  EOverlapModeRaw mode = ("NoOverlap"    == fsEvtOverMode ? EOverlapModeRaw::NoOverlap
                       : ("MergeOverlap" == fsEvtOverMode ? EOverlapModeRaw::MergeOverlap
                       : ("AllowOverlap" == fsEvtOverMode ? EOverlapModeRaw::AllowOverlap
                                                          : EOverlapModeRaw::NoOverlap)));
  fpAlgo->SetEventOverlapMode(mode);
  /// Extract refdet
  RawEventBuilderDetector refDet = GetDetectorBuilderCfg(fsRefDet);
  if (kRawEventBuilderDetUndef != refDet) {
    fpAlgo->SetReferenceDetector(refDet);
  }
  else {
    LOG(info) << "CbmDeviceBuildDigiEvents::InitTask => Trying to change "
                 "reference to unsupported detector, ignored! "
              << fsRefDet;
  }

  /// Extract detector to add if any
  for (std::vector<std::string>::iterator itStrAdd = fvsAddDet.begin();
       itStrAdd != fvsAddDet.end();
       ++itStrAdd) {
    RawEventBuilderDetector addDet = GetDetectorBuilderCfg(*itStrAdd);
    if (kRawEventBuilderDetUndef != addDet) {
      fpAlgo->AddDetector(addDet);
    }
    else {
      LOG(info) << "CbmDeviceBuildDigiEvents::InitTask => Trying to add "
                   "unsupported detector, ignored! "
                << (*itStrAdd);
      continue;
    }
  }

  /// Extract detector to remove if any
  for (std::vector<std::string>::iterator itStrRem = fvsDelDet.begin();
       itStrRem != fvsDelDet.end();
       ++itStrRem) {
    RawEventBuilderDetector remDet = GetDetectorBuilderCfg(*itStrRem);
    if (kRawEventBuilderDetUndef != remDet) {
      fpAlgo->RemoveDetector(remDet);
    }
    else {
      LOG(info) << "CbmDeviceBuildDigiEvents::InitTask => Trying to remove "
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
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger window with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found "
        << (*itStrTrigWin) << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet = (*itStrTrigWin).substr(0, charPosDel);
    ECbmModuleId selDet = GetDetectorId(sSelDet);
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
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
        << "CbmDeviceBuildDigiEvents::InitTask => "
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
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger min Nb with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,uMinNb but instead found " << (*itStrMinNb)
        << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet = (*itStrMinNb).substr(0, charPosDel);
    ECbmModuleId selDet = GetDetectorId(sSelDet);
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger min Nb for unsupported detector, ignored! "
        << sSelDet;
      continue;
    }

    /// Min number
    charPosDel++;
    UInt_t uMinNb = std::stoul((*itStrMinNb).substr(charPosDel));

    fpAlgo->SetTriggerMinNumber(selDet, uMinNb);
  }

  /// Extract MaxNb for trigger if any
  for (std::vector<std::string>::iterator itStrMaxNb = fvsSetTrigMaxNb.begin();
       itStrMaxNb != fvsSetTrigMaxNb.end();
       ++itStrMaxNb) {
    size_t charPosDel = (*itStrMaxNb).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger Max Nb with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,uMaxNb but instead found " << (*itStrMaxNb)
        << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet = (*itStrMaxNb).substr(0, charPosDel);
    ECbmModuleId selDet = GetDetectorId(sSelDet);
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger Max Nb for unsupported detector, ignored! "
        << sSelDet;
      continue;
    }

    /// Max number
    charPosDel++;
    Int_t iMaxNb = std::stol((*itStrMaxNb).substr(charPosDel));

    fpAlgo->SetTriggerMaxNumber(selDet, iMaxNb);
  }

  /// Extract MinLayersNb for trigger if any
  for (std::vector<std::string>::iterator itStrMinLayersNb = fvsSetTrigMinLayersNb.begin();
       itStrMinLayersNb != fvsSetTrigMinLayersNb.end();
       ++itStrMinLayersNb) {
    size_t charPosDel = (*itStrMinLayersNb).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger min layers Nb with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,uMinLayersNb but instead found " << (*itStrMinLayersNb)
        << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet = (*itStrMinLayersNb).substr(0, charPosDel);
    ECbmModuleId selDet = GetDetectorId(sSelDet);
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set trigger min layers Nb for unsupported detector, ignored! "
        << sSelDet;
      continue;
    }

    /// Min number
    charPosDel++;
    UInt_t uMinLayersNb = std::stoul((*itStrMinLayersNb).substr(charPosDel));

    fpAlgo->SetTriggerMinLayersNumber(selDet, uMinLayersNb);
  }

  /// Extract Histograms Max Digi limits if any
  for (std::vector<std::string>::iterator itStrHistMaxDigi = fvsSetHistMaxDigiNb.begin();
       itStrHistMaxDigi != fvsSetHistMaxDigiNb.end();
       ++itStrHistMaxDigi) {
    size_t charPosDel = (*itStrHistMaxDigi).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set Histos max Digi nb with invalid option pattern, ignored! "
        << " (Should be ECbmModuleId,dMaxDigiNb but instead found " << (*itStrHistMaxDigi)
        << " )";
      continue;
    }

    /// Detector Enum Tag
    std::string sSelDet = (*itStrHistMaxDigi).substr(0, charPosDel);
    ECbmModuleId selDet = GetDetectorId(sSelDet);
    if (ECbmModuleId::kNotExist == selDet) {
      LOG(info)
        << "CbmDeviceBuildDigiEvents::InitTask => "
        << "Trying to set Histos max Digi nb for unsupported detector, ignored! "
        << sSelDet;
      continue;
    }

    /// Min number
    charPosDel++;
    Double_t dHistMaxDigiNb = std::stod((*itStrHistMaxDigi).substr(charPosDel));

    LOG(debug) << "set Histos max Digi nb to " << dHistMaxDigiNb;
    fpAlgo->SetHistogramMaxDigiNb(selDet, dHistMaxDigiNb);
  }

  /// FIXME: Re-enable clang formatting after formatted lines
  /* clang-format on */

  /// Create input vectors
  fvDigiT0   = new std::vector<CbmBmonDigi>(1000000);
  fvDigiSts  = new std::vector<CbmStsDigi>(1000000);
  fvDigiMuch = new std::vector<CbmMuchDigi>(1000000);
  fvDigiTrd  = new std::vector<CbmTrdDigi>(1000000);
  fvDigiTof  = new std::vector<CbmTofDigi>(1000000);
  fvDigiRich = new std::vector<CbmRichDigi>(1000000);
  fvDigiPsd  = new std::vector<CbmPsdDigi>(1000000);

  fCbmTsEventHeader = new CbmTsEventHeader();

  /// Digis storage
  fpAlgo->SetDigis(fvDigiT0);
  fpAlgo->SetDigis(fvDigiSts);
  fpAlgo->SetDigis(fvDigiMuch);
  fpAlgo->SetDigis(fvDigiTrd);
  fpAlgo->SetDigis(fvDigiTof);
  fpAlgo->SetDigis(fvDigiRich);
  fpAlgo->SetDigis(fvDigiPsd);

  // Mvd currently not implemented in event builder
  //std::vector<CbmMvdDigi>* pMvdDigi = new std::vector<CbmMvdDigi>();

  fTimeSliceMetaDataArray = new TClonesArray("TimesliceMetaData", 1);
  if (NULL == fTimeSliceMetaDataArray) { throw InitTaskError("Failed creating the TS meta data TClonesarray "); }
  fpAlgo->SetTimeSliceMetaDataArray(fTimeSliceMetaDataArray);

  /// Now that everything is set, initialize the Algorithm
  if (kFALSE == fpAlgo->InitAlgo()) { throw InitTaskError("Failed to initialize the algorithm class."); }

  /// Histograms management
  if (kTRUE == fbFillHistos) {
    /// Comment to prevent clang format single lining
    if (kFALSE == InitHistograms()) { throw InitTaskError("Failed to initialize the histograms."); }
  }
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceBuildDigiEvents::IsChannelNameAllowed(std::string channelName)
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

RawEventBuilderDetector CbmDeviceBuildDigiEvents::GetDetectorBuilderCfg(std::string detName)
{
  /// FIXME: Disable clang formatting for now as it corrupts all alignment
  /* clang-format off */
  RawEventBuilderDetector cfgDet = ("kT0"    == detName ? kRawEventBuilderDetT0
                                 : ("kSts"   == detName ? kRawEventBuilderDetSts
                                 : ("kMuch"  == detName ? kRawEventBuilderDetMuch
                                 : ("kTrd"   == detName ? kRawEventBuilderDetTrd
                                 : ("kTrd2D" == detName ? kRawEventBuilderDetTrd2D
                                 : ("kTof"   == detName ? kRawEventBuilderDetTof
                                 : ("kRich"  == detName ? kRawEventBuilderDetRich
                                 : ("kPsd"   == detName ? kRawEventBuilderDetPsd
                                                        : kRawEventBuilderDetUndef))))))));
  return cfgDet;
  /// FIXME: Re-enable clang formatting after formatted lines
  /* clang-format on */
}

ECbmModuleId CbmDeviceBuildDigiEvents::GetDetectorId(std::string detName)
{
  /// FIXME: Disable clang formatting for now as it corrupts all alignment
  /* clang-format off */
  ECbmModuleId detId = ("kT0"    == detName ? ECbmModuleId::kT0
                     : ("kSts"   == detName ? ECbmModuleId::kSts
                     : ("kMuch"  == detName ? ECbmModuleId::kMuch
                     : ("kTrd"   == detName ? ECbmModuleId::kTrd
                     : ("kTrd2D" == detName ? ECbmModuleId::kTrd2d
                     : ("kTof"   == detName ? ECbmModuleId::kTof
                     : ("kRich"  == detName ? ECbmModuleId::kRich
                     : ("kPsd"   == detName ? ECbmModuleId::kPsd
                                            : ECbmModuleId::kNotExist))))))));
  return detId;
  /// FIXME: Re-enable clang formatting after formatted lines
  /* clang-format on */
}


bool CbmDeviceBuildDigiEvents::InitHistograms()
{
  bool initOK = true;

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

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceBuildDigiEvents::HandleData(FairMQParts& parts, int /*index*/)
{
  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with " << parts.Size() << " parts"
             << ", size0: " << parts.At(0)->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  /// Extract unpacked data from input message
  uint32_t uPartIdx = 0;

  /// TS header
  //  Deserialize<RootSerializer>(*parts.At(uPartIdx), fCbmTsEventHeader);
  RootSerializer().Deserialize(*parts.At(uPartIdx), fCbmTsEventHeader);
  ++uPartIdx;

  /// T0
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrT0(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issT0(msgStrT0);
    boost::archive::binary_iarchive inputArchiveT0(issT0);
    inputArchiveT0 >> *fvDigiT0;
  }
  ++uPartIdx;

  /// STS
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrSts(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issSts(msgStrSts);
    boost::archive::binary_iarchive inputArchiveSts(issSts);
    inputArchiveSts >> *fvDigiSts;
  }
  ++uPartIdx;

  /// MUCH
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrMuch(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issMuch(msgStrMuch);
    boost::archive::binary_iarchive inputArchiveMuch(issMuch);
    inputArchiveMuch >> *fvDigiMuch;
  }
  ++uPartIdx;

  /// TRD
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrTrd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issTrd(msgStrTrd);
    boost::archive::binary_iarchive inputArchiveTrd(issTrd);
    inputArchiveTrd >> *fvDigiTrd;
  }
  ++uPartIdx;

  /// T0F
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrTof(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issTof(msgStrTof);
    boost::archive::binary_iarchive inputArchiveTof(issTof);
    inputArchiveTof >> *fvDigiTof;
  }
  ++uPartIdx;

  /// RICH
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrRich(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issRich(msgStrRich);
    boost::archive::binary_iarchive inputArchiveRich(issRich);
    inputArchiveRich >> *fvDigiRich;
  }
  ++uPartIdx;

  /// PSD
  if (0 < (parts.At(uPartIdx))->GetSize()) {
    std::string msgStrPsd(static_cast<char*>(parts.At(uPartIdx)->GetData()), (parts.At(uPartIdx))->GetSize());
    std::istringstream issPsd(msgStrPsd);
    boost::archive::binary_iarchive inputArchivePsd(issPsd);
    inputArchivePsd >> *fvDigiPsd;
  }
  ++uPartIdx;

  /// TS metadata
  //  Deserialize<RootSerializer>(*parts.At(uPartIdx), fTsMetaData);
  RootSerializer().Deserialize(*parts.At(uPartIdx), fTsMetaData);
  new ((*fTimeSliceMetaDataArray)[fTimeSliceMetaDataArray->GetEntriesFast()])
    TimesliceMetaData(std::move(*fTsMetaData));
  ++uPartIdx;

  LOG(debug) << "T0 Vector size: " << fvDigiT0->size();
  LOG(debug) << "STS Vector size: " << fvDigiSts->size();
  LOG(debug) << "MUCH Vector size: " << fvDigiMuch->size();
  LOG(debug) << "TRD Vector size: " << fvDigiTrd->size();
  LOG(debug) << "TOF Vector size: " << fvDigiTof->size();
  LOG(debug) << "RICH Vector size: " << fvDigiRich->size();
  LOG(debug) << "PSD Vector size: " << fvDigiPsd->size();

  if (1 == fulNumMessages) {
    /// First message received
    fpAlgo->SetTsParameters(0, fTsMetaData->GetDuration(), fTsMetaData->GetOverlapDuration());
  }

  /// Call Algo ProcessTs method
  fpAlgo->ProcessTs();

  /// Send events vector to ouput
  if (!fbDoNotSend) {
    if (fbDigiEventOutput) {
      if (!(SendDigiEvents(parts))) return false;
    }
    else {
      if (!(SendEvents(parts))) return false;
    }
  }

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
  }

  return true;
}

bool CbmDeviceBuildDigiEvents::SendEvents(FairMQParts& partsIn)
{
  /// Get vector reference from algo
  std::vector<CbmEvent*> vEvents = fpAlgo->GetEventVector();

  /// Move CbmEvent from temporary vector to std::vector of full objects
  LOG(debug) << "Vector size: " << vEvents.size();
  std::vector<CbmEvent> vOutEvents;
  for (CbmEvent* event : vEvents) {
    LOG(debug) << "Vector ptr: " << event->ToString();
    vOutEvents.push_back(std::move(*event));
    LOG(debug) << "Vector obj: " << vOutEvents[(vOutEvents.size()) - 1].ToString();
  }

  /// Serialize the array of events into a single MQ message
  /// FIXME: Find out if possible to use only the boost serializer
  FairMQMessagePtr message(NewMessage());
  //  Serialize<RootSerializer>(*message, &(vOutEvents));
  RootSerializer().Serialize(*message, &(vOutEvents));
  /*
  std::stringstream ossEvt;
  boost::archive::binary_oarchive oaEvt(ossEvt);
  oaEvt << vOutEvents;
  std::string* strMsgEvt = new std::string(ossEvt.str());
*/

  /// Add it at the end of the input composed message
  /// FIXME: Find out if possible to use only the boost serializer
  FairMQParts partsOut(std::move(partsIn));
  partsOut.AddPart(std::move(message));
  /*
  partsOut.AddPart(NewMessage(
    const_cast<char*>(strMsgEvt->c_str()),  // data
    strMsgEvt->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgEvt));  // object that manages the data
*/
  if (Send(partsOut, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  vOutEvents.clear();

  return true;
}

bool CbmDeviceBuildDigiEvents::SendDigiEvents(FairMQParts& partsIn)
{
  /// Get vector reference from algo
  std::vector<CbmEvent*> vEvents = fpAlgo->GetEventVector();

  /// Move CbmEvent from temporary vector to std::vector of full objects
  LOG(debug) << "In Vector size: " << vEvents.size();
  std::vector<CbmDigiEvent> vOutEvents;
  vOutEvents.reserve(vEvents.size());
  for (CbmEvent* event : vEvents) {
    CbmDigiEvent selEvent;
    selEvent.fTime   = event->GetStartTime();
    selEvent.fNumber = event->GetNumber();

    /// FIXME: for pure digi based event, we select "continuous slices of digis"
    ///        => Copy block of [First Digi index, last digi index] with assign(it_start, it_stop)
    /// FIXME: Keep TRD1D + TRD2D support, may lead to holes in the digi sequence!
    ///        => Would need to keep the loop

    /// Get the proper order for block selection as TRD1D and TRD2D may insert indices in separate loops
    /// => Needed to ensure that the start and stop of the block copy do not trigger a vector size exception
    event->SortIndices();

    /// for each detector, find the data in the Digi vectors and copy them
    /// TODO: Template + loop on list of data types?
    /// ==> T0
    uint32_t uNbDigis = (0 < event->GetNofData(ECbmDataType::kT0Digi) ? event->GetNofData(ECbmDataType::kT0Digi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiT0->begin() + event->GetIndex(ECbmDataType::kT0Digi, 0);
      auto stopIt  = fvDigiT0->begin() + event->GetIndex(ECbmDataType::kT0Digi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fT0.fDigis.assign(startIt, stopIt);
    }

    /// ==> STS
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kStsDigi) ? event->GetNofData(ECbmDataType::kStsDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiSts->begin() + event->GetIndex(ECbmDataType::kStsDigi, 0);
      auto stopIt  = fvDigiSts->begin() + event->GetIndex(ECbmDataType::kStsDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fSts.fDigis.assign(startIt, stopIt);
    }

    /// ==> MUCH
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kMuchDigi) ? event->GetNofData(ECbmDataType::kMuchDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiMuch->begin() + event->GetIndex(ECbmDataType::kMuchDigi, 0);
      auto stopIt  = fvDigiMuch->begin() + event->GetIndex(ECbmDataType::kMuchDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fMuch.fDigis.assign(startIt, stopIt);
    }

    /// ==> TRD + TRD2D
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kTrdDigi) ? event->GetNofData(ECbmDataType::kTrdDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiTrd->begin() + event->GetIndex(ECbmDataType::kTrdDigi, 0);
      auto stopIt  = fvDigiTrd->begin() + event->GetIndex(ECbmDataType::kTrdDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fTrd.fDigis.assign(startIt, stopIt);
    }

    /// ==> TOF
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kTofDigi) ? event->GetNofData(ECbmDataType::kTofDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiTof->begin() + event->GetIndex(ECbmDataType::kTofDigi, 0);
      auto stopIt  = fvDigiTof->begin() + event->GetIndex(ECbmDataType::kTofDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fTof.fDigis.assign(startIt, stopIt);
    }

    /// ==> RICH
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kRichDigi) ? event->GetNofData(ECbmDataType::kRichDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiRich->begin() + event->GetIndex(ECbmDataType::kRichDigi, 0);
      auto stopIt  = fvDigiRich->begin() + event->GetIndex(ECbmDataType::kRichDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fRich.fDigis.assign(startIt, stopIt);
    }

    /// ==> PSD
    uNbDigis = (0 < event->GetNofData(ECbmDataType::kPsdDigi) ? event->GetNofData(ECbmDataType::kPsdDigi) : 0);
    if (uNbDigis) {
      auto startIt = fvDigiPsd->begin() + event->GetIndex(ECbmDataType::kPsdDigi, 0);
      auto stopIt  = fvDigiPsd->begin() + event->GetIndex(ECbmDataType::kPsdDigi, uNbDigis - 1);
      ++stopIt;
      selEvent.fData.fPsd.fDigis.assign(startIt, stopIt);
    }

    vOutEvents.push_back(std::move(selEvent));
  }

  LOG(debug) << "Out Vector size: " << vEvents.size();
  /// Serialize the array of events into a single MQ message
  std::stringstream ossEvt;
  boost::archive::binary_oarchive oaEvt(ossEvt);
  oaEvt << vOutEvents;
  std::string* strMsgEvt = new std::string(ossEvt.str());
  FairMQMessagePtr message(NewMessage(
    const_cast<char*>(strMsgEvt->c_str()),  // data
    strMsgEvt->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgEvt));  // object that manages the data
  LOG(debug) << "Serializing done";

  /// Make a new composed messaged with TsHeader + vector of Digi Event + TsMetaData
  /// FIXME: Find out if possible to use only the boost serializer
  FairMQParts partsOut;
  partsOut.AddPart(std::move(partsIn.At(0)));                   // TsHeader
  partsOut.AddPart(std::move(partsIn.At(partsIn.Size() - 1)));  // TsMetaData
  partsOut.AddPart(std::move(message));                         // DigiEvent vector
  LOG(debug) << "Message preparation done";

  if (Send(partsOut, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  vOutEvents.clear();

  return true;
}

bool CbmDeviceBuildDigiEvents::SendHistoConfAndData()
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
  fpAlgo->ResetHistograms(kFALSE);

  return true;
}

bool CbmDeviceBuildDigiEvents::SendHistograms()
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
  fpAlgo->ResetHistograms(kFALSE);

  return true;
}

CbmDeviceBuildDigiEvents::~CbmDeviceBuildDigiEvents()
{
  /// Clear metadata
  if (fCbmTsEventHeader) delete fCbmTsEventHeader;

  /// Clear vectors
  if (fvDigiT0) fvDigiT0->clear();
  if (fvDigiSts) fvDigiSts->clear();
  if (fvDigiMuch) fvDigiMuch->clear();
  if (fvDigiTrd) fvDigiTrd->clear();
  if (fvDigiTof) fvDigiTof->clear();
  if (fvDigiRich) fvDigiRich->clear();
  if (fvDigiPsd) fvDigiPsd->clear();

  /// Clear metadata
  if (fTimeSliceMetaDataArray) {
    fTimeSliceMetaDataArray->Clear();
    delete fTsMetaData;

    delete fTimeSliceMetaDataArray;
  }
  if (fpAlgo) delete fpAlgo;
}

void CbmDeviceBuildDigiEvents::Finish() {}
