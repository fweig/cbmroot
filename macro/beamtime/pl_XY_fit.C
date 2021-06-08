/* Copyright (C) 2017-2018 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_XY_fit(Int_t iNSt = 4, Int_t NewNbinsX = 32, Int_t NewNbinsY = 32, Int_t iOpt = 0)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  can->Divide(4, iNSt);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kFALSE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH3* h3;
  TH3* h3f;
  TH2* h2n;
  TH2* h2m;
  TH2* h2s;
  TString hname;
  TString cOpt;

  Int_t iCan = 1;
  gROOT->cd();
  switch (iOpt) {
    case 0: cOpt = "X"; break;
    case 1: cOpt = "Y"; break;
    case 2: cOpt = "T"; break;
  }

  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    can->cd(iCan);

    hname = Form("hXY_D%s_%d", cOpt.Data(), iSt);
    h3f   = (TH3*) gROOT->FindObjectAny(hname);
    cout << hname.Data() << " with pointer  " << h3f << " at iCan = " << iCan << endl;
    if (h3f != NULL) {

      can->cd(iCan);
      h3f->ProjectionZ()->Draw();
      gPad->SetLogy();
      iCan++;

      Int_t Nxbins = h3f->GetNbinsX();
      Int_t Nybins = h3f->GetNbinsY();
      Int_t Nzbins = h3f->GetNbinsZ();

      Int_t iDeltaX = Nxbins / NewNbinsX;
      Int_t iDeltaY = Nybins / NewNbinsY;
      cout << "Nbins: " << Nxbins << "," << Nybins << "," << Nzbins << " -> Dbin" << iDeltaX << "," << iDeltaY << endl;

      h2n = new TH2D(Form("hTH3FitNorm%s%d", cOpt.Data(), iSt),
                     Form("#Delta%s Fit Entries; x (cm); y (cm)", cOpt.Data()), NewNbinsX, h3f->GetXaxis()->GetXmin(),
                     h3f->GetXaxis()->GetXmax(), NewNbinsY, h3f->GetYaxis()->GetXmin(), h3f->GetYaxis()->GetXmax());
      h2m = new TH2D(Form("hTH3FitMean%s%d", cOpt.Data(), iSt), Form("#Delta%s Fit Mean; x (cm); y (cm)", cOpt.Data()),
                     NewNbinsX, h3f->GetXaxis()->GetXmin(), h3f->GetXaxis()->GetXmax(), NewNbinsY,
                     h3f->GetYaxis()->GetXmin(), h3f->GetYaxis()->GetXmax());
      h2s = new TH2D(Form("hTH3FitSigma%s%d", cOpt.Data(), iSt),
                     Form("#Delta%s Fit Sigma; x (cm); y (cm)", cOpt.Data()), NewNbinsX, h3f->GetXaxis()->GetXmin(),
                     h3f->GetXaxis()->GetXmax(), NewNbinsY, h3f->GetYaxis()->GetXmin(), h3f->GetYaxis()->GetXmax());

      Int_t iCol = 0;
      ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

      for (Int_t ix = 0; ix < NewNbinsX; ix++) {
        for (Int_t iy = 0; iy < NewNbinsY; iy++) {
          cout << " project ix=" << ix << ", iy=" << iy << endl;

          TH1* hsel       = h3f->ProjectionZ(Form("Delta%s%d_%d_%d", cOpt.Data(), iSt, ix, iy), ix * iDeltaX + 1,
                                       (ix + 1) * iDeltaX + 1, iy * iDeltaY + 1, (iy + 1) * iDeltaY + 1);
          Double_t dFMean = hsel->GetMean();
          Double_t dFLim  = 2.5 * hsel->GetRMS();
          //	 hsel->SetLineC.olor(iCol++);
          cout << " fit ix=" << ix << ", iy=" << iy << ", Integral: " << hsel->Integral() << ", Mean " << dFMean
               << ", Lim " << dFLim << endl;
          if (hsel->Integral() > 10) {
            TFitResultPtr fRes = hsel->Fit("gaus", "S", "HEsame", dFMean - dFLim, dFMean + dFLim);
            Int_t iFitStatus   = fRes;
            if (iFitStatus == 0) {  // check validity of fit
              h2n->SetBinContent(ix + 1, iy + 1, hsel->Integral());
              h2m->SetBinContent(ix + 1, iy + 1, fRes->Parameter(1));
              h2s->SetBinContent(ix + 1, iy + 1, fRes->Parameter(2));
              cout << "Fres: " << fRes->Parameter(1) << ", " << fRes->Parameter(2) << endl;
            }
          }
        }
      }
      can->cd(iCan);
      {
        cout << "Draw " << h2n->GetName() << endl;
        h2n->Draw("colz");
        //gPad->SetLinz();
        gPad->SetMargin(0.23, 0.23, 0.23, 0.23);
      }
      iCan++;
      can->cd(iCan);
      {
        cout << "Draw " << h2m->GetName() << endl;
        h2m->Draw("colz");
        gPad->SetMargin(0.23, 0.23, 0.23, 0.23);
      }
      iCan++;
      can->cd(iCan);
      {
        cout << "Draw " << h2s->GetName() << endl;
        h2s->Draw("colz");
        gPad->SetMargin(0.23, 0.23, 0.23, 0.23);
      }
      iCan++;
    }
    else
      cout << hname << " not found" << endl;
  }
  can->SaveAs("pl_XY_fit.pdf");
}
