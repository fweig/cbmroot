/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Florian Uhlig [committer] */

/**
 *  CbmMQTsaMultiSampler.cpp
 *
 * @since 2017-11-17
 * @author F. Uhlig
 */


#include "CbmMQTsaMultiSampler.h"

#include "CbmFlesCanvasTools.h"
#include "CbmFormatDecHexPrintout.h"

#include "TimesliceInputArchive.hpp"
#include "TimesliceMultiInputArchive.hpp"
#include "TimesliceMultiSubscriber.hpp"
#include "TimesliceSubscriber.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"  // device->fConfig

#include <TCanvas.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TProfile.h>

#include "BoostSerializer.h"
#include <boost/algorithm/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/serialization/utility.hpp>

#include "RootSerializer.h"

namespace filesys = boost::filesystem;

#include <thread>  // this_thread::sleep_for

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <stdio.h>

using namespace std;

#include <stdexcept>

struct InitTaskError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

CbmMQTsaMultiSampler::CbmMQTsaMultiSampler()
  : FairMQDevice()
  , fMaxTimeslices(0)
  , fFileName("")
  , fDirName("")
  , fInputFileList()
  , fFileCounter(0)
  , fHost("")
  , fPort(0)
  , fHighWaterMark(1)
  , fTSCounter(0)
  , fMessageCounter(0)
  , fSource(nullptr)
  , fTime()
  , fLastPublishTime {std::chrono::system_clock::now()}
{
}

