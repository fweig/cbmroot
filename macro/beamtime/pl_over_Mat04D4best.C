/* Copyright (C) 2016-2020 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_over_Mat04D4best(Int_t iLog = 0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 1000, 800);
  can->Divide(5, 6);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);
  // cout << " DirLevel "<< gROOT->GetDirLevel()<< endl;

  TH1* h;
  TH1* h1;
  TH1* hDT04;
  TH1* h2px;
  TH1* h2py;
  TH1* h2pfx;
  TH1* h2pfxpx;
  TH1* h2pfxx;
  TH1* h2pfxt;
  TH1* h2clu;

  TH2* h2;

  Double_t NEvents;
  Double_t NFinalHits;
  Double_t dTRes;
  Double_t dTRMS;
  Double_t dTMean;
  Double_t dMeanX;
  Double_t dSigX;
  Double_t dMeanY;
  Double_t dSigY;
  Double_t dMeanCluSize0;
  Double_t dMeanCluSize4;

  // if (hPla!=NULL) hPla->Delete();

  can->cd(1);
  {
    gROOT->cd();
    TString hname = "hDXDY04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (NULL == h2) return;
    if (100 > h2->GetEntries()) {
      cout << "Insufficient number of events " << h2->GetEntries() << endl;
      return;
    }
    if (h2 != NULL) {
      h2->Draw("colz");

      gPad->SetLogz();
      can->cd(2);
      h2px = h2->ProjectionX();

      TFitResultPtr fResx = h2px->Fit("gaus", "S", "", -2. * h2px->GetRMS(), 2. * h2px->GetRMS());
      if (static_cast<Int_t>(fResx)) { cout << "res X fit failed" << endl; }
      else {
        dMeanX = fResx->Parameter(1);
        dSigX  = fResx->Parameter(2);
      }
      h2py                = h2->ProjectionY();
      TFitResultPtr fResy = h2py->Fit("gaus", "S", "", -2. * h2py->GetRMS(), 2. * h2py->GetRMS());
      if (static_cast<Int_t>(fResy)) { cout << "res Y fit failed" << endl; }
      else {
        dMeanY = fResy->Parameter(1);
        dSigY  = fResy->Parameter(2);
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(2);
  {
    gROOT->cd();
    TString hname = "hDXDT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(3);
  {
    gROOT->cd();
    TString hname = "hDYDT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(4);
  {
    gROOT->cd();
    TString hname = "hDTD4DT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      TProfile* h2pfx = h2->ProfileX();
      h2pfx->Draw("same");

      TString hname2 = "hDTD4DT04D4best_pfx_px";
      h2pfxpx        = (TH1*) gROOT->FindObjectAny(hname2);
      if (h2pfxpx != NULL) {
        h2pfxpx->SetLineColor(6);
        h2pfxpx->Draw("same");
      }
      Int_t iNbinsX = h2->GetNbinsX();
      cout << " Histo " << hname << " has " << iNbinsX << " Xbins" << endl;
      const char* cnamP = Form("%s_py", (char*) hname.Data());
      hDT04             = (TH1D*) h2->ProjectionY(cnamP, 1, iNbinsX);
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(5);
  {
    gROOT->cd();
    TString hname = "hChi04D4best";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw();
      NFinalHits = h1->GetEntries();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(6);
  {
    gROOT->cd();
    TString hname = "hNMatchD4sel";  // normalization
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw();
      gPad->SetLogy();
      NEvents = h1->GetEntries();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(7);
  {
    gROOT->cd();
    hDT04->Draw();
    gPad->SetLogy();
    TFitResultPtr fRes = hDT04->Fit("gaus", "S");
    // fRes->Print();
    if (static_cast<Int_t>(fRes)) { cout << "res T fit failed" << endl; }
    else {
      dTMean = fRes->Parameter(1);
      dTRes  = fRes->Parameter(2);
    }
    dTRMS = hDT04->GetRMS();
  }

  can->cd(8);
  {
    gROOT->cd();
    TString hname = "hDTD4sel";
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw();
      //  Double_t NSel=h1->Integral(BL,h1->GetNbinsX(),"");
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(9);
  {
    gROOT->cd();
    // TString hname="hDistDT04D4best";
    TString hname = "hTexpDT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      h2->ProfileX()->Draw("same");
      gPad->SetLogz();
      TString hname2 = "hTexpDT04D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(10);
  {
    gROOT->cd();
    TString hname = "hDTMul4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(11);
  {
    gROOT->cd();
    TString hname = "hDTX4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      TProfile* h2pfx = h2->ProfileX();
      h2pfx->Draw("same");

      TString hname2 = "hDTX4D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(12);
  {
    gROOT->cd();
    TString hname = "hDTY4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      TProfile* h2pfxx = h2->ProfileX();
      h2pfxx->Draw("same");
      gPad->SetLogz();

      TString hname2 = "hDTY4D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(13);
  {
    gROOT->cd();
    TString hname = "hDTMul0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(14);
  {
    gROOT->cd();
    TString hname = "hDTX0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(15);
  {
    gROOT->cd();
    TString hname = "hDTY0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(16);
  {
    gROOT->cd();
    TString hname = "hCluSize0DT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      TH1D* h2clu   = h2->ProjectionX("_px", 1, h2->GetNbinsY());
      dMeanCluSize0 = h2clu->GetMean();
      h2->ProfileX()->Draw("same");

      TString hname2 = "hCluSize0DT04D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(17);
  {
    gROOT->cd();
    TString hname = "hCluSize4DT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
      TH1D* h2clu   = h2->ProjectionX("_px", 1, h2->GetNbinsY());
      dMeanCluSize4 = h2clu->GetMean();
      h2->ProfileX()->Draw("same");

      TString hname2 = "hCluSize4DT04D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(18);
  {
    gROOT->cd();
    TString hname = "hTot0DT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      TProfile* h2pfxt = h2->ProfileX();
      h2pfxt->Draw("same");
      gPad->SetLogz();

      TString hname2 = "hTot0DT04D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(19);
  {
    gROOT->cd();
    TString hname = "hTot4DT04D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      TProfile* h2pfxt = h2->ProfileX();
      h2pfxt->Draw("same");
      gPad->SetLogz();

      TString hname2 = "hTot4DT04D4best_pfx_px";
      TH1* hpfxpx    = (TH1*) gROOT->FindObjectAny(hname2);
      if (hpfxpx != NULL) {
        hpfxpx->SetLineColor(6);
        hpfxpx->Draw("same");
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(20);
  {
    gROOT->cd();
    TString hname = "hDXX4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(21);
  {
    gROOT->cd();
    TString hname = "hDXY4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(22);
  {
    gROOT->cd();
    TString hname = "hDYX4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(23);
  {
    gROOT->cd();
    TString hname = "hDYY4D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(24);
  {
    gROOT->cd();
    TString hname = "hDXX0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(25);
  {
    gROOT->cd();
    TString hname = "hDXY0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(26);
  {
    gROOT->cd();
    TString hname = "hDYX0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(27);
  {
    gROOT->cd();
    TString hname = "hDYY0D4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("colz");
      gPad->SetLogz();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(28);
  {
    gROOT->cd();
    TString hname = "hTofD4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("");
      gPad->SetLogy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }

  can->cd(29);
  {
    gROOT->cd();
    TString hname = "hVelD4best";
    h2            = (TH2*) gROOT->FindObjectAny(hname);
    if (h2 != NULL) {
      h2->Draw("");
      gPad->SetLogy();
    }
    else {
      cout << hname << " not found" << endl;
    }
  }
  //report summary
  Double_t dEff = NFinalHits / NEvents;
  cout << Form("<IRes> Efficiency: %6.3f, %7.0f of %7.0f events, mean Dt %6.4f "
               "ns, 1-sig time res: %6.4f ns, RMS: %6.4f ns, DX %5.2f, DY "
               "%5.2f, SigX %5.2f, SigY %5.2f, CluSize %4.1f,%4.1f ",
               dEff, NFinalHits, NEvents, dTMean, dTRes, dTRMS, dMeanX, dMeanY, dSigX, dSigY, dMeanCluSize0,
               dMeanCluSize4)
       << endl;

  // cout << Form("    D4best 1-sigma timing resolution at lower index %d: %6.1f ns, RMS: %6.1f ns from  %6.0f entries of %6.0f (%6.3f)",BL,BRes,Brms,BEntries,NSel,BEntries/NSel) <<endl;
  if (iLog != 0) gROOT->ProcessLine(Form(".! echo %f  > Test.res", dTRes));

  can->SaveAs("pl_over_Mat04D4best.pdf");
}
