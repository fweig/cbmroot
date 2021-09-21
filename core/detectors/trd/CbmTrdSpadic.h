/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmTrdSpadic.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Software representation of the SPADIC v2.2+
 * @version 0.1
 * @date 2021-02-15
 * @todo include parameter loading and usage for all object dependent response 
 * functions.
 *
 * 
 */

#ifndef CBMTRDSPADIC_H
#define CBMTRDSPADIC_H

#include "CbmTrdDefs.h"  // for MIP dEdx value
#include "CbmTrdDigi.h"  // for ClockCycle
#include "CbmTrdParSpadic.h"
#include "CbmTrddEdxUtils.h"  // for MIP dEdx

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, Option_t
#include <TF1.h>         // for response functions
#include <TObject.h>     // for TObject inheritence

#include <cstdint>
#include <vector>

#include <MessageTypes.h>
#include <cmath>

class CbmTrdSpadic {

public:
  /**
   * @brief Construct a new CbmTrdSpadic object
   * Default and standard c'tor for a CbmTrdSpadic object
   * 
   */
  CbmTrdSpadic();

  /**
   * @brief Copy construct a new CbmTrdSpadic object (not implemented!)
   * 
   */
  CbmTrdSpadic(const CbmTrdSpadic&) = delete;

  /**
   * @brief Assignment of CbmTrdSpadic objects (not implemented!)
   * 
   * @return CbmTrdSpadic& 
   */
  CbmTrdSpadic& operator=(const CbmTrdSpadic&);

  /**
   * @brief Destroy the Cbm Trd Spadic object
   * 
   */
  virtual ~CbmTrdSpadic();


  /**
   * @brief enum for response parameters
   * 
   */
  enum class eResponsePars : size_t
  {
    kShapingTime = 0,
    kShapingOrder,
    kInputCharge,
    kBinTimeshift,
    kNrPresamples,
    kChargeToMaxAdcCal,
    kMipCal
  };
  /**
   * @brief Get the Parameter value
   * 
   * @param partype 
   * @return Double_t 
   */
  Double_t GetParameter(eResponsePars partype) { return fvecResponsePars[static_cast<size_t>(partype)]; }

  static UInt_t GetNrChannels() { return NSPADICCH; }

  /**
   * @brief Get a TF1 response function object.
   * 
   *
   * @return std::shared_ptr<TF1>
   */
  static std::shared_ptr<TF1> GetResponseFunc();

  /**
   * @brief Get the Nr Of Adc Samples (Spadic default)
   * 
   * @return constexpr UInt_t 
   */
  static constexpr UInt_t GetNrOfAdcSamples() { return fgNrOfAdcSamples; }

  /**
   * @brief Get the Nr Of Preamples
   * 
   * @return constexpr UInt_t 
   */
  static constexpr UInt_t GetNrOfPresamples() { return fgNrOfPresamples; }

  /**
   * @brief Get the Shaping Order 
   * 
   * @return constexpr UInt_t 
   */
  static constexpr UInt_t GetShapingOrder() { return fgShapingOrder; }

  /**
   * @brief Get the Shaping Time (Spadic default)
   * 
   * @return Float_t 
   */
  static Float_t GetShapingTime() { return fgShapingTime; }

  /**
   * @brief Get the Clock Cycle 
   * 
   * @return Double_t 
   */
  static Double_t GetClockCycle() { return fgClockCycleLength; }

  /**
   * @brief Get the Charge To Max Adc Calibration value
   * 
   * @return Double_t 
   */
  static Double_t GetChargeToMaxAdcCal() { return fgChargeToMaxAdcCal; }

  /**
  * @brief Response of the SPADIC v2.2 to an input charge, for a given timesample.
  * 
  * In the parameter array thinks like shaping time, order, input charge and so on * can and have to be passed. Use this function for fitting for exampl.
  * Use SpadicResponse(UInt_t samplenr) to get the response for the paramters 
  * setted in the given CbmTrdSpadic object.
  *
  * @param samplenr 
  * @param par 
  * @return Double_t 
  */
  static Double_t Response(Double_t* samplenr, Double_t* par);

  /**
   * @brief Get the Analog Response to a given input charge per adc time bin
   * 
   * If fNoiseLvl is set to a value above 0 it is automatically added to the response.
   *
   * @param inputCharge [keV] and not calibrated to MIP see fgMaxAdcMipCalibration
   * @param binTimeshift timeshift of the signal within a CC [ns]
   * @return std::vector<std::int16_t> calibrated to keV and MIP
   */
  std::vector<std::int16_t> GetAnalogResponsePulse(Double_t inputCharge, Double_t binTimeshift = 0.0);

