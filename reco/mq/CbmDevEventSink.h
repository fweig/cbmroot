/* Copyright (C) 2020-2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer], Dominik Smith */

#ifndef CBMDEVICEEVTSINK_H_
#define CBMDEVICEEVTSINK_H_

/// CBM headers
#include "CbmDigiEvent.h"
#include "CbmEvent.h"
#include "CbmMqTMessage.h"
#include "CbmMuchDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

#include "TimesliceMetaData.h"

/// FAIRROOT headers
#include "FairMQDevice.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "Rtypes.h"
#include "TClonesArray.h"
#include "TObjArray.h"

/// C/C++ headers
#include <chrono>
#include <map>
#include <vector>

class TFile;
class TList;
class TClonesArray;
class FairRunOnline;
class FairRootManager;

class CbmEventTimeslice {
  /// TODO: rename to CbmTsWithEvents
public:
  CbmEventTimeslice(FairMQParts& parts);
  ~CbmEventTimeslice();

  std::vector<CbmDigiEvent> GetSelectedData();

  TimesliceMetaData fTsMetaData;
  std::vector<CbmDigiEvent> fvEvents;
};

class CbmDevEventSink : public FairMQDevice {
public:
  CbmDevEventSink();
  virtual ~CbmDevEventSink();

protected:
  virtual void InitTask();
  bool HandleMissTsData(FairMQMessagePtr&, int);
  bool HandleData(FairMQParts&, int);
  bool HandleCommand(FairMQMessagePtr&, int);

private:
  /// Constants

  /// Control flags
  Bool_t fbFillHistos = false;  //! Switch ON/OFF filling of histograms
  Bool_t fbFinishDone = false;  //! Keep track of whether the Finish was already called

  /// User settings parameters
  /// Algo enum settings
  std::string fsOutputFileName = "mcbm_digis_events.root";
  /// message queues
  std::string fsChannelNameMissedTs    = "missedts";
  std::string fsChannelNameDataInput   = "events";
  std::string fsChannelNameCommands    = "commands";
  std::string fsChannelNameHistosInput = "histogram-in";
  /// Histograms management
  uint32_t fuPublishFreqTs  = 100;
  double_t fdMinPublishTime = 0.5;
  double_t fdMaxPublishTime = 5.0;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Statistics & missed TS detection
  uint64_t fuPrevTsIndex                                 = 0;
  uint64_t fulNumMessages                                = 0;
  uint64_t fulTsCounter                                  = 0;
  uint64_t fulMissedTsCounter                            = 0;
  std::chrono::system_clock::time_point fLastPublishTime = std::chrono::system_clock::now();

  /// Control Commands reception
  bool fbReceivedEof      = false;
  uint64_t fuLastTsIndex  = 0;
  uint64_t fuTotalTsCount = 0;

  /// Data reception
  /// TS MetaData storage
  TClonesArray* fTimeSliceMetaDataArray = nullptr;  //!
  TimesliceMetaData* fTsMetaData        = nullptr;
  /// CbmEvents
  std::vector<CbmDigiEvent>* fEventsSel = nullptr;  //! output container of CbmEvents

  /// Storage for re-ordering
  /// Missed TS vector
  std::vector<uint64_t> fvulMissedTsIndices = {};
  /// Buffered TS
  std::map<uint64_t, CbmEventTimeslice> fmFullTsStorage = {};

  /// Data storage
  FairRunOnline* fpRun           = nullptr;
  FairRootManager* fpFairRootMgr = nullptr;

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

  /// Internal methods
  bool IsChannelNameAllowed(std::string channelName);
  bool InitHistograms();
  void CheckTsQueues();
  void PrepareTreeEntry(CbmEventTimeslice unpTs);
  void DumpTreeEntry();
  bool SendHistoConfAndData();
  bool SendHistograms();
  void Finish();
};

#endif /* CBMDEVICEEVTSINK_H_ */
