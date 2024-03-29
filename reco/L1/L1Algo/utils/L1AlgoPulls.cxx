/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#include "L1AlgoPulls.h"

const TString L1TrackParametersNames[TL1TrackParameters::NParameters] = {"x", "y", "tx", "ty", "qp"};

void L1AlgoPulls::Init()
{
  fL1 = CbmL1::Instance();

  static bool first_call = 1;
  if (first_call) {
    //     TDirectory *curdir = gDirectory;
    //     fL1->histodir->cd();
    //     gDirectory->mkdir("L1AlgoPulls");
    //     gDirectory->cd("L1AlgoPulls");

    // add global pulls
    for (int i = 0; i < TL1TrackParameters::NParameters; i++) {
      TString name = "pull_";
      name += L1TrackParametersNames[i];
      histoPull[i] = new TH1F(name, name, 50, -10, 10);
    }

#ifdef BUILD_HISTO_FOR_EACH_STANTION
    // add station pulls
    for (int i = TL1TrackParameters::NParameters; i < (NStations + 1) * TL1TrackParameters::NParameters; i++) {
      int ista     = i / TL1TrackParameters::NParameters - 1;
      TString name = "pull_sta";
      name += ista;
      name += "_";
      name += L1TrackParametersNames[i % TL1TrackParameters::NParameters];
      histoPull[i] = new TH1F(name, name, 50, -10, 10);
    }
#endif

    // add global residuals
    for (int i = 0; i < TL1TrackParameters::NParameters; i++) {
      TString name = "residual_";
      name += L1TrackParametersNames[i];
      float size = 10;
      switch (i) {
        case 0: size = .01; break;
        case 1: size = .01; break;
        case 2: size = 0.003; break;
        case 3: size = 0.003; break;
        case 4: size = 0.1; break;
      };
      histoRes[i] = new TH1F(name, name, 50, -size, size);
    }

    // define style
    histoStyle = new TStyle("histoStyle", "Plain Style(no colors/fill areas)");

    histoStyle->SetTextFont(textFont);
    histoStyle->SetPadColor(0);
    histoStyle->SetCanvasColor(0);
    histoStyle->SetTitleColor(0);
    histoStyle->SetStatColor(0);

    histoStyle->SetOptTitle(0);  // without main up title
    histoStyle->SetOptStat(1000001010);
    //   The parameter mode can be = ksiourmen  (default = 000001111)
    //   k = 1;  kurtosis printed
    //   k = 2;  kurtosis and kurtosis error printed
    //   s = 1;  skewness printed
    //   s = 2;  skewness and skewness error printed
    //   i = 1;  integral of bins printed
    //   o = 1;  number of overflows printed
    //   u = 1;  number of underflows printed
    //   r = 1;  rms printed
    //   r = 2;  rms and rms error printed
    //   m = 1;  mean value printed
    //   m = 2;  mean and mean error values printed
    //   e = 1;  number of entries printed
    //   n = 1;  name of histogram is printed

    histoStyle->SetOptFit(10001);
    //   The parameter mode can be = pcev  (default = 0111)
    //   p = 1;  print Probability
    //   c = 1;  print Chisquare/Number of degress of freedom
    //   e = 1;  print errors (if e=1, v must be 1)
    //   v = 1;  print name/values of parameters

    histoStyle->SetStatW(0.175);
    histoStyle->SetStatH(0.02);
    histoStyle->SetStatX(0.95);
    histoStyle->SetStatY(0.97);
    histoStyle->SetStatFontSize(0.05);

    histoStyle->SetStatFont(textFont);


    //     gDirectory->cd("..");
    //     curdir->cd();
    first_call = 0;
  }
};

//  void L1AlgoPulls::AddVec(L1TrackPar& T_, L1HitIndex_t ih)
// {
//   for (int i = 0; i < fvecLen; i++)
//     AddOne(T_,i,ih);
// }

void L1AlgoPulls::AddOne(L1TrackPar& T_, int i, L1HitIndex_t ih)
{
  fNAllPulls++;
  TL1TrackParameters T(T_, i);

  if (T_.chi2[i] > csCut * T_.NDF[i]) return;
  // get error
  TL1TrackParameters err;
  const L1TrackPar& tr = T_;

  if (!(std::isfinite(tr.C00[i]) && tr.C00[i] > 0)) return;
  if (!(std::isfinite(tr.C11[i]) && tr.C11[i] > 0)) return;
  if (!(std::isfinite(tr.C22[i]) && tr.C22[i] > 0)) return;
  if (!(std::isfinite(tr.C33[i]) && tr.C33[i] > 0)) return;
  if (!(std::isfinite(tr.C44[i]) && tr.C44[i] > 0)) return;
  err.x  = sqrt(tr.C00[i]);
  err.y  = sqrt(tr.C11[i]);
  err.tx = sqrt(tr.C22[i]);
  err.ty = sqrt(tr.C33[i]);
  err.qp = sqrt(tr.C44[i]);

  // mc data
  int iMCP = fL1->fvHitPointIndexes[ih];
  if (iMCP < 0) return;
  CbmL1MCPoint& mcP = fL1->fvMCPoints[iMCP];
  TL1TrackParameters mc(mcP);

  // fill residuals
  TL1TrackParameters res = (mc - T);
  fGRes.push_back(res);

  // fill pulls
  TL1TrackParameters P = res / err;
  fGPulls.push_back(P);

#ifdef BUILD_HISTO_FOR_EACH_STANTION
  int ista = mcP.iStation - 2;  // last hit
  //   int ista = fL1->algo->vSFlag[ fL1->algo->GetInputData()->GetHit(ih).f ]/4 - 2; // last hit
  fStaPulls[ista].push_back(P);
#endif  // BUILD_HISTO_FOR_EACH_STANTION
};

