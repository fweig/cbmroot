
Bool_t SpillRatePlot(TString sFilename)
{
  UInt_t fuTotalNrOfDpb = 5;

  TH1* tempOneDimHist = NULL;
  TH2* tempTwoDimHist = NULL;

  TFile* pFile = TFile::Open(sFilename);
  gROOT->cd();

  /// Obtaining the raw hit rate evolution plot (vs DPB and time )
  TH2* phMcbmHitsNbFineEvoDpb       = new TH2D();
  TH2* phMcbmHitsRateEvoDpbSpillOff = new TH2D();
  TH2* phMcbmHitsRateEvoDpbSpillOn  = new TH2D();
  tempTwoDimHist                    = (TH2*) (pFile->FindObjectAny("hMcbmHitsNbFineEvo"));
  if (NULL != tempTwoDimHist) {
    tempTwoDimHist->Copy(*(phMcbmHitsNbFineEvoDpb));        ///?
    tempTwoDimHist->Copy(*(phMcbmHitsRateEvoDpbSpillOff));  ///?
    tempTwoDimHist->Copy(*(phMcbmHitsRateEvoDpbSpillOn));   ///?
  }                                                         //  if( NULL != tempTwoDimHist )
  else
    return kFALSE;

  /// Obtaining the diamond hit rate variation plot (vs time )
  TH1* phDiamHitRateVariations = new TH1D();
  TH1* phDiamSpillOnOffFlag    = new TH1D();
  tempOneDimHist               = (TH1*) (pFile->FindObjectAny("hDiamondHitsRateDerivative"));
  if (NULL != tempOneDimHist) {
    tempOneDimHist->Copy(*(phDiamHitRateVariations));  ///?
    tempOneDimHist->Copy(*(phDiamSpillOnOffFlag));     ///?
  }                                                    // if( NULL != tempOneDimHist )
  else
    return kFALSE;

  phMcbmHitsRateEvoDpbSpillOff->Reset();
  phMcbmHitsRateEvoDpbSpillOn->Reset();
  phDiamSpillOnOffFlag->Reset();

  UInt_t uNbBinsDiamPlot = phDiamHitRateVariations->GetNbinsX();
  UInt_t uNbBinsRatePlot = phMcbmHitsNbFineEvoDpb->GetNbinsX();

  if (uNbBinsDiamPlot + 1 != uNbBinsRatePlot) {
    std::cout << " Wrong number of bins in input plots: " << uNbBinsDiamPlot << " vs " << uNbBinsRatePlot << std::endl;
    return kFALSE;
  }  // if( uNbBinsDiamPlot + 1 != uNbBinsRatePlot )

  ///------------------------------------------------------------------
  /// New plots
  TH2* hMcbmHitsCountPerSpillEvo = new TH2D("hMcbmHitsCountPerSpillEvo",
                                            "Hits count per spill as function of spill index; Spill #[]; DPB "
                                            "[]; Hit count []",
                                            2000, -0.5, 2000.5, fuTotalNrOfDpb, 0., fuTotalNrOfDpb);

  const Int_t iNbBinsLog        = 82;
  Double_t dBinsLog[iNbBinsLog] = {1e0, 2e0, 3e0, 4e0, 5e0, 6e0, 7e0, 8e0, 9e0, 1e1, 2e1, 3e1, 4e1, 5e1, 6e1, 7e1, 8e1,
                                   9e1, 1e2, 2e2, 3e2, 4e2, 5e2, 6e2, 7e2, 8e2, 9e2, 1e3, 2e3, 3e3, 4e3, 5e3, 6e3, 7e3,
                                   8e3, 9e3, 1e4, 2e4, 3e4, 4e4, 5e4, 6e4, 7e4, 8e4, 9e4, 1e5, 2e5, 3e5, 4e5, 5e5, 6e5,
                                   7e5, 8e5, 9e5, 1e6, 2e6, 3e6, 4e6, 5e6, 6e6, 7e6, 8e6, 9e6, 1e7, 2e7, 3e7, 4e7, 5e7,
                                   6e7, 7e7, 8e7, 9e7, 1e8, 2e8, 3e8, 4e8, 5e8, 6e8, 7e8, 8e8, 9e8, 1e9};

  TH2* hMcbmHitsCountPerSpillTofVsDiam = new TH2D("hMcbmHitsCountPerSpillTofVsDiam",
                                                  "Hits count per spill as in TOF (2 DPBs) vs T0; Hit nb per spill "
                                                  "T0[]; Hit nb per spill TOF []; Spills []",
                                                  iNbBinsLog - 1, dBinsLog, iNbBinsLog - 1, dBinsLog);
  ///------------------------------------------------------------------

  Bool_t bSpillOn          = kFALSE;
  UInt_t uSpillIdx         = 0;
  Double_t dPrevVar        = 0;
  Double_t dSpillCountTof  = 0.0;
  Double_t dSpillCountDiam = 0.0;
  for (UInt_t uBin = 1; uBin < uNbBinsRatePlot; ++uBin) {
    /// Update the SpillOn flag
    Double_t dDiamRate    = phMcbmHitsNbFineEvoDpb->GetBinContent(uBin + 1, 5);
    Double_t dDiamRateVar = phDiamHitRateVariations->GetBinContent(uBin);
    if (bSpillOn) {
      /// Spill is ON, check if we are back into the off spill plateau
      if (-6000 < dDiamRateVar && dDiamRateVar < 6000 && dPrevVar < dDiamRateVar && dDiamRate < 15000) {
        bSpillOn = kFALSE;

        hMcbmHitsCountPerSpillTofVsDiam->Fill(dSpillCountDiam, dSpillCountTof);
        dSpillCountTof  = 0.0;
        dSpillCountDiam = 0.0;
      }  // if( -60 < dDiamRateVar && dDiamRateVar < 60 )
    }    // if( bSpillOn )
    else {
      /// Spill if OFF, check if we are back in the spike at spill start
      if (6000 < dDiamRateVar) {
        bSpillOn = kTRUE;
        uSpillIdx++;
      }  // if( 60 < dDiamRateVar )
    }    // else of if( bSpillOn )
    dPrevVar = dDiamRateVar;

    /// Fill the Spill flag evo plot
    phDiamSpillOnOffFlag->SetBinContent(uBin, bSpillOn);

    /// Loop on DPBs and fill the proper rate plot
    tempTwoDimHist = bSpillOn ? phMcbmHitsRateEvoDpbSpillOn : phMcbmHitsRateEvoDpbSpillOff;
    for (UInt_t uDpb = 0; uDpb < fuTotalNrOfDpb; ++uDpb) {
      tempTwoDimHist->SetBinContent(uBin + 1, uDpb + 1, phMcbmHitsNbFineEvoDpb->GetBinContent(uBin + 1, uDpb + 1));

      /// Count the hits nb per spill (factor 0.1 to convert from rate to count in 100ms bin)
      if (bSpillOn)
        hMcbmHitsCountPerSpillEvo->Fill(uSpillIdx, uDpb,
                                        phMcbmHitsNbFineEvoDpb->GetBinContent(uBin + 1, uDpb + 1) * 0.1);
    }  // for( UInt_t uDpb = 0; uDpb < fuTotalNrOfDpb; ++uDpb )

    /// Count the hits nb per spill for TOF and diamond (factor 0.1 to convert from rate to count in 100ms bin)
    if (bSpillOn) {
      dSpillCountTof +=
        (phMcbmHitsNbFineEvoDpb->GetBinContent(uBin + 1, 3) + phMcbmHitsNbFineEvoDpb->GetBinContent(uBin + 1, 4)) * 0.1;
      dSpillCountDiam += phMcbmHitsNbFineEvoDpb->GetBinContent(uBin + 1, 5) * 0.1;
    }  // if( bSpillOn )
  }    // for( UInt_t uBin = 1; uBin < uNbBinsRatePlot; uBin ++ )

  TCanvas* cDiamVarSpillFlag = new TCanvas("cDiamVarSpillFlag", "Diamond hit rate variation and SpillOnFlag evolution");
  cDiamVarSpillFlag->Divide(2);

  cDiamVarSpillFlag->cd(1);
  phDiamHitRateVariations->Draw("hist");

  cDiamVarSpillFlag->cd(2);
  phDiamSpillOnOffFlag->Draw("hist");

  TCanvas* cDpbHitRateEvo = new TCanvas("cDpbHitRateEvo", "Evolution of the hit rate per DPB and same in/out of spill");
  cDpbHitRateEvo->Divide(3);

  cDpbHitRateEvo->cd(1);
  gPad->SetLogz();
  phMcbmHitsNbFineEvoDpb->Draw("colz");

  cDpbHitRateEvo->cd(2);
  gPad->SetLogz();
  phMcbmHitsRateEvoDpbSpillOn->Draw("colz");

  cDpbHitRateEvo->cd(3);
  phMcbmHitsRateEvoDpbSpillOff->Draw("colz");

  TCanvas* cSpillCounts = new TCanvas("cSpillCounts", "Hits count per spill for each DPB");

  cSpillCounts->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hMcbmHitsCountPerSpillEvo->Draw("colz");

  TCanvas* cSpillCountsComp =
    new TCanvas("cSpillCountsComp", "Hits count per spill for each TOF VS same for T0-Diamond");

  cSpillCountsComp->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  gPad->SetLogy();
  hMcbmHitsCountPerSpillTofVsDiam->Draw("colz");

  return kTRUE;
}
