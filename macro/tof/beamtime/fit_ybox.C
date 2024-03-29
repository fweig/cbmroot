/* Copyright (C) 2016 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

// box with polynomial filling
Double_t f1_xboxe(double* x, double* par)
{
  double xx    = x[0];
  double wx    = 1. - par[4] * TMath::Power(xx + par[5], 2);
  double xboxe = par[0] * 0.25 * (1. + TMath::Erf((xx + par[1] - par[3]) / par[2]))
                 * (1. + TMath::Erf((-xx + par[1] + par[3]) / par[2]));
  return xboxe * wx;

  /* 
       common/pawpar/p(6)
       real wx 
       wx=1.-p(5)*(x+p(6))**2
C       wx=1.
       xboxe=p(1)*0.25*(1.+erf(( x+p(2)-p(4))/p(3)))
     &                *(1.+erf((-x+p(2)+p(4))/p(3)))
       xboxe=xboxe*wx
  */
}

void fit_ybox(TString hname)
{
  TH1* h1;
  TH2* h2;
  h1 = (TH1*) gROOT->FindObjectAny(hname);
  if (NULL != h1) {
    TAxis* xaxis  = h1->GetXaxis();
    Double_t Ymin = xaxis->GetXmin();
    Double_t Ymax = xaxis->GetXmax();
    TF1* f1       = new TF1("YBox", f1_xboxe, Ymin, Ymax, 6);
    Double_t yini = (h1->GetMaximum() + h1->GetMinimum()) * 0.5;
    f1->SetParameters(yini, Ymax * 0.8, 2., -1., 0., 0.);
    h1->Fit("YBox");

    double res[10];
    double err[10];
    res[9] = f1->GetChisquare();

    for (int i = 0; i < 6; i++) {
      res[i] = f1->GetParameter(i);
      err[i] = f1->GetParError(i);
      //cout << " FPar "<< i << ": " << res[i] << ", " << err[i] << endl;
    }
    cout << "YBox Fit of " << hname << " ended with chi2 = " << res[9]
         << Form(", strip length %7.2f +/- %5.2f, position resolution %7.2f "
                 "+/- %5.2f at y_cen = %7.2f +/- %5.2f",
                 2. * res[1], 2. * err[1], res[2], err[2], res[3], err[3])
         << endl;
  }
}

void pl_four4(TString hname)
{
  TFile* rootfile = gDirectory->GetFile();
  cout << " pl_four4 called " << endl;
  TKey* key = rootfile->FindKey(hname);
  if (key == 0) {
    cout << "!!Histogram " << hname << " does not exist!!" << endl;
    throw 1;
  }
  TH1F* hst = (TH1F*) rootfile->Get(hname);
  double res[10];
  double err[10];
  for (int i = 0; i < 8; i++) {
    res[i] = hst->GetBinContent(i);
    err[i] = hst->GetBinError(i);
    cout << " FPar " << i << ": " << res[i] << ", " << err[i] << endl;
  }

  TF1* f1 = new TF1("Fourier4", f1_four4, -180., 180., 9);
  f1->SetParameters(res);
  f1->Draw("");
}
