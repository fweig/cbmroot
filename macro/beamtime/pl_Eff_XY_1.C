void pl_Eff_XY_1(Int_t iDut = 910, Double_t dEffMin = 0.5, Double_t dEffMax = 1., Double_t dThr = 0.01,
                 TString sysinfo = "")
{
  gROOT->LoadMacro("pl_Datime.C");
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
  TH2* h2acc;
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
  hname = Form("hDutXY_Found_%03d", iDut);
  cout << " Look for histo " << hname << endl;
  h2 = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2f   = (TH2*) h2->Clone();
    h2acc = (TH2*) h2->Clone("Dut acceptance");
    h2acc->Reset();
    Int_t Nbins   = h2->GetNbinsX() * h2->GetNbinsY();
    Double_t dMax = dThr * h2->GetMaximum();
    for (Int_t i = 0; i < Nbins; i++)
      h2->GetBinContent(i + 1) < dMax ? h2acc->SetBinContent(i + 1, 0.) : h2acc->SetBinContent(i + 1, 1.);
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  hname = Form("hDutXY_Missed_%03d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2m = (TH2*) h2->Clone();
    h2m->Draw("colz");
  }
  else {
    cout << hname << " not found" << endl;
  }


  can->cd(3);
  TH2* h2fA = (TH2*) h2f->Clone("hDutXY_FoundAcc");
  h2fA->Multiply(h2fA, h2acc, 1., 1., "B");
  Nfound = h2fA->GetEntries();

  TH2* h2mA = (TH2*) h2m->Clone("hDutXY_MissedAcc");
  h2mA->Multiply(h2mA, h2acc, 1., 1., "B");
  Nmissed = h2mA->GetEntries();

  h2all = (TH2*) h2f->Clone("hDutXY_all");
  h2all->Add(h2mA, h2fA, 1., 1.);

  TEfficiency* pEffDut = new TEfficiency(*h2fA, *h2all);
  pEffDut->SetTitle("Efficiency of DUT");
  pEffDut->SetName("hDutXY_eff");
  pEffDut->Draw("colz");
  gPad->Update();

  auto h2Eff = pEffDut->GetPaintedHistogram();
  h2Eff->SetMinimum(dEffMin);
  h2Eff->SetMaximum(dEffMax);

  Double_t dEff = Nfound / (Nfound + Nmissed);
  cout << Form("Average efficiency of Dut in acceptance: with thr %5.2f: %6.3f", dThr, dEff) << endl;

  can->cd(4);
  TPaveLabel* tit = new TPaveLabel(0.1, 0.1, 0.9, 0.9, Form(" average efficiency of %03d: %5.3f", iDut, dEff));
  tit->SetFillColor(0);
  tit->SetTextFont(52);
  tit->SetBorderSize(0);
  tit->Draw();

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  gInterpreter->ProcessLine(FADD.Data());

  can->SaveAs(Form("pl_Eff_XY_%03d.pdf", iDut));
}
