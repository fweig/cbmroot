/* Copyright (C) 2018 Physikalisches Institut, Universitaet Heidelberg, Heidelberg
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christian Simon [committer] */

void pl_Eff_TIS(Int_t iDut = 910, Double_t dEffMin = 0.5, Double_t dEffMax = 1., TString sysinfo = "")
{
  gROOT->LoadMacro(((TString) gSystem->Getenv("VMCWORKDIR") + "/macro/beamtime/pl_Datime.C").Data());
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 450, 600);
  can->Divide(2, 3);

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
  TH2* h2f;
  TH2* h2m;
  TH2* h2all;

  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);
  Double_t Nfound  = 0.;
  Double_t Nmissed = 0.;
  gROOT->cd();
  hname = Form("hDutTIS_Found_%d", iDut);
  cout << " Look for histo " << hname << endl;
  h2 = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    //gPad->SetLogy();
    h2f    = (TH2*) h2->Clone();
    Nfound = h2f->GetEntries();
  }
  else {
    cout << hname << " not found" << endl;
  }

  hname = Form("hDutTIS_Missed_%d", iDut);
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2m     = (TH2*) h2->Clone();
    Nmissed = h2m->GetEntries();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);

  h2m->Draw("colz");
  //h1f->Draw("same");

  can->cd(3);

  h2all = (TH2*) h2f->Clone("hDutTIS_all");
  h2all->Add(h2m, h2f, 1., 1.);
  h2all->Draw("colz");

  /*
  TEfficiency* pEffDut = new TEfficiency(*h2f, *h2all);
  pEffDut->SetTitle("Efficiency of DUT");
  pEffDut->SetName("hDutTIS_eff");
  pEffDut->Draw("colz");
  gPad->Update();
*/

  can->cd(4);
  h1all = h2all->ProjectionX();
  h1f   = h2f->ProjectionX();
  h1m   = h2m->ProjectionX();

  TEfficiency* pEffDut = new TEfficiency(*h1f, *h1all);
  pEffDut->SetTitle("Efficiency of DUT");
  pEffDut->SetName("hDutTIS_eff1D");
  pEffDut->Draw("AP");
  gPad->Update();

  auto graph = pEffDut->GetPaintedGraph();
  graph->SetMinimum(dEffMin);
  graph->SetMaximum(dEffMax);
  graph->GetXaxis()->SetRangeUser(0., 10.);
  /*
 auto heff = pEffDut->GetPaintedHistogram();
 heff->SetMinimum(dEffMin);
 heff->SetMaximum(dEffMax); 
 */
  gPad->Update();
  gPad->SetGridx();
  gPad->SetGridy();

  return;

  can->cd(4);
  hname = Form("hDutDTLH_TIS_%d", iDut);
  h2f   = (TH2*) gROOT->FindObjectAny(hname);
  if (h2f != NULL) {
    h2f->Draw("colz");
    h2f->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(5);
  hname = Form("hDutDTLH_Missed_TIS_%d", iDut);
  h2m   = (TH2*) gROOT->FindObjectAny(hname);
  if (h2m != NULL) {
    h2m->Draw("colz");
    h2m->ProfileX()->Draw("same");
    gPad->SetLogz();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(6);
  TH2* TIS2eff = (TH2*) h2f->Clone("TIS2Deff");
  TIS2eff->Add(h2f, h2m, 1., 1.);
  TIS2eff->Divide(h2f, TIS2eff, 1., 1., "B");
  TIS2eff->Draw("colz");

  Double_t dEff = Nfound / (Nfound + Nmissed);
  cout << "Average efficiency of Dut: " << dEff << endl;

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }

  can->SaveAs(Form("pl_Eff_TIS_%d.pdf", iDut));
}
