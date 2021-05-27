
Bool_t DataRateMoniPlot()
{
  const UInt_t kuNbFlimLinks = 12;

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
  TProfile* tempProf = NULL;
  TProfile* phMsSzTime[kuNbRuns][kuNbFlimLinks];
  TFile* pFile[kuNbRuns];
  for (UInt_t uRun = 0; uRun < kuNbRuns; ++uRun) {
    pFile[uRun] = TFile::Open(Form("data/HistosDataRates_%03u.root", uRunId[uRun]));
    gROOT->cd();

    for (UInt_t uLink = 0; uLink < kuNbFlimLinks; ++uLink) {
      phMsSzTime[uRun][uLink] = new TProfile();
      tempProf                = (TProfile*) (pFile[uRun]->FindObjectAny(Form("MsSzTime_link_%02u", uLink)));
      if (NULL != tempProf) {
        tempProf->Copy(*(phMsSzTime[uRun][uLink]));  ///?
      }                                              //  if( NULL != tempH1 )
      else
        phMsSzTime[uRun][uLink] = nullptr;
    }  // for( UInt_t uLink = 0; uLink < kuNbFlimLinks; ++uLink )
  }    // for( UInt_t uRun = 0; uRun < kuNbRuns; ++ uRun )

  /// Plotting: THStacks with 4 runs per pad, 3*2 Pads per Canvas, 1 Canvas per plot
  TCanvas* cMsSzTime[kuNbRuns];

  for (UInt_t uRun = 0; uRun < kuNbRuns; ++uRun) {
    cMsSzTime[uRun] = new TCanvas(Form("cMsSzTime_%03u", uRunId[uRun]),
                                  Form("Mean MS size in Bytes per link in run %3u", uRunId[uRun]));
    cMsSzTime[uRun]->Divide(4, 4);

    for (UInt_t uLink = 0; uLink < kuNbFlimLinks; ++uLink) {
      if (nullptr == phMsSzTime[uRun][uLink]) continue;

      cMsSzTime[uRun]->cd(1 + uLink);
      gPad->SetGridx();
      gPad->SetGridy();
      phMsSzTime[uRun][uLink]->Draw("hist");
    }  // for( UInt_t uLink = 0; uLink < kuNbFlimLinks; ++uLink )

    cMsSzTime[uRun]->SaveAs(Form("data/cMsSzTime_%03u.png", uRunId[uRun]));
  }  // for( UInt_t uRun = 0; uRun < kuNbRuns; ++ uRun )


  return kTRUE;
}
