/** @file pl_lambda.C
  * @copyright Copyright (C) 2018 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Norbert Herrmann [orginator] **/

void pl_lambda(Double_t sf = 0.)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  TCanvas* can = new TCanvas("can", "can", 50, 0, 900, 900);
  can->Divide(3, 4);

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptFit(1111);

  gROOT->cd();
  gROOT->SetDirLevel(1);

  TH1* h;
  TH1* h1;
  TH2* h2;
  TH1* hMinvall;

  // if (h!=NULL) h->Delete();

  can->cd(1);
  gROOT->cd();
  TString hname1 = Form("hDperp");
  h1             = (TH1*) gROOT->FindObjectAny(hname1);
  if (h1 != NULL) {
    h1->Draw();
    //gPad->SetLogz();
  }
  else {
    cout << hname1 << " not found -> return" << endl;
    return;
  }

  can->cd(2);
  gROOT->cd();

  TString hname2 = Form("hDperp2");
  h1             = (TH1*) gROOT->FindObjectAny(hname2);
  if (h1 != NULL) {
    h1->Draw();
    //gPad->SetLogz();
  }

  can->cd(3);
  gROOT->cd();
  TString hname3 = Form("hDperpS");
  h1             = (TH1*) gROOT->FindObjectAny(hname3);
  if (h1 != NULL) {
    h1->Draw();
    //gPad->SetLogz();
  }
  else {
    cout << hname3 << " not found" << endl;
  }

  can->cd(4);
  gROOT->cd();
  TString hname4 = Form("hD0prim");
  h1             = (TH1*) gROOT->FindObjectAny(hname4);
  if (h1 != NULL) {
    h1->Draw();
    //gPad->SetLogz();
  }
  else {
    cout << hname4 << " not found" << endl;
  }

  can->cd(7);
  gROOT->cd();
  TString hname7 = Form("hMinv");
  h1             = (TH1*) gROOT->FindObjectAny(hname7);
  if (h1 != NULL) {
    h1->Draw();
    h1->SetLineColor(4);
    hname7   = "hMinvall";
    hMinvall = (TH1*) gROOT->FindObjectAny(hname7);
    if (NULL != hMinvall) hMinvall->Delete();
    hMinvall = (TH1*) h1->Clone();
    hMinvall->SetName("hMinvall");
    hMinvall->SetTitle("hMinvall");

    hname7 = "hMIXMinv";
    h      = (TH1*) gROOT->FindObjectAny(hname7);
    if (sf == 0) {  // find proper MIX scaling factor
      Double_t NML   = 1.090;
      Double_t NMH   = 1.097;
      Double_t dComb = h1->Integral(h1->FindBin(NML), h1->FindBin(NMH));
      Double_t dMix  = h->Integral(h->FindBin(NML), h->FindBin(NMH));
      sf             = dComb / dMix;
      cout << "Normalization factor sf = " << sf << endl;
    }
    h->Scale(sf);
    h->Draw("same");
    h->SetLineColor(7);

    hname7        = "hMinvdif";
    TH1* hMinvdif = (TH1*) gROOT->FindObjectAny(hname7);
    if (NULL != hMinvdif) hMinvdif->Delete();
    hMinvdif = (TH1*) h1->Clone();
    hMinvdif->Add(h1, h, 1., -1.);
    hMinvdif->SetName("hMinvdif");
    hMinvdif->SetTitle("hMinvdif");
    hMinvdif->Draw("same");
    hMinvdif->SetLineColor(3);
    //gPad->SetLogz();
  }
  else {
    cout << hname7 << " not found" << endl;
  }

  can->cd(5);
  gROOT->cd();
  TString hname5 = Form("hOpAng");
  h1             = (TH1*) gROOT->FindObjectAny(hname5);
  if (h1 != NULL) {
    h1->Draw();
    gPad->SetLogy();
    hname5 = "hMIXOpAng";
    h      = (TH1*) gROOT->FindObjectAny(hname5);
    h->Draw("same");
    h->Scale(sf);
    h->SetLineColor(7);
  }
  else {
    cout << hname5 << " not found" << endl;
  }

  can->cd(6);
  gROOT->cd();
  TString hname6 = Form("hDCA");
  h1             = (TH1*) gROOT->FindObjectAny(hname6);
  if (h1 != NULL) {
    h1->Draw();
    hname6 = "hMIXDCA";
    h      = (TH1*) gROOT->FindObjectAny(hname6);
    h->Scale(sf);
    h->Draw("same");
    h->SetLineColor(7);
    gPad->SetLogy();
  }
  else {
    cout << hname6 << " not found" << endl;
  }

  can->cd(8);
  gROOT->cd();
  TString hname8 = Form("hPathLen");
  h1             = (TH1*) gROOT->FindObjectAny(hname8);
  if (h1 != NULL) {
    h1->Draw();
    hname8 = "hMIXPathLen";
    h      = (TH1*) gROOT->FindObjectAny(hname8);
    h->Scale(sf);
    h->Draw("same");
    h->SetLineColor(7);
    hname8   = "hMCPathLen";
    TH1* hMC = (TH1*) gROOT->FindObjectAny(hname8);
    hMC->Draw("same");
    hMC->SetLineColor(2);

    hname8     = "hLdif";
    TH1* hLdif = (TH1*) gROOT->FindObjectAny(hname8);
    if (NULL != hLdif) hLdif->Delete();
    hLdif = (TH1*) h1->Clone();
    hLdif->Add(h1, h, 1., -1.);
    hLdif->SetName("hLdif");
    hLdif->SetTitle("hLdif");
    hLdif->Draw("same");
    hLdif->SetLineColor(3);
    //gPad->SetLogz();
  }
  else {
    cout << hname8 << " not found" << endl;
  }

  can->cd(9);
  gROOT->cd();
  TString hname9     = Form("hMMom");
  h1                 = (TH1*) gROOT->FindObjectAny(hname9);
  Double_t dNLamPrim = 0;
  if (h1 != NULL) {

    hname9    = "hMCLamMom";
    TH1* h    = (TH1*) gROOT->FindObjectAny(hname9);
    dNLamPrim = h->GetEntries();
    if (h->GetEntries() > 0) {
      h->SetMinimum(0.1);
      h->Draw();
      h1->Draw("same");
      h->SetLineColor(kRed);
    }
    else {
      h1->Draw();
    }
    hname9 = "hMIXMMom";
    h      = (TH1*) gROOT->FindObjectAny(hname9);
    h->Draw("same");
    h->Scale(sf);
    h->SetLineColor(7);

    hname9       = "hMomdif";
    TH1* hMomdif = (TH1*) gROOT->FindObjectAny(hname9);
    if (NULL != hMomdif) hMomdif->Delete();
    hMomdif = (TH1*) h1->Clone();
    hMomdif->Add(h1, h, 1., -1.);
    hMomdif->SetName("hMomdif");
    hMomdif->SetTitle("hMomdif");
    hMomdif->Draw("same");
    hMomdif->SetLineColor(3);

    gPad->SetLogy();
  }
  else {
    cout << hname9 << " not found" << endl;
  }

  can->cd(10);
  TString hname10 = Form("hMinvdif");
  h1              = (TH1*) gROOT->FindObjectAny(hname10);
  if (h1 != NULL) {
    TF1* fSignal = (TF1*) gROOT->FindObjectAny("fSignal");
    if (NULL != fSignal) fSignal->Delete();
    fSignal = new TF1("fSignal", "gaus", 1.090, 1.140);
    //h1->Fit("gaus","","H",1.1,1.2);
    Double_t MMin = 1.09;
    Double_t MMax = 1.142;
    Double_t param[3];
    param[0] = h1->GetMaximum();
    param[1] = 1.1156;
    param[2] = 0.01;
    fSignal->SetParameters(param);
    h1->Fit("fSignal", "", "", MMin, MMax);
    gPad->SetGrid();
    fSignal->GetParameters(param);
    Double_t MMEAN   = param[1];
    Double_t MSIG    = param[2];
    Double_t dFRange = 2.5;
    MMin             = MMEAN - dFRange * MSIG;
    MMax             = MMEAN + dFRange * MSIG;
    cout << " Fit results: " << MMEAN << ", " << MSIG << ", new range: " << MMin << " - " << MMax << endl;
    h1->Fit("fSignal", "", "", MMin, MMax);
    fSignal->GetParameters(param);
    MMEAN = param[1];
    MSIG  = param[2];
    cout << " Fit results: " << MMEAN << ", " << MSIG << endl;
    Double_t dSignal = h1->Integral(h1->FindBin(MMin), h1->FindBin(MMax));
    Double_t dAll    = hMinvall->Integral(hMinvall->FindBin(MMin), hMinvall->FindBin(MMax));
    Double_t dBckgd  = dAll - dSignal;
    Double_t SoB     = dSignal / dBckgd;
    Double_t Signif  = dSignal / TMath::Sqrt(dAll);
    cout << "Integral counts in Signal [" << MMin << "," << MMax << "] "
         << h1->Integral(h1->FindBin(MMin), h1->FindBin(MMax)) << endl;
    cout << "Integral counts in Signal + Bckgrd[] "
         << hMinvall->Integral(hMinvall->FindBin(MMin), hMinvall->FindBin(MMax)) << endl;
    cout << "Signal over background: " << SoB << endl;
    cout << "Significance: " << Signif << endl;
    cout << "Acc * eff = " << dSignal / dNLamPrim << endl;
    cout << "Total number of events " << mul_b_gen->GetEntries() << endl;
  }

  can->cd(11);
  TString hname11 = Form("hLdif");
  h1              = (TH1*) gROOT->FindObjectAny(hname11);
  if (h1 != NULL) {
    h1->Draw();
    //   h1->Fit("gaus","","H",1.1,1.2);
  }

  can->cd(12);
  TString hname12 = Form("hMomdif");
  h1              = (TH1*) gROOT->FindObjectAny(hname12);
  if (h1 != NULL) {
    h1->Draw();
    //h1->Fit("gaus","","H",1.1,1.2);
  }
  // gStyle->SetOptStat(0);
  //can->Update();


  can->SaveAs(Form("pl_lambda.pdf"));
}
