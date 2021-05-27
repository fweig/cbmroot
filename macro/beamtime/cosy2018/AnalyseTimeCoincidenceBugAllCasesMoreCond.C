void save_canvas_to_file(TFile* outf, TCanvas* canv, TString runCase)
{
  outf->cd();
  gDirectory->mkdir(runCase);
  gDirectory->cd(runCase);
  canv->Write();
  gDirectory->cd("..");
}

void save_plot_to_pdf(TH1* plot, TCanvas* canv, TString drawOption, TString runCase, UInt_t* uNbChanFixed,
                      UInt_t* uNbChanScan, UInt_t uFile)
{
  TCanvas* canvTmp =
    new TCanvas(canv->GetName() + TString(Form("_%uch_%uch", uNbChanFixed[uFile], uNbChanScan[uFile])),
                canv->GetTitle() + TString(Form(", %u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile])));
  gStyle->SetOptStat(0);
  gStyle->SetPalette(105);
  plot->GetXaxis()->SetRangeUser(-1000, 996);
  gPad->SetGridx();
  gPad->SetGridy();
  plot->Draw(drawOption);
  //canvTmp->SaveAs(TString("plotsMoreCondMain/") + runCase + TString(plot->GetName()) + TString(
  canvTmp->SaveAs(TString("plotsMoreCondNoise/") + runCase + TString(plot->GetName())
                  + TString(Form("_%uch-%uch.pdf", uNbChanFixed[uFile], uNbChanScan[uFile])));
  delete canvTmp;
}

void save_plotstack_to_pdf(THStack* plot, TCanvas* canv, TString drawOption, TString runCase, UInt_t* uNbChanFixed,
                           UInt_t* uNbChanScan, UInt_t uFile)
{
  TCanvas* canvTmp =
    new TCanvas(canv->GetName() + TString(Form("_%uch_%uch", uNbChanFixed[uFile], uNbChanScan[uFile])),
                canv->GetTitle() + TString(Form(", %u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile])));
  gStyle->SetOptStat(0);
  gStyle->SetPalette(105);
  //plot->GetXaxis()->SetRangeUser(-1000, 996);
  gPad->SetGridx();
  gPad->SetGridy();
  plot->Draw(drawOption);
  //canvTmp->SaveAs(TString("plotsMoreCondMain/") + runCase + TString(plot->GetName()) + TString(
  canvTmp->SaveAs(TString("plotsMoreCondNoise/") + runCase + TString(plot->GetName())
                  + TString(Form("_%uch-%uch.pdf", uNbChanFixed[uFile], uNbChanScan[uFile])));
  delete canvTmp;
}

void save_canvas_to_pdf(TCanvas* canv, TString runCase)
{
  canv->SaveAs(TString("canvasesMoreCondNoise/") + runCase + TString(canv->GetName()) + TString(".pdf"));
  //canv->SaveAs(TString("canvasesMoreCondMain/") + runCase + TString(canv->GetName()) + TString(".pdf") );
}

Bool_t analyseTimeCoincidenceBugCase(const UInt_t kuNbFiles, const UInt_t kuNbStats, TString* sFilename,
                                     UInt_t* uNbChanFixed, UInt_t* uNbChanScan, TString* sStats, TString runCase,
                                     TFile* outf)
{
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
  Int_t canvCols = 3;
  Int_t canvRows = 2;

  /// Display canvases preparation
  TCanvas* cRatioEvo = new TCanvas("cRatioEvo", "Evolution of hits ratio, 1 = all and only pulser hits there");
  cRatioEvo->Divide(canvCols, canvRows);

  /*TCanvas * cRatioEvoActive = new TCanvas( "cRatioEvoActive",
                                      "Evolution of hits ratio, 1 = all and only pulser hits there" );
   cRatioEvoActive->Divide( canvCols, canvRows );*/

  TCanvas* cpRatioEvoActive =
    new TCanvas("cpRatioEvoActive", "Evolution of hits ratio, 1 = all and only pulser hits there");
  cpRatioEvoActive->Divide(canvCols, canvRows);

  TCanvas* cRatioProj =
    new TCanvas("cRatioProj", "Evolution of hits ratio projection, 12 = all and only pulser hits there");
  cRatioProj->Divide(canvCols, canvRows);

  TCanvas* cCoincWin =
    new TCanvas("cCoincWin", "Evolution of hits ratio projection VS distance between the pulse groups");
  cCoincWin->Divide(canvCols, canvRows);

  TCanvas* cCoincWinMin = new TCanvas("cCoincWinMin", "Evolution of minimum hits ratio projection VS distance "
                                                      "between the pulse groups");
  cCoincWinMin->Divide(canvCols, canvRows);

  TCanvas* cCoincWinMax = new TCanvas("cCoincWinMax", "Evolution of maximum hits ratio projection VS distance "
                                                      "between the pulse groups");
  cCoincWinMax->Divide(canvCols, canvRows);

  TCanvas* cCoincWinMinMax =
    new TCanvas("cCoincWinMinMax", "Evolution of maximum hit losses/copies VS distance between "
                                   "the pulse groups");
  cCoincWinMinMax->Divide(canvCols, canvRows);

  TCanvas* cProfiles = new TCanvas("cProfiles", "Profiles of hits ratio VS Time delay AND Run case");
  cProfiles->Divide(canvCols, 1);

  TCanvas* cHitChanProfiles =
    new TCanvas("cHitChanProfiles", "Profiles of hits per channel VS Time delay AND Run case");
  cHitChanProfiles->Divide(3, 1);

  for (UInt_t uStat = 0; uStat < kuNbStats; uStat++) {
    pppDelayCaseRatio[uStat] =
      new TProfile2D("pppDelayCaseRatio_" + sStats[uStat],
                     "Profile of " + sStats[uStat] + " hits ratio VS time delay AND run case; Delay [ns]; run case",
                     uNbBinsDist, dDistMin, dDistMax, kuNbFiles, 0, kuNbFiles, 0., 10.);
  }

  pppDelayCaseRatio[0]->GetZaxis()->SetRangeUser(0.0, 1.1);

  pppNbFiredChanPerMs    = new TProfile2D("pppNbFiredChanPerMs",
                                       "Profile of fired channels per ms VS time delay AND run "
                                       "case; Delay [ns]; run case",
                                       uNbBinsDist, dDistMin, dDistMax, kuNbFiles, 0, kuNbFiles, 0., 12.);
  pppNbMissingChanPerMs  = new TProfile2D("pppNbMissingChanPerMs",
                                         "Profile of not fired channels per ms VS time delay AND run "
                                         "case; Delay [ns]; run case",
                                         uNbBinsDist, dDistMin, dDistMax, kuNbFiles, 0, kuNbFiles, 0., 12.);
  pppNbMultiHitChanPerMs = new TProfile2D("pppNbMultiHitChanPerMs",
                                          "Profile of multi hit channels per ms VS time delay AND run "
                                          "case; Delay [ns]; run case",
                                          uNbBinsDist, dDistMin, dDistMax, kuNbFiles, 0, kuNbFiles, 0., 12.);


  /// Loop on scan cases
  TH2* tempTwoDimHist = NULL;
  for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
    pFile[uFile] = TFile::Open(sFilename[uFile]);
    gROOT->cd();

    /// Obtaining the raw plots from the input files ( and converting them to TH2D for rescaling)
    phHodoChanRateEvoDouble[uFile] = new TH2D();
    tempTwoDimHist                 = (TH2*) (pFile[uFile]->FindObjectAny("hHodoChanRateEvo_000"));
    if (NULL != tempTwoDimHist) tempTwoDimHist->Copy(*(phHodoChanRateEvoDouble[uFile]));  ///?
    else
      return kFALSE;

    tempTwoDimHist = (TH2*) (pFile[uFile]->FindObjectAny("hCoincWinFlagEvoMoreCond"));
    if (NULL != tempTwoDimHist)
      phCoincWinFlagEvo[uFile] = (TH2D*) (tempTwoDimHist->Clone(
        Form("phCoincWinFlagEvoMoreCond_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile])));
    else
      return kFALSE;

    /// Recover the NbFiredChan and NbMultiChan 2D plots
    tempTwoDimHist = (TH2*) (pFile[uFile]->FindObjectAny("hNbFiredChanPerMs00"));
    if (NULL != tempTwoDimHist) {
      phNbFiredChanPerMs[uFile] =
        (TH2D*) (tempTwoDimHist->Clone(Form("phNbFiredChanPerMs_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile])));
    }
    else {
      return kFALSE;
    }

    tempTwoDimHist = (TH2*) (pFile[uFile]->FindObjectAny("hNbMultiHitChanPerMs00"));
    if (NULL != tempTwoDimHist) {
      phNbMultiHitChanPerMs[uFile] =
        (TH2D*) (tempTwoDimHist->Clone(Form("phNbMultiHitChanPerMs_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile])));
    }
    else {
      return kFALSE;
    }


    phHodoChanRateEvoDouble[uFile]->SetName(
      Form("hHodoChanRateEvoDouble_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));

    /// Convert hit counts to measured to expected ratios, may also work easier with Scale!
    phHodoChanRateEvoDouble[uFile]->Scale(hitsExpected);


    /* phChanRatioEvoActive[uFile] = new TH2D( Form( "hChanRatioEvoActive_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile] ),
                                          Form( "Ratio measured hits/expected for %u fixed ch and %u scanning ch; Delay [ns]; Active chan []", uNbChanFixed[uFile], uNbChanScan[uFile] ),
                                          uNbBinsDist, dDistMin, dDistMax,
                                          12, -0.5, 11.5 );*/
    pppChanRatioEvoActive[uFile] =
      new TProfile2D(Form("pppChanRatioEvoActive_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                     Form("Ratio measured hits/expected for %u fixed ch and %u scanning ch; "
                          "Delay [ns]; Active chan []",
                          uNbChanFixed[uFile], uNbChanScan[uFile]),
                     uNbBinsDist, dDistMin, dDistMax, 12, -0.5, 11.5, 0., 2.);

    phHodoChanRateEvoProj[uFile] = phHodoChanRateEvoDouble[uFile]->ProjectionX(
      Form("hHodoChanRateEvoProj_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));
    phHodoChanRateEvoProjY[uFile] = phHodoChanRateEvoDouble[uFile]->ProjectionY(
      Form("hHodoChanRateEvoProjY_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));

    /// Make Profile X of the NbFiredChan and NbMultiChan 2D plots
    ppNbFiredChanPerMs[uFile] =
      phNbFiredChanPerMs[uFile]->ProfileX(Form("hNbFiredChanPerMsProf_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));
    ppNbMultiHitChanPerMs[uFile] = phNbMultiHitChanPerMs[uFile]->ProfileX(
      Form("hNbMultiHitChanPerMsProf_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]));


    /// Displaying
    cRatioEvo->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    phHodoChanRateEvoDouble[uFile]->Draw("colz");

    cRatioProj->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    phHodoChanRateEvoProj[uFile]->Scale(1.0 / (uNbChanFixed[uFile] + uNbChanScan[uFile]));
    phHodoChanRateEvoProj[uFile]->Draw("hist");

    /// Prepare the profile objects
    ppCoincidenceWindow[uFile] =
      new TProfile(Form("ppCoincidenceWindow_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                   Form("Coincidence window with %u fixed ch and %u scanning ch; Delay [ns]", uNbChanFixed[uFile],
                        uNbChanScan[uFile]),
                   uNbBinsDist, dDistMin, dDistMax);

    ppCoincidenceMin[uFile] = new TProfile(Form("ppCoincidenceMin_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                           Form("Coincidence lowest hit ratio with %u fixed ch and %u scanning ch; "
                                                "Delay [ns]; Ratio",
                                                uNbChanFixed[uFile], uNbChanScan[uFile]),
                                           uNbBinsDist, dDistMin, dDistMax);

    ppCoincidenceMax[uFile] = new TProfile(Form("ppCoincidenceMax_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                           Form("Coincidence highest hit ratio with %u fixed ch and %u scanning ch; "
                                                "Delay [ns]; Ratio",
                                                uNbChanFixed[uFile], uNbChanScan[uFile]),
                                           uNbBinsDist, dDistMin, dDistMax);

    ppCoincidenceMean[uFile] = new TProfile(Form("ppCoincidenceMean_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
                                            Form("Coincidence mean hit ratio with %u fixed ch and %u scanning ch; "
                                                 "Delay [ns]; Ratio",
                                                 uNbChanFixed[uFile], uNbChanScan[uFile]),
                                            uNbBinsDist, dDistMin, dDistMax);


    ppCoincidenceFlag[uFile] = new TProfile(
      Form("ppCoincidenceFlag_%u_%u", uNbChanFixed[uFile], uNbChanScan[uFile]),
      Form("Coincidence flag with %u fixed ch and %u scanning ch; Delay [ns]", uNbChanFixed[uFile], uNbChanScan[uFile]),
      uNbBinsDist, dDistMin, dDistMax);

    /// Loop on the time in run bins (1/2s stepping) with check on the corresponding delay stepping
    for (UInt_t uBin = 1;
         uBin <= phHodoChanRateEvoProj[uFile]->GetNbinsX()
         && dDistMin + dDistStep * phHodoChanRateEvoProj[uFile]->GetBinCenter(uBin) / dDistStepDuration <= dDistMax;
         ++uBin) {
      /// Delay corresponding to the bin
      Double_t dDist = dDistMin + dDistStep * phHodoChanRateEvoProj[uFile]->GetBinCenter(uBin) / dDistStepDuration;


      ppCoincidenceWindow[uFile]->Fill(dDist, phHodoChanRateEvoProj[uFile]->GetBinContent(uBin));

      ppCoincidenceFlag[uFile]->Fill(
        dDist, phCoincWinFlagEvo[uFile]->GetBinContent(phCoincWinFlagEvo[uFile]->GetBin(uBin, 1)));

      /// Fill the Profile2D for NbFiredChan and NbMultiChan
      pppNbFiredChanPerMs->Fill(dDist, uFile, ppNbFiredChanPerMs[uFile]->GetBinContent(uBin), 1);
      pppNbMissingChanPerMs->Fill(
        dDist, uFile, uNbChanFixed[uFile] + uNbChanScan[uFile] - ppNbFiredChanPerMs[uFile]->GetBinContent(uBin), 1);
      pppNbMultiHitChanPerMs->Fill(dDist, uFile, ppNbMultiHitChanPerMs[uFile]->GetBinContent(uBin), 1);

      /// Loop on channels
      Double_t dMinRatio  = 2000.0;
      Double_t dMaxRatio  = -2000.0;
      UInt_t uActiveChIdx = 0;
      for (UInt_t uBinY = 1; uBinY <= phHodoChanRateEvoDouble[uFile]->GetNbinsY(); ++uBinY) {
        /// check if channels is active
        if (1 < phHodoChanRateEvoProjY[uFile]->GetBinContent(uBinY)) {
          /// Get the ratio for this channel and delay and check if it is the minimum/maximum
          Double_t dHitRatio =
            phHodoChanRateEvoDouble[uFile]->GetBinContent(phHodoChanRateEvoDouble[uFile]->GetBin(uBin, uBinY));
          if (dHitRatio < dMinRatio) dMinRatio = dHitRatio;

          if (dMaxRatio < dHitRatio) dMaxRatio = dHitRatio;

          /// Fill the mean ratio plot
          ppCoincidenceMean[uFile]->Fill(dDist, dHitRatio);
          pppDelayCaseRatio[1]->Fill(dDist, uFile, dHitRatio, 1);

          //phChanRatioEvoActive[uFile]->Fill( dDist, uActiveChIdx, dHitRatio);
          pppChanRatioEvoActive[uFile]->Fill(dDist, uActiveChIdx, dHitRatio, 1);

          uActiveChIdx++;
        }  // if( 0 < phHodoChanRateEvoProjY[uFile]->GetBinContent( uBinY ) )
      }    //for( UInt_t uBinY = 1; uBinY <= phHodoChanRateEvoDouble[uFile]->GetNbinsY(); ++ uBinY )

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
    save_plot_to_pdf(pppChanRatioEvoActive[uFile], cpRatioEvoActive, "colz", runCase, uNbChanFixed, uNbChanScan, uFile);


    cCoincWin->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    ppCoincidenceWindow[uFile]->Draw();
    save_plot_to_pdf(ppCoincidenceWindow[uFile], cCoincWin, "", runCase, uNbChanFixed, uNbChanScan, uFile);

    cCoincWinMin->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    ppCoincidenceMin[uFile]->Draw("hist e0");
    save_plot_to_pdf(ppCoincidenceMin[uFile], cCoincWinMin, "hist e0", runCase, uNbChanFixed, uNbChanScan, uFile);

    cCoincWinMax->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();
    ppCoincidenceMax[uFile]->Draw("hist e0");
    save_plot_to_pdf(ppCoincidenceMax[uFile], cCoincWinMax, "hist e0", runCase, uNbChanFixed, uNbChanScan, uFile);

    cCoincWinMinMax->cd(1 + uFile);
    gPad->SetGridx();
    gPad->SetGridy();

    stackCoincMinMax[uFile] =
      new THStack(Form("stackCoincMinMax_%d", uFile), Form("Evolution of maximum hit losses/copies VS distance "
                                                           "between the %u ch group and the %u ch one; Time delay "
                                                           "[ns]; (observed hits)/(expected hits) ratio",
                                                           uNbChanFixed[uFile], uNbChanScan[uFile]));

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

    save_plotstack_to_pdf(stackCoincMinMax[uFile], cCoincWinMinMax, "nostack,hist e0", runCase, uNbChanFixed,
                          uNbChanScan, uFile);

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
      pppDelayCaseRatio[uStat]->GetYaxis()->SetBinLabel(uFile + 1,
                                                        Form("%d ch, %d ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
    }
  }

  for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
    pppNbFiredChanPerMs->GetYaxis()->SetBinLabel(uFile + 1,
                                                 Form("%u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
    pppNbMissingChanPerMs->GetYaxis()->SetBinLabel(uFile + 1,
                                                   Form("%u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
    pppNbMultiHitChanPerMs->GetYaxis()->SetBinLabel(uFile + 1,
                                                    Form("%u ch, %u ch", uNbChanFixed[uFile], uNbChanScan[uFile]));
  }


  /// plot profiles2D
  for (UInt_t uStat = 0; uStat < kuNbStats; uStat++) {
    cProfiles->cd(uStat + 1);
    gStyle->SetOptStat(0);
    gPad->SetGridx();
    gPad->SetGridy();
    gStyle->SetPalette(105);
    pppDelayCaseRatio[uStat]->Draw("COLZ");
    save_plot_to_pdf(pppDelayCaseRatio[uStat], cProfiles, "COLZ", runCase, uNbChanFixed, uNbChanScan, uStat);
  }
  save_canvas_to_file(outf, cProfiles, runCase);

  cHitChanProfiles->cd(1);
  gStyle->SetOptStat(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gStyle->SetPalette(105);
  pppNbFiredChanPerMs->Draw("COLZ");
  save_plot_to_pdf(pppNbFiredChanPerMs, cHitChanProfiles, "COLZ", runCase, uNbChanFixed, uNbChanScan, 1);

  cHitChanProfiles->cd(2);
  gStyle->SetOptStat(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gStyle->SetPalette(105);
  pppNbMissingChanPerMs->Draw("COLZ");
  save_plot_to_pdf(pppNbMissingChanPerMs, cHitChanProfiles, "COLZ", runCase, uNbChanFixed, uNbChanScan, 2);

  cHitChanProfiles->cd(3);
  gStyle->SetOptStat(0);
  gPad->SetGridx();
  gPad->SetGridy();
  gStyle->SetPalette(105);
  pppNbMultiHitChanPerMs->Draw("COLZ");
  save_plot_to_pdf(pppNbMultiHitChanPerMs, cHitChanProfiles, "COLZ", runCase, uNbChanFixed, uNbChanScan, 3);
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

  for (UInt_t uFile = 0; uFile < kuNbFiles; uFile++) {
    delete pppChanRatioEvoActive[uFile];
    delete ppCoincidenceWindow[uFile];
    delete ppCoincidenceMin[uFile];
    delete ppCoincidenceMax[uFile];
    delete ppCoincidenceMean[uFile];
    delete ppCoincidenceFlag[uFile];
  }  // for( UInt_t uFile = 0; uFile < kuNbFiles; uFile++ )

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

//Bool_t AnalyseTimeCoincidenceBugAllCasesMoreCond( TString sFilename = "STSXyterPulserMoreCond.root" )
Bool_t AnalyseTimeCoincidenceBugAllCasesMoreCond(TString sFilename = "STSXyterPulserNoiseCond.root")
{
  /// Open output root file
  TDirectory* oldDir = gDirectory;
  TFile* histoFile   = new TFile(sFilename, "RECREATE");
  if (NULL == histoFile) return kFALSE;

  /// Run all cases
  AnalyseTimeCoincidenceBugSixN(histoFile);
  AnalyseTimeCoincidenceBugSeven(histoFile);
  AnalyseTimeCoincidenceBugEight(histoFile);
  AnalyseTimeCoincidenceBugNine(histoFile);
  AnalyseTimeCoincidenceBugTen(histoFile);
  AnalyseTimeCoincidenceBugEleven(histoFile);
  AnalyseTimeCoincidenceBugTwelve(histoFile);

  /// Close the file
  histoFile->Close();
  oldDir->cd();

  return kTRUE;
}