void L1AlgoPulls::Print()
{  // TODO: renew
  cout << "All pulls:     " << fNAllPulls << endl;
  cout << "Correct pulls: " << fGPulls.size() << endl;
  cout << "x     y    tx    ty    qp" << endl;
  for (unsigned int i = 0; i < fGPulls.size(); i++) {
    TL1TrackParameters& pull = fGPulls[i];
    pull.Print();
  }
};

void L1AlgoPulls::Build(bool draw)
{
  // --- fill histograms ---
  // global pulls
  for (unsigned int i = 0; i < fGPulls.size(); i++) {
    TL1TrackParameters& pull = fGPulls[i];
    for (int ih = 0; ih < TL1TrackParameters::NParameters; ih++) {
      if (TailCut > fabs(pull[ih])) histoPull[ih]->Fill(pull[ih]);
    }
  }
#ifdef BUILD_HISTO_FOR_EACH_STANTION
  // station pulls
  for (int iSta = 0; iSta < NStations; iSta++) {
    vector<TL1TrackParameters>& Pulls = fStaPulls[iSta];
    for (int i = 0; i < Pulls.size(); i++) {
      TL1TrackParameters& pull = Pulls[i];
      for (int ih = 0; ih < TL1TrackParameters::NParameters; ih++) {
        if (TailCut > fabs(pull[ih])) histoPull[(iSta + 1) * TL1TrackParameters::NParameters + ih]->Fill(pull[ih]);
      }
    }
  }
#endif  // BUILD_HISTO_FOR_EACH_STANTION

  // global residuals
  for (unsigned int i = 0; i < fGRes.size(); i++) {
    TL1TrackParameters& res = fGRes[i];
    for (int ih = 0; ih < TL1TrackParameters::NParameters; ih++) {
      if (TailCut > fabs(res[ih])) histoRes[ih]->Fill(res[ih]);
    }
  }

  // --- draw histograms --- and save info
  float pulls[(NStations + 1) * TL1TrackParameters::NParameters][2],  // 0 - sigma, 1 - RMS
    residuals[(NStations + 1) * TL1TrackParameters::NParameters][2];

  system("mkdir L1_Pulls -p");
  chdir("L1_Pulls");
  TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 600, 400);
  c2->cd();
  histoStyle->cd();
  for (int ih = 0; ih < (NStations + 1) * TL1TrackParameters::NParameters; ih++) {
    makeUpHisto(histoPull[ih], histoPull[ih]->GetName(), pulls[ih][0]);
    pulls[ih][1] = histoPull[ih]->GetRMS();
    if (draw) {
      histoPull[ih]->Draw();
      TString name = histoPull[ih]->GetName();
      name += ".pdf";
      c2->SaveAs(name);
    }
  }
  for (int ih = 0; ih < (0 + 1) * TL1TrackParameters::NParameters; ih++) {
    makeUpHisto(histoRes[ih], histoRes[ih]->GetName(), residuals[ih][0]);
    residuals[ih][1] = histoRes[ih]->GetRMS();
    if (draw) {
      histoRes[ih]->Draw();
      TString name = histoRes[ih]->GetName();
      name += ".pdf";
      c2->SaveAs(name);
    }
  }
  chdir("..");

  // --- print information ---
  cout << "All entries:     " << fNAllPulls << endl;
  cout << "Correct entries: " << fGPulls.size() << endl;
  cout << "Pulls sigma & RMS: " << endl;
  for (int ih = 0; ih < (NStations + 1) * TL1TrackParameters::NParameters; ih++) {
    int ipar = ih % TL1TrackParameters::NParameters;
    int ista = ih / TL1TrackParameters::NParameters;
    if ((ista > 0) && (ipar == 0)) cout << "Station " << ista - 1 << endl;
    cout << L1TrackParametersNames[ipar] << "\t" << pulls[ih][0] << "\t" << pulls[ih][1] << endl;
  }
  cout << "Residuals sigma & RMS: " << endl;
  for (int ih = 0; ih < (0 + 1) * TL1TrackParameters::NParameters; ih++) {
    int ipar = ih % TL1TrackParameters::NParameters;
    cout << L1TrackParametersNames[ipar] << "\t" << residuals[ih][0] << "\t" << residuals[ih][1] << endl;
  }
}

void L1AlgoPulls::makeUpHisto(TH1* hist, TString title, float& sigma)
{
  if (hist && (hist->GetEntries() != 0)) {
    TF1* fit = new TF1("fit", "gaus");
    fit->SetLineColor(2);
    fit->SetLineWidth(3);
    hist->Fit("fit", "", "", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    sigma = fit->GetParameter(2);

    hist->GetXaxis()->SetLabelFont(textFont);
    hist->GetXaxis()->SetTitleFont(textFont);
    hist->GetYaxis()->SetLabelFont(textFont);
    hist->GetYaxis()->SetTitleFont(textFont);

    hist->GetXaxis()->SetTitle(title);
    hist->GetXaxis()->SetTitleOffset(1);
    hist->GetYaxis()->SetTitle("Entries");
    hist->GetYaxis()->SetTitleOffset(1.05);  // good then entries per bit <= 9999
  }
  else {
    std::cout << " E: Read hists error! " << std::endl;
  }
}
