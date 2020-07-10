void pl_Eff_XY(Int_t iDut       = 910,
               Double_t dEffMin = 0.5,
               Double_t dEffMax = 1.,
               TString sysinfo  = "") {
  gROOT->LoadMacro(
    ((TString) gSystem->Getenv("VMCWORKDIR") + "/macro/beamtime/pl_Datime.C")
      .Data());
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 700, 700);
  can->Divide(2, 2);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h;
  TH1* h1;
  TH2* h2f;
  TH2* h2m;
  TH2* h2all;
  TH2* h2;
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);
  Double_t Nfound  = 0.;
  Double_t Nmissed = 0.;
  gROOT->cd();
  hname = Form("hDutXY_Found_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h2 = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2f    = (TH2*) h2->Clone();
    Nfound = h2f->GetEntries();
  } else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  hname = Form("hDutXY_Missed_%d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2m     = (TH2*) h2->Clone();
    Nmissed = h2m->GetEntries();
    h2m->Draw("colz");
  } else {
    cout << hname << " not found" << endl;
  }


  can->cd(3);

  h2all = (TH2*) h2f->Clone("hDutXY_all");
  h2all->Add(h2m, h2f, 1., 1.);

  TEfficiency* pEffDut = new TEfficiency(*h2f, *h2all);
  pEffDut->SetTitle("Efficiency of DUT");
  pEffDut->SetName("hDutXY_eff");
  pEffDut->Draw("colz");
  gPad->Update();

  auto h2Eff = pEffDut->GetPaintedHistogram();
  h2Eff->SetMinimum(dEffMin);
  h2Eff->SetMaximum(dEffMax);

  Double_t dEff = Nfound / (Nfound + Nmissed);
  cout << "Average efficiency of Dut: " << Form("%6.3f", dEff) << endl;

  can->cd(4);
  TPaveLabel* tit = new TPaveLabel(
    0.1, 0.1, 0.9, 0.9, Form(" average efficiency of %d: %5.3f", iDut, dEff));
  tit->SetFillColor(0);
  tit->SetTextFont(52);
  tit->SetBorderSize(0);
  tit->Draw();

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }

  can->SaveAs(Form("pl_Eff_XY_%d.pdf", iDut));
}