  /**
   * @brief Get a Noise value based on the width of the noise distribution in sigmas.
   * 
   * @param sigmaNoise 
   * @return Int_t 
   */
  Int_t GetNoise(Double_t sigmaNoise);

  /**
   * @brief Get the Baseline Lvl value
   * 
   * @return UInt_t 
   */
  UInt_t GetBaselineLvl() { return fBaselineLvl; }

  /**
   * @brief Check the passed pulse for samples in clipping and reduce the value of 
   * those to fClippingStart.
   * 
   * @tparam Pulse 
   * @param pulse 
   */
  template<class Pulse>
  void ApplyClipping(Pulse* pulse)
  {
    for (auto& sample : *pulse) {
      if (sample > fClippingStart) sample = fClippingStart;
    }
  }

  /**
   * @brief Get the Analytic Timeshift value ns precision
   * Calculate the timeshift within the spadic response based on the idea that on
   * an analytical base it is only related to the offset between the clock cycle 
   * and the absolute time.
   * @param absolutetime 
   * @return Double_t 
   */
  Double_t GetAnalyticTimeshift(Double_t absolutetime);

  /**
   * @brief Get the Dynamic Range value
   * 
   * @return UInt_t 
   */
  UInt_t GetDynamicRange() { return fDynamicRange; }

  /**
   * @brief Get the Peaking Sample Pos
   * 
   * Returns the sample posisiton were the response function peaks, based 
   * on the number of presamples and shaping time.
   * @return UInt_t 
   */
  UInt_t GetPeakingSamplePos();

  /**
   * @brief Get the Clipping Start value
   * 
   * @return Int_t 
   */
  Int_t GetClippingStart() { return fClippingStart; }

  /**
   * @brief Get the Adc Noise Level value
   * 
   * @return Float_t 
   */
  Float_t GetAdcNoiseLevel() { return fNoiseLvl; }

  /**
   * @brief Get the Max Adc To Energy Cal value
   * 
   * @return Float_t 
   */
  Float_t GetMaxAdcToEnergyCal() { return fMaxAdcToEnergyCal; }

  /**
   * @brief Get the MaxAdc to Mip Calibration value
   * 
   * @return Double_t 
   */
  Double_t GetMipCalibration() { return fMaxAdcToMipCal; }


  /**
   * @brief Get the Module Thickness value
   * 
   * @return Float_t 
   */
  Float_t GetModuleThickness() { return fModuleThickness; }

  /**
   * @brief Get the Use Baseline Avg flag
   * 
   * The spadic either simply delievers n presamples or an average baseline as a single presample.
   * 
   * @return true 
   * @return false 
  */
  bool GetUseBaselineAvg() { return fDoUseBaselineAvg; }

  /**
   * @brief Return energy [keV (MC input GeV)] for a given max adc value, based on max adc to energy 
   * calibration.
   * 
   * @param maxadc 
   * @return Float_t 
   */
  Float_t MaxAdcToEnergyCal(Float_t maxadc);


  /**
   * @brief Get the Trigger Decision 
   * Get the trigger decision for the spadic. Currently this is based on the &
   * differential trigger approach.
   * @todo Write a trigger base and derived class to allow for easy interchange 
   * between different trigger modes. This part can than be dropped.
   * @tparam Pulse 
   * @param pulse 
   * @param multitriggersample 
   * @return CbmTrdDigi::eTriggerType 
   */
  template<class precision>
  CbmTrdDigi::eTriggerType GetTriggerDecision(std::vector<precision>* pulse, UInt_t* sndtriggersample = nullptr)
  {
    if (pulse->empty()) return CbmTrdDigi::eTriggerType::kNTrg;

    if (sndtriggersample) *sndtriggersample = 0;
    Int_t slopeFst   = 0;
    Int_t slopeSnd   = 0;
    bool selftrigger = false;
    bool falling     = false;
    bool multihit    = false;


    // Loop from the pulse beginning to last sample which could release a trigger.
    // Since, we need two positive sloapes in a row, the last two samples can not release a trigger alone
    for (auto sampleIt = pulse->begin(); sampleIt < pulse->end() - 2; ++sampleIt) {
      slopeFst = *(sampleIt + 1) - *sampleIt;
      if (slopeFst < fTriggerSlopeConditionFst && !selftrigger) continue;

      slopeSnd = *(sampleIt + 1) - *sampleIt;
      if (slopeSnd < 0 && !selftrigger) continue;

      if (!selftrigger) {
        if (slopeFst >= 2 * fTriggerSlopeConditionFst) selftrigger = true;
        if (slopeFst >= fTriggerSlopeConditionFst && slopeSnd >= fTriggerSlopeConditionSnd) selftrigger = true;
      }
      else {
        if (falling) {
          if (slopeFst >= 2 * fTriggerSlopeConditionFst) multihit = true;
          if (slopeFst >= fTriggerSlopeConditionFst && slopeSnd >= fTriggerSlopeConditionSnd) multihit = true;
          if (sndtriggersample) *sndtriggersample = sampleIt - pulse->begin();
        }
        if (slopeFst < 0 && slopeSnd < 0) falling = true;
      }
      if (multihit) break;
    }
    CbmTrdDigi::eTriggerType triggerType = CbmTrdDigi::eTriggerType::kNeighbor;
    if (selftrigger && !multihit) triggerType = CbmTrdDigi::eTriggerType::kSelf;
    if (selftrigger && multihit) triggerType = CbmTrdDigi::eTriggerType::kMulti;

    return triggerType;
  }
  /**
   * @brief Set the Dynamic Range value
   * 
   * @return UInt_t 
   */
  void SetDynamicRange(UInt_t value) { fDynamicRange = value; }

