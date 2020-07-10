Bool_t ResidualsPeakPosCosy2018(TString sPath      = "./data",
                                Double_t dFitRange = 3) {
  UInt_t uNbRuns = 89;

  std::vector<Double_t> dMeanX_S1(uNbRuns, 0.0);
  std::vector<Double_t> dMeanY_S1(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaX_S1(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaY_S1(uNbRuns, 0.0);

  std::vector<Double_t> dMeanX_S2(uNbRuns, 0.0);
  std::vector<Double_t> dMeanY_S2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaX_S2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaY_S2(uNbRuns, 0.0);

  TFile* pFile = NULL;
  for (UInt_t uRun = 0; uRun < uNbRuns; uRun++) {
    pFile = new TFile(
      Form("%s/EfficiencyHistos_dryrun_r%04u.root", sPath.Data(), uRun + 1),
      "READ");

    if (NULL != pFile) {
      gDirectory->cd("Sts_Cuts");

      TH2* phResidualPlot = dynamic_cast<TH2*>(
        gDirectory->FindObjectAny("fhResidualsBestPairsHodoS1"));
      if (NULL != phResidualPlot) {
        if (1000 < phResidualPlot->GetEntries()) {
          Double_t dMeanX = phResidualPlot->GetMean(1);
          Double_t dMeanY = phResidualPlot->GetMean(2);

          Double_t dRmsX = phResidualPlot->GetRMS(1);
          Double_t dRmsY = phResidualPlot->GetRMS(2);

          phResidualPlot->GetXaxis()->SetRangeUser(dMeanX - dFitRange * dRmsX,
                                                   dMeanX + dFitRange * dRmsX);
          phResidualPlot->GetYaxis()->SetRangeUser(dMeanY - dFitRange * dRmsY,
                                                   dMeanY + dFitRange * dRmsY);

          TF2* pFunc = new TF2("xyfit",
                               "xygaus",
                               dMeanX - dFitRange * dRmsX,
                               dMeanX + dFitRange * dRmsX,
                               dMeanY - dFitRange * dRmsY,
                               dMeanY + dFitRange * dRmsY);
          phResidualPlot->Fit("xyfit", "QRM0");

          dMeanX_S1[uRun]  = pFunc->GetParameter("MeanX");
          dSigmaX_S1[uRun] = pFunc->GetParameter("SigmaX");
          dMeanY_S1[uRun]  = pFunc->GetParameter("MeanY");
          dSigmaY_S1[uRun] = pFunc->GetParameter("SigmaY");

          delete pFunc;
        }  // if( 1000 < phResidualPlot->GetEntries() )
      }    // if( NULL != phResidualPlot )

      phResidualPlot = dynamic_cast<TH2*>(
        gDirectory->FindObjectAny("fhResidualsBestPairsHodoS2"));
      if (NULL != phResidualPlot) {
        if (1000 < phResidualPlot->GetEntries()) {
          Double_t dMeanX = phResidualPlot->GetMean(1);
          Double_t dMeanY = phResidualPlot->GetMean(2);

          Double_t dRmsX = phResidualPlot->GetRMS(1);
          Double_t dRmsY = phResidualPlot->GetRMS(2);

          phResidualPlot->GetXaxis()->SetRangeUser(dMeanX - dFitRange * dRmsX,
                                                   dMeanX + dFitRange * dRmsX);
          phResidualPlot->GetYaxis()->SetRangeUser(dMeanY - dFitRange * dRmsY,
                                                   dMeanY + dFitRange * dRmsY);

          TF2* pFunc = new TF2("xyfit",
                               "xygaus",
                               dMeanX - dFitRange * dRmsX,
                               dMeanX + dFitRange * dRmsX,
                               dMeanY - dFitRange * dRmsY,
                               dMeanY + dFitRange * dRmsY);
          phResidualPlot->Fit("xyfit", "QRM0");

          dMeanX_S2[uRun]  = pFunc->GetParameter("MeanX");
          dSigmaX_S2[uRun] = pFunc->GetParameter("SigmaX");
          dMeanY_S2[uRun]  = pFunc->GetParameter("MeanY");
          dSigmaY_S2[uRun] = pFunc->GetParameter("SigmaY");

          delete pFunc;
        }  // if( 1000 < phResidualPlot->GetEntries() )
      }    // if( NULL != fhResidualsBestPairsHodoS2 )

      pFile->Close();

      std::cout << Form("Run %04u done!", uRun + 1) << std::endl;
    }  // if( NULL != pFile )
    else
      return kFALSE;
  }  // for( UInt_t uRun = 0; uRun < uNbRuns; uRun++ )

  /// Printout the fitted values
  TString sMeanX_S1  = "   Double_t dMeanX_S1[ kiNbRuns ] = {\n      ";
  TString sMeanY_S1  = "   Double_t dMeanY_S1[ kiNbRuns ] = {\n      ";
  TString sSigmaX_S1 = "   Double_t dSigmaX_S1[ kiNbRuns ] = {\n      ";
  TString sSigmaY_S1 = "   Double_t dSigmaY_S1[ kiNbRuns ] = {\n      ";

  TString sMeanX_S2  = "   Double_t dMeanX_S2[ kiNbRuns ] = {\n      ";
  TString sMeanY_S2  = "   Double_t dMeanY_S2[ kiNbRuns ] = {\n      ";
  TString sSigmaX_S2 = "   Double_t dSigmaX_S2[ kiNbRuns ] = {\n      ";
  TString sSigmaY_S2 = "   Double_t dSigmaY_S2[ kiNbRuns ] = {\n      ";

  for (UInt_t uRun = 0; uRun < uNbRuns; uRun++) {
    TString sEnd = (uNbRuns - 1 == uRun
                      ? " };\n"
                      : (19 == uRun % 20 && 0 < uRun ? ",\n      " : ", "));
    sMeanX_S1 += Form("%6.3f%s", dMeanX_S1[uRun], sEnd.Data());
    sMeanY_S1 += Form("%6.3f%s", dMeanY_S1[uRun], sEnd.Data());
    sSigmaX_S1 += Form("%6.3f%s", dSigmaX_S1[uRun], sEnd.Data());
    sSigmaY_S1 += Form("%6.3f%s", dSigmaY_S1[uRun], sEnd.Data());

    sMeanX_S2 += Form("%6.3f%s", dMeanX_S2[uRun], sEnd.Data());
    sMeanY_S2 += Form("%6.3f%s", dMeanY_S2[uRun], sEnd.Data());
    sSigmaX_S2 += Form("%6.3f%s", dSigmaX_S2[uRun], sEnd.Data());
    sSigmaY_S2 += Form("%6.3f%s", dSigmaY_S2[uRun], sEnd.Data());
  }  // for( UInt_t uRun = 0; uRun < uNbRuns; uRun++ )

  std::cout << sMeanX_S1 << std::endl;
  std::cout << sMeanY_S1 << std::endl;
  std::cout << sSigmaX_S1 << std::endl;
  std::cout << sSigmaY_S1 << std::endl;

  std::cout << sMeanX_S2 << std::endl;
  std::cout << sMeanY_S2 << std::endl;
  std::cout << sSigmaX_S2 << std::endl;
  std::cout << sSigmaY_S2 << std::endl;

  return kTRUE;
}
