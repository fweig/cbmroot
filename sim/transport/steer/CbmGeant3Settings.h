/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/** @file CbmGeant3Settings.h
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 21.01.2020
 **/

#ifndef CBMGEANT3SETTINGS_H
#define CBMGEANT3SETTINGS_H 1

/** @class CbmGeant3Settings
 ** @brief User interface class to define the Geant3 simulation settings
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 21.01.2020
 **/

#include "CbmVMCSettings.h"

class TVirtualMC;

class CbmGeant3Settings : public CbmVMCSettings {
public:
  CbmGeant3Settings()                         = default;
  ~CbmGeant3Settings()                        = default;
  CbmGeant3Settings(const CbmGeant3Settings&) = delete;
  CbmGeant3Settings& operator=(const CbmGeant3Settings&) = delete;

  /** @brief Set all parameters defined in this class
   ** @param[in] vmc Pointer to the VirtualMC class
   **
   **/
  void Init(TVirtualMC*);

  /** @brief Control the rayleigh scattering process
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no Rayleigh scattering 
   **     = 1 Rayleigh scattering (Default)
   ** @endcode
   **/
  void SetProcessRayleighScattering(Int_t val)
  {
    CheckValueInRange(val, 0, 1, "SetProcessRayleighScattering");
    fProcessRayleighScattering = val;
  }

  /** @brief Control the process of cherenkov production
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no Cerenkov photons produced
   **     = 1 production of Cerenkov photons (Default)
   **     = 2 production of Cerenkov photons with primary stopped at each step 
   ** @endcode
   **/
  void SetProcessCherenkovProduction(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessCherenkovProduction");
    fProcessCherenkov = val;
  }

  /** @brief Control the process of energy loss in thin materials
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no collision sampling (Default)
   **     = 1 collision sampling switched on => no delta-rays
   ** @endcode
   **/
  void SetProcessEneryLossStraggling(Int_t val)
  {
    CheckValueInRange(val, 0, 1, "SetProcessEneryLossStraggling");
    fProcessEnergyLossStraggling = val;
  }


  /** @brief Control the automatic calculation of tracking medium parameters
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no automatic calculation
   **     = 1 automatic calculation (Default)
   ** @endcode
   **/
  void SetAutomaticTrackingMediumParameters(Int_t val)
  {
    CheckValueInRange(val, 0, 1, "SetAutomaticTrackingMediumParameters");
    fAutomaticTrackingMediumParameters = val;
  }

  /** @brief Control if particles should be stopped
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 particles are transported normally
   **     = 1 particles will be stopped according to their residual
   **         range if they are far enough from the boundary
   **     = 2 particles will be stopped according to their residual
   **         range  if they are not in a sensitive material and are
   **         far enough from the boundary
   **         If they are in a sensitive volume additional checks are done (Default)
   ** @endcode
   **/
  void SetParticleStoppingMethod(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetParticleStoppingMethod");
    fStoppingMethod = val;
  }

  /** @brief Control the tracking optimization performed via the GSORD routine
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val < 1 no optimization at all; GSORD calls disabled
   **     = 0 no optimization; only user calls to GSORD kept
   **     = 1 all non-GSORDered volumes are ordered along the best axis
   **     = 2 all volumes are ordered along the best axis (Default)
   ** @endcode
   **/
  void SetTrackingOptimizationMethod(Int_t val)
  {
    CheckValueInRange(val, -1, 2, "SetTrackingOptimizationMethod");
    fTrackingOptimizationMethod = val;
  }

  /** @brief Control the calculation of the cross section tables
   ** @param[in] minekin  minimum kinetic energy in GeV
   ** @param[in] maxekin  maximum kinetic energy in GeV
   ** @param[in] bins     number of logarithmic bins (<200)
   **/
  void SetCrossSectionTableLimits(Double_t minekin, Double_t maxekin, Int_t bins)
  {
    CheckValueInRange(minekin, 0., 100., "SetCrossSectionTableLimits");
    CheckValueInRange(maxekin, 0., 100., "SetCrossSectionTableLimits");
    CheckValueInRange(bins, 1, 200, "SetCrossSectionTableLimits");
    fCrossSectionMinEnergy = minekin;
    fCrossSectionMaxEnergy = maxekin;
    fCrossSectionBins      = bins;
  }

  /** @brief Control the debug output
   ** @param[in] val Switch on/off the output of debug information at each step of transport
   ** @param[in] minevent First event for which debug output should be produced
   ** @param[in] maxevent Last event for which debug output should be produced
   ** The debug output will be produced for each event between the minevent and maxevent
   **/
  void SetDebugOutput(Bool_t val = kTRUE, Int_t minevent = 0, Int_t maxevent = 100)
  {
    fDebugOutput   = val;
    fDebugMinEvent = minevent;
    fDebugMaxEvent = maxevent;
  }

private:
  Int_t fProcessRayleighScattering {1};
  Int_t fProcessCherenkov {1};
  Int_t fProcessEnergyLossStraggling {0};
  Int_t fAutomaticTrackingMediumParameters {1};
  Int_t fStoppingMethod {2};
  Int_t fTrackingOptimizationMethod {2};

  Double_t fCrossSectionMinEnergy {5.e-7};  // GeV
  Double_t fCrossSectionMaxEnergy {1.e4};   // Gev
  Double_t fCrossSectionBins {90};

  Bool_t fDebugOutput {kFALSE};
  Int_t fDebugMinEvent {0};
  Int_t fDebugMaxEvent {100};

  ClassDef(CbmGeant3Settings, 1);
};

#endif /* CBMGEANT3SETTINGS_H */
