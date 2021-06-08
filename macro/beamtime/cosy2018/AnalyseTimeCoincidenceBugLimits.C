/* Copyright (C) 2018 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

void save_canvas_to_file(TFile* outf, TCanvas* canv, TString runCase)
{
  outf->cd();
  gDirectory->mkdir(runCase);
  gDirectory->cd(runCase);
  canv->Write();
  gDirectory->cd("..");
}

void save_canvas_to_pdf(TCanvas* canv, TString runCase)
{
  canv->SaveAs(TString("canvasesLimits/") + runCase + TString(canv->GetName()) + TString(".pdf"));
}

UInt_t uScanCaseOffset        = 7;
const UInt_t kuMaxNbFiredChan = 11 + 1;
const UInt_t kuMaxNbTotChan   = 12 + 1;
Double_t dDummyStart          = 1e5;
Double_t dDummyStop           = -1e5;
Double_t dNoiseBlockStart[kuMaxNbFiredChan][kuMaxNbTotChan];
Double_t dNoiseBlockStop[kuMaxNbFiredChan][kuMaxNbTotChan];
Double_t dMainBlockStart[kuMaxNbFiredChan][kuMaxNbTotChan];
Double_t dMainBlockStop[kuMaxNbFiredChan][kuMaxNbTotChan];

Bool_t analyseTimeCoincidenceBugCase(const UInt_t kuNbFiles, const UInt_t kuNbStats, TString* sFilename,
                                     UInt_t* uNbChanFixed, UInt_t* uNbChanScan, TString* sStats, TString runCase,
                                     TFile* outf)
{
  TFile* pFile[kuNbFiles];

  /// Scaling function, the constant should lead to a ratio of ~1 for the 1st bins
  Double_t hitsExpected = 1 / 500.0;  // per 1/2s bin

  /// Declaration of plotting variable
  TH2D* phNbFiredChanPerMs[kuNbFiles];

  /// Delay scan parameters
  Double_t dDistMin          = -1000.0;
  Double_t dDistMax          = 1000.0;
  Double_t dDistStep         = 4.0;
  UInt_t uNbBinsDist         = (dDistMax - dDistMin) / dDistStep;
  Double_t dDistStepDuration = 2.0;                         /// second
  Double_t dDistStepPulses   = dDistStepDuration * 1000.0;  /// 1 kHz pulser


  /// canvas division
  Int_t canvCols = 3;
  Int_t canvRows = 2;

  /// Loop on scan cases
  TH2* tempTwoDimHist = NULL;
  for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
    pFile[uFile] = TFile::Open(sFilename[uFile]);
    gROOT->cd();

    UInt_t uNbChanTot = uNbChanFixed[uFile] + uNbChanScan[uFile];

    TH1D* phNbFiredChanProj[uNbChanTot - uScanCaseOffset + 1];
    TH1D* phNbFiredChanScan[uNbChanTot - uScanCaseOffset + 1];
    TCanvas* cNbFiredProj = new TCanvas(Form("cNbFiredProj_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                        "Numbers of fired Channels projections for each number of channels");
    cNbFiredProj->Divide(canvCols, canvRows);

    TCanvas* cNbFiredProjStack = new TCanvas(Form("cNbFiredProjStack_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                             "Numbers of fired Channels projections for each number of channels");

    THStack* stackFiredProj = new THStack("stackFiredProj", "Stack of the projections");

    /// Recover the NbFiredChan and NbMultiChan 2D plots
    tempTwoDimHist = (TH2*) (pFile[uFile]->FindObjectAny("hNbFiredChanPerMs00"));
    if (NULL != tempTwoDimHist) {
      phNbFiredChanPerMs[uFile] =
        (TH2D*) (tempTwoDimHist->Clone(Form("phNbFiredChanPerMs_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile])));
    }
    else {
      return kFALSE;
    }

    for (UInt_t uNbChan = uScanCaseOffset; uNbChan <= uNbChanTot; uNbChan++) {
      phNbFiredChanProj[uNbChan - uScanCaseOffset] = phNbFiredChanPerMs[uFile]->ProjectionX(
        Form("hNbFiredChanProj_%u_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan), uNbChan, uNbChan);
      phNbFiredChanScan[uNbChan - uScanCaseOffset] =
        new TH1D(Form("hNbFiredChanScan_%u_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan),
                 Form("Number of pulses with %u fired channels instead of %u "
                      "for the %u-%u case",
                      uNbChan, uNbChanTot, uNbChanFixed[uFile], uNbChanScan[uFile]),
                 uNbBinsDist, dDistMin, dDistMax);

      /// Loop on the time in run bins (1/2s stepping) with check on the corresponding delay stepping
      for (UInt_t uBin = 1;
           uBin <= phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetNbinsX()
           && dDistMin
                  + dDistStep * phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetBinCenter(uBin) / dDistStepDuration
                <= dDistMax;
           ++uBin) {
        /// Delay corresponding to the bin
        Double_t dDist =
          dDistMin + dDistStep * phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetBinCenter(uBin) / dDistStepDuration;

        phNbFiredChanScan[uNbChan - uScanCaseOffset]->Fill(
          dDist, phNbFiredChanProj[uNbChan - uScanCaseOffset]->GetBinContent(uBin) / dDistStepPulses);
      }  // loop on bins until end or out of coincidence window

      /// Loop on the time delay bins and check if we are starting to see the wrong number of channels
      if (uNbChan < uNbChanTot) {
        std::vector<Double_t> dDelayStartNoise;
        std::vector<Double_t> dDelayStopNoise;
        std::vector<Double_t> dDelayStartMain;
        std::vector<Double_t> dDelayStopMain;
        Bool_t bStartFlagNoise            = kFALSE;
        Bool_t bStartFlagMain             = kFALSE;
        Bool_t bStopFlagNoise             = kTRUE;
        Bool_t bStopFlagMain              = kTRUE;
        Double_t dCountRatioPreviousDelay = 0.0;
        for (UInt_t uBin = 1; uBin <= phNbFiredChanScan[uNbChan - uScanCaseOffset]->GetNbinsX(); ++uBin) {
          /// Delay corresponding to the bin
          Double_t dDist = phNbFiredChanScan[uNbChan - uScanCaseOffset]->GetBinCenter(uBin);

          /// Fraction of pulses having this number of fired channels
          Double_t dCountRatio = phNbFiredChanScan[uNbChan - uScanCaseOffset]->GetBinContent(uBin);

          if (1e-3 < dCountRatio && dCountRatioPreviousDelay <= 1e-3 && bStopFlagNoise) {
            bStartFlagNoise = kTRUE;
            bStopFlagNoise  = kFALSE;
            dDelayStartNoise.push_back(dDist);
          }  // if( 1e-3 < dCountRatio && dCountRatioPreviousDelay <= 1e-3 )
          else if (1e-3 > dCountRatio && dCountRatioPreviousDelay >= 1e-3 && bStartFlagNoise) {
            bStartFlagNoise = kFALSE;
            bStopFlagNoise  = kTRUE;
            dDelayStopNoise.push_back(dDist);
          }  // if( 1e-3 > dCountRatio && dCountRatioPreviousDelay >= 1e-3 )

          if (1e-2 < dCountRatio && dCountRatioPreviousDelay <= 1e-2 && bStopFlagMain) {
            bStartFlagMain = kTRUE;
            bStopFlagMain  = kFALSE;
            dDelayStartMain.push_back(dDist);
          }  // if( 1e-2 < dCountRatio && dCountRatioPreviousDelay <= 1e-2 )
          else if (1e-2 > dCountRatio && dCountRatioPreviousDelay >= 1e-2 && bStartFlagMain) {
            bStartFlagMain = kFALSE;
            bStopFlagMain  = kTRUE;
            dDelayStopMain.push_back(dDist);
          }  // if( 1e-2 > dCountRatio && dCountRatioPreviousDelay >= 1e-2 )
             /*
               if( 0.0 == dCountRatio )
               {
                  bStartFlagNoise = kFALSE;
                  bStartFlagMain  = kFALSE;
                  bStopFlagNoise  = kTRUE;
                  bStopFlagMain   = kTRUE;
               } // if( 0.0 == dCountRatio )
*/
          dCountRatioPreviousDelay = dCountRatio;
        }  // loop on bins until end

        if (dDelayStartNoise.size() != dDelayStopNoise.size()) {
          std::cout << Form("Case %2u-%2u fired %2u ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan)
                    << " Unmatched Start-Stop for noise! " << dDelayStartNoise.size() << " " << dDelayStopNoise.size()
                    << std::endl;
        }  // if( dDelayStartNoise.size() != dDelayStopNoise.size() )

        std::cout << Form("Case %2u-%2u fired %2u noise blocks: ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan);
        for (UInt_t uNoiseBlock = 0; uNoiseBlock < dDelayStartNoise.size() && uNoiseBlock < dDelayStopNoise.size();
             ++uNoiseBlock) {
          std::cout << Form("%4.0f to % 4.0f, ", dDelayStartNoise[uNoiseBlock], dDelayStopNoise[uNoiseBlock]);

          if (dDelayStartNoise[uNoiseBlock] < dNoiseBlockStart[uNbChan][uNbChanTot])
            dNoiseBlockStart[uNbChan][uNbChanTot] = dDelayStartNoise[uNoiseBlock];
          if (dNoiseBlockStop[uNbChan][uNbChanTot] < dDelayStopNoise[uNoiseBlock])
            dNoiseBlockStop[uNbChan][uNbChanTot] = dDelayStopNoise[uNoiseBlock];
        }  // Loop on noise blocks
        std::cout << std::endl;

        if (dDelayStartMain.size() != dDelayStopMain.size()) {
          std::cout << Form("Case %2u-%2u fired %2u ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan)
                    << " Unmatched Start-Stop for main! " << dDelayStartMain.size() << " " << dDelayStopMain.size()
                    << std::endl;
        }  // if( dDelayStartMain.size() != dDelayStopMain.size() )

        std::cout << Form("Case %2u-%2u fired %2u main blocks:  ", uNbChanFixed[uFile], uNbChanScan[uFile], uNbChan);
        for (UInt_t uMainBlock = 0; uMainBlock < dDelayStartMain.size() && uMainBlock < dDelayStopMain.size();
             ++uMainBlock) {
          std::cout << Form("%4.0f to % 4.0f, ", dDelayStartMain[uMainBlock], dDelayStopMain[uMainBlock]);

          if (dDelayStartMain[uMainBlock] < dMainBlockStart[uNbChan][uNbChanTot])
            dMainBlockStart[uNbChan][uNbChanTot] = dDelayStartMain[uMainBlock];
          if (dMainBlockStop[uNbChan][uNbChanTot] < dDelayStopMain[uMainBlock])
            dMainBlockStop[uNbChan][uNbChanTot] = dDelayStopMain[uMainBlock];
        }  // Loop on noise blocks
        std::cout << std::endl;

      }  // if( uNbChan < uNbChanTot )

      cNbFiredProj->cd(uNbChan - 7 + 1);
      gPad->SetGridx();
      gPad->SetGridy();
      //         phNbFiredChanProj[uNbChan - uScanCaseOffset]->Draw();
      phNbFiredChanScan[uNbChan - uScanCaseOffset]->Draw("hist");

      //         stackFiredProj->Add( phNbFiredChanProj[uNbChan - uScanCaseOffset] );
      stackFiredProj->Add(phNbFiredChanScan[uNbChan - uScanCaseOffset]);
    }  // Loop on missing channels cases

    cNbFiredProjStack->cd();
    gPad->SetGridx();
    gPad->SetGridy();
    stackFiredProj->Draw("nostack,hist");

    save_canvas_to_file(outf, cNbFiredProj, runCase);
    save_canvas_to_file(outf, cNbFiredProjStack, runCase);
    delete cNbFiredProj;
    delete cNbFiredProjStack;

    for (UInt_t uNbChan = uScanCaseOffset; uNbChan <= uNbChanTot; uNbChan++) {
      delete phNbFiredChanProj[uNbChan - uScanCaseOffset];
      delete phNbFiredChanScan[uNbChan - uScanCaseOffset];
    }  // for (UInt_t uNbChan = uScanCaseOffset; uNbChan <= uNbChanTot; uNbChan++)
  }    // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++ )

  return kTRUE;
}