void CbmMQTsaMultiSampler::InitTask()
try {
  // Get the values from the command line options (via fConfig)
  fFileName                 = fConfig->GetValue<string>("filename");
  fDirName                  = fConfig->GetValue<string>("dirname");
  fHost                     = fConfig->GetValue<string>("flib-host");
  fPort                     = fConfig->GetValue<uint64_t>("flib-port");
  fHighWaterMark            = fConfig->GetValue<uint64_t>("high-water-mark");
  fMaxTimeslices            = fConfig->GetValue<uint64_t>("max-timeslices");
  fbNoSplitTs               = fConfig->GetValue<bool>("no-split-ts");
  fbSendTsPerSysId          = fConfig->GetValue<bool>("send-ts-per-sysid");
  fbSendTsPerChannel        = fConfig->GetValue<bool>("send-ts-per-channel");
  fsChannelNameMissedTs     = fConfig->GetValue<std::string>("ChNameMissTs");
  fsChannelNameCommands     = fConfig->GetValue<std::string>("ChNameCmds");
  fuPublishFreqTs           = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime          = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime          = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameHistosInput  = fConfig->GetValue<std::string>("ChNameIn");
  fsChannelNameHistosConfig = fConfig->GetValue<std::string>("ChNameHistCfg");
  fsChannelNameCanvasConfig = fConfig->GetValue<std::string>("ChNameCanvCfg");

  if (fbNoSplitTs) {
    if (fbSendTsPerSysId) {
      if (fbSendTsPerChannel) {
        LOG(warning) << "Both no-split-ts, send-ts-per-sysid and "
                        "send-ts-per-channel options used => "
                     << " second and third one will be ignored!!!!";
      }  // if( fbSendTsPerSysId )
      else
        LOG(warning) << "Both no-split-ts and send-ts-per-sysid options used => "
                     << " second one will be ignored!!!!";
    }  // if( fbSendTsPerSysId )
    else if (fbSendTsPerChannel) {
      LOG(warning) << "Both no-split-ts and send-ts-per-channel options used => "
                   << " second one will be ignored!!!!";
    }  // else if( fbSendTsPerSysId ) of if( fbSendTsPerSysId )
  }    // if( fbNoSplitTs )
  else if (fbSendTsPerSysId && fbSendTsPerChannel) {
    LOG(warning) << "Both send-ts-per-sysid and send-ts-per-channel options used => "
                 << " second one will be ignored!!!!";
  }  // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs )

  /// Extract SysId and channel information if provided in the binary options
  std::vector<std::string> vSysIdChanPairs = fConfig->GetValue<std::vector<std::string>>("sysid-chan");
  for (uint32_t uPair = 0; uPair < vSysIdChanPairs.size(); ++uPair) {
    const size_t sep = vSysIdChanPairs[uPair].find(':');
    if (string::npos == sep || 0 == sep || vSysIdChanPairs[uPair].size() == sep) {
      LOG(info) << vSysIdChanPairs[uPair];
      throw InitTaskError("Provided pair of SysId + Channel name is missing a : or an argument.");
    }  // if( string::npos == sep || 0 == sep || vSysIdChanPairs[ uPair ].size() == sep )

    /// Extract SysId
    std::string sSysId  = vSysIdChanPairs[uPair].substr(0, sep);
    const size_t hexPos = sSysId.find("0x");
    int iSysId;
    if (string::npos == hexPos) iSysId = std::stoi(sSysId);
    else
      iSysId = std::stoi(sSysId.substr(hexPos + 2), nullptr, 16);

    /// Extract Channel name
    std::string sChannelName = vSysIdChanPairs[uPair].substr(sep + 1);

    /// Look if SysId is already defined
    const vector<int>::const_iterator pos = std::find(fSysId.begin(), fSysId.end(), iSysId);
    if (fSysId.end() != pos) {
      /// SysId already there, redefine the corresponding channel name
      const vector<std::string>::size_type idx = pos - fSysId.begin();
      fAllowedChannels[idx]                    = sChannelName;
    }  // if( fSysId.end() != pos )
    else {
      /// SysId unknown yet, add both SysId and channe name at end of respective vectors
      fSysId.push_back(iSysId);
      fAllowedChannels.push_back(sChannelName);
    }  // else of if( fSysId.end() != pos )

    LOG(info) << vSysIdChanPairs[uPair] << " " << iSysId << " " << sChannelName;
  }  // for( uint32_t uPair = 0; uPair < vSysIdChanPairs.size(); ++uPair )

  if (0 == fMaxTimeslices) fMaxTimeslices = UINT_MAX;

  // Check which input is defined
  // Posibilities
  // filename && ! dirname : single file
  // filename with wildcards && diranme : all files with filename regex in the directory
  // host && port : connect to the flim server

  bool isGoodInputCombi {false};
  if (0 != fFileName.size() && 0 == fDirName.size() && 0 == fHost.size() && 0 == fPort) {
    isGoodInputCombi = true;
    fInputFileList.push_back(fFileName);
  }
  else if (0 != fFileName.size() && 0 != fDirName.size() && 0 == fHost.size() && 0 == fPort) {
    isGoodInputCombi = true;
    fInputFileList.push_back(fFileName);
  }
  else if (0 == fFileName.size() && 0 == fDirName.size() && 0 != fHost.size() && 0 != fPort) {
    isGoodInputCombi = true;
    LOG(info) << "Host: " << fHost;
    LOG(info) << "Port: " << fPort;
  }
  else if (0 == fFileName.size() && 0 == fDirName.size() && 0 != fHost.size() && 0 == fPort) {
    isGoodInputCombi = true;
    LOG(info) << "Host string: " << fHost;
  }
  else {
    isGoodInputCombi = false;
  }


  if (!isGoodInputCombi) {
    throw InitTaskError("Wrong combination of inputs. Either file or wildcard file + directory "
                        "or host + port are allowed combination.");
  }


  LOG(info) << "MaxTimeslices: " << fMaxTimeslices;

  // Get the information about created channels from the device
  // Check if the defined channels from the topology (by name)
  // are in the list of channels which are possible/allowed
  // for the device
  // The idea is to check at initilization if the devices are
  // properly connected. For the time beeing this is done with a
  // nameing convention. It is not avoided that someone sends other
  // data on this channel.
  int noChannel = fChannels.size();
  LOG(info) << "Number of defined output channels: " << noChannel;
  for (auto const& entry : fChannels) {
    /// Catches and ignores the channels for missing TS indices and commands
    /// Same for the histogram channels
    if (entry.first == fsChannelNameMissedTs || entry.first == fsChannelNameCommands
        || (0 < fuPublishFreqTs
            && (entry.first == fsChannelNameHistosInput || entry.first == fsChannelNameHistosConfig
                || entry.first == fsChannelNameCanvasConfig))) {
      continue;
    }  // if( entry.first == fsChannelNameMissedTs || entry.first == fsChannelNameCommands || histo channels name)

    LOG(info) << "Channel name: " << entry.first;
    if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
  }

  for (auto const& value : fComponentsToSend) {
    LOG(info) << "Value : " << value;
    if (value > 1) {
      throw InitTaskError("Sending same data to more than one output channel "
                          "not implemented yet.");
    }
  }


  if (0 == fFileName.size() && 0 != fHost.size() && 0 != fPort) {
    // Don't add the protocol since this is done now in the TimesliceMultiSubscriber
    //std::string connector = "tcp://" + fHost + ":" + std::to_string(fPort);
    std::string connector = fHost + ":" + std::to_string(fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fSource = new fles::TimesliceMultiSubscriber(connector);
  }
  else if (0 == fFileName.size() && 0 != fHost.size()) {
    std::string connector = fHost;
    LOG(info) << "Open TSPublisher with host string: " << connector;
    fSource = new fles::TimesliceMultiSubscriber(connector, fHighWaterMark);
  }
  else {
    // Create a ";" separated string with all file names
    std::string fileList {""};
    for (const auto& obj : fInputFileList) {
      std::string fileName = obj;
      fileList += fileName;
      fileList += ";";
    }
    fileList.pop_back();  // Remove the last ;
    LOG(info) << "Input File String: " << fileList;
    fSource = new fles::TimesliceMultiInputArchive(fileList, fDirName);
    if (!fSource) { throw InitTaskError("Could open files from file list."); }
  }

  LOG(info) << "High-Water Mark: " << fHighWaterMark;
  LOG(info) << "Max. Timeslices: " << fMaxTimeslices;
  if (fbNoSplitTs) { LOG(info) << "Sending TS copies in no-split mode"; }  // if( fbNoSplitTs )
  else if (fbSendTsPerSysId) {
    LOG(info) << "Sending components in separate TS per SysId";
  }  // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs
  else if (fbSendTsPerChannel) {
    LOG(info) << "Sending components in separate TS per channel";
  }  // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs )

  fTime = std::chrono::steady_clock::now();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  ChangeState(fair::mq::Transition::ErrorFound);
}

bool CbmMQTsaMultiSampler::IsChannelNameAllowed(std::string channelName)
{
  /// If sending full TS, accept any name!
  if (fbNoSplitTs) {
    fComponentsToSend[0]++;
    fChannelsToSend[0].push_back(channelName);
    return true;
  }  // if( fbNoSplitTs )

  bool bFoundMatch = false;
  //  for(auto const &entry : fAllowedChannels) {
  for (uint32_t idx = 0; idx < fAllowedChannels.size(); ++idx) {
    auto const& entry = fAllowedChannels[idx];
    LOG(info) << "Looking for name " << channelName << " in " << entry;
    std::size_t pos1 = channelName.find(entry);
    if (pos1 != std::string::npos) {
      /*
      const vector<std::string>::const_iterator pos =
         std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos-fAllowedChannels.begin();
*/
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName << " found in list of allowed channel names at position " << idx
                << " (SysId 0x" << std::hex << fSysId[idx] << std::dec << ")";
      fComponentsToSend[idx]++;
      fChannelsToSend[idx].push_back(channelName);

      /// If sending per channel, do not stop the loop as we allow more than 1 comp type per channel
      if (fbSendTsPerChannel) bFoundMatch = true;
      else
        return true;
    }  // if (pos1!=std::string::npos)
  }
  /// If sending per channel, do not stop the loop but still check if at least 1 match found
  if (fbSendTsPerChannel && bFoundMatch) return true;

  LOG(info) << "Channel name " << channelName << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

bool CbmMQTsaMultiSampler::InitHistograms()
{
  LOG(info) << "Histograms publication frequency in TS:    " << fuPublishFreqTs;
  LOG(info) << "Histograms publication min. interval in s: " << fdMinPublishTime;
  LOG(info) << "Histograms publication max. interval in s: " << fdMaxPublishTime;

  /// Vector of pointers on each histo (+ optionally desired folder)
  std::vector<std::pair<TNamed*, std::string>> vHistos = {};
  /// Vector of pointers on each canvas (+ optionally desired folder)
  std::vector<std::pair<TCanvas*, std::string>> vCanvases = {};

  /// Histos creation and obtain pointer on them
  fhTsRate       = new TH1I("TsRate", "TS rate; t [s]", 1800, 0., 1800.);
  fhTsSize       = new TH1I("TsSize", "Size of TS; Size [MB]", 15000, 0., 15000.);
  fhTsSizeEvo    = new TProfile("TsSizeEvo", "Evolution of the TS Size; t [s]; Mean size [MB]", 1800, 0., 1800.);
  fhTsMaxSizeEvo = new TH1F("TsMaxSizeEvo", "Evolution of maximal TS Size; t [s]; Max size [MB]", 1800, 0., 1800.);
  fhMissedTS     = new TH1I("Missed_TS", "Missed TS", 2, -0.5, 1.5);
  fhMissedTSEvo  = new TProfile("Missed_TS_Evo", "Missed TS evolution; t [s]", 1800, 0., 1800.);

  /// Add histo pointers to the histo vector
  vHistos.push_back(std::pair<TNamed*, std::string>(fhTsRate, "Sampler"));
  vHistos.push_back(std::pair<TNamed*, std::string>(fhTsSize, "Sampler"));
  vHistos.push_back(std::pair<TNamed*, std::string>(fhTsSizeEvo, "Sampler"));
  vHistos.push_back(std::pair<TNamed*, std::string>(fhTsMaxSizeEvo, "Sampler"));
  vHistos.push_back(std::pair<TNamed*, std::string>(fhMissedTS, "Sampler"));
  vHistos.push_back(std::pair<TNamed*, std::string>(fhMissedTSEvo, "Sampler"));

  /// Canvases creation
  Double_t w = 10;
  Double_t h = 10;
  fcSummary  = new TCanvas("cSampSummary", "Sampler monitoring plots", w, h);
  fcSummary->Divide(2, 3);

  fcSummary->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTsRate->Draw("hist");

  fcSummary->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  gPad->SetLogy();
  fhTsSize->Draw("hist");

  fcSummary->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTsSizeEvo->Draw("hist");

  fcSummary->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  fhTsMaxSizeEvo->Draw("hist");

  fcSummary->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  fhMissedTS->Draw("hist");

  fcSummary->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  fhMissedTSEvo->Draw("el");

  /// Add canvas pointers to the canvas vector
  vCanvases.push_back(std::pair<TCanvas*, std::string>(fcSummary, "canvases"));

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
    if (Send(messageHist, fsChannelNameHistosConfig) < 0) {
      LOG(fatal) << "Problem sending histo config";
    }  // if( Send( messageHist, fsChannelNameHistosConfig ) < 0 )

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

    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageCan, psCanvConfig);

    /// Send message to the common canvas config messages queue
    if (Send(messageCan, fsChannelNameCanvasConfig) < 0) {
      LOG(fatal) << "Problem sending canvas config";
    }  // if( Send( messageCan, fsChannelNameCanvasConfig ) < 0 )

    LOG(info) << "Config string of Canvas  " << psCanvConfig.first.data() << " is " << psCanvConfig.second.data();
  }  //  for( UInt_t uCanv = 0; uCanv < vCanvases.size(); ++uCanv )

  return true;
}

