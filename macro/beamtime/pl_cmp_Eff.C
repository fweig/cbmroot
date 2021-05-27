/** @file pl_cmp_Eff.C
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

void pl_cmp_Eff(Int_t iDut = 900, Int_t iMode = 4, Double_t dEffMin = 0., Double_t dEffMax = 1.05,
                TString AnaOpt = "DT50_Req0_910911500_921_911921_600_0.5_5.0_trk111_Calr0111_"
                                 "20161210_0646_DT50_Req1"
                //TString AnaOpt="DT50_Req0_910911500_921_911921_600_0.5_7.0_trk100_Calr0111_20161210_0646_DT50_Req1"
                //TString AnaOpt="DT50_Req0_910911500_921_911921_600_0.5_4.0_trk100_Calr0111_20161210_0646_DT50_Req1"
                //TString AnaOpt="DT50_Req0_910911500_921_911921_601_0.5_5.0_trk111_Calr0096_20161209_2047_DT50_Req1"
)
{
  // input files ...
  // const Int_t nF=6;
  // Int_t iRun[nF]={96,111,128,148,150,158};
  const Int_t nF = 6;
  Int_t iRun[nF] = {600};

  const Int_t nPar1   = 3;
  TString cPar[nPar1] = {"", "_900", "_901"};
  const Int_t nPar2   = 2;
  Int_t iPar[nPar2]   = {900, 901};
  TString hFileForm   = "hst/"
                      "600.100.-1.0%s_050_030040500_500_%03d041_031_0.9_2.5_"
                      "trk004_Cal600.100.5.0_Ana.hst.root";
  //TString hFileForm="hst/600.100.-1.0%s_050_030040500_500_%03d041_031_0.9_2.5_trk005_Cal600.100.5.0_Ana.hst.root";
  //TString hFileForm="hst/600.100.-1.0%s_050_030040500_500_%03d041_031_0.9_2.5_trk006_Cal600.100.5.0_Ana.hst.root";

  //plot initialisation
  TCanvas* can = new TCanvas("can", "can", 50, 50, 500, 600);
  Int_t nx     = 1;
  Int_t ny     = 2;
  can->Divide(nx, ny);  //,0,0);

  gPad->SetFillColor(0);
  gPad->SetLeftMargin(3.);
  gPad->SetRightMargin(3.);
  gPad->SetTopMargin(2.);
  gPad->SetBottomMargin(3.);

  gStyle->SetPalette(1);
  gStyle->SetOptStat(kTRUE);
  gStyle->SetOptStat(" ");
  gStyle->SetOptStat(10);
  gStyle->SetOptStat(kFALSE);
  gStyle->SetTitleSize(0.08, "x");  // axis labels
  gStyle->SetTitleSize(0.08, "y");

  cout << "gStyle label size: " << gStyle->GetLabelSize() << endl;

  // file opening

  TFile* f[nF];
  TString hFname[nF];
  TString cLegTxt[nF];
  TString cFname;
  TString cRun = "";
  TString cCmd = "";
  ifstream in;
  TString inFile;
  Int_t iFDut[nF];

  Int_t FMode = 1;
  switch (FMode) {
    case 0:
      for (Int_t i = 0; i < nF; i++) {
        cRun = Form("r%04d*", iRun[i]);
        if (AnaOpt.Contains("Cal")) {
          cout << "AnaOpt contains explicit calibration " << endl;
          cCmd = "ls -1 ./hst/" + cRun + AnaOpt + "_Ana.hst.root > Tmp.out";
        }
        else {
          cout << "AnaOpt does not contains explicit calibration, assume "
                  "native one "
               << endl;
          cCmd = "ls -1 ./hst/" + cRun + AnaOpt + "Cal" + cRun + "_Ana.hst.root > Tmp.out";
        }
        //cout << "execute " << cCmd << endl;
        gSystem->Exec(cCmd);
        // read cCmd result into file name variable
        //if(NULL != in) in.close();
        in.open(Form("%sTmp.out", "./"));
        in >> inFile;
        cout << " inFile = " << inFile << endl;
        if (inFile != "") {
          cFname = inFile;
          //cFname = "./hst/" + cRun + "_" + AnaOpt + "_Ana.hst.root";
          f[i] = new TFile(cFname.Data(), "Read");
          if (f[i] != NULL) cout << cFname.Data() << " opened at " << f[i] << endl;
        }
        else
          f[i] = NULL;
      }
    case 1:
      Int_t i = 0;
      for (Int_t iPar1 = 0; iPar1 < nPar1; iPar1++) {
        for (Int_t iPar2 = 0; iPar2 < nPar2; iPar2++) {
          hFname[i]  = Form(hFileForm.Data(), cPar[iPar1].Data(), iPar[iPar2]);
          cLegTxt[i] = Form("Pat %s, \t Dut %d", cPar[iPar1].Data(), iPar[iPar2]);
          f[i]       = new TFile(hFname[i].Data(), "Read");
          iFDut[i]   = iPar[iPar2];
          i++;
        }
      }
  }
  gROOT->cd();

  Int_t ih = 0;
  TString cMode;
  TString cTitle;
  switch (iMode) {
    case 0:
      cMode  = "DTLH";
      cTitle = Form("Time to last hit in DUT %d", iDut);
      break;
    case 1:
      cMode  = "Mul";
      cTitle = Form("Hit multiplicity in REF for DUT %d", iDut);
      break;
    case 2:
      cMode  = "TIS";
      cTitle = Form("Time in Spill for DUT %d", iDut);
      break;
    case 3:
      cMode  = "Chi";
      cTitle = Form("Chi2 of DUT %d", iDut);
      break;

    case 4:
      cMode  = "Vel";
      cTitle = Form("Velocity of DUT");  // %d",iDut);
      break;

    default:;
  }
  TString hname0 = Form("hDut%s_Found_%d", cMode.Data(), iDut);
  TString hname1 = Form("hDut%s_Missed_%d", cMode.Data(), iDut);

  TH1* hfound[nF];
  TH1* hmiss[nF];
  TH1* hall[nF];
  TH1* heff[nF];

  Int_t LCol[6] = {1, 2, 3, 4, 6, 7};
  Int_t LSty[6] = {1, 1, 1, 1, 1, 1};
  Int_t NSmooth = 2;

  can->cd(1);

  //TLegend *leg = new TLegend(0.2,0.6,0.4,0.9); //x1,y1,x2,y2,header
  TLegend* leg = new TLegend(0.6, 0.2, 0.8, 0.4);  //x1,y1,x2,y2,header

  leg->UseCurrentStyle();
  leg->SetTextSize(0.03);
  // leg->SetHeader("TOF setups");

  TEfficiency* pEffDut[nF];

  Double_t dYMax = 0.;
  for (Int_t iF = 0; iF < nF; iF++) {
    cout << " add histos from file " << iF << " with pointer " << f[iF] << endl;
    if (NULL == f[iF]) continue;
    f[iF]->Print();

    hname0 = Form("hDut%s_Found_%d", cMode.Data(), iFDut[iF]);
    hname1 = Form("hDut%s_Missed_%d", cMode.Data(), iFDut[iF]);
    cout << "Get histo " << hname0.Data() << endl;

    hfound[iF] = (TH1*) (f[iF]->Get(hname0))->Clone();
    hmiss[iF]  = (TH1*) (f[iF]->Get(hname1))->Clone();
    hall[iF]   = (TH1*) hfound[iF]->Clone();
    hall[iF]->Add(hmiss[iF], hfound[iF], 1., 1.);
    hfound[iF]->SetTitle(cTitle);
    hfound[iF]->SetLineColor(LCol[iF]);
    hfound[iF]->SetLineStyle(LSty[iF]);
    if (hfound[iF]->GetMaximum() > dYMax) dYMax = hfound[iF]->GetMaximum();
    cout << "dYMax = " << dYMax << endl;
    pEffDut[iF] = new TEfficiency(*hfound[iF], *hall[iF]);
    pEffDut[iF]->SetTitle(Form("Efficiency of DUT"));  // %d",iDut));
    pEffDut[iF]->UseCurrentStyle();
    pEffDut[iF]->SetLineColor(LCol[iF]);
    pEffDut[iF]->SetLineStyle(LSty[iF]);
    //leg->AddEntry(pEffDut[iF],Form("Run %d",iRun[iF]),"l");
    leg->AddEntry(pEffDut[iF], cLegTxt[iF].Data(), "l");
  }

  //plotting
  can->cd(1);
  for (Int_t iF = 0; iF < nF; iF++) {
    if (NULL == f[iF]) continue;
    if (iF == 0) {
      hfound[iF]->SetMaximum(dYMax * 1.2);
      hfound[iF]->Draw();
      //gPad->SetLogy();
    }
    else {
      cout << "Print ";
      hfound[iF]->Print();
      gPad->Update();
      hfound[iF]->Draw("same");
    }
  }

  can->cd(2);
  for (Int_t iF = 0; iF < nF; iF++) {
    if (NULL == f[iF]) continue;
    if (iF == 0) {
      pEffDut[iF]->Draw("AP");
      gPad->Update();
      auto graph = pEffDut[iF]->GetPaintedGraph();
      graph->SetMinimum(dEffMin);
      graph->SetMaximum(dEffMax);
      switch (iMode) {
        case 0: graph->GetXaxis()->SetRangeUser(0., 10.); break;
        default:;
      }
      gPad->Update();
      gPad->SetGridx();
      gPad->SetGridy();
    }
    else {
      cout << "draw " << pEffDut[iF]->GetTitle() << endl;
      pEffDut[iF]->Draw("same");
    }
  }
  leg->Draw();

  gROOT->LoadMacro("pl_Datime.C");
  //TString FADD=Form("pl_Datime(\"%s\")",AnaOpt.Data());
  TString FADD = Form("pl_Datime(\"%s\")", hFileForm.Data());

  gInterpreter->ProcessLine(FADD.Data());

  can->SaveAs(Form("pl_cmp_Eff_%s_%d.pdf", cMode.Data(), iDut));

  return;
}
