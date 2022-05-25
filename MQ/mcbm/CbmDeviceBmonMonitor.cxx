/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * CbmDeviceBmonMonitor.cxx
 *
 * @since 2022-05-23
 * @author P.-A. Loizeau
 */

#include "CbmDeviceBmonMonitor.h"

#include "CbmBmonUnpackConfig.h"
#include "CbmFlesCanvasTools.h"
#include "CbmMQDefs.h"
#include "CbmMuchUnpackConfig.h"
#include "CbmPsdUnpackConfig.h"
#include "CbmRichUnpackConfig.h"
#include "CbmSetup.h"
#include "CbmStsUnpackConfig.h"
#include "CbmTofUnpackConfig.h"
#include "CbmTofUnpackMonitor.h"
#include "CbmTrdUnpackConfig.h"
#include "CbmTrdUnpackFaspConfig.h"

#include "StorableTimeslice.hpp"
#include "TimesliceMetaData.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"

#include "BoostSerializer.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>

#include "RootSerializer.h"
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;

//Bool_t bMcbm2018MonitorTaskT0ResetHistos = kFALSE;

CbmDeviceBmonMonitor::CbmDeviceBmonMonitor() {}

void CbmDeviceBmonMonitor::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmDeviceBmonMonitor.";
  fsSetupName              = fConfig->GetValue<std::string>("Setup");
  fuRunId                  = fConfig->GetValue<uint32_t>("RunId");
  fbUnpBmon                = fConfig->GetValue<bool>("UnpBmon");
  fbIgnoreOverlapMs        = fConfig->GetValue<bool>("IgnOverMs");
  fbOutputFullTimeSorting  = fConfig->GetValue<bool>("FullTimeSort");
  fvsSetTimeOffs           = fConfig->GetValue<std::vector<std::string>>("SetTimeOffs");
  fsChannelNameDataInput   = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameDataOutput  = fConfig->GetValue<std::string>("TsNameOut");
  fuPublishFreqTs          = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime         = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime         = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

Bool_t CbmDeviceBmonMonitor::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceBmonMonitor.";

  // ----- FIXME: Environment settings? or binary option?
  TString srcDir = std::getenv("VMCWORKDIR");  // top source directory, standard C++ library
  //  TString srcDir = gSystem->Getenv("VMCWORKDIR");  // top source directory

  // -----   CbmSetup   -----------------------------------------------------
  // TODO: support for multiple setups on Par Server? with request containing setup name?
  CbmSetup* cbmsetup = CbmSetup::Instance();
  FairMQMessagePtr req(NewSimpleMessage("setup"));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, "parameters") > 0) {
    if (Receive(rep, "parameters") >= 0) {
      if (0 != rep->GetSize()) {
        CbmSetupStorable* exchangableSetup;

        CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
        exchangableSetup = dynamic_cast<CbmSetupStorable*>(tmsg.ReadObject(tmsg.GetClass()));

        if (nullptr != exchangableSetup) {
          /// Prevent clang format single line if
          cbmsetup->LoadStoredSetup(exchangableSetup);
        }
        else {
          LOG(error) << "Received corrupt reply. Setup not available";
          throw InitTaskError("Setup not received from par-server.");
        }
      }  // if( 0 !=  rep->GetSize() )
      else {
        LOG(error) << "Received empty reply. Setup not available";
        throw InitTaskError("Setup not received from par-server.");
      }  // else of if( 0 !=  rep->GetSize() )
    }    // if( Receive( rep, "parameters" ) >= 0)
  }      // if( Send(req, "parameters") > 0 )
  // ------------------------------------------------------------------------

  /// Initialize the UnpackerConfigs objects and their "user options"
  // ---- BMON ----
  std::shared_ptr<CbmBmonUnpackConfig> bmonconfig = nullptr;
  if (fbUnpBmon) {
    bmonconfig = std::make_shared<CbmBmonUnpackConfig>("", fuRunId);
    if (bmonconfig) {
      // bmonconfig->SetDebugState();
      bmonconfig->SetDoWriteOutput();
      // bmonconfig->SetDoWriteOptOutA("CbmBmonErrors");
      std::string parfilesbasepathBmon = Form("%s/macro/beamtime/mcbm2022/", srcDir.Data());
      bmonconfig->SetParFilesBasePath(parfilesbasepathBmon);
      bmonconfig->SetParFileName("mBmonCriPar.par");
      bmonconfig->SetSystemTimeOffset(-1220);  // [ns] value to be updated

      /// Enable Monitor plots
      auto monitor = std::make_shared<CbmTofUnpackMonitor>();
      monitor->SetBmonMode(true);
      monitor->SetInternalHttpMode(false);
      if (2337 <= fuRunId) {
        monitor->SetSpillThreshold(250);
        monitor->SetSpillThresholdNonPulser(100);
      }
      bmonconfig->SetMonitor(monitor);
    }
  }
  // -------------

  /// Enable full time sorting instead of time sorting per FLIM link
  if (bmonconfig) SetUnpackConfig(bmonconfig);

  /// Load time offsets
  for (std::vector<std::string>::iterator itStrOffs = fvsSetTimeOffs.begin(); itStrOffs != fvsSetTimeOffs.end();
       ++itStrOffs) {
    size_t charPosDel = (*itStrOffs).find(',');
    if (std::string::npos == charPosDel) {
      LOG(info) << "CbmDeviceBmonMonitor::InitContainers => "
                << "Trying to set trigger window with invalid option pattern, ignored! "
                << " (Should be ECbmModuleId,dWinBeg,dWinEnd but instead found " << (*itStrOffs) << " )";
    }  // if( std::string::npos == charPosDel )

    /// Detector Enum Tag
    std::string sSelDet = (*itStrOffs).substr(0, charPosDel);
    /// Min number
    charPosDel++;
    int32_t iOffset = std::stoi((*itStrOffs).substr(charPosDel));

    if ("kT0" == sSelDet && fBmonConfig) {  //
      fBmonConfig->SetSystemTimeOffset(iOffset);
    }  // else if( "kT0" == sSelDet )
    else {
      LOG(info) << "CbmDeviceBmonMonitor::InitContainers => Trying to set time "
                   "offset for unsupported detector, ignored! "
                << (sSelDet);
      continue;
    }  // else of detector enum detection
  }  // for( std::vector< std::string >::iterator itStrAdd = fvsAddDet.begin(); itStrAdd != fvsAddDet.end(); ++itStrAdd )

  Bool_t initOK = kTRUE;
  // --- Bmon
  if (fBmonConfig) {
    fBmonConfig->InitOutput();
    //    RegisterOutputs(ioman, fBmonConfig);  /// Framework bound work = kept in this Task
    fBmonConfig->SetAlgo();
    fBmonConfig->LoadParFileName();  /// Needed to change the Parameter file name before it is used!!!
    initOK &= InitParameters(fBmonConfig->GetParContainerRequest());  /// Framework bound work = kept in this Device
    fBmonConfig->InitAlgo();
    // initPerformanceMaps(fkFlesBmon, "Bmon");
  }

  /// Event header object
  fCbmTsEventHeader = new CbmTsEventHeader();

  return initOK;
}

