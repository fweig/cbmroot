Bool_t DtPeakPosSummaryCosy2018(TString sPath      = "./data",
                                Double_t dFitRange = 200) {
  static const UInt_t uNbRuns = 89;

  TH1* pSummaryDtMeanX1Y1 = new TH1D(
    "SummaryDtMeanX1Y1",
    "Position of Dt peak X1Y1 per run at COSY 2018; Run []; X1Y1 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtMeanX2Y2 = new TH1D(
    "SummaryDtMeanX2Y2",
    "Position of Dt peak X2Y2 per run at COSY 2018; Run []; X2Y2 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtMeanN1P1 = new TH1D(
    "SummaryDtMeanN1P1",
    "Position of Dt peak N1P1 per run at COSY 2018; Run []; N1P1 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtMeanN2P2 = new TH1D(
    "SummaryDtMeanN2P2",
    "Position of Dt peak N2P2 per run at COSY 2018; Run []; N2P2 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtMeanX1Y1X2Y2 =
    new TH1D("SummaryDtMeanX1Y1X2Y2",
             "Position of Dt peak X1Y1X2Y2 per run at COSY 2018; Run []; "
             "X1Y1X2Y2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtMeanN1P1N2P2 =
    new TH1D("SummaryDtMeanN1P1N2P2",
             "Position of Dt peak N1P1N2P2 per run at COSY 2018; Run []; "
             "N1P1N2P2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtMeanX1Y1X2Y2N1P1 =
    new TH1D("SummaryDtMeanX1Y1X2Y2N1P1",
             "Position of Dt peak X1Y1X2Y2N1P1 per run at COSY 2018; Run []; "
             "X1Y1X2Y2N1P1 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtMeanX1Y1X2Y2N2P2 =
    new TH1D("SummaryDtMeanX1Y1X2Y2N2P2",
             "Position of Dt peak X1Y1X2Y2N2P2 per run at COSY 2018; Run []; "
             "X1Y1X2Y2N2P2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtMeanH1H2S1S2 =
    new TH1D("SummaryDtMeanH1H2S1S2",
             "Position of Dt peak H1H2S1S2 per run at COSY 2018; Run []; "
             "H1H2S1S2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);

  TH1* pSummaryDtSigmaX1Y1 = new TH1D(
    "SummaryDtSigmaX1Y1",
    "Position of Dt peak X1Y1 per run at COSY 2018; Run []; X1Y1 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtSigmaX2Y2 = new TH1D(
    "SummaryDtSigmaX2Y2",
    "Position of Dt peak X2Y2 per run at COSY 2018; Run []; X2Y2 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtSigmaN1P1 = new TH1D(
    "SummaryDtSigmaN1P1",
    "Position of Dt peak N1P1 per run at COSY 2018; Run []; N1P1 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtSigmaN2P2 = new TH1D(
    "SummaryDtSigmaN2P2",
    "Position of Dt peak N2P2 per run at COSY 2018; Run []; N2P2 peak [ns]",
    uNbRuns,
    1,
    uNbRuns + 1);
  TH1* pSummaryDtSigmaX1Y1X2Y2 =
    new TH1D("SummaryDtSigmaX1Y1X2Y2",
             "Position of Dt peak X1Y1X2Y2 per run at COSY 2018; Run []; "
             "X1Y1X2Y2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtSigmaN1P1N2P2 =
    new TH1D("SummaryDtSigmaN1P1N2P2",
             "Position of Dt peak N1P1N2P2 per run at COSY 2018; Run []; "
             "N1P1N2P2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtSigmaX1Y1X2Y2N1P1 =
    new TH1D("SummaryDtSigmaX1Y1X2Y2N1P1",
             "Position of Dt peak X1Y1X2Y2N1P1 per run at COSY 2018; Run []; "
             "X1Y1X2Y2N1P1 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtSigmaX1Y1X2Y2N2P2 =
    new TH1D("SummaryDtSigmaX1Y1X2Y2N2P2",
             "Position of Dt peak X1Y1X2Y2N2P2 per run at COSY 2018; Run []; "
             "X1Y1X2Y2N2P2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);
  TH1* pSummaryDtSigmaH1H2S1S2 =
    new TH1D("SummaryDtSigmaH1H2S1S2",
             "Position of Dt peak H1H2S1S2 per run at COSY 2018; Run []; "
             "H1H2S1S2 peak [ns]",
             uNbRuns,
             1,
             uNbRuns + 1);

  TFile* pFile = NULL;

  std::vector<Double_t> dMeanH1(uNbRuns, 0.0);
  std::vector<Double_t> dMeanH2(uNbRuns, 0.0);
  std::vector<Double_t> dMeanS1(uNbRuns, 0.0);
  std::vector<Double_t> dMeanS2(uNbRuns, 0.0);
  std::vector<Double_t> dMeanH1H2(uNbRuns, 0.0);
  std::vector<Double_t> dMeanS1S2(uNbRuns, 0.0);
  std::vector<Double_t> dMeanH1H2S1(uNbRuns, 0.0);
  std::vector<Double_t> dMeanH1H2S2(uNbRuns, 0.0);
  std::vector<Double_t> dMeanH1H2S1S2(uNbRuns, 0.0);

  std::vector<Double_t> dSigmaH1(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaH2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaS1(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaS2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaH1H2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaS1S2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaH1H2S1(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaH1H2S2(uNbRuns, 0.0);
  std::vector<Double_t> dSigmaH1H2S1S2(uNbRuns, 0.0);

  std::vector<Double_t> dWidthH1(uNbRuns, 0.0);
  std::vector<Double_t> dWidthH2(uNbRuns, 0.0);
  std::vector<Double_t> dWidthS1(uNbRuns, 0.0);
  std::vector<Double_t> dWidthS2(uNbRuns, 0.0);
  std::vector<Double_t> dWidthH1H2(uNbRuns, 0.0);
  std::vector<Double_t> dWidthS1S2(uNbRuns, 0.0);
  std::vector<Double_t> dWidthH1H2S1(uNbRuns, 0.0);
  std::vector<Double_t> dWidthH1H2S2(uNbRuns, 0.0);
  std::vector<Double_t> dWidthH1H2S1S2(uNbRuns, 0.0);


  //   TCanvas * canvDt[uNbRuns];
  for (UInt_t uRun = 0; uRun < uNbRuns; uRun++) {
    /*
      canvDt[ uRun ] = new TCanvas( Form("canvDt%02u", uRun), Form("Dt plots %02u", uRun) );
      canvDt[ uRun ]->Divide( 3, 3 );
*/
    pFile = new TFile(
      Form("%s/EfficiencyHistos_dryrun_r%04u.root", sPath.Data(), uRun + 1),
      "READ");

    if (NULL != pFile) {
      gDirectory->cd("Sts_Raw");

      TH1* phDtPlot =
        dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhSetupSortedDtX1Y1"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fX1Y1_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->SetParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fX1Y1_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanX1Y1->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaX1Y1->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanH1[uRun]  = pFunc->GetParameter(1);
              dSigmaH1[uRun] = pFunc->GetParameter(2);
              dWidthH1[uRun] = 4.0 * dSigmaH1[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 1 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot =
        dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhSetupSortedDtX2Y2"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fX2Y2_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->SetParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fX2Y2_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanX2Y2->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaX2Y2->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanH2[uRun]  = pFunc->GetParameter(1);
              dSigmaH2[uRun] = pFunc->GetParameter(2);
              dWidthH2[uRun] = 4.0 * dSigmaH2[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 2 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot =
        dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhSetupSortedDtN1P1"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fN1P1_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->SetParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fN1P1_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanN1P1->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaN1P1->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanS1[uRun]  = pFunc->GetParameter(1);
              dSigmaS1[uRun] = pFunc->GetParameter(2);
              dWidthS1[uRun] = 8.0 * dSigmaS1[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 3 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot =
        dynamic_cast<TH1*>(gDirectory->FindObjectAny("fhSetupSortedDtN2P2"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fN2P2_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->SetParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fN2P2_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanN2P2->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaN2P2->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanS2[uRun]  = pFunc->GetParameter(1);
              dSigmaS2[uRun] = pFunc->GetParameter(2);
              dWidthS2[uRun] = 8.0 * dSigmaS2[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 4 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot = dynamic_cast<TH1*>(
        gDirectory->FindObjectAny("fhSetupSortedDtX1Y1X2Y2"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fX1Y1X2Y2_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->FixParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fX1Y1X2Y2_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanX1Y1X2Y2->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaX1Y1X2Y2->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanH1H2[uRun]  = pFunc->GetParameter(1);
              dSigmaH1H2[uRun] = pFunc->GetParameter(2);
              dWidthH1H2[uRun] = 4.0 * dSigmaH1H2[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 5 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot = dynamic_cast<TH1*>(
        gDirectory->FindObjectAny("fhSetupSortedDtN1P1N2P2"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fN1P1N2P2_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->FixParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fN1P1N2P2_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanN1P1N2P2->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaN1P1N2P2->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanS1S2[uRun]  = pFunc->GetParameter(1);
              dSigmaS1S2[uRun] = pFunc->GetParameter(2);
              dWidthS1S2[uRun] = 4.0 * dSigmaS1S2[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 6 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot = dynamic_cast<TH1*>(
        gDirectory->FindObjectAny("fhSetupSortedDtX1Y1X2Y2N1P1"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fX1Y1X2Y2N1P1_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->FixParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fX1Y1X2Y2N1P1_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanX1Y1X2Y2N1P1->Fill(uRun + 1,
                                               pFunc->GetParameter(1));
              pSummaryDtSigmaX1Y1X2Y2N1P1->Fill(uRun + 1,
                                                pFunc->GetParameter(2));

              dMeanH1H2S1[uRun]  = pFunc->GetParameter(1);
              dSigmaH1H2S1[uRun] = pFunc->GetParameter(2);
              dWidthH1H2S1[uRun] = 4.0 * dSigmaH1H2S1[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 7 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot = dynamic_cast<TH1*>(
        gDirectory->FindObjectAny("fhSetupSortedDtX1Y1X2Y2N2P2"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fX1Y1X2Y2N2P2_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->FixParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fX1Y1X2Y2N2P2_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanX1Y1X2Y2N2P2->Fill(uRun + 1,
                                               pFunc->GetParameter(1));
              pSummaryDtSigmaX1Y1X2Y2N2P2->Fill(uRun + 1,
                                                pFunc->GetParameter(2));

              dMeanH1H2S2[uRun]  = pFunc->GetParameter(1);
              dSigmaH1H2S2[uRun] = pFunc->GetParameter(2);
              dWidthH1H2S2[uRun] = 4.0 * dSigmaH1H2S2[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 8 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )

      phDtPlot = dynamic_cast<TH1*>(
        gDirectory->FindObjectAny("fhSetupSortedDtH1H2S1S2"));
      if (NULL != phDtPlot) {
        if (1000 < phDtPlot->GetEntries()) {
          Double_t dPeakPos = phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin());

          phDtPlot->GetXaxis()->SetRangeUser(dPeakPos - dFitRange,
                                             dPeakPos + dFitRange);

          // Read integral and check how much we have close to the peak
          Double_t dZoomCounts = phDtPlot->Integral();

          if (1000 < dZoomCounts && TMath::Abs(dPeakPos) < 1000) {
            TF1* pFunc = new TF1(Form("fH1H2S1S2_%04u", uRun + 1),
                                 "gaus",
                                 dPeakPos - dFitRange,
                                 dPeakPos + dFitRange);
            pFunc->SetParameter(
              0, phDtPlot->GetBinCenter(phDtPlot->GetMaximumBin()));
            pFunc->FixParameter(1, dPeakPos);
            pFunc->SetParameter(
              2, 3.0);  // Hardcode start with ~BinWidth, do better later

            // Using integral instead of bin center seems to lead to unrealistic values => no "I"
            phDtPlot->Fit(Form("fH1H2S1S2_%04u", uRun + 1), "QRM0");

            if (pFunc->GetParameter(2) < dFitRange) {
              pSummaryDtMeanH1H2S1S2->Fill(uRun + 1, pFunc->GetParameter(1));
              pSummaryDtSigmaH1H2S1S2->Fill(uRun + 1, pFunc->GetParameter(2));

              dMeanH1H2S1S2[uRun]  = pFunc->GetParameter(1);
              dSigmaH1H2S1S2[uRun] = pFunc->GetParameter(2);
              dWidthH1H2S1S2[uRun] = 4.0 * dSigmaH1H2S1S2[uRun];
            }  // if( pFunc->GetParameter(2) < dFitRange )
               /*
                  canvDt[ uRun ]->cd( 9 );
                  phDtPlot->Draw( "hist" );
*/
            delete pFunc;
          }  // if( 1000 < dZoomCounts && TMath::Abs( dPeakPos ) < 1000 )
        }    // if( 1000 < phDtPlot->GetEntries() )
      }      // if( NULL != phDtPlot )
             /*
         std::string sDump;
         std::cout << "Press enter for next file" << std::endl;
         std::cin >> sDump;
//         std::cin.ignore();
*/
      pFile->Close();

      std::cout << Form("Run %04u done!", uRun + 1) << std::endl;
    }  // if( NULL != pFile )
    else
      continue;
  }  // for( UInt_t uRun = 0; uRun < uNbRuns; uRun++ )

  TCanvas* canvasMean = new TCanvas("canvMean", "Mean of gauss fit on dt peak");
  canvasMean->Divide(3, 3);
  canvasMean->cd(1);
  pSummaryDtMeanX1Y1->Draw("hist");
  canvasMean->cd(2);
  pSummaryDtMeanX2Y2->Draw("hist");
  canvasMean->cd(3);
  pSummaryDtMeanN1P1->Draw("hist");
  canvasMean->cd(4);
  pSummaryDtMeanN2P2->Draw("hist");
  canvasMean->cd(5);
  pSummaryDtMeanX1Y1X2Y2->Draw("hist");
  canvasMean->cd(6);
  pSummaryDtMeanN1P1N2P2->Draw("hist");
  canvasMean->cd(7);
  pSummaryDtMeanX1Y1X2Y2N1P1->Draw("hist");
  canvasMean->cd(8);
  pSummaryDtMeanX1Y1X2Y2N2P2->Draw("hist");
  canvasMean->cd(9);
  pSummaryDtMeanH1H2S1S2->Draw("hist");

  TCanvas* canvasSigma =
    new TCanvas("canvSigma", "Sigma of gauss fit on dt peak");
  canvasSigma->Divide(3, 3);
  canvasSigma->cd(1);
  pSummaryDtSigmaX1Y1->Draw("hist");
  canvasSigma->cd(2);
  pSummaryDtSigmaX2Y2->Draw("hist");
  canvasSigma->cd(3);
  pSummaryDtSigmaN1P1->Draw("hist");
  canvasSigma->cd(4);
  pSummaryDtSigmaN2P2->Draw("hist");
  canvasSigma->cd(5);
  pSummaryDtSigmaX1Y1X2Y2->Draw("hist");
  canvasSigma->cd(6);
  pSummaryDtSigmaN1P1N2P2->Draw("hist");
  canvasSigma->cd(7);
  pSummaryDtSigmaX1Y1X2Y2N1P1->Draw("hist");
  canvasSigma->cd(8);
  pSummaryDtSigmaX1Y1X2Y2N2P2->Draw("hist");
  canvasSigma->cd(9);
  pSummaryDtSigmaH1H2S1S2->Draw("hist");

  pFile = new TFile("./DtPeakSummary_Cosy2018.root", "RECREATE");
  pSummaryDtMeanX1Y1->Write();
  pSummaryDtMeanX2Y2->Write();
  pSummaryDtMeanN1P1->Write();
  pSummaryDtMeanN2P2->Write();
  pSummaryDtMeanX1Y1X2Y2->Write();
  pSummaryDtMeanN1P1N2P2->Write();
  pSummaryDtMeanX1Y1X2Y2N1P1->Write();
  pSummaryDtMeanX1Y1X2Y2N2P2->Write();
  pSummaryDtMeanH1H2S1S2->Write();

  pSummaryDtSigmaX1Y1->Write();
  pSummaryDtSigmaX2Y2->Write();
  pSummaryDtSigmaN1P1->Write();
  pSummaryDtSigmaN2P2->Write();
  pSummaryDtSigmaX1Y1X2Y2->Write();
  pSummaryDtSigmaN1P1N2P2->Write();
  pSummaryDtSigmaX1Y1X2Y2N1P1->Write();
  pSummaryDtSigmaX1Y1X2Y2N2P2->Write();
  pSummaryDtSigmaH1H2S1S2->Write();
  pFile->Close();

  /// Printout the fitted values
  TString sMeanH1       = "   Double_t dMeanH1[ kiNbRuns ] = {\n      ";
  TString sMeanH2       = "   Double_t dMeanH2[ kiNbRuns ] = {\n      ";
  TString sMeanS1       = "   Double_t dMeanS1[ kiNbRuns ] = {\n      ";
  TString sMeanS2       = "   Double_t dMeanS2[ kiNbRuns ] = {\n      ";
  TString sMeanH1H2     = "   Double_t dMeanH1H2[ kiNbRuns ] = {\n      ";
  TString sMeanS1S2     = "   Double_t dMeanS1S2[ kiNbRuns ] = {\n      ";
  TString sMeanH1H2S1   = "   Double_t dMeanH1H2S1[ kiNbRuns ] = {\n      ";
  TString sMeanH1H2S2   = "   Double_t dMeanH1H2S2[ kiNbRuns ] = {\n      ";
  TString sMeanH1H2S1S2 = "   Double_t dMeanH1H2S1S2[ kiNbRuns ] = {\n      ";

  TString sSigmaH1       = "   Double_t dSigmaH1[ kiNbRuns ] = {\n      ";
  TString sSigmaH2       = "   Double_t dSigmaH2[ kiNbRuns ] = {\n      ";
  TString sSigmaS1       = "   Double_t dSigmaS1[ kiNbRuns ] = {\n      ";
  TString sSigmaS2       = "   Double_t dSigmaS2[ kiNbRuns ] = {\n      ";
  TString sSigmaH1H2     = "   Double_t dSigmaH1H2[ kiNbRuns ] = {\n      ";
  TString sSigmaS1S2     = "   Double_t dSigmaS1S2[ kiNbRuns ] = {\n      ";
  TString sSigmaH1H2S1   = "   Double_t dSigmaH1H2S1[ kiNbRuns ] = {\n      ";
  TString sSigmaH1H2S2   = "   Double_t dSigmaH1H2S2[ kiNbRuns ] = {\n      ";
  TString sSigmaH1H2S1S2 = "   Double_t dSigmaH1H2S1S2[ kiNbRuns ] = {\n      ";

  TString sWidthH1       = "   Double_t dWidthH1[ kiNbRuns ] = {\n      ";
  TString sWidthH2       = "   Double_t dWidthH2[ kiNbRuns ] = {\n      ";
  TString sWidthS1       = "   Double_t dWidthS1[ kiNbRuns ] = {\n      ";
  TString sWidthS2       = "   Double_t dWidthS2[ kiNbRuns ] = {\n      ";
  TString sWidthH1H2     = "   Double_t dWidthH1H2[ kiNbRuns ] = {\n      ";
  TString sWidthS1S2     = "   Double_t dWidthS1S2[ kiNbRuns ] = {\n      ";
  TString sWidthH1H2S1   = "   Double_t dWidthH1H2S1[ kiNbRuns ] = {\n      ";
  TString sWidthH1H2S2   = "   Double_t dWidthH1H2S2[ kiNbRuns ] = {\n      ";
  TString sWidthH1H2S1S2 = "   Double_t dWidthH1H2S1S2[ kiNbRuns ] = {\n      ";

  for (UInt_t uRun = 0; uRun < uNbRuns; uRun++) {
    TString sEnd = (uNbRuns - 1 == uRun
                      ? " };\n"
                      : (19 == uRun % 20 && 0 < uRun ? ",\n      " : ", "));
    sMeanH1 += Form("%6.3f%s", dMeanH1[uRun], sEnd.Data());
    sMeanH2 += Form("%6.3f%s", dMeanH2[uRun], sEnd.Data());
    sMeanS1 += Form("%6.3f%s", dMeanS1[uRun], sEnd.Data());
    sMeanS2 += Form("%6.3f%s", dMeanS2[uRun], sEnd.Data());
    sMeanH1H2 += Form("%6.3f%s", dMeanH1H2[uRun], sEnd.Data());
    sMeanS1S2 += Form("%6.3f%s", dMeanS1S2[uRun], sEnd.Data());
    sMeanH1H2S1 += Form("%6.3f%s", dMeanH1H2S1[uRun], sEnd.Data());
    sMeanH1H2S2 += Form("%6.3f%s", dMeanH1H2S2[uRun], sEnd.Data());
    sMeanH1H2S1S2 += Form("%6.3f%s", dMeanH1H2S1S2[uRun], sEnd.Data());

    sSigmaH1 += Form("%6.3f%s", dSigmaH1[uRun], sEnd.Data());
    sSigmaH2 += Form("%6.3f%s", dSigmaH2[uRun], sEnd.Data());
    sSigmaS1 += Form("%6.3f%s", dSigmaS1[uRun], sEnd.Data());
    sSigmaS2 += Form("%6.3f%s", dSigmaS2[uRun], sEnd.Data());
    sSigmaH1H2 += Form("%6.3f%s", dSigmaH1H2[uRun], sEnd.Data());
    sSigmaS1S2 += Form("%6.3f%s", dSigmaS1S2[uRun], sEnd.Data());
    sSigmaH1H2S1 += Form("%6.3f%s", dSigmaH1H2S1[uRun], sEnd.Data());
    sSigmaH1H2S2 += Form("%6.3f%s", dSigmaH1H2S2[uRun], sEnd.Data());
    sSigmaH1H2S1S2 += Form("%6.3f%s", dSigmaH1H2S1S2[uRun], sEnd.Data());

    sWidthH1 += Form("%6.3f%s", dWidthH1[uRun], sEnd.Data());
    sWidthH2 += Form("%6.3f%s", dWidthH2[uRun], sEnd.Data());
    sWidthS1 += Form("%6.3f%s", dWidthS1[uRun], sEnd.Data());
    sWidthS2 += Form("%6.3f%s", dWidthS2[uRun], sEnd.Data());
    sWidthH1H2 += Form("%6.3f%s", dWidthH1H2[uRun], sEnd.Data());
    sWidthS1S2 += Form("%6.3f%s", dWidthS1S2[uRun], sEnd.Data());
    sWidthH1H2S1 += Form("%6.3f%s", dWidthH1H2S1[uRun], sEnd.Data());
    sWidthH1H2S2 += Form("%6.3f%s", dWidthH1H2S2[uRun], sEnd.Data());
    sWidthH1H2S1S2 += Form("%6.3f%s", dWidthH1H2S1S2[uRun], sEnd.Data());
    /*
      if( 0 == uRun % 20 && 0 < uRun )
      {
         sMeanH1       += "\n   ";
         sMeanH2       += "\n   ";
         sMeanS1       += "\n   ";
         sMeanS2       += "\n   ";
         sMeanH1H2     += "\n   ";
         sMeanS1S2     += "\n   ";
         sMeanH1H2S1   += "\n   ";
         sMeanH1H2S2   += "\n   ";
         sMeanH1H2S1S2 += "\n   ";

         sSigmaH1       += "\n   ";
         sSigmaH2       += "\n   ";
         sSigmaS1       += "\n   ";
         sSigmaS2       += "\n   ";
         sSigmaH1H2     += "\n   ";
         sSigmaS1S2     += "\n   ";
         sSigmaH1H2S1   += "\n   ";
         sSigmaH1H2S2   += "\n   ";
         sSigmaH1H2S1S2 += "\n   ";

         sWidthH1       += "\n   ";
         sWidthH2       += "\n   ";
         sWidthS1       += "\n   ";
         sWidthS2       += "\n   ";
         sWidthH1H2     += "\n   ";
         sWidthS1S2     += "\n   ";
         sWidthH1H2S1   += "\n   ";
         sWidthH1H2S2   += "\n   ";
         sWidthH1H2S1S2 += "\n   ";
      } // if( 0 == uRun % 20 && 0 < uRun )
*/
  }  // for( UInt_t uRun = 0; uRun < uNbRuns; uRun++ )
     /*
   sMeanH1       += "}";
   sMeanH2       += "}";
   sMeanS1       += "}";
   sMeanS2       += "}";
   sMeanH1H2     += "}";
   sMeanS1S2     += "}";
   sMeanH1H2S1   += "}";
   sMeanH1H2S2   += "}";
   sMeanH1H2S1S2 += "}";

   sSigmaH1       += "}";
   sSigmaH2       += "}";
   sSigmaS1       += "}";
   sSigmaS2       += "}";
   sSigmaH1H2     += "}";
   sSigmaS1S2     += "}";
   sSigmaH1H2S1   += "}";
   sSigmaH1H2S2   += "}";
   sSigmaH1H2S1S2 += "}";

   sWidthH1       += "}";
   sWidthH2       += "}";
   sWidthS1       += "}";
   sWidthS2       += "}";
   sWidthH1H2     += "}";
   sWidthS1S2     += "}";
   sWidthH1H2S1   += "}";
   sWidthH1H2S2   += "}";
   sWidthH1H2S1S2 += "}";
*/
  std::cout << sMeanH1 << std::endl;
  std::cout << sMeanH2 << std::endl;
  std::cout << sMeanS1 << std::endl;
  std::cout << sMeanS2 << std::endl;
  std::cout << sMeanH1H2 << std::endl;
  std::cout << sMeanS1S2 << std::endl;
  std::cout << sMeanH1H2S1 << std::endl;
  std::cout << sMeanH1H2S2 << std::endl;
  std::cout << sMeanH1H2S1S2 << std::endl;

  std::cout << sSigmaH1 << std::endl;
  std::cout << sSigmaH2 << std::endl;
  std::cout << sSigmaS1 << std::endl;
  std::cout << sSigmaS2 << std::endl;
  std::cout << sSigmaH1H2 << std::endl;
  std::cout << sSigmaS1S2 << std::endl;
  std::cout << sSigmaH1H2S1 << std::endl;
  std::cout << sSigmaH1H2S2 << std::endl;
  std::cout << sSigmaH1H2S1S2 << std::endl;

  std::cout << sWidthH1 << std::endl;
  std::cout << sWidthH2 << std::endl;
  std::cout << sWidthS1 << std::endl;
  std::cout << sWidthS2 << std::endl;
  std::cout << sWidthH1H2 << std::endl;
  std::cout << sWidthS1S2 << std::endl;
  std::cout << sWidthH1H2S1 << std::endl;
  std::cout << sWidthH1H2S2 << std::endl;
  std::cout << sWidthH1H2S1S2 << std::endl;

  std::cout << dMeanH1[83] << " " << dMeanH2[83] << " " << dMeanS1[83] << " "
            << dMeanS2[83] << " " << dMeanH1H2[83] << " " << dMeanS1S2[83]
            << " " << dMeanH1H2S1[83] << " " << dMeanH1H2S2[83] << " "
            << dMeanH1H2S1S2[83] << std::endl;
  std::cout << dSigmaH1[83] << " " << dSigmaH2[83] << " " << dSigmaS1[83] << " "
            << dSigmaS2[83] << " " << dSigmaH1H2[83] << " " << dSigmaS1S2[83]
            << " " << dSigmaH1H2S1[83] << " " << dSigmaH1H2S2[83] << " "
            << dSigmaH1H2S1S2[83] << std::endl;
  std::cout << dWidthH1[83] << " " << dWidthH2[83] << " " << dWidthS1[83] << " "
            << dWidthS2[83] << " " << dWidthH1H2[83] << " " << dWidthS1S2[83]
            << " " << dWidthH1H2S1[83] << " " << dWidthH1H2S2[83] << " "
            << dWidthH1H2S1S2[83] << std::endl;

  return kTRUE;
}
