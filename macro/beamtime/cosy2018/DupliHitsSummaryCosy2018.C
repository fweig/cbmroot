Bool_t DupliHitsSummaryCosy2018()
{
  UInt_t uNbRuns = 89;

  TH1* pSummaryDupliHitsRatio = new TH1D("SummaryDupliHitsRatio",
                                         "Proportion of Dupli Hits among message per run at COSY 2018; Run "
                                         "[]; Dupli hits [\%]",
                                         uNbRuns, 1, uNbRuns + 1);
  TH1* pSummaryGoodHitsRatio  = new TH1D("SummaryGoodHitsRatio",
                                        "Proportion of Good Hits among message per run at COSY 2018; Run "
                                        "[]; Good hits [\%]",
                                        uNbRuns, 1, uNbRuns + 1);
  TH1* pSummaryTsMsbRatio     = new TH1D("SummaryTsMsbRatio",
                                     "Proportion of TS MSB among message per "
                                     "run at COSY 2018; Run []; TS MSB [\%]",
                                     uNbRuns, 1, uNbRuns + 1);
  TH1* pSummaryEmptyRatio     = new TH1D("SummaryEmptyRatio",
                                     "Proportion of EMPTY among message per "
                                     "run at COSY 2018; Run []; Empty [\%]",
                                     uNbRuns, 1, uNbRuns + 1);

  TFile* pFile = NULL;
  for (UInt_t uRun = 0; uRun < uNbRuns; uRun++) {
    pFile = new TFile(Form("data/DuplisHistos_r%04u.root", uRun + 1), "READ");

    if (NULL != pFile) {
      gDirectory->cd("Sts_Raw");
      gDirectory->cd("Sts_Duplis");
      TProfile* phFractionTypes = dynamic_cast<TProfile*>(gDirectory->FindObjectAny("fhFractionTypes"));
      if (NULL != phFractionTypes) {
        pSummaryGoodHitsRatio->Fill(uRun + 1, 100.0 * phFractionTypes->GetBinContent(1));
        pSummaryDupliHitsRatio->Fill(uRun + 1, 100.0 * phFractionTypes->GetBinContent(2));
        pSummaryTsMsbRatio->Fill(uRun + 1, 100.0 * phFractionTypes->GetBinContent(3));
        pSummaryEmptyRatio->Fill(uRun + 1, 100.0 * phFractionTypes->GetBinContent(5));
      }  // if( NULL != phFractionTypes )
      pFile->Close();

      std::cout << Form("Run %04u done!", uRun + 1) << std::endl;
    }  // if( NULL != pFile )
    else
      continue;
  }  // for( UInt_t uRun = 0; uRun < uNbRuns; uRun++ )

  pSummaryDupliHitsRatio->Draw("Hist");

  pFile = new TFile("data/DuplisHitsSummary_Cosy2018.root", "RECREATE");
  pSummaryDupliHitsRatio->Write();
  pSummaryGoodHitsRatio->Write();
  pSummaryTsMsbRatio->Write();
  pSummaryEmptyRatio->Write();
  pFile->Close();

  return kTRUE;
}
