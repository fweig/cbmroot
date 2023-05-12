/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMSTSPARASIC_H
#define CBMSTSPARASIC_H 1

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDefNV

#include <array>
#include <set>
#include <string>  // for string

class TF1;

/** @class CbmStsParAsic
 ** @brief Parameters of the STS readout ASIC
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 23.03.2020
 **
 ** This class represents the configuration parameters of a readout ASIC of the STS (STSXYTER).
 **/
class CbmStsParAsic {

public:
  /** @brief Default constructor **/
  CbmStsParAsic() {};


  /** @brief Constructor with parameters
   ** @param nChannels   Number of readout channels
   ** @param nAdc  Number of ADC channels
   ** @param dynRange  Dynamic range of ADC [e]
   ** @param threshold  ADC threshold [e]
   ** @param timeResol   Time resolution [ns]
   ** @param deadTime  Single-channel dead time [ns]
   ** @param noise   Noise RMS [e]
   ** @param znr   Zero-crossing noise rate [1/ns]
   **/
  CbmStsParAsic(uint16_t nChannels, uint16_t nAdc, double dynRange, double threshold, double timeResol, double deadTime,
                double noise, double znr);


  /** @brief Copy constructor (implicitely disable move constructor and assignment)**/
  CbmStsParAsic(const CbmStsParAsic&);


  /** @brief Copy assignment operator **/
  CbmStsParAsic& operator=(const CbmStsParAsic& other);


  /** @brief Destructor **/
  ~CbmStsParAsic();


  /** @brief Charge from ADC channel (mean)
   ** @param adc ADC channel
   ** @return Mean charge in ADC channel [e]
   */
  double AdcToCharge(uint16_t adc) const { return fThreshold + fDynRange / double(fNofAdc) * (double(adc) + 0.5); }


  /** @brief Randomly deactivate a fraction of the channels
   ** @param fraction  Fraction of channels to deactivate
   ** @return Number of deactivated channels
   **/
  uint16_t DeactivateRandomChannels(double fraction);


  /** @brief ADC channel for a given charge
   ** @param charge  Charge [e]
   ** @return ADC channel number
   **
   ** Returns -1 for charge below threshold.
   **/
  int16_t ChargeToAdc(double charge) const;


  /** @brief Single-channel dead time
   ** @return Dead time [ns]
   **/
  double GetDeadTime() const { return fDeadTime; }


  /** @brief Dynamic range of ADC
   ** @return Dynamic range [e]
   **/
  double GetDynRange() const { return fDynRange; }


  /** @brief Number of ADC channels
   ** @return Number of ADC channels
   **/
  uint16_t GetNofAdc() const { return fNofAdc; }


  /** @brief Number of readout channels
   ** @return Number of readout channels
   **/
  uint16_t GetNofChannels() const { return fNofChannels; }


  /** @brief Electronic noise RMS
   ** @return Noise RMS [e]
   **/
  double GetNoise() const { return fNoise; }


  /** @brief Single-channel noise rate
   ** @return Noise rate [1/s]
   **/
  double GetNoiseRate() const;


  /** @brief Random noise charge
   ** @return Charge of a random noise signal [e]
   **
   ** The noise charge is samples from a Gaussian with zero mean
   ** and width equal to the noise RMS, starting from threshold
   ** and up to 10 times the noise RMS.
   **/
  double GetRandomNoiseCharge() const;


  /** @brief ADC Threshold
   ** @return Threshold [e]
   **/
  double GetThreshold() const { return fThreshold; }


  /** @brief Time resolution
   ** @return Time resolution [ns]
   **/
  double GetTimeResol() const { return fTimeResolution; }


  /** @brief Zero-crossing noise rate
   ** @return Zero-crossing noise rate [1/ns]
   **/
  double GetZeroNoiseRate() const { return fZeroNoiseRate; }


  /** @brief Initialisation
   **
   ** Calculates the noise charge distribution.
   **/
  void Init();


  /** @brief Check for a channel being active
   ** @param channel  Channel number within ASIC
   ** @return True if the channel is active
   **/
  Bool_t IsChannelActive(uint16_t channel) const { return fDeadChannels.find(channel) == fDeadChannels.end(); }


  /** @brief Set parameters
   ** @param nChannels          Number of readout channels
   ** @param nAdc             Number of ADC channels
   ** @param dynRange         Dynamic range [e]
   ** @param threshold        Threshold [e]
   ** @param timeResol        Time resolution [ns]
   ** @param deadTime         Channel dead time [ns]
   ** @param noise            Noise RMS
   ** @param zeroNoiseRate    Zero-crossing noise rate
   ** @param deadChannels     Set of dead channels
   **/
  void Set(uint16_t nChannels, uint16_t nAdc, double dynRange, double threshold, double timeResol, double deadTime,
           double noise, double zeroNoiseRate, std::set<uint16_t> deadChannels = {});


  /** @brief Set time offset
   ** @param offset  Time offset for this ASIC [ns]
   **
   ** The time offset will be subtracted from the message at the unpacking stage.
   */
  void SetTimeOffset(double offset) { fTimeOffset = offset; }


  /** @brief Set coefficients for walk correction
   ** @param par Array of correction parameters
   **/
  void SetWalkCoef(std::array<double, 32> par) { fWalkCoef = par; }

  /** @brief Get one of the coefficients for walk correction
   ** @param uIdx Index of the correction parameter
   ** @return Double value of coeff
   **/
  double GetWalkCoef(uint32_t uIdx) const
  {
    if (uIdx < 32) return fWalkCoef[uIdx];
    return 0.;
  }

  /** @brief Info to string **/
  std::string ToString() const;


private:
  uint16_t fNofChannels           = 0;                   ///< Number of readout channels
  uint16_t fNofAdc                = 0;                   ///< Number of ADC channels
  double fDynRange                = 0.;                  ///< Dynamic range [e]
  double fThreshold               = 0.;                  ///< Threshold [e]
  double fTimeResolution          = 0.;                  ///< Time resolution [ns]
  double fDeadTime                = 0.;                  ///< Channel dead time [ns]
  double fNoise                   = 0.;                  ///< RMS of noise [e]
  double fZeroNoiseRate           = 0.;                  ///< Zero-crossing noise rate [1/ns]
  double fTimeOffset              = 0.;                  ///< Time offset [ns]
  std::array<double, 32> fWalkCoef = {{0., 0., 0., 0.,    ///< Parameters for correction of walk effect
                                       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                       0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}};
  std::set<uint16_t> fDeadChannels {};                   ///< Map of dead channels

  bool fIsInit = kFALSE;  //! Flag for being initialised

  /** @brief Noise charge distribution. Is instantiated by the Init
   ** method in order to avoid frequent re-calculation. **/
  TF1* fNoiseCharge = nullptr;  //!

  ClassDefNV(CbmStsParAsic, 4);
};

#endif /* CBMSTSPARASIC_H */
