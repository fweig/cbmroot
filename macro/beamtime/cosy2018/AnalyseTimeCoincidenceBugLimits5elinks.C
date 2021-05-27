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
  canv->SaveAs(TString("canvasesLimits5elinks/") + runCase + TString(canv->GetName()) + TString(".pdf"));
}

UInt_t uScanCaseOffset        = 16;
const UInt_t kuMaxNbFiredChan = 29 + 1;
const UInt_t kuMaxNbTotChan   = 30 + 1;
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
  Int_t canvCols = 5;
  Int_t canvRows = 3;

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

      cNbFiredProj->cd(uNbChan - uScanCaseOffset + 1);
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

// n + m = 16 cases
Bool_t AnalyseTimeCoincidenceBug16(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 15;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0088_20181010_1230.root", "data/PulserHistos_r0089_20181010_1247.root",
    "data/PulserHistos_r0090_20181010_1304.root", "data/PulserHistos_r0091_20181010_1321.root",
    "data/PulserHistos_r0092_20181010_1338.root", "data/PulserHistos_r0093_20181010_1355.root",
    "data/PulserHistos_r0094_20181010_1412.root", "data/PulserHistos_r0095_20181010_1429.root",
    "data/PulserHistos_r0096_20181010_1446.root", "data/PulserHistos_r0097_20181010_1503.root",
    "data/PulserHistos_r0098_20181010_1520.root", "data/PulserHistos_r0099_20181010_1616.root",
    "data/PulserHistos_r0100_20181010_1633.root", "data/PulserHistos_r0101_20181010_1650.root",
    "data/PulserHistos_r0102_20181010_1707.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  UInt_t uNbChanScan[kuNbFiles]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "16tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 17 cases
Bool_t AnalyseTimeCoincidenceBug17(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 14;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0103_20181010_1724.root", "data/PulserHistos_r0104_20181010_1741.root",
    "data/PulserHistos_r0105_20181010_1758.root", "data/PulserHistos_r0106_20181010_1815.root",
    "data/PulserHistos_r0107_20181010_1832.root", "data/PulserHistos_r0108_20181010_1849.root",
    "data/PulserHistos_r0109_20181010_1906.root", "data/PulserHistos_r0110_20181010_1923.root",
    "data/PulserHistos_r0111_20181010_1940.root", "data/PulserHistos_r0112_20181010_1957.root",
    "data/PulserHistos_r0113_20181010_2014.root", "data/PulserHistos_r0114_20181010_2031.root",
    "data/PulserHistos_r0115_20181010_2048.root", "data/PulserHistos_r0116_20181011_0842.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
  UInt_t uNbChanScan[kuNbFiles]  = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "17tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 18 cases
Bool_t AnalyseTimeCoincidenceBug18(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 13;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0117_20181011_0859.root", "data/PulserHistos_r0118_20181011_0916.root",
    "data/PulserHistos_r0119_20181011_0933.root", "data/PulserHistos_r0120_20181011_0950.root",
    "data/PulserHistos_r0121_20181011_1007.root", "data/PulserHistos_r0122_20181011_1024.root",
    "data/PulserHistos_r0123_20181011_1041.root", "data/PulserHistos_r0124_20181011_1058.root",
    "data/PulserHistos_r0125_20181011_1115.root", "data/PulserHistos_r0126_20181011_1132.root",
    "data/PulserHistos_r0127_20181011_1149.root", "data/PulserHistos_r0128_20181011_1206.root",
    "data/PulserHistos_r0129_20181011_1223.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3};
  UInt_t uNbChanScan[kuNbFiles]  = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "18tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 19 cases
Bool_t AnalyseTimeCoincidenceBug19(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 12;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0130_20181011_1239.root", "data/PulserHistos_r0131_20181011_1257.root",
    "data/PulserHistos_r0132_20181011_1314.root", "data/PulserHistos_r0133_20181011_1331.root",
    "data/PulserHistos_r0134_20181011_1348.root", "data/PulserHistos_r0135_20181011_1405.root",
    "data/PulserHistos_r0136_20181011_1421.root", "data/PulserHistos_r0137_20181011_1438.root",
    "data/PulserHistos_r0138_20181011_1455.root", "data/PulserHistos_r0139_20181011_1512.root",
    "data/PulserHistos_r0140_20181011_1529.root", "data/PulserHistos_r0141_20181011_1546.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
  UInt_t uNbChanScan[kuNbFiles]  = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "19tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 20 cases
Bool_t AnalyseTimeCoincidenceBug20(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 11;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0142_20181011_1605.root", "data/PulserHistos_r0143_20181011_1622.root",
    "data/PulserHistos_r0144_20181011_1639.root", "data/PulserHistos_r0145_20181011_1657.root",
    "data/PulserHistos_r0146_20181011_1713.root", "data/PulserHistos_r0147_20181011_1730.root",
    "data/PulserHistos_r0148_20181011_1747.root", "data/PulserHistos_r0149_20181011_1804.root",
    "data/PulserHistos_r0150_20181011_1821.root", "data/PulserHistos_r0151_20181011_1838.root",
    "data/PulserHistos_r0152_20181011_1855.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5};
  UInt_t uNbChanScan[kuNbFiles]  = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "20tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 21 cases
Bool_t AnalyseTimeCoincidenceBug21(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 10;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0153_20181011_1912.root", "data/PulserHistos_r0154_20181011_1929.root",
    "data/PulserHistos_r0155_20181011_1946.root", "data/PulserHistos_r0156_20181011_2003.root",
    "data/PulserHistos_r0157_20181011_2020.root", "data/PulserHistos_r0158_20181011_2037.root",
    "data/PulserHistos_r0159_20181011_2054.root", "data/PulserHistos_r0160_20181011_2111.root",
    "data/PulserHistos_r0161_20181011_2128.root", "data/PulserHistos_r0162_20181011_2145.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6};
  UInt_t uNbChanScan[kuNbFiles]  = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "21tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 22 cases
Bool_t AnalyseTimeCoincidenceBug22(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 9;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0163_20181011_2202.root", "data/PulserHistos_r0164_20181011_2219.root",
    "data/PulserHistos_r0165_20181011_2236.root", "data/PulserHistos_r0166_20181011_2253.root",
    "data/PulserHistos_r0167_20181011_2310.root", "data/PulserHistos_r0168_20181011_2327.root",
    "data/PulserHistos_r0169_20181011_2344.root", "data/PulserHistos_r0170_20181012_0001.root",
    "data/PulserHistos_r0171_20181012_0018.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7};
  UInt_t uNbChanScan[kuNbFiles]  = {7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "22tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 23 cases
Bool_t AnalyseTimeCoincidenceBug23(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 8;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0172_20181012_0035.root", "data/PulserHistos_r0173_20181012_0052.root",
    "data/PulserHistos_r0174_20181012_0109.root", "data/PulserHistos_r0175_20181012_0126.root",
    "data/PulserHistos_r0176_20181012_0143.root", "data/PulserHistos_r0177_20181012_0200.root",
    "data/PulserHistos_r0178_20181012_0217.root", "data/PulserHistos_r0179_20181012_0234.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8};
  UInt_t uNbChanScan[kuNbFiles]  = {8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "23tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 24 cases
Bool_t AnalyseTimeCoincidenceBug24(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 7;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0180_20181012_0251.root", "data/PulserHistos_r0181_20181012_0308.root",
    "data/PulserHistos_r0182_20181012_0325.root", "data/PulserHistos_r0183_20181012_0342.root",
    "data/PulserHistos_r0184_20181012_0359.root", "data/PulserHistos_r0185_20181012_0416.root",
    "data/PulserHistos_r0186_20181012_1330.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9};
  UInt_t uNbChanScan[kuNbFiles]  = {9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "24tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 25 cases
Bool_t AnalyseTimeCoincidenceBug25(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 6;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0187_20181012_1347.root", "data/PulserHistos_r0188_20181012_1404.root",
    "data/PulserHistos_r0189_20181012_1421.root", "data/PulserHistos_r0190_20181012_1438.root",
    "data/PulserHistos_r0191_20181012_1455.root", "data/PulserHistos_r0192_20181012_1512.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10};
  UInt_t uNbChanScan[kuNbFiles]  = {10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "25tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 26 cases
Bool_t AnalyseTimeCoincidenceBug26(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 5;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0193_20181012_1528.root", "data/PulserHistos_r0194_20181012_1546.root",
    "data/PulserHistos_r0195_20181012_1603.root", "data/PulserHistos_r0196_20181012_1620.root",
    "data/PulserHistos_r0197_20181012_1636.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11};
  UInt_t uNbChanScan[kuNbFiles]  = {11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "26tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 27 cases
Bool_t AnalyseTimeCoincidenceBug27(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles       = 4;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {
    "data/PulserHistos_r0198_20181012_1653.root", "data/PulserHistos_r0199_20181012_1710.root",
    "data/PulserHistos_r0200_20181012_1727.root", "data/PulserHistos_r0201_20181012_2250.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12};
  UInt_t uNbChanScan[kuNbFiles]  = {12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "27tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 28 cases
Bool_t AnalyseTimeCoincidenceBug28(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles         = 3;
  const UInt_t kuNbStats         = 3;
  TString sFilename[kuNbFiles]   = {"data/PulserHistos_r0202_20181012_2057.root",
                                  "data/PulserHistos_r0203_20181012_2114.root",
                                  "data/PulserHistos_r0204_20181012_2131.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13};
  UInt_t uNbChanScan[kuNbFiles]  = {13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "28tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 29 cases
Bool_t AnalyseTimeCoincidenceBug29(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles         = 2;
  const UInt_t kuNbStats         = 3;
  TString sFilename[kuNbFiles]   = {"data/PulserHistos_r0205_20181012_2148.root",
                                  "data/PulserHistos_r0206_20181012_2205.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14};
  UInt_t uNbChanScan[kuNbFiles]  = {14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "29tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}

// n + m = 30 cases
Bool_t AnalyseTimeCoincidenceBug30(TFile* outf)
{
  /// Data source parameters
  const UInt_t kuNbFiles         = 1;
  const UInt_t kuNbStats         = 3;
  TString sFilename[kuNbFiles]   = {"data/PulserHistos_r0207_20181012_2222.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15};
  UInt_t uNbChanScan[kuNbFiles]  = {15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "30tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles, kuNbStats, sFilename, uNbChanFixed, uNbChanScan, sStats, runCase,
                                       outf);
}
Bool_t AnalyseTimeCoincidenceBugLimits(TString sFilename = "STSXyterPulserLimits_5Elinks.root")
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
  AnalyseTimeCoincidenceBug16(histoFile);
  AnalyseTimeCoincidenceBug17(histoFile);
  AnalyseTimeCoincidenceBug18(histoFile);
  AnalyseTimeCoincidenceBug19(histoFile);
  AnalyseTimeCoincidenceBug20(histoFile);
  AnalyseTimeCoincidenceBug21(histoFile);
  AnalyseTimeCoincidenceBug22(histoFile);
  AnalyseTimeCoincidenceBug23(histoFile);
  AnalyseTimeCoincidenceBug24(histoFile);
  AnalyseTimeCoincidenceBug25(histoFile);
  AnalyseTimeCoincidenceBug26(histoFile);
  AnalyseTimeCoincidenceBug27(histoFile);
  AnalyseTimeCoincidenceBug28(histoFile);
  AnalyseTimeCoincidenceBug29(histoFile);
  AnalyseTimeCoincidenceBug30(histoFile);

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
