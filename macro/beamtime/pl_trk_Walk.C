/** @file pl_trk_Walk.C
  * @copyright Copyright (C) 2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [committer] **/

void pl_trk_Walk(Int_t iId = 900, Int_t iOpt = 1, Double_t dMax = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 1400, 900);
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH2* h;
  TH2* h2;
  Int_t iRp = iId % 10;
  Int_t iSm = ((iId - iRp) / 10) % 10;
  Int_t iTy = (iId - iSm * 10 - iRp) / 100;

  TString cOpt = "Walk";

  Int_t iDet  = 0;
  Int_t iCanv = 0;

  gROOT->cd();
  TString hname = "";
  Int_t iCol    = 1;
  switch (iOpt) {
    case 0:; break;
    case 1: {
      can->Divide(4, 8, 0.01, 0.01);
      for (Int_t iCh = 0; iCh < 32; iCh++) {
        can->cd(iCanv + 1);
        iCanv++;
        for (Int_t iSide = 0; iSide < 2; iSide++) {
          hname = Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iTy, iSm, iRp, iCh, iSide, cOpt.Data());
          h     = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) {
            TProfile* hProf = h->ProfileX(Form("%s_pfx%d%d", hname.Data(), iCh, iSide));
            iCol            = iSide + 2;
            hProf->SetLineColor(iCol);
            hProf->SetMarkerColor(iCol);
            hProf->SetMarkerStyle(20 + iSide);
            if (iSide == 0) {
              if (dMax > 0.) {
                hProf->SetMaximum(dMax);
                hProf->SetMinimum(-dMax);
              }
              hProf->Draw();
            }
            else {
              hProf->Draw("same");
            }
          }
        }
      }
      break;

      case 20: {
        can->Divide(4, 8, 0.01, 0.01);
        for (Int_t iCh = 0; iCh < 32; iCh++) {
          can->cd(iCanv + 1);
          iCanv++;
          Int_t iSide = 0;
          hname       = Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iTy, iSm, iRp, iCh, iSide, cOpt.Data());
          h           = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) {
            if (dMax > 0.) {
              //			  h->GetYAxis()->SetMaximum(dMax);
              //			  h->GetYAxis()->SetMinimum(-dMax);
            }
            h->Draw("colz");
          }
        }
      } break;

      case 21: {
        can->Divide(4, 8, 0.01, 0.01);
        for (Int_t iCh = 0; iCh < 32; iCh++) {
          can->cd(iCanv + 1);
          iCanv++;
          Int_t iSide = 1;
          hname       = Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iTy, iSm, iRp, iCh, iSide, cOpt.Data());
          h           = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) { h->Draw("colz"); }
        }
      } break;

      case 200: {
        can->Divide(4, 8, 0.01, 0.01);
        for (Int_t iCh = 0; iCh < 32; iCh++) {
          can->cd(iCanv + 1);
          iCanv++;
          Int_t iSide = 0;
          hname       = Form("cl_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iTy, iSm, iRp, iCh, iSide, cOpt.Data());
          h           = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) { h->Draw("colz"); }
        }
      } break;

      case 201: {
        can->Divide(4, 8, 0.01, 0.01);
        for (Int_t iCh = 0; iCh < 32; iCh++) {
          can->cd(iCanv + 1);
          iCanv++;
          Int_t iSide = 1;
          hname       = Form("cl_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iTy, iSm, iRp, iCh, iSide, cOpt.Data());
          h           = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) { h->Draw("colz"); }
        }
      } break;

      default:;
    }
  }
  can->SaveAs(Form("pl_trk_Walk_%d.pdf", iId));
}
