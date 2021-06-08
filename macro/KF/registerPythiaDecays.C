/* Copyright (C) 2018-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

void registerPythiaDecays(int iDecay)
{
  KFPartEfficiencies eff;

  if (iDecay == eff.GetParticleIndex(429) || iDecay == eff.GetParticleIndex(-429)) {
    Double_t lifetime = eff.partLifeTime[iDecay];  // lifetime
    Double_t mass     = eff.partMass[iDecay];
    Int_t PDG         = eff.partPDG[iDecay];
    Double_t charge   = eff.partCharge[iDecay];

    for (int iPall = eff.GetParticleIndex(429); iPall <= eff.GetParticleIndex(-429); iPall++) {
      Double_t lifetime = eff.partLifeTime[iPall];  // lifetime
      Double_t mass     = eff.partMass[iPall];
      Int_t PDG         = eff.partPDG[iPall];
      Double_t charge   = eff.partCharge[iPall];

      FairParticle* newParticle = new FairParticle(PDG, eff.partTitle[iPall].data(), kPTHadron, mass, charge, lifetime,
                                                   "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE);
      newParticle->Print();
      FairRunSim::Instance()->AddNewParticle(newParticle);
    }
    TString pythia6Config = gSystem->Getenv("VMCWORKDIR");
    pythia6Config += "/macro/KF/DecayConfig.C()";
    FairRunSim::Instance()->SetPythiaDecayer(pythia6Config);
  }
}
