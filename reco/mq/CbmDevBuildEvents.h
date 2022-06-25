/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith[committer] */

/**
 * CbmDevBuildEvents.h
 *
 * @since 2022-02-01
 * @author D. Smith
 */

#ifndef CBMDEVICEEVENTBUILDER_H_
#define CBMDEVICEEVENTBUILDER_H_

/// CBM headers
#include "EventBuilder.h"

/// FAIRROOT headers
#include "FairMQDevice.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "Rtypes.h"
#include "TObjArray.h"

/// C/C++ headers
#include <vector>

class FairRunOnline;
class FairRootManager;

class TClonesArray;
class TimesliceMetaData;

class CbmDevBuildEvents : public FairMQDevice {
public:
  CbmDevBuildEvents();
  virtual ~CbmDevBuildEvents();

protected:
  virtual void InitTask();
  bool HandleData(FairMQParts&, int);

private:
  Bool_t fbFinishDone = false;  //! Keep track of whether the Finish was already called

  /// User settings parameters
  /// message queues
  std::string fsChannelNameDataInput  = "trigger";
  std::string fsChannelNameDataOutput = "events";

  /// List of MQ channels names
  std::vector<std::string> fsAllowedChannels = {fsChannelNameDataInput};

  /// Statistics & first TS rejection
  uint64_t fulNumMessages = 0;
  uint64_t fulTsCounter   = 0;

  /// Processing algos
  cbm::algo::EventBuilder fEvbuildAlgo;

  /// Data storage
  std::string fsOutputFileName             = "";
  FairRunOnline* fpRun                     = nullptr;
  FairRootManager* fpFairRootMgr           = nullptr;
  std::vector<CbmDigiEvent>* fEventsSelOut = nullptr;  // output container of CbmDigiEvents
  TClonesArray* fTimeSliceMetaDataArrayOut = nullptr;  // output container of meta data

  bool IsChannelNameAllowed(std::string channelName);
  bool SendEvents(const std::vector<CbmDigiEvent>& vEvents, const TimesliceMetaData* tsMetaData);

  // Get detector type from string containing name
  ECbmModuleId GetDetectorId(std::string detName);

  void DumpTreeEntry();
  void Finish();
};

#endif /* CBMDEVICEEVENTBUILDER_H_ */