bool CbmMQTsaMultiSampler::ConditionalRun()
{
  if (0 < fuPublishFreqTs && 0 == fTSCounter) { InitHistograms(); }  // if( 0 < fuPublishFreqTs )

  /// initialize the source (connect to emitter, ...)
  if (0 == fTSCounter && nullptr != dynamic_cast<fles::TimesliceMultiSubscriber*>(fSource)) {
    dynamic_cast<fles::TimesliceMultiSubscriber*>(fSource)->InitTimesliceSubscriber();
  }  // if( 0 == fTSCounter && nullptr != dynamic_cast< fles::TimesliceMultiSubscriber >(fSource) )

  auto timeslice = fSource->get();
  if (timeslice) {
    if (fTSCounter < fMaxTimeslices) {
      fTSCounter++;

      const fles::Timeslice& ts = *timeslice;
      uint64_t uTsIndex         = ts.index();

      if (0 < fuPublishFreqTs) {
        uint64_t uTsTime = ts.descriptor(0, 0).idx;
        if (0 == fuStartTime) { fuStartTime = uTsTime; }  // if( 0 == fuStartTime )
        fdTimeToStart    = static_cast<double_t>(uTsTime - fuStartTime) / 1e9;
        uint64_t uSizeMb = 0;

        for (uint64_t uComp = 0; uComp < ts.num_components(); ++uComp) {
          uSizeMb += ts.size_component(uComp) / (1024 * 1024);
        }  // for( uint_t uComp = 0; uComp < ts.num_components(); ++uComp )


        fhTsRate->Fill(fdTimeToStart);
        fhTsSize->Fill(uSizeMb);
        fhTsSizeEvo->Fill(fdTimeToStart, uSizeMb);

        /// Fill max size per s (assumes the histo binning is 1 second!)
        if (0. == fdLastMaxTime) {
          fdLastMaxTime = fdTimeToStart;
          fdTsMaxSize   = uSizeMb;
        }  // if( 0. == fdLastMaxTime )
        else if (1. <= fdTimeToStart - fdLastMaxTime) {
          fhTsMaxSizeEvo->Fill(fdLastMaxTime, fdTsMaxSize);
          fdLastMaxTime = fdTimeToStart;
          fdTsMaxSize   = uSizeMb;
        }  // else if if( 1 <= fdTimeToStart - fdLastMaxTime )
        else if (fdTsMaxSize < uSizeMb) {
          fdTsMaxSize = uSizeMb;
        }  // else if( fdTsMaxSize < uSizeMb )
      }    // if( 0 < fuPublishFreqTs )

      /// Missed TS detection (only if output channel name defined by user)
      if ((uTsIndex != (fuPrevTsIndex + 1)) && (0 != fuPrevTsIndex && 0 != uTsIndex)) {
        LOG(info) << "Missed Timeslices. Old TS Index was " << fuPrevTsIndex << " New TS Index is " << uTsIndex
                  << " diff is " << uTsIndex - fuPrevTsIndex << " Missing are " << uTsIndex - fuPrevTsIndex - 1;

        if ("" != fsChannelNameMissedTs) {
          /// Add missing TS indices to a vector and send it in appropriate channel
          std::vector<uint64_t> vulMissedIndices;
          for (uint64_t ulMiss = fuPrevTsIndex + 1; ulMiss < uTsIndex; ++ulMiss) {
            vulMissedIndices.emplace_back(ulMiss);
          }  // for( uint64_t ulMiss = fuPrevTsIndex + 1; ulMiss < uTsIndex; ++ulMiss )
          if (!SendMissedTsIdx(vulMissedIndices)) {
            /// If command channel defined, send command to all "slaves"
            if ("" != fsChannelNameCommands) {
              /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
              std::this_thread::sleep_for(std::chrono::milliseconds(1000));
              SendCommand("STOP");
            }  // if( "" != fsChannelNameCommands )

            return false;
          }  // if( !SendMissedTsIdx( vulMissedIndices ) )
        }    // if( "" != fsChannelNameMissedTs )

        if (0 < fuPublishFreqTs) {
          fhMissedTS->Fill(1, uTsIndex - fuPrevTsIndex - 1);
          fhMissedTSEvo->Fill(fdTimeToStart, 1, uTsIndex - fuPrevTsIndex - 1);
        }  // if( 0 < fuPublishFreqTs )

      }  // if( ( uTsIndex != ( fuPrevTsIndex + 1 ) ) && ( 0 != fuPrevTsIndex && 0 != uTsIndex ) )

      if (0 < fuPublishFreqTs) {
        fhMissedTS->Fill(0);
        fhMissedTSEvo->Fill(fdTimeToStart, 0, 1);
      }  // else if( 0 < fuPublishFreqTs )

      fuPrevTsIndex = uTsIndex;

      if (fTSCounter % 10000 == 0) { LOG(info) << "Received TS " << fTSCounter << " with index " << uTsIndex; }

      LOG(debug) << "Found " << ts.num_components() << " different components in timeslice";


      //      CheckTimeslice(ts);

      if (fbNoSplitTs) {
        /// This is a special case for the TOF + T0
        /// => Inefficient as copy the TS as many times as need!
        if (!CreateAndSendFullTs(ts)) {
          /// If command channel defined, send command to all "slaves"
          if ("" != fsChannelNameCommands) {
            /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            SendCommand("STOP");
          }  // if( "" != fsChannelNameCommands )

          return false;
        }  // if( !CreateAndSendFullTs( ts ) )
      }    // if( fbNoSplitTs )
      else if (fbSendTsPerSysId) {
        /// This assumes that the order of the components does NOT change after the first TS
        /// That should be the case as the component index correspond to a physical link idx
        if (!CreateAndCombineComponentsPerSysId(ts)) {
          /// If command channel defined, send command to all "slaves"
          if ("" != fsChannelNameCommands) {
            /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            SendCommand("STOP");
          }  // if( "" != fsChannelNameCommands )

          return false;
        }  // if( !CreateAndCombineComponentsPerSysId( ts ) )
      }    // else if( fbSendTsPerSysId ) of  if( fbNoSplitTs )
      else if (fbSendTsPerChannel) {
        /// This assumes that the order of the components does NOT change after the first TS
        /// That should be the case as the component index correspond to a physical link idx
        if (!CreateAndCombineComponentsPerChannel(ts)) {
          /// If command channel defined, send command to all "slaves"
          if ("" != fsChannelNameCommands) {
            /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            SendCommand("STOP");
          }  // if( "" != fsChannelNameCommands )

          return false;
        }  // if( !CreateAndCombineComponentsPerChannel( ts ) )
      }    // else if( fbSendTsPerChannel ) of  if( fbSendTsPerSysId )
      else {
        for (unsigned int nrComp = 0; nrComp < ts.num_components(); ++nrComp) {
          if (!CreateAndSendComponent(ts, nrComp)) {
            /// If command channel defined, send command to all "slaves"
            if ("" != fsChannelNameCommands) {
              /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
              std::this_thread::sleep_for(std::chrono::milliseconds(1000));
              SendCommand("STOP");
            }  // if( "" != fsChannelNameCommands )

            return false;
          }  // if( !CreateAndSendComponent(ts, nrComp) )
        }    // for (unsigned int nrComp = 0; nrComp < ts.num_components(); ++nrComp)
      }      // else of if( fbSendTsPerSysId )

      if (0 < fuPublishFreqTs) {
        /// Send histograms periodically.
        /// Use also runtime checker to trigger sending after M s if
        /// processing too slow or delay sending if processing too fast
        std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double_t> elapsedSeconds    = currentTime - fLastPublishTime;
        if ((fdMaxPublishTime < elapsedSeconds.count())
            || (0 == fTSCounter % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count())) {
          SendHistograms();
          fLastPublishTime = std::chrono::system_clock::now();
        }  // if( ( fdMaxPublishTime < elapsedSeconds.count() ) || ( 0 == fTSCounter % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )
      }    // if( 0 < fuPublishFreqTs )

      return true;
    }  // if (fTSCounter < fMaxTimeslices)
    else {
      CalcRuntime();

      /// If command channel defined, send command to all "slaves"
      if ("" != fsChannelNameCommands) {
        /// Wait 1 s before sending an EOF to let all slaves finish processing previous data
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::string sCmd = "EOF ";
        sCmd += FormatDecPrintout(fuPrevTsIndex);
        sCmd += " ";
        sCmd += FormatDecPrintout(fTSCounter);
        SendCommand(sCmd);
      }  // if( "" != fsChannelNameCommands )

      return false;
    }  // else of if (fTSCounter < fMaxTimeslices)
  }    // if (timeslice)
  else {
    CalcRuntime();

    /// If command channel defined, send command to all "slaves"
    if ("" != fsChannelNameCommands) {
      /// Wait 1 s before sending an EOF to let all slaves finish processing previous data
      std::this_thread::sleep_for(std::chrono::seconds(10));
      std::string sCmd = "EOF ";
      sCmd += FormatDecPrintout(fuPrevTsIndex);
      sCmd += " ";
      sCmd += FormatDecPrintout(fTSCounter);
      SendCommand(sCmd);
    }  // if( "" != fsChannelNameCommands )

    return false;
  }  // else of if (timeslice)
}

