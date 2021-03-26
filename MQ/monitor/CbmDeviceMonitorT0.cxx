/**
 * CbmDeviceMonitorT0.cxx
 *
 * @since 2019-03-26
 * @author F. Uhlig
 */

#include "CbmDeviceMonitorT0.h"

#include "CbmFlesCanvasTools.h"
#include "CbmMcbm2018MonitorAlgoT0.h"

#include "StorableTimeslice.hpp"

#include "BoostSerializer.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig
#include "FairParGenericSet.h"
#include "RootSerializer.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TNamed.h"

#include <array>
#include <iomanip>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/utility.hpp>

#include <stdexcept>
struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

using namespace std;


CbmDeviceMonitorT0::CbmDeviceMonitorT0()
  : fbIgnoreOverlapMs {false}
  , fsChannelNameDataInput {"t0component"}
  , fsChannelNameHistosInput {"histogram-in"}
  , fsChannelNameHistosConfig {"histo-conf"}
  , fsChannelNameCanvasConfig {"canvas-conf"}
  , fuHistoryHistoSize {3600}
  , fuMinTotPulser {185}
  , fuMaxTotPulser {195}
  , fuOffSpillCountLimit {25}
  , fuOffSpillCountLimitNonPulser {10}
  , fdSpillCheckInterval {0.0128}
  , fvuChanMap {0, 1, 2, 3, 4, 5, 6, 7}
  , fuPublishFreqTs {100}
  , fdMinPublishTime {0.5}
  , fdMaxPublishTime {5.0}
  , fsAllowedChannels {fsChannelNameDataInput}
  , fParCList {nullptr}
  , fulNumMessages {0}
  , fulTsCounter {0}
  , fLastPublishTime {std::chrono::system_clock::now()}
  , fMonitorAlgo {new CbmMcbm2018MonitorAlgoT0()}
  , fArrayHisto {}
  , fvpsHistosFolder {}
  , fvpsCanvasConfig {}
{
}

void CbmDeviceMonitorT0::InitTask() try {
  /// Read options from executable
  LOG(info) << "Init options for CbmMqStarHistoServer.";
  fbIgnoreOverlapMs         = fConfig->GetValue<bool>("IgnOverMs");
  fuHistoryHistoSize        = fConfig->GetValue<uint32_t>("HistEvoSz");
  fuMinTotPulser            = fConfig->GetValue<uint32_t>("PulsTotMin");
  fuMaxTotPulser            = fConfig->GetValue<uint32_t>("PulsTotMax");
  fuOffSpillCountLimit      = fConfig->GetValue<uint32_t>("SpillThr");
  fuOffSpillCountLimitNonPulser = fConfig->GetValue<uint32_t>("SpillThrNonPuls");
  fdSpillCheckInterval          = fConfig->GetValue<double>("SpillCheckInt");
  std::string sChanMap      = fConfig->GetValue<std::string>("ChanMap");
  fuPublishFreqTs           = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime          = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime          = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameDataInput    = fConfig->GetValue<std::string>("TsNameIn");
  fsChannelNameHistosInput  = fConfig->GetValue<std::string>("ChNameIn");
  fsChannelNameHistosConfig = fConfig->GetValue<std::string>("ChNameHistCfg");
  fsChannelNameCanvasConfig = fConfig->GetValue<std::string>("ChNameCanvCfg");
  fsAllowedChannels[0]      = fsChannelNameDataInput;

  UInt_t uChanIdx   = 0;
  size_t charPosDel = sChanMap.find(',');
  while (uChanIdx < fvuChanMap.size() && std::string::npos != charPosDel) {
    LOG(info) << sChanMap.substr(0, charPosDel);
    fvuChanMap[uChanIdx] = std::stoul(sChanMap.substr(0, charPosDel));
    LOG(info) << fvuChanMap[uChanIdx];
    sChanMap = sChanMap.substr(charPosDel + 1);
    LOG(info) << sChanMap;
    uChanIdx++;
    charPosDel = sChanMap.find(',');
  }  // while( uChanIdx < fvuChanMap.size() && std::string::npos != charPosDel )
  if (uChanIdx < fvuChanMap.size()) {
    LOG(info) << sChanMap;
    fvuChanMap[uChanIdx] = std::stoul(sChanMap);
    LOG(info) << fvuChanMap[uChanIdx];
  }  // if( uChanIdx < fvuChanMap.size() )

  LOG(info) << "Histograms publication frequency in TS:    " << fuPublishFreqTs;
  LOG(info) << "Histograms publication min. interval in s: "
            << fdMinPublishTime;
  LOG(info) << "Histograms publication max. interval in s: "
            << fdMaxPublishTime;

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
      if (!IsChannelNameAllowed(entry.first))
        throw InitTaskError("Channel name does not match.");
      OnData(entry.first, &CbmDeviceMonitorT0::HandleData);
    }  // if( entry.first.find( "ts" )
  }    // for( auto const &entry : fChannels )
  InitContainers();
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  ChangeState(fair::mq::Transition::ErrorFound);
}

