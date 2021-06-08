/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/** @file CbmVMCSettings.h
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 21.01.2020
 **/

#ifndef CBMVMCSETTINGS_H
#define CBMVMCSETTINGS_H 1

/** @class CbmVMCSettings
 ** @brief User interface class to define the transport simulation settings which are 
 **        common for both used transport engines
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 21.01.2020
 **/

#include <Logger.h>

#include <TObject.h>

template<typename T>
void CheckValueInRange(const T& value, const T& low, const T& high, std::string functionName)
{
  if ((value < low) || (value > high)) {
    LOG(fatal) << "You try to set the value " << value << "which is out of the bounds(" << low << "," << high
               << ") for the function " << functionName;
  }
}

class TVirtualMC;

class CbmVMCSettings : public TObject {

public:
  CbmVMCSettings()                      = default;
  ~CbmVMCSettings()                     = default;
  CbmVMCSettings(const CbmVMCSettings&) = delete;
  CbmVMCSettings& operator=(const CbmVMCSettings&) = delete;


  /** @brief Set all parameters defined in this class
   ** @param[in] vmc Pointer to the VirtualMC class
   **
   **/
  void Init(TVirtualMC*);

  /** @brief Control the pair production process
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no pair production
   **     = 1 Pair production with generation of secondary e+/e- (Default)
   **     = 2 Pair production without generation of secondary e+/e-
   ** @endcode
   **/
  void SetProcessPairProduction(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessPairProduction");
    fProcessPairProduction = val;
  }

  /** @brief Control the Compton scattering process
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no Compton scattering
   **     = 1 Compton scattering with production of e- (Default)
   **     = 2 Compton scattering without production of e-
   ** @endcode
   **/
  void SetProcessComptonScattering(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessComptonScattering");
    fProcessComptonScattering = val;
  }

  /** @brief Control the photo-electric effect
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no photo-electric effect
   **     = 1 photo-electric effect with production of e- (Default)
   **     = 2 photo-electric effect without production of e-
   ** @endcode
   **/
  void SetProcessPhotoEffect(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessPhotoEffect");
    fProcessPhotoEffect = val;
  }

  /** @brief Control the process of nuclear fission induced by a photon
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no photo-fission (Default)
   **     = 1 photo-fission with generation of secondaries
   **     = 2 photo-fission without generation of secondaries
   ** @endcode
   **/
  void SetProcessPhotoFission(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessPhotoFission");
    fProcessPhotoFission = val;
  }

  /** @brief Control the delta ray production
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no delta ray production
   **     = 1 delta ray production with generation of e- (Default)
   **     = 2 delta ray production without generation of e-
   ** @endcode
   **/
  void SetProcessDeltaRay(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessDeltaRay");
    fProcessDeltaRay = val;
  }

  /** @brief Control the positron annihilation process
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no positron annihilation
   **     = 1 positron annihilation with generation of photons (Default)
   **     = 2 positron annihilation without generation of photons 
   ** @endcode
   **/
  void SetProcessAnnihilation(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessAnnihilation");
    fProcessAnnihilation = val;
  }

  /** @brief Control the process of bremsstrahlung
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no bremsstrahlung
   **     = 1 bremsstrahlung with generation of gammas (Default)
   **     = 2 bremsstrahlung without generation of gammas
   ** @endcode
   **/
  void SetProcessBremsstrahlung(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessBremsstrahlung");
    fProcessBremsstrahlung = val;
  }

  /** @brief Control the hadronic interactions
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no hadronic interaction
   **     = 1 hadronic interaction with generation of secondaries (Default)
   **     = 2 hadronic interaction without generation of secondaries
   **     > 2 Can be used in user code to choose a hadronic package
   **     = 5 hadronic interactions using the gcalor package
   ** @endcode
   **/
  void SetProcessHadronicInteraction(Int_t val)
  {
    CheckValueInRange(val, 0, 5, "SetProcessHadronicInteraction");
    fProcessHadronicInteraction = val;
  }

  /** @brief Control the muon-nucleus interaction
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no muon-nucleus interaction
   **     = 1 muon-nucleus interactions with generation of secondaries (Default)
   **     = 2 muon-nucleus interactions without generation of secondaries
   ** @endcode
   **/
  void SetProcessMuonNuclearInteraction(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessMuonNuclearInteraction");
    fProcessMuonNuclearInteraction = val;
  }

  /** @brief Control the decay of particles in flight
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no decay in flight
   **     = 1 decay in flight with generation of secondaries (Default)
   **     = 2 decay in flight without generation of secondaries
   ** @endcode
   **/
  void SetProcessDecay(Int_t val)
  {
    CheckValueInRange(val, 0, 2, "SetProcessDecay");
    fProcessDecay = val;
  }

