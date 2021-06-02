/** @file CbmStsParSensorCond.h
  * @copyright Copyright (C) 2014-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsParSensorCond.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 28.11.2014
 ** @date 26.03.2020
 **
 ** Renamed from CbmStsSensorConditions
 **/

#ifndef CBMSTSPARSENSORCOND_H
#define CBMSTSPARSENSORCOND_H 1

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDefNV
#include <RtypesCore.h>  // for Double_t, Bool_t, Int_t, kFALSE

#include <cassert>  // for assert
#include <string>   // for string


/** @class CbmStsParSensorCond
 ** @brief Parameters for operating conditions of a STS sensor
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 28.11.2014
 ** @date 26.03.2020
 **
 ** For the managed parameters, see the default constructor. Derived
 ** parameters are calculated on instantiation.
 **/
class CbmStsParSensorCond {

public:
  /** @brief Default constructor **/
  CbmStsParSensorCond();


  /** @brief Standard constructor
     ** @param vFD           Full depletion voltage [V]
     ** @param vBias         Bias voltage [V]
     ** @param temperature   Temperature [K]
     ** @param cCoupling     Coupling capacitance [pF]
     ** @param cInterstrip   Inter-strip capacitance [pF]
     **/
  CbmStsParSensorCond(Double_t vFD, Double_t vBias, Double_t temperature, Double_t cCoupling, Double_t cInterstrip);


  /** @brief Copy constructor **/
  CbmStsParSensorCond(const CbmStsParSensorCond&);


  /** @brief Move constructor (disabled) **/
  CbmStsParSensorCond(CbmStsParSensorCond&&) = delete;


  /** @brief Destructor **/
  ~CbmStsParSensorCond();


  /** @brief Coupling capacitance
     ** @return Coupling capacitance [pF]
     **/
  Double_t GetCcoupling() const { return fCcoupling; }


  /** @brief Inter-strip capacitance
     ** @return Inter-strip capacitance [pF]
     **/
  Double_t GetCinterstrip() const { return fCinterstrip; }


  /** @brief Cross-talk coefficient
     ** @return Cross-talk coefficient
     **
     ** The cross-talk coefficient is derived from the capacitances.
     **/
  Double_t GetCrossTalkCoeff() const
  {
    assert(fIsInit);
    return fCrossTalkCoeff;
  }


  /** @brief Hall mobility
     ** @param eField  Electric field [V/cm]
     ** @param chargeType (0 = electron, 1 = hole)
     ** @return Hall mobility [cm**2/(Vs)]
     **/
  Double_t GetHallMobility(Double_t eField, Int_t chargeType) const;


  /** @brief Temperature
     ** @return Temperature [K]
     **/
  Double_t GetTemperature() const { return fTemperature; }


  /** @brief Bias voltage
     ** @return Bias voltage [V]
     **/
  Double_t GetVbias() const { return fVbias; }


  /** Full depletion voltage
     ** @return Full depletion voltage [V]
     **/
  Double_t GetVfd() const { return fVfd; }


  /** @brief Calculate the derived parameters
     **
     ** Since the derived parameters are not streamed, the Init method
     ** must be called whenever the object was instantiated with the
     ** default constructor, e.g., when streamed.
     */
  void Init();


  /** @brief Copy assignment operator **/
  CbmStsParSensorCond& operator=(const CbmStsParSensorCond&);


  /** @brief Move assignment operator (disabled) **/
  CbmStsParSensorCond& operator=(CbmStsParSensorCond&&) = delete;


  /** @brief Set the condition parameters
     ** @param vDep        Full-depletion voltage [V]
     ** @param vBias       Bias voltage [V]
     ** @param temperature Temperature [K]
     ** @param cCoupling   Coupling capacitance [pF]
     ** @param cInterstrip Inter-strip capacitance [pF]
     **/
  void SetParams(Double_t vFd, Double_t vBias, Double_t temperature, Double_t cCoupling, Double_t cInterstrip);


  /** @brief String output **/
  std::string ToString() const;


private:
  // --- Stored parameters
  Double_t fVfd         = 0.;    ///< Full depletion voltage [V]
  Double_t fVbias       = 0.;    ///< Bias voltage [V]
  Double_t fTemperature = 273.;  ///< Temperature [K]
  Double_t fCcoupling   = 0.;    ///< Coupling capacitance [pF]
  Double_t fCinterstrip = 0.;    ///< Inter-strip capacitance [pF]

  // --- Derived parameters
  Double_t fCrossTalkCoeff = 0.;  //! Cross-talk coefficient
  Double_t fMuLowE         = 0.;  //!
  Double_t fBetaE          = 0.;  //!
  Double_t fVsatE          = 0.;  //!
  Double_t fRhallE         = 0.;  //!
  Double_t fMuLowH         = 0.;  //!
  Double_t fBetaH          = 0.;  //!
  Double_t fVsatH          = 0.;  //!
  Double_t fRhallH         = 0.;  //!

  Bool_t fIsInit = kFALSE;  //! Initialisation flag


  ClassDefNV(CbmStsParSensorCond, 2);
};

#endif /* CBMSTSPARSENSORCOND_H */
