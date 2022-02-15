/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith[committer] */

/**
 * CbmDeviceEventBuilder.h
 *
 * @since 2022-02-01
 * @author D. Smith
 */

#ifndef CBMDEVICEEVENTBUILDER_H_
#define CBMDEVICEEVENTBUILDER_H_

/// CBM headers
#include "TimeClusterTrigger.h"

#include "EventBuilder.h"

/// FAIRROOT headers
#include "FairMQDevice.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "Rtypes.h"
#include "TObjArray.h"

/// C/C++ headers
#include <chrono>
#include <vector>

class CbmTsEventHeader;

class FairRunOnline;
class FairRootManager;

class CbmDeviceEventBuilder : public FairMQDevice {
public:
  CbmDeviceEventBuilder();
  virtual ~CbmDeviceEventBuilder();

protected:
  virtual void InitTask();
  bool HandleData(FairMQParts&, int);
  bool HandleCommand(FairMQMessagePtr&, int);

private:
  Bool_t fbFinishDone = false;  //! Keep track of whether the Finish was already called

  /// Constants

  /// Control flags
  //Bool_t fbIgnoreTsOverlap = kFALSE;  //! Ignore data in Overlap part of the TS
  //Bool_t fbFillHistos      = kTRUE;   //! Switch ON/OFF filling of histograms

  /// User settings parameters
  /// Algo enum settings
  ECbmModuleId fTriggerDet = ECbmModuleId::kT0;

  std::string fsOutputFileName = "";
  /// message queues
  std::string fsChannelNameDataInput   = "unpts_0";
  std::string fsChannelNameDataOutput  = "events";
  std::string fsChannelNameCommands    = "commands";
  std::string fsChannelNameHistosInput = "histogram-in";
  /// Histograms management
  uint32_t fuPublishFreqTs  = 100;
  double_t fdMinPublishTime = 0.5;
  double_t fdMaxPublishTime = 5.0;

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Statistics & first TS rejection
  uint64_t fulNumMessages                                = 0;
  uint64_t fulTsCounter                                  = 0;
  std::chrono::system_clock::time_point fLastPublishTime = std::chrono::system_clock::now();

  /// Processing algos
  cbm::algo::TimeClusterTrigger fTriggerAlgo;
  cbm::algo::EventBuilder fEvbuildAlgo;

  // Trigger algorithm params
  double fTriggerWindow = 0.;
  int32_t fMinNumDigis  = 0;
  double fDeadTime      = 0.;

  /// TS MetaData stable values storage
  size_t fuNbCoreMsPerTs    = 0;        //!
  size_t fuNbOverMsPerTs    = 0;        //!
  Double_t fdMsSizeInNs     = 1280000;  //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs = -1.0;     //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsOverSizeInNs = -1.0;     //! Total size of the overlap MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs = -1.0;     //! Total size of all MS in a TS, [nanoseconds]

  /// Data reception
  /// TS information in header
  CbmTsEventHeader* fCbmTsEventHeader = nullptr;

  /// Data storage
  FairRunOnline* fpRun           = nullptr;
  FairRootManager* fpFairRootMgr = nullptr;

  /// CbmEvents
  std::vector<CbmDigiEvent>* fEventsSel = nullptr;  //! output container of CbmEvents

  bool IsChannelNameAllowed(std::string channelName);
  bool SendEvents(FairMQParts& partsIn, const std::vector<CbmDigiEvent>& vEvents);

  // --- Extract digi times into to a vector
  template<class TDigi>
  std::vector<double> GetDigiTimes(const std::vector<TDigi>& digiVec)
  {
    std::vector<double> digiTimes(digiVec.size());
    std::transform(digiVec.begin(), digiVec.end(), digiTimes.begin(), [](const TDigi& digi) { return digi.GetTime(); });
    return digiTimes;
  }

  // Get trigger times using trigger algorithm
  std::vector<double> GetTriggerTimes(const CbmDigiTimeslice& ts);

  // Get detector type from string containing name
  ECbmModuleId GetDetectorId(std::string detName);

  void DumpTreeEntry();
  void Finish();
};

#endif /* CBMDEVICEEVENTBUILDER_H_ */
