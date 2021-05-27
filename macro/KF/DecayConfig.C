/** @file DecayConfig.C
  * @copyright Copyright (C) 2018 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [orginator] **/

void DecayConfig()
{

  KFPartEfficiencies eff;

  for (int iParticle = eff.GetParticleIndex(429); iParticle <= eff.GetParticleIndex(-429); iParticle++) {
    Double_t lifetime = eff.partLifeTime[iParticle];  // lifetime
    Double_t mass     = eff.partMass[iParticle];
    Int_t PDG         = eff.partPDG[iParticle];
    Double_t charge   = eff.partCharge[iParticle];

    TPythia6Decayer* decayer = TPythia6Decayer::Instance();

    TString tableName = gSystem->Getenv("VMCWORKDIR");
    tableName += "/input/PythiaDecayTable.txt";

    decayer->SetDecayTableFile(tableName.Data());
    decayer->ReadDecayTable();

    decayer->SetForceDecay(TPythia6Decayer::kAll);
    Int_t products[4] = {0};
    Int_t mult[4]     = {0};
    Int_t npart       = eff.GetNDaughters(iParticle);

    for (int iD = 0; iD < npart; iD++) {
      products[iD] = eff.GetDaughterPDG(iParticle, iD);
      mult[iD]     = 1;
    }
    decayer->ForceParticleDecay(PDG, products, mult, npart);

    gMC->SetExternalDecayer(decayer);

    TPythia6& pythia6 = *(TPythia6::Instance());

    gMC->SetUserDecay(PDG);
    pythia6.SetMDCY(pythia6.Pycomp(PDG), 1, 1);
  }
}