bool CbmMQTsaMultiSampler::CreateAndSendComponent(const fles::Timeslice& ts, int nrComp)
{

  // Check if component has to be send. If the corresponding channel
  // is connected create the new timeslice and send it to the
  // correct channel

  LOG(debug) << "SysID: " << static_cast<int>(ts.descriptor(nrComp, 0).sys_id);
  const vector<int>::const_iterator pos =
    std::find(fSysId.begin(), fSysId.end(), static_cast<int>(ts.descriptor(nrComp, 0).sys_id));
  if (pos != fSysId.end()) {
    const vector<std::string>::size_type idx = pos - fSysId.begin();
    if (fComponentsToSend[idx] > 0) {
      LOG(debug) << "Create timeslice component for link " << nrComp;

      fles::StorableTimeslice component {static_cast<uint32_t>(ts.num_core_microslices()), ts.index()};
      component.append_component(ts.num_microslices(0));

      for (size_t m = 0; m < ts.num_microslices(nrComp); ++m) {
        component.append_microslice(0, m, ts.descriptor(nrComp, m), ts.content(nrComp, m));
      }
      /*
      LOG(info) << "Number of core microslices before: " << ts.num_core_microslices();
      LOG(info) << "Number of core microslices after : " << component.num_core_microslices();
      LOG(info) << "Number of microslices: " << component.num_microslices(0);
*/
      if (!SendData(component, idx)) return false;
      return true;
    }
  }
  return true;
}