bool CbmDeviceMonitorT0::IsChannelNameAllowed(std::string channelName) {
  for (auto const& entry : fsAllowedChannels) {
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      const vector<std::string>::const_iterator pos =
        std::find(fsAllowedChannels.begin(), fsAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx =
        pos - fsAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
                << " found in list of allowed channel names at position "
                << idx;
      return true;
    }  // if (pos1!=std::string::npos)
  }    // for(auto const &entry : fsAllowedChannels)
  LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceMonitorT0::InitContainers() {
  LOG(info) << "Init parameter containers for CbmDeviceMonitorT0.";

  fParCList = fMonitorAlgo->GetParList();

  for (int iparC = 0; iparC < fParCList->GetEntries(); iparC++) {
    FairParGenericSet* tempObj = (FairParGenericSet*) (fParCList->At(iparC));
    fParCList->Remove(tempObj);
    std::string paramName {tempObj->GetName()};
    // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
    // Should only be used for small data because of the cost of an additional copy

    // Her must come the proper Runid
    std::string message = paramName + ",111";
    LOG(info) << "Requesting parameter container " << paramName
              << ", sending message: " << message;

    FairMQMessagePtr req(NewSimpleMessage(message));
    FairMQMessagePtr rep(NewMessage());

    FairParGenericSet* newObj = nullptr;

    if (Send(req, "parameters") > 0) {
      if (Receive(rep, "parameters") >= 0) {
        if (rep->GetSize() != 0) {
          CbmMQTMessage tmsg(rep->GetData(), rep->GetSize());
          newObj =
            static_cast<FairParGenericSet*>(tmsg.ReadObject(tmsg.GetClass()));
          LOG(info) << "Received unpack parameter from the server:";
          newObj->print();
        } else {
          LOG(error) << "Received empty reply. Parameter not available";
        }  // if (rep->GetSize() != 0)
      }    // if (Receive(rep, "parameters") >= 0)
    }      // if (Send(req, "parameters") > 0)
    fParCList->AddAt(newObj, iparC);
    delete tempObj;
  }  // for ( int iparC = 0; iparC < fParCList->GetEntries(); iparC++ )

  /// Need to add accessors for all options
  fMonitorAlgo->SetIgnoreOverlapMs(fbIgnoreOverlapMs);
  fMonitorAlgo->SetMonitorMode(kTRUE);
  fMonitorAlgo->SetHistoryHistoSize(fuHistoryHistoSize);
  fMonitorAlgo->SetPulserTotLimits(fuMinTotPulser, fuMaxTotPulser);
  fMonitorAlgo->SetSpillThreshold(fuOffSpillCountLimit);
  fMonitorAlgo->SetSpillThresholdNonPulser(fuOffSpillCountLimitNonPulser);
  fMonitorAlgo->SetSpillCheckInterval(fdSpillCheckInterval);
  fMonitorAlgo->SetChannelMap(fvuChanMap[0], fvuChanMap[1], fvuChanMap[2], fvuChanMap[3], fvuChanMap[4], fvuChanMap[5],
                              fvuChanMap[6], fvuChanMap[7]);

  //   fMonitorAlgo->AddMsComponentToList(0, 0x90);

  Bool_t initOK = fMonitorAlgo->InitContainers();

  //   Bool_t initOK = fMonitorAlgo->ReInitContainers();

  /// Histos creation and obtain pointer on them
  /// Trigger histo creation on all associated algos
  initOK &= fMonitorAlgo->CreateHistograms();

  /// Obtain vector of pointers on each histo from the algo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos =
    fMonitorAlgo->GetHistoVector();
  /// Obtain vector of pointers on each canvas from the algo (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases =
    fMonitorAlgo->GetCanvasVector();

  /// Add pointers to each histo in the histo array
  /// Create histo config vector
  /// ===> Use an std::vector< std::pair< std::string, std::string > > with < Histo name, Folder >
  ///      and send it through a separate channel using the BoostSerializer
  for (UInt_t uHisto = 0; uHisto < vHistos.size(); ++uHisto) {
    //         LOG(info) << "Registering  " << vHistos[ uHisto ].first->GetName()
    //                   << " in " << vHistos[ uHisto ].second.data()
    //                   ;
    fArrayHisto.Add(vHistos[uHisto].first);
    std::pair<std::string, std::string> psHistoConfig(
      vHistos[uHisto].first->GetName(), vHistos[uHisto].second);
    fvpsHistosFolder.push_back(psHistoConfig);

    /// Serialize the vector of histo config into a single MQ message
    FairMQMessagePtr messageHist(NewMessage());
    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(
      *messageHist, psHistoConfig);

    /// Send message to the common histogram config messages queue
    if (Send(messageHist, fsChannelNameHistosConfig) < 0) {
      LOG(error) << "Problem sending histo config";
      return false;
    }  // if( Send( messageHist, fsChannelNameHistosConfig ) < 0 )

    LOG(info) << "Config of hist  " << psHistoConfig.first.data()
              << " in folder " << psHistoConfig.second.data();
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

    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(
      *messageCan, psCanvConfig);

    /// Send message to the common canvas config messages queue
    if (Send(messageCan, fsChannelNameCanvasConfig) < 0) {
      LOG(error) << "Problem sending canvas config";
      return false;
    }  // if( Send( messageCan, fsChannelNameCanvasConfig ) < 0 )

    LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data()
              << " is " << psCanvConfig.second.data();
  }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

  return initOK;
}


// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceMonitorT0::HandleData(FairMQMessagePtr& msg, int /*index*/) {
  fulNumMessages++;
  LOG(debug) << "Received message number " << fulNumMessages << " with size "
             << msg->GetSize();

  if (0 == fulNumMessages % 10000)
    LOG(info) << "Received " << fulNumMessages << " messages";

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
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
  std::chrono::system_clock::time_point currentTime =
    std::chrono::system_clock::now();
  std::chrono::duration<double_t> elapsedSeconds =
    currentTime - fLastPublishTime;
  if ((fdMaxPublishTime < elapsedSeconds.count())
      || (0 == fulNumMessages % fuPublishFreqTs
          && fdMinPublishTime < elapsedSeconds.count())) {
    SendHistograms();
    fLastPublishTime = std::chrono::system_clock::now();
  }  // if( ( fdMaxPublishTime < elapsedSeconds.count() ) || ( 0 == fulNumMessages % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )

  return true;
}

