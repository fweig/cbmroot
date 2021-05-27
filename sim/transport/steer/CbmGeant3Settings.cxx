/** @file CbmGeant3Settings.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmGeant3Settings.h"

#include <Logger.h>

#include <TGeant3.h>
#include <TVirtualMC.h>

void CbmGeant3Settings::Init(TVirtualMC* vmc)
{

  CbmVMCSettings::Init(vmc);

  TGeant3* vmcg3 = dynamic_cast<TGeant3*>(vmc);

  assert(vmcg3);
  LOG(info) << ": Configuring Geant3";

  // Set Number of events to be processed
  // Can't be changed from the user code
  vmcg3->SetTRIG(1);

  // Print every 100th event : GTREVE_ROOT : Transporting primary track No
  // Can't be changed from user code
  vmcg3->SetSWIT(4, 100);

  if (fDebugOutput) {
    vmcg3->SetDEBU(fDebugMinEvent, fDebugMaxEvent, 1);
    // IF (ISWIT(2).EQ.2) CALL GPCXYZ ! step by step printed debug
    vmcg3->SetSWIT(2, 2);
  }
  else {
    vmcg3->SetDEBU(0, 0, 1);
  }

  // Switch on/off the rayleigh scattering
  vmcg3->SetRAYL(fProcessRayleighScattering);

  //  Control Cerenkov production
  vmcg3->SetCKOV(fProcessCherenkov);

  // Switch on/off the energy loss fluctuations with the Photo-Absorption Ionization model
  vmcg3->SetSTRA(fProcessEnergyLossStraggling);

  // Control the automatic calculation of tracking medium parameters
  vmcg3->SetAUTO(fAutomaticTrackingMediumParameters);

  // Control if particles should be stopped
  vmcg3->SetABAN(fStoppingMethod);

  //  This flag controls the tracking optimization performed via the
  //  GSORD routine:
  vmcg3->SetOPTI(fTrackingOptimizationMethod);

  //  Control the cross section tabulations
  vmcg3->SetERAN(fCrossSectionMinEnergy, fCrossSectionMaxEnergy, fCrossSectionBins);
}

ClassImp(CbmGeant3Settings);
