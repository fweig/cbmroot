/* Copyright (C) 2010 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Evgeny Kryshen [committer] */

{
  Double_t scale = 1.388;  //Scale bgd with this factor

  TFile* f0 = new TFile("histo_bgd.root", "read");

  fhBgdB1B2      = new TH2D(*hBgdB1B2);
  fhSigB1B2      = new TH2D(*hSigB1B2);
  fhBgdB1Chi2    = new TH2D(*hBgdB1Chi2);
  fhSigB1Chi2    = new TH2D(*hSigB1Chi2);
  fhBgdChi1Chi2  = new TH2D(*hBgdChi1Chi2);
  fhSigChi1Chi2  = new TH2D(*hSigChi1Chi2);
  fhBgdChiDca    = new TH2D(*hBgdChiDca);
  fhSigChiDca    = new TH2D(*hSigChiDca);
  fhBgdPcaBmo    = new TH2D(*hBgdPcaBmo);
  fhSigPcaBmo    = new TH2D(*hSigPcaBmo);
  fhBgdM         = new TH1D(*hBgdM);
  fhSigM         = new TH1D(*hSigM);
  fhBgdB1Chi2M   = new TH1D(*hBgdB1Chi2M);
  fhSigB1Chi2M   = new TH1D(*hSigB1Chi2M);
  fhBgdChi1Chi2M = new TH1D(*hBgdChi1Chi2M);
  fhSigChi1Chi2M = new TH1D(*hSigChi1Chi2M);
  fhBgdB1B2M     = new TH1D(*hBgdB1B2M);
  fhSigB1B2M     = new TH1D(*hSigB1B2M);
  fhBgdChiDcaM   = new TH1D(*hBgdChiDcaM);
  fhSigChiDcaM   = new TH1D(*hSigChiDcaM);
  fhBgdPcaBmoM   = new TH1D(*hBgdPcaBmoM);
  fhSigPcaBmoM   = new TH1D(*hSigPcaBmoM);

  for (Int_t i = 0; i < 100; i++) {
    char fileName[40];
    sprintf(fileName, "histo.%02i.root", i);
    TFile* f = TFile::Open(fileName, "read");
    if (!f) continue;
    hBgdB1B2      = (TH2D*) f->Get("hBgdB1B2");
    hSigB1B2      = (TH2D*) f->Get("hSigB1B2");
    hBgdB1Chi2    = (TH2D*) f->Get("hBgdB1Chi2");
    hSigB1Chi2    = (TH2D*) f->Get("hSigB1Chi2");
    hBgdChi1Chi2  = (TH2D*) f->Get("hBgdChi1Chi2");
    hSigChi1Chi2  = (TH2D*) f->Get("hSigChi1Chi2");
    hBgdChiDca    = (TH2D*) f->Get("hBgdChiDca");
    hSigChiDca    = (TH2D*) f->Get("hSigChiDca");
    hBgdPcaBmo    = (TH2D*) f->Get("hBgdPcaBmo");
    hSigPcaBmo    = (TH2D*) f->Get("hSigPcaBmo");
    hBgdM         = (TH1D*) f->Get("hBgdM");
    hSigM         = (TH1D*) f->Get("hSigM");
    hBgdB1Chi2M   = (TH1D*) f->Get("hBgdB1Chi2M");
    hSigB1Chi2M   = (TH1D*) f->Get("hSigB1Chi2M");
    hBgdChi1Chi2M = (TH1D*) f->Get("hBgdChi1Chi2M");
    hSigChi1Chi2M = (TH1D*) f->Get("hSigChi1Chi2M");
    hBgdB1B2M     = (TH1D*) f->Get("hBgdB1B2M");
    hSigB1B2M     = (TH1D*) f->Get("hSigB1B2M");
    hBgdChiDcaM   = (TH1D*) f->Get("hBgdChiDcaM");
    hSigChiDcaM   = (TH1D*) f->Get("hSigChiDcaM");
    hBgdPcaBmoM   = (TH1D*) f->Get("hBgdPcaBmoM");
    hSigPcaBmoM   = (TH1D*) f->Get("hSigPcaBmoM");
    fhBgdB1B2->Add(hBgdB1B2);
    fhSigB1B2->Add(hSigB1B2);
    fhBgdB1Chi2->Add(hBgdB1Chi2);
    fhSigB1Chi2->Add(hSigB1Chi2);
    fhBgdChi1Chi2->Add(hBgdChi1Chi2);
    fhSigChi1Chi2->Add(hSigChi1Chi2);
    fhBgdChiDca->Add(hBgdChiDca);
    fhSigChiDca->Add(hSigChiDca);
    fhBgdPcaBmo->Add(hBgdPcaBmo);
    fhSigPcaBmo->Add(hSigPcaBmo);
    fhBgdM->Add(hBgdM);
    fhSigM->Add(hSigM);
    fhBgdB1Chi2M->Add(hBgdB1Chi2M);
    fhSigB1Chi2M->Add(hSigB1Chi2M);
    fhBgdChi1Chi2M->Add(hBgdChi1Chi2M);
    fhSigChi1Chi2M->Add(hSigChi1Chi2M);
    fhBgdB1B2M->Add(hBgdB1B2M);
    fhSigB1B2M->Add(hSigB1B2M);
    fhBgdChiDcaM->Add(hBgdChiDcaM);
    fhSigChiDcaM->Add(hSigChiDcaM);
    fhBgdPcaBmoM->Add(hBgdPcaBmoM);
    fhSigPcaBmoM->Add(hSigPcaBmoM);
    f->Close();
  }
  fhBgdB1B2->Scale(scale);
  fhBgdB1Chi2->Scale(scale);
  fhBgdChi1Chi2->Scale(scale);
  fhBgdChiDca->Scale(scale);
  fhBgdPcaBmo->Scale(scale);
  fhBgdM->Scale(scale);
  fhBgdB1Chi2M->Scale(scale);
  fhBgdChi1Chi2M->Scale(scale);
  fhBgdB1B2M->Scale(scale);
  fhBgdChiDcaM->Scale(scale);
  fhBgdPcaBmoM->Scale(scale);

  TFile* f = new TFile("histo.root", "recreate");
  fhBgdB1B2->Write();
  fhSigB1B2->Write();
  fhBgdB1Chi2->Write();
  fhSigB1Chi2->Write();
  fhBgdChi1Chi2->Write();
  fhSigChi1Chi2->Write();
  fhBgdChiDca->Write();
  fhSigChiDca->Write();
  fhBgdPcaBmo->Write();
  fhSigPcaBmo->Write();
  fhBgdM->Write();
  fhSigM->Write();
  fhBgdB1Chi2M->Write();
  fhSigB1Chi2M->Write();
  fhBgdChi1Chi2M->Write();
  fhSigChi1Chi2M->Write();
  fhBgdB1B2M->Write();
  fhSigB1B2M->Write();
  fhBgdChiDcaM->Write();
  fhSigChiDcaM->Write();
  fhBgdPcaBmoM->Write();
  fhSigPcaBmoM->Write();
  f->Close();
}
