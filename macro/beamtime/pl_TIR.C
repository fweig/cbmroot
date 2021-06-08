/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void pl_TIR(Double_t Tstart = 0., Double_t Tend = 1000., Double_t dFracMin = 0.0, Double_t dFracMax = 1.05,
            TString sysinfo = "")
{
  gROOT->LoadMacro(((TString) gSystem->Getenv("VMCWORKDIR") + "/macro/beamtime/pl_Datime.C").Data());
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 600, 600);
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
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* h2pfx = NULL;

  can->cd(1);
  gROOT->cd();
  hname = "TIR_all";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->GetXaxis()->SetRangeUser(Tstart, Tend);
    h1->Draw("");
    h1->SetLineColor(3);
    h1->GetXaxis()->SetTitle("time [s]");
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIR_all = (TH1*) h1->Clone();

  if (NULL != hTIR_all) {
    hTIR_all->SetMinimum(hTIR_all->GetMaximum() / 1.E3);
    hTIR_all->Draw();
  }
  else
    return;

  hname = "TIR_sel";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->GetXaxis()->SetTitle("time [s]");
    gPad->SetLogy();
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIR_sel = (TH1*) h1->Clone();

  hname = "TIR_sel1";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->SetLineColor(2);
    //   hTIR_all->SetMinimum( h1->GetMinimum() );
    //   gPad->Update();
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIR_sel1 = (TH1*) h1->Clone();

  hname = "TIR_sel2";
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1->Draw("same");
    h1->SetLineColor(7);
  }
  else {
    cout << hname << " not found" << endl;
  }
  TH1* hTIR_sel2 = (TH1*) h1->Clone();

  can->cd(2);
  gROOT->cd();
  /*
 TH1F *hTIRselfrac = (TH1F *)hTIR_all->Clone();
 hTIRselfrac->SetName("hTIRselfrac");
 hTIRselfrac->SetTitle("MRef - selector probability");
 hTIRselfrac->Divide(hTIR_sel, hTIR_all, 1., 1., "B");
 hTIRselfrac->SetMaximum(dFracMax);
 hTIRselfrac->SetMinimum(0.0001);
 hTIRselfrac->Draw();
 hTIRselfrac->SetLineColor(hTIR_sel->GetLineColor()); 
 */

  TEfficiency* pEffSel = new TEfficiency(*hTIR_sel2, *hTIR_all);
  pEffSel->SetTitle("Selector (MRef & Sel2)  efficiency; time (s); efficiency");
  pEffSel->Draw("AP");
  gPad->Update();
  auto graph = pEffSel->GetPaintedGraph();
  graph->GetXaxis()->SetRangeUser(Tstart, Tend);
  gPad->Update();
  /*
 TH1F *hTIRsel1frac = (TH1F *)hTIR_all->Clone();
 hTIRsel1frac->SetName("hTIRsel1frac");
 hTIRsel1frac->SetTitle("Dut & MRef coinicidence probability");
 hTIRsel1frac->Divide(hTIR_sel1, hTIR_all, 1., 1., "B");
 hTIRsel1frac->Draw("same");
 hTIRsel1frac->SetLineColor(hTIR_sel1->GetLineColor()); 
 */
  // gPad->SetLogy();

  can->cd(3);
  /*
 TH1F *hselsel1frac = (TH1F *)hTIR_sel->Clone();
 hselsel1frac->SetName("hselsel1frac");
 hselsel1frac->SetTitle("Relative efficiency of DUT");
 // hselsel1frac->Divide(hTIR_sel1, hTIR_sel, 1., 1., "B");
 hselsel1frac->Divide(hTIR_sel1, hTIR_sel, 1., 1., "");
 hselsel1frac->Draw("E1");
 hselsel1frac->SetLineColor(hTIR_sel1->GetLineColor()); 
*/
  TEfficiency* pEffDut = new TEfficiency(*hTIR_sel1, *hTIR_sel2);
  pEffDut->SetTitle("Relative efficiency of DUT; time (s); efficiency");
  pEffDut->Draw("AP");
  // gPad->SetLogy();
  gPad->Update();
  auto gEffDut = pEffDut->GetPaintedGraph();
  gEffDut->GetXaxis()->SetRangeUser(Tstart, Tend);
  gEffDut->SetMinimum(dFracMin);
  gEffDut->SetMaximum(dFracMax);

  gPad->Update();

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }

  can->SaveAs(Form("pl_TIR.pdf"));
}
