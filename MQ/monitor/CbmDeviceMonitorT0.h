/* Copyright (C) 2019-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Florian Uhlig [committer] */

/**
 * CbmDeviceMonitorT0.h
 *
 * @since 2019-03-26
 * @author F. Uhlig
 */

#ifndef CBMDEVICEMONITORT0_H_
#define CBMDEVICEMONITORT0_H_

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

class CbmDeviceMonitorT0 : public FairMQDevice {
public:
  CbmDeviceMonitorT0();
  virtual ~CbmDeviceMonitorT0();

protected:
  virtual void InitTask();
  bool HandleData(FairMQMessagePtr&, int);

private:
  /// Constants
  static const uint16_t kusSysId = 0x90;

  /// Control flags
  Bool_t fbIgnoreOverlapMs;  //! Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice
  Bool_t fbComponentsAddedToList = kFALSE;

  /// User settings parameters
  std::string fsChannelNameDataInput;
  std::string fsChannelNameHistosInput;
  std::string fsChannelNameHistosConfig;
  std::string fsChannelNameCanvasConfig;
  uint32_t fuHistoryHistoSize;
  uint32_t fuMinTotPulser;
  uint32_t fuMaxTotPulser;
  uint32_t fuOffSpillCountLimit;
  uint32_t fuOffSpillCountLimitNonPulser;
  double fdSpillCheckInterval;
  std::vector<uint32_t> fvuChanMap;
  uint32_t fuPublishFreqTs;
  double_t fdMinPublishTime;
  double_t fdMaxPublishTime;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels;

  /// Parameters management
  TList* fParCList;

  /// Statistics & first TS rejection
  uint64_t fulNumMessages;
  uint64_t fulTsCounter;
  std::chrono::system_clock::time_point fLastPublishTime;

  /// Processing algo
  CbmMcbm2018MonitorAlgoT0* fMonitorAlgo;

  /// Array of histograms to send to the histogram server
  TObjArray fArrayHisto;
  /// Vector of string pairs with ( HistoName, FolderPath ) to send to the histogram server
  std::vector<std::pair<std::string, std::string>> fvpsHistosFolder;
  /// Vector of string pairs with ( CanvasName, CanvasConfig ) to send to the histogram server
  /// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
  /// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),HistoName(s),DrawOptions(s)"
  std::vector<std::pair<std::string, std::string>> fvpsCanvasConfig;

  bool IsChannelNameAllowed(std::string channelName);
  Bool_t InitContainers();
  Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void Finish();
  bool SendHistograms();
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage {
public:
  CbmMQTMessage(void* buf, Int_t len) : TMessage(buf, len) { ResetBit(kIsOwner); }
};


#endif /* CBMDEVICEMONITORT0_H_ */
