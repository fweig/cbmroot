/** @file registerGeantDecays.C
  * @copyright Copyright (C) 2018-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

void registerGeantDecays(int iDecay)
{
  KFPartEfficiencies eff;

  std::cout << "---- Executing macro registerGeantDecays -----" << std::endl;

  if (iDecay > -1) {
    Double_t lifetime = eff.partLifeTime[iDecay];  // lifetime
    Double_t mass     = eff.partMass[iDecay];
    Int_t PDG         = eff.partPDG[iDecay];
    Double_t charge   = eff.partCharge[iDecay];

    if (!(iDecay == eff.GetParticleIndex(429) || iDecay == eff.GetParticleIndex(-429))) {
      TVirtualMC::GetMC()->DefineParticle(PDG, eff.partTitle[iDecay].data(), kPTHadron, mass, charge, lifetime,
                                          "hadron", 0.0, 1, 1, 0, 1, 1, 0, 0, 1, kFALSE);

      Int_t mode[6][3];
      Float_t bratio[6];

      for (Int_t kz = 0; kz < 6; kz++) {
        bratio[kz]  = 0.;
        mode[kz][0] = 0;
        mode[kz][1] = 0;
        mode[kz][2] = 0;
      }
      bratio[0] = 100.;
      for (int iD = 0; iD < eff.GetNDaughters(iDecay); iD++) {
        if (iD > 2) continue;
        mode[0][iD] = eff.GetDaughterPDG(iDecay, iD);  //pi+
      }

      TVirtualMC::GetMC()->SetDecayMode(PDG, bratio, mode);
    }
  }

  //Register hypernuclei from KF Particle Finder
  for (int iP = eff.fFirstHypernucleusIndex; iP <= eff.fLastHypernucleusIndex; iP++) {
    Double_t lifetime = eff.partLifeTime[iP];  // lifetime
    Double_t mass     = eff.partMass[iP];
    Int_t PDG         = eff.partPDG[iP];
    Double_t charge   = eff.partCharge[iP];

    TVirtualMC::GetMC()->DefineParticle(PDG, eff.partTitle[iP].data(), kPTHadron, mass, charge, lifetime, "hadron", 0.0,
                                        1, 1, 0, 1, 1, 0, 0, 1, kFALSE);

    Int_t mode[6][3];
    Float_t bratio[6];

    for (Int_t kz = 0; kz < 6; kz++) {
      bratio[kz]  = 0.;
      mode[kz][0] = 0;
      mode[kz][1] = 0;
      mode[kz][2] = 0;
    }
    bratio[0] = 100.;
    for (int iD = 0; iD < eff.GetNDaughters(iP); iD++) {
      if (iD > 2) continue;
      mode[0][iD] = eff.GetDaughterPDG(iP, iD);  //pi+
    }

    TVirtualMC::GetMC()->SetDecayMode(PDG, bratio, mode);
  }

  //Set correct decay modes for pi0 and eta decays
  Float_t bratioEta[6];
  Int_t modeEta[6][3];

  bratioEta[0] = 39.41;    //2gamma
  bratioEta[1] = 32.68;    //3pi0
  bratioEta[2] = 22.92;    //pi+pi-pi0
  bratioEta[3] = 4.22;     //pi+pi-gamma
  bratioEta[4] = 0.69;     //e+e-gamma
  bratioEta[5] = 2.56e-2;  //pi02gamma

  modeEta[0][0] = 22;
  modeEta[0][1] = 22;
  modeEta[0][2] = 0;  //2gamma
  modeEta[1][0] = 111;
  modeEta[1][1] = 111;
  modeEta[1][2] = 111;  //3pi0
  modeEta[2][0] = 211;
  modeEta[2][1] = -211;
  modeEta[2][2] = 111;  //pi+pi-pi0
  modeEta[3][0] = 211;
  modeEta[3][1] = -211;
  modeEta[3][2] = 22;  //pi+pi-gamma
  modeEta[4][0] = 11;
  modeEta[4][1] = -11;
  modeEta[4][2] = 22;  //e+e-gamma
  modeEta[5][0] = 111;
  modeEta[5][1] = 22;
  modeEta[5][2] = 22;  //pi02gamma

  TVirtualMC::GetMC()->SetDecayMode(221, bratioEta, modeEta);


  Float_t bratioPi0[6];
  Int_t modePi0[6][3];

  for (Int_t kz = 0; kz < 6; ++kz) {
    bratioPi0[kz]  = 0.;
    modePi0[kz][0] = 0;
    modePi0[kz][1] = 0;
    modePi0[kz][2] = 0;
  }

  bratioPi0[0] = 98.823;
  bratioPi0[1] = 1.174;

  modePi0[0][0] = 22;
  modePi0[0][1] = 22;  //2gamma
  modePi0[1][0] = 11;
  modePi0[1][1] = -11;
  modePi0[1][2] = 22;  //e+e-gamma

  TVirtualMC::GetMC()->SetDecayMode(111, bratioPi0, modePi0);
}
