
Bool_t T0MoniPlot()
{
  /// 23/03/2019
  const UInt_t kuNbRunPerStack = 4;
  const UInt_t kuNbRuns        = 22;
  UInt_t uRunId[kuNbRuns]      = {
    89,  90,  91,  92,  93,
    94,  95,  96,  97,  // Detectors parameters scan
    98,  99,  100, 101, 102,
    103, 104, 105, 106,  // Beam intensity and target scan (=> collision rate scan up, empty, no beam)
    107, 109, 110,       // T0 threshold scan
    111                  // Cooldown run
  };

  /// Obtaining the plots
  TH1* tempH1 = NULL;
  TH1* phHitsPerSpill[kuNbRuns];
  TH1* phHitCntEvo[kuNbRuns];
  TFile* pFile[kuNbRuns];
  for (UInt_t uRun = 0; uRun < kuNbRuns; ++uRun) {
    pFile[uRun] = TFile::Open(Form("data/HistosMonitorT0_%03u.root", uRunId[uRun]));
    gROOT->cd();

    phHitsPerSpill[uRun] = new TH1D();
    tempH1               = (TH1*) (pFile[uRun]->FindObjectAny("hHitsPerSpill"));
    if (NULL != tempH1) {
      tempH1->Copy(*(phHitsPerSpill[uRun]));  ///?
    }                                         //  if( NULL != tempH1 )
    else
      return kFALSE;

    phHitCntEvo[uRun] = new TH1D();
    tempH1            = (TH1*) (pFile[uRun]->FindObjectAny("hHitCntEvo"));
    if (NULL != tempH1) {
      tempH1->Copy(*(phHitCntEvo[uRun]));  ///?
    }                                      //  if( NULL != tempH1 )
    else
      return kFALSE;
  }  // for( UInt_t uRun = 0; uRun < kuNbRuns; ++ uRun )

  /// Plotting: THStacks with 4 runs per pad, 3*2 Pads per Canvas, 1 Canvas per plot
  TCanvas* cHitsSpill = new TCanvas("cHitsSpill", "T0 hits per spill");
  cHitsSpill->Divide(3, 2);

  TCanvas* cHitsCnt = new TCanvas("cHitsCnt", "T0 hits count vs Time in Run");
  cHitsCnt->Divide(3, 2);

  TCanvas* cHitsSpillAll = new TCanvas("cHitsSpillAll", "T0 hits per spill");
  cHitsSpillAll->Divide(6, 4);

  TCanvas* cHitsCntAll = new TCanvas("cHitsCntAll", "T0 hits count vs Time in Run");
  cHitsCntAll->Divide(6, 4);

  THStack* pStacksHitSpill[kuNbRuns / kuNbRunPerStack + (kuNbRuns % kuNbRunPerStack ? 1 : 0)];
  THStack* pStacksHitEvo[kuNbRuns / kuNbRunPerStack + (kuNbRuns % kuNbRunPerStack ? 1 : 0)];
  for (UInt_t uRun = 0; uRun < kuNbRuns; ++uRun) {
    if (0 == uRun % kuNbRunPerStack) {
      pStacksHitSpill[uRun / kuNbRunPerStack] = new THStack(
        Form("stackHitSpill_%02u", uRunId[uRun]),
        Form("Evolution of T0 hits per spill for runs %02u to %02u", uRunId[uRun], uRunId[uRun + kuNbRunPerStack - 1]));
      pStacksHitEvo[uRun / kuNbRunPerStack] =
        new THStack(Form("stackHitCnt_%02u", uRunId[uRun]), Form("Evolution of To hits count for runs %02u to %02u",
                                                                 uRunId[uRun], uRunId[uRun + kuNbRunPerStack - 1]));
    }  // if( 0 == uRun % kuNbRunPerStack )
    switch (uRun % kuNbRunPerStack) {
      case 0:
        phHitsPerSpill[uRun]->SetLineColor(kBlack);
        phHitCntEvo[uRun]->SetLineColor(kBlack);
        break;
      case 1:
        phHitsPerSpill[uRun]->SetLineColor(kRed);
        phHitCntEvo[uRun]->SetLineColor(kRed);
        break;
      case 2:
        phHitsPerSpill[uRun]->SetLineColor(kBlue);
        phHitCntEvo[uRun]->SetLineColor(kBlue);
        break;
      case 3:
        phHitsPerSpill[uRun]->SetLineColor(kViolet);
        phHitCntEvo[uRun]->SetLineColor(kViolet);
        break;
    }  // switch( uRun % kuNbRunPerStack )
    pStacksHitSpill[uRun / kuNbRunPerStack]->Add(phHitsPerSpill[uRun]);
    pStacksHitEvo[uRun / kuNbRunPerStack]->Add(phHitCntEvo[uRun]);

    cHitsSpill->cd(1 + uRun / kuNbRunPerStack);
    gPad->SetGridx();
    gPad->SetGridy();
    pStacksHitSpill[uRun / kuNbRunPerStack]->Draw("nostack,hist");
    pStacksHitSpill[uRun / kuNbRunPerStack]->GetXaxis()->SetRangeUser(0.0, 200.0);

    cHitsCnt->cd(1 + uRun / kuNbRunPerStack);
    gPad->SetGridx();
    gPad->SetGridy();
    pStacksHitEvo[uRun / kuNbRunPerStack]->Draw("nostack,hist");
    pStacksHitEvo[uRun / kuNbRunPerStack]->GetXaxis()->SetRangeUser(0.0, 1400.0);

    cHitsSpillAll->cd(1 + uRun);
    gPad->SetGridx();
    gPad->SetGridy();
    phHitsPerSpill[uRun]->Draw("hist");
    phHitsPerSpill[uRun]->GetXaxis()->SetRangeUser(0.0, 200.0);

    cHitsCntAll->cd(1 + uRun);
    gPad->SetGridx();
    gPad->SetGridy();
    phHitCntEvo[uRun]->Draw("hist");
    phHitCntEvo[uRun]->GetXaxis()->SetRangeUser(0.0, 1400.0);
  }  // for( UInt_t uRun = 0; uRun < kuNbRuns; ++ uRun )


  return kTRUE;
}
