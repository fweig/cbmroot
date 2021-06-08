/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void pl_Eff_Mul(Int_t iDut = 910, Double_t dEffMin = 0.5, Double_t dEffMax = 1., TString sysinfo = "")
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
  TH1* h1f;
  TH1* h1m;
  TH1* h1all;
  TH2* h2;
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);
  Double_t Nfound  = 0.;
  Double_t Nmissed = 0.;
  gROOT->cd();
  hname = Form("hDutMul_Found_%03d", iDut);
  cout << " Look for histo " << hname << endl;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
    h1f    = (TH1*) h1->Clone();
    Nfound = h1f->GetEntries();
  }
  else {
    cout << hname << " not found" << endl;
  }

  hname = Form("hDutMul_Missed_%d", iDut);
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1m     = (TH1*) h1->Clone();
    Nmissed = h1m->GetEntries();
    h1m->Draw("same");
    h1m->SetLineColor(2);
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);

  h1m->Draw();
  h1f->Draw("same");

  can->cd(3);

  h1all = (TH1*) h1f->Clone();
  h1all->Add(h1m, h1f, 1., 1.);

  TEfficiency* pEffDut = new TEfficiency(*h1f, *h1all);
  pEffDut->SetTitle("Efficiency of DUT");
  pEffDut->Draw("AP");
  gPad->Update();

  auto graph = pEffDut->GetPaintedGraph();
  graph->SetMinimum(dEffMin);
  graph->SetMaximum(dEffMax);
  // graph->GetXaxis()->SetRangeUser(0.,10.);
  /*
 auto heff = pEffDut->GetPaintedHistogram();
 heff->SetMinimum(dEffMin);
 heff->SetMaximum(dEffMax); 
 */
  gPad->Update();
  gPad->SetGridx();
  gPad->SetGridy();

  Double_t dEff = Nfound / (Nfound + Nmissed);
  cout << "Average efficiency of Dut: " << dEff << endl;

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  gInterpreter->ProcessLine(FADD.Data());

  can->SaveAs(Form("pl_Eff_Mul_%03d.pdf", iDut));
}
