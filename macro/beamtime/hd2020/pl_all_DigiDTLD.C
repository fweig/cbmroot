/* Copyright (C) 2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_all_DigiDTLD(Int_t iNDet = 6, Double_t dDTthr = 2., Int_t iOpt = 0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 48, 55, 900, 900);
  //TCanvas *can = new TCanvas("can","can",48,56,900,700);
  //can->Divide(4,4,0.01,0.01);
  //  can->Divide(2,3,0.01,0.01);
  can->Divide(5, 7, 0.01, 0.01);
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

  const Int_t iType[6]   = {0, 9, 7, 5, 6, 8};
  const Int_t iSmNum[6]  = {5, 1, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {5, 2, 1, 1, 2, 8};

  Double_t dTime = 0.;
  Int_t iCanv    = 0;

  Int_t jSmType = 5;
  Int_t jSm     = 0;
  Int_t jRp     = 0;

  // if (h!=NULL) h->Delete();

  for (Int_t iCh = 0; iCh < iNDet; iCh++) {
    for (Int_t iSm = 0; iSm < iSmNum[iCh]; iSm++) {
      for (Int_t iRpc = 0; iRpc < iRpcNum[iCh]; iRpc++) {
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = Form("cl_SmT%01d_sm%03d_rpc%03d_DigiDTLD", iType[iCh], iSm, iRpc);
        switch (iOpt) {
          case 0:; break;

          case 1: hname = Form("%s_fdead", hname.Data()); break;

          case 2: hname = Form("%s_py", hname.Data()); break;

          case 3: hname = Form("%s_py_dead", hname.Data()); break;

          default:;
        }

        h2 = (TH2*) gROOT->FindObjectAny(hname);
        TH1D* hx;
        TH1D* hy;
        TH1D* hy_dead;
        Double_t dDeadtimeSum = 0.;
        if (h2 != NULL) {
          switch (iOpt) {
            case 0:
              h2->Draw("colz");
              gPad->SetLogz();

              // Determine time duration an data taking
              hnameT = Form("cl_SmT%01d_sm%03d_rpc%03d_rate", jSmType, jSm, jRp);
              hTime  = (TH1*) gROOT->FindObjectAny(hnameT);
              for (dTime = 0; dTime < hTime->GetNbinsX(); dTime++)
                if (hTime->GetBinContent(dTime + 1) == 0) break;
              cout << "Normalize for a run duration of " << dTime << " s" << endl;

              // create result histograms
              hx = h2->ProjectionX(Form("%s_fdead", hname.Data()));
              hx->GetYaxis()->SetTitle("deadtime fraction");
              hx->Reset();
              hy      = h2->ProjectionY(Form("%s_py", hname.Data()));
              hy_dead = h2->ProjectionY(Form("%s_py_dead", hname.Data()));
              hy_dead->GetYaxis()->SetTitle("deadtime fraction");
              hy_dead->Reset();
              for (Int_t iT = hy->GetNbinsX(); iT > 0; iT--) {
                dDeadtimeSum += hy->GetBinContent(iT) * hy->GetXaxis()->GetBinLowEdge(iT);
                Double_t dDeadFrac = dDeadtimeSum / h2->GetNbinsX() / dTime;
                hy_dead->SetBinContent(iT, dDeadFrac);
              }

              for (Int_t iCh = 0; iCh < h2->GetNbinsX(); iCh++) {
                TH1D* hCh          = h2->ProjectionY(Form("%s_%d_py", hname.Data(), iCh), iCh + 1, iCh + 1);
                Double_t dAll      = hCh->GetEntries();
                Double_t dTAllMean = hCh->GetMean();
                if (dAll > 0) {
                  Double_t BL    = hCh->GetXaxis()->FindBin(dDTthr);
                  Double_t dLate = hCh->Integral(BL, hCh->GetNbinsX(), "");
                  hCh->GetXaxis()->SetRange(BL, hCh->GetNbinsX());
                  Double_t dTLateMean = hCh->GetMean();
                  //Double_t dLateRatio=dLate*dTLateMean/dAll/dTAllMean;
                  Double_t dLateRatio = dLate * dTLateMean / dTime;
                  cout << Form("Long DT fraction for %s, ch %d: %6.3f, dTAll "
                               "%6.3f, dTLate %6.3f",
                               hname.Data(), iCh, dLateRatio, dTAllMean, dTLateMean)
                       << endl;
                  hx->SetBinContent(iCh + 1, dLateRatio);
                }
              }
              break;

            case 1: h2->Draw(); break;

            case 2:
            case 3:
              h2->Draw();
              gPad->SetLogy();
              break;
          }
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_DigiDTLD.pdf"));
}
