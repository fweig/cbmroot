/* Copyright (C) 2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith[committer] */

/**
 * CbmDevTrigger.h
 *
 * @since 2022-02-01
 * @author D. Smith
 */

#ifndef CBMDEVICETRIGGER_H_
#define CBMDEVICETRIGGER_H_

/// CBM headers
#include "CbmDefs.h"

#include "TimeClusterTrigger.h"

/// FAIRROOT headers
#include "FairMQDevice.h"

/// FAIRSOFT headers (geant, boost, ...)
#include "Rtypes.h"
#include "TObjArray.h"

/// C/C++ headers
#include <chrono>
#include <vector>

class CbmDigiTimeslice;
class CbmTsEventHeader;
class TimesliceMetaData;

class CbmDevTrigger : public FairMQDevice {
public:
  CbmDevTrigger();
  virtual ~CbmDevTrigger() {};

protected:
  virtual void InitTask();
  bool HandleData(FairMQParts&, int);

private:
  /// User settings parameters
  /// Algo enum settings
  ECbmModuleId fTriggerDet = ECbmModuleId::kNotExist;

  /// message queues
  std::string fChannelNameDataInput  = "";
  std::string fChannelNameDataOutput = "";

  /// Statistics
  uint64_t fNumMessages = 0;

  /// Processing algos
  cbm::algo::TimeClusterTrigger fTriggerAlgo;

  // Trigger algorithm params
  double fTriggerWindow = 0.;
  int32_t fMinNumDigis  = 0;
  double fDeadTime      = 0.;

  bool SendTriggers(const std::vector<double>& vTriggers, FairMQParts& partsIn);

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
};

#endif /* CBMDEVICETRIGGER_H_ */
