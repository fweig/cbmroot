/** @file add_histo.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Anna Senger [committer] **/

void add_histo(Int_t NofFiles = 5000, TString type = "sub_histo", TString dir = "sis100_muon_lmvm/8gev/centr_010")
{
  TString name;
  name.Form("%s/1/%s.root", dir.Data(), type.Data());
  TFile* f = new TFile(name);

  if (type == "sup_histo") {

    TH1D* h0 = (TH1D*) f->Get("h0");
    TH1D* h1 = (TH1D*) f->Get("h1");
    h1->SetLineColor(kBlack);
    TH1D* h2 = (TH1D*) f->Get("h2");
    h2->SetLineColor(kRed);
    TH1D* h3 = (TH1D*) f->Get("h3");
    h3->SetLineColor(kBlue);
    TH1D* h4 = (TH1D*) f->Get("h4");
    h4->SetLineColor(kGreen);
    TH1D* h5 = (TH1D*) f->Get("h5");
    h5->SetLineColor(kMagenta);

    TH1D* h1a = (TH1D*) h1->Clone("BgSup_Vtx");
    h1a->Reset();
    TH1D* h2a = (TH1D*) h2->Clone("BgSup_VtxSts");
    h2a->Reset();
    TH1D* h3a = (TH1D*) h3->Clone("BgSup_VtxStsMuch");
    h3a->Reset();
    TH1D* h4a = (TH1D*) h4->Clone("BgSup_VtxStsMuchTrd");
    h4a->Reset();
    TH1D* h5a = (TH1D*) h5->Clone("BgSup_VtxStsMuchTrdTof");
    h5a->Reset();

    for (int k = 1; k < NofFiles + 1; k++) {
      name.Form("%s/%d/%s.root", dir.Data(), k, type.Data());
      if (k % 100 == 0) cout << k << " - " << name << endl;

      TFile* f1 = new TFile(name);
      if (!f1 || f1->IsZombie() || f1->GetNkeys() < 1 || f1->TestBit(TFile::kRecovered)) {
        f1->Close();
        continue;
      }

      h0->Add((TH1D*) f1->Get("h0"));
      h1->Add((TH1D*) f1->Get("h1"));
      h2->Add((TH1D*) f1->Get("h2"));
      h3->Add((TH1D*) f1->Get("h3"));
      h4->Add((TH1D*) f1->Get("h4"));
      h5->Add((TH1D*) f1->Get("h5"));

      f1->Close();
    }

    h1a->Divide(h0, h1);
    h2a->Divide(h0, h2);
    h3a->Divide(h0, h3);
    h4a->Divide(h0, h4);
    h5a->Divide(h0, h5);

    name.Form("%s_%s.root", dir.Data(), type.Data());
    TFile* FFF = new TFile(name, "recreate");
    h1a->Write();
    h2a->Write();
    h3a->Write();
    h4a->Write();
    h5a->Write();

    FFF->Close();
  }
  else if (type == "YPt_histo") {

    TH2D* h0 = (TH2D*) f->Get("YPt_pluto");

    TH2D* h1 = (TH2D*) f->Get("YPt_StsAcc");
    TH2D* h2 = (TH2D*) f->Get("YPt_StsMuchAcc");
    TH2D* h3 = (TH2D*) f->Get("YPt_StsMuchTrdAcc");
    TH2D* h4 = (TH2D*) f->Get("YPt_StsMuchTrdTofAcc");

    TH2D* h0a = (TH2D*) f->Get("YPt_VtxReco");
    TH2D* h1a = (TH2D*) f->Get("YPt_VtxStsReco");
    TH2D* h2a = (TH2D*) f->Get("YPt_VtxStsMuchReco");
    TH2D* h3a = (TH2D*) f->Get("YPt_VtxStsMuchTrdReco");
    TH2D* h4a = (TH2D*) f->Get("YPt_VtxStsMuchTrdTofReco");

    Double_t N = 1;

    for (int k = 1; k < NofFiles + 1; k++) {
      name.Form("%s/%d/%s.root", dir.Data(), k, type.Data());
      if (k % 100 == 0) cout << k << " - " << name << endl;

      TFile* f1 = new TFile(name);
      if (!f1 || f1->IsZombie() || f1->GetNkeys() < 1 || f1->TestBit(TFile::kRecovered)) {
        f1->Close();
        continue;
      }
      N++;
      h0->Add((TH2D*) f1->Get("YPt_pluto"));

      h1->Add((TH2D*) f1->Get("YPt_StsAcc"));
      h2->Add((TH2D*) f1->Get("YPt_StsMuchAcc"));
      h3->Add((TH2D*) f1->Get("YPt_StsMuchTrdAcc"));
      h4->Add((TH2D*) f1->Get("YPt_StsMuchTrdTofAcc"));

      h0a->Add((TH2D*) f1->Get("YPt_VtxReco"));
      h1a->Add((TH2D*) f1->Get("YPt_VtxStsReco"));
      h2a->Add((TH2D*) f1->Get("YPt_VtxStsMuchReco"));
      h3a->Add((TH2D*) f1->Get("YPt_VtxStsMuchTrdReco"));
      h4a->Add((TH2D*) f1->Get("YPt_VtxStsMuchTrdTofReco"));

      f1->Close();
    }

    h0->Scale(1. / N);
    h1->Scale(1. / N);
    h2->Scale(1. / N);
    h3->Scale(1. / N);
    h4->Scale(1. / N);

    h0a->Scale(1. / N);
    h1a->Scale(1. / N);
    h2a->Scale(1. / N);
    h3a->Scale(1. / N);
    h4a->Scale(1. / N);

    name.Form("%s_%s.root", dir.Data(), type.Data());
    TFile* FFF = new TFile(name, "recreate");
    h0->Write();
    h1->Write();
    h2->Write();
    h3->Write();
    h4->Write();

    h0a->Write();
    h1a->Write();
    h2a->Write();
    h3a->Write();
    h4a->Write();

    FFF->Close();
  }
  else if (type == "YPtM") {

    TH3D* h1   = (TH3D*) f->Get("YPtM");
    Double_t N = 1;

    for (int k = 1; k < NofFiles + 1; k++) {
      name.Form("%s/%d/%s.root", dir.Data(), k, type.Data());
      if (k % 100 == 0) cout << k << " - " << name << endl;

      TFile* f1 = new TFile(name);
      if (!f1 || f1->IsZombie() || f1->GetNkeys() < 1 || f1->TestBit(TFile::kRecovered)) {
        f1->Close();
        continue;
      }
      N++;
      h1->Add((TH3D*) f1->Get("YPtM"));
      f1->Close();
    }

    h1->Scale(1. / N);

    name.Form("%s_%s.root", dir.Data(), type.Data());
    TFile* FFF = new TFile(name, "recreate");
    h1->Write();
    FFF->Close();
  }
  f->Close();
}
