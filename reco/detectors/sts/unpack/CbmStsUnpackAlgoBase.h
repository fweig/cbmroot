/* Copyright (C) 2010 - 2021 Fair GmbH 
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith */

/**
 * @file CbmStsUnpackAlgoBase.h
 * @author Dominik Smith (d.smith@gsi.de)
 * @brief Baseclass for the Sts unpacker algorithms
 * @version 0.1
 * @date 2021-04-21
 * 
 * @copyright Copyright (c) 2021
 * 
 * This is the base class for the algorithmic part of the tsa data unpacking 
 * processes of the CbmSts.
 * The actual translation from tsa to digi happens in the derived classes. 
 * 
 * 
*/

#ifndef CbmStsUnpackAlgoBase_H
#define CbmStsUnpackAlgoBase_H

#include "CbmErrorMessage.h"  // REMARK see remark in CbmStsUnpackConfig
#include "CbmRecoUnpackAlgo.tmpl"
#include "CbmStsDigi.h"
#include "CbmStsUnpackMonitor.h"

#include "Timeslice.hpp"  // timeslice

#include <FairTask.h>  // for InitStatus

#include <Rtypes.h>  // for types
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

class CbmStsUnpackAlgoBase : public CbmRecoUnpackAlgo<CbmStsDigi, CbmErrorMessage> {
public:
  /** @brief Create the Cbm Sts Unpack AlgoBase object */
  CbmStsUnpackAlgoBase(std::string name);

  /** @brief Destroy the Cbm Sts Unpack Task object */
  virtual ~CbmStsUnpackAlgoBase();

  /** @brief Copy constructor - not implemented **/
  CbmStsUnpackAlgoBase(const CbmStsUnpackAlgoBase&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmStsUnpackAlgoBase& operator=(const CbmStsUnpackAlgoBase&) = delete;

  // Setters
  /** @brief Set a predefined monitor @param monitor predefined unpacking monitor */
  void SetMonitor(std::shared_ptr<CbmStsUnpackMonitor> monitor) { fMonitor = monitor; }

  /** @brief Set the minimum adc cut value @param[in] value */
  void SetMinAdcCut(uint32_t value) { fdAdcCut = value; }

  /** @brief Set the time offset per Asic */
  void SetAsicTimeOffsetVec(std::vector<double> value) { fvdTimeOffsetNsAsics.swap(value); }

  /** @brief Set the time offset per Asic */
  void SetTimeOffsetNsAsic(const uint32_t uAsicIdx, const double dOffsetIn = 0.0);

  /**
   * @brief Get the requested parameter containers. To be defined in the derived classes!
   * Return the required parameter containers together with the paths to the ascii
   * files to.
   *
   * @param[in] std::string geoTag as used in CbmSetup
   * @param[in] std::uint32_t runId for runwise defined parameters
   * @return fParContVec
  */
  virtual std::vector<std::pair<std::string, std::shared_ptr<FairParGenericSet>>>*
  GetParContainerRequest(std::string geoTag, std::uint32_t runId);

  /**
   * @brief Mask a Noisy Channel
   *
   * @param uFeb
   * @param uChan
   * @param bMasked
  */
  virtual void MaskNoisyChannel(const uint32_t uFeb, const uint32_t uChan, const bool bMasked = true)
  {
    (void) uFeb;
    (void) uChan;
    (void) bMasked;
  };

protected:
  // Monitoring
  /** @brief Potential (online) monitor for the unpacking process */
  std::shared_ptr<CbmStsUnpackMonitor> fMonitor = nullptr;

  /** @brief Minimum adc cut to store a hit */
  uint32_t fdAdcCut = 0;

  /** @brief Time offsets per Asic??? @todo expert confirmation required */
  std::vector<double> fvdTimeOffsetNsAsics = {};

private:
  ClassDef(CbmStsUnpackAlgoBase, 2)
};

#endif  // CbmStsUnpackAlgoBase_H
