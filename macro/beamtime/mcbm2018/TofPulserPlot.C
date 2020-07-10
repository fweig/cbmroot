
Bool_t TofPulserPlot(TString sFilename) {

  TProfile* tempProfile = NULL;

  TFile* pFile = TFile::Open(sFilename);
  gROOT->cd();

  const UInt_t kuNbGdpbs      = 3;
  const UInt_t kuNbFeePerGdpb = 30;

  /// Obtaining the TOF pulser difference evolution plots (relative to gDPB 0 FEE 0)
  TProfile* phTofPulserEvoFeeToRef[kuNbGdpbs][kuNbFeePerGdpb];

  for (UInt_t uGdpb = 0; uGdpb < kuNbGdpbs; ++uGdpb) {
    for (UInt_t uFee = 0; uFee < kuNbFeePerGdpb; ++uFee) {
      phTofPulserEvoFeeToRef[uGdpb][uFee] = new TProfile();
      tempProfile                         = (TProfile*) (pFile->FindObjectAny(
        Form("hTimeDiffEvoFeeFee_g00_f00_g%02u_f%02u", uGdpb, uFee)));
      if (NULL != tempProfile) {
        tempProfile->Copy(*(phTofPulserEvoFeeToRef[uGdpb][uFee]));  ///?
      }  //  if( NULL != tempProfile )
      else
        return kFALSE;
    }  // for( UInt_t uFee = 0; uFee < kuNbFeePerGdpb; ++ uFee )
  }    // for( UInt_t uGdpb = 0; uGdpb < kuNbGdpbs; ++ uGdpb )

  /// Plotting: THStacks with 3 FEE per pad, 2*5 Pads per Canvas, 1 Canvas per gDPB (30 = 2 * 3 * 5 )
  for (UInt_t uGdpb = 0; uGdpb < kuNbGdpbs; ++uGdpb) {
    TCanvas* cGdpb = new TCanvas(Form("cGdpb%02u", uGdpb),
                                 Form("Evolution of Time difference profile "
                                      "against gDPB 0 FEE 0 for gDPB %02u",
                                      uGdpb));
    cGdpb->Divide(5, 2);

    THStack* pStacks[kuNbFeePerGdpb / 3];
    for (UInt_t uFee = 0; uFee < kuNbFeePerGdpb; ++uFee) {
      if (0 == uFee % 3)
        pStacks[uFee / 3] = new THStack(
          Form(
            "stackGdpb%02uFee%02u_%02u_%02u", uGdpb, uFee, uFee + 1, uFee + 2),
          Form("Evolution of time diff against ref FEE for gDPB %02u FEE %02u "
               "%02u %02u",
               uGdpb,
               uFee,
               uFee + 1,
               uFee + 2));
      switch (uFee % 3) {
        case 0:
          phTofPulserEvoFeeToRef[uGdpb][uFee]->SetLineColor(kBlack);
          break;
        case 1: phTofPulserEvoFeeToRef[uGdpb][uFee]->SetLineColor(kRed); break;
        case 2: phTofPulserEvoFeeToRef[uGdpb][uFee]->SetLineColor(kBlue); break;
      }  // switch( uFee % 3 )
      pStacks[uFee / 3]->Add(phTofPulserEvoFeeToRef[uGdpb][uFee]);

      cGdpb->cd(1 + uFee / 3);
      gPad->SetGridx();
      gPad->SetGridy();
      pStacks[uFee / 3]->Draw("nostack,histe0");
    }  // for( UInt_t uFee = 0; uFee < kuNbFeePerGdpb; ++ uFee )
  }    // for( UInt_t uGdpb = 0; uGdpb < kuNbGdpbs; ++ uGdpb )


  return kTRUE;
}
