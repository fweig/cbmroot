/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmRichUnpackConfig.h
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

#ifndef CbmRichUnpackConfig_H
#define CbmRichUnpackConfig_H

#include "CbmRecoUnpackConfig.tmpl"
#include "CbmRichDigi.h"
#include "CbmRichUnpackAlgo.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmRichUnpackConfig : public CbmRecoUnpackConfig<CbmRichUnpackAlgo, CbmRichDigi, std::nullptr_t, std::nullptr_t> {

public:
  /**
   * @brief Create the Cbm Trd Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmRichUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Trd Unpack Task object
   *
  */
  virtual ~CbmRichUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmRichUnpackConfig(const CbmRichUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmRichUnpackConfig& operator=(const CbmRichUnpackConfig&) = delete;

  // Getters


  /**
   * @brief Initialize the algorithm, should include all steps needing te parameter objects to be present.
   * In this function most initialization steps of the unpacker algorithms happen.
  */
  void InitAlgo();

  // Setters

  void MaskDiRICH(Int_t DiRICH) { fMaskedDiRICHes.push_back(DiRICH); }

protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   *
   * @return Bool_t initOk
  */
  virtual std::shared_ptr<CbmRichUnpackAlgo> chooseAlgo();

  std::vector<Int_t> fMaskedDiRICHes = {};

private:
  ClassDef(CbmRichUnpackConfig, 3)
};

#endif  // CbmRichUnpackConfig_H
