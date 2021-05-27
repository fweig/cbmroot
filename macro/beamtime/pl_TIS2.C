/** @file pl_TIS2.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

void pl_TIS2(Double_t dTmin = 0., Double_t dTmax = 1., TString sysinfo = "")
{
  gROOT->LoadMacro("pl_Datime.C");
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 750, 600);
  can->Divide(2, 3);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h;
  TH1* h1;
  TH1 *hhpx, *htpx;
  TH2* h2;
  // if (hPla!=NULL) hPla->Delete();
  TString hname   = "";
  TProfile* hhpfx = NULL;
  TProfile* htpfx = NULL;

  can->cd(1);
  gROOT->cd();
  hname = "TIS_Nhit";
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    hhpx  = (TH1*) h2->ProjectionX();
    hhpfx = (TProfile*) h2->ProfileX();
    //h1->SetLineColor(3);
    //h1->GetXaxis()->SetTitle("time [s]");
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(2);
  gROOT->cd();
  hname = "TIS_Ntrk";
  h2    = (TH2*) gROOT->FindObjectAny(hname);
  if (h2 != NULL) {
    h2->Draw("colz");
    gPad->SetLogz();
    htpx  = (TH1*) h2->ProjectionX("htpx", 2, -1);
    htpfx = (TProfile*) h2->ProfileX();
  }
  else {
    cout << hname << " not found" << endl;
  }

  can->cd(3);
  gROOT->cd();
  hhpx->Draw();
  hhpx->GetXaxis()->SetRangeUser(dTmin, dTmax);

  can->cd(4);
  gROOT->cd();
  htpx->Draw();
  htpx->GetXaxis()->SetRangeUser(dTmin, dTmax);

  can->cd(5);
  gROOT->cd();
  hhpfx->Draw();
  hhpfx->GetXaxis()->SetRangeUser(dTmin, dTmax);

  can->cd(6);
  gROOT->cd();
  htpfx->Draw();
  htpfx->GetXaxis()->SetRangeUser(dTmin, dTmax);

  TString FADD = Form("pl_Datime(\"%s\")", sysinfo.Data());
  // gInterpreter->ProcessLine(FADD.Data());

  can->SaveAs(Form("pl_TIS2.pdf"));
}
