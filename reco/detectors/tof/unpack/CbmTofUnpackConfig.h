/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmTofUnpackConfig.h
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

#ifndef CbmTofUnpackConfig_H
#define CbmTofUnpackConfig_H

#include "CbmErrorMessage.h"
#include "CbmRecoUnpackConfig.tmpl"
#include "CbmTofDigi.h"
#include "CbmTofUnpackAlgo.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmTofUnpackConfig : public CbmRecoUnpackConfig<CbmTofUnpackAlgo, CbmTofDigi, CbmErrorMessage> {

public:
  /**
   * @brief Create the Cbm Tof Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmTofUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Tof Unpack Task object
   *
  */
  virtual ~CbmTofUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmTofUnpackConfig(const CbmTofUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTofUnpackConfig& operator=(const CbmTofUnpackConfig&) = delete;

  // Getters


  /**
   * @brief Prepare the unpacker to be ready to run.
   * In this function all initialization steps of the unpacker algorithms happen.
  */
  void InitUnpacker();

  // Setters

protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   *
   * @return Bool_t initOk
  */
  virtual std::shared_ptr<CbmTofUnpackAlgo> chooseAlgo();

  /** @brief Geometry setup tag for the given detector as used by CbmSetup objects */
  std::string fGeoSetupTag = "";

  /** @brief RunId of the current run, if not known 0 is a valid runtime case. Used runId based parameter loading. */
  UInt_t fRunId = 0;

private:
  ClassDef(CbmTofUnpackConfig, 1)
};

#endif  // CbmTofUnpackConfig_H
