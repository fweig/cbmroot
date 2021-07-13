/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmTrdRawToDigiMaxAdcR.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Class for extracting information from raw signals to digi level
 * @version 0.1
 * @date 2021-02-18
 * 
 * @copyright Copyright (c) 2021
 *
 * This implemented the max adc method to extract the input charge for a given 
 * Spadic response
 *
 */

#ifndef CbmTrdRawToDigiMaxAdcR_H
#define CbmTrdRawToDigiMaxAdcR_H

#include "CbmTrdDigi.h"  // for MakeDigi
#include "CbmTrdRawToDigiBaseR.h"

#include <Rtypes.h>  // for types and classdef
#include <RtypesCore.h>
#include <TObject.h>  // for TObject inheritance

#include <cstddef>

class CbmTrdRawToDigiMaxAdcR : public CbmTrdRawToDigiBaseR {


public:
  /** @brief default Constructor with messages
   **/
  CbmTrdRawToDigiMaxAdcR();

  /**
   * @brief Copy constructor - not implemented!
   * 
   */
  CbmTrdRawToDigiMaxAdcR(const CbmTrdRawToDigiMaxAdcR&) = delete;

  /**
   * @brief Assignment operator - not implemented!
   * 
   * @return CbmTrdRawToDigiMaxAdcR 
   */
  CbmTrdRawToDigiMaxAdcR operator=(const CbmTrdRawToDigiMaxAdcR&);

  /** @brief Destructor **/
  virtual ~CbmTrdRawToDigiMaxAdcR() { ; }

  /**
   * @brief Get the Bin Time Shift value 
   * 
   * No method developed up to now to extract a time-shift together with the max 
   * adc charge esxtraction method.
   * @param samples 
   * @return ULong64_t 
   */
  ULong64_t GetBinTimeShift(const std::vector<std::int16_t>* /*samples*/) { return 0; };

  /**
   * @brief Get the MaxAdc value
   * 
   * @param samples 
   * @return Float_t 
   */
  Float_t GetMaxAdcValue(const std::vector<std::int16_t>* samples);

  /**
   * @brief Set the peaking range in sample numbers
   * 
   * Use this to define a peaking range for the max adc extraction.
   * Presamples have to be included into the values!
   * @param value 
   */
  void SetPeakingRange(size_t min, size_t max)
  {
    fPeakingBinMin = min;
    fPeakingBinMax = max;
  }


private:
  /**
   * @brief First sample to look for the max adc
   * 
   */
  size_t fPeakingBinMin = fNrOfPresamples;

  /**
   * @brief Last sample to look for the max adc
   * Default value is set based on the Shaping time + 5 samples safety margin.
   * @remark the peaking time strongly depends on the input signal. Effective range of 
   * the shaping time is between 120 and 240 ns.
   */
  size_t fPeakingBinMax =
    static_cast<size_t>(CbmTrdSpadic::GetShapingTime() / CbmTrdSpadic::GetClockCycle() + fNrOfPresamples + 5);


protected:
public:
  ClassDef(CbmTrdRawToDigiMaxAdcR, 2);
};

#endif