bool CbmDeviceMonitorT0::SendHistograms() {
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
  Serialize<RootSerializer>(*message, &fArrayHisto);

  // test code to check if deserialization works
  /*
  TObject* tempObject = nullptr;
  Deserialize<RootDeserializer>(*message, tempObject);

  if (TString(tempObject->ClassName()).EqualTo("TObjArray")) {
   TObjArray* arrayHisto = static_cast<TObjArray*>(tempObject);
   LOG(info) << "Array contains " << arrayHisto->GetEntriesFast()
             << " entries";
    for (Int_t i = 0; i < arrayHisto->GetEntriesFast(); i++) {
      TObject* obj = arrayHisto->At(i);
      LOG(info) << obj->GetName();
      TH1* histogram = static_cast<TH1*>(obj);
      LOG(info) << histogram->GetNbinsX();
    }
  }
*/

  /// Send message to the common histogram messages queue
  if (Send(message, fsChannelNameHistosInput) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }  // if( Send( message, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  fMonitorAlgo->ResetHistograms(kFALSE);

  return true;
}


CbmDeviceMonitorT0::~CbmDeviceMonitorT0() {}


Bool_t CbmDeviceMonitorT0::DoUnpack(const fles::Timeslice& ts,
                                    size_t /*component*/) {
  fulTsCounter++;

  if (kFALSE == fbComponentsAddedToList) {
    for (uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx) {
      if (kusSysId == ts.descriptor(uCompIdx, 0).sys_id) {
        fMonitorAlgo->AddMsComponentToList(uCompIdx, kusSysId);
      }  // if( kusSysId == ts.descriptor( uCompIdx, 0 ).sys_id )
    }    // for( uint32_t uComp = 0; uComp < ts.num_components(); ++uComp )
    fbComponentsAddedToList = kTRUE;
  }  // if( kFALSE == fbComponentsAddedToList )

  if (kFALSE == fMonitorAlgo->ProcessTs(ts)) {
    LOG(error) << "Failed processing TS " << ts.index()
               << " in unpacker algorithm class";
    return kTRUE;
  }  // if( kFALSE == fMonitorAlgo->ProcessTs( ts ) )

  /// Clear the digis vector in case it was filled
  fMonitorAlgo->ClearVector();

  if (0 == fulTsCounter % 10000)
    LOG(info) << "Processed " << fulTsCounter << " time slices";

  return kTRUE;
}

void CbmDeviceMonitorT0::Finish() {}
