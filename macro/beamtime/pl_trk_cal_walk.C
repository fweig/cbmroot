/** @file pl_trk_cal_walk.C
  * @copyright Copyright (C) 2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

void pl_trk_cal_walk(Int_t iType = 0, Int_t iSm = 0, Int_t iRpc = 0, Int_t iSide = 0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(4, 8);
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  Int_t iCan = 1;
  for (Int_t iCh = 0; iCh < 32; iCh++) {
    if (iCan == 36) iCan = 1;
    can->cd(iCan++);
    gROOT->cd();
    TString hname = Form("cal_SmT%d_sm%03d_rpc%03d_Ch%03d_S%d_Walk", iType, iSm, iRpc, iCh, iSide);
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      //      gPad->SetGridx();
      //      gPad->SetGridy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->SaveAs(Form("pl_trk_walk_%d%d%d_%d.pdf", iType, iSm, iRpc, iSide));
}