// 6 + n
Bool_t AnalyseTimeCoincidenceBugSixN(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 6;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0031_20180803_0824.root", "data/PulserHistos_r0032_20180803_0841.root",
    "data/PulserHistos_r0033_20180803_0858.root", "data/PulserHistos_r0034_20180803_0915.root",
    "data/PulserHistos_r0035_20180803_0932.root", "data/PulserHistos_r0036_20180803_0949.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {6, 6, 6, 6, 6, 6};
  UInt_t uNbChanScan[kuNbFiles]  = {1, 2, 3, 4, 5, 6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "6n";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// 7
Bool_t AnalyseTimeCoincidenceBugSeven(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 6;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0031_20180803_0824.root", "data/PulserHistos_r0037_20180803_1006.root",
    "data/PulserHistos_r0038_20180803_1023.root", "data/PulserHistos_r0039_20180803_1040.root",
    "data/PulserHistos_r0040_20180803_1057.root", "data/PulserHistos_r0041_20180803_1114.root",
  };
  UInt_t uNbChanFixed[kuNbFiles] = {6, 5, 4, 3, 2, 1};
  UInt_t uNbChanScan[kuNbFiles]  = {1, 2, 3, 4, 5, 6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "7tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// 8
Bool_t AnalyseTimeCoincidenceBugEight(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 5;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0032_20180803_0841.root", "data/PulserHistos_r0042_20180803_1131.root",
    "data/PulserHistos_r0043_20180803_1148.root", "data/PulserHistos_r0044_20180803_1205.root",
    "data/PulserHistos_r0045_20180803_1222.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {6, 5, 4, 3, 2};
  UInt_t uNbChanScan[kuNbFiles]  = {2, 3, 4, 5, 6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "8tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// 9
Bool_t AnalyseTimeCoincidenceBugNine(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 4;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0033_20180803_0858.root", "data/PulserHistos_r0046_20180803_1239.root",
    "data/PulserHistos_r0047_20180803_1256.root", "data/PulserHistos_r0048_20180803_1313.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {6, 5, 4, 3};
  UInt_t uNbChanScan[kuNbFiles]  = {3, 4, 5, 6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "9tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}


// 10
Bool_t AnalyseTimeCoincidenceBugTen(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles         = 3;
  const UInt_t kuNbStats         = 3;
  TString sFilename[kuNbFiles]   = {"data/PulserHistos_r0034_20180803_0915.root",
                                  "data/PulserHistos_r0049_20180803_1330.root",
                                  "data/PulserHistos_r0050_20180803_1346.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {6, 5, 4};
  UInt_t uNbChanScan[kuNbFiles]  = {4, 5, 6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "10tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// 11
Bool_t AnalyseTimeCoincidenceBugEleven(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles         = 2;
  const UInt_t kuNbStats         = 3;
  TString sFilename[kuNbFiles]   = {"data/PulserHistos_r0035_20180803_0932.root",
                                  "data/PulserHistos_r0051_20180803_1404.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {6, 5};
  UInt_t uNbChanScan[kuNbFiles]  = {5, 6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "11tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// 12
Bool_t AnalyseTimeCoincidenceBugTwelve(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles         = 1;
  const UInt_t kuNbStats         = 3;
  TString sFilename[kuNbFiles]   = {"data/PulserHistos_r0036_20180803_0949.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {6};
  UInt_t uNbChanScan[kuNbFiles]  = {6};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "12tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

Bool_t AnalyseTimeCoincidenceBugLimits(TString sFilename = "STSXyterPulserLimits.root")
{
  /// Open output root file
  TDirectory* oldDir = gDirectory;
  TFile* histoFile   = new TFile(sFilename, "RECREATE");
  if (NULL == histoFile) return kFALSE;

  /// Prepare the arrays with the limits
  for (UInt_t uNbFiredChan = 0; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan) {
    for (UInt_t uNbTotChan = 0; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan) {
      dNoiseBlockStart[uNbFiredChan][uNbTotChan] = dDummyStart;
      dNoiseBlockStop[uNbFiredChan][uNbTotChan]  = dDummyStop;
      dMainBlockStart[uNbFiredChan][uNbTotChan]  = dDummyStart;
      dMainBlockStop[uNbFiredChan][uNbTotChan]   = dDummyStop;
    }  // for( UInt_t uNbTotChan = 0; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
  }    // for( UInt_t uNbFiredChan = 0; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )

  /// Run all cases
  AnalyseTimeCoincidenceBugSixN(histoFile);
  AnalyseTimeCoincidenceBugSeven(histoFile);
  AnalyseTimeCoincidenceBugEight(histoFile);
  AnalyseTimeCoincidenceBugNine(histoFile);
  AnalyseTimeCoincidenceBugTen(histoFile);
  AnalyseTimeCoincidenceBugEleven(histoFile);
  AnalyseTimeCoincidenceBugTwelve(histoFile);

  /// Printout limits for noise blocks
  std::cout << "Noise blocks limits" << std::endl;
  std::cout << "          ";
  for (UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan)
    std::cout << Form("%2u channels ", uNbTotChan);
  std::cout << std::endl;
  for (UInt_t uNbFiredChan = uScanCaseOffset; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan) {
    std::cout << Form("%2u fired: ", uNbFiredChan);
    for (UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan) {
      if (dNoiseBlockStart[uNbFiredChan][uNbTotChan] < dDummyStart
          && dDummyStop < dNoiseBlockStop[uNbFiredChan][uNbTotChan])
        std::cout << Form("%4.0f - %4.0f ", dNoiseBlockStart[uNbFiredChan][uNbTotChan],
                          dNoiseBlockStop[uNbFiredChan][uNbTotChan]);
      else
        std::cout << "----------- ";
    }  // for( UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
    std::cout << std::endl;
  }  // for( UInt_t uNbFiredChan = uScanCaseOffset; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )

  /// Printout limits for Main blocks
  std::cout << "Main blocks limits" << std::endl;
  std::cout << "          ";
  for (UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan)
    std::cout << Form("%2u channels ", uNbTotChan);
  std::cout << std::endl;
  for (UInt_t uNbFiredChan = uScanCaseOffset; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan) {
    std::cout << Form("%2u fired: ", uNbFiredChan);
    for (UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan) {
      if (dMainBlockStart[uNbFiredChan][uNbTotChan] < dDummyStart
          && dDummyStop < dMainBlockStop[uNbFiredChan][uNbTotChan])
        std::cout << Form("%4.0f - %4.0f ", dMainBlockStart[uNbFiredChan][uNbTotChan],
                          dMainBlockStop[uNbFiredChan][uNbTotChan]);
      else
        std::cout << "----------- ";
    }  // for( UInt_t uNbTotChan = uScanCaseOffset + 1; uNbTotChan < kuMaxNbTotChan; ++uNbTotChan )
    std::cout << std::endl;
  }  // for( UInt_t uNbFiredChan = uScanCaseOffset; uNbFiredChan < kuMaxNbFiredChan; ++uNbFiredChan )

  /// Close the file
  histoFile->Close();
  oldDir->cd();

  return kTRUE;
}
