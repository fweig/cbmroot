/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Dominik Smith [committer] */

/**
 * CbmDevUnpack.h
 *
 * @since 2020-05-04
 * @author P.-A. Loizeau
 */

#ifndef CBMDEVUNPACK_H
#define CBMDEVUNPACK_H

#include "CbmMqTMessage.h"
#include "CbmTsEventHeader.h"

#include "Timeslice.hpp"

#include "FairMQDevice.h"
#include "FairParGenericSet.h"

#include "Rtypes.h"
#include "TObjArray.h"

#include <chrono>
#include <map>
#include <vector>

#include "StsReadoutConfig.h"
#include "UnpackSts.h"

class TimesliceMetaData;
struct CbmDigiTimeslice;

class CbmDevUnpack : public FairMQDevice {
public:
  CbmDevUnpack();
  virtual ~CbmDevUnpack();

private:
  std::map<uint16_t, cbm::algo::UnpackSts> fAlgoSts = {};
  cbm::algo::StsReadoutConfig fStsConfig {};

  /// message queues
  std::string fsChannelNameDataInput  = "ts-request";
  std::string fsChannelNameDataOutput = "unpts_0";
  std::string fsChannelNameCommands   = "commands";

  /// Statistics & first TS rejection
  size_t fNumMessages = 0;
  size_t fNumTs       = 0;

  /// Pointer to the Timeslice header conatining start time and index
  CbmTsEventHeader* fCbmTsEventHeader = nullptr;

  /** @brief Read command line parameters for MQ device */
  virtual void InitTask();

  /** @brief Called by run loop, does init steps on first TS */
  bool ConditionalRun();

  /** @brief Initialize runtime parameters for UnpackSts algos */
  bool InitAlgos();

  /**
   * @brief Unpack a single timeslice 
   * @param ts Input FLES timeslice
  */
  CbmDigiTimeslice DoUnpack(const fles::Timeslice& ts);

  /** @brief Serialize unpacked digi timeslice and send to output channel */
  bool SendData(const CbmDigiTimeslice& timeslice, const TimesliceMetaData& TsMetaData);
};

#endif /* CBMDEVUNPACK_H */