bool CbmMQTsaMultiSampler::CreateAndCombineComponentsPerSysId(const fles::Timeslice& ts)
{
  /// First build the list of components for each SysId if it was not already done
  if (false == fbListCompPerSysIdReady) {
    for (uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx) {
      uint16_t usSysId = ts.descriptor(uCompIdx, 0).sys_id;

      const vector<int>::const_iterator pos = std::find(fSysId.begin(), fSysId.end(), usSysId);
      if (fSysId.end() != pos) {
        const vector<std::string>::size_type idx = pos - fSysId.begin();

        fvvCompPerSysId[idx].push_back(uCompIdx);
      }  // if( fSysId.end() != pos )
    }    // for( uint32_t uNrComp = 0; uNrComp < ts.num_components(); ++uNrComp )

    for (uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx) {
      std::stringstream ss;
      ss << "Found " << std::setw(2) << fvvCompPerSysId[uSysIdx].size() << " components for SysId 0x" << std::hex
         << std::setw(2) << fSysId[uSysIdx] << std::dec << " :";

      for (uint32_t uComp = 0; uComp < fvvCompPerSysId[uSysIdx].size(); ++uComp) {
        ss << " " << std::setw(3) << fvvCompPerSysId[uSysIdx][uComp];
      }  // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uSysIdx ].size(); ++uComp )

      LOG(info) << ss.str();
    }  // for( uint32_t uSysId = 0; uSysId < fSysId.size(); ++uSysId )

    fbListCompPerSysIdReady = true;
  }  // if( false == fbListCompPerSysIdReady )

  /// Then loop on all possible SysId and send TS with their respective components if needed
  for (uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx) {
    if (0 < fComponentsToSend[uSysIdx]) {
      LOG(debug) << "Create timeslice with components for SysId " << std::hex << fSysId[uSysIdx] << std::dec;

      if (0 < fvvCompPerSysId[uSysIdx].size()) {
        fles::StorableTimeslice component {static_cast<uint32_t>(ts.num_core_microslices()), ts.index()};

        for (uint32_t uComp = 0; uComp < fvvCompPerSysId[uSysIdx].size(); ++uComp) {
          uint32_t uNumMsInComp = ts.num_microslices(fvvCompPerSysId[uSysIdx][uComp]);
          component.append_component(uNumMsInComp);

          LOG(debug) << "Add components to TS for SysId " << std::hex << fSysId[uSysIdx] << std::dec << " TS "
                     << ts.index() << " Comp " << fvvCompPerSysId[uSysIdx][uComp];

          for (size_t m = 0; m < uNumMsInComp; ++m) {
            component.append_microslice(uComp, m, ts.descriptor(fvvCompPerSysId[uSysIdx][uComp], m),
                                        ts.content(fvvCompPerSysId[uSysIdx][uComp], m));
          }  // for( size_t m = 0; m < uNumMsInComp; ++m )
        }    // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uSysIdx ].size(); ++uComp )

        LOG(debug) << "Prepared timeslice for SysId " << std::hex << fSysId[uSysIdx] << std::dec << " with "
                   << component.num_components() << " components";

        if (!SendData(component, uSysIdx)) return false;
      }  // if( 0 < fvvCompPerSysId[ uSysIdx ].size() )
    }    // if( 0 < fComponentsToSend[ uSysIdx ] )
  }      // for( uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )

  return true;
}

