/* Copyright (C) 2010 - 2021 Goethe-University Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig */

/**
 * @file CbmTrdRawToDigiFitR.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Class for extracting information from raw signals to digi level
 * @version 0.1
 * @date 2021-02-18
 * 
 * @copyright Copyright (c) 2021
 *
 * This implemented a fit method to extract the input charge for a given 
 * Spadic response
 *
 */

#ifndef CbmTrdRawToDigiFitR_H
#define CbmTrdRawToDigiFitR_H

#include "CbmTrdRawToDigiBaseR.h"

#include <Rtypes.h>  // for types and classdef
#include <RtypesCore.h>
#include <TObject.h>  // for TObject inheritance

#include <cstddef>
#include <cstdint>

class CbmTrdRawToDigiFitR : public CbmTrdRawToDigiBaseR {


public:
  /** @brief default Constructor with messages
   **/
  CbmTrdRawToDigiFitR();

  /**
   * @brief Copy constructor - not implemented!
   * 
   */
  CbmTrdRawToDigiFitR(const CbmTrdRawToDigiFitR&) = delete;

  /**
   * @brief Assignment operator - not implemented!
   * 
   * @return CbmTrdRawToDigiFitR 
   */
  CbmTrdRawToDigiFitR operator=(const CbmTrdRawToDigiFitR&);

  /** @brief Destructor **/
  virtual ~CbmTrdRawToDigiFitR() { ; }

  /**
   * @brief Get the Bin Time Shift value 
   * 
   * @param samples 
   * @return ULong64_t 
   */
  ULong64_t GetBinTimeShift(const std::vector<std::int16_t>* /*samples*/);

  /**
   * @brief Get the MaxAdc value
   * 
   * @param samples 
   * @return Float_t 
   */
  Float_t GetMaxAdcValue(const std::vector<std::int16_t>* /*samples*/);

  /**
   * @brief Get the Response Function object
   * 
   * @return std::shared_ptr<TF1> 
   */
  std::shared_ptr<TF1> GetResponseFunc() { return fResponseFunc; }

  /**
   * @brief Set the Nr Of Presamples
   * 
   * @param value 
   */
  virtual void SetNrOfPresamples(UInt_t value)
  {
    fNrOfPresamples = value;
    fResponseFunc->FixParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kNrPresamples), fNrOfPresamples);
  }

  /**
   * @brief Set the Shaping Order
   * 
   * @param value 
   */
  virtual void SetShapingOrder(std::uint8_t value)
  {
    fResponseFunc->FixParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kShapingOrder), value);
  }

  /**
   * @brief Set and fix the Shaping Time 
   * 
   * @param value 
   */
  virtual void SetShapingTime(Double_t value)
  {
    fResponseFunc->FixParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kShapingTime), value);
  }

  /**
   * @brief Set and fix the Bin Timeshift value
   * 
   * @param value 
   */
  virtual void SetBinTimeshift(Double_t value)
  {
    fResponseFunc->FixParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kBinTimeshift), value);
  }

  /**
   * @brief Set the Charge Calibration 
   * 
   * @param value 
   */
  virtual void SetChargeToMaxAdcCal(Double_t value)
  {
    fResponseFunc->FixParameter(static_cast<size_t>(CbmTrdSpadic::eResponsePars::kChargeToMaxAdcCal), value);
  }

  /**
   * @brief Set the Fit Range
   * 
   * @param start 
   * @param end 
   */
  void SetFitRange(UInt_t start, UInt_t end)
  {
    fFitRangeStart = start;
    fFitRangeEnd   = end;
  }


private:
  /**
  * @brief Perform the fit of the input signal
  * 
  * @param samples 
  */
  void fitResponse(const std::vector<std::int16_t>* samples);

  /**
   * @brief Response function
   *
   * Spadic obejct internal response function.
   * Use the Getter to access the function and set/fix/etc. parameters.
   * 
   */
  std::shared_ptr<TF1> fResponseFunc = CbmTrdSpadic::GetResponseFunc();

  /**
   * @brief Fix the extraction parameters to the default values
   *
   * Fix NrPresamples, ShapingOrder and ShapingTime to the default values
   * Can still be overwritten by the setter functions
   */
  void fixExtractionPars();

  /**
   * @brief Fix the passed extraction parameter (wrapper function)
   *
   */
  void fixExtractionPar(CbmTrdSpadic::eResponsePars ipar);

  // Data member

  /**
   * @brief First sample that is used for the fit
   * 
   */
  UInt_t fFitRangeStart = fNrOfPresamples;

  /**
   * @brief Last sample that is used for the fit
   * 
   */
  UInt_t fFitRangeEnd = 10;


protected:
public:
  ClassDef(CbmTrdRawToDigiFitR, 2);
};

#endif
