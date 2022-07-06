/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void pl_Eff_TIR(Int_t iDut = 900, Double_t dEffMin = 0., Double_t dEffMax = 1., Int_t iBl = 0, Int_t iBh = 8,
                Double_t TIRmin = 0., Double_t TIRmax = 30., TString sysinfo = "")
{
  gROOT->LoadMacro("pl_Datime.C");
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 900, 800);
  can->Divide(1, 3);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH1* h1f;
  TH1* h1m;
  TH1* h1all;
  TH2* h2f;
  TH2* h2m;
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);
  gPad->Divide(2, 1);
  gPad->cd(1);
  Double_t Nfound  = 0.;
  Double_t Nmissed = 0.;
  gROOT->cd();
  hname = Form("hDutTIR_Found_%03d", iDut);
  cout << " Look for histo " << hname << endl;
  h2 = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2->GetXaxis()->SetRangeUser(TIRmin, TIRmax);

    gPad->SetLogz();
    h2f    = (TH2*) h2->Clone();
    Nfound = h2f->GetEntries();
    h1f    = (TH1*) h2f->ProjectionX("_px", iBl + 1, iBh + 1);
  }
  else {
    cout << hname << " not found" << endl;
    return;
  }

  can->cd(1);
  // gPad->Divide(2,1);
  gPad->cd(2);
  hname = Form("hDutTIR_Missed_%03d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2m     = (TH2*) h2->Clone();
    Nmissed = h2m->GetEntries();
    h2m->Draw("colz");
    h2m->GetXaxis()->SetRangeUser(TIRmin, TIRmax);

    gPad->SetLogz();
    h1m = (TH1*) h2m->ProjectionX("_px", iBl + 1, iBh + 1);
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  h1all = (TH1*) h1f->Clone("hDutTIR_all");
  h1all->Add(h1m, h1f, 1., 1.);
  h1all->SetMinimum(h1all->GetMaximum() * 1.E-4);
  h1all->Draw();
  h1all->SetLineColor(kBlack);
  h1f->Draw("same");
  h1f->SetLineColor(kBlue);
  h1m->Draw("same");
  h1m->SetLineColor(kRed);
  gPad->SetLogy();
  h1all->GetXaxis()->SetRangeUser(TIRmin, TIRmax);
  h1all->GetXaxis()->SetTitle("Time (s)");
  gPad->Update();

  can->cd(3);

  TEfficiency* pEffDut = new TEfficiency(*h1f, *h1all);
  pEffDut->SetTitle(Form("Efficiency of DUT, GET4 %d - %d ", iBl, iBh));
  pEffDut->SetName("hDutTIR_eff");
  pEffDut->Draw("AP");
  gPad->Update();

  auto graph = pEffDut->GetPaintedGraph();
  graph->SetMinimum(dEffMin);
  graph->SetMaximum(dEffMax);
  graph->GetXaxis()->SetRangeUser(TIRmin, TIRmax);
  /*
 auto heff = pEffDut->GetPaintedHistogram();
 heff->SetMinimum(dEffMin);
 heff->SetMaximum(dEffMax); 
 */
  gPad->Update();
  gPad->SetGridx();
  gPad->SetGridy();

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  gInterpreter->ProcessLine(FADD.Data());

  can->SaveAs(Form("pl_Eff_TIR_%03d.pdf", iDut));
}
