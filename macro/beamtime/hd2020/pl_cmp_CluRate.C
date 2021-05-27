void pl_cmp_CluRate(Int_t iNSt = 3, Long_t iSet = 900032500, Int_t iOpt = 0, Double_t Tstart = 0., Double_t Tend = 10.,
                    Int_t iMode = 1)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);
  //  can->Divide(2,2,0,0);
  can->Divide(1, 3, 0.01, 0.01);
  Float_t lsize = 0.09;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  gStyle->SetOptStat(kFALSE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);
  gStyle->SetPadLeftMargin(0.4);
  gStyle->SetTitleOffset(1.0, "x");
  gStyle->SetTitleOffset(0.8, "y");
  gStyle->SetTitleFontSize(0.08);


  TH1* h[iNSt];
  TH1* hrat1[iNSt];
  TH1* hrat2[iNSt];

  TH2* h2;
  Int_t iType[6]         = {0, 9, 5, 6, 7, 8};
  const Int_t iSmNum[6]  = {5, 1, 1, 1, 1, 1};
  const Int_t iRpcNum[6] = {5, 2, 1, 2, 1, 8};
  Int_t iId_full[iNSt];

  Int_t iCanv = 0;
  Int_t iCol  = 0;
  // if (h!=NULL) h->Delete();
  TLegend* leg = new TLegend(0.78, 0.6, 0.85, 0.9);
  leg->SetTextSize(0.05);

  Long_t iSet_tmp = iSet;

  can->cd(1);
  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    gROOT->cd();
    Int_t iId     = iSet_tmp % 1000;
    iId_full[iSt] = iId;
    iSet_tmp      = (iSet_tmp - iId) / 1000;
    Int_t iRp     = iId % 10;
    iId -= iRp;
    iId /= 10;
    Int_t iSm = iId % 10;
    iId -= iSm;
    iId /= 10;
    iType[iSt] = iId;

    TString hname = "";
    switch (iOpt) {
      case 0: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iType[iSt], iSm, iRp); break;
      case 1: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate10s", iType[iSt], iSm, iRp); break;
    }
    h[iSt] = (TH1*) gROOT->FindObjectAny(hname);
    if (h[iSt] != NULL) {
      h[iSt]->GetXaxis()->SetRangeUser(Tstart, Tend);
      cout << "Draw " << iSt << ": " << hname << endl;
      leg->AddEntry(h[iSt], Form("%03d", iId_full[iSt]), "p");

      switch (iMode) {
        case 0:
          h[iSt]->Draw("");
          //h->UseCurrentStyle();
          gPad->SetLogy();
          break;
        case 1:
          h[iSt]->UseCurrentStyle();  // set current defaults
          h[iSt]->SetMarkerStyle(20 + iSm);
          h[iSt]->SetTitle("");
          iCol = iSt + 1;
          if (iCol == 5) iCol++;
          h[iSt]->SetLineColor(iCol);
          h[iSt]->SetLineStyle(1);
          h[iSt]->SetMarkerColor(iCol);
          if (iSt == 0) h[iSt]->Draw("LPE");
          else
            h[iSt]->Draw("LPEsame");
          break;
      }
    }
    else {
      cout << "Histogram " << hname << " not existing. " << endl;
    }
  }
  leg->Draw();

  can->cd(2);
  Double_t RatMax = 1.5;
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  for (Int_t iSt = 1; iSt < iNSt; iSt++) {
    hrat1[iSt] = (TH1*) h[iSt]->Clone();
    hrat1[iSt]->Divide(h[iSt], h[0], 1., 1.);
    hrat1[iSt]->SetYTitle(Form("Ratio to %03d", iId_full[0]));
    hrat1[iSt]->SetMaximum(RatMax);
    hrat1[iSt]->SetMinimum(1.E-2);
    if (iSt == 1) hrat1[iSt]->Draw("LPE");
    else
      hrat1[iSt]->Draw("LPEsame");
  }

  can->cd(3);
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  for (Int_t iSt = 2; iSt < iNSt; iSt++) {
    hrat2[iSt] = (TH1*) h[iSt]->Clone();
    hrat2[iSt]->Divide(h[iSt], h[1], 1., 1.);
    hrat2[iSt]->SetYTitle(Form("Ratio to %03d", iId_full[1]));
    hrat2[iSt]->SetMinimum(1.E-2);
    gPad->SetLogy();
    if (iSt == 2) hrat2[iSt]->Draw("LPE");
    else
      hrat2[iSt]->Draw("LPEsame");
  }

  can->SaveAs(Form("pl_cmp_CluRate%ld_%d.pdf", iSet, iOpt));
}
