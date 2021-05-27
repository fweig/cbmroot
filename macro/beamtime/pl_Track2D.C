/** @file pl_Track2D.C
  * @copyright Copyright (C) 2020 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [orginator] **/

void pl_Track2D(Int_t iOpt = 1, Int_t iCounterId = 22, Int_t iStrip = -1, Double_t TotMax = 10.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  //can->Divide(4, 4, 0.01, 0.01);
  //  can->Divide(2,2,0,0);
  if (iStrip > -1) can->Divide(1, 2, 0.01, 0.01);

  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH2* h;
  TH2* h2;

  TString cOpt;

  switch (iOpt) {
    case 0: cOpt = "Size"; break;
    case 1: cOpt = "Pos"; break;
    case 2: cOpt = "TOff"; break;
    case 3: cOpt = "Tot"; break;
    case 4:
    case 14: cOpt = "Walk"; break;
    case 5: cOpt = "Walk"; break;
    case 6: cOpt = "Mul"; break;
    case 7: cOpt = "Trms"; break;
    case 8: cOpt = "DelPos"; break;
    case 9: cOpt = "DelTOff"; break;
    case 10: cOpt = "DelMatPos"; break;
    case 11: cOpt = "DelMatTOff"; break;
    default:;
  }

  Int_t iDet       = 0;
  Double_t dAvMean = 0.;
  Double_t dAvRMS  = 0.;
  Int_t iCanv      = 0;

  Int_t iRp   = iCounterId % 10;
  Int_t iSm   = ((iCounterId - iRp) / 10) % 10;
  Int_t iType = (iCounterId - iSm * 10 - iRp) / 100;


  gROOT->cd();
  TString hname = "";
  Int_t iCol    = 1;
  switch (iOpt) {
    case 4: {
      Int_t iDraw = 0;
      for (Int_t iSide = 0; iSide < 2; iSide++) {
        iCol = 1;
        for (Int_t iCh = 0; iCh < 32; iCh++) {
          if (iStrip > -1)
            if (iCh != iStrip) continue;
          cout << "CS " << iCh << iSide << endl;
          hname = Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iType, iSm, iRp, iCh, iSide, cOpt.Data());
          h     = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) {
            TProfile* hProf = h->ProfileX(Form("%s_pfx%d%d", hname.Data(), iCh, iSide));
            hProf->SetLineColor(iCol);
            hProf->SetLineStyle(1);
            hProf->SetMarkerColor(iCol);
            hProf->SetMarkerStyle(24 + iSide);
            iCol++;
            if (iCh == 0) iCol = 1;
            if (iDraw == 0) {
              hProf->SetMaximum(0.4);
              hProf->SetMinimum(-0.4);
              hProf->GetXaxis()->SetRangeUser(0., TotMax);
              hProf->Draw("LP");
            }
            else {
              hProf->Draw("LPsame");
            }
            iDraw++;
          }
        }
      }
    } break;

    case 14: {
      Int_t iCanv = 1;
      for (Int_t iSide = 0; iSide < 2; iSide++)
        for (Int_t iCh = 0; iCh < 32; iCh++) {
          if (iStrip > -1)
            if (iCh != iStrip) continue;
          hname = Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iType, iSm, iRp, iCh, iSide, cOpt.Data());
          h     = (TH2*) gROOT->FindObjectAny(hname);
          if (h != NULL) {
            can->cd(iCanv++);
            h->Draw("colz");
            gPad->SetLogz();
            h->ProfileX()->Draw("same");
            TString hName2 = Form("Cor_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_Walk_px", iType, iSm, iRp, iCh, iSide);
            TH1* hCor      = (TH1*) gROOT->FindObjectAny(hName2);
            if (NULL != hCor) {
              hCor->SetLineColor(6);
              hCor->Draw("same");
            }
          }
        }
    } break;

    default:
      hname = Form("cal_SmT%01d_sm%03d_rpc%03d_%s", iType, iSm, iRp, cOpt.Data());
      h     = (TH2*) gROOT->FindObjectAny(hname);
      if (h != NULL) {
        if (iOpt == 2 || iOpt == 2) { gPad->SetLogz(); }
        h->Draw("colz");
        h->ProfileX()->Draw("same");
      }
  }

  can->SaveAs(Form("pl_Track_%s_%d.pdf", cOpt.Data(), iCounterId));
}