  /**
   * @brief Set the Clipping Start value
   * 
   * @param value 
   */
  void SetClippingStart(Int_t value) { fClippingStart = value; }

  void SetNoiseLevel(Double_t value) { fNoiseLvl = value; }

  /**
   * @brief Set the Trigger Slope Condition value
   * 
   * @param value 
   */
  void SetFstTriggerSlopeCondition(Double_t value) { fTriggerSlopeConditionFst = value; }

  /**
   * @brief Set the Trigger Slope Condition value
   * 
   * @param value 
   */
  void SetSndTriggerSlopeCondition(Double_t value) { fTriggerSlopeConditionSnd = value; }

  /**
   * @brief Set the Max Adc To Energy Cal value
   * 
   * @param value 
   */
  void SetMaxAdcToEnergyCal(Double_t value) { fMaxAdcToEnergyCal = value; }

  /**
   * @brief Set the max adc to mip calibration value
   * 
   * @param value 
   */
  void SetMipCalibration(Double_t value) { fMaxAdcToMipCal = value; }


  /**
   * @brief Set the Module Thickness value
   * 
   * @param value 
   */
  void SetModuleThickness(Double_t value) { fModuleThickness = value; }

  /**
   * @brief Set the Baseline Lvl value
   * 
   * @param value 
   */
  void SetBaselineLvl(UInt_t value) { fBaselineLvl = value; }

  /**
   * @brief Set the Use Baseline Average flag
   * 
   * @param value 
  */
  void SetUseBaselineAverage(bool value = true) { fDoUseBaselineAvg = value; }

  /**
   * @brief Response of the SPADIC v2.2 to an input charge, for a given timesample.
   * 
   * Return the spadic response based on the parameters setted in this 
   * CbmTrdSpadic object.
   *
   * @param samplenr
   * @return Double_t 
   */
  Double_t Response(UInt_t samplenr);

  void SetParameter(eResponsePars ipar, Double_t value) { fvecResponsePars.at(static_cast<size_t>(ipar)) = value; }

private:
  /**
   * @brief Clockcycle length of SPADIC v2.2
   * 
   */
  static const Double_t fgClockCycleLength;

  /**
   * @brief Vector for the response parameters. Order based on eResponsePars.
   * The given values here correspond to the design/default values for SPADIC v2.2
   * - Shaping time = 142 ns (measured value from desy2019/lab2020)
   * (design value = 120 ns).
   * - Shaping order for the signal shaper.
   * The default value (=1) corresponds to SPADIC v2.2 standard settings.
   * - Input charge, default = 0 ADC units, charge to which the Spadic responses
   * - Bin timeshift = ClockCycleLength / 2 ns shift placed at the sample center
   * - Number of presamples = 2 standard value of presamples before the signal
   * response starts
   */
  std::vector<Double_t> fvecResponsePars = {fgShapingTime,          fgShapingOrder,   0.0,
                                            fgClockCycleLength / 2, fgNrOfPresamples, fgChargeToMaxAdcCal};

  /**
   * @brief Number of ADC samples.
   * 
   * Number of ADC samples available.
   * Default (32) corresponds to the maximum number of samples for SPADIC v2.2
   */
  static constexpr UInt_t fgNrOfAdcSamples = 32;

  /**
   * @brief Spadic shaping time.
   * 
   * Shaping time of the Spadic signal shaper, value corresponds to the best 
   * knowledge from measurements (status 23.02.2021 PR) please update if newer 
   * results are available
   * 
   */
  static const Double_t fgShapingTime;

