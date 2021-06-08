/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

void pl_over_deltof(Int_t iSel = 0, Int_t iNDet = 1)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 900, 900);
  can->Divide(3, 6);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH2* h2;
  Int_t iType[6]   = {0, 1, 6, 5, 9, 8};
  Int_t iNumSm[6]  = {6, 3, 1, 1, 3, 2};
  Int_t iNumRpc[6] = {3, 3, 2, 1, 2, 1};
  Int_t iCanv      = 0;
  // if (h!=NULL) h->Delete();


  for (Int_t iSm = 0; iSm < iNumSm[0]; iSm++) {
    for (Int_t iRpc = 0; iRpc < iNumRpc[0]; iRpc++) {
      can->cd(iCanv + 1);
      iCanv++;
      gROOT->cd();
      TString hname2 = Form("cl_SmT%d_sm%03d_rpc%03d_Sel%02d_DelTof", iType[0], iSm, iRpc, iSel);

      h2 = (TH2*) gROOT->FindObjectAny(hname2);
      //h2=(TH2 *)gROOT->FindObjectAny(hname);
      if (h2 != NULL) {
        h2->Draw("colz");
        gPad->SetLogz();
        h2->ProfileX()->Draw("same");
      }
      else {
        cout << "Histogram " << hname2 << " not existing. " << endl;
      }
    }
  }

  can->SaveAs(Form("pl_over_deltof%d.pdf", iSel));
}
