//---------------------------------------------------
//
// invariant mass spectra for background
// using Super Event technics
//
// CbmAnaDimuonAnalysis has to be used with UseCuts(kTRUE)
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void InvariantMassSpectra_SE(Int_t energy = 8, Int_t NofFiles = 200)
{
  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(4);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(55);
  gStyle->SetPaintTextFormat("2.1f");

  Int_t NofBins  = 100;
  Int_t NofBinsM = 400;

  Double_t minY  = -2;
  Double_t maxY  = 6;
  Double_t min   = 0;
  Double_t maxPt = 10;
  Double_t maxM  = 4;

  TH1D* invM_bg = new TH1D("invM_bg", "invM_bg", NofBinsM, min, maxM);
  (invM_bg->GetXaxis())->SetTitle("m_{inv} (GeV/c^{2})");
  (invM_bg->GetYaxis())->SetTitle("counts/(events #times 10 MeV/c^{2})");
  invM_bg->SetLineColor(kRed);
  invM_bg->SetMarkerColor(kRed);
  invM_bg->SetMarkerStyle(20);

  TH3D* YPtM_bg = new TH3D("YPtM_bg", "YPtM_bg", NofBins, minY, maxY, NofBins, min, maxPt, NofBinsM, min, maxM);

  TString name;

  TClonesArray* MuPlus  = new TClonesArray("CbmAnaMuonCandidate");
  TClonesArray* MuMinus = new TClonesArray("CbmAnaMuonCandidate");
  TTree* InputTree;

  Double_t NofEvents;

  TLorentzVector P1, P2, M;
  TTree* Plus = new TTree("Plus", "part-");
  Plus->Branch("P0", &P1(0), "Px/D:Py:Pz:E", 10000000);

  TTree* Minus = new TTree("Minus", "part+");
  Minus->Branch("P0", &P2(0), "Px/D:Py:Pz:E", 10000000);

  for (int k = 1; k < NofFiles + 1; k++) {
    name.Form("/lustre/cbm/prod/mc/OCT19/sis100_muon_lmvm/%dgev/centr/%d/"
              "muons.ana.root",
              energy, k);

    TFile* f = new TFile(name);
    if (f->IsZombie() || f->GetNkeys() < 1 || f->TestBit(TFile::kRecovered)) {
      f->Close();
      continue;
    }

    if (k % 100 == 0) cout << "Input File " << k << " : " << f->GetName() << endl;

    InputTree = (TTree*) f->Get("cbmsim");
    if (!InputTree) {
      f->Close();
      continue;
    }

    InputTree->SetBranchAddress("MuPlus", &MuPlus);
    InputTree->SetBranchAddress("MuMinus", &MuMinus);

    //-----------------------------------------------------------

    for (int iEvent = 0; iEvent < InputTree->GetEntries(); iEvent++) {
      InputTree->GetEntry(iEvent);

      NofEvents++;

      int NofPlus  = MuPlus->GetEntriesFast();
      int NofMinus = MuMinus->GetEntriesFast();

      for (int iPart = 0; iPart < NofPlus; iPart++) {
        CbmAnaMuonCandidate* mu_pl = (CbmAnaMuonCandidate*) MuPlus->At(iPart);
        P1                         = *mu_pl->GetMomentum();
        Plus->Fill();
      }
      for (int jPart = 0; jPart < NofMinus; jPart++) {
        CbmAnaMuonCandidate* mu_mn = (CbmAnaMuonCandidate*) MuMinus->At(jPart);
        P2                         = *mu_mn->GetMomentum();
        Minus->Fill();
      }
    }
    f->Close();
  }

  Plus->SetBranchAddress("P0", &P1(0));
  Minus->SetBranchAddress("P0", &P2(0));

  Double_t entriesPlus  = Plus->GetEntries();
  Double_t entriesMinus = Minus->GetEntries();

  for (int jPart = 0; jPart < entriesPlus; jPart++) {
    if (jPart % 1000 == 0) cout << entriesPlus << " ----- " << jPart << endl;
    Plus->GetEntry(jPart);
    for (int iPart = 0; iPart < entriesMinus; iPart++) {
      Minus->GetEntry(iPart);
      M = P1 + P2;
      invM_bg->Fill(M.M(), 1. / 10.);
      YPtM_bg->Fill(M.Rapidity(), M.Pt(), M.M());
    }
  }

  invM_bg->Scale(1. / NofEvents / NofEvents);
  YPtM_bg->Scale(1. / NofEvents / NofEvents);

  name.Form("invM_bg_SE_%d.root", energy);
  TFile* FFF = new TFile(name, "recreate");
  invM_bg->Write();
  YPtM_bg->Write();
  FFF->Close();
}
