/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

/**
 * @file CbmBmonUnpackConfig.h
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

#ifndef CbmBmonUnpackConfig_H
#define CbmBmonUnpackConfig_H

#include "CbmErrorMessage.h"
#include "CbmTofDigi.h"
#include "CbmTofUnpackAlgo.h"
#include "CbmTofUnpackConfig.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmBmonUnpackConfig : public CbmTofUnpackConfig {

public:
  /**
   * @brief Create the Cbm Tof Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmBmonUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Tof Unpack Task object
   *
  */
  virtual ~CbmBmonUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmBmonUnpackConfig(const CbmBmonUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmBmonUnpackConfig& operator=(const CbmBmonUnpackConfig&) = delete;

  // Getters

  /**
   * @brief Register the output vectors (if needed) to the FairRoot Manager)
   * FIXME: To be removed when the T0/BMON will have its own Digi class properly exposing the right container name
   *
  */
  void RegisterOutput(FairRootManager* ioman);


  // Setters

protected:
private:
  ClassDef(CbmBmonUnpackConfig, 1)
};

#endif  // CbmBmonUnpackConfig_H
