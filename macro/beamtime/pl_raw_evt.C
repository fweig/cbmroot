/** @file pl_raw_evt.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

void pl_raw_evt()
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(2, 3, 0.01, 0.01);
  //  can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.09;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetStatY(0.5);
  //gStyle->SetStatX(0.5);
  gStyle->SetStatW(0.5);
  gStyle->SetStatH(0.3);

  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(11);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  TString hname;

  can->cd(1);
  hname = Form("hEvDetMul");
  h     = (TH1*) gROOT->FindObjectAny(hname);
  h->Draw();
  gPad->SetLogy();

  can->cd(2);
  hname = Form("hPulMul");
  h     = (TH1*) gROOT->FindObjectAny(hname);
  h->Draw();
  gPad->SetLogy();

  can->cd(3);
  hname = Form("hPulserTimesRaw");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  h2->Draw("colz");
  gPad->SetLogz();

  can->cd(4);
  hname = Form("hPulserTimesCor");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  h2->Draw("colz");
  gPad->SetLogz();

  can->cd(5);
  hname = Form("hDigiTimesRaw");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  h2->Draw("colz");
  gPad->SetLogz();

  can->cd(6);
  hname = Form("hDigiTimesCor");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  h2->Draw("colz");
  gPad->SetLogz();

  can->SaveAs(Form("pl_raw_evt.pdf"));
}
