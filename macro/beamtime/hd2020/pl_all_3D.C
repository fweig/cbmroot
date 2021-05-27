/** @file pl_all_3D.C
  * @copyright Copyright (C) 2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [orginator] **/

void pl_all_3D(Int_t iOpt = 0, Int_t iSel = 0, Int_t iNSt = 2)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(4, 3, 0.01, 0.01);
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH3* h;
  TH2* h2;
  const Int_t iType[6]   = {9, 6, 7, 5, 6, 8};
  const Int_t iSmNum[6]  = {3, 2, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {2, 2, 1, 1, 2, 8};
  TString cOpt;

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    for (Int_t iSm = 0; iSm < iSmNum[iSt]; iSm++) {
      for (Int_t iRp = 0; iRp < iRpcNum[iSt]; iRp++) {
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Walk2", iType[iSt], iSm, iRp, iSel);
        h             = (TH3*) gROOT->FindObjectAny(hname);
        if (h != NULL) {
          switch (iOpt) {
            case 0:
              cOpt = "yx";
              h->Project3D(cOpt)->Draw("colz");
              break;

            case 1:
              cOpt = "yx";
              h->Project3D(cOpt)->Draw("colz");
              gPad->SetLogz();
              break;

            case 2:
              cOpt = "pfyx";
              h2   = (TH2*) h->Project3DProfile("yx");
              h2->SetMinimum(-0.2);
              h2->SetMaximum(0.2);
              h2->Draw("colz");
              break;
            default:;
          }
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_%s.pdf", cOpt.Data()));
}