bool CbmMQTsaMultiSampler::CreateAndCombineComponentsPerChannel(const fles::Timeslice& ts)
{

  /// First build the list of components for each channel name if it was not already done
  if (false == fbListCompPerChannelReady) {
    /// First add each channel enabled for sending to the list of channels we will use
    for (uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx) {
      if (0 < fComponentsToSend[uSysIdx]) {
        for (uint32_t uChan = 0; uChan < fChannelsToSend[uSysIdx].size(); ++uChan) {
          const vector<std::string>::const_iterator pos =
            std::find(fvChannelsToSend.begin(), fvChannelsToSend.end(), fChannelsToSend[uSysIdx][uChan]);
          if (fvChannelsToSend.end() == pos) {
            fvChannelsToSend.push_back(fChannelsToSend[uSysIdx][uChan]);
            fvvCompPerChannel.push_back(std::vector<uint32_t>());
          }
        }  // for( uChan = 0; uChan < fChannelsToSend[ uSysIdx ].size(); ++ uChan )
      }    // if( 0 < fComponentsToSend[ uSysIdx ] )
    }      // for( uint32_t uSysIdx = 0; uSysIdx < fComponentsToSend.size(); ++uSysIdx )

    /// Now resize the vector in which we will store fo each sending channel the list of components
    fvvCompPerChannel.resize(fvChannelsToSend.size());

    /// Check for each component if its system is enabled and if the name of its channel(s) is in the list
    /// If yes, add it to the vector of the corresponding channel
    for (uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx) {
      uint16_t usSysId = ts.descriptor(uCompIdx, 0).sys_id;

      const vector<int>::const_iterator pos = std::find(fSysId.begin(), fSysId.end(), usSysId);
      if (fSysId.end() != pos) {
        const vector<std::string>::size_type idxSys = pos - fSysId.begin();

        if (0 < fComponentsToSend[idxSys]) {
          for (uint32_t uChan = 0; uChan < fChannelsToSend[idxSys].size(); ++uChan) {
            const vector<std::string>::const_iterator posCh =
              std::find(fvChannelsToSend.begin(), fvChannelsToSend.end(), fChannelsToSend[idxSys][uChan]);
            if (fvChannelsToSend.end() != posCh) {
              const vector<std::string>::size_type idxChan = posCh - fvChannelsToSend.begin();
              fvvCompPerChannel[idxChan].push_back(uCompIdx);
            }  // if( fvChannelsToSend.end() != posCh )
          }    // for( uChan = 0; uChan < fChannelsToSend[ idxSys ].size(); ++ uChan )
        }      // if( 0 < fComponentsToSend[ uSysIdx ] )
      }        // if( fSysId.end() != pos )
    }          // for( uint32_t uNrComp = 0; uNrComp < ts.num_components(); ++uNrComp )

    for (uint32_t uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx) {
      std::stringstream ss;
      ss << "Found " << std::setw(2) << fvvCompPerChannel[uChanIdx].size() << " components for channel "
         << fvChannelsToSend[uChanIdx] << " :";

      for (uint32_t uComp = 0; uComp < fvvCompPerChannel[uChanIdx].size(); ++uComp) {
        ss << " " << std::setw(3) << fvvCompPerChannel[uChanIdx][uComp];
      }  // for( uint32_t uComp = 0; uComp < fvvCompPerChannel[ uChanIdx ].size(); ++uComp )

      LOG(info) << ss.str();
    }  // for( uint32_t uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx )

    fbListCompPerChannelReady = true;
  }  // if( false == fbListCompPerSysIdReady )

  /// Loop on channels
  /// Loop on possible SysId and check channels

  /// Then loop on all possible channels and send TS with their respective components if needed
  for (uint32_t uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx) {
    LOG(debug) << "Create timeslice with components for channel " << fvChannelsToSend[uChanIdx];

    if (0 < fvvCompPerChannel[uChanIdx].size()) {
      fles::StorableTimeslice component {static_cast<uint32_t>(ts.num_core_microslices()), ts.index()};

      for (uint32_t uComp = 0; uComp < fvvCompPerChannel[uChanIdx].size(); ++uComp) {
        uint32_t uNumMsInComp = ts.num_microslices(fvvCompPerChannel[uChanIdx][uComp]);
        component.append_component(uNumMsInComp);

        LOG(debug) << "Add components to TS for SysId " << std::hex
                   << static_cast<uint16_t>(ts.descriptor(fvvCompPerChannel[uChanIdx][uComp], 0).sys_id) << std::dec
                   << " TS " << ts.index() << " Comp " << fvvCompPerChannel[uChanIdx][uComp];

        for (size_t m = 0; m < uNumMsInComp; ++m) {
          component.append_microslice(uComp, m, ts.descriptor(fvvCompPerChannel[uChanIdx][uComp], m),
                                      ts.content(fvvCompPerChannel[uChanIdx][uComp], m));
        }  // for( size_t m = 0; m < uNumMsInComp; ++m )
      }    // for( uint32_t uComp = 0; uComp < fvvCompPerChannel[ uChanIdx ].size(); ++uComp )

      LOG(debug) << "Prepared timeslice for channel " << fvChannelsToSend[uChanIdx] << " with "
                 << component.num_components() << " components";

      if (!SendData(component, fvChannelsToSend[uChanIdx])) return false;
    }  // if( 0 < fvvCompPerSysId[ uSysIdx ].size() )
  }    // for( uChanIdx = 0; uChanIdx < fvChannelsToSend.size(); ++uChanIdx )

  return true;
}

