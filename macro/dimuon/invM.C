/** @file invM.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger [orginator] **/

//---------------------------------------------------
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void invM(TString dataSet = "muons")
{

  TH1D* invM_sgn = new TH1D("invM_omega", "invM_omega", 400, 0, 4);

  TString name = dataSet + ".ana.root";

  TClonesArray* MuPlus  = new TClonesArray("CbmAnaMuonCandidate");
  TClonesArray* MuMinus = new TClonesArray("CbmAnaMuonCandidate");
  TTree* InputTree;

  TFile* f = new TFile(name);
  if (f->IsZombie() || f->GetNkeys() < 1 || f->TestBit(TFile::kRecovered)) f->Close();

  InputTree = (TTree*) f->Get("cbmsim");
  InputTree->SetBranchAddress("MuPlus", &MuPlus);
  InputTree->SetBranchAddress("MuMinus", &MuMinus);

  for (int iEvent = 0; iEvent < InputTree->GetEntries(); iEvent++) {

    InputTree->GetEntry(iEvent);

    int NofPlus  = MuPlus->GetEntriesFast();
    int NofMinus = MuMinus->GetEntriesFast();

    if (NofPlus == 0 || NofMinus == 0) continue;

    for (int iPart = 0; iPart < NofPlus; iPart++) {
      CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) MuPlus->At(iPart);
      TLorentzVector* P_pl       = mu_pl->GetMomentum();

      for (int jPart = 0; jPart < NofMinus; jPart++) {
        CbmAnaMuonCandidate* mu_mn = (CbmAnaMuonCandidate*) MuMinus->At(jPart);
        TLorentzVector* P_mn       = mu_mn->GetMomentum();
        TLorentzVector M(*P_pl + *P_mn);
        invM_sgn->Fill(M.M());
      }
    }
  }
  f->Close();

  if (invM_sgn->GetEntries() > 10) {
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
  }
}
