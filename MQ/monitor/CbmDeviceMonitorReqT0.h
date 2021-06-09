/** @file CbmDeviceMonitorReqT0.h
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

#ifndef CBMDEVICEMONITORREQT0_H_
#define CBMDEVICEMONITORREQT0_H_

#include "Timeslice.hpp"

#include "FairMQDevice.h"

#include "Rtypes.h"
#include "TMessage.h"
#include "TObjArray.h"

#include <chrono>
#include <map>
#include <vector>

class TList;
class CbmMcbm2018MonitorAlgoT0;

class CbmDeviceMonitorReqT0 : public FairMQDevice {
public:
  CbmDeviceMonitorReqT0();
  virtual ~CbmDeviceMonitorReqT0();

protected:
  virtual void InitTask();
  virtual bool ConditionalRun();

private:
  /// Constants
  static const uint16_t kusSysId = 0x90;

  /// Control flags
  Bool_t fbIgnoreOverlapMs       = kFALSE;  //! Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice
  Bool_t fbComponentsAddedToList = kFALSE;

  /// User settings parameters
  std::string fsChannelNameDataInput     = "ts-request";
  std::string fsTsBlockName              = "t0block";
  std::string fsChannelNameHistosInput   = "histogram-in";
  uint32_t fuHistoryHistoSize            = 3600;
  uint32_t fuMinTotPulser                = 185;
  uint32_t fuMaxTotPulser                = 195;
  uint32_t fuOffSpillCountLimit          = 25;
  uint32_t fuOffSpillCountLimitNonPulser = 10;
  double fdSpillCheckInterval            = 0.0128;
  std::vector<uint32_t> fvuChanMap       = {0, 1, 2, 3, 4, 5, 6, 7};
  uint32_t fuPublishFreqTs               = 100;
  double_t fdMinPublishTime              = 0.5;
  double_t fdMaxPublishTime              = 5.0;

  /// Parameters management
  TList* fParCList = nullptr;

  /// Statistics & first TS rejection
  uint64_t fulNumMessages                                = 0;
  uint64_t fulTsCounter                                  = 0;
  std::chrono::system_clock::time_point fLastPublishTime = std::chrono::system_clock::now();

  /// Processing algo
  CbmMcbm2018MonitorAlgoT0* fMonitorAlgo;

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

  bool InitContainers();
  bool InitHistograms();
  bool DoUnpack(const fles::Timeslice& ts, size_t component);
  void Finish();
  bool SendHistoConfAndData();
  bool SendHistograms();
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};


#endif /* CBMDEVICEMONITORREQT0_H_ */
