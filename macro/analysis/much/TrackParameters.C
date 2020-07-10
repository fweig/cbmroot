//---------------------------------------------------
//
// parameters of muon track candidates
//
// CbmAnaDimuonAnalysis has to be used with UseCuts(kFALSE)
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void TrackParameters(Int_t energy   = 8,
                     Int_t NofFiles = 1000,
                     TString dir    = "/lustre/cbm/prod/mc/OCT19") {
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(4);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("2.1f");

  TH1D *chi2Vertex[2], *chi2STS[2], *chi2MUCH[2], *NofSTS[2], *NofMUCH[2],
    *NofTRD[2];

  chi2Vertex[0] = new TH1D(
    "chi2Vertex_omega", "#chi^{2}/ndf in vertex for omega muons", 200, 0, 20);
  chi2Vertex[0]->GetXaxis()->SetTitle("#chi^{2}/ndf");
  chi2Vertex[0]->GetYaxis()->SetTitle("counts/event");
  chi2Vertex[0]->SetLineColor(kRed);
  chi2Vertex[0]->SetMarkerColor(kRed);
  chi2Vertex[0]->SetMarkerStyle(20);

  chi2Vertex[1] = (TH1D*) chi2Vertex[0]->Clone("chi2Vertex_bg");
  chi2Vertex[1]->SetTitle("#chi^{2}/ndf in vertex for background");
  chi2Vertex[1]->SetLineColor(kBlack);
  chi2Vertex[1]->SetMarkerColor(kBlack);

  chi2STS[0] = (TH1D*) chi2Vertex[0]->Clone("chi2STS_omega");
  chi2STS[0]->SetTitle("#chi^{2}/ndf of STS track for omega muons");

  chi2STS[1] = (TH1D*) chi2Vertex[1]->Clone("chi2STS_bg");
  chi2STS[1]->SetTitle("#chi^{2}/ndf of STS track for background");

  chi2MUCH[0] = (TH1D*) chi2Vertex[0]->Clone("chi2MUCH_omega");
  chi2MUCH[0]->SetTitle("#chi^{2}/ndf of MUCH track for omega muons");

  chi2MUCH[1] = (TH1D*) chi2Vertex[1]->Clone("chi2MUCH_bg");
  chi2MUCH[1]->SetTitle("#chi^{2}/ndf of MUCH track for background");

  NofSTS[0] = new TH1D(
    "NofSTS_omega", "number of STS hits for omega muons", 16, -0.5, 15.5);
  NofSTS[0]->GetXaxis()->SetTitle("number of hits");
  NofSTS[0]->GetYaxis()->SetTitle("counts/event");
  NofSTS[0]->SetLineColor(kRed);
  NofSTS[0]->SetMarkerColor(kRed);
  NofSTS[0]->SetMarkerStyle(20);

  NofSTS[1] = (TH1D*) NofSTS[0]->Clone("NofSTS_bg");
  NofSTS[1]->SetTitle("number of STS hits for background");
  NofSTS[1]->SetLineColor(kBlack);
  NofSTS[1]->SetMarkerColor(kBlack);

  NofMUCH[0] = (TH1D*) NofSTS[0]->Clone("NofMUCH_omega");
  NofMUCH[0]->SetTitle("number of MUCH hits for omega muons");

  NofMUCH[1] = (TH1D*) NofSTS[1]->Clone("NofMUCH_bg");
  NofMUCH[1]->SetTitle("number of MUCH hits for background");

  NofTRD[0] = (TH1D*) NofSTS[0]->Clone("NofTRD_omega");
  NofTRD[0]->SetTitle("number of TRD hits for omega muons");

  NofTRD[1] = (TH1D*) NofSTS[1]->Clone("NofTRD_bg");
  NofTRD[1]->SetTitle("number of TRD hits for background");

  TString name;

  TClonesArray* MuPlus  = new TClonesArray("CbmAnaMuonCandidate");
  TClonesArray* MuMinus = new TClonesArray("CbmAnaMuonCandidate");
  TTree* InputTree;

  Double_t NofEvents[2];

  //--------------------------------------------------------
  for (int i = 0; i < 2; i++)
    for (int k = 1; k < NofFiles + 1; k++) {
      if (i == 0)
        name.Form("%s/sis100_muon_lmvm/%dgev/omega/%d/muons.ana.root",
                  dir.Data(),
                  energy,
                  k);
      else
        name.Form("%s/sis100_muon_lmvm/%dgev/centr/%d/muons.ana.root",
                  dir.Data(),
                  energy,
                  k);

      TFile* f = new TFile(name);
      if (f->IsZombie() || f->GetNkeys() < 1 || f->TestBit(TFile::kRecovered)) {
        f->Close();
        continue;
      }

      if (k % 100 == 0)
        cout << "Input File " << k << " : " << f->GetName() << endl;

      InputTree = (TTree*) f->Get("cbmsim");
      InputTree->SetBranchAddress("MuPlus", &MuPlus);
      InputTree->SetBranchAddress("MuMinus", &MuMinus);

      //-----------------------------------------------------------

      for (int iEvent = 0; iEvent < InputTree->GetEntries(); iEvent++) {
        InputTree->GetEntry(iEvent);

        NofEvents[i]++;

        int NofPlus  = MuPlus->GetEntriesFast();
        int NofMinus = MuMinus->GetEntriesFast();
        for (int iPart = 0; iPart < NofPlus; iPart++) {
          Bool_t trigger = kFALSE;

          CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) MuPlus->At(iPart);

          if ((i == 0 && mu_pl->GetTrueMu() == 1) || i == 1) trigger = kTRUE;
          if (!trigger) continue;

          chi2Vertex[i]->Fill(mu_pl->GetChiToVertex());
          chi2STS[i]->Fill(mu_pl->GetChiSts());
          chi2MUCH[i]->Fill(mu_pl->GetChiMuch());

          NofSTS[i]->Fill(mu_pl->GetNStsHits());
          NofMUCH[i]->Fill(mu_pl->GetNMuchHits());
          NofTRD[i]->Fill(mu_pl->GetNTrdHits());
        }
        for (int jPart = 0; jPart < NofMinus; jPart++) {
          Bool_t trigger = kFALSE;

          CbmAnaMuonCandidate* mu_mn =
            (CbmAnaMuonCandidate*) MuMinus->At(jPart);

          if ((i == 0 && mu_mn->GetTrueMu() == 1) || i == 1) trigger = kTRUE;
          if (!trigger) continue;

          chi2Vertex[i]->Fill(mu_mn->GetChiToVertex());
          chi2STS[i]->Fill(mu_mn->GetChiSts());
          chi2MUCH[i]->Fill(mu_mn->GetChiMuch());

          NofSTS[i]->Fill(mu_mn->GetNStsHits());
          NofMUCH[i]->Fill(mu_mn->GetNMuchHits());
          NofTRD[i]->Fill(mu_mn->GetNTrdHits());
        }
      }
      f->Close();
    }


  name.Form("TrackParameters_bg_omega_%dgev.root", energy);
  TFile* FFF = new TFile(name, "recreate");

  for (int i = 0; i < 2; i++) {
    /*
  chi2Vertex[i]->Scale(1./chi2Vertex[i]->Integral());
  chi2STS[i]->Scale(1./chi2STS[i]->Integral());
  chi2MUCH[i]->Scale(1./chi2MUCH[i]->Integral());
  
  NofSTS[i]->Scale(1./NofSTS[i]->Integral());
  NofMUCH[i]->Scale(1./NofMUCH[i]->Integral());
  NofTRD[i]->Scale(1./NofTRD[i]->Integral());
   */


    chi2Vertex[i]->Scale(1. / NofEvents[i]);
    chi2STS[i]->Scale(1. / NofEvents[i]);
    chi2MUCH[i]->Scale(1. / NofEvents[i]);

    NofSTS[i]->Scale(1. / NofEvents[i]);
    NofMUCH[i]->Scale(1. / NofEvents[i]);
    NofTRD[i]->Scale(1. / NofEvents[i]);

    chi2Vertex[i]->Write();
    chi2STS[i]->Write();
    chi2MUCH[i]->Write();
    NofSTS[i]->Write();
    NofMUCH[i]->Write();
    NofTRD[i]->Write();
  }
  FFF->Close();
}
