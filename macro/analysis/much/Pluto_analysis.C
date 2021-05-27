//---------------------------------------------------
//
// PLUTO parameters of low-mass vector mesons and thermal muons @ 8 A GeV/c
// and J/psi muons @ 10 A GeV/c
/*
   Particles:
   0 - rho
   1 - omega
   2 - omegaD
   3 - phi
   4 - eta
   5 - etaD
   6 - qgp
   7 - J/psi, but now only 10 A GeV/c
*/
//
// TODO macro for all SIS100 energies
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------
void Pluto_analysis(int part = 0, int energy = 8, int NofFiles = 5000)
{
  TDatabasePDG* dataBase = TDatabasePDG::Instance();
  Int_t iEvent;
  Double_t nEvents;

  gStyle->SetCanvasColor(10);
  gStyle->SetFrameFillColor(10);
  gStyle->SetHistLineWidth(6);
  gStyle->SetPadColor(10);
  gStyle->SetStatColor(10);
  gStyle->SetPalette(1);

  TClonesArray* fParticles = new TClonesArray("PParticle", 100);

  TString partName;
  TString partpartName;
  TString chanName;
  TString cktName;

  chanName.Form("mpmm");
  cktName.Form("cktA");

  TString sgn[]    = {"rho0", "omega", "omegaD", "phi", "eta", "etaD", "rapp.qgp", "jpsi"};
  TString sgnsgn[] = {"rho0", "omega", "omega", "phi", "eta", "eta", "rapp.qgp", "jpsi"};

  partName     = sgn[part];
  partpartName = sgnsgn[part];
  if (part == 2) chanName.Form("pi0mpmm");
  if (part == 5) chanName.Form("gmpmm");
  if (part == 6) cktName.Form("cktRapp");
  if (part == 7) {
    cktName.Form("charm");
    energy = 10;
  }

  Double_t brOmega  = 9e-5;
  Double_t brOmegaD = 1.3e-4;
  Double_t brEta    = 5.8e-6;
  Double_t brEtaD   = 3.1e-4;
  Double_t brRho    = 4.55e-5;
  Double_t brPhi    = 2.87e-4;
  Double_t brJPsi   = 0.06;

  Double_t multOmega  = 19.;
  Double_t multOmegaD = 19.;
  Double_t multEta    = 16.;
  Double_t multEtaD   = 16.;
  Double_t multRho    = 9.;
  Double_t multPhi    = 0.12;
  Double_t multQGP    = 3.2e-3;
  Double_t multJPsi   = 1e-6;

  Double_t partMulti[] = {multRho * brRho, multOmega * brOmega, multOmegaD * brOmegaD,
                          multPhi * brPhi, multEta * brEta,     multEtaD * brEtaD,
                          multQGP,         multJPsi * brJPsi};
  Double_t multi[]     = {multRho, multOmega, multOmegaD, multPhi, multEta, multEtaD, 1, multJPsi};

  cout << "**********************" << endl;
  cout << partName << " - " << partMulti[part] << endl;
  cout << "**********************" << endl;


  Int_t NofBins  = 100;
  Int_t NofBinsM = 400;

  Double_t minY  = -2;
  Double_t maxY  = 6;
  Double_t min   = 0;
  Double_t maxPt = 10;
  Double_t maxP  = 20;
  Double_t maxM  = 4;

  //----------------------------------
  //----------------------------------
  TString histoName;
  histoName.Form("YPt_%s", partName.Data());

  TH2D* YPt_part = new TH2D(histoName.Data(), histoName.Data(), NofBins, minY, maxY, NofBins, min, maxPt);
  (YPt_part->GetXaxis())->SetTitle("y");
  (YPt_part->GetYaxis())->SetTitle("p_{t} (GeV/c)");
  (YPt_part->GetZaxis())->SetTitle("counts/events");

  //----------------------------------
  //----------------------------------
  histoName.Form("invM_%s", partName.Data());

  TH1D* invM_part = new TH1D(histoName.Data(), histoName.Data(), NofBinsM, min, maxM);
  (invM_part->GetXaxis())->SetTitle("m_{inv}(GeV/c^{2}");
  (invM_part->GetYaxis())->SetTitle("counts/(events #times 10 MeV/c^{2})");
  invM_part->SetLineColor(kRed);

  //----------------------------------
  //----------------------------------
  histoName.Form("P_%s", partName.Data());

  TH1D* P_part = new TH1D(histoName.Data(), histoName.Data(), NofBins, min, maxP);
  (P_part->GetXaxis())->SetTitle("p (GeV/c)");
  (P_part->GetYaxis())->SetTitle("counts/events");
  P_part->SetLineColor(kRed);

  //----------------------------------
  //----------------------------------
  histoName.Form("Pt_%s", partName.Data());

  TH1D* Pt_part = new TH1D(histoName.Data(), histoName.Data(), NofBins, min, maxPt);
  (Pt_part->GetXaxis())->SetTitle("p_{t} (GeV/c)");
  (Pt_part->GetYaxis())->SetTitle("counts/events");
  Pt_part->SetLineColor(kRed);

  //----------------------------------
  //----------------------------------
  histoName.Form("Y_%s", partName.Data());

  TH1D* Y_part = new TH1D(histoName.Data(), histoName.Data(), NofBins, minY, maxY);
  (Y_part->GetXaxis())->SetTitle("y");
  (Y_part->GetYaxis())->SetTitle("counts/events");
  Y_part->SetLineColor(kRed);

  //----------------------------------
  //----------------------------------
  histoName.Form("YPtM_%s", partName.Data());

  TH3D* YPtM_part =
    new TH3D(histoName.Data(), histoName.Data(), NofBins, minY, maxY, NofBins, min, maxPt, NofBinsM, min, maxM);

  //----------------------------------

  for (int i = 1; i < NofFiles + 1; i++) {
    TString fileName;

    char str[4];
    sprintf(str, "%4d", i);
    for (int it = 0; it < 4; it++) {
      if (' ' == str[it]) str[it] = '0';
    }
    fileName.Form("/lustre/cbm/prod/gen/pluto/auau/%s/%dgev/%s/%s/"
                  "pluto.auau.%dgev.%s.%s.%s.root",
                  cktName.Data(), energy, partpartName.Data(), chanName.Data(), energy, partpartName.Data(),
                  chanName.Data(), str);
    TFile* InputFile = new TFile(fileName.Data());
    TTree* InputTree = (TTree*) InputFile->Get("data");
    InputTree->SetBranchAddress("Particles", &fParticles);

    if (i % 100 == 0) cout << "-----------> " << fileName << endl;

    for (iEvent = 0; iEvent < InputTree->GetEntries(); iEvent++) {
      nEvents++;

      InputTree->GetEntry(iEvent);

      PParticle* Part1 = (PParticle*) fParticles->At(1);
      PParticle* Part2 = (PParticle*) fParticles->At(2);

      if (part == 2 || part == 5) {
        Part1 = (PParticle*) fParticles->At(2);
        Part2 = (PParticle*) fParticles->At(3);
      }

      TLorentzVector mom1 = Part1->Vect4();
      TLorentzVector mom2 = Part2->Vect4();

      TLorentzVector Mom = mom1 + mom2;
      YPt_part->Fill(Mom.Rapidity(), Mom.Pt());
      Y_part->Fill(Mom.Rapidity());
      Pt_part->Fill(Mom.Pt());
      P_part->Fill(Mom.P());
      invM_part->Fill(Mom.M(), 1. / 10);
      YPtM_part->Fill(Mom.Rapidity(), Mom.Pt(), Mom.M());
    }

    InputFile->Close();
  }


  invM_part->Scale(partMulti[part] / nEvents);
  YPtM_part->Scale(multi[part] / nEvents);

  P_part->Scale(multi[part] / nEvents);
  Pt_part->Scale(multi[part] / nEvents);
  Y_part->Scale(multi[part] / nEvents);
  YPt_part->Scale(multi[part] / nEvents);

  TString name;
  name.Form("%dgev.pluto.%s.root", energy, partName.Data());
  TFile* FFF = new TFile(name.Data(), "recreate");
  invM_part->Write();
  P_part->Write();
  Pt_part->Write();
  Y_part->Write();
  YPt_part->Write();
  YPtM_part->Write();
  FFF->Close();
}
