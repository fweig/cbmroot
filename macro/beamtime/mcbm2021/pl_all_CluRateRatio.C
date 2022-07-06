/* Copyright (C) 2020-2022 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void pl_all_CluRateRatio(Int_t iRef = 500, Int_t iNSt = 3, Double_t Tstart = 0., Double_t Tend = 800., Int_t iMode = 0,
                         Int_t iOpt = 0, Double_t Ymax = 5E3, Double_t THR = 1.E5)
{
  //  TCanvas *can = new TCanvas("can22","can22");
  //  can->Divide(2,2);
  //  TCanvas *can = new TCanvas("can","can",48,55,700,900);
  TCanvas* can = new TCanvas("can", "can", 48, 56, 900, 900);


  can->Divide(1, 2, 0.01, 0.01);

  Float_t lsize = 0.06;

  gPad->SetFillColor(0);
  gStyle->SetPalette(1);
  gStyle->SetLabelSize(lsize);

  //gStyle->SetOptStat(kTRUE);
  //gROOT->cd();
  //gROOT->SetDirLevel(2);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetTitleOffset(1.3, "y");
  gStyle->SetOptStat(0);

  TH1* h;
  TH1* hRef;
  TH1* hRat;
  TH1* hDis;
  TH2* h2;

  const Int_t iTSR[11]     = {500, 41, 31, 900, 901, 700, 701, 600, 601, 33, 43};                   // March 2022
  const Double_t dArea[11] = {1., 864., 864., 864., 864., 1664., 1664., 172.8, 172.8, 864., 864.};  // March 2022

  //const Int_t iTSR[11] = {500, 2, 12, 1, 11, 3, 11, 0, 10, 4, 14};  // July 2021
  //const Int_t iTSR[11] = {500, 41, 31, 900, 901, 910, 911, 600, 601, 33, 43};  // May 2021
  //const Int_t iPlot[11]    = {  1,    1,    1,    1,    1,    1,    1,    1,   1,   1,  1};
  const Int_t iPlot[11] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  //const Double_t dArea[11] = {1., 864., 864., 864., 756., 852., 852., 172.8, 172.8, 864., 864.};  // May
  //const Double_t dArea[11] = {1., 432., 432., 432., 432., 426., 426., 172.8, 172.8, 432., 432.}; // Jul
  const Double_t dDist[11] = {1., 353., 532.5, 386., 416., 416., 445., 478., 485., 353., 543.};

  Int_t iCanv = 0;
  // if (h!=NULL) h->Delete();
  TString hname;
  gROOT->LoadMacro("shift_hst.C");

  can->cd(1);

  Int_t iRp     = iRef % 10;
  Int_t iSmType = (iRef - iRp) / 10;
  Int_t iSm     = iSmType % 10;
  iSmType       = (iSmType - iSm) / 10;
  Int_t IndRef  = 0;
  for (IndRef = 0; IndRef < 11; IndRef++)
    if (iTSR[IndRef] == iRef) break;
  cout << "Reference counter " << iRef << " found at index " << IndRef << endl;

  gROOT->cd();
  switch (iMode) {
    case 0: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iSmType, iSm, iRp); break;
    case 1: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate10s", iSmType, iSm, iRp); break;
    case 2: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_digirate", iSmType, iSm, iRp); break;
  }
  h = (TH1*) gROOT->FindObjectAny(hname);
  if (h != NULL) {
    hRef = (TH1*) h->Clone();
    switch (iOpt) {
      case 10:
      case 0:  //rate
        hRef->Add(h, hRef, 0., 1.);
        //	hRef->SetMaximum(1.E5);
        gPad->SetLogy();
        break;
      case 1:  //rate/area
      {
        hRef->Add(h, hRef, 0., 1. / dArea[IndRef]);
        hRef->SetMaximum(Ymax);
        hRef->GetYaxis()->SetTitle("Flux (Hz/cm^{2})");
        //ScaleXaxis(hRef,ScaleX); //shift to align times
        TH1* hRefFlux = (TH1*) hRef;  //->Clone();
        cout << "New RefFlux histo: " << hRefFlux->GetName() << endl;
        hRefFlux->SetName("hRefFlux");
      } break;
      case 2:  //flux=rate/area*dist**2
        hRef->Add(h, hRef, 0., dDist[IndRef] * dDist[IndRef] / dArea[IndRef]);
        break;
    }
    hRef->GetXaxis()->SetRangeUser(Tstart, Tend);
    hRef->Draw("histE");
    hRef->Sumw2();
    Double_t RateMax = hRef->GetMaximum();
    TH1F* hRefRate =
      new TH1F("hRefRate", Form("Rate distribution; %s", hRef->GetYaxis()->GetTitle()), 100, 0., RateMax);
    for (Int_t iBin = hRef->FindBin(Tstart); iBin < hRef->FindBin(Tend); iBin++)
      hRefRate->Fill(hRef->GetBinContent(iBin));
    //hRefRate->Sumw2();
    Double_t RateAv  = hRefRate->GetMean(1);
    Double_t RateRMS = hRefRate->GetStdDev(1);
    cout << "Reference counter average rate " << RateAv << ", RMS " << RateRMS << endl;
    gStyle->SetOptStat(1111);
    TCanvas* can2 = new TCanvas("can2", "can2", 1400, 56, 500, 500);
    can2->cd(1);
    hRefRate->Draw("histE");
    hRefRate->UseCurrentStyle();
    gPad->Update();
  }

  can->cd(1);
  Int_t iCol = 0;
  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    iCol++;
    if (iCol == 5 || iCol == 10) iCol++;  // skip yellow
    if (iPlot[iSt] == 0) continue;

    iRp     = iTSR[iSt] % 10;
    iSmType = (iTSR[iSt] - iRp) / 10;
    iSm     = iSmType % 10;
    iSmType = (iSmType - iSm) / 10;

    gROOT->cd();
    switch (iMode) {
      case 0: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iSmType, iSm, iRp); break;
      case 1: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate10s", iSmType, iSm, iRp); break;
      case 2: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_digirate", iSmType, iSm, iRp); break;
    }
    h = (TH1*) gROOT->FindObjectAny(hname);
    if (h != NULL) {
      hDis = (TH1*) h->Clone();
      hDis->SetName(Form("hDis_%d", iTSR[iSt]));
      switch (iOpt) {
        case 10:
        case 0:  //rate
          hDis->Add(h, hDis, 0., 1.);
          break;
        case 1:  //rate/area
        {
          cout << "Scale station " << iSt << " by " << 1. / dArea[iSt] << endl;
          hDis->Add(h, hDis, 0., 1. / dArea[iSt]);
          hDis->GetYaxis()->SetTitle("Flux (Hz/cm^{2})");
          //ScaleXaxis(hDis,ScaleX); // shift in x - direction, need shift_hst.C to be loaded
          TH1* hFlux = (TH1*) hDis->Clone();
          hFlux->SetName(Form("hFlux_%d", iTSR[iSt]));
        } break;
        case 2:  //flux=rate/area*dist**2
          hDis->Add(h, hDis, 0., dDist[iSt] * dDist[iSt] / dArea[iSt]);
          break;
      }

      hDis->Draw("samehistE");
      hDis->SetLineColor(iCol);
      //h->UseCurrentStyle();
      //gPad->SetLogy();
    }
    else {
      cout << "Histogram " << hname << " not existing. " << endl;
    }
  }

  // determine and plot ratios
  can->cd(2);
  iCol            = 0;
  Bool_t bIniPlot = kFALSE;
  TLegend* leg    = new TLegend(0.25, 0.7, 0.35, 0.95);
  leg->SetTextSize(0.03);
  for (Int_t iSt = 0; iSt < iNSt; iSt++) {
    iCol++;
    if (iCol == 5 || iCol == 10) iCol++;  // skip yellow
    if (iPlot[iSt] == 0) continue;
    iRp     = iTSR[iSt] % 10;
    iSmType = (iTSR[iSt] - iRp) / 10;
    iSm     = iSmType % 10;
    iSmType = (iSmType - iSm) / 10;

    gROOT->cd();
    switch (iMode) {
      case 0: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate", iSmType, iSm, iRp); break;
      case 1: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_rate10s", iSmType, iSm, iRp); break;
      case 2: hname = Form("cl_SmT%01d_sm%03d_rpc%03d_digirate", iSmType, iSm, iRp); break;
    }
    h = (TH1*) gROOT->FindObjectAny(hname);
    if (h != NULL) {
      hRat = (TH1*) h->Clone();
      h->Sumw2();
      hRat->SetName(Form("hRat_%d", iTSR[iSt]));
      hRat->SetTitle("ratio");
      hRat->GetYaxis()->SetTitle("ratio");
      leg->AddEntry(hRat, Form("%d", iTSR[iSt]), "l");
      switch (iOpt) {
        case 0:  //rate
          hRat->Divide(h, hRef, 1., 1., "B");
          break;
        case 1:  //rate/area
          h    = (TH1*) gROOT->FindObjectAny(Form("hFlux_%d", iTSR[iSt]));
          hRef = (TH1*) gROOT->FindObjectAny(Form("hRefFlux"));
          hRat->Divide(h, hRef, 1., 1., "B");
          //hRat->Divide(h, hRef, 1. / dArea[iSt], 1. / dArea[IndRef], "B");
          break;
        case 2:  //flux=rate/area*dist**2
          hRat->Divide(h, hRef, dDist[iSt] * dDist[iSt] / dArea[iSt], dDist[IndRef] * dDist[IndRef] / dArea[IndRef],
                       "B");
          break;
        case 10: {
          Double_t dVal = 0.;
          Double_t dErr = 0.;
          for (Int_t iBin = 0; iBin < h->GetNbinsX(); iBin++) {
            if (iBin < 100)
              cout << "h " << h->GetName() << " bin " << iBin << ", cts " << hRef->GetBinContent(iBin + 1) << ", val "
                   << dVal << endl;
            if (hRef->GetBinContent(iBin + 1) > THR) {
              dVal = h->GetBinContent(iBin + 1) / hRef->GetBinContent(iBin + 1);
              dErr = TMath::Sqrt(TMath::Power(h->GetBinContent(iBin + 1), -0.5)
                                 + TMath::Power(hRef->GetBinContent(iBin + 1), -0.5))
                     * dVal;
            }
            else {
              dErr = 0.;
            }
            hRat->SetBinContent(iBin + 1, dVal);
            hRat->SetBinError(iBin + 1, dErr);
          }
        } break;
      }
      if (!bIniPlot) {
        bIniPlot = kTRUE;
        hRat->SetMinimum(1.E-2);
        hRat->SetMaximum(2.);
        hRat->Draw("L E");
        hRat->GetXaxis()->SetRangeUser(Tstart, Tend);
      }
      else
        hRat->Draw("L E SAME");

      hRat->SetLineColor(iCol);
      //h->UseCurrentStyle();
      //gPad->SetLogy();
    }
    else {
      cout << "Histogram " << hname << " not existing. " << endl;
    }
    leg->Draw();
  }
  can->SaveAs(Form("pl_all_CluRateRatio.pdf"));
}
