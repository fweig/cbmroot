/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_all_2D(Int_t iOpt = 0, Int_t iNSt = 7)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 1000);
  can->Divide(5, 8, 0.01, 0.01);
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
    case 5: cOpt = "AvLnWalk"; break;
    case 6: cOpt = "Mul"; break;
    case 7: cOpt = "Trms"; break;
    case 8: cOpt = "DelPos"; break;
    case 9: cOpt = "DelTOff"; break;
    case 10: cOpt = "DelMatPos"; break;
    case 11: cOpt = "DelMatTOff"; break;
    case 12: cOpt = "rate"; break;
    case 13: cOpt = "GloPos"; break;
    default:;
  }

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    // cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    for (Int_t iSm = 0; iSm < iSmNum[iSt]; iSm++) {
      //cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
      for (Int_t iRp = 0; iRp < iRpcNum[iSt]; iRp++) {
        //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
        can->cd(iCanv + 1);
        iCanv++;
        gROOT->cd();
        TString hname = Form("cl_SmT%01d_sm%03d_rpc%03d_%s", iType[iSt], iSm, iRp, cOpt.Data());
        h             = (TH2*) gROOT->FindObjectAny(hname);
        if (h != NULL) {
          cout << h->GetName() << " has " << h->GetEntries() << " entries " << endl;
          if (iOpt == 4 || iOpt == 5) { gPad->SetLogz(); }
          h->Draw("colz");
          if (iOpt == 7) { h->ProfileX()->Draw("same"); }
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_%s.pdf", cOpt.Data()));
}
