/** @file pl_EvCluMul.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

void pl_EvCluMul(Double_t dTmin = 0., Double_t dTmax = 1800.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(2, 2, 0.01, 0.01);
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

  gStyle->SetOptStat(kFALSE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  TString hname;

  can->cd(1);
  hname = Form("hEvCluMul");
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  h2->Draw("colz");
  gPad->SetLogz();
  h2->GetXaxis()->SetRangeUser(dTmin, dTmax);

  can->cd(2);
  TH1* hy = h2->ProjectionY();
  hy->Draw();
  gPad->SetLogy();

  can->cd(3);
  TProfile* hpx = h2->ProfileX();
  hpx->Draw();
  //    gPad->SetLogz();
  hpx->GetXaxis()->SetRangeUser(dTmin, dTmax);

  can->cd(4);


  can->SaveAs(Form("pl_EvCluMul.pdf"));
}
