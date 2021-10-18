/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmPsdUnpackConfig.h
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

#ifndef CbmPsdUnpackConfig_H
#define CbmPsdUnpackConfig_H

#include "CbmPsdDigi.h"
#include "CbmPsdUnpackAlgo.h"
#include "CbmRecoUnpackConfig.tmpl"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmPsdUnpackConfig : public CbmRecoUnpackConfig<CbmPsdUnpackAlgo, CbmPsdDigi, CbmPsdDsp> {

public:
  /**
   * @brief Create the Cbm Trd Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmPsdUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Trd Unpack Task object
   *
  */
  virtual ~CbmPsdUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmPsdUnpackConfig(const CbmPsdUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmPsdUnpackConfig& operator=(const CbmPsdUnpackConfig&) = delete;

  // Getters


  // Setters

protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   *
   * @return Bool_t initOk
  */
  virtual std::shared_ptr<CbmPsdUnpackAlgo> chooseAlgo();

private:
  ClassDef(CbmPsdUnpackConfig, 3)
};

#endif  // CbmPsdUnpackConfig_H
