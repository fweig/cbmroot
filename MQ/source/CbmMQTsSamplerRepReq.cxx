/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmMQTsSamplerRepReq.h"

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
//#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/serialization/utility.hpp>

#include "RootSerializer.h"

//namespace filesys = boost::filesystem;

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

CbmMQTsSamplerRepReq::CbmMQTsSamplerRepReq()
  : FairMQDevice()
  , fTime()
  , fLastPublishTime {std::chrono::system_clock::now()}
{
}

void CbmMQTsSamplerRepReq::InitTask()
try {
  // Get the values from the command line options (via fConfig)
  fsFileName               = fConfig->GetValue<string>("filename");
  fsDirName                = fConfig->GetValue<string>("dirname");
  fsHost                   = fConfig->GetValue<string>("fles-host");
  fusPort                  = fConfig->GetValue<uint16_t>("fles-port");
  fulHighWaterMark         = fConfig->GetValue<uint64_t>("high-water-mark");
  fulMaxTimeslices         = fConfig->GetValue<uint64_t>("max-timeslices");
  fsChannelNameTsRequest   = fConfig->GetValue<std::string>("ChNameTsReq");
  fbNoSplitTs              = fConfig->GetValue<bool>("no-split-ts");
  fbSendTsPerSysId         = fConfig->GetValue<bool>("send-ts-per-sysid");
  fbSendTsPerBlock         = fConfig->GetValue<bool>("send-ts-per-block");
  fsChannelNameMissedTs    = fConfig->GetValue<std::string>("ChNameMissTs");
  fsChannelNameCommands    = fConfig->GetValue<std::string>("ChNameCmds");
  fuPublishFreqTs          = fConfig->GetValue<uint32_t>("PubFreqTs");
  fdMinPublishTime         = fConfig->GetValue<double_t>("PubTimeMin");
  fdMaxPublishTime         = fConfig->GetValue<double_t>("PubTimeMax");
  fsChannelNameHistosInput = fConfig->GetValue<std::string>("ChNameIn");

  if (fbNoSplitTs) {
    if (fbSendTsPerSysId) {
      if (fbSendTsPerBlock) {
        LOG(warning) << "Both no-split-ts, send-ts-per-sysid and send-ts-per-block options used => "
                     << " second and third one will be ignored!!!!";
      }  // if( fbSendTsPerBlock )
      else
        LOG(warning) << "Both no-split-ts and send-ts-per-sysid options used => "
                     << " second one will be ignored!!!!";
    }  // if( fbSendTsPerSysId )
    else if (fbSendTsPerBlock) {
      LOG(warning) << "Both no-split-ts and send-ts-per-block options used => "
                   << " second one will be ignored!!!!";
    }  // else if( fbSendTsPerSysId ) of if( fbSendTsPerSysId )
    else
      LOG(debug) << "Running in no-split-ts mode!";
  }  // if( fbNoSplitTs )
  else if (fbSendTsPerBlock) {
    if (fbSendTsPerSysId) {
      LOG(warning) << "Both send-ts-per-sysid and send-ts-per-block options used => "
                   << " second one will be ignored!!!!";
    }  // if (fbSendTsPerSysId)
    else
      LOG(debug) << "Running in send-ts-per-block mode!";
  }  // else if( fbSendTsPerBlock ) of if( fbNoSplitTs )
  else if (fbSendTsPerSysId) {
    LOG(debug) << "Running in send-ts-per-sysid mode!";
  }  // else if (fbSendTsPerSysId) else if( fbSendTsPerBlock ) of if( fbNoSplitTs )
  else {
    LOG(debug) << "Running in no-split-ts mode by default!";
    fbNoSplitTs = true;
  }  // else of else if (fbSendTsPerSysId) else if( fbSendTsPerBlock ) of if( fbNoSplitTs )

  /// Extract SysId and channel information if provided in the binary options
  std::vector<std::string> vSysIdBlockPairs = fConfig->GetValue<std::vector<std::string>>("block-sysid");
  for (uint32_t uPair = 0; uPair < vSysIdBlockPairs.size(); ++uPair) {
    const size_t sep = vSysIdBlockPairs[uPair].find(':');
    if (string::npos == sep || 0 == sep || vSysIdBlockPairs[uPair].size() == sep) {
      LOG(info) << vSysIdBlockPairs[uPair];
      throw InitTaskError("Provided pair of Block name + SysId is missing a : or an argument.");
    }  // if( string::npos == sep || 0 == sep || vSysIdBlockPairs[ uPair ].size() == sep )

    /// Extract Block name
    std::string sBlockName = vSysIdBlockPairs[uPair].substr(0, sep);

    /// Extract SysId
    /// TODO: or component name
    std::string sSysId  = vSysIdBlockPairs[uPair].substr(sep + 1);
    const size_t hexPos = sSysId.find("0x");
    uint16_t usSysId;
    if (string::npos == hexPos) usSysId = std::stoi(sSysId);
    else
      usSysId = std::stoi(sSysId.substr(hexPos + 2), nullptr, 16);

    LOG(debug) << "Extracted block info from pair \"" << vSysIdBlockPairs[uPair] << "\": name is " << sBlockName
               << " and SysId is " << usSysId << " extracted from " << sSysId;

    /// Check if SysId already in use
    uint32_t uSysIdIdx = 0;
    for (; uSysIdIdx < fSysId.size() && fSysId[uSysIdIdx] != usSysId; ++uSysIdIdx) {}
    if (uSysIdIdx == fSysId.size()) { throw InitTaskError("Unknown System ID for " + vSysIdBlockPairs[uPair]); }
    else if (true == fComponentActive[uSysIdIdx]) {
      throw InitTaskError("System ID already in use by another block for " + vSysIdBlockPairs[uPair]);
    }
    fComponentActive[uSysIdIdx] = true;

    /// Look if Block is already defined
    auto itBlock = fvBlocksToSend.begin();
    for (; itBlock != fvBlocksToSend.end(); ++itBlock) {
      if ((*itBlock).first == sBlockName) break;
    }  // for( ; itBlock != fvBlocksToSend.end(); ++itBlock)
    if (fvBlocksToSend.end() != itBlock) {
      /// Block already there, add the SysId to its list
      (*itBlock).second.insert(usSysId);
    }  // if( fvBlocksToSend.end() != itBlock )
    else {
      /// Block unknown yet, add both Block and First SysId
      fvBlocksToSend.push_back(std::pair<std::string, std::set<uint16_t>>(sBlockName, {usSysId}));
      fvvCompPerBlock.push_back(std::vector<uint32_t>({}));
    }  // else of if( fSysId.end() != pos )

    LOG(info) << vSysIdBlockPairs[uPair] << " Added SysId 0x" << std::hex << usSysId << std::dec << " to "
              << sBlockName;
  }  // for( uint32_t uPair = 0; uPair < vSysIdBlockPairs.size(); ++uPair )

  if (0 == fulMaxTimeslices) fulMaxTimeslices = UINT_MAX;

  // Check which input is defined
  // Possibilities:
  // filename && ! dirname : single file
  // filename with wildcards && dirname : all files with filename regex in the directory
  // host && port : connect to the flesnet server
  bool isGoodInputCombi {false};
  if (0 != fsFileName.size() && 0 == fsDirName.size() && 0 == fsHost.size() && 0 == fusPort) {
    isGoodInputCombi = true;
    fvsInputFileList.push_back(fsFileName);
  }
  else if (0 != fsFileName.size() && 0 != fsDirName.size() && 0 == fsHost.size() && 0 == fusPort) {
    isGoodInputCombi = true;
    fvsInputFileList.push_back(fsFileName);
  }
  else if (0 == fsFileName.size() && 0 == fsDirName.size() && 0 != fsHost.size() && 0 != fusPort) {
    isGoodInputCombi = true;
    LOG(info) << "Host: " << fsHost;
    LOG(info) << "Port: " << fusPort;
  }
  else if (0 == fsFileName.size() && 0 == fsDirName.size() && 0 != fsHost.size() && 0 == fusPort) {
    isGoodInputCombi = true;
    LOG(info) << "Host string: " << fsHost;
  }
  else {
    isGoodInputCombi = false;
  }

  if (!isGoodInputCombi) {
    throw InitTaskError("Wrong combination of inputs. Either file or wildcard file + directory "
                        "or host + port are allowed combination.");
  }

  LOG(info) << "MaxTimeslices: " << fulMaxTimeslices;

  if (0 == fsFileName.size() && 0 != fsHost.size() && 0 != fusPort) {
    // Don't add the protocol since this is done now in the TimesliceMultiSubscriber
    //std::string connector = "tcp://" + fsHost + ":" + std::to_string(fusPort);
    std::string connector = fsHost + ":" + std::to_string(fusPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fSource = new fles::TimesliceMultiSubscriber(connector);
  }
  else if (0 == fsFileName.size() && 0 != fsHost.size()) {
    std::string connector = fsHost;
    LOG(info) << "Open TSPublisher with host string: " << connector;
    fSource = new fles::TimesliceMultiSubscriber(connector, fulHighWaterMark);
  }
  else {
    // Create a ";" separated string with all file names
    std::string fileList {""};
    for (const auto& obj : fvsInputFileList) {
      std::string fileName = obj;
      fileList += fileName;
      fileList += ";";
    }
    fileList.pop_back();  // Remove the last ;
    LOG(info) << "Input File String: " << fileList;
    fSource = new fles::TimesliceMultiInputArchive(fileList, fsDirName);
    if (!fSource) { throw InitTaskError("Could open files from file list."); }
  }

  LOG(info) << "High-Water Mark: " << fulHighWaterMark;
  LOG(info) << "Max. Timeslices: " << fulMaxTimeslices;
  if (fbNoSplitTs) { LOG(info) << "Sending TS copies in no-split mode"; }  // if( fbNoSplitTs )
  else if (fbSendTsPerSysId) {
    LOG(info) << "Sending components in separate TS per SysId";
  }  // else if( fbSendTsPerSysId ) of if( fbNoSplitTs )
  else if (fbSendTsPerBlock) {
    LOG(info) << "Sending components in separate TS per block (multiple SysId)";
  }  // else if( fbSendTsPerBlock ) of if( fbSendTsPerSysId ) of if( fbNoSplitTs )

  OnData(fsChannelNameTsRequest, &CbmMQTsSamplerRepReq::HandleRequest);

  fTime = std::chrono::steady_clock::now();
}
catch (InitTaskError& e) {
  LOG(error) << e.what();
  ChangeState(fair::mq::Transition::ErrorFound);
}
catch (boost::bad_any_cast& e) {
  LOG(error) << "Error during InitTask: " << e.what();
  ChangeState(fair::mq::Transition::ErrorFound);
}

bool CbmMQTsSamplerRepReq::InitHistograms()
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

  return true;
}

