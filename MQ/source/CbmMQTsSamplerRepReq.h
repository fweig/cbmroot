/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/********
  * TODO:
  * Remove mode "Full TS spreading to multiple outputs"
  * Keep track of components sent in split TS mode
  * HW mark when sending independent components
  * Use exceptions + try/catch instead of boolean return values
  ********/

#ifndef CBMMQTSSAMPLERREPREQ_H_
#define CBMMQTSSAMPLERREPREQ_H_


#include "MicrosliceDescriptor.hpp"
#include "StorableTimeslice.hpp"
#include "Timeslice.hpp"
#include "TimesliceSource.hpp"

#include "FairMQDevice.h"

class TCanvas;
class TH1F;
class TH1I;
class TProfile;
#include <TObjArray.h>

#include <ctime>
#include <deque>
#include <string>
#include <utility>
#include <vector>

class CbmMQTsSamplerRepReq : public FairMQDevice {
public:
  CbmMQTsSamplerRepReq();
  virtual ~CbmMQTsSamplerRepReq();

protected:
  uint64_t fulMaxTimeslices;

  std::string fsFileName = "";
  std::string fsDirName  = "";

  std::vector<std::string> fvsInputFileList = {};  ///< List of input files
  std::string fsHost                        = "";
  uint16_t fusPort                          = 0;
  uint64_t fulHighWaterMark                 = 10;

  std::string fsChannelNameTsRequest = "ts-request";
  bool fbNoSplitTs                   = true;
  bool fbSendTsPerSysId              = false;
  bool fbSendTsPerBlock              = false;

  std::string fsChannelNameHistosInput = "histogram-in";
  uint32_t fuPublishFreqTs             = 0;
  double_t fdMinPublishTime            = 0.5;
  double_t fdMaxPublishTime            = 5;

  uint64_t fulFirstTsIndex   = 0;
  uint64_t fulPrevTsIndex    = 0;
  uint64_t fulTsCounter      = 0;
  uint64_t fulMessageCounter = 0;

  virtual void InitTask();
  bool HandleRequest(FairMQMessagePtr&, int);

private:
  void CalcRuntime();
  bool IsChannelNameAllowed(std::string);

  std::unique_ptr<fles::Timeslice> GetNewTs();
  bool AddNewTsInBuffer();
  bool CreateAndSendFullTs();
  bool PrepareCompListPerSysId();
  bool CreateCombinedComponentsPerSysId(std::string sSystemName);
  bool CreateCombinedComponentsPerSysId(int iSysId);
  bool CreateCombinedComponentsPerSysId(uint uCompIndex);
  bool PrepareCompListPerBlock();
  bool CreateCombinedComponentsPerBlock(std::string sBlockName);

  bool SendFirstTsIndex();
  bool SendData(const fles::StorableTimeslice& component);
  bool SendMissedTsIdx(std::vector<uint64_t> vIndices);
  bool SendCommand(std::string sCommand);

  bool InitHistograms();
  bool SendHistoConfAndData();
  bool SendHistograms();
  bool ResetHistograms();

  fles::TimesliceSource* fSource = nullptr;  //!
  std::chrono::steady_clock::time_point fTime;
  std::chrono::system_clock::time_point fLastPublishTime;


  // The vector fAllowedChannels contain the list of defined components names
  // which are used for connecting the different devices. A request
  // using the name stscomponent will receive timeslices containing the
  // sts component only. The corresponding system ids are defined in the
  // vector fSysId.
  // The Blocks are defined by the user by combining a name with a list of components,
  // either by name or by SysId
  // A components can only be added to one block, attempts to double book will throw
  // an init error
  std::vector<std::string> fComponents = {"mvdcomponent", "stscomponent", "richcomponent", "muchcomponent",
                                          "trdcomponent", "tofcomponent", "psdcomponent",  "t0component"};
  std::vector<int> fSysId              = {0x20, 0x10, 0x30, 0x50, 0x40, 0x60, 0x80, 0x90};
  std::vector<bool> fComponentActive   = {false, false, false, false, false, false, false, false};

  bool fbListCompPerSysIdReady                       = false;
  std::vector<std::vector<uint32_t>> fvvCompPerSysId = {{}, {}, {}, {}, {}, {}, {}, {}};

  bool fbListCompPerBlockReady                                           = false;
  std::vector<std::pair<std::string, std::set<uint16_t>>> fvBlocksToSend = {};
  std::vector<std::vector<uint32_t>> fvvCompPerBlock                     = {};

  /// Buffering of partially sent timeslices, limited by fulHighWaterMark
  std::deque<std::unique_ptr<fles::Timeslice>> fdpTimesliceBuffer = {};
  std::deque<std::vector<bool>> fdbCompSentFlags                  = {};

  /// Flag indicating the EOF was reached to avoid sending an emergency STOP
  bool fbEofFound = false;

  std::string fsChannelNameMissedTs = "";
  std::string fsChannelNameCommands = "";

  /// Array of histograms to send to the histogram server
  TObjArray fArrayHisto = {};
  /// Vector of string pairs with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder = {};
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig = {};
  /// Flag indicating whether the histograms and canvases configurations were already published
  bool fbConfigSent = false;

  /// Histograms
  TH1I* fhTsRate          = nullptr;
  TH1I* fhTsSize          = nullptr;
  TProfile* fhTsSizeEvo   = nullptr;
  TH1F* fhTsMaxSizeEvo    = nullptr;
  TH1I* fhMissedTS        = nullptr;
  TProfile* fhMissedTSEvo = nullptr;
  TCanvas* fcSummary      = nullptr;
  uint64_t fuStartTime    = 0;
  double_t fdTimeToStart  = 0.;
  double_t fdLastMaxTime  = 0.;
  double_t fdTsMaxSize    = 0.;
};

#endif /* CBMMQTSASAMPLER_H_ */
