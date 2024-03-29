/* Copyright (C) 2017-2018 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */
void pl_TIS(Double_t dEffMin = 0.1, Double_t dEffMax = 1.01, TString sysinfo = "")
{
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
  TH2* h2;
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);
  gROOT->cd();
  hname = "TIS_all";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("");
    h1->SetLineColor(3);
    h1->GetXaxis()->SetTitle("time [s]");
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIS_all = (TH1*) h1->Clone();

  hname = "tof_trb_time_in_spill";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) { h1->Draw("same"); }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  if (NULL != hTIS_all) hTIS_all->Draw();
  else
    return;

  hname = "TIS_sel";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->GetXaxis()->SetTitle("time [s]");
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIS_sel = (TH1*) h1->Clone();

  hname = "TIS_sel1";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->SetLineColor(2);
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIS_sel1 = (TH1*) h1->Clone();

  hname = "TIS_sel2";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->SetLineColor(7);
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIS_sel2 = (TH1*) h1->Clone();

  can->cd(3);
  gROOT->cd();
  /*
 TH1F *hTISselfrac = (TH1F *)hTIS_all->Clone();
 hTISselfrac->SetName("hTISselfrac");
 hTISselfrac->SetTitle("MRef - selector probability");
 hTISselfrac->Divide(hTIS_sel, hTIS_all, 1., 1., "B");
 hTISselfrac->SetMaximum(dFracMax);
 hTISselfrac->SetMinimum(0.0001);
 hTISselfrac->Draw();
 hTISselfrac->SetLineColor(hTIS_sel->GetLineColor()); 
 */

  TEfficiency* pEffSel = new TEfficiency(*hTIS_sel2, *hTIS_all);
  pEffSel->SetTitle("Selector (MRef & Sel2)  efficiency");
  pEffSel->Draw("AP");
  gPad->Update();
  auto graph = pEffSel->GetPaintedGraph();
  graph->SetMinimum(dEffMin);
  graph->SetMaximum(dEffMax);
  graph->GetXaxis()->SetRangeUser(0., 10.);
  gPad->Update();
  /*
 TH1F *hTISsel1frac = (TH1F *)hTIS_all->Clone();
 hTISsel1frac->SetName("hTISsel1frac");
 hTISsel1frac->SetTitle("Dut & MRef coinicidence probability");
 hTISsel1frac->Divide(hTIS_sel1, hTIS_all, 1., 1., "B");
 hTISsel1frac->Draw("same");
 hTISsel1frac->SetLineColor(hTIS_sel1->GetLineColor()); 
 */
  // gPad->SetLogy();

  can->cd(4);
  /*
 TH1F *hselsel1frac = (TH1F *)hTIS_sel->Clone();
 hselsel1frac->SetName("hselsel1frac");
 hselsel1frac->SetTitle("Relative efficiency of DUT");
 // hselsel1frac->Divide(hTIS_sel1, hTIS_sel, 1., 1., "B");
 hselsel1frac->Divide(hTIS_sel1, hTIS_sel, 1., 1., "");
 hselsel1frac->Draw("E1");
 hselsel1frac->SetLineColor(hTIS_sel1->GetLineColor()); 
*/
  TEfficiency* pEffDut = new TEfficiency(*hTIS_sel1, *hTIS_sel2);
  pEffDut->SetTitle("Relative efficiency of DUT");
  pEffDut->Draw("AP");
  // gPad->SetLogy();
  gPad->Update();

  auto gEffDut = pEffDut->GetPaintedGraph();
  gEffDut->SetMinimum(dEffMin);
  gEffDut->SetMaximum(dEffMax);
  gEffDut->GetXaxis()->SetRangeUser(0., 10.);
  gPad->Update();

  can->cd(5);
  gROOT->cd();
  hname = "hTISDT04D4best";
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    h2->GetYaxis()->SetTitleOffset(1.3);
    h2pfx = (TProfile*) h2->ProfileX("hTISDT04D4best_pfs", 1, -1, "s");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(6);
  h2pfx->Draw();
  TH1D* h2pfxes = (TH1D*) h2pfx->ProjectionX("hTISDT04D4best_RMS", "C=E");
  h2pfxes->Draw("");
  h2pfxes->GetYaxis()->SetTitle("RMS(#Deltat) [ns]");
  h2pfxes->GetYaxis()->SetTitleOffset(1.3);

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }

  can->SaveAs(Form("pl_TIS.pdf"));
}