bool CbmMQTsaMultiSampler::CreateAndSendFullTs(const fles::Timeslice& ts)
{
  /// Send full TS to all enabled channels
  for (uint32_t uChanIdx = 0; uChanIdx < fChannelsToSend.size(); ++uChanIdx) {
    if (0 < fComponentsToSend[uChanIdx]) {
      LOG(debug) << "Copy timeslice component for channel " << fChannelsToSend[uChanIdx][0];

      fles::StorableTimeslice fullTs {ts};
      if (!SendData(fullTs, uChanIdx)) return false;
    }  // if( 0 < fComponentsToSend[ uChanIdx ] )
  }    // for( uint32_t uChanIdx = 0; uChanIdx < fChannelsToSend.size(); ++uChanIdx )
  return true;
}

bool CbmMQTsaMultiSampler::SendData(const fles::StorableTimeslice& component, int idx)
{
  // serialize the timeslice and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << component;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  // TODO: Implement sending same data to more than one channel
  // Need to create new message (copy message??)
  if (fComponentsToSend[idx] > 1) { LOG(info) << "Need to copy FairMessage"; }

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).

  LOG(debug) << "Send data to channel " << fChannelsToSend[idx][0];
  if (Send(msg, fChannelsToSend[idx][0]) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  fMessageCounter++;
  LOG(debug) << "Send message " << fMessageCounter << " with a size of " << msg->GetSize();

  return true;
}

