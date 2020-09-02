void save_canvas_to_file(TFile* outf, TCanvas* canv, TString runCase) {
  outf->cd();
  gDirectory->mkdir(runCase);
  gDirectory->cd(runCase);
  canv->Write();
  gDirectory->cd("..");
}

void save_plot_to_pdf(TH1* plot,
                      TCanvas* canv,
                      TString drawOption,
                      TString runCase,
                      UInt_t* uNbChanFixed,
                      UInt_t* uNbChanScan,
                      UInt_t uFile) {
  TCanvas* canvTmp = new TCanvas(
    canv->GetName()
      + TString(Form("_%uch_%uch", uNbChanFixed[uFile], uNbChanScan[uFile])),
    canv->GetTitle()
      + TString(
          Form(", %u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile])));
  gStyle->SetOptStat(0);
  gStyle->SetPalette(105);
  plot->GetXaxis()->SetRangeUser(-1000, 996);
  gPad->SetGridx();
  gPad->SetGridy();
  plot->Draw(drawOption);
  //canvTmp->SaveAs(TString("plotsMoreCondMain/") + runCase + TString(plot->GetName()) + TString(
  canvTmp->SaveAs(
    TString("plots5elinks/") + runCase + TString(plot->GetName())
    + TString(Form("_%uch-%uch.pdf", uNbChanFixed[uFile], uNbChanScan[uFile])));
  delete canvTmp;
}

void save_plotstack_to_pdf(THStack* plot,
                           TCanvas* canv,
                           TString drawOption,
                           TString runCase,
                           UInt_t* uNbChanFixed,
                           UInt_t* uNbChanScan,
                           UInt_t uFile) {
  TCanvas* canvTmp = new TCanvas(
    canv->GetName()
      + TString(Form("_%uch_%uch", uNbChanFixed[uFile], uNbChanScan[uFile])),
    canv->GetTitle()
      + TString(
          Form(", %u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile])));
  gStyle->SetOptStat(0);
  gStyle->SetPalette(105);
  //plot->GetXaxis()->SetRangeUser(-1000, 996);
  gPad->SetGridx();
  gPad->SetGridy();
  plot->Draw(drawOption);
  //canvTmp->SaveAs(TString("plotsMoreCondMain/") + runCase + TString(plot->GetName()) + TString(
  canvTmp->SaveAs(
    TString("plots5elinks/") + runCase + TString(plot->GetName())
    + TString(Form("_%uch-%uch.pdf", uNbChanFixed[uFile], uNbChanScan[uFile])));
  delete canvTmp;
}

void save_canvas_to_pdf(TCanvas* canv, TString runCase) {
  canv->SaveAs(TString("canvases5eLinks/") + runCase + TString(canv->GetName())
               + TString(".pdf"));
  //canv->SaveAs(TString("canvasesMoreCondMain/") + runCase + TString(canv->GetName()) + TString(".pdf") );
}

Bool_t analyseTimeCoincidenceBugCase(const UInt_t kuNbFiles,
                                     const UInt_t kuNbStats,
                                     TString* sFilename,
                                     UInt_t* uNbChanFixed,
                                     UInt_t* uNbChanScan,
                                     TString* sStats,
                                     TString runCase,
                                     TFile* outf) {
  TFile* pFile[kuNbFiles];

  /// Scaling function, the constant should lead to a ratio of ~1 for the 1st bins
  Double_t hitsExpected = 1 / 500.0;

  /// Declaration of plotting variable
  TH2D* phHodoChanRateEvoDouble[kuNbFiles];
  TH2D* phChanRatioEvoActive[kuNbFiles];
  TH2D* phCoincWinFlagEvo[kuNbFiles];
  TH2D* phNbFiredChanPerMs[kuNbFiles];
  TH2D* phNbMultiHitChanPerMs[kuNbFiles];
  TH1D* phHodoChanRateEvoProj[kuNbFiles];
  TH1D* phHodoChanRateEvoProjY[kuNbFiles];
  TH1D* phHodoChanRateEvoFlag[kuNbFiles];

  TProfile* ppCoincidenceWindow[kuNbFiles];
  TProfile* ppCoincidenceMin[kuNbFiles];
  TProfile* ppCoincidenceMax[kuNbFiles];
  TProfile* ppCoincidenceMean[kuNbFiles];
  TProfile* ppCoincidenceFlag[kuNbFiles];
  TProfile* ppNbFiredChanPerMs[kuNbFiles];
  TProfile* ppNbMultiHitChanPerMs[kuNbFiles];
  THStack* stackCoincMinMax[kuNbFiles];

  TProfile2D* pppDelayCaseRatio[kuNbStats];
  /// 0: min, 1: mean, 2: max
  /// X: Time delay, Y: Run case, Z: Hits ratio
  /// TString sStats[ kuNbStats ] = { "min", "mean", "max" };

  TProfile2D* pppNbFiredChanPerMs;
  TProfile2D* pppNbMissingChanPerMs;
  TProfile2D* pppNbMultiHitChanPerMs;
  TProfile2D* pppChanRatioEvoActive[kuNbFiles];

  /// Delay scan parameters
  Double_t dDistMin          = -1000.0;
  Double_t dDistMax          = 1000.0;
  Double_t dDistStep         = 4.0;
  UInt_t uNbBinsDist         = (dDistMax - dDistMin) / dDistStep;
  Double_t dDistStepDuration = 2.0;  /// second

  /// canvas division
  Int_t canvCols = 5;
  Int_t canvRows = 3;

  /// Display canvases preparation
  TCanvas* cRatioEvo = new TCanvas(
    "cRatioEvo", "Evolution of hits ratio, 1 = all and only pulser hits there");
  cRatioEvo->Divide(canvCols, canvRows);

  /*TCanvas * cRatioEvoActive = new TCanvas( "cRatioEvoActive",
                                      "Evolution of hits ratio, 1 = all and only pulser hits there" );
   cRatioEvoActive->Divide( canvCols, canvRows );*/

  TCanvas* cpRatioEvoActive =
    new TCanvas("cpRatioEvoActive",
                "Evolution of hits ratio, 1 = all and only pulser hits there");
  cpRatioEvoActive->Divide(canvCols, canvRows);

  TCanvas* cRatioProj = new TCanvas(
    "cRatioProj",
    "Evolution of hits ratio projection, 12 = all and only pulser hits there");
  cRatioProj->Divide(canvCols, canvRows);

  TCanvas* cCoincWin = new TCanvas(
    "cCoincWin",
    "Evolution of hits ratio projection VS distance between the pulse groups");
  cCoincWin->Divide(canvCols, canvRows);

  TCanvas* cCoincWinMin =
    new TCanvas("cCoincWinMin",
                "Evolution of minimum hits ratio projection VS distance "
                "between the pulse groups");
  cCoincWinMin->Divide(canvCols, canvRows);

  TCanvas* cCoincWinMax =
    new TCanvas("cCoincWinMax",
                "Evolution of maximum hits ratio projection VS distance "
                "between the pulse groups");
  cCoincWinMax->Divide(canvCols, canvRows);

  TCanvas* cCoincWinMinMax =
    new TCanvas("cCoincWinMinMax",
                "Evolution of maximum hit losses/copies VS distance between "
                "the pulse groups");
  cCoincWinMinMax->Divide(canvCols, canvRows);

  TCanvas* cProfiles = new TCanvas(
    "cProfiles", "Profiles of hits ratio VS Time delay AND Run case");
  cProfiles->Divide(canvCols, 1);

  TCanvas* cHitChanProfiles =
    new TCanvas("cHitChanProfiles",
                "Profiles of hits per channel VS Time delay AND Run case");
  cHitChanProfiles->Divide(3, 1);

  for (UInt_t uStat = 0; uStat < kuNbStats; uStat++) {
    pppDelayCaseRatio[uStat] = new TProfile2D(
      "pppDelayCaseRatio_" + sStats[uStat],
      "Profile of " + sStats[uStat]
        + " hits ratio VS time delay AND run case; Delay [ns]; run case",
      uNbBinsDist,
      dDistMin,
      dDistMax,
      kuNbFiles,
      0,
      kuNbFiles,
      0.,
      10.);
  }

  pppDelayCaseRatio[0]->GetZaxis()->SetRangeUser(0.0, 1.1);

  pppNbFiredChanPerMs =
    new TProfile2D("pppNbFiredChanPerMs",
                   "Profile of fired channels per ms VS time delay AND run "
                   "case; Delay [ns]; run case",
                   uNbBinsDist,
                   dDistMin,
                   dDistMax,
                   kuNbFiles,
                   0,
                   kuNbFiles,
                   0.,
                   12.);
  pppNbMissingChanPerMs =
    new TProfile2D("pppNbMissingChanPerMs",
                   "Profile of not fired channels per ms VS time delay AND run "
                   "case; Delay [ns]; run case",
                   uNbBinsDist,
                   dDistMin,
                   dDistMax,
                   kuNbFiles,
                   0,
                   kuNbFiles,
                   0.,
                   12.);
  pppNbMultiHitChanPerMs =
    new TProfile2D("pppNbMultiHitChanPerMs",
                   "Profile of multi hit channels per ms VS time delay AND run "
                   "case; Delay [ns]; run case",
                   uNbBinsDist,
                   dDistMin,
                   dDistMax,
                   kuNbFiles,
                   0,
                   kuNbFiles,
                   0.,
                   12.);

  /// Loop on scan cases
  TH2* tempTwoDimHist = NULL;
  for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
    pFile[uFile] = TFile::Open(sFilename[uFile]);
    gROOT->cd();

    /// Obtaining the raw plots from the input files ( and converting them to TH2D for rescaling)
    phHodoChanRateEvoDouble[uFile] = new TH2D();
    tempTwoDimHist =
      (TH2*) (pFile[uFile]->FindObjectAny("hHodoChanRateEvo_000"));
    if (NULL != tempTwoDimHist)
      tempTwoDimHist->Copy(*(phHodoChanRateEvoDouble[uFile]));  ///?
    else
      return kFALSE;

    tempTwoDimHist =
      (TH2*) (pFile[uFile]->FindObjectAny("hCoincWinFlagEvoMoreCond"));
    if (NULL != tempTwoDimHist)
      phCoincWinFlagEvo[uFile] =
        (TH2D*) (tempTwoDimHist->Clone(Form("phCoincWinFlagEvoMoreCond_%u_%u",
                                            uNbChanFixed[uFile],
                                            uNbChanScan[uFile])));
    else
      return kFALSE;

    /// Recover the NbFiredChan and NbMultiChan 2D plots
    tempTwoDimHist =
      (TH2*) (pFile[uFile]->FindObjectAny("hNbFiredChanPerMs00"));
    if (NULL != tempTwoDimHist) {
      phNbFiredChanPerMs[uFile] = (TH2D*) (tempTwoDimHist->Clone(Form(
        "phNbFiredChanPerMs_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile])));
    } else {
      return kFALSE;
    }

    tempTwoDimHist =
      (TH2*) (pFile[uFile]->FindObjectAny("hNbMultiHitChanPerMs00"));
    if (NULL != tempTwoDimHist) {
      phNbMultiHitChanPerMs[uFile] =
        (TH2D*) (tempTwoDimHist->Clone(Form("phNbMultiHitChanPerMs_%u_%u",
                                            uNbChanFixed[uFile],
                                            uNbChanScan[uFile])));
    } else {
      return kFALSE;
    }


    phHodoChanRateEvoDouble[uFile]->SetName(Form(
      "hHodoChanRateEvoDouble_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));

    /// Convert hit counts to measured to expected ratios, may also work easier with Scale!
    phHodoChanRateEvoDouble[uFile]->Scale(hitsExpected);


    /* phChanRatioEvoActive[uFile] = new TH2D( Form( "hChanRatioEvoActive_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile] ),
                                          Form( "Ratio measured hits/expected for %u fixed ch and %u scanning ch; Delay [ns]; Active chan []", uNbChanFixed[uFile], uNbChanScan[uFile] ),
                                          uNbBinsDist, dDistMin, dDistMax,
                                          12, -0.5, 11.5 );*/
    pppChanRatioEvoActive[uFile] = new TProfile2D(
      Form(
        "pppChanRatioEvoActive_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Ratio measured hits/expected for %u fixed ch and %u scanning ch; "
           "Delay [ns]; Active chan []",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]),
      uNbBinsDist,
      dDistMin,
      dDistMax,
      12,
      -0.5,
      11.5,
      0.,
      2.);

    phHodoChanRateEvoProj[uFile] =
      phHodoChanRateEvoDouble[uFile]->ProjectionX(Form(
        "hHodoChanRateEvoProj_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));
    phHodoChanRateEvoProjY[uFile] = phHodoChanRateEvoDouble[uFile]->ProjectionY(
      Form("hHodoChanRateEvoProjY_%u_%u",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]));

    /// Make Profile X of the NbFiredChan and NbMultiChan 2D plots
    ppNbFiredChanPerMs[uFile]    = phNbFiredChanPerMs[uFile]->ProfileX(Form(
      "hNbFiredChanPerMsProf_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));
    ppNbMultiHitChanPerMs[uFile] = phNbMultiHitChanPerMs[uFile]->ProfileX(
      Form("hNbMultiHitChanPerMsProf_%u_%u",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]));


    /// Displaying
    cRatioEvo->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    phHodoChanRateEvoDouble[uFile]->Draw("colz");

    cRatioProj->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    phHodoChanRateEvoProj[uFile]->Scale(
      1.0 / (uNbChanFixed[uFile] + uNbChanScan[uFile]));
    phHodoChanRateEvoProj[uFile]->Draw("hist");

    /// Prepare the profile objects
    ppCoincidenceWindow[uFile] = new TProfile(
      Form(
        "ppCoincidenceWindow_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Coincidence window with %u fixed ch and %u scanning ch; Delay [ns]",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]),
      uNbBinsDist,
      dDistMin,
      dDistMax);

    ppCoincidenceMin[uFile] = new TProfile(
      Form("ppCoincidenceMin_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Coincidence lowest hit ratio with %u fixed ch and %u scanning ch; "
           "Delay [ns]; Ratio",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]),
      uNbBinsDist,
      dDistMin,
      dDistMax);

    ppCoincidenceMax[uFile] = new TProfile(
      Form("ppCoincidenceMax_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Coincidence highest hit ratio with %u fixed ch and %u scanning ch; "
           "Delay [ns]; Ratio",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]),
      uNbBinsDist,
      dDistMin,
      dDistMax);

    ppCoincidenceMean[uFile] = new TProfile(
      Form("ppCoincidenceMean_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Coincidence mean hit ratio with %u fixed ch and %u scanning ch; "
           "Delay [ns]; Ratio",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]),
      uNbBinsDist,
      dDistMin,
      dDistMax);


    ppCoincidenceFlag[uFile] = new TProfile(
      Form("ppCoincidenceFlag_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Coincidence flag with %u fixed ch and %u scanning ch; Delay [ns]",
           uNbChanFixed[uFile],
           uNbChanScan[uFile]),
      uNbBinsDist,
      dDistMin,
      dDistMax);

    /// Loop on the time in run bins (1/2s stepping) with check on the corresponding delay stepping
    for (UInt_t uBin = 1;
         uBin <= phHodoChanRateEvoProj[uFile]->GetNbinsX()
         && dDistMin
                + dDistStep * phHodoChanRateEvoProj[uFile]->GetBinCenter(uBin)
                    / dDistStepDuration
              <= dDistMax;
         ++uBin) {
      /// Delay corresponding to the bin
      Double_t dDist = dDistMin
                       + dDistStep
                           * phHodoChanRateEvoProj[uFile]->GetBinCenter(uBin)
                           / dDistStepDuration;


      ppCoincidenceWindow[uFile]->Fill(
        dDist, phHodoChanRateEvoProj[uFile]->GetBinContent(uBin));

      ppCoincidenceFlag[uFile]->Fill(
        dDist,
        phCoincWinFlagEvo[uFile]->GetBinContent(
          phCoincWinFlagEvo[uFile]->GetBin(uBin, 1)));

      /// Fill the Profile2D for NbFiredChan and NbMultiChan
      pppNbFiredChanPerMs->Fill(
        dDist, uFile, ppNbFiredChanPerMs[uFile]->GetBinContent(uBin), 1);
      pppNbMissingChanPerMs->Fill(
        dDist,
        uFile,
        uNbChanFixed[uFile] + uNbChanScan[uFile]
          - ppNbFiredChanPerMs[uFile]->GetBinContent(uBin),
        1);
      pppNbMultiHitChanPerMs->Fill(
        dDist, uFile, ppNbMultiHitChanPerMs[uFile]->GetBinContent(uBin), 1);

      /// Loop on channels
      Double_t dMinRatio  = 2000.0;
      Double_t dMaxRatio  = -2000.0;
      UInt_t uActiveChIdx = 0;
      for (UInt_t uBinY = 1;
           uBinY <= phHodoChanRateEvoDouble[uFile]->GetNbinsY();
           ++uBinY) {
        /// check if channels is active
        if (1 < phHodoChanRateEvoProjY[uFile]->GetBinContent(uBinY)) {
          /// Get the ratio for this channel and delay and check if it is the minimum/maximum
          Double_t dHitRatio = phHodoChanRateEvoDouble[uFile]->GetBinContent(
            phHodoChanRateEvoDouble[uFile]->GetBin(uBin, uBinY));
          if (dHitRatio < dMinRatio) dMinRatio = dHitRatio;

          if (dMaxRatio < dHitRatio) dMaxRatio = dHitRatio;

          /// Fill the mean ratio plot
          ppCoincidenceMean[uFile]->Fill(dDist, dHitRatio);
          pppDelayCaseRatio[1]->Fill(dDist, uFile, dHitRatio, 1);

          //phChanRatioEvoActive[uFile]->Fill( dDist, uActiveChIdx, dHitRatio);
          pppChanRatioEvoActive[uFile]->Fill(dDist, uActiveChIdx, dHitRatio, 1);

          uActiveChIdx++;
        }  // if( 0 < phHodoChanRateEvoProjY[uFile]->GetBinContent( uBinY ) )
      }  //for( UInt_t uBinY = 1; uBinY <= phHodoChanRateEvoDouble[uFile]->GetNbinsY(); ++ uBinY )

      /// fill the minimal/maximal plots
      if (2000.0 != dMinRatio) {
        ppCoincidenceMin[uFile]->Fill(dDist, dMinRatio);
        pppDelayCaseRatio[0]->Fill(dDist, uFile, dMinRatio, 1);
      }
      if (-2000.0 != dMaxRatio) {
        ppCoincidenceMax[uFile]->Fill(dDist, dMaxRatio);
        pppDelayCaseRatio[2]->Fill(dDist, uFile, dMaxRatio, 1);
      }
    }  // loop on bins until end or out of coincidence window

    /// Displaying
    /* cRatioEvoActive->cd( 1 + uFile );
      gPad->SetGridx();
      gPad->SetGridy();
      phChanRatioEvoActive[ uFile ]->Draw( "colz" );
      /// Save plot to Png
      save_plot_to_pdf(phChanRatioEvoActive[ uFile ],
                               cRatioEvoActive,
                               "colz",
                               runCase,
                               uNbChanFixed,
                               uNbChanScan,
                               uFile);*/

    cpRatioEvoActive->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    pppChanRatioEvoActive[uFile]->Draw("colz");
    /// Save plot to Png
    save_plot_to_pdf(pppChanRatioEvoActive[uFile],
                     cpRatioEvoActive,
                     "colz",
                     runCase,
                     uNbChanFixed,
                     uNbChanScan,
                     uFile);


    cCoincWin->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    ppCoincidenceWindow[uFile]->Draw();
    save_plot_to_pdf(ppCoincidenceWindow[uFile],
                     cCoincWin,
                     "",
                     runCase,
                     uNbChanFixed,
                     uNbChanScan,
                     uFile);

    cCoincWinMin->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    ppCoincidenceMin[uFile]->Draw("hist e0");
    save_plot_to_pdf(ppCoincidenceMin[uFile],
                     cCoincWinMin,
                     "hist e0",
                     runCase,
                     uNbChanFixed,
                     uNbChanScan,
                     uFile);

    cCoincWinMax->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    ppCoincidenceMax[uFile]->Draw("hist e0");
    save_plot_to_pdf(ppCoincidenceMax[uFile],
                     cCoincWinMax,
                     "hist e0",
                     runCase,
                     uNbChanFixed,
                     uNbChanScan,
                     uFile);

    cCoincWinMinMax->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();

    stackCoincMinMax[uFile] =
      new THStack(Form("stackCoincMinMax_%d", uFile),
                  Form("Evolution of maximum hit losses/copies VS distance "
                       "between the %u ch group and the %u ch one; Time delay "
                       "[ns]; (observed hits)/(expected hits) ratio",
                       uNbChanFixed[uFile],
                       uNbChanScan[uFile]));

    ppCoincidenceFlag[uFile]->SetLineColor(kGray);
    ppCoincidenceFlag[uFile]->SetFillColor(kGray);
    ppCoincidenceFlag[uFile]->SetFillStyle(3345);
    stackCoincMinMax[uFile]->Add(ppCoincidenceFlag[uFile]);

    ppCoincidenceMin[uFile]->SetLineColor(kRed);
    stackCoincMinMax[uFile]->Add(ppCoincidenceMin[uFile]);

    ppCoincidenceMean[uFile]->SetLineColor(kGreen);
    stackCoincMinMax[uFile]->Add(ppCoincidenceMean[uFile]);

    //      ppCoincidenceMax[ uFile ]->SetLineColor( kBlack );
    stackCoincMinMax[uFile]->Add(ppCoincidenceMax[uFile]);

    stackCoincMinMax[uFile]->Draw("nostack,histe0");

    save_plotstack_to_pdf(stackCoincMinMax[uFile],
                          cCoincWinMinMax,
                          "nostack,hist e0",
                          runCase,
                          uNbChanFixed,
                          uNbChanScan,
                          uFile);

  }  // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++ )

  /// Save canvases to file
  //save_canvas_to_file(outf, cRatioEvoActive, runCase);
  save_canvas_to_file(outf, cpRatioEvoActive, runCase);
  save_canvas_to_file(outf, cCoincWin, runCase);
  save_canvas_to_file(outf, cCoincWinMin, runCase);
  save_canvas_to_file(outf, cCoincWinMax, runCase);
  save_canvas_to_file(outf, cCoincWinMinMax, runCase);


  /// Save canvases to png <=  in "plots" folder
  //save_canvas_to_pdf(cRatioEvoActive, runCase);
  save_canvas_to_pdf(cpRatioEvoActive, runCase);
  save_canvas_to_pdf(cCoincWin, runCase);
  save_canvas_to_pdf(cCoincWinMin, runCase);
  save_canvas_to_pdf(cCoincWinMax, runCase);
  save_canvas_to_pdf(cCoincWinMinMax, runCase);

  /// set bin labels for profile2d
  for (UInt_t uStat = 0; uStat < kuNbStats; uStat++) {
    for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
      pppDelayCaseRatio[uStat]->GetYaxis()->SetBinLabel(
        uFile + 1,
        Form("%d ch, %d ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
    }
  }

  for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
    pppNbFiredChanPerMs->GetYaxis()->SetBinLabel(
      uFile + 1, Form("%u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
    pppNbMissingChanPerMs->GetYaxis()->SetBinLabel(
      uFile + 1, Form("%u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
    pppNbMultiHitChanPerMs->GetYaxis()->SetBinLabel(
      uFile + 1, Form("%u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
  }


  /// plot profiles2D
  for (UInt_t uStat = 0; uStat < kuNbStats; uStat++) {
    cProfiles->cd(uStat + 1);
    gStyle->SetOptStat(0);
    gPad->SetGridx();
    gPad->SetGridy();
    gStyle->SetPalette(105);
    pppDelayCaseRatio[uStat]->Draw("COLZ");
    save_plot_to_pdf(pppDelayCaseRatio[uStat],
                     cProfiles,
                     "COLZ",
                     runCase,
                     uNbChanFixed,
                     uNbChanScan,
                     uStat);
  }
  save_canvas_to_file(outf, cProfiles, runCase);

  cHitChanProfiles->cd(1);
  gStyle->SetOptStat(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gStyle->SetPalette(105);
  pppNbFiredChanPerMs->Draw("COLZ");
  save_plot_to_pdf(pppNbFiredChanPerMs,
                   cHitChanProfiles,
                   "COLZ",
                   runCase,
                   uNbChanFixed,
                   uNbChanScan,
                   1);

  cHitChanProfiles->cd(2);
  gStyle->SetOptStat(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gStyle->SetPalette(105);
  pppNbMissingChanPerMs->Draw("COLZ");
  save_plot_to_pdf(pppNbMissingChanPerMs,
                   cHitChanProfiles,
                   "COLZ",
                   runCase,
                   uNbChanFixed,
                   uNbChanScan,
                   2);

  cHitChanProfiles->cd(3);
  gStyle->SetOptStat(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gStyle->SetPalette(105);
  pppNbMultiHitChanPerMs->Draw("COLZ");
  save_plot_to_pdf(pppNbMultiHitChanPerMs,
                   cHitChanProfiles,
                   "COLZ",
                   runCase,
                   uNbChanFixed,
                   uNbChanScan,
                   3);
  save_canvas_to_file(outf, cHitChanProfiles, runCase);

  delete cRatioEvo;
  delete cpRatioEvoActive;
  delete cRatioProj;
  delete cCoincWin;
  delete cCoincWinMin;
  delete cCoincWinMax;
  delete cCoincWinMinMax;
  delete cProfiles;
  delete cHitChanProfiles;

  for (UInt_t uStat = 0; uStat < kuNbStats; uStat++)
    delete pppDelayCaseRatio[uStat];
  delete pppNbFiredChanPerMs;
  delete pppNbMissingChanPerMs;
  delete pppNbMultiHitChanPerMs;

  return kTRUE;
}

// n + m = 16 cases
Bool_t AnalyseTimeCoincidenceBug16(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 15;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0088_20181010_1230.root",
                                  "data/PulserHistos_r0089_20181010_1247.root",
                                  "data/PulserHistos_r0090_20181010_1304.root",
                                  "data/PulserHistos_r0091_20181010_1321.root",
                                  "data/PulserHistos_r0092_20181010_1338.root",
                                  "data/PulserHistos_r0093_20181010_1355.root",
                                  "data/PulserHistos_r0094_20181010_1412.root",
                                  "data/PulserHistos_r0095_20181010_1429.root",
                                  "data/PulserHistos_r0096_20181010_1446.root",
                                  "data/PulserHistos_r0097_20181010_1503.root",
                                  "data/PulserHistos_r0098_20181010_1520.root",
                                  "data/PulserHistos_r0099_20181010_1616.root",
                                  "data/PulserHistos_r0100_20181010_1633.root",
                                  "data/PulserHistos_r0101_20181010_1650.root",
                                  "data/PulserHistos_r0102_20181010_1707.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  UInt_t uNbChanScan[kuNbFiles] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "16tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 17 cases
Bool_t AnalyseTimeCoincidenceBug17(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 14;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0103_20181010_1724.root",
                                  "data/PulserHistos_r0104_20181010_1741.root",
                                  "data/PulserHistos_r0105_20181010_1758.root",
                                  "data/PulserHistos_r0106_20181010_1815.root",
                                  "data/PulserHistos_r0107_20181010_1832.root",
                                  "data/PulserHistos_r0108_20181010_1849.root",
                                  "data/PulserHistos_r0109_20181010_1906.root",
                                  "data/PulserHistos_r0110_20181010_1923.root",
                                  "data/PulserHistos_r0111_20181010_1940.root",
                                  "data/PulserHistos_r0112_20181010_1957.root",
                                  "data/PulserHistos_r0113_20181010_2014.root",
                                  "data/PulserHistos_r0114_20181010_2031.root",
                                  "data/PulserHistos_r0115_20181010_2048.root",
                                  "data/PulserHistos_r0116_20181011_0842.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
  UInt_t uNbChanScan[kuNbFiles] = {
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "17tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 18 cases
Bool_t AnalyseTimeCoincidenceBug18(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 13;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0117_20181011_0859.root",
                                  "data/PulserHistos_r0118_20181011_0916.root",
                                  "data/PulserHistos_r0119_20181011_0933.root",
                                  "data/PulserHistos_r0120_20181011_0950.root",
                                  "data/PulserHistos_r0121_20181011_1007.root",
                                  "data/PulserHistos_r0122_20181011_1024.root",
                                  "data/PulserHistos_r0123_20181011_1041.root",
                                  "data/PulserHistos_r0124_20181011_1058.root",
                                  "data/PulserHistos_r0125_20181011_1115.root",
                                  "data/PulserHistos_r0126_20181011_1132.root",
                                  "data/PulserHistos_r0127_20181011_1149.root",
                                  "data/PulserHistos_r0128_20181011_1206.root",
                                  "data/PulserHistos_r0129_20181011_1223.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3};
  UInt_t uNbChanScan[kuNbFiles] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "18tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 19 cases
Bool_t AnalyseTimeCoincidenceBug19(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 12;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0130_20181011_1239.root",
                                  "data/PulserHistos_r0131_20181011_1257.root",
                                  "data/PulserHistos_r0132_20181011_1314.root",
                                  "data/PulserHistos_r0133_20181011_1331.root",
                                  "data/PulserHistos_r0134_20181011_1348.root",
                                  "data/PulserHistos_r0135_20181011_1405.root",
                                  "data/PulserHistos_r0136_20181011_1421.root",
                                  "data/PulserHistos_r0137_20181011_1438.root",
                                  "data/PulserHistos_r0138_20181011_1455.root",
                                  "data/PulserHistos_r0139_20181011_1512.root",
                                  "data/PulserHistos_r0140_20181011_1529.root",
                                  "data/PulserHistos_r0141_20181011_1546.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
  UInt_t uNbChanScan[kuNbFiles]  = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "19tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 20 cases
Bool_t AnalyseTimeCoincidenceBug20(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 11;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0142_20181011_1605.root",
                                  "data/PulserHistos_r0143_20181011_1622.root",
                                  "data/PulserHistos_r0144_20181011_1639.root",
                                  "data/PulserHistos_r0145_20181011_1657.root",
                                  "data/PulserHistos_r0146_20181011_1713.root",
                                  "data/PulserHistos_r0147_20181011_1730.root",
                                  "data/PulserHistos_r0148_20181011_1747.root",
                                  "data/PulserHistos_r0149_20181011_1804.root",
                                  "data/PulserHistos_r0150_20181011_1821.root",
                                  "data/PulserHistos_r0151_20181011_1838.root",
                                  "data/PulserHistos_r0152_20181011_1855.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5};
  UInt_t uNbChanScan[kuNbFiles]  = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "20tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 21 cases
Bool_t AnalyseTimeCoincidenceBug21(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 10;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0153_20181011_1912.root",
                                  "data/PulserHistos_r0154_20181011_1929.root",
                                  "data/PulserHistos_r0155_20181011_1946.root",
                                  "data/PulserHistos_r0156_20181011_2003.root",
                                  "data/PulserHistos_r0157_20181011_2020.root",
                                  "data/PulserHistos_r0158_20181011_2037.root",
                                  "data/PulserHistos_r0159_20181011_2054.root",
                                  "data/PulserHistos_r0160_20181011_2111.root",
                                  "data/PulserHistos_r0161_20181011_2128.root",
                                  "data/PulserHistos_r0162_20181011_2145.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6};
  UInt_t uNbChanScan[kuNbFiles]  = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "21tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 22 cases
Bool_t AnalyseTimeCoincidenceBug22(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 9;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0163_20181011_2202.root",
                                  "data/PulserHistos_r0164_20181011_2219.root",
                                  "data/PulserHistos_r0165_20181011_2236.root",
                                  "data/PulserHistos_r0166_20181011_2253.root",
                                  "data/PulserHistos_r0167_20181011_2310.root",
                                  "data/PulserHistos_r0168_20181011_2327.root",
                                  "data/PulserHistos_r0169_20181011_2344.root",
                                  "data/PulserHistos_r0170_20181012_0001.root",
                                  "data/PulserHistos_r0171_20181012_0018.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8, 7};
  UInt_t uNbChanScan[kuNbFiles]  = {7, 8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "22tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 23 cases
Bool_t AnalyseTimeCoincidenceBug23(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 8;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0172_20181012_0035.root",
                                  "data/PulserHistos_r0173_20181012_0052.root",
                                  "data/PulserHistos_r0174_20181012_0109.root",
                                  "data/PulserHistos_r0175_20181012_0126.root",
                                  "data/PulserHistos_r0176_20181012_0143.root",
                                  "data/PulserHistos_r0177_20181012_0200.root",
                                  "data/PulserHistos_r0178_20181012_0217.root",
                                  "data/PulserHistos_r0179_20181012_0234.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9, 8};
  UInt_t uNbChanScan[kuNbFiles]  = {8, 9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "23tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 24 cases
Bool_t AnalyseTimeCoincidenceBug24(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 7;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0180_20181012_0251.root",
                                  "data/PulserHistos_r0181_20181012_0308.root",
                                  "data/PulserHistos_r0182_20181012_0325.root",
                                  "data/PulserHistos_r0183_20181012_0342.root",
                                  "data/PulserHistos_r0184_20181012_0359.root",
                                  "data/PulserHistos_r0185_20181012_0416.root",
                                  "data/PulserHistos_r0186_20181012_1330.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10, 9};
  UInt_t uNbChanScan[kuNbFiles]  = {9, 10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "24tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 25 cases
Bool_t AnalyseTimeCoincidenceBug25(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 6;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0187_20181012_1347.root",
                                  "data/PulserHistos_r0188_20181012_1404.root",
                                  "data/PulserHistos_r0189_20181012_1421.root",
                                  "data/PulserHistos_r0190_20181012_1438.root",
                                  "data/PulserHistos_r0191_20181012_1455.root",
                                  "data/PulserHistos_r0192_20181012_1512.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11, 10};
  UInt_t uNbChanScan[kuNbFiles]  = {10, 11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "25tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 26 cases
Bool_t AnalyseTimeCoincidenceBug26(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 5;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0193_20181012_1528.root",
                                  "data/PulserHistos_r0194_20181012_1546.root",
                                  "data/PulserHistos_r0195_20181012_1603.root",
                                  "data/PulserHistos_r0196_20181012_1620.root",
                                  "data/PulserHistos_r0197_20181012_1636.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12, 11};
  UInt_t uNbChanScan[kuNbFiles]  = {11, 12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "26tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 27 cases
Bool_t AnalyseTimeCoincidenceBug27(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 4;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0198_20181012_1653.root",
                                  "data/PulserHistos_r0199_20181012_1710.root",
                                  "data/PulserHistos_r0200_20181012_1727.root",
                                  "data/PulserHistos_r0201_20181012_2250.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13, 12};
  UInt_t uNbChanScan[kuNbFiles]  = {12, 13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "27tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 28 cases
Bool_t AnalyseTimeCoincidenceBug28(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 3;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0202_20181012_2057.root",
                                  "data/PulserHistos_r0203_20181012_2114.root",
                                  "data/PulserHistos_r0204_20181012_2131.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14, 13};
  UInt_t uNbChanScan[kuNbFiles]  = {13, 14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "28tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 29 cases
Bool_t AnalyseTimeCoincidenceBug29(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 2;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0205_20181012_2148.root",
                                  "data/PulserHistos_r0206_20181012_2205.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15, 14};
  UInt_t uNbChanScan[kuNbFiles]  = {14, 15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "29tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

// n + m = 30 cases
Bool_t AnalyseTimeCoincidenceBug30(TFile* outf) {
  /// Data source parameters
  const UInt_t kuNbFiles       = 1;
  const UInt_t kuNbStats       = 3;
  TString sFilename[kuNbFiles] = {"data/PulserHistos_r0207_20181012_2222.root"};
  UInt_t uNbChanFixed[kuNbFiles] = {15};
  UInt_t uNbChanScan[kuNbFiles]  = {15};

  TString sStats[kuNbStats] = {"min", "mean", "max"};
  TString runCase           = "30tot";
  return analyseTimeCoincidenceBugCase(kuNbFiles,
                                       kuNbStats,
                                       sFilename,
                                       uNbChanFixed,
                                       uNbChanScan,
                                       sStats,
                                       runCase,
                                       outf);
}

Bool_t AnalyseTimeCoincidenceBugAllCases5elinks(
  TString sFilename = "STSXyterPulserNoiseCond_5Elinks.root") {
  /// Open output root file
  TDirectory* oldDir = gDirectory;
  TFile* histoFile   = new TFile(sFilename, "RECREATE");
  if (NULL == histoFile) return kFALSE;

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

  /// Close the file
  histoFile->Close();
  oldDir->cd();

  return kTRUE;
}
