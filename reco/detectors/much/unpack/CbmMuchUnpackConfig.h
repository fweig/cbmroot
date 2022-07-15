/* Copyright (C) 2022 Fair GmbH, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/**
 * @file CbmMuchUnpackConfig.h
 * @brief Configuration class for an unpacker algorithm
 * @version 0.1
 * @date 2022-02-22
 *
 * This is the common configuration class for unpacking algorithms
 *
*/

#ifndef CbmMuchUnpackConfig_H
#define CbmMuchUnpackConfig_H

#include "CbmErrorMessage.h"  // REMARK this should become a Much specific container I Would propose PR
#include "CbmMuchDigi.h"
#include "CbmMuchUnpackAlgo.h"
#include "CbmMuchUnpackMonitor.h"
#include "CbmRecoUnpackConfig.tmpl"

#include <FairLogger.h>
#include <Logger.h>

#include <Rtypes.h>
#include <RtypesCore.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class CbmMuchUnpackConfig : public CbmRecoUnpackConfig<CbmMuchUnpackAlgo, CbmMuchDigi, CbmMuchDigi, CbmErrorMessage> {

public:
  /**
   * @brief Create the Cbm Sts Unpack Task object
   *
   * @param geoSetupTag Geometry setup tag for the given detector as used by CbmSetup objects
   * @param runid set if unpacker is rerun on a special run with special parameters
   *@remark We use the string instead of CbmSetup here, to not having to link against sim/steer...
  */
  CbmMuchUnpackConfig(std::string detGeoSetupTag, UInt_t runid = 0);

  /**
   * @brief Destroy the Cbm Sts Unpack Task object
   *
  */
  virtual ~CbmMuchUnpackConfig();

  /** @brief Copy constructor - not implemented **/
  CbmMuchUnpackConfig(const CbmMuchUnpackConfig&) = delete;

  /** @brief Assignment operator - not implemented **/
  CbmMuchUnpackConfig& operator=(const CbmMuchUnpackConfig&) = delete;

  struct FebChanMaskReco {
    UInt_t uFeb;
    UInt_t uChan;
    Bool_t bMasked;
  };

  // Getters
  /** @brief Get the potentially added monitor. */
  std::shared_ptr<CbmMuchUnpackMonitor> GetMonitor() { return fMonitor; }

  /**
   * @brief Initialize the algorithm, should include all steps needing te parameter objects to be present.
   * In this function most initialization steps of the unpacker algorithms happen.
  */
  void InitAlgo();

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

  /** @brief Enable/Disable the duplicate digis rejection, without or with same ADC checks */
  void SetDuplicatesRejection(bool bIn = true, bool bDiffAdc = true)
  {
    fbRejectDuplicateDigis = bIn;
    fbDupliWithoutAdc      = bDiffAdc;
  }

  /** @brief Set the minimum adc cut value @param[in] value */
  void SetMinAdcCut(uint32_t value) { fdAdcCut = value; }

  void SetMinAdcCut(uint32_t febid, uint32_t value) { fdAdcCut_perFeb[febid] = value; }

  /** @brief Add a monitor to the unpacker. @param value CbmMuchUnpackMonitor */
  void SetMonitor(std::shared_ptr<CbmMuchUnpackMonitor> value) { fMonitor = value; }

  /**
    * @brief Sets the name of the parameter file to be used.
    *
    * @param[in] std:string, path should not be included as set in the Config class
  */
  void SetParFileName(std::string sNewName) { fsParFileName = sNewName; }
  void LoadParFileName() { fAlgo->SetParFileName(fsParFileName); }


protected:
  /**
   * @brief Choose the derived unpacker algorithm to be used for the DAQ output to Digi translation. If algo was already set manually by the user this algorithm is used.
   *
   * @return Bool_t initOk
  */
  virtual std::shared_ptr<CbmMuchUnpackAlgo> chooseAlgo();

  /** @brief pointer to the monitor object */
  std::shared_ptr<CbmMuchUnpackMonitor> fMonitor = nullptr;

  /** @brief Minimum adc cut to store a hit */
  uint32_t fdAdcCut = 0;

  /** @brief Minimum adc cut per Feb to store a hit */
  std::map<uint32_t, uint32_t> fdAdcCut_perFeb;

  /** @brief Vector with the Asic time offsets */
  std::vector<double> fvdTimeOffsetNsAsics = {};

  /** @brief Enables the rejection of duplicate digis */
  bool fbRejectDuplicateDigis = false;

  /** @brief If rejecting duplicate digis, enables rejection even if ADC differs*/
  bool fbDupliWithoutAdc = true;

  /// Temporary storage of user parameters
  std::vector<FebChanMaskReco> fvChanMasks = {};

  /// Parameter file name
  std::string fsParFileName = "mMuchPar.par";

private:
  ClassDef(CbmMuchUnpackConfig, 2)
};

#endif  // CbmMuchUnpackConfig_H