  /** @brief Control the continuous enery loss process 
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no continuous energy loss
   **     = 1 continuous energy loss with generation of delta-rays above the value defined for
   **         the DCUTE cut and restricted Landau fluctuations below DCUTE (Default)
   **     = 2 continuous energy loss without generation of delta-rays and full 
   **         Landau-Vavilov-Gauss fluctuations. In This case the delta-ray production process
   **         is switched of to avoid double counting
   **     = 3 Same as 1, kept for backward compatibility
   **     = 4 Energy loss without fluctuations. The value obtained from the tables is used directly
   ** @endcode
   **/
  void SetProcessEnergyLossModel(Int_t val)
  {
    CheckValueInRange(val, 0, 4, "SetProcessEnergyLossModel");
    fProcessEnergyLossModel = val;
  }

  /** @brief Control the multiple scattering process
   ** @param[in] val Value to be set 
   **
   ** @code
   ** val = 0 no multiple scattering
   **     = 1 multiple scattering according to Moliere theory (Default)
   **     = 2 same as 1, kept for backward compatibility
   **     = 3 pure gaussian scattering according to the Rossi formula
   ** @endcode
   **/
  void SetProcessMultipleScattering(Int_t val)
  {
    CheckValueInRange(val, 0, 3, "SetProcessMultipleScattering");
    fProcessMultipleScattering = val;
  }

  /** @brief Set the energy threshold for the transport of gammas
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutGammas(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutGammas");
    fEnergyCutGammas = val;
  }

  /** @brief Set the energy threshold for the transport of electros and positrons
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutElectrons(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutElectrons");
    fEnergyCutElectrons = val;
  }

  /** @brief Set the energy threshold for the transport of neutral hadrons
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutNeutralHadrons(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutNeutralHadrons");
    fEnergyCutNeutralHadrons = val;
  }

  /** @brief Set the energy threshold for the transport of charged hadrons and ions
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutChargedHadrons(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutChargedHadrons");
    fEnergyCutChargedHadrons = val;
  }

  /** @brief Set the energy threshold for the transport of muons
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutMuons(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutMuons");
    fEnergyCutMuons = val;
  }

  /** @brief Set the energy threshold for photons produced by electron bremsstrahlung
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutElectronBremsstrahlung(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutElectronBremsstrahlung");
    fEnergyCutElectronBremsstrahlung = val;
  }

  /** @brief Set the energy threshold for photons produced by muon bremsstrahlung
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutMuonHadronBremsstrahlung(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutMuonHadronBremsstrahlung");
    fEnergyCutMuonHadronBremsstrahlung = val;
  }

  /** @brief Set the energy threshold for electrons produced by electron delta-rays
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutElectronDeltaRay(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutElectronDeltaRay");
    fEnergyCutElectronDeltaRay = val;
  }

  /** @brief Set the energy threshold for electrons produced by muon or hadron delta-rays
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutMuonDeltaRay(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutMuonDeltaRay");
    fEnergyCutMuonDeltaRay = val;
  }

  /** @brief Set the energy threshold for e+e- direct pair production by muons
   ** @param[in] val Value to be set 
   **
   ** The parameter is the kinetic energy in GeV
   **/
  void SetEnergyCutMuonPairProduction(Double_t val)
  {
    CheckValueInRange(val, 0., 100., "SetEnergyCutMuonPairProduction");
    fEnergyCutMuonPairProduction = val;
  }

  /** @brief Set the time of flight threshold from the primary interaction time
   ** @param[in] val Value to be set 
   **
   ** The parameter is the time in s
   **/
  void SetTimeCutTof(Double_t val)
  {
    CheckValueInRange(val, 0., 2., "SetTimeCutTof");
    fTimeCutTof = val;
  }

private:
  Int_t fProcessPairProduction {1};
  Int_t fProcessComptonScattering {1};
  Int_t fProcessPhotoEffect {1};
  Int_t fProcessPhotoFission {0};
  Int_t fProcessDeltaRay {1};
  Int_t fProcessAnnihilation {1};
  Int_t fProcessBremsstrahlung {1};
  Int_t fProcessHadronicInteraction {1};
  Int_t fProcessMuonNuclearInteraction {1};
  Int_t fProcessDecay {1};
  Int_t fProcessEnergyLossModel {1};
  Int_t fProcessMultipleScattering {1};

  Double_t fEnergyCutGammas {1.e-3};                    // GeV
  Double_t fEnergyCutElectrons {1.e-3};                 // GeV
  Double_t fEnergyCutNeutralHadrons {1.e-3};            // GeV
  Double_t fEnergyCutChargedHadrons {1.e-3};            // GeV
  Double_t fEnergyCutMuons {1.e-3};                     // GeV
  Double_t fEnergyCutElectronBremsstrahlung {1.e-3};    // GeV
  Double_t fEnergyCutMuonHadronBremsstrahlung {1.e-3};  // GeV
  Double_t fEnergyCutElectronDeltaRay {1.e-3};          // GeV
  Double_t fEnergyCutMuonDeltaRay {1.e-3};              // GeV
  Double_t fEnergyCutMuonPairProduction {1.e-3};        // GeV

  Double_t fTimeCutTof {1.0};  // s

  ClassDef(CbmVMCSettings, 1);
};

#endif /* CBMVMCSETTINGS_H */
