Bool_t EfficiencySummaryCosy2018(TString sPath = "./data")
{
  UInt_t uNbRuns = 89;

  TH2* phEfficiencySummary =
    new TH2D("hEfficiencySummary", "Efficiency as function of run, with GeoCut; Run; ; Efficiency", uNbRuns, 1,
             uNbRuns + 1, 8, -0.5, 7.5);
  phEfficiencySummary->GetYaxis()->SetBinLabel(1, "S1");
  phEfficiencySummary->GetYaxis()->SetBinLabel(2, "S2");
  phEfficiencySummary->GetYaxis()->SetBinLabel(3, "S1|S2");
  phEfficiencySummary->GetYaxis()->SetBinLabel(4, "S2|S1");
  phEfficiencySummary->GetYaxis()->SetBinLabel(5, "S1 corr");
  phEfficiencySummary->GetYaxis()->SetBinLabel(6, "S2 corr");
  phEfficiencySummary->GetYaxis()->SetBinLabel(7, "S1|S2 corr");
  phEfficiencySummary->GetYaxis()->SetBinLabel(8, "S2|S1 corr");

  TFile* pFile = NULL;
  for (UInt_t uRun = 0; uRun < uNbRuns; uRun++) {
    // No Corr
    pFile = new TFile(Form("%s/EfficiencyHistos_NoCorr_r%04u.root", sPath.Data(), uRun + 1), "READ");

    if (NULL != pFile) {
      gDirectory->cd("Sts_Eff");

      TProfile* phEfficiency = dynamic_cast<TProfile*>(gDirectory->FindObjectAny("fhEfficiency"));
      if (NULL != phEfficiency) {
        phEfficiencySummary->Fill(uRun + 1, 0., phEfficiency->GetBinContent(5));
        phEfficiencySummary->Fill(uRun + 1, 1., phEfficiency->GetBinContent(6));
        phEfficiencySummary->Fill(uRun + 1, 2., phEfficiency->GetBinContent(7));
        phEfficiencySummary->Fill(uRun + 1, 3., phEfficiency->GetBinContent(8));
      }  // if( NULL != phEfficiency )

      pFile->Close();

      std::cout << Form("Run %04u done!", uRun + 1) << std::endl;
    }  // if( NULL != pFile )
    else
      return kFALSE;

    // Dead Corr
    pFile = new TFile(Form("%s/EfficiencyHistos_DeadCorr_r%04u.root", sPath.Data(), uRun + 1), "READ");

    if (NULL != pFile) {
      gDirectory->cd("Sts_Eff");

      TProfile* phEfficiency = dynamic_cast<TProfile*>(gDirectory->FindObjectAny("fhEfficiency"));
      if (NULL != phEfficiency) {
        phEfficiencySummary->Fill(uRun + 1, 4., phEfficiency->GetBinContent(5));
        phEfficiencySummary->Fill(uRun + 1, 5., phEfficiency->GetBinContent(6));
        phEfficiencySummary->Fill(uRun + 1, 6., phEfficiency->GetBinContent(7));
        phEfficiencySummary->Fill(uRun + 1, 7., phEfficiency->GetBinContent(8));
      }  // if( NULL != phEfficiency )

      pFile->Close();

      std::cout << Form("Run %04u done!", uRun + 1) << std::endl;
    }  // if( NULL != pFile )
    else
      return kFALSE;
  }  // for( UInt_t uRun = 0; uRun < uNbRuns; uRun++ )

  /// Display the summary plot
  phEfficiencySummary->Draw("colz");
  gPad->SetGridx();
  gPad->SetGridy();

  return kTRUE;
}
