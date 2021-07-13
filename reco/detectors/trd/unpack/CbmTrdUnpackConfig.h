/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmTrdUnpackConfig.h
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

#ifndef CbmTrdUnpackConfig_H
#define CbmTrdUnpackConfig_H


#include "CbmRecoUnpackConfig.tmpl"
#include "CbmTrdRawToDigiBaseR.h"
#include "CbmTrdRawToDigiMaxAdcR.h"
#include "CbmTrdUnpackAlgoBaseR.h"
#include "CbmTrdUnpackMonitor.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmTrdUnpackConfig :
  public CbmRecoUnpackConfig<CbmTrdUnpackAlgoBaseR, CbmTrdDigi, CbmTrdRawMessageSpadic, std::pair<size_t, size_t>> {

public:
  /**
   * @brief Create the Cbm Trd Unpack Task object
   * 
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmTrdUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Trd Unpack Task object
   * 
  */
  virtual ~CbmTrdUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackConfig(const CbmTrdUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackConfig& operator=(const CbmTrdUnpackConfig&) = delete;

  // Getters
  /** @brief Get the potentially added monitor. */
  std::shared_ptr<CbmTrdUnpackMonitor> GetMonitor() { return fMonitor; }

  /** @brief Get the spadic object attached to this config. @return CbmTrdSpadic */
  std::shared_ptr<CbmTrdSpadic> GetSpadicObject() { return fSpadic; }

  /**
   * @brief Prepare the unpacker to be ready to run.
   * In this function all initialization steps of the unpacker algorithms happen.
  */
  void InitUnpacker();

  // Setters

  /** @brief Add a monitor to the unpacker. @param value CbmTrdUnpackMonitor */
  void SetMonitor(std::shared_ptr<CbmTrdUnpackMonitor> value) { fMonitor = value; }

  /**
   * @brief Set the raw to digi method
   * 
   * @param value 
  */
  void SetRawToDigiMethod(std::shared_ptr<CbmTrdRawToDigiBaseR> value) { fRTDMethod = value; }

  /**
   * @brief Set the Spadic Object
   * 
   * @param value 
  */
  void SetSpadicObject(std::shared_ptr<CbmTrdSpadic> value) { fSpadic = value; }

protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   * 
   * @return Bool_t initOk 
  */
  virtual std::shared_ptr<CbmTrdUnpackAlgoBaseR> chooseAlgo();

  /** @brief pointer to the raw msg to digi method */
  std::shared_ptr<CbmTrdRawToDigiBaseR> fRTDMethod = std::make_shared<CbmTrdRawToDigiMaxAdcR>();

  /** @brief Spadic software reprensentation object */
  std::shared_ptr<CbmTrdSpadic> fSpadic = nullptr;

  /** @brief pointer to the monitor object */
  std::shared_ptr<CbmTrdUnpackMonitor> fMonitor = nullptr;

  /** @brief Geometry setup tag for the given detector as used by CbmSetup objects */
  std::string fGeoSetupTag = "";

  /** @brief RunId of the current run, if not known 0 is a valid runtime case. Used runId based parameter loading. */
  UInt_t fRunId = 0;

private:
  ClassDef(CbmTrdUnpackConfig, 2)
};

#endif  // CbmTrdUnpackConfig_H
