/** @file pl_all_Track2D.C
  * @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

void pl_all_Track2D(Int_t iOpt = 1, Int_t iNSt = 4)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(5, 7, 0.01, 0.01);
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
  const Int_t iType[6]   = {0, 9, 6, 5, 7, 8};
  const Int_t iSmNum[6]  = {5, 2, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {5, 2, 2, 1, 1, 8};
  TString cOpt;

  switch (iOpt) {
    case 0: cOpt = "Position"; break;
    case 1: cOpt = "Pos"; break;
    case 2: cOpt = "TOff"; break;
    case 3: cOpt = "Tot"; break;
    case 4: cOpt = "Walk"; break;
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

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    // cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    for (Int_t iSm = 0; iSm < iSmNum[iSt]; iSm++) {
      //cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
      for (Int_t iRp = 0; iRp < iRpcNum[iSt]; iRp++) {
        //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = "";
        Int_t iCol    = 1;
        switch (iOpt) {
          case 4:
            for (Int_t iSide = 0; iSide < 2; iSide++)
              for (Int_t iCh = 0; iCh < 32; iCh++) {
                hname = Form("cal_SmT%01d_sm%03d_rpc%03d_Ch%03d_S%d_%s", iType[iSt], iSm, iRp, iCh, iSide, cOpt.Data());
                h     = (TH2*) gROOT->FindObjectAny(hname);
                if (h != NULL) {
                  TProfile* hProf = h->ProfileX(Form("%s_pfx%d%d", hname.Data(), iCh, iSide));
                  hProf->SetLineColor(iCol);
                  hProf->SetLineStyle(1);
                  hProf->SetMarkerColor(iCol);
                  hProf->SetMarkerStyle(24 + iSide);
                  iCol++;
                  if (iCh == 0) iCol = 1;
                  if (iCh == 0 && iSide == 0) {
                    hProf->SetMaximum(0.4);
                    hProf->SetMinimum(-0.4);
                    hProf->GetXaxis()->SetRangeUser(0., 10.);
                    hProf->Draw("LP");
                  }
                  else {
                    hProf->Draw("LPsame");
                  }
                }
              }
            break;
          default:
            hname = Form("cal_SmT%01d_sm%03d_rpc%03d_%s", iType[iSt], iSm, iRp, cOpt.Data());
            h     = (TH2*) gROOT->FindObjectAny(hname);
            if (h != NULL) {
              if (iOpt == 2 || iOpt == 2) { gPad->SetLogz(); }
              h->Draw("colz");
              h->ProfileX()->Draw("same");
              iDet++;
              dAvMean += h->ProfileX()->GetMean(2);
              dAvRMS += h->ProfileX()->GetRMS(2);
              cout << "TrackQA " << cOpt.Data() << " for TSR " << iType[iSt] << iSm << iRp << ": Off "
                   << h->ProfileX()->GetMean(2) << ", RMS " << h->ProfileX()->GetRMS(2) << endl;
            }
        }
      }
    }
  }
  dAvMean /= (Double_t) iDet;
  dAvRMS /= (Double_t) iDet;
  cout << "TrackQA " << cOpt.Data() << ": AvOff " << dAvMean << ", AvRMS " << dAvRMS << endl;
  dAvMean = TMath::Abs(dAvMean);
  gROOT->ProcessLine(Form(".! echo %d > %sAvOff.res", (Int_t)(dAvMean * 1.E4), cOpt.Data()));
  gROOT->ProcessLine(Form(".! echo %d > %sAvRMS.res", (Int_t)(dAvRMS * 1.E4), cOpt.Data()));

  can->SaveAs(Form("pl_all_Track_%s.pdf", cOpt.Data()));
}