Bool_t
CbmDeviceBmonMonitor::InitParameters(std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>* reqparvec)
{
  LOG(info) << "CbmDeviceBmonMonitor::InitParameters";
  if (!reqparvec) {
    LOG(info) << "CbmDeviceBmonMonitor::InitParameters - empty requirements vector no parameters initialized.";
    return kTRUE;
  }

  // Now get the actual ascii files and init the containers with the asciiIo
  for (auto& pair : *reqparvec) {
    /*
    auto filepath = pair.first;
    auto parset   = pair.second;
    FairParAsciiFileIo asciiInput;
    if (!filepath.empty()) {
      if (asciiInput.open(filepath.data())) { parset->init(&asciiInput); }
    }
    * */
    std::string paramName {pair.second->GetName()};
    // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
    // Should only be used for small data because of the cost of an additional copy

    // Here must come the proper Runid
    std::string message = paramName + ",111";
    LOG(info) << "Requesting parameter container " << paramName << ", sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    FairParGenericSet* newObj = nullptr;

    if (Send(req, "parameters") > 0) {
      if (Receive(rep, "parameters") >= 0) {
        if (0 != rep->GetSize()) {
          CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
          newObj = static_cast<FairParGenericSet*>(tmsg.ReadObject(tmsg.GetClass()));
          LOG(info) << "Received unpack parameter from the server: " << newObj->GetName();
          newObj->print();
        }  // if( 0 !=  rep->GetSize() )
        else {
          LOG(error) << "Received empty reply. Parameter not available";
          return kFALSE;
        }                       // else of if( 0 !=  rep->GetSize() )
      }                         // if( Receive( rep, "parameters" ) >= 0)
    }                           // if( Send(req, "parameters") > 0 )
    pair.second.reset(newObj);  /// Potentially unsafe reasignment of raw pointer to the shared pointer?
    //delete newObj;
  }
  return kTRUE;
}

