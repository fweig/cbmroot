/* Copyright (C) 2021-2022 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_all_Sel2D(Int_t iOpt = 0, Int_t iSel = 0, Int_t iOpt2 = 0, Int_t iNSt = 7)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(5, 8, 0.01, 0.01);
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.07;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);

  TH1* hp;
  TH2* h;
  TH2* h2;
  const Int_t iType[7]   = {0, 2, 9, 7, 6, 5, 8};
  const Int_t iSmNum[7]  = {5, 1, 1, 1, 1, 1, 2};
  const Int_t iRpcNum[7] = {5, 5, 2, 2, 2, 1, 1};

  TString cOpt;

  switch (iOpt) {
    case 0: cOpt = "Size"; break;
    case 1: cOpt = "Pos"; break;
    case 2: cOpt = "TOff"; break;
    case 3: cOpt = "Tot"; break;
    case 4: cOpt = "AvWalk"; break;
    case 5: cOpt = "DelTof"; break;
    case 6: cOpt = "dXdY"; break;
    case 7: cOpt = "TofOff"; break;
    case 7: cOpt = "QASY"; break;
    default:;
  }

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();
  TString FitHName[100];
  Double_t FitIntegral[100];
  Double_t FitMean[100];
  Double_t FitWidth[100];
  Int_t NFit = 0;

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    // cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    for (Int_t iSm = 0; iSm < iSmNum[iSt]; iSm++) {
      //cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
      for (Int_t iRp = 0; iRp < iRpcNum[iSt]; iRp++) {
        //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_%s", iType[iSt], iSm, iRp, iSel, cOpt.Data());
        h             = (TH2*) gROOT->FindObjectAny(hname);
        if (h != NULL) {
          h->Draw("colz");
          //gPad->SetLogz();

          if (iOpt2 > 0) switch (iOpt) {
              case 6: {
                switch (iOpt2) {
                  case 1:  // x-projection
                    hp = h->ProjectionX();
                    hp->Draw();
                    break;
                  case 2:  // y-projection
                    hp = h->ProjectionY();
                    hp->Draw();
                    break;
                }
                Double_t dFMean = hp->GetMean();
                Double_t dFLim  = 2.5 * hp->GetRMS();
                if (hp->Integral() > 10) {
                  TFitResultPtr fRes = hp->Fit("gaus", "S", "HEsame", dFMean - dFLim, dFMean + dFLim);
                  FitHName[NFit]     = hp->GetName();
                  FitIntegral[NFit]  = hp->Integral();
                  FitMean[NFit]      = fRes->Parameter(1);
                  FitWidth[NFit]     = fRes->Parameter(2);
                  NFit++;
                  ;
                }
              } break;  // case 6 end

              default:;
                switch (iOpt2) {
                  case 1:  // x-projection
                    hp = h->ProjectionX();
                    hp->Draw();
                    break;
                  case 2:  // y-projection
                    hp = h->ProjectionY();
                    hp->Draw();
                    break;
                }
            }
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
        if (iRp == 10) break;
      }
    }
  }
  if (iOpt2 > 0) {
    for (Int_t iFit = 0; iFit < NFit; iFit++) {
      cout << "FitRes " << FitHName[iFit] << ", Stat: " << FitIntegral[iFit] << ", Mean " << FitMean[iFit] << ", Width "
           << FitWidth[iFit] << endl;
    }
  }

  can->SaveAs(Form("pl_all_Sel%d_%s.pdf", iSel, cOpt.Data()));
}