  /**
   * @brief Spadic Shaping order
   * 
   * Default value of the Spadic shaping order = 1 (status 23.02.2021 PR)
   */
  static constexpr UInt_t fgShapingOrder = 1;

  /**
   * @brief Number of presamples in the ADC signal in front of the actual signal
   * 
   * Default Spadic value = 2 (status 23.02.2021 PR) please update if this changes.
   * Default Spadic value = 1 (status 16.06.2021 PR) please update if this changes.
   */
  static constexpr UInt_t fgNrOfPresamples = 1;

  /**
   * @brief Calibration factor for input charges
   * 
   * Charge calibration for fit response to max adc, i.e. this scales the response * for a given input charge to match the corresponding max adc value.
   * Default Spadic value = 2.73 extracted from fits to desy2019 and lab data
   * (status 23.02.2021 PR) please update if this changes.
   */
  static const Double_t fgChargeToMaxAdcCal;

  /**
   * @brief Dynamic range of the spadic 
   * The Spadic uses a 9-Bit ADC
   */
  UInt_t fDynamicRange = std::pow(2, 9);

  /**
   * @brief Value where a analog and or digital clipping appears
   * By default it is set to the dynamic range of Spadic.
   */
  Int_t fClippingStart = fDynamicRange;

  /**
   * @brief Slope trigger condition for a two stage differential triggger.
   *
   * This defines the min diff between the first two consecutive rising samples in 
   * the pulse, to mark the pulse with CbmTrdDigi::eTriggerType::kSelf.
   * An analytical MIP pulse with a conservative shaping time of 300 ns (keep in mind 
   * that a pulse is not a delta input) has a diff of 16 at the first relevant step. 
   * Based on response definition status March 2021 - PR.
   * 
   */
  Int_t fTriggerSlopeConditionFst = 15;

  /**
   * @brief Slope trigger condition for a two stage differential triggger.
   *
   * This defines the min diff between the second pair of two consecutive rising 
   * samples in the pulse, to mark the pulse with CbmTrdDigi::eTriggerType::kSelf.
   * An analytical MIP pulse with a conservative shaping time of 300 ns (keep in mind 
   * that a pulse is not a delta input) has a diff of 10 at the second (this) 
   * relevant step. Based on response definition status March 2021 - PR.
   * Remark: There is an ignore switch for this condition if the slope of the first 
   * check exceeds the condition by 2. Since, a 120 ns shaping time pulse will have 
   * the combination of 30 and 5 as slopes.
   */
  Int_t fTriggerSlopeConditionSnd = 9;

  /**
   * @brief Thickness of the module [cm] this spadic is mounted on.
   * Should be setted during runtime from parameters. For the time being the 
   * default 1.2 cm should be fine, since all modules with spadics have the same 
   * thickness.
   * @todo connect this to the parameters in runtime usage.
   */
  Float_t fModuleThickness = 1.20;

  /**
   * @brief Baseline level of the current Spadic
   * Used for MaxAdcToMip calibration and baseline placement in Analogue Response
   */
  // UInt_t fBaselineLvl = 10;
  UInt_t fBaselineLvl = 0;

  /**
   * @brief Calibration for MIP to 7% of MaxAdc dynamic range.
   * 0.65 = charge from PRF broad distribution on central pad
   * fModuleThickness * CbmTrdMipDeDx() = mean deposited mip energy per chamber
   * fgChargeToMaxAdcCal = Factor between input charge for spadic response and max 
   * adc value handled in response function
   * 0.08 * (fDynamicRange - 10) = 8% of the dynamic range with 10 ADU as baseline 
   * approximation 
   */
  Float_t fMaxAdcToMipCal = (CbmTrddEdxUtils::MipDeDx() * fModuleThickness * CbmTrddEdxUtils::MeanChargeCentrPadPRF())
                            / (CbmTrddEdxUtils::MipCaliTarget() * (fDynamicRange - fBaselineLvl));
  // Float_t fMaxAdcToMipCal = 1;

  /** @brief Calibration value to calculate energy in keV based on the max adc value. Default = Simulation calibration. */
  Float_t fMaxAdcToEnergyCal = fMaxAdcToMipCal;

  /** @brief Noise level for signal response simulation [gaus sigmas] */
  Float_t fNoiseLvl = 1;

  /** @brief Flag wether we have n standard presamples or the average baseline in a single presamples */
  bool fDoUseBaselineAvg = false;

public:
  ClassDef(CbmTrdSpadic, 2);
};
#endif