bool CbmDeviceBmonMonitor::InitHistograms()
{
  /// Histos creation and obtain pointer on them
  /// Trigger histo creation on all associated algos
  // ALGO: bool initOK = fMonitorAlgo->CreateHistograms();
  bool initOK = true;

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  // ALGO: std::vector<std::pair<TNamed*, std::string>> vHistos = fMonitorAlgo->GetHistoVector();
  std::vector<std::pair<TNamed*, std::string>> vHistos = fBmonConfig->GetMonitor()->GetHistoVector();
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  // ALGO: std::vector<std::pair<TCanvas*, std::string>> vCanvases = fMonitorAlgo->GetCanvasVector();
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = fBmonConfig->GetMonitor()->GetCanvasVector();

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

// Method called by run loop and requesting new data from the TS source whenever
bool CbmDeviceBmonMonitor::ConditionalRun()
{
  /// First do Algo related Initialization steps if needed
  if (0 == fulNumMessages) {
    try {
      InitContainers();
    }
    catch (InitTaskError& e) {
      LOG(error) << e.what();
      ChangeState(fair::mq::Transition::ErrorFound);
    }
  }  // if( 0 == fulNumMessages)

  if (0 == fulNumMessages) InitHistograms();

  /// If first TS of this device, ask for the start time (lead to skip of 1 TS for 1st request)
  if (!fbStartTimeSet) {
    /// Request the start time
    std::string message = "SendFirstTimesliceIndex";
    LOG(debug) << "Requesting start time by sending message: SendFirstTimesliceIndex" << message;
    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    if (Send(req, fsChannelNameDataInput) <= 0) {
      LOG(error) << "Failed to send the request! message was " << message;
      return false;
    }  // if (Send(req, fsChannelNameDataInput) <= 0)
    else if (Receive(rep, fsChannelNameDataInput) < 0) {
      LOG(error) << "Failed to receive a reply to the request! message was " << message;
      return false;
    }  // else if (Receive(rep, fsChannelNameDataInput) < 0)
    else if (rep->GetSize() == 0) {
      LOG(error) << "Received empty reply. Something went wrong with the timeslice generation! message was " << message;
      return false;
    }  // else if (rep->GetSize() == 0)
    std::string sReply;
    std::string msgStrRep(static_cast<char*>(rep->GetData()), rep->GetSize());
    std::istringstream issRep(msgStrRep);
    boost::archive::binary_iarchive inputArchiveRep(issRep);
    inputArchiveRep >> sReply;

    fBmonConfig->GetMonitor()->SetHistosStartTime((1e-9) * static_cast<double>(std::stoul(sReply)));
    fbStartTimeSet = true;
  }

  /// First request a new TS (full one)
  std::string message = "full";
  LOG(debug) << "Requesting new TS by sending message: full" << message;
  FairMQMessagePtr req(NewSimpleMessage(message));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, fsChannelNameDataInput) <= 0) {
    LOG(error) << "Failed to send the request! message was " << message;
    return false;
  }  // if (Send(req, fsChannelNameDataInput) <= 0)
  else if (Receive(rep, fsChannelNameDataInput) < 0) {
    LOG(error) << "Failed to receive a reply to the request! message was " << message;
    return false;
  }  // else if (Receive(rep, fsChannelNameDataInput) < 0)
  else if (rep->GetSize() == 0) {
    LOG(error) << "Received empty reply. Something went wrong with the timeslice generation! message was " << message;
    return false;
  }  // else if (rep->GetSize() == 0)

  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with size " << rep->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  std::string msgStr(static_cast<char*>(rep->GetData()), rep->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  /// Create an empty TS and fill it with the incoming message
  fles::StorableTimeslice ts {0};
  inputArchive >> ts;

  /// On first TS, extract the TS parameters from header (by definition stable over time)
  if (-1.0 == fdTsCoreSizeInNs) {
    fuNbCoreMsPerTs  = ts.num_core_microslices();
    fuNbOverMsPerTs  = ts.num_microslices(0) - ts.num_core_microslices();
    fdMsSizeInNs     = (ts.descriptor(0, fuNbCoreMsPerTs).idx - ts.descriptor(0, 0).idx) / fuNbCoreMsPerTs;
    fdTsCoreSizeInNs = fdMsSizeInNs * (fuNbCoreMsPerTs);
    fdTsOverSizeInNs = fdMsSizeInNs * (fuNbOverMsPerTs);
    fdTsFullSizeInNs = fdTsCoreSizeInNs + fdTsOverSizeInNs;
    LOG(info) << "Timeslice parameters: each TS has " << fuNbCoreMsPerTs << " Core MS and " << fuNbOverMsPerTs
              << " Overlap MS, for a MS duration of " << fdMsSizeInNs << " ns, a core duration of " << fdTsCoreSizeInNs
              << " ns and a full duration of " << fdTsFullSizeInNs << " ns";
    fTsMetaData = new TimesliceMetaData(ts.descriptor(0, 0).idx, fdTsCoreSizeInNs, fdTsOverSizeInNs, ts.index());
  }  // if( -1.0 == fdTsCoreSizeInNs )
  else {
    /// Update only the fields changing from TS to TS
    fTsMetaData->SetStartTime(ts.descriptor(0, 0).idx);
    fTsMetaData->SetIndex(ts.index());
  }

  /// Process the Timeslice
  DoUnpack(ts, 0);

  // Reset the event header for a new timeslice
  fCbmTsEventHeader->Reset();

  // Reset the unpackers for a new timeslice, e.g. clear the output vectors
  // ---- Bmon ----
  if (fBmonConfig) fBmonConfig->Reset();

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

  return true;
}

