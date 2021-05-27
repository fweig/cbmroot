/** @file pl_all_DigiDTFD.C
  * @copyright Copyright (C) 2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [orginator] **/

void pl_all_DigiDTFD(Int_t iOpt = 0, Double_t dYmax = 0., Int_t iNDet = 2)
{
  Int_t iOpt1 = iOpt % 10;
  Int_t iOpt2 = (iOpt - iOpt1) / 10 % 10;
  Int_t iOpt3 = (iOpt - iOpt2 * 10 - iOpt1) / 100 % 10;
  Int_t iOpt4 = (iOpt - iOpt3 * 100 - iOpt2 * 10 - iOpt1) / 1000 % 10;

  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 900, 900);
  //TCanvas *can = new TCanvas("can","can",48,56,900,700);
  //can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,3,0.01,0.01);
  //can->Divide(5, 7, 0.01, 0.01);
  switch (iOpt3) {
    case 0: can->Divide(5, 7, 0.01, 0.01); break;
    case 1: can->Divide(5, 4, 0.01, 0.01); break;
    case 3: can->Divide(1, 4, 0.01, 0.01); break;
    case 4: can->Divide(1, 1, 0.01, 0.01); break;
  }

  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* h;
  TH2* h2;
  TH1* hTime;
  TString hnameT;

  const Int_t iType[6]   = {0, 5, 9, 7, 6, 8};
  const Int_t iSmNum[6]  = {5, 1, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {5, 1, 2, 1, 2, 8};

  Double_t dTime = 0.;
  Int_t iCanv    = 0;

  Int_t jSmType = 5;
  Int_t jSm     = 0;
  Int_t jRp     = 0;

  // if (h!=NULL) h->Delete();
  Int_t iCol = 1;

  for (Int_t iCh = 0; iCh < iNDet; iCh++) {
    for (Int_t iSm = 0; iSm < iSmNum[iCh]; iSm++) {
      if (iOpt3 == 1) {
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        iCol = 1;
      }
      else {
        if (iOpt3 == 4) {
          iCol = 1;
          if (iCh != iOpt4) continue;
        }
      }
      for (Int_t iRpc = 0; iRpc < iRpcNum[iCh]; iRpc++) {
        if (iOpt3 == 0) {
          can->cd(iCanv + 1);
          iCanv++;
          gROOT->cd();
          iCol = 4;
        }

        TString hname = "";
        switch (iOpt1) {
          case 0: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTFD", iType[iCh], iSm, iRpc); break;

          case 1: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTMul", iType[iCh], iSm, iRpc); break;

          default:;
        }
        iCol++;
        if (iCol == 5) iCol++;
        h2 = (TH2*) gROOT->FindObjectAny(hname);
        TH1D* hx;
        TH1D* hy;
        TH1* hp;
        if (h2 != NULL) {
          switch (iOpt2) {
            case 0:
              h2->Draw("colz");
              gPad->SetLogz();
              break;
            case 1:
              hp = (TH1*) h2->ProjectionY();
              hp->SetLineColor(iCol);
              switch (iOpt3) {
                case 0: hp->Draw(); break;
                case 1:
                case 4:
                  if (iRpc == 0) {
                    if (dYmax > 0.) hp->SetMaximum(dYmax);
                    hp->Draw();
                  }
                  else
                    hp->Draw("same");
              }
              cout << "plot " << hp->GetName() << " into canv " << iCanv << " with col " << iCol << endl;
              //gPad->SetLogy();
              break;

            case 2:
              h2->SetMarkerSize(5);
              h2->ProfileX()->Draw();
              //gPad->SetLogz();
              break;

            case 3:
              for (Int_t iCh = 0; iCh < h2->GetNbinsX(); iCh++) {
                if (iCh == 0) h2->ProjectionY(Form("%s_py%d", h2->GetName(), iCh), iCh + 1, iCh + 1)->Draw();
                else {
                  h2->ProjectionY(Form("%s_py%d", h2->GetName(), iCh), iCh + 1, iCh + 1)->Draw("same");
                }
              }
              break;
          }
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_DigiDTFD.pdf"));
}
