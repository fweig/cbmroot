/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmTsConsumerReqDevExample.h"

#include "CbmFlesCanvasTools.h"

#include "StorableTimeslice.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"
#include <thread>

#include "BoostSerializer.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

#include <array>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "RootSerializer.h"
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;


CbmTsConsumerReqDevExample::CbmTsConsumerReqDevExample()
// ALGO: : fMonitorAlgo {new CbmMcbm2018MonitorAlgoT0()}
{
}

void CbmTsConsumerReqDevExample::InitTask()
try {
  /// Read options from executable
  LOG(info) << "Init options for CbmMqStarHistoServer.";
  fbIgnoreOverlapMs        = fConfig->GetValue<bool>("IgnOverMs");
  fuPublishFreqTs          = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime         = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime         = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameDataInput   = fConfig->GetValue<std::string>("TsNameIn");
  fsTsBlockName            = fConfig->GetValue<std::string>("TsBlockName");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");

  LOG(info) << "Histograms publication frequency in TS:    " << fuPublishFreqTs;
  LOG(info) << "Histograms publication min. interval in s: " << fdMinPublishTime;
  LOG(info) << "Histograms publication max. interval in s: " << fdMaxPublishTime;
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  ChangeState(fair::mq::Transition::ErrorFound);
}

bool CbmTsConsumerReqDevExample::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmTsConsumerReqDevExample.";

  // ALGO: fParCList = fMonitorAlgo->GetParList();
  fParCList = new TList();

  for (int iparC = 0; iparC < fParCList->GetEntries(); iparC++) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);
    std::string paramName {tempObj->GetName()};
    // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
    // Should only be used for small data because of the cost of an additional copy

    // Her must come the proper Runid
    std::string message = paramName + ",111";
    LOG(info) << "Requesting parameter container " << paramName << ", sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    FairParGenericSet* newObj = nullptr;

    if (Send(req, "parameters") > 0) {
      if (Receive(rep, "parameters") >= 0) {
        if (rep->GetSize() != 0) {
          CbmMqTMessage tmsg(rep->GetData(), rep->GetSize());
          newObj = static_cast<FairParGenericSet*>(tmsg.ReadObject(tmsg.GetClass()));
          LOG(info) << "Received unpack parameter from the server:";
          newObj->print();
        }
        else {
          LOG(error) << "Received empty reply. Parameter not available";
        }  // if (rep->GetSize() != 0)
      }    // if (Receive(rep, "parameters") >= 0)
    }      // if (Send(req, "parameters") > 0)
    fParCList->AddAt(newObj, iparC);
    delete tempObj;
  }  // for ( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )

  /// Apply options to the processing algo
  // ALGO: fMonitorAlgo->SetIgnoreOverlapMs(fbIgnoreOverlapMs);

  //   fMonitorAlgo->AddMsComponentToList(0, 0x90);

  // ALGO: Bool_t initOK = fMonitorAlgo->InitContainers();
  bool initOK = true;

  return initOK;
}

bool CbmTsConsumerReqDevExample::InitHistograms()
{
  /// Histos creation and obtain pointer on them
  /// Trigger histo creation on all associated algos
  // ALGO: bool initOK = fMonitorAlgo->CreateHistograms();
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


bool CbmTsConsumerReqDevExample::ConditionalRun()
{
  /// First request a new TS (full or single system components or multi-syst components block)
  std::string message = fsTsBlockName;
  if ("" == message) message = std::to_string(kusSysId);
  LOG(debug) << "Requesting new TS by sending message: " << message;
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

  /// Message received, do Algo related Initialization steps if needed
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

  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with size " << rep->GetSize();

  if (0 == fulNumMessages % 10000) LOG(info) << "Received " << fulNumMessages << " messages";

  std::string msgStr(static_cast<char*>(rep->GetData()), rep->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  /// Create an empty TS and fill it with the incoming message
  fles::StorableTimeslice component {0};
  inputArchive >> component;

  /// Process the Timeslice
  DoUnpack(component, 0);

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

bool CbmTsConsumerReqDevExample::SendHistoConfAndData()
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

  for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv) {
    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    //    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageCan, fvpsCanvasConfig[uCanv]);
    BoostSerializer<std::pair<std::string, std::string>>().Serialize(*messageCan, fvpsCanvasConfig[uCanv]);

    partsOut.AddPart(std::move(messageCan));
  }  // for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv)

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

  return true;
}

bool CbmTsConsumerReqDevExample::SendHistograms()
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


CbmTsConsumerReqDevExample::~CbmTsConsumerReqDevExample() {}


Bool_t CbmTsConsumerReqDevExample::DoUnpack(const fles::Timeslice& ts, size_t /*component*/)
{
  fulTsCounter++;

  if (kFALSE == fbComponentsAddedToList) {
    for (uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx) {
      if (kusSysId == ts.descriptor(uCompIdx, 0).sys_id) {
        /// Do something here
        // ALGO:
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }  // if( kusSysId == ts.descriptor( uCompIdx, 0 ).sys_id )
    }    // for( uint32_t uComp = 0; uComp < ts.num_components(); ++uComp )
    fbComponentsAddedToList = kTRUE;
  }  // if( kFALSE == fbComponentsAddedToList )

  // ALGO:
  /*
  if (kFALSE == fMonitorAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index() << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )
  */

  /// Clear the digis vector in case it was filled
  // ALGO: fMonitorAlgo->ClearVector();

  if (0 == fulTsCounter % 10000) LOG(info) << "Processed " << fulTsCounter << " time slices";

  return kTRUE;
}

void CbmTsConsumerReqDevExample::Finish() {}