bool CbmDeviceBmonMonitor::SendUnpData()
{
  FairMQParts parts;

  /// Prepare serialized versions of the TS Event header
  FairMQMessagePtr messTsHeader(NewMessage());
  //  Serialize<RootSerializer>(*messTsHeader, fCbmTsEventHeader);
  RootSerializer().Serialize(*messTsHeader, fCbmTsEventHeader);

  parts.AddPart(std::move(messTsHeader));

  // ---- T0 ----
  std::stringstream ossBmon;
  boost::archive::binary_oarchive oaBmon(ossBmon);
  if (fBmonConfig) {  //
    oaBmon << *(fBmonConfig->GetOutputVec());
  }
  else {
    oaBmon << (std::vector<CbmTofDigi>());
  }
  std::string* strMsgBmon = new std::string(ossBmon.str());

  parts.AddPart(NewMessage(
    const_cast<char*>(strMsgBmon->c_str()),  // data
    strMsgBmon->length(),                    // size
    [](void*, void* object) { delete static_cast<std::string*>(object); },
    strMsgBmon));  // object that manages the data

  /// Prepare serialized versions of the TS Meta
  /// FIXME: only for TS duration and overlap, should be sent to parameter service instead as stable values in run
  ///        Index and start time are already included in the TsHeader object!
  FairMQMessagePtr messTsMeta(NewMessage());
  //  Serialize<RootSerializer>(*messTsMeta, fTsMetaData);
  RootSerializer().Serialize(*messTsMeta, fTsMetaData);
  parts.AddPart(std::move(messTsMeta));

  if (Send(parts, fsChannelNameDataOutput) < 0) {
    LOG(error) << "Problem sending data to " << fsChannelNameDataOutput;
    return false;
  }

  return true;
}


bool CbmDeviceBmonMonitor::SendHistoConfAndData()
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
  // ALGO: fMonitorAlgo->ResetHistograms(kFALSE);
  fBmonConfig->GetMonitor()->ResetHistograms();
  fBmonConfig->GetMonitor()->ResetBmonHistograms(kFALSE);

  return true;
}

bool CbmDeviceBmonMonitor::SendHistograms()
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
  // ALGO: fMonitorAlgo->ResetHistograms(kFALSE);

  return true;
}


CbmDeviceBmonMonitor::~CbmDeviceBmonMonitor()
{
  if (fBmonConfig) fBmonConfig->GetUnpacker()->Finish();
}

Bool_t CbmDeviceBmonMonitor::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  fulTsCounter++;
  // Prepare timeslice
  //  const fles::Timeslice& timeslice = *ts;

  fCbmTsEventHeader->SetTsIndex(ts.index());
  fCbmTsEventHeader->SetTsStartTime(ts.start_time());

  uint64_t nComponents = ts.num_components();
  // if (fDoDebugPrints) LOG(info) << "Unpack: TS index " << ts.index() << " components " << nComponents;
  LOG(debug) << "Unpack: TS index " << ts.index() << " components " << nComponents;

  for (uint64_t component = 0; component < nComponents; component++) {
    auto systemId = static_cast<std::uint16_t>(ts.descriptor(component, 0).sys_id);

    switch (systemId) {
      case fkFlesBmon: {
        if (fBmonConfig) {
          fCbmTsEventHeader->AddNDigisBmon(
            unpack(systemId, &ts, component, fBmonConfig, fBmonConfig->GetOptOutAVec(), fBmonConfig->GetOptOutBVec()));
        }
        break;
      }
      default: {
        if (fDoDebugPrints) LOG(error) << "Unpack: Unknown system ID " << systemId << " for component " << component;
        break;
      }
    }
  }

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << " time slices";

  return kTRUE;
}
/**
 * @brief Get the Trd Spadic
 * @return std::shared_ptr<CbmTrdSpadic>
*/
std::shared_ptr<CbmTrdSpadic> CbmDeviceBmonMonitor::GetTrdSpadic(bool useAvgBaseline)
{
  auto spadic = std::make_shared<CbmTrdSpadic>();
  spadic->SetUseBaselineAverage(useAvgBaseline);
  spadic->SetMaxAdcToEnergyCal(1.0);

  return spadic;
}

void CbmDeviceBmonMonitor::Finish() {}
