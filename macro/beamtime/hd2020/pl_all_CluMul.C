/* Copyright (C) 2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_all_CluMul(Int_t iNSt = 2, Double_t MulMax = 100)
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

  TH1* h;
  TH2* h2;
  const Int_t iType[6]   = {9, 6, 7, 5, 6, 8};
  const Int_t iSmNum[6]  = {3, 2, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {2, 2, 1, 1, 2, 8};

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    //   cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    for (Int_t iSm = 0; iSm < iSmNum[iSt]; iSm++) {
      // cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
      for (Int_t iRp = 0; iRp < iRpcNum[iSt]; iRp++) {
        //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = Form("cl_SmT%01d_sm%03d_rpc%03d_Mul", iType[iSt], iSm, iRp);
        h             = (TH1*) gROOT->FindObjectAny(hname);
        if (h != NULL) {
          h->GetXaxis()->SetRange(0., MulMax);
          h->Draw("");
          gPad->SetLogy();
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
        if (iRp == 10) break;
      }
    }
  }
  can->SaveAs(Form("pl_all_CluMul.pdf"));
}
