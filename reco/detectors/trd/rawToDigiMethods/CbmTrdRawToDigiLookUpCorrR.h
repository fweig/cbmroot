/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmTrdRawToDigiLookUpCorrR.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Class for extracting information from raw signals to digi level
 * @version 0.1
 * @date 2021-02-18
 * 
 * @copyright Copyright (c) 2021
 *
 * This implements a look up table based method to extract the input charge
 * for a given Spadic response. The look up is based on the correlation of two 
 * selected samples.
 * It is the legacy version of the original CbmTrdRawToDigi class.
 *
 */

#ifndef CbmTrdRawToDigiLookUpCorrR_H
#define CbmTrdRawToDigiLookUpCorrR_H

#include "CbmTrdRawToDigiBaseR.h"

#include <Rtypes.h>  // for types and classdef
#include <RtypesCore.h>
#include <TH2.h>
#include <TObject.h>  // for TObject inheritance

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

class CbmTrdRawToDigiLookUpCorrR : public CbmTrdRawToDigiBaseR {


public:
  enum class eLookUpModes : size_t
  {
    /**
     * @brief Look up mode based on two samples and statistical filling.
     * Look up mode based on two samples with maps based on statistical extraction 
     * stored in the passed infile.
     * 
     */
    kTwoSamplesFileInput = 0,

    /**
     * @brief Look up mode based on two samples and analytical on the fly filling.
     * Look up mode based on two samples with maps based on analytical filling 
     * created dynamically when this mode is selected.
     */
    kTwoSamplesDynamicAnalytic
  };

  /**
   * @brief Construct a new CbmTrdRawToDigiLookUpCorrR
   * 
   * @param infile 
   */
  CbmTrdRawToDigiLookUpCorrR(std::string infile = "", eLookUpModes mode = eLookUpModes::kTwoSamplesFileInput);

  /**
   * @brief Copy constructor - not implemented!
   * 
   */
  CbmTrdRawToDigiLookUpCorrR(const CbmTrdRawToDigiLookUpCorrR&) = delete;

  /**
   * @brief Assignment operator - not implemented!
   * 
   * @return CbmTrdRawToDigiLookUpCorrR 
   */
  CbmTrdRawToDigiLookUpCorrR operator=(const CbmTrdRawToDigiLookUpCorrR&);

  /** @brief Destructor **/
  virtual ~CbmTrdRawToDigiLookUpCorrR() { ; }

  /**
   * @brief Initialise the method after all settings have been passed
   * 
   * This initialises the method if none default settings are used.
   * If it is not call after a change of settings, the defaults will still be used!
   *
   * @param infile
   * @return true
   * @return false
   */
  bool Init(std::string infile = "");

  /**
   * @brief Create the Debug Histo File 
   * If called, the debug histos are written to a file which is written to disk.
   */
  void CreateDebugHistoFile();

  /**
   * @brief Get the Bin Time Shift value 
   * 
   * Extract the bin time shift from the look up tables. The value is also stored 
   * in fCurrentTimeshift.
   * @param samples 
   * @return ULong64_t 
   */
  ULong64_t GetBinTimeShift(const std::vector<std::int16_t>* samples);

  /**
   * @brief Get the MaxAdc value
   * 
   * @param samples 
   * @return Float_t 
   */
  Float_t GetMaxAdcValue(const std::vector<std::int16_t>* samples);

  /**
   * @brief Get the defined Look Up Mode 
   * 
   * @return eLookUpModes 
   */
  eLookUpModes GetLookUpMode() { return fLookUpMode; }

  /**
   * @brief Get the First Sample Pos
   * 
   * @return UInt_t 
   */
  UInt_t GetFirstSamplePos() { return fFirstLookUpSamplePos; }

  /**
   * @brief Get the Second Sample Pos
   * 
   * @return UInt_t 
   */
  UInt_t GetSecondSamplePos() { return fSecondLookUpSamplePos; }

  /**
   * @brief Set the Look Up Mode
   * 
   * @param value 
   */
  void SetLookUpMode(eLookUpModes value, std::string infile = "")
  {
    fLookUpMode = value;
    prepareLookUpTables(infile);
  }

  /**
   * @brief Set the position of the first look up sample
   * 
   * @param value 
   */
  void SetFirstSamplePos(UInt_t value) { fFirstLookUpSamplePos = value; }

  /**
   * @brief Set the position of the second look up sample
   * 
   * @param value 
   */
  void SetSecondSamplePos(UInt_t value) { fSecondLookUpSamplePos = value; }

  /**
   * @brief Set the Nr Of Presamples also resets the look up sample positions
   * 
   * @param value 
   */
  virtual void SetNrOfPresamples(UInt_t value)
  {
    fNrOfPresamples = value;
    SetFirstSamplePos(value + 1);
    SetSecondSamplePos(value + 2);
  }


private:
protected:
  /**
   * @brief Prepare the look up tables for the used look up mode
   * 
   * @param infile 
   */
  void prepareLookUpTables(std::string infile);

  /**
   * @brief Load the look up tables from the given file
   * 
   * @param infile 
   */
  void loadLookUpTables(std::string infile);

  /**
   * @brief Create a Analytical Look Up Tables
   * 
   * Two of three look up modes are based on dynamically created look up tables 
   * based on analytical approaches.
   */
  void createAnalyticLookUpTables();

  /**
   * @brief Used look up mode
   * 
   */
  eLookUpModes fLookUpMode;


  /**
   * @brief Look up map for the max adc values for 
   * Keys correspond to charge[fCurrentTimeshift],
   * charge[fSecondLookUpSamplePos] and the mapped value is the input charge in 
   * MIP calibrated max adc units
   */
  std::map<Int_t, std::map<Int_t, Float_t>> fMaxAdcLookUpMap = {};

  /**
   * @brief Look up map for the bin time shift value
   * Keys correspond to charge[fFirstLookUpSamplePos],
   * charge[fSecondLookUpSamplePos] and the mapped value is the timeshift
   */
  std::map<UInt_t, std::map<UInt_t, ULong64_t>> fTimeshiftLookUpMap = {};

  /**
   * @brief Position of the first sample used in the look up tables
   * Default corresponds to the value used in the look ups in the parameters 
   * repository
   */
  UInt_t fFirstLookUpSamplePos = fNrOfPresamples + 1;

  /**
   * @brief Position of the second sample used in the look up tables
   * Default corresponds to the value used in the look ups in the parameters 
   * repository
   */
  // UInt_t fSecondLookUpSamplePos = fNrOfPresamples + 2;
  UInt_t fSecondLookUpSamplePos = fNrOfPresamples + 5;

  /**
   * @brief Baseline corrected sample value of the first used sample
   * 
   */
  Int_t fcorrFirstSample = 0;

  /**
   * @brief Baseline corrected sample value of the second used sample
   * 
   */
  Int_t fcorrSecondSample = 0;

  /**
   * @brief File name where the look up histos are stored
   * 
   */
  std::string fLookUpFilename = "";


public:
  ClassDef(CbmTrdRawToDigiLookUpCorrR, 2);
};

#endif
