/** @file CbmStsParAsic.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsParAsic.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 23.03.2020
 **/

#ifndef CBMSTSPARASIC_H
#define CBMSTSPARASIC_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDefNV
#include <RtypesCore.h>  // for Double_t, UShort_t, Bool_t, Short_t, kFALSE

#include <set>
#include <string>  // for string

class TF1;

/** @class CbmStsParAsic
 ** @brief Parameters of the STS readout ASIC
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 23.03.2020
 **
 ** This class represents the configuration parameters of a
 ** readout ASIC of the STS (STSXYTER).
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
  CbmStsParAsic(UShort_t nChannels, UShort_t nAdc, Double_t dynRange, Double_t threshold, Double_t timeResol,
                Double_t deadTime, Double_t noise, Double_t znr);


  /** @brief Copy constructor (disabled) **/
  CbmStsParAsic(const CbmStsParAsic&);


  /** @brief Move constructor (disabled) **/
  CbmStsParAsic(CbmStsParAsic&&) = default;


  /** @brief Copy assignment operator **/
  CbmStsParAsic& operator=(const CbmStsParAsic& other);


  /** @brief Move assignment operator (disabled) **/
  CbmStsParAsic& operator=(CbmStsParAsic&&) = default;


  /** @brief Destructor **/
  ~CbmStsParAsic();


  /** @brief Charge from ADC channel (mean)
     ** @param adc ADC channel
     ** @return Mean charge in ADC channel [e]
     */
  Double_t AdcToCharge(UShort_t adc) const
  {
    return fThreshold + fDynRange / Double_t(fNofAdc) * (Double_t(adc) + 0.5);
  }


  /** @brief Randomly deactivate a fraction of the channels
   ** @param fraction  Fraction of channels to deactivate
   ** @return Number of deactivated channels
   **/
  UInt_t DeactivateRandomChannels(Double_t fraction);


  /** @brief ADC channel for a given charge
     ** @param charge  Charge [e]
     ** @return ADC channel number
     **
     ** Returns -1 for charge below threshold.
     **/
  Short_t ChargeToAdc(Double_t charge) const;


  /** @brief Single-channel dead time
     ** @return Dead time [ns]
     **/
  Double_t GetDeadTime() const { return fDeadTime; }


  /** @brief Dynamic range of ADC
     ** @return Dynamic range [e]
     **/
  Double_t GetDynRange() const { return fDynRange; }


  /** @brief Number of ADC channels
     ** @return Number of ADC channels
     **/
  UShort_t GetNofAdc() const { return fNofAdc; }


  /** @brief Number of readout channels
     ** @return Number of readout channels
     **/
  UShort_t GetNofChannels() const { return fNofChannels; }


  /** @brief Electronic noise RMS
     ** @return Noise RMS [e]
     **/
  Double_t GetNoise() const { return fNoise; }


  /** @brief Single-channel noise rate
     ** @return Noise rate [1/s]
     **/
  Double_t GetNoiseRate() const;


  /** @brief Random noise charge
     ** @return Charge of a random noise signal [e]
     **
     ** The noise charge is samples from a Gaussian with zero mean
     ** and width equal to the noise RMS, starting from threshold
     ** and up to 10 times the noise RMS.
     **/
  Double_t GetRandomNoiseCharge() const;


  /** @brief ADC Threshold
     ** @return Threshold [e]
     **/
  Double_t GetThreshold() const { return fThreshold; }


  /** @brief Time resolution
     ** @return Time resolution [ns]
     **/
  Double_t GetTimeResol() const { return fTimeResolution; }


  /** @brief Zero-crossing noise rate
     ** @return Zero-crossing noise rate [1/ns]
     **/
  Double_t GetZeroNoiseRate() const { return fZeroNoiseRate; }


  /** @brief Initialisation
     **
     ** Calculates the noise charge distribution.
     **/
  void Init();


  /** @brief Check for a channel being active
     ** @param channel  Channel number within ASIC
     ** @return True if the channel is active
     **/
  Bool_t IsChannelActive(UShort_t channel) const { return fDeadChannels.find(channel) == fDeadChannels.end(); }


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
  void Set(UShort_t nChannels, UShort_t nAdc, Double_t dynRange, Double_t threshold, Double_t timeResol,
           Double_t deadTime, Double_t noise, Double_t zeroNoiseRate, std::set<UShort_t> deadChannels = {});


  /** @brief Info to string **/
  std::string ToString() const;


private:
  UShort_t fNofChannels    = 0;         ///< Number of readout channels
  UShort_t fNofAdc         = 0;         ///< Number of ADC channels
  Double_t fDynRange       = 0.;        ///< Dynamic range [e]
  Double_t fThreshold      = 0.;        ///< Threshold [e]
  Double_t fTimeResolution = 0.;        ///< Time resolution [ns]
  Double_t fDeadTime       = 0.;        ///< Channel dead time [ns]
  Double_t fNoise          = 0.;        ///< RMS of noise [e]
  Double_t fZeroNoiseRate  = 0.;        ///< Zero-crossing noise rate [1/ns]
  std::set<UShort_t> fDeadChannels {};  ///< Map of dead channels

  Bool_t fIsInit = kFALSE;  //! Flag for being initialised

  /** @brief Noise charge distribution. Is instantiated by the Init
     ** method in order to avoid frequent re-calculation. **/
  TF1* fNoiseCharge = nullptr;  //!

  ClassDefNV(CbmStsParAsic, 3);
};

#endif /* CBMSTSPARASIC_H */
