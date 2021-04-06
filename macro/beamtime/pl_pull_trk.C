void pl_pull_trk(Int_t NSt = 8, Int_t iVar = 0, Int_t iFit = 0, Int_t iDrop = -1)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 800, 800);
  switch (NSt) {
    case 7: can->Divide(3, 4); break;
    case 6:
    case 5:
    case 4: can->Divide(3, 3); break;
    case 9: can->Divide(3, 4); break;
    case 18: can->Divide(4, 5); break;
    default: can->Divide(4, 4); ;
  }
  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(kTRUE);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;

  const Int_t MSt = 30;
  Double_t vSt[MSt];
  Double_t vMean[MSt];
  Double_t vSig[MSt];
  Double_t vRes[MSt];
  Double_t vStErr[MSt];
  Double_t vMeanErr[MSt];
  Double_t vSigErr[MSt];
  Double_t vResErr[MSt];
  // if (h!=NULL) h->Delete();
  Int_t iCan   = 1;
  Int_t iIndSt = 0;
  TString var;
  Double_t Nall;

  switch (iVar) {
    case 0: var = "X"; break;
    case 1: var = "Y"; break;
    case 2: var = "Z"; break;
    case 3: var = "T"; break;
    case 4: var = "TB"; break;
  }
  for (Int_t iSt = 0; iSt < NSt; iSt++) {
    can->cd(iCan++);
    gROOT->cd();
    TString hname = Form("hPull%s_Station_%d", var.Data(), iSt);
    h1            = (TH1*) gROOT->FindObjectAny(hname);
    if (h1 != NULL) {
      h1->Draw("");
      Nall = h1->GetEntries();
      gPad->SetLogy();
      gPad->SetGridx();
      if (iFit > 0) {
        //Double_t dFMean   = h1->GetMean();
        Double_t dFMean    = h1->GetBinCenter(h1->GetMaximumBin());
        Double_t dFLim     = 2.0 * h1->GetRMS();
        Double_t dBinSize  = h1->GetBinWidth(1);
        dFLim              = TMath::Max(dFLim, 5. * dBinSize);
        TFitResultPtr fRes = h1->Fit("gaus", "SQM", "", dFMean - dFLim, dFMean + dFLim);
        //cout << " fRes = "<< fRes <<endl;
        if (-1 == fRes) continue;
        if (iDrop == iSt) {  // drop station from deconvolution
          continue;
        }
        cout << "Add " << iSt << " as station index " << iIndSt << endl;
        vSt[iIndSt]      = iSt;
        vMean[iIndSt]    = fRes->Parameter(1);
        vSig[iIndSt]     = fRes->Parameter(2);
        vStErr[iIndSt]   = 0.;
        vMeanErr[iIndSt] = fRes->ParError(1);
        vSigErr[iIndSt]  = fRes->ParError(2);
        //vSig[iIndSt]=TMath::Max(20.,vSig[iSt]);
        iIndSt++;
      }
    }
    else {
      cout << hname << " not found" << endl;
    }
  }
  if (0 == iFit) return;
  cout << "Process " << iIndSt << " fit values " << endl;
  can->cd(iCan++);
  Double_t dLMargin   = 0.35;
  Double_t dTitOffset = 1.8;
  gPad->SetLeftMargin(dLMargin);
  TGraphErrors* grm = new TGraphErrors(iIndSt, vSt, vMean, vStErr, vMeanErr);
  grm->SetTitle("Mean");
  grm->GetXaxis()->SetTitle("Station number");
  switch (iVar) {
    case 0:
    case 1:
    case 2: grm->GetYaxis()->SetTitle("mean deviation (cm)"); break;
    default: grm->GetYaxis()->SetTitle("mean deviation (ns)");
  }
  grm->GetYaxis()->SetTitleOffset(dTitOffset);
  grm->GetXaxis()->SetLimits(-0.5, NSt - 0.5);
  grm->SetMarkerStyle(24);
  grm->Draw("APLE");

  can->cd(iCan++);
  gPad->SetLeftMargin(dLMargin);
  TGraphErrors* grs = new TGraphErrors(iIndSt, vSt, vSig, vStErr, vSigErr);
  grs->SetTitle("Gaussian width");
  grs->GetXaxis()->SetTitle("Station number");
  switch (iVar) {
    case 0:
    case 1:
    case 2: grs->GetYaxis()->SetTitle("Gaussian sigma (cm)"); break;
    default: grs->GetYaxis()->SetTitle("Gaussian sigma (ns)");
  }
  grs->GetYaxis()->SetTitleOffset(dTitOffset);
  grs->GetXaxis()->SetLimits(-0.5, NSt - 0.5);
  grs->SetMarkerStyle(24);
  grs->Draw("APLE");

  can->cd(iCan++);
  gPad->SetLeftMargin(dLMargin);
  Double_t val = (iIndSt - 1) * (iIndSt - 1);
  TMatrixD a(iIndSt, iIndSt);
  for (Int_t i = 0; i < iIndSt; i++)
    for (Int_t j = 0; j < iIndSt; j++) {
      if (i == j) { a[i][j] = 1; }
      else {
        a[i][j] = 1. / val;
      }
    }
  a.Draw("colz");
  a.Print();

  // can->cd(iCan++);
  TMatrixD ainv = a;
  ainv.Invert();
  ainv.Draw("colz");
  ainv.Print();
  TMatrixD aSig(iIndSt, 1);
  for (Int_t i = 0; i < iIndSt; i++)
    aSig[i][0] = vSig[i] * vSig[i];

  cout << "Measured gaussian widths: " << endl;
  aSig.Print();
  TMatrixD xRes = ainv * aSig;
  cout << "Resolution of counters: " << endl;
  xRes.Print();

  //can->cd(iCan++);
  for (Int_t i = 0; i < iIndSt; i++) {
    vRes[i]    = TMath::Sqrt(TMath::Abs(xRes[i][0]));
    vResErr[i] = vSigErr[i];
  }
  TGraphErrors* grr = new TGraphErrors(iIndSt, vSt, vRes, vStErr, vResErr);
  grr->SetTitle("Final resolution");
  grr->GetXaxis()->SetTitle("Station number");
  switch (iVar) {
    case 0:
    case 1:
    case 2: grr->GetYaxis()->SetTitle("resolution (cm)"); break;
    default: grr->GetYaxis()->SetTitle("resolution (ns)");
  }
  grr->GetYaxis()->SetTitleOffset(dTitOffset);
  grr->GetXaxis()->SetLimits(-0.5, NSt - 0.5);
  //grr->GetXaxis()->SetRangeUser(-0.5,NSt-0.5);
  grr->SetMarkerStyle(24);
  grr->Draw("APLE");

  for (Int_t i = 0; i < iIndSt; i++)
    cout << Form("GMean %6.3f +/- %6.5f, GSig: %6.3f +/- %6.5f => ResC %d: %6.3f ", vMean[i], vMeanErr[i], vSig[i],
                 vSigErr[i], i, vRes[i])
         << endl;

  cout << "Res-summary " << iVar << ": Nall, sigs = " << Nall;
  for (Int_t i = 0; i < iIndSt; i++)
    cout << Form(", %7.4f", vRes[i]);
  cout << endl;

  can->SaveAs(Form("pl_pull_trk_%s%02d.pdf", var.Data(), NSt));
}
