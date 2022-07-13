/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer], Alexandru Bercuci*/

/**
 * @file CbmTrdUnpackFaspConfig.h
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

#ifndef CbmTrdUnpackFaspConfig_H
#define CbmTrdUnpackFaspConfig_H


#include "CbmRecoUnpackConfig.tmpl"
#include "CbmTrdDigi.h"
#include "CbmTrdParFasp.h"
#include "CbmTrdUnpackFaspAlgo.h"
#include "CbmTrdUnpackFaspMonitor.h"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

class CbmTrdUnpackFaspConfig : public CbmRecoUnpackConfig<CbmTrdUnpackFaspAlgo, CbmTrdDigi> {

public:
  /**
   * @brief Create the Cbm Trd Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmTrdUnpackFaspConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Trd Unpack Task object
   *
  */
  virtual ~CbmTrdUnpackFaspConfig();

  /** @brief Copy constructor - not implemented **/
  CbmTrdUnpackFaspConfig(const CbmTrdUnpackFaspConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmTrdUnpackFaspConfig& operator=(const CbmTrdUnpackFaspConfig&) = delete;

  /** @brief Get the monitor. */
  std::shared_ptr<CbmTrdUnpackFaspMonitor> GetMonitor() { return fMonitor; }

  /** @brief Initialize the algorithm, include all calibration for Trd FASP.*/
  void InitAlgo();

  /** @brief define fasp mapping for each module
   * @param modAddress module address according to geometry
   * @param faspMap mapped ids of FASP ASICs for module
   */
  void SetFaspMapping(int modAddress, uint8_t faspMap[NFASPMOD]);
  /** @brief define crob id mapping for each module
   * @param modAddress module address according to geometry
   * @param crobMap mapped ids of CROBs eq_id for module
   */
  void SetCrobMapping(int modAddress, uint16_t crobMap[NFASPMOD]);

  /** @brief Add a monitor to the unpacker. 
   *  @param value CbmTrdUnpackFaspMonitor */
  void SetMonitor(std::shared_ptr<CbmTrdUnpackFaspMonitor> value) { fMonitor = value; }

protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   *
   * @return Bool_t initOk
  */
  virtual std::shared_ptr<CbmTrdUnpackFaspAlgo> chooseAlgo();

private:
  std::map<uint32_t, uint8_t[NFASPMOD]> fFaspMap;   ///> Module address to FASP id mapping
  std::map<uint32_t, uint16_t[NCROBMOD]> fCrobMap;  ///> Module address to CROB id mapping
  /** @brief pointer to the monitor object */
  std::shared_ptr<CbmTrdUnpackFaspMonitor> fMonitor = nullptr;

  ClassDef(CbmTrdUnpackFaspConfig, 5)
};

#endif  // CbmTrdUnpackFaspConfig_H