bool CbmMQTsSamplerRepReq::HandleRequest(FairMQMessagePtr& msgReq, int)
{
  /// Initialize the histograms
  if (0 < fuPublishFreqTs && 0 == fulTsCounter) { InitHistograms(); }  // if( 0 < fuPublishFreqTs )

  if (fbNoSplitTs) {

    if (!CreateAndSendFullTs()) {
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
    /// TODO: add support for alternative request with "system name" instead of "system ID"
    std::string reqStr(static_cast<char*>(msgReq->GetData()), msgReq->GetSize());
    int iSysId = std::stoi(reqStr);
    LOG(debug) << "Received TS SysId component request from client: 0x" << std::hex << iSysId << std::dec;

    /// This assumes that the order of the components does NOT change after the first TS
    /// That should be the case as the component index correspond to a physical link idx
    if (!CreateCombinedComponentsPerSysId(iSysId)) {
      /// If command channel defined, send command to all "slaves"
      if ("" != fsChannelNameCommands) {
        /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SendCommand("STOP");
      }  // if( "" != fsChannelNameCommands )

      return false;
    }  // if(!CreateAndCombineComponentsPerSysId(iSysId) )
  }    // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs
  else if (fbSendTsPerBlock) {
    std::string reqStr(static_cast<char*>(msgReq->GetData()), msgReq->GetSize());
    LOG(debug) << "Received TS components block request from client: " << reqStr;

    /// This assumes that the order of the components does NOT change after the first TS
    /// That should be the case as the component index correspond to a physical link idx
    if (!CreateCombinedComponentsPerBlock(reqStr)) {
      /// If command channel defined, send command to all "slaves"
      if ("" != fsChannelNameCommands) {
        /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SendCommand("STOP");
      }  // if( "" != fsChannelNameCommands )

      return false;
    }  // if( !CreateAndCombineComponentsPerChannel(reqStr) )
  }    // else if( fbSendTsPerSysId && fbSendTsPerSysId ) of if( fbNoSplitTs )

  /// Send histograms each 100 time slices. Should be each ~1s
  /// Use also runtime checker to trigger sending after M s if
  /// processing too slow or delay sending if processing too fast
  std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
  std::chrono::duration<double_t> elapsedSeconds    = currentTime - fLastPublishTime;
  if ((fdMaxPublishTime < elapsedSeconds.count())
      || (0 == fulMessageCounter % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count())) {
    if (!fbConfigSent) {
      // Send the configuration only once per run!
      fbConfigSent = SendHistoConfAndData();
    }  // if( !fbConfigSent )
    else
      SendHistograms();

    fLastPublishTime = std::chrono::system_clock::now();
  }  // if( ( fdMaxPublishTime < elapsedSeconds.count() ) || ( 0 == fulMessageCounter % fuPublishFreqTs && fdMinPublishTime < elapsedSeconds.count() ) )

  return true;
}

std::unique_ptr<fles::Timeslice> CbmMQTsSamplerRepReq::GetNewTs()
{
  /// Initialize the source (connect to emitter, ...)
  if (0 == fulTsCounter && nullptr != dynamic_cast<fles::TimesliceMultiSubscriber*>(fSource)) {
    dynamic_cast<fles::TimesliceMultiSubscriber*>(fSource)->InitTimesliceSubscriber();
  }  // if( 0 == fulTsCounter && nullptr != dynamic_cast< fles::TimesliceMultiSubscriber >(fSource) )

  std::unique_ptr<fles::Timeslice> timeslice = fSource->get();
  if (timeslice) {
    if (fulTsCounter < fulMaxTimeslices) {
      fulTsCounter++;

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
      if ((uTsIndex != (fulPrevTsIndex + 1)) && (0 != fulPrevTsIndex && 0 != uTsIndex)) {
        LOG(info) << "Missed Timeslices. Old TS Index was " << fulPrevTsIndex << " New TS Index is " << uTsIndex
                  << " diff is " << uTsIndex - fulPrevTsIndex << " Missing are " << uTsIndex - fulPrevTsIndex - 1;

        if ("" != fsChannelNameMissedTs) {
          /// Add missing TS indices to a vector and send it in appropriate channel
          std::vector<uint64_t> vulMissedIndices;
          for (uint64_t ulMiss = fulPrevTsIndex + 1; ulMiss < uTsIndex; ++ulMiss) {
            vulMissedIndices.emplace_back(ulMiss);
          }  // for( uint64_t ulMiss = fulPrevTsIndex + 1; ulMiss < uTsIndex; ++ulMiss )
          if (!SendMissedTsIdx(vulMissedIndices)) {
            /// If command channel defined, send command to all "slaves"
            if ("" != fsChannelNameCommands) {
              /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
              std::this_thread::sleep_for(std::chrono::milliseconds(1000));
              SendCommand("STOP");
            }  // if( "" != fsChannelNameCommands )

            return nullptr;
          }  // if( !SendMissedTsIdx( vulMissedIndices ) )
        }    // if( "" != fsChannelNameMissedTs )

        if (0 < fuPublishFreqTs) {
          fhMissedTS->Fill(1, uTsIndex - fulPrevTsIndex - 1);
          fhMissedTSEvo->Fill(fdTimeToStart, 1, uTsIndex - fulPrevTsIndex - 1);
        }  // if( 0 < fuPublishFreqTs )

      }  // if( ( uTsIndex != ( fulPrevTsIndex + 1 ) ) && ( 0 != fulPrevTsIndex && 0 != uTsIndex ) )

      if (0 < fuPublishFreqTs) {
        fhMissedTS->Fill(0);
        fhMissedTSEvo->Fill(fdTimeToStart, 0, 1);
      }  // else if( 0 < fuPublishFreqTs )

      fulPrevTsIndex = uTsIndex;

      if (fulTsCounter % 10000 == 0) { LOG(info) << "Received TS " << fulTsCounter << " with index " << uTsIndex; }

      LOG(debug) << "Found " << ts.num_components() << " different components in timeslice";
    }  // else of if (fulTsCounter < fulMaxTimeslices)
    return timeslice;
  }  // if (timeslice)
  else {
    CalcRuntime();

    /// If command channel defined, send command to all "slaves"
    if ("" != fsChannelNameCommands) {
      /// Wait 1 s before sending an EOF to let all slaves finish processing previous data
      std::this_thread::sleep_for(std::chrono::seconds(10));
      std::string sCmd = "EOF ";
      sCmd += FormatDecPrintout(fulPrevTsIndex);
      sCmd += " ";
      sCmd += FormatDecPrintout(fulTsCounter);
      SendCommand(sCmd);
    }  // if( "" != fsChannelNameCommands )

    return nullptr;
  }  // else of if (timeslice)
}

bool CbmMQTsSamplerRepReq::AddNewTsInBuffer()
{
  /// Remove the first TS(s) in buffer if we reached the HighWater mark
  while (fulHighWaterMark <= fdpTimesliceBuffer.size()) {
    fdpTimesliceBuffer.pop_front();
    fdbCompSentFlags.pop_front();
  }  // while( fulHighWaterMark <= fdpTimesliceBuffer.size() )

  /// Add a new TS and "fail" if we did not get it
  fdpTimesliceBuffer.push_back(GetNewTs());
  if (nullptr == fdpTimesliceBuffer.back()) {
    fdpTimesliceBuffer.pop_back();
    return false;
  }  // if(nullptr == fdpTimesliceBuffer[fdpTimesliceBuffer.size() - 1])

  /// Now that we got the TS, we can add the corresponding list of "Sent" flags,
  /// with the proper dimension
  if (fbSendTsPerBlock) { fdbCompSentFlags.push_back(std::vector<bool>(fvBlocksToSend.size(), false)); }
  else {
    fdbCompSentFlags.push_back(std::vector<bool>(fComponentActive.size(), false));
  }
  return true;
}

bool CbmMQTsSamplerRepReq::CreateAndSendFullTs()
{
  std::unique_ptr<fles::Timeslice> timeslice = GetNewTs();
  if (timeslice) {
    /// Send full TS as response to the request
    const fles::Timeslice& ts = *timeslice;
    fles::StorableTimeslice fullTs {ts};
    if (!SendData(fullTs)) {
      /// If command channel defined, send command to all "slaves"
      if ("" != fsChannelNameCommands) {
        /// Wait 1 s before sending a STOP to let all slaves finish processing previous data
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        SendCommand("STOP");
      }  // if( "" != fsChannelNameCommands )

      return false;
    }  // if (!SendData(fullTs, uChanIdx))
    return true;
  }  // if (timeslice)
  else {
    return false;
  }  // else of if (timeslice)
}

bool CbmMQTsSamplerRepReq::PrepareCompListPerSysId()
{
  if (false == fbListCompPerSysIdReady) {
    /// Check if already at least one TS in the buffer (should not be the case
    /// => if not, add one
    if (0 == fdpTimesliceBuffer.size()) {
      if (!AddNewTsInBuffer()) return false;
    }  // if( 0 == fdpTimesliceBuffer.size() )

    if (nullptr == fdpTimesliceBuffer.front()) return false;

    for (uint32_t uCompIdx = 0; uCompIdx < fdpTimesliceBuffer.front()->num_components(); ++uCompIdx) {
      uint16_t usMsSysId = fdpTimesliceBuffer.front()->descriptor(uCompIdx, 0).sys_id;

      const vector<int>::const_iterator pos = std::find(fSysId.begin(), fSysId.end(), usMsSysId);
      if (fSysId.end() != pos) {
        const vector<std::string>::size_type idx = pos - fSysId.begin();

        fvvCompPerSysId[idx].push_back(uCompIdx);
      }  // if( fSysId.end() != pos )
    }    // for( uint32_t uCompIdx = 0; uCompIdx < ts.num_components(); ++uCompIdx )

    for (uint32_t uSysIdx = 0; uSysIdx < fComponents.size(); ++uSysIdx) {
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

  return true;
}
bool CbmMQTsSamplerRepReq::CreateCombinedComponentsPerSysId(std::string sSystemName)
{
  /// Check if the requested System name is in the list of known components
  /// 1) First build the list of components for each SysId if it was not already done
  if (!PrepareCompListPerSysId()) return false;

  /// 2) Search for requested System name is in the list of known components, get its index and then send the TS
  const vector<std::string>::const_iterator pos = std::find(fComponents.begin(), fComponents.end(), sSystemName);
  if (fComponents.end() != pos) {
    const vector<std::string>::size_type idx = pos - fComponents.begin();
    return CreateCombinedComponentsPerSysId(static_cast<uint32_t>(idx));
  }  // if (fComponents.end() != pos)
  else {
    LOG(error) << "Did not find " << sSystemName << " in the list of known systems";
    return false;
  }  // else of if (fComponents.end() != pos)
}
bool CbmMQTsSamplerRepReq::CreateCombinedComponentsPerSysId(int iSysId)
{
  /// Check if the requested System ID is in the list of known components
  /// 1) First build the list of components for each SysId if it was not already done
  if (!PrepareCompListPerSysId()) return false;

  /// 2) Search for requested System ID is in the list of known components, get its index and then send the TS
  const vector<int>::const_iterator pos = std::find(fSysId.begin(), fSysId.end(), iSysId);
  if (fSysId.end() != pos) {
    const vector<int>::size_type idx = pos - fSysId.begin();
    return CreateCombinedComponentsPerSysId(static_cast<uint32_t>(idx));
  }  // if (fSysId.end() != pos)
  else {
    LOG(error) << "Did not find 0x" << std::hex << iSysId << std::dec << " in the list of known systems";
    return false;
  }  // else of if (fSysId.end() != pos)
}
bool CbmMQTsSamplerRepReq::CreateCombinedComponentsPerSysId(uint uCompIndex)
{
  /// Then loop on all possible SysId and send TS with their respective components if needed
  LOG(debug) << "Create timeslice with components for SysId " << std::hex << fSysId[uCompIndex] << std::dec;

  if (0 < fvvCompPerSysId[uCompIndex].size()) {
    /// Search if TS in buffer where all components for this system where not sent yet
    uint32_t uTsIndex = 0;
    for (; uTsIndex < fdpTimesliceBuffer.size(); ++uTsIndex) {
      if (false == fdbCompSentFlags[uTsIndex][uCompIndex]) break;
    }  // for( ; uTsIndex < fdpTimesliceBuffer.size(); ++uTsIndex )

    /// If all TS in buffer have sent this one, get a new TS
    if (fdpTimesliceBuffer.size() == uTsIndex) {
      --uTsIndex;
      if (!AddNewTsInBuffer()) return false;
    }  // if( fdpTimesliceBuffer.size() == uTsIndex )

    /// Prepare the custom TS and send it
    fles::StorableTimeslice component {static_cast<uint32_t>(fdpTimesliceBuffer[uTsIndex]->num_core_microslices()),
                                       fdpTimesliceBuffer[uTsIndex]->index()};

    for (uint32_t uComp = 0; uComp < fvvCompPerSysId[uCompIndex].size(); ++uComp) {
      uint32_t uNumMsInComp = fdpTimesliceBuffer[uTsIndex]->num_microslices(fvvCompPerSysId[uCompIndex][uComp]);
      component.append_component(uNumMsInComp);

      LOG(debug) << "Add components to TS for SysId " << std::hex << fSysId[uCompIndex] << std::dec << " TS "
                 << fdpTimesliceBuffer[uTsIndex]->index() << " Comp " << fvvCompPerSysId[uCompIndex][uComp];

      for (size_t m = 0; m < uNumMsInComp; ++m) {
        component.append_microslice(uComp, m,
                                    fdpTimesliceBuffer[uTsIndex]->descriptor(fvvCompPerSysId[uCompIndex][uComp], m),
                                    fdpTimesliceBuffer[uTsIndex]->content(fvvCompPerSysId[uCompIndex][uComp], m));
      }  // for( size_t m = 0; m < uNumMsInComp; ++m )
    }    // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uCompIndex ].size(); ++uComp )

    LOG(debug) << "Prepared timeslice for SysId " << std::hex << fSysId[uCompIndex] << std::dec << " with "
               << component.num_components() << " components";

    if (!SendData(component)) return false;

    fdbCompSentFlags[uTsIndex][uCompIndex] = true;
  }  // if (0 < fvvCompPerSysId[uCompIndex].size())

  return true;
}

bool CbmMQTsSamplerRepReq::PrepareCompListPerBlock()
{
  if (false == fbListCompPerBlockReady) {
    /// 1) First build the list of components for each SysId if it was not already done
    if (!PrepareCompListPerSysId()) return false;

    /// 2) Build the list of components for each block, based on its list of system IDs
    for (auto itBlock = fvBlocksToSend.begin(); itBlock != fvBlocksToSend.end(); ++itBlock) {
      auto uBlockIdx = itBlock - fvBlocksToSend.begin();

      for (auto itSys = (itBlock->second).begin(); itSys != (itBlock->second).end(); ++itSys) {
        /// Check if this system ID is existing
        const vector<int>::const_iterator pos = std::find(fSysId.begin(), fSysId.end(), *itSys);
        if (fSysId.end() != pos) {
          const vector<int>::size_type idxSys = pos - fSysId.begin();

          /// Add all components to the list
          for (uint32_t uComp = 0; uComp < fvvCompPerSysId[idxSys].size(); ++uComp) {
            fvvCompPerBlock[uBlockIdx].push_back(fvvCompPerSysId[idxSys][uComp]);
          }  // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ idxSys ].size(); ++uComp )
        }    // if (fSysId.end() != pos)
        else {
          LOG(error) << "Error when building the components list for block " << itBlock->first;
          LOG(error) << "Did not find 0x" << std::hex << *itSys << std::dec << " in the list of known systems";
          return false;
        }  // else of if (fSysId.end() != pos)
      }    // for( auto itSys = (itBlock->second).begin(); itSys != (itBlock->second).end(); ++itSys )
    }      // for( auto itBlock = fvBlocksToSend.begin(); itBlock != fvBlocksToSend.end(); ++itBlock)

    fbListCompPerBlockReady = true;
  }  // if( false == fbListCompPerBlockReady )

  return true;
}

bool CbmMQTsSamplerRepReq::CreateCombinedComponentsPerBlock(std::string sBlockName)
{
  /// Check if the requested Block is in the list of known blocks
  /// 1) First build the list of components for each block if it was not already done
  if (!PrepareCompListPerBlock()) return false;

  /// 2) Search for requested block is in the list of known blocks, get its index and then send the TS
  for (auto itKnownBlock = fvBlocksToSend.begin(); itKnownBlock != fvBlocksToSend.end(); ++itKnownBlock) {
    if ((*itKnownBlock).first == sBlockName) {
      auto uBlockIdx = itKnownBlock - fvBlocksToSend.begin();

      /// Search if TS in buffer where all components for this system where not sent yet
      uint32_t uTsIndex = 0;
      for (; uTsIndex < fdpTimesliceBuffer.size(); ++uTsIndex) {
        if (false == fdbCompSentFlags[uTsIndex][uBlockIdx]) break;
      }  // for( ; uTsIndex < fdpTimesliceBuffer.size(); ++uTsIndex )

      /// If all TS in buffer have sent this one, get a new TS
      if (fdpTimesliceBuffer.size() == uTsIndex) {
        --uTsIndex;
        if (!AddNewTsInBuffer()) return false;
      }  // if( fdpTimesliceBuffer.size() == uTsIndex )

      /// Prepare the custom TS and send it
      fles::StorableTimeslice component {static_cast<uint32_t>(fdpTimesliceBuffer[uTsIndex]->num_core_microslices()),
                                         fdpTimesliceBuffer[uTsIndex]->index()};

      for (uint32_t uComp = 0; uComp < fvvCompPerBlock[uBlockIdx].size(); ++uComp) {
        uint32_t uNumMsInComp = fdpTimesliceBuffer[uTsIndex]->num_microslices(fvvCompPerBlock[uBlockIdx][uComp]);
        component.append_component(uNumMsInComp);

        LOG(debug) << "Add components to TS for Block " << sBlockName << " TS " << fdpTimesliceBuffer[uTsIndex]->index()
                   << " Comp " << fvvCompPerBlock[uBlockIdx][uComp];

        for (size_t m = 0; m < uNumMsInComp; ++m) {
          component.append_microslice(uComp, m,
                                      fdpTimesliceBuffer[uTsIndex]->descriptor(fvvCompPerBlock[uBlockIdx][uComp], m),
                                      fdpTimesliceBuffer[uTsIndex]->content(fvvCompPerBlock[uBlockIdx][uComp], m));
        }  // for( size_t m = 0; m < uNumMsInComp; ++m )
      }    // for( uint32_t uComp = 0; uComp < fvvCompPerSysId[ uCompIndex ].size(); ++uComp )

      LOG(debug) << "Prepared timeslice for Block " << sBlockName << " with " << component.num_components()
                 << " components";

      if (!SendData(component)) return false;

      fdbCompSentFlags[uTsIndex][uBlockIdx] = true;
      return true;
    }  // if( (*itKnownBlock).first == sBlockName )
  }    // for( auto itKnownBlock = fvBlocksToSend.begin(); itKnownBlock != fvBlocksToSend.end(); ++itKnownBlock)

  /// Should reach here only if the block name was not found in the list!
  LOG(error) << "Requested block " << sBlockName << " not found in the list of known blocks";
  return false;
}

bool CbmMQTsSamplerRepReq::SendData(const fles::StorableTimeslice& component)
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
  if (Send(msg, fsChannelNameTsRequest) < 0) {
    LOG(error) << "Problem sending data";
    return false;
  }

  fulMessageCounter++;
  LOG(debug) << "Send message " << fulMessageCounter << " with a size of " << msg->GetSize();

  return true;
}
bool CbmMQTsSamplerRepReq::SendMissedTsIdx(std::vector<uint64_t> vIndices)
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
bool CbmMQTsSamplerRepReq::SendCommand(std::string sCommand)
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
bool CbmMQTsSamplerRepReq::SendHistoConfAndData()
{
  /// Prepare multiparts message and header
  std::pair<uint32_t, uint32_t> pairHeader(fvpsHistosFolder.size(), fvpsCanvasConfig.size());
  FairMQMessagePtr messageHeader(NewMessage());
  Serialize<BoostSerializer<std::pair<uint32_t, uint32_t>>>(*messageHeader, pairHeader);

  FairMQParts partsOut;
  partsOut.AddPart(std::move(messageHeader));

  for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto) {
    /// Serialize the vector of histo config into a single MQ message
    FairMQMessagePtr messageHist(NewMessage());
    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageHist, fvpsHistosFolder[uHisto]);

    partsOut.AddPart(std::move(messageHist));
  }  // for (UInt_t uHisto = 0; uHisto < fvpsHistosFolder.size(); ++uHisto)

  for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv) {
    /// Serialize the vector of canvas config into a single MQ message
    FairMQMessagePtr messageCan(NewMessage());
    Serialize<BoostSerializer<std::pair<std::string, std::string>>>(*messageCan, fvpsCanvasConfig[uCanv]);

    partsOut.AddPart(std::move(messageCan));
  }  // for (UInt_t uCanv = 0; uCanv < fvpsCanvasConfig.size(); ++uCanv)

  /// Serialize the array of histos into a single MQ message
  FairMQMessagePtr msgHistos(NewMessage());
  Serialize<RootSerializer>(*msgHistos, &fArrayHisto);

  partsOut.AddPart(std::move(msgHistos));

  /// Send the multi-parts message to the common histogram messages queue
  if (Send(partsOut, fsChannelNameHistosInput) < 0) {
    LOG(error) << "CbmMQTsSamplerRepReq::SendHistoConfAndData => Problem sending data";
    return false;
  }  // if( Send( partsOut, fsChannelNameHistosInput ) < 0 )

  /// Reset the histograms after sending them (but do not reset the time)
  ResetHistograms();

  return true;
}

bool CbmMQTsSamplerRepReq::SendHistograms()
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

bool CbmMQTsSamplerRepReq::ResetHistograms()
{
  fhTsRate->Reset();
  fhTsSize->Reset();
  fhTsSizeEvo->Reset();
  fhTsMaxSizeEvo->Reset();
  fhMissedTS->Reset();
  fhMissedTSEvo->Reset();

  return true;
}

CbmMQTsSamplerRepReq::~CbmMQTsSamplerRepReq() {}

void CbmMQTsSamplerRepReq::CalcRuntime()
{
  std::chrono::duration<double> run_time = std::chrono::steady_clock::now() - fTime;

  LOG(info) << "Runtime: " << run_time.count();
  LOG(info) << "No more input data";
}