bool CbmMQTsaMultiSampler::SendData(const fles::StorableTimeslice& component, std::string sChannel)
{
  // serialize the timeslice and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << component;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).
  LOG(debug) << "Send data to channel " << sChannel;
  if (Send(msg, sChannel) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  fMessageCounter++;
  LOG(debug) << "Send message " << fMessageCounter << " with a size of " << msg->GetSize();

  return true;
}
bool CbmMQTsaMultiSampler::SendMissedTsIdx(std::vector<uint64_t> vIndices)
{
  // serialize the vector and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << vIndices;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).
  LOG(debug) << "Send data to channel " << fsChannelNameMissedTs;
  if (Send(msg, fsChannelNameMissedTs) < 0) {
    LOG(error) << "Problem sending missed TS indices to channel " << fsChannelNameMissedTs;
    return false;
  }  // if( Send( msg, fsChannelNameMissedTs ) < 0 )

  return true;
}
bool CbmMQTsaMultiSampler::SendCommand(std::string sCommand)
{
  // serialize the vector and create the message
  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << sCommand;
  std::string* strMsg = new std::string(oss.str());

  FairMQMessagePtr msg(NewMessage(
    const_cast<char*>(strMsg->c_str()),  // data
    strMsg->length(),                    // size
    [](void* /*data*/, void* object) { delete static_cast<std::string*>(object); },
    strMsg));  // object that manages the data

  //  FairMQMessagePtr msg( NewMessage( const_cast<char*>( sCommand.c_str() ), // data
  //                                    sCommand.length(), // size
  //                                    []( void* /*data*/, void* object ){ delete static_cast< std::string * >( object ); },
  //                                    &sCommand ) ); // object that manages the data

  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).
  LOG(debug) << "Send data to channel " << fsChannelNameCommands;
  if (Send(msg, fsChannelNameCommands) < 0) {
    LOG(error) << "Problem sending missed TS indices to channel " << fsChannelNameCommands;
    return false;
  }  // if( Send( msg, fsChannelNameMissedTs ) < 0 )

  return true;
}

bool CbmMQTsaMultiSampler::SendHistograms()
{
  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr message(NewMessage());
  Serialize<RootSerializer>(*message, &fArrayHisto);

  /// Send message to the common histogram messages queue
  if (Send(message, fsChannelNameHistosInput) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }  // if( Send( message, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  ResetHistograms();

  return true;
}


bool CbmMQTsaMultiSampler::ResetHistograms()
{
  fhTsRate->Reset();
  fhTsSize->Reset();
  fhTsSizeEvo->Reset();
  fhTsMaxSizeEvo->Reset();
  fhMissedTS->Reset();
  fhMissedTSEvo->Reset();

  return true;
}

CbmMQTsaMultiSampler::~CbmMQTsaMultiSampler() {}

void CbmMQTsaMultiSampler::CalcRuntime()
{
  std::chrono::duration<double> run_time = std::chrono::steady_clock::now() - fTime;

  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}


void CbmMQTsaMultiSampler::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(info) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id) << std::dec;
  LOG(info) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver) << std::dec;
  LOG(info) << "Equipement ID: " << mdsc.eq_id;
  LOG(info) << "Flags: " << mdsc.flags;
  LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id) << std::dec;
  LOG(info) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver) << std::dec;
  LOG(info) << "Microslice Idx: " << mdsc.idx;
  LOG(info) << "Checksum: " << mdsc.crc;
  LOG(info) << "Size: " << mdsc.size;
  LOG(info) << "Offset: " << mdsc.offset;
}

bool CbmMQTsaMultiSampler::CheckTimeslice(const fles::Timeslice& ts)
{
  if (0 == ts.num_components()) {
    LOG(error) << "No Component in TS " << ts.index();
    return 1;
  }
  LOG(info) << "Found " << ts.num_components() << " different components in timeslice";

  for (size_t c = 0; c < ts.num_components(); ++c) {
    LOG(info) << "Found " << ts.num_microslices(c) << " microslices in component " << c;
    LOG(info) << "Component " << c << " has a size of " << ts.size_component(c) << " bytes";
    LOG(info) << "Component " << c << " has the system id 0x" << std::hex
              << static_cast<int>(ts.descriptor(c, 0).sys_id) << std::dec;
    LOG(info) << "Component " << c << " has the system id 0x" << static_cast<int>(ts.descriptor(c, 0).sys_id);

    /*
    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
*/
  }

  return true;
}
