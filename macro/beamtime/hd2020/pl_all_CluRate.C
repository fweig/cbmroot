void pl_all_CluRate(Int_t iNSt      = 2,
                    Int_t iOpt      = 0,
                    Double_t Tstart = 0.,
                    Double_t Tend   = 800.,
                    Int_t iMode     = 0) {
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  switch (iMode) {
    case 0:
      switch (iNSt) {
        case 6: can->Divide(5, 7, 0.01, 0.01); break;
        case 5: can->Divide(5, 6, 0.01, 0.01); break;
        default: can->Divide(5, 6, 0.01, 0.01); break;
      }
      break;
    case 1:
      can->Divide(1, 2, 0.01, 0.01);
      ;
      break;
  }
  //  can->Divide(2,2,0,0);
  Float_t lsize = 0.06;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);
  gStyle->SetPadLeftMargin(0.4);
  gStyle->SetTitleOffset(1.0, "x");
  gStyle->SetTitleOffset(1.2, "y");
  gStyle->SetTitleFontSize(0.03);


  TH1* h;
  TH2* h2;
  const Int_t iType[6]   = {9, 6, 7, 5, 6, 8};
  const Int_t iSmNum[6]  = {3, 2, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {2, 2, 1, 1, 2, 8};

  Int_t iCanv = 0;
  Int_t iCol  = 0;
  // if (h!=NULL) h->Delete();

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    //   cout << "plot station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
    for (Int_t iSm = 0; iSm < iSmNum[iSt]; iSm++) {
      //cout << "plot module at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
      for (Int_t iRp = 0; iRp < iRpcNum[iSt]; iRp++) {
        //cout << "plot rpc at station "<<iSt<<" with "<< iSmNum[iSt] <<" modules of "<<iRpcNum[iSt]<<" Rpcs each"<<endl;
        gROOT->cd();
        TString hname = "";
        switch (iOpt) {
          case 0:
            hname =
              Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iType[iSt], iSm, iRp);
            break;
          case 1:
            hname =
              Form("cl_SmT%01d_sm%03d_rpc%03d_rate10s", iType[iSt], iSm, iRp);
            break;
        }
        h = (TH1*) gROOT->FindObjectAny(hname);
        if (h != NULL) {
          h->GetXaxis()->SetRangeUser(Tstart, Tend);
          switch (iMode) {
            case 0:
              can->cd(iCanv + 1);
              iCanv++;
              h->Draw("");
              //h->UseCurrentStyle();
              gPad->SetLogy();
              break;
            case 1:
              can->cd(iSt + 1);
              if (iSm > 0) continue;
              h->UseCurrentStyle();  // set current defaults
              h->SetMarkerStyle(20 + iSm);
              iCol = iRp + 1;
              if (iCol == 5) iCol++;
              h->SetLineColor(iCol);
              h->SetLineStyle(1);
              h->SetMarkerColor(iCol);
              if (iSm == 0 && iRp == 0)
                h->Draw("LPE");
              else
                h->Draw("LPEsame");
              break;
          }
        } else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_CluRate%d_%d.pdf", iNSt, iOpt));
}
