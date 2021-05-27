/** @file pl_Eff_DTLH.C
  * @copyright Copyright (C) 2018 Physikalisches Institut, Universitaet Heidelberg, Heidelberg
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Christian Simon [orginator] **/

void pl_Eff_DTLH(Int_t iDut = 910, Double_t dEffMin = 0.5, Double_t dEffMax = 1., TString sysinfo = "")
{
  gROOT->LoadMacro(((TString) gSystem->Getenv("VMCWORKDIR") + "/macro/beamtime/pl_Datime.C").Data());
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 600, 600);
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
  hname = Form("hDutDTLH_Found_%d", iDut);
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
    return;
  }

  hname = Form("hDutDTLH_Missed_%d", iDut);
  h1    = (TH1*) gROOT->FindObjectAny(hname);
  if (h1 != NULL) {
    h1m     = (TH1*) h1->Clone();
    Nmissed = h1m->GetEntries();
    h1m->Draw("same");
    h1m->SetLineColor(2);
  }
  else {
    cout << hname << " not found" << endl;
    return;
  }

  can->cd(2);

  h1all = (TH1*) h1f->Clone("hDutDTLH_all");
  h1all->Add(h1m, h1f, 1., 1.);

  h1all->Draw();
  h1all->SetLineColor(1);
  h1m->Draw("same");
  h1f->Draw("same");

  can->cd(3);

  TEfficiency* pEffDut = new TEfficiency(*h1f, *h1all);
  pEffDut->SetTitle("Efficiency of DUT");
  pEffDut->SetName("hDutDTLH_eff");
  pEffDut->Draw("AP");
  gPad->Update();

  auto graph = pEffDut->GetPaintedGraph();
  graph->SetMinimum(dEffMin);
  graph->SetMaximum(dEffMax);
  graph->GetXaxis()->SetRangeUser(0., 12.);
  graph->Draw();
  /*
 auto heff = pEffDut->GetPaintedHistogram();
 heff->SetMinimum(dEffMin);
 heff->SetMaximum(dEffMax); 
 */
  gPad->Update();

  graph->GetXaxis()->SetRangeUser(0., 12.);
  gPad->SetGridx();
  gPad->SetGridy();

  /*
  TF1 *f1 = new TF1("Restore","1.-0.5*exp(-exp(x)*0.0001)",1.,10.);
  f1->Draw("same");
 */

  Double_t dEff = Nfound / (Nfound + Nmissed);
  cout << "Average efficiency of Dut: " << dEff << endl;
  Double_t dDt = TMath::Power(10., h1f->GetMean());

  cout << "Average time distance of Dut-Hits: " << dDt << " ns => <rate> = " << 10.E9 / dDt << " Hz / cell " << endl;

  can->cd(4);
  TPaveText* pt = new TPaveText(0.1, 0.1, 0.9, 0.9);
  pt->AddText(Form("average efficiency of %d: %5.3f", iDut, dEff));
  pt->AddText(Form("average rate: %9.3f kHz/cell ", 10.E6 / dDt));
  pt->SetFillColor(0);
  pt->SetTextFont(52);
  pt->SetBorderSize(0);
  pt->Draw();

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  if (gROOT->IsBatch()) { gInterpreter->ProcessLine(FADD.Data()); }


  can->SaveAs(Form("pl_Eff_DTLH_%d.pdf", iDut));
}
