/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */


/**
 * @file CbmTrdRawToDigiBaseR.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Base class for extracting of information from raw signals to digi level
 * @version 0.1
 * @date 2021-02-18
 * 
 * @copyright Copyright (c) 2021
 *
 * Abstract base class for specified extraction methods of input charge and in-bin 
 * time-shifts. The actual methods are implemented in the derived classes.
 *
 */

#ifndef CBMTRDRAWTODIGIBASER_H
#define CBMTRDRAWTODIGIBASER_H

#include "CbmTrdDigi.h"  // for MakeDigi
#include "CbmTrdRawMessageSpadic.h"
#include "CbmTrdSpadic.h"

#include <Rtypes.h>  // for types and classdef
#include <RtypesCore.h>
#include <TObject.h>  // for TObject inheritance

#include <cstdint>
#include <memory>  // for shared_ptr

class CbmTrdRawToDigiBaseR : public TObject {
public:
  /**
   * @brief Construct a new CbmTrdRawToDigiBaseR object
   * 
   */
  CbmTrdRawToDigiBaseR();

  /** @brief Destructor **/
  virtual ~CbmTrdRawToDigiBaseR() { ; }


  /**
   * @brief Get digi from input values. Implemented in the derived classes.
   *
   * Get digi from the input values. The methods how a timeshift within the bins 
   * is estimated and how the input charge to the asic is extracted are 
   * implemented in the derived classes
   *
   * @param samples Adc samples
   * @param padChNr Channel number of the pad
   * @param uniqueModuleId unique Id of the module
   * @param time   Absolute time [ns].
   * @param triggerType SPADIC trigger type see CbmTrdTriggerType.
   * @param errClass SPADIC signal error parametrization based on message type.
   * @return std::shared_ptr<CbmTrdDigi> *
   */

  std::unique_ptr<CbmTrdDigi> MakeDigi(const std::vector<std::int16_t>* samples, Int_t padChNr, Int_t uniqueModuleId,
                                       ULong64_t time, CbmTrdDigi::eTriggerType triggerType, Int_t errClass);

  /**
   * @brief Get the Bin Time Shift value 
   * 
   * @param samples 
   * @return ULong64_t 
   */
  virtual ULong64_t GetBinTimeShift(const std::vector<std::int16_t>* samples) = 0;

  /**
   * @brief Get the MaxAdc value
   * 
   * @param samples 
   * @return Float_t 
   */
  virtual Float_t GetMaxAdcValue(const std::vector<std::int16_t>* samples) = 0;

  /**
   * @brief Get the Baseline value
   * 
   * The digi charge is an unsigned. Hence, we need to get the baseline to 0
   *
   * @param samples 
   * @return Float_t 
   */
  Float_t GetBaseline(const std::vector<std::int16_t>* samples);

  /**
   * @brief Get the spadic class
   * 
   * @return std::shared_ptr<CbmTrdSpadic> 
   */
  std::shared_ptr<CbmTrdSpadic> GetSpadicObject() { return fSpadic; }

  /**
   * @brief Get the Nr Of Presamples
   * 
   * @return UInt_t 
   */
  UInt_t GetNrOfPresamples() { return fNrOfPresamples; }

  /**
   * @brief Get the Digi Trigger Type from the raw message triggertype
   * 
   * @param tt 
   * @return CbmTrdDigi::eTriggerType 
  */
  static CbmTrdDigi::eTriggerType GetDigiTriggerType(Spadic::eTriggerType tt);

  /**
   * @brief Set the Nr Of Presamples
   * 
   * @param value 
   */
  virtual void SetNrOfPresamples(UInt_t value) { fNrOfPresamples = value; }


  /**
   * @brief Set the spadic class
   * 
   * @param spadic
   */
  void SetSpadicObject(std::shared_ptr<CbmTrdSpadic> spadic) { fSpadic = spadic; }

private:
  /**
   * @brief Copy constructor - not implemented!
   * 
   */
  CbmTrdRawToDigiBaseR(const CbmTrdRawToDigiBaseR&) = delete;

protected:
  /**
   * @brief Number of presamples before the signal starts (SPADIC default 2)
   * 
   */
  UInt_t fNrOfPresamples = CbmTrdSpadic::GetNrOfPresamples();

  /**
   * @brief Bin timeshift for the current sample set in [ns]
   * 
   */
  ULong64_t fCurrentTimeshift = 0;

  std::shared_ptr<CbmTrdSpadic> fSpadic = std::make_shared<CbmTrdSpadic>();

public:
  ClassDef(CbmTrdRawToDigiBaseR, 2);
};

#endif
