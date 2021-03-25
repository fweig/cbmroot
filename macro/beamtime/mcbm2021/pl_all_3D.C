void pl_all_3D(Int_t iOpt = 0, Int_t iSel = 0, Int_t iNSt = 4)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  can->Divide(5, 6, 0.01, 0.01);
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
  const Int_t iType[6]   = {0, 9, 6, 5, 7, 8};
  const Int_t iSmNum[6]  = {5, 1, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {5, 2, 2, 1, 1, 8};
  TString cOpt;

  switch (iOpt) {
    case 0: cOpt = "yx"; break;

    case 1:; break;

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
        TString hname = Form("cl_SmT%01d_sm%03d_rpc%03d_Sel%02d_Walk2", iType[iSt], iSm, iRp, iSel);
        h             = (TH3*) gROOT->FindObjectAny(hname);
        if (h != NULL) {
          if (iOpt == 4 || iOpt == 5) { gPad->SetLogz(); }
          h->Project3D(cOpt)->Draw("colz");
        }
        else {
          cout << "Histogram " << hname << " not existing. " << endl;
        }
      }
    }
  }
  can->SaveAs(Form("pl_all_%s.pdf", cOpt.Data()));
}
