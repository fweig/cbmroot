/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmStsUnpackConfig.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Configuration class for an unpacker algorithm
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This is the common configuration class for unpacking algorithms
 * 
*/

#ifndef CbmStsUnpackConfig_H
#define CbmStsUnpackConfig_H

#include "CbmErrorMessage.h"  // REMARK this should become a Sts specific container I Would propose PR
#include "CbmRecoUnpackConfig.tmpl"
#include "CbmStsDigi.h"
#include "CbmStsUnpackAlgo.h"
#include "CbmStsUnpackMonitor.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmStsUnpackConfig : public CbmRecoUnpackConfig<CbmStsUnpackAlgoBase, CbmStsDigi, CbmErrorMessage> {

public:
  /**
   * @brief Create the Cbm Trd Unpack Task object
   * 
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmStsUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Trd Unpack Task object
   * 
  */
  virtual ~CbmStsUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmStsUnpackConfig(const CbmStsUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmStsUnpackConfig& operator=(const CbmStsUnpackConfig&) = delete;

  struct FebChanMaskReco {
    UInt_t uFeb;
    UInt_t uChan;
    Bool_t bMasked;
  };

  // Getters
  /** @brief Get the potentially added monitor. */
  std::shared_ptr<CbmStsUnpackMonitor> GetMonitor() { return fMonitor; }

  /**
   * @brief Prepare the unpacker to be ready to run.
   * In this function all initialization steps of the unpacker algorithms happen.
  */
  void InitUnpacker();

  void MaskNoisyChannel(UInt_t uFeb, UInt_t uChan, Bool_t bMasked = kTRUE)
  {
    fvChanMasks.emplace_back(FebChanMaskReco {uFeb, uChan, bMasked});
  }


  // Setters
  /**
   * @brief Set the Asic Time Offset
   * 
   * @param asicid Idx of the ASIC with the given time offset
   * @param value time offset
  */
  void SetAsicTimeOffset(size_t asicid, double value)
  {
    if (fvdTimeOffsetNsAsics.size() < (asicid + 1)) fvdTimeOffsetNsAsics.resize(asicid + 1);
    fvdTimeOffsetNsAsics.at(asicid) = value;
  }

  /** @brief Set the minimum adc cut value @param[in] value */
  void SetMinAdcCut(uint32_t value) { fdAdcCut = value; }

  /** @brief Add a monitor to the unpacker. @param value CbmStsUnpackMonitor */
  void SetMonitor(std::shared_ptr<CbmStsUnpackMonitor> value) { fMonitor = value; }


protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   * 
   * @return Bool_t initOk 
  */
  virtual std::shared_ptr<CbmStsUnpackAlgoBase> chooseAlgo();

  /** @brief pointer to the monitor object */
  std::shared_ptr<CbmStsUnpackMonitor> fMonitor = nullptr;

  /** @brief Minimum adc cut to store a hit */
  uint32_t fdAdcCut = 0;

  /** @brief Vector with the Asic time offsets */
  std::vector<double> fvdTimeOffsetNsAsics = {};

  /// Temporary storage of user parameters
  std::vector<FebChanMaskReco> fvChanMasks = {};

  /** @brief Geometry setup tag for the given detector as used by CbmSetup objects */
  std::string fGeoSetupTag = "";

  /** @brief RunId of the current run, if not known 0 is a valid runtime case. Used runId based parameter loading. */
  UInt_t fRunId = 0;

private:
  ClassDef(CbmStsUnpackConfig, 2)
};

#endif  // CbmStsUnpackConfig_H
