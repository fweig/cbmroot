/** @file AnalyzeDigiMcbmFull.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

Bool_t AnalyzeDigiMcbmFull(Long64_t liNbEntryToRead = -1, TString sInputFileName = "data/unp_mcbm.root")
{
  Double_t dOffsetRunStartNs = 34062540800;
  Double_t dTsDurationNs     = 25600 * 4 * 100;
  Double_t dBinLengthNs      = 100;
  /// Add bins for the offset of STS/MUCH!
  UInt_t uExtraBins = 3000;

  UInt_t uNbBinsTs = dTsDurationNs / dBinLengthNs + uExtraBins;  /// Add bins for the offset of STS/MUCH!
  std::vector<UInt_t> vuNbStsDigisBin(uNbBinsTs, 0);
  std::vector<UInt_t> vuNbMuchDigisBin(uNbBinsTs, 0);
  std::vector<UInt_t> vuNbTofDigisBin(uNbBinsTs, 0);
  std::vector<UInt_t> vuNbT0DigisBin(uNbBinsTs, 0);
  std::vector<UInt_t> vuNbAllDigisBin(uNbBinsTs, 0);
  std::vector<Double_t> vdDigisMeanTimeBin(uNbBinsTs, 0);

  /// Event detection
  UInt_t uThrNbStsDigi  = 1;
  UInt_t uThrNbMuchDigi = 1;
  UInt_t uThrNbTofDigi  = 7;
  UInt_t uThrNbT0Digi   = 0;
  UInt_t uThrNbAllDigi  = 9;

  std::vector<Double_t> vdBinsWithEventStartTime;
  std::vector<Double_t> vdBinsWithEventStopTime;
  std::vector<Double_t> vdMeanTimeEventSeeds;

  /// Event analysis
  Double_t dEventWinToMeanMinNs = -150;
  Double_t dEventWinToMeanMaxNs = 150;
  std::vector<std::vector<Double_t>> vdStsDigiTimePerEvent;
  std::vector<std::vector<Double_t>> vdMuchDigiTimePerEvent;
  std::vector<std::vector<Double_t>> vdTofDigiTimePerEvent;
  std::vector<std::vector<Double_t>> vdT0DigiTimePerEvent;

  /// Histograms
  std::vector<CbmTofDigi>* vDigisT0            = new std::vector<CbmTofDigi>();
  std::vector<CbmStsDigi>* vDigisSts           = new std::vector<CbmStsDigi>();
  std::vector<CbmMuchBeamTimeDigi>* vDigisMuch = new std::vector<CbmMuchBeamTimeDigi>();
  std::vector<CbmTofDigi>* vDigisTof           = new std::vector<CbmTofDigi>();

  /// Raw
  TH2* hDigisNbEvoTs = new TH2D("hDigisNbEvoTs",
                                "Nb Digis vs Tree entry (TS), per system; "
                                "Entry []; System []; Counts [Digis]",
                                10000, 0, 10000, 4, 0, 4);
  TH2* hDigisNbEvo   = new TH2D("hDigisNbEvo",
                              "Nb Digis vs Tree entry (TS), per system; Time "
                              "in run [s]; System []; Counts [Digis]",
                              12000, 0, 120, 4, 0, 4);
  /// Counts in small time bins
  TH2* hBinCntStsEvoTs  = new TH2D("hBinCntStsEvoTs",
                                  "STS digi counts per time bin vs Tree entry "
                                  "(TS); Entry []; Digi counts []; Bins []",
                                  10000, 0, 10000, 200, 0, 200);
  TH2* hBinCntMuchEvoTs = new TH2D("hBinCntMuchEvoTs",
                                   "MUCH digi counts per time bin vs Tree entry (TS); Entry []; Digi "
                                   "counts []; Bins []",
                                   10000, 0, 10000, 200, 0, 200);
  TH2* hBinCntTofEvoTs  = new TH2D("hBinCntTofEvoTs",
                                  "TOF digi counts per time bin vs Tree entry "
                                  "(TS); Entry []; Digi counts []; Bins []",
                                  10000, 0, 10000, 200, 0, 200);
  TH2* hBinCntT0EvoTs   = new TH2D("hBinCntT0EvoTs",
                                 "T0 digi counts per time bin vs Tree entry "
                                 "(TS); Entry []; Digi counts []; Bins []",
                                 10000, 0, 10000, 200, 0, 200);
  TH2* hBinCntAllEvoTs  = new TH2D("hBinCntAllEvoTs",
                                  "Global digi counts per time bin vs Tree entry (TS); Entry []; "
                                  "Digi counts []; Bins []",
                                  10000, 0, 10000, 200, 0, 200);

  TH2* hBinCntStsMuch = new TH2D("hBinCntStsMuch",
                                 "MUCH digi counts per time bin vs same for STS; STS Digi count "
                                 "[]; MUCH Digi counts []; Bins []",
                                 200, 0, 200, 200, 0, 200);

  TH2* hBinCntStsTof  = new TH2D("hBinCntStsTof",
                                "TOF digi counts per time bin vs same for STS; STS Digi count []; "
                                "TOF Digi counts []; Bins []",
                                200, 0, 200, 200, 0, 200);
  TH2* hBinCntMuchTof = new TH2D("hBinCntMuchTof",
                                 "TOF digi counts per time bin vs same for MUCH; MUCH Digi count "
                                 "[]; TOF Digi counts []; Bins []",
                                 200, 0, 200, 200, 0, 200);

  TH2* hBinCntStsAll  = new TH2D("hBinCntStsAll",
                                "Global digi counts per time bin vs same for STS; STS Digi count "
                                "[]; ALL Digi counts []; Bins []",
                                200, 0, 200, 200, 0, 200);
  TH2* hBinCntMuchAll = new TH2D("hBinCntMuchAll",
                                 "Global digi counts per time bin vs same for MUCH; MUCH Digi "
                                 "count []; ALL Digi counts []; Bins []",
                                 200, 0, 200, 200, 0, 200);
  TH2* hBinCntTofAll  = new TH2D("hBinCntTofAll",
                                "Global digi counts per time bin vs same for TOF; TOF Digi count "
                                "[]; ALL Digi counts []; Bins []",
                                200, 0, 200, 200, 0, 200);
  TH2* hBinCntT0All   = new TH2D("hBinCntT0All",
                               "Global digi counts per time bin vs same for T0; T0 Digi count "
                               "[]; ALL Digi counts []; Bins []",
                               200, 0, 200, 200, 0, 200);

  /// Event detection
  TH1* hEventSeedEvo =
    new TH1D("hEventSeedEvo", "Nb of event seed vs time in run; Time in run [s]; Event seed Nb []", 12000, 0, 120);

  /// Event Analysis
  TH1* hStsTofTimeCorr =
    new TH1D("hStsTofTimeCorr",
             "Time difference between each STS and TOF Digi pair in event; "
             "tSts - tTOF [ns]; Digi pairs []",
             (Int_t)(dEventWinToMeanMaxNs - dEventWinToMeanMinNs) / 5, dEventWinToMeanMinNs, dEventWinToMeanMaxNs);
  TH1* hMuchTofTimeCorr =
    new TH1D("hMuchTofTimeCorr",
             "Time difference between each MUCH and TOF Digi pair in event; "
             "tSts - tTOF [ns]; Digi pairs []",
             (Int_t)(dEventWinToMeanMaxNs - dEventWinToMeanMinNs) / 5, dEventWinToMeanMinNs, dEventWinToMeanMaxNs);
  TH1* hStsMuchTimeCorr =
    new TH1D("hStsMuchTimeCorr",
             "Time difference between each STS and MUCH Digi pair in event; "
             "tSts - tMUCH [ns]; Digi pairs []",
             (Int_t)(dEventWinToMeanMaxNs - dEventWinToMeanMinNs) / 5, dEventWinToMeanMinNs, dEventWinToMeanMaxNs);

  TH1* hStsT0TimeCorr =
    new TH1D("hStsT0TimeCorr",
             "Time difference between each STS and T0 Digi pair in event; tSts "
             "- tT0 [ns]; Digi pairs []",
             (Int_t)(dEventWinToMeanMaxNs - dEventWinToMeanMinNs) / 5, dEventWinToMeanMinNs, dEventWinToMeanMaxNs);
  TH1* hMuchT0TimeCorr =
    new TH1D("hMuchT0TimeCorr",
             "Time difference between each MUCH and T0 Digi pair in event; "
             "tSts - tT0 [ns]; Digi pairs []",
             (Int_t)(dEventWinToMeanMaxNs - dEventWinToMeanMinNs) / 5, dEventWinToMeanMinNs, dEventWinToMeanMaxNs);
  TH1* hTofT0TimeCorr =
    new TH1D("hTofT0TimeCorr",
             "Time difference between each Tof and T0 Digi pair in event; tTof "
             "- tT0 [ns]; Digi pairs []",
             (Int_t)(dEventWinToMeanMaxNs - dEventWinToMeanMinNs) / 5, dEventWinToMeanMinNs, dEventWinToMeanMaxNs);

  TFile* pFile = new TFile(sInputFileName, "READ");
  gROOT->cd();

  TTree* pTree = (TTree*) pFile->Get("cbmsim");

  pTree->SetBranchAddress("T0Digi", &vDigisT0);
  pTree->SetBranchAddress("StsDigi", &vDigisSts);
  pTree->SetBranchAddress("MuchBeamTimeDigi", &vDigisMuch);
  pTree->SetBranchAddress("TofDigi", &vDigisTof);

  //read the number of entries in the tree
  Long64_t liNbEntries = pTree->GetEntries();

  std::cout << " Nb Entries: " << liNbEntries << " Tree addr: " << pTree << std::endl;

  if (-1 == liNbEntryToRead || liNbEntries < liNbEntryToRead) liNbEntryToRead = liNbEntries;

  for (Long64_t liEntry = 3; liEntry < liNbEntryToRead; liEntry++) {
    pTree->GetEntry(liEntry);

    UInt_t uNbDigisT0   = vDigisT0->size();
    UInt_t uNbDigisSts  = vDigisSts->size();
    UInt_t uNbDigisMuch = vDigisMuch->size();
    UInt_t uNbDigisTof  = vDigisTof->size();

    if (0 == liEntry % 1000)
      std::cout << "Event " << std::setw(6) << liEntry << " Nb Sts digis is " << std::setw(6) << uNbDigisSts
                << " Nb Much digis is " << std::setw(6) << uNbDigisMuch << " Nb Tof digis is " << std::setw(6)
                << uNbDigisTof << " Nb T0 digis is " << std::setw(6) << uNbDigisT0 << std::endl;

    hDigisNbEvoTs->Fill(liEntry, 0., uNbDigisSts);
    hDigisNbEvoTs->Fill(liEntry, 1., uNbDigisMuch);
    hDigisNbEvoTs->Fill(liEntry, 2., uNbDigisTof);
    hDigisNbEvoTs->Fill(liEntry, 3., uNbDigisT0);
    /*
      if( 0 < uNbDigisSts )
      {
         std::cout << " First STS digi time is " << std::setw(12) << ( vDigisT0->at( 0 ) ).GetTime() - 3.4e10
                   << " Last  STS digi time is " << std::setw(12) << ( vDigisT0->at( uNbDigisSts - 1) ).GetTime()- 3.4e10
                   << " Diff is " << ( pDigiLast->GetTime() - pDigiFirst->GetTime() )
                   << std::endl;
      } //if( 0 < uNbDigisSts )
      if( 0 < uNbDigisMuch )
      {
         std::cout << " First MUCH digi time is " << std::setw(12) << ( vDigisMuch->at( 0 ) ).GetTime()- 3.4e10
                   << " Last  MUCH digi time is " << std::setw(12) << ( vDigisMuch->at( uNbDigisMuch - 1 ) ).GetTime()- 3.4e10
                   << " Diff is " << ( pDigiLast->GetTime() - pDigiFirst->GetTime() )
                   << std::endl;
      } //if( 0 < uNbDigisMuch )
      if( 0 < uNbDigisTof )
      {
         std::cout << " First TOF digi time is " << std::setw(12) << ( vDigisTof->at( 0 ) ).GetTime()- 3.4e10
                   << " Last  TOF digi time is " << std::setw(12) << ( vDigisTof->at( uNbDigisTof - 1 ) ).GetTime()- 3.4e10
                   << " Diff is " << ( pDigiLast->GetTime() - pDigiFirst->GetTime() )
                   << std::endl;
      } //if( 0 < uNbDigisTof )
      std::cout << "---------------------------"
                << std::endl;
*/
    Double_t dTsStartNs =
      dOffsetRunStartNs + liEntry * dTsDurationNs - uExtraBins * dBinLengthNs;  /// Add bins for the offset of STS/MUCH!

    for (UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi) {
      Double_t dTime = (vDigisT0->at(uStsDigi)).GetTime();
      hDigisNbEvo->Fill(dTime * 1e-9, 0);

      UInt_t uBin = (dTime - dTsStartNs) / dBinLengthNs;
      if (uNbBinsTs <= uBin) {
        std::cout << "STS " << uBin << " / " << uNbBinsTs << " " << uStsDigi << " / " << uNbDigisSts << std::endl;
        continue;
      }  // if( uNbBinsTs <= uBin )
      vuNbStsDigisBin[uBin]++;
      vuNbAllDigisBin[uBin]++;

      vdDigisMeanTimeBin[uBin] += dTime;
    }  // for( UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi )

    for (UInt_t uMuchDigi = 0; uMuchDigi < uNbDigisMuch; ++uMuchDigi) {
      Double_t dTime = (vDigisMuch->at(uMuchDigi)).GetTime();
      hDigisNbEvo->Fill(dTime * 1e-9, 1);

      UInt_t uBin = (dTime - dTsStartNs) / dBinLengthNs;
      if (uNbBinsTs <= uBin) {
        std::cout << "MUCH " << uBin << " / " << uNbBinsTs << " " << uMuchDigi << " / " << uNbDigisMuch << std::endl;
        continue;
      }  // if( uNbBinsTs <= uBin )
      vuNbMuchDigisBin[uBin]++;
      vuNbAllDigisBin[uBin]++;

      vdDigisMeanTimeBin[uBin] += dTime;
    }  // for( UInt_t uMuchDigi = 0; uMuchDigi < uNbDigisMuch; ++uMuchDigi )

    for (UInt_t uTofDigi = 0; uTofDigi < uNbDigisTof; ++uTofDigi) {
      Double_t dTime = (vDigisTof->at(uTofDigi)).GetTime();
      hDigisNbEvo->Fill(dTime * 1e-9, 2);

      UInt_t uBin = (dTime - dTsStartNs) / dBinLengthNs;
      if (uNbBinsTs <= uBin) {
        std::cout << "TOF " << uBin << " / " << uNbBinsTs << " " << uTofDigi << " / " << uNbDigisTof << std::endl;
        continue;
      }  // if( uNbBinsTs <= uBin )
      vuNbTofDigisBin[uBin]++;
      vuNbAllDigisBin[uBin]++;

      vdDigisMeanTimeBin[uBin] += dTime;
    }  // for( UInt_t uTofDigi = 0; uTofDigi < uNbDigisTof; ++uTofDigi )

    for (UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi) {
      Double_t dTime = (vDigisT0->at(uT0Digi)).GetTime();
      hDigisNbEvo->Fill(dTime * 1e-9, 3);

      UInt_t uBin = (dTime - dTsStartNs) / dBinLengthNs;
      if (uNbBinsTs <= uBin) {
        std::cout << "T0 " << uBin << " / " << uNbBinsTs << " " << uT0Digi << " / " << uNbDigisT0 << std::endl;
        continue;
      }  // if( uNbBinsTs <= uBin )
      vuNbT0DigisBin[uBin]++;
      vuNbAllDigisBin[uBin]++;

      vdDigisMeanTimeBin[uBin] += dTime;
    }  // for( UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi )

    for (UInt_t uBin = 0; uBin < uNbBinsTs; ++uBin) {
      if (0 < vuNbStsDigisBin[uBin]) {
        hBinCntStsEvoTs->Fill(liEntry, vuNbStsDigisBin[uBin]);

        if (0 < vuNbMuchDigisBin[uBin]) hBinCntStsMuch->Fill(vuNbStsDigisBin[uBin], vuNbMuchDigisBin[uBin]);

        if (0 < vuNbTofDigisBin[uBin]) hBinCntStsTof->Fill(vuNbStsDigisBin[uBin], vuNbTofDigisBin[uBin]);
      }  // if( 0 < vuNbStsDigisBin[  uBin ] )

      if (0 < vuNbMuchDigisBin[uBin]) {
        hBinCntMuchEvoTs->Fill(liEntry, vuNbMuchDigisBin[uBin]);

        if (0 < vuNbTofDigisBin[uBin]) hBinCntMuchTof->Fill(vuNbMuchDigisBin[uBin], vuNbTofDigisBin[uBin]);
      }  // if( 0 < vuNbMuchDigisBin[ uBin ] )

      if (0 < vuNbTofDigisBin[uBin]) {
        hBinCntTofEvoTs->Fill(liEntry, vuNbTofDigisBin[uBin]);
      }  // if( 0 < vuNbTofDigisBin[ uBin ] )

      if (0 < vuNbT0DigisBin[uBin]) {
        hBinCntT0EvoTs->Fill(liEntry, vuNbT0DigisBin[uBin]);
      }  // if( 0 < vuNbTofDigisBin[ uBin ] )

      if (0 < vuNbAllDigisBin[uBin]) {
        hBinCntAllEvoTs->Fill(liEntry, vuNbAllDigisBin[uBin]);

        hBinCntStsAll->Fill(vuNbStsDigisBin[uBin], vuNbAllDigisBin[uBin]);
        hBinCntMuchAll->Fill(vuNbMuchDigisBin[uBin], vuNbAllDigisBin[uBin]);
        hBinCntTofAll->Fill(vuNbTofDigisBin[uBin], vuNbAllDigisBin[uBin]);
        hBinCntT0All->Fill(vuNbT0DigisBin[uBin], vuNbAllDigisBin[uBin]);
      }  // if( 0 < vuNbAllDigisBin[ uBin ] )

      /// Event detection
      if (uThrNbStsDigi <= vuNbStsDigisBin[uBin] && uThrNbMuchDigi <= vuNbMuchDigisBin[uBin]
          && uThrNbTofDigi <= vuNbTofDigisBin[uBin] && uThrNbT0Digi <= vuNbT0DigisBin[uBin]
          && uThrNbAllDigi <= vuNbAllDigisBin[uBin]
          && (0 == vdBinsWithEventStopTime.size()
              || vdBinsWithEventStopTime[vdBinsWithEventStopTime.size() - 1] < (dTsStartNs + uBin * dBinLengthNs))) {
        vdBinsWithEventStartTime.push_back(dTsStartNs + uBin * dBinLengthNs);
        vdBinsWithEventStopTime.push_back(dTsStartNs + (uBin + 1) * dBinLengthNs);
        vdMeanTimeEventSeeds.push_back(vdDigisMeanTimeBin[uBin] / vuNbAllDigisBin[uBin]);
        hEventSeedEvo->Fill(vdDigisMeanTimeBin[uBin] / vuNbAllDigisBin[uBin] * 1e-9);
      }  // if all thresholds passed

      vuNbStsDigisBin[uBin]    = 0;
      vuNbMuchDigisBin[uBin]   = 0;
      vuNbTofDigisBin[uBin]    = 0;
      vuNbT0DigisBin[uBin]     = 0;
      vuNbAllDigisBin[uBin]    = 0;
      vdDigisMeanTimeBin[uBin] = 0;
    }  // for( UInt_t uBin = 0; uBin < uNbBinsTs; ++uBin )
  }    // for( Long64_t liEntry = 0; liEntry < nentries; liEntry++)
       /*
   /// Compute mean time of event seeds
   Long64_t liEntry = 3;
   UInt_t uNbBinsWithEvent = vdBinsWithEventStartTime.size();
   for( UInt_t uBinWithEvent = 0; uBinWithEvent < uNbBinsWithEvent; ++uBinWithEvent )
   {
      if( 0 == uBinWithEvent % 1000 )
         std::cout << uBinWithEvent << " / " << uNbBinsWithEvent
                   << std::endl;

      Double_t dBinStartTime = vdBinsWithEventStartTime[ uBinWithEvent ];
      Double_t dBinStopTime  = vdBinsWithEventStopTime[  uBinWithEvent ];
      Double_t dMeanTime = 0;
      UInt_t   uDigisNbInBin = 0;

      Bool_t bStsDone  = kFALSE;
      Bool_t bMuchDone = kFALSE;
      Bool_t bTofDone  = kFALSE;
      Bool_t bT0Done   = kFALSE;

      for( ; liEntry < liNbEntryToRead; liEntry++)
      {
         pTree->GetEntry(liEntry);

         UInt_t uNbDigisT0   = vDigisT0->size();
         UInt_t uNbDigisSts  = vDigisSts->size();
         UInt_t uNbDigisMuch = vDigisMuch->size();
         UInt_t uNbDigisTof  = vDigisTof->size();

         Double_t dTsStartNs = dOffsetRunStartNs + liEntry * dTsDurationNs - uExtraBins * dBinLengthNs; /// Add bins for the offset of STS/MUCH!

         for( UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi )
         {
            Double_t dTime = ( vDigisT0->at( uStsDigi) ).GetTime();
            if( dBinStartTime <= dTime && dTime < dBinStopTime )
            {
               dMeanTime += dTime;
               uDigisNbInBin++;
            } // if( dBinStartTime <= dTime && dTime < dBinStopTime )

            if( dBinStopTime <= dTime )
            {
               bStsDone = kTRUE;
               break;
            } // if( dBinStopTime <= dTime )

         } // for( UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi )

         for( UInt_t uMuchDigi = 0; uMuchDigi < uNbDigisMuch; ++uMuchDigi )
         {
            Double_t dTime = ( vDigisMuch->at( uMuchDigi ) ).GetTime();
            if( dBinStartTime <= dTime && dTime < dBinStopTime )
            {
               dMeanTime += dTime;
               uDigisNbInBin++;
            } // if( dBinStartTime <= dTime && dTime < dBinStopTime )

            if( dBinStopTime <= dTime )
            {
               bMuchDone = kTRUE;
               break;
            } // if( dBinStopTime <= dTime )
         } // for( UInt_t uMuchDigi = 0; uMuchDigi < uNbDigisMuch; ++uMuchDigi )

         for( UInt_t uTofDigi = 0; uTofDigi < uNbDigisTof; ++uTofDigi )
         {
            Double_t dTime = ( vDigisTof->at( uTofDigi ) ).GetTime();
            if( dBinStartTime <= dTime && dTime < dBinStopTime )
            {
               dMeanTime += dTime;
               uDigisNbInBin++;
            } // if( dBinStartTime <= dTime && dTime < dBinStopTime )

            if( dBinStopTime <= dTime )
            {
               bTofDone = kTRUE;
               break;
            } // if( dBinStopTime <= dTime )
         } // for( UInt_t uTofDigi = 0; uTofDigi < uNbDigisTof; ++uTofDigi )

         for( UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi )
         {
            Double_t dTime = ( vDigisT0->at( uT0Digi ) ).GetTime();
            if( dBinStartTime <= dTime && dTime < dBinStopTime )
            {
               dMeanTime += dTime;
               uDigisNbInBin++;
            } // if( dBinStartTime <= dTime && dTime < dBinStopTime )

            if( dBinStopTime <= dTime )
            {
               bT0Done = kTRUE;
               break;
            } // if( dBinStopTime <= dTime )
         } // for( UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi )

         if( kTRUE == bStsDone && kTRUE == bMuchDone && kTRUE == bTofDone && kTRUE == bT0Done )
            break;
      } // for( ; liEntry < nentries; liEntry++)

      dMeanTime /= uDigisNbInBin;
      vdMeanTimeEventSeeds.push_back( dMeanTime );

      hEventSeedEvo->Fill( dMeanTime * 1e-9 );
   } // for( UInt_t uBinWithEvent = 0; uBinWithEvent < vdBinsWithEventStartTime.size(); ++uBinWithEvent )
*/
  /// Analyze events
  UInt_t uNbEventSeeds = vdMeanTimeEventSeeds.size();
  vdStsDigiTimePerEvent.resize(uNbEventSeeds);
  vdMuchDigiTimePerEvent.resize(uNbEventSeeds);
  vdTofDigiTimePerEvent.resize(uNbEventSeeds);
  vdT0DigiTimePerEvent.resize(uNbEventSeeds);
  Long64_t liEntry = 3;
  for (UInt_t uEventSeed = 0; uEventSeed < uNbEventSeeds; ++uEventSeed) {
    if (0 == uEventSeed % 1000) std::cout << uEventSeed << " / " << uNbEventSeeds << std::endl;

    Double_t dEvtStartTime = vdMeanTimeEventSeeds[uEventSeed] + dEventWinToMeanMinNs;
    Double_t dEvtStopTime  = vdMeanTimeEventSeeds[uEventSeed] + dEventWinToMeanMaxNs;

    Bool_t bStsDone  = kFALSE;
    Bool_t bMuchDone = kFALSE;
    Bool_t bTofDone  = kFALSE;
    Bool_t bT0Done   = kFALSE;

    for (; liEntry < liNbEntryToRead; liEntry++) {
      pTree->GetEntry(liEntry);

      UInt_t uNbDigisT0   = vDigisT0->size();
      UInt_t uNbDigisSts  = vDigisSts->size();
      UInt_t uNbDigisMuch = vDigisMuch->size();
      UInt_t uNbDigisTof  = vDigisTof->size();

      for (UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi) {
        Double_t dTime = (vDigisT0->at(uStsDigi)).GetTime();
        if (dEvtStartTime <= dTime && dTime < dEvtStopTime) {
          vdStsDigiTimePerEvent[uEventSeed].push_back(dTime);
        }  // if( dEvtStartTime <= dTime && dTime < dEvtStopTime )

        if (dEvtStopTime <= dTime) {
          bStsDone = kTRUE;
          break;
        }  // if( dEvtStopTime <= dTime )
      }    // for( UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi )

      for (UInt_t uMuchDigi = 0; uMuchDigi < uNbDigisMuch; ++uMuchDigi) {
        Double_t dTime = (vDigisMuch->at(uMuchDigi)).GetTime();
        if (dEvtStartTime <= dTime && dTime < dEvtStopTime) {
          vdMuchDigiTimePerEvent[uEventSeed].push_back(dTime);
        }  // if( dEvtStartTime <= dTime && dTime < dEvtStopTime )

        if (dEvtStopTime <= dTime) {
          bMuchDone = kTRUE;
          break;
        }  // if( dEvtStopTime <= dTime )
      }    // for( UInt_t uMuchDigi = 0; uMuchDigi < uNbDigisMuch; ++uMuchDigi )

      for (UInt_t uTofDigi = 0; uTofDigi < uNbDigisTof; ++uTofDigi) {
        Double_t dTime = (vDigisTof->at(uTofDigi)).GetTime();
        if (dEvtStartTime <= dTime && dTime < dEvtStopTime) {
          vdTofDigiTimePerEvent[uEventSeed].push_back(dTime);
        }  // if( dEvtStartTime <= dTime && dTime < dEvtStopTime )

        if (dEvtStopTime <= dTime) {
          bTofDone = kTRUE;
          break;
        }  // if( dEvtStopTime <= dTime )
      }    // for( UInt_t uTofDigi = 0; uTofDigi < uNbDigisTof; ++uTofDigi )

      for (UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi) {
        Double_t dTime = (vDigisT0->at(uT0Digi)).GetTime();
        if (dEvtStartTime <= dTime && dTime < dEvtStopTime) {
          vdT0DigiTimePerEvent[uEventSeed].push_back(dTime);
        }  // if( dEvtStartTime <= dTime && dTime < dEvtStopTime )

        if (dEvtStopTime <= dTime) {
          bT0Done = kTRUE;
          break;
        }  // if( dEvtStopTime <= dTime )
      }    // for( UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi )

      if (kTRUE == bStsDone && kTRUE == bMuchDone && kTRUE == bTofDone && kTRUE == bT0Done) break;
    }  // for( ; liEntry < nentries; liEntry++)

    for (UInt_t uStsTime = 0; uStsTime < vdStsDigiTimePerEvent[uEventSeed].size(); ++uStsTime) {
      for (UInt_t uMuchTime = 0; uMuchTime < vdMuchDigiTimePerEvent[uEventSeed].size(); ++uMuchTime) {
        hStsMuchTimeCorr->Fill(vdStsDigiTimePerEvent[uEventSeed][uStsTime]
                               - vdMuchDigiTimePerEvent[uEventSeed][uMuchTime]);
      }  // for( UInt_t uMuchTime = 0; uMuchTime < vdMuchDigiTimePerEvent[ uEventSeed ].size(); ++uMuchTime )

      for (UInt_t uTofTime = 0; uTofTime < vdTofDigiTimePerEvent[uEventSeed].size(); ++uTofTime) {
        hStsTofTimeCorr->Fill(vdStsDigiTimePerEvent[uEventSeed][uStsTime]
                              - vdTofDigiTimePerEvent[uEventSeed][uTofTime]);
      }  // for( UInt_t uTofTime = 0; uTofTime < vdTofDigiTimePerEvent[ uEventSeed ].size(); ++uTofTime )
    }    // for( UInt_t uStsTime = 0; uStsTime < vdStsDigiTimePerEvent[ uEventSeed ].size(); ++uStsTime )

    for (UInt_t uMuchTime = 0; uMuchTime < vdMuchDigiTimePerEvent[uEventSeed].size(); ++uMuchTime) {
      for (UInt_t uTofTime = 0; uTofTime < vdTofDigiTimePerEvent[uEventSeed].size(); ++uTofTime) {
        hMuchTofTimeCorr->Fill(vdMuchDigiTimePerEvent[uEventSeed][uMuchTime]
                               - vdTofDigiTimePerEvent[uEventSeed][uTofTime]);
      }  // for( UInt_t uTofTime = 0; uTofTime < vdTofDigiTimePerEvent[ uEventSeed ].size(); ++uTofTime )
    }    // for( UInt_t uMuchTime = 0; uMuchTime < vdMuchDigiTimePerEvent[ uEventSeed ].size(); ++uMuchTime )


    for (UInt_t uT0Time = 0; uT0Time < vdT0DigiTimePerEvent[uEventSeed].size(); ++uT0Time) {
      for (UInt_t uStsTime = 0; uStsTime < vdStsDigiTimePerEvent[uEventSeed].size(); ++uStsTime) {
        hStsT0TimeCorr->Fill(vdStsDigiTimePerEvent[uEventSeed][uStsTime] - vdT0DigiTimePerEvent[uEventSeed][uT0Time]);
      }  // for( UInt_t uStsTime = 0; uStsTime < vdStsDigiTimePerEvent[ uEventSeed ].size(); ++uStsTime )

      for (UInt_t uMuchTime = 0; uMuchTime < vdMuchDigiTimePerEvent[uEventSeed].size(); ++uMuchTime) {
        hMuchT0TimeCorr->Fill(vdMuchDigiTimePerEvent[uEventSeed][uMuchTime]
                              - vdT0DigiTimePerEvent[uEventSeed][uT0Time]);
      }  // for( UInt_t uMuchTime = 0; uMuchTime < vdMuchDigiTimePerEvent[ uEventSeed ].size(); ++uMuchTime )

      for (UInt_t uTofTime = 0; uTofTime < vdTofDigiTimePerEvent[uEventSeed].size(); ++uTofTime) {
        hTofT0TimeCorr->Fill(vdTofDigiTimePerEvent[uEventSeed][uTofTime] - vdT0DigiTimePerEvent[uEventSeed][uT0Time]);
      }  // for( UInt_t uTofTime = 0; uTofTime < vdTofDigiTimePerEvent[ uEventSeed ].size(); ++uTofTime )
    }    // for( UInt_t uT0Time = 0; uT0Time < vdT0DigiTimePerEvent[ uEventSeed ].size(); ++uT0Time )
  }      // for( UInt_t uEventSeed = 0; uEventSeed < uNbEventSeeds; ++uEventSeed )


  pFile->Close();

  TCanvas* cDigisNb = new TCanvas("cDigisNb", "Digis Nb, per system, vs TS index and time in run");
  cDigisNb->Divide(2);

  cDigisNb->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hDigisNbEvoTs->Draw("colz");

  cDigisNb->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hDigisNbEvo->Draw("colz");

  TCanvas* cBinCntEvo = new TCanvas("cBinCntEvo", "Digis count per bin vs TS index and time in run, per system");
  cBinCntEvo->Divide(3, 2);

  cBinCntEvo->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntStsEvoTs->Draw("colz");

  cBinCntEvo->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntMuchEvoTs->Draw("colz");

  cBinCntEvo->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntTofEvoTs->Draw("colz");

  cBinCntEvo->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntT0EvoTs->Draw("colz");

  cBinCntEvo->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntAllEvoTs->Draw("colz");

  TCanvas* cBinCntSts = new TCanvas("cBinCntSts", "Digis count per bin in each system vs same in STS");
  cBinCntSts->Divide(2, 2);

  cBinCntSts->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntStsMuch->Draw("colz");

  cBinCntSts->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntMuchTof->Draw("colz");

  cBinCntSts->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntStsTof->Draw("colz");

  TCanvas* cBinCntAll = new TCanvas("cBinCntAll", "Global Digis count per bin vs same  in each system");
  cBinCntAll->Divide(2, 2);

  cBinCntAll->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntStsAll->Draw("colz");

  cBinCntAll->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntMuchAll->Draw("colz");

  cBinCntAll->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntTofAll->Draw("colz");

  cBinCntAll->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hBinCntT0All->Draw("colz");

  TCanvas* cEventSeedEvo = new TCanvas("cEventSeedEvo", "Event seed number vs time in run");
  cEventSeedEvo->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  hEventSeedEvo->Draw();

  TCanvas* cEventTimeCorr = new TCanvas("cEventTimeCorr", "Time correlation between systems in events");
  cEventTimeCorr->Divide(2, 2);

  cEventTimeCorr->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  hStsTofTimeCorr->Draw("");

  cEventTimeCorr->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  hMuchTofTimeCorr->Draw("");

  cEventTimeCorr->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  hStsMuchTimeCorr->Draw("");

  TCanvas* cEventTimeCorrT0 = new TCanvas("cEventTimeCorrT0", "Time correlation between systems in events");
  cEventTimeCorrT0->Divide(2, 2);

  cEventTimeCorrT0->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  hStsT0TimeCorr->Draw("");

  cEventTimeCorrT0->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  hMuchT0TimeCorr->Draw("");

  cEventTimeCorrT0->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  hTofT0TimeCorr->Draw("");

  TFile* outFile = new TFile("data/HistosDigiAnaMcbmFull.root", "recreate");
  outFile->cd();

  hDigisNbEvoTs->Write();
  hDigisNbEvo->Write();
  hBinCntStsEvoTs->Write();
  hBinCntMuchEvoTs->Write();
  hBinCntTofEvoTs->Write();
  hBinCntT0EvoTs->Write();
  hBinCntAllEvoTs->Write();
  hBinCntStsMuch->Write();
  hBinCntStsTof->Write();
  hBinCntMuchTof->Write();
  hBinCntStsAll->Write();
  hBinCntMuchAll->Write();
  hBinCntTofAll->Write();
  hBinCntT0All->Write();
  hEventSeedEvo->Write();
  hStsTofTimeCorr->Write();
  hMuchTofTimeCorr->Write();
  hStsMuchTimeCorr->Write();
  hStsT0TimeCorr->Write();
  hMuchT0TimeCorr->Write();
  hTofT0TimeCorr->Write();

  cDigisNb->Write();
  cBinCntEvo->Write();
  cBinCntSts->Write();
  cBinCntAll->Write();
  cEventSeedEvo->Write();
  cEventTimeCorr->Write();
  cEventTimeCorrT0->Write();

  gROOT->cd();
  outFile->Close();

  return kTRUE;
}
