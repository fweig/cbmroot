/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmTrdUnpackConfigFasp2D.h
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

#ifndef CbmTrdUnpackConfigFasp2D_H
#define CbmTrdUnpackConfigFasp2D_H


#include "CbmRecoUnpackConfig.tmpl"
#include "CbmTrdDigi.h"
#include "CbmTrdUnpackAlgoFasp2D.h"
#include "CbmTrdParFasp.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <map>

class CbmTrdUnpackConfigFasp2D : public CbmRecoUnpackConfig<CbmTrdUnpackAlgoFasp2D, CbmTrdDigi> {

public:
  /**
   * @brief Create the Cbm Trd Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmTrdUnpackConfigFasp2D(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Trd Unpack Task object
   *
  */
  virtual ~CbmTrdUnpackConfigFasp2D();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackConfigFasp2D(const CbmTrdUnpackConfigFasp2D&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackConfigFasp2D& operator=(const CbmTrdUnpackConfigFasp2D&) = delete;

  /**
   * @brief Initialize the algorithm, include all calibration for Trd FASP.
  */
  void InitAlgo();

  /** @brief define fasp mapping for each module
   * @param modAddress module address according to geometry
   * @param faspMap mapped ids of FASP ASICs for module
   */
  void SetFaspMapping(int modAddress, uint8_t faspMap[NFASPMOD]);
protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   *
   * @return Bool_t initOk
  */
  virtual std::shared_ptr<CbmTrdUnpackAlgoFasp2D> chooseAlgo();

private:
  std::map<uint32_t, uint8_t[NFASPMOD]> fFaspMap;  ///> DAQ packing of FASP id
  
  ClassDef(CbmTrdUnpackConfigFasp2D, 4)
};

#endif  // CbmTrdUnpackConfigFasp2D_H
