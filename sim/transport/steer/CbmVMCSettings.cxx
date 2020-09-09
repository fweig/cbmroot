#include "CbmVMCSettings.h"

#include <Logger.h>

#include <TVirtualMC.h>

void CbmVMCSettings::Init(TVirtualMC* vmc) {

  LOG(info) << ": Configuring global VMC settings";
  assert(vmc);

  // TODO: These settings were taken over from SetCuts.C. Their meanings
  // will have to be looked up and documented.


  // The processes with the following names are defined for TGeant3 and TGeant4
  // such that they can be used similarily for both transport engines
  // PAIR, COMP, PHOT, PFIS, DRAY, ANNI, BREM, HADR, MUNU,
  // DCAY, LOSS, MULS

  // Also the following processes are defined for both transport engines. To unknown
  // reasons they are only applied for Geant3
  // RAYL, CKOV

  // The following process is not set for any of the two transport engines
  // For Geant3 the default value is used
  // SYNC

  // This ppocess is only defined for TGeant4
  // Somehow it is also available in the Geant3 Fortran code but it is not forwarded
  // from the TGeant3 C++ Wraper
  // LABS

  // The energy loss straggling can be only set for Geant3. For Geant4 there is a code fragment
  // which produces a warning informing the user that this property is only
  // supported via /mcPhysics/emModel commands of Geant4
  // STRA

  // Processes
  vmc->SetProcess("PAIR", fProcessPairProduction);     // pair production
  vmc->SetProcess("COMP", fProcessComptonScattering);  // Compton scattering
  vmc->SetProcess("PHOT", fProcessPhotoEffect);        // photo electric effect
  vmc->SetProcess("PFIS", fProcessPhotoFission);       // photofission
  vmc->SetProcess("DRAY", fProcessDeltaRay);           // delta-ray
  vmc->SetProcess("ANNI", fProcessAnnihilation);       // annihilation
  vmc->SetProcess("BREM", fProcessBremsstrahlung);     // bremsstrahlung
  vmc->SetProcess("HADR", fProcessHadronicInteraction);  // hadronic process
  vmc->SetProcess("MUNU",
                  fProcessMuonNuclearInteraction);   // muon nuclear interaction
  vmc->SetProcess("DCAY", fProcessDecay);            // decay
  vmc->SetProcess("LOSS", fProcessEnergyLossModel);  // energy loss
  vmc->SetProcess("MULS", fProcessMultipleScattering);  // multiple scattering

  // The cuts with the following names are defined for TGeant3 and TGeant4
  // such that they can be used similarily for both transport engines
  // The values given will be taken as they are for Geant3
  // For Geant4 the values are used to calculate the proper values in the format
  // needed by Geant
  // CUTGAM, CUTELE, CUTNEU ,CUTHAD, CUTMUO, BCUTE, BCUTM
  // DCUTE, DCUTM, PPCUTM, TOFMAX

  vmc->SetCut("CUTGAM", fEnergyCutGammas);          // gammas (GeV)
  vmc->SetCut("CUTELE", fEnergyCutElectrons);       // electrons (GeV)
  vmc->SetCut("CUTNEU", fEnergyCutNeutralHadrons);  // neutral hadrons (GeV)
  vmc->SetCut("CUTHAD", fEnergyCutChargedHadrons);  // charged hadrons (GeV)
  vmc->SetCut("CUTMUO", fEnergyCutMuons);           // muons (GeV)
  vmc->SetCut(
    "BCUTE",
    fEnergyCutElectronBremsstrahlung);  // electron bremsstrahlung (GeV)
  vmc->SetCut(
    "BCUTM",
    fEnergyCutMuonHadronBremsstrahlung);  // muon/hadron bremsstrahlung(GeV)
  vmc->SetCut("DCUTE",
              fEnergyCutElectronDeltaRay);  // delta-rays by electrons (GeV)
  vmc->SetCut("DCUTM", fEnergyCutMuonDeltaRay);  // delta-rays by muons (GeV)
  vmc->SetCut(
    "PPCUTM",
    fEnergyCutMuonPairProduction);     // direct pair production by muons (GeV)
  vmc->SetCut("TOFMAX", fTimeCutTof);  // time of flight cut in seconds
}

ClassImp(CbmVMCSettings);
