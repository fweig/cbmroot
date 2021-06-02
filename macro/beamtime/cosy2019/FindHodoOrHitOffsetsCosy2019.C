/** @file FindHodoOrHitOffsetsCosy2019.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/

Bool_t FindHodoOrHitOffsetsCosy2019(Long64_t liNbEntryToRead = -1, UInt_t uRunId = 25, Double_t dWinStart = -100.,
                                    Double_t dWinStop = 100., UInt_t uHodoWinLimClk = 100, UInt_t uStsWinLimClk = 500,
                                    TString sInputFileName     = "data/unp_cosy",
                                    TString sInputFileNameHits = "data/cosy2019")
{
  /// Ignore runs where the T0 FEE was not used
  if (uRunId < 13) return kFALSE;

  /// Constants
  static const UInt_t kuNbChanPerAsic = 128;
  static const UInt_t kuNbAsicPerFeb  = 8;
  static const UInt_t kuNbFebs        = 2;
  static const UInt_t kuNbAsics       = kuNbAsicPerFeb * kuNbFebs;
  static const Double_t kdClkCycle    = 6.25;  // ns

  /// Axis variables
  UInt_t uNbTimeBins     = dWinStop - dWinStart;
  Double_t dHodoWinStart = (1 + 2 * uHodoWinLimClk) * kdClkCycle / 2 * -1;
  Double_t dHodoWinStop  = (1 + 2 * uHodoWinLimClk) * kdClkCycle / 2;
  Double_t dStsWinStart  = (1 + 2 * uStsWinLimClk) * kdClkCycle / 2 * -1;
  Double_t dStsWinStop   = (1 + 2 * uStsWinLimClk) * kdClkCycle / 2;


  /// Histograms
  /// Raw
  TH2* hDigisNbEvoTs = new TH2D("hDigisNbEvoTs",
                                "Nb Digis vs Tree entry (TS), per OR channel; "
                                "Entry []; Channel []; Counts [Digis]",
                                10000, 0, 10000, 4, 0, 4);
  TH2* hDigisNbEvo   = new TH2D("hDigisNbEvo",
                              "Nb Digis vs Time, per OR channel; Time in run "
                              "[s]; Channel []; Counts [Digis]",
                              12000, 0, 120, 4, 0, 4);
  /// Time diff
  TH2* hTimeDiffAxis  = new TH2D("hTimeDiffAxis",
                                "Time difference to A_X for A_Y, B_X and B_Y; "
                                "T_n - t_AX [s]; Axis; Counts [Digis]",
                                uNbTimeBins, dWinStart, dWinStop, 3, 0.5, 3.5);
  TH2* hTimeDiffBest  = new TH2D("hTimeDiffBest",
                                "Time difference to A_X for A_Y, B_X and B_Y, best match; T_n - "
                                "t_AX [s]; Axis; Counts [Digis]",
                                uNbTimeBins, dWinStart, dWinStop, 3, 0.5, 3.5);
  TH2* hTimeDiffEvoAY = new TH2D("hTimeDiffEvoAY",
                                 "Time difference to A_X vs Time; Time in run "
                                 "[s]; t_AY - t_AX; Counts [Digis]",
                                 1200, 0, 120, uNbTimeBins, dWinStart, dWinStop);
  TH2* hTimeDiffEvoBX = new TH2D("hTimeDiffEvoBX",
                                 "Time difference to A_X vs Time; Time in run "
                                 "[s]; t_BX - t_AX; Counts [Digis]",
                                 1200, 0, 120, uNbTimeBins, dWinStart, dWinStop);
  TH2* hTimeDiffEvoBY = new TH2D("hTimeDiffEvoBY",
                                 "Time difference to A_X vs Time; Time in run "
                                 "[s]; t_BY - t_AX; Counts [Digis]",
                                 1200, 0, 120, uNbTimeBins, dWinStart, dWinStop);
  /// Coincidence stats
  TH1* hCoincNbEvo    = new TH1D("hCoincNbEvo", "Nb OR Coinc vs Time; Time in run [s]; Coinc Counts []", 12000, 0, 120);
  TH2* hTimeDiffEvoAB = new TH2D("hTimeDiffEvoAB",
                                 "Mean Time difference of hodo B to A vs Time; Time in run [s]; "
                                 "(t_BX + t_BY)/2 - (t_AX + t_AY)/2; Counts [Digis]",
                                 1200, 0, 120, uNbTimeBins, dWinStart, dWinStop);

  /// Hits Raw
  TH2* hHitsNbEvoTs = new TH2D("hHitsNbEvoTs",
                               "Nb Hits vs Tree entry (TS), per system; Entry "
                               "[]; System []; Counts [Hits]",
                               10000, 0, 10000, 2, 0, 2);
  TH2* hHitsNbEvo   = new TH2D("hHitsNbEvo",
                             "Nb Hits vs Tree entry (TS), per system; Time in "
                             "run [s]; System []; Counts [Hits]",
                             12000, 0, 1200, 2, 0, 2);
  /// Hits Detectors
  TH2* hDetHitsNbEvoTs = new TH2D("hDetHitsNbEvoTs",
                                  "Nb Hits vs Tree entry (TS), per detector; "
                                  "Entry []; System []; Counts [Hits]",
                                  10000, 0, 10000, 4, 0, 4);
  TH2* hDetHitsNbEvo   = new TH2D("hDetHitsNbEvo",
                                "Nb Hits vs Tree entry (TS), per detector; "
                                "Time in run [s]; System []; Counts [Hits]",
                                12000, 0, 1200, 4, 0, 4);

  /// Hodo Time difference
  TH1* hTimeDiffHitHodoA    = new TH1D("hTimeDiffHitHodoA",
                                    "Time difference between Digis in the front hodoscopes and OR "
                                    "coincidences; T_A - T_Or [ns]; Counts [Digis]",
                                    (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);
  TH1* hTimeDiffHitHodoB    = new TH1D("hTimeDiffHitHodoB",
                                    "Time difference between Digis in the back hodoscopes and OR "
                                    "coincidences; T_B - T_Or [ns]; Counts [Digis]",
                                    (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);
  TH2* hTimeDiffEvoHitHodoA = new TH2D("hTimeDiffEvoHitHodoA",
                                       "Evolution of Time difference between Digis in the front hodoscopes and OR "
                                       "coinc; Time in Run [s]; T_A - T_Or [ns]; Counts [Digis]",
                                       1200, 0, 120, (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);
  TH2* hTimeDiffEvoHitHodoB = new TH2D("hTimeDiffEvoHitHodoB",
                                       "Evolution of Time difference between Digis in the back hodoscopes and OR "
                                       "coinc; Time in Run [s]; T_B - T_Or [ns]; Counts [Digis]",
                                       1200, 0, 120, (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);

  /// STS time difference
  TH1* hTimeDiffHitSts    = new TH1D("hTimeDiffHitSts",
                                  "Time difference between Digis on the STS and hodoscopes OR; "
                                  "T_Sts - T_Or [ns]; Counts [Digis]",
                                  (2 * uStsWinLimClk + 1), dStsWinStart, dStsWinStop);
  TH2* hTimeDiffEvoHitSts = new TH2D("hTimeDiffEvoHitSts",
                                     "Evolution of Time difference between Digis in the front hodoscopes and OR "
                                     "coinc; Time in Run [s]; T_Sts - T_Or [ns]; Counts [Digis]",
                                     1200, 0, 120, (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);

  /// STS if Hodo there
  TH2* hStsPosAll =
    new TH2D("hStsPosAll", "Position of all STS hits; X [cm]; Y [cm]; Counts [Hits]", 180, -4.0, 4.0, 700, -4.0, 4.0);
  TH2* hStsPosHodo = new TH2D("hStsPosHodo",
                              "Position of STS hits within time window of hodoscopes "
                              "coincidence; X [cm]; Y [cm]; Counts [Hits]",
                              180, -4.0, 4.0, 700, -4.0, 4.0);

  /// Input arrays
  std::vector<CbmTofDigi>* vDigisT0 = new std::vector<CbmTofDigi>();

  /// Input initiaalization
  sInputFileName += Form("_%04u.root", uRunId);
  TFile* pFile = new TFile(sInputFileName, "READ");
  gROOT->cd();

  TTree* pTree = (TTree*) pFile->Get("cbmsim");

  pTree->SetBranchAddress("T0Digi", &vDigisT0);

  /// Temp storage
  std::vector<CbmTofDigi*> vDigisHodoAX;
  std::vector<CbmTofDigi*> vDigisHodoAY;
  std::vector<CbmTofDigi*> vDigisHodoBX;
  std::vector<CbmTofDigi*> vDigisHodoBY;
  std::vector<Double_t> vdCoincTimeHodoA;
  std::vector<Double_t> vdCoincTimeHodoB;
  std::vector<Double_t> vdCoincTimeHodoOr;
  //read the number of entries in the tree
  Long64_t liNbEntries = pTree->GetEntries();

  std::cout << " Nb Entries: " << liNbEntries << " Tree addr: " << pTree << std::endl;

  if (-1 == liNbEntryToRead || liNbEntries < liNbEntryToRead) liNbEntryToRead = liNbEntries;

  for (Long64_t liEntry = 1; liEntry < liNbEntryToRead; liEntry++) {
    pTree->GetEntry(liEntry);

    UInt_t uNbDigisT0 = vDigisT0->size();

    if (0 == liEntry % 100)
      std::cout << "Event " << std::setw(6) << liEntry << " Nb T0 digis is " << std::setw(6) << uNbDigisT0 << std::endl;

    for (UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi) {
      CbmTofDigi& pDigi = vDigisT0->at(uT0Digi) UInt_t uChannel = pDigi.GetChannel();
      Double_t dTime                                            = pDigi.GetTime();

      hDigisNbEvoTs->Fill(liEntry, uChannel);
      hDigisNbEvo->Fill(dTime * 1e-9, uChannel);

      switch (uChannel) {
        case 0: vDigisHodoAX.push_back(pDigi); break;
        case 1: vDigisHodoAY.push_back(pDigi); break;
        case 2: vDigisHodoBX.push_back(pDigi); break;
        case 3: vDigisHodoBY.push_back(pDigi); break;
        default: break;
      }  // switch( uChannel )
    }    // for( UInt_t uT0Digi = 0; uT0Digi < uNbDigisT0; ++uT0Digi )

    /// Efficient detection rely on time sorted arrays!
    /// Sort the Hodo A X array respective to time
    std::sort(vDigisHodoAX.begin(), vDigisHodoAX.end(), CompareCbmDataTime<CbmDigi>);
    /// Sort the Hodo A Y array respective to time
    std::sort(vDigisHodoAY.begin(), vDigisHodoAY.end(), CompareCbmDataTime<CbmDigi>);
    /// Sort the Hodo B X array respective to time
    std::sort(vDigisHodoBX.begin(), vDigisHodoBX.end(), CompareCbmDataTime<CbmDigi>);
    /// Sort the Hodo B Y array respective to time
    std::sort(vDigisHodoBY.begin(), vDigisHodoBY.end(), CompareCbmDataTime<CbmDigi>);

    /// Coincidence search
    /// Hodo A
    UInt_t uFirstCandAY      = 0;
    Double_t dBestTimeAY     = 1e12;
    Double_t dBestTimeDiffAY = 1e12;
    UInt_t uBestCandAY       = vDigisHodoAY.size();
    for (UInt_t uDigiAX = 0; uDigiAX < vDigisHodoAX.size(); ++uDigiAX) {
      Double_t dTimeAX = vDigisHodoAX[uDigiAX]->GetTime();

      dBestTimeDiffAY = 1e12;
      uBestCandAY     = vDigisHodoAY.size();

      /// Search for best matching A_Y candidate
      for (UInt_t uDigiAY = uFirstCandAY; uDigiAY < vDigisHodoAY.size(); ++uDigiAY) {
        Double_t dTimeAY = vDigisHodoAY[uDigiAY]->GetTime();

        Double_t dTimeDiff = dTimeAY - dTimeAX;
        /// Jump condition
        if (dTimeDiff < dWinStart) {
          uFirstCandAY = uDigiAY;
          continue;
        }  // if( dTimeDiff < dWinStart )
        /// Stop condition
        if (dWinStop < dTimeDiff) { break; }  // if( dWinStop < dTimeDiff )

        /// Match condidate
        hTimeDiffAxis->Fill(dTimeDiff, 1);
        hTimeDiffEvoAY->Fill(dTimeAY * 1e-9, dTimeDiff);

        if (TMath::Abs(dTimeDiff) < dBestTimeDiffAY) {
          dBestTimeAY     = dTimeAY;
          dBestTimeDiffAY = dTimeDiff;
          uBestCandAY     = uDigiAY;
        }  // if( TMath::Abs( dTimeDiff ) < dBestTimeDiffAY )
      }    // for( UInt_t uDigiAY = uFirstCandAY; uDigiAY < vDigisHodoAY.size(); ++uDigiAY )

      if (uBestCandAY < vDigisHodoAY.size()) {
        hTimeDiffBest->Fill(dBestTimeDiffAY, 1);
        vdCoincTimeHodoA.push_back((dTimeAX + dBestTimeAY) / 2.0);
      }  // if( uBestCandAY < vDigisHodoAY.size() )
    }    // for( UInt_t uDigiAX = 0; uDigiAX < vDigisHodoAX.size(); ++uDigiAX )

    /// Hodo B
    UInt_t uFirstCandBY      = 0;
    Double_t dBestTimeBY     = 1e12;
    Double_t dBestTimeDiffBY = 1e12;
    UInt_t uBestCandBY       = vDigisHodoBY.size();
    for (UInt_t uDigiBX = 0; uDigiBX < vDigisHodoBX.size(); ++uDigiBX) {
      Double_t dTimeBX = vDigisHodoBX[uDigiBX]->GetTime();

      dBestTimeDiffBY = 1e12;
      uBestCandBY     = vDigisHodoBY.size();

      /// Search for best matching B_Y candidate
      for (UInt_t uDigiBY = uFirstCandBY; uDigiBY < vDigisHodoBY.size(); ++uDigiBY) {
        Double_t dTimeBY = vDigisHodoBY[uDigiBY]->GetTime();

        Double_t dTimeDiff = dTimeBY - dTimeBX;
        /// Jump condition
        if (dTimeDiff < dWinStart) {
          uFirstCandBY = uDigiBY;
          continue;
        }  // if( dTimeDiff < dWinStart )
        /// Stop condition
        if (dWinStop < dTimeDiff) { break; }  // if( dWinStop < dTimeDiff )

        /// Match condidate
        hTimeDiffAxis->Fill(dTimeDiff, 3);
        hTimeDiffEvoBY->Fill(dTimeBY * 1e-9, dTimeDiff);

        if (TMath::Abs(dTimeDiff) < dBestTimeDiffBY) {
          dBestTimeBY     = dTimeBY;
          dBestTimeDiffBY = dTimeDiff;
          uBestCandBY     = uDigiBY;
        }  // if( TMath::Abs( dTimeDiff ) < dBestTimeDiffBY )
      }    // for( UInt_t uDigiBY = 0; uDigiBY < vDigisHodoBY.size(); ++uDigiBY )

      if (uBestCandBY < vDigisHodoBY.size()) {
        hTimeDiffBest->Fill(dBestTimeDiffBY, 3);
        vdCoincTimeHodoB.push_back((dTimeBX + dBestTimeBY) / 2.0);
      }  // if( uBestCandBY < vDigisHodoBY.size() )
    }    // for( UInt_t uDigiBX = 0; uDigiBX < vDigisHodoBX.size(); ++uDigiBX )

    UInt_t uFirstCandB      = 0;
    Double_t dBestTimeB     = 1e12;
    Double_t dBestTimeDiffB = 1e12;
    UInt_t uBestCandB       = vdCoincTimeHodoB.size();
    /// Loop on A Coinc looking for best B coinc
    for (UInt_t uCoincA = 0; uCoincA < vdCoincTimeHodoA.size(); ++uCoincA) {
      dBestTimeDiffB = 1e12;
      uBestCandB     = vdCoincTimeHodoB.size();

      /// Search for best matching B candidate
      for (UInt_t uCoincB = uFirstCandB; uCoincB < vdCoincTimeHodoB.size(); ++uCoincB) {
        Double_t dTimeDiff = vdCoincTimeHodoB[uCoincB] - vdCoincTimeHodoA[uCoincA];
        /// Jump condition
        if (dTimeDiff < dWinStart) {
          uFirstCandB = uCoincB;
          continue;
        }  // if( dTimeDiff < dWinStart )
        /// Stop condition
        if (dWinStop < dTimeDiff) { break; }  // if( dWinStop < dTimeDiff )

        /// Match condidate
        hTimeDiffAxis->Fill(dTimeDiff, 2);
        hTimeDiffEvoBX->Fill(vdCoincTimeHodoB[uCoincB] * 1e-9, dTimeDiff);

        if (TMath::Abs(dTimeDiff) < dBestTimeDiffB) {
          dBestTimeB     = vdCoincTimeHodoB[uCoincB];
          dBestTimeDiffB = dTimeDiff;
          uBestCandB     = uCoincB;
        }  // if( TMath::Abs( dTimeDiff ) < dBestTimeDiffB )
      }    // for( UInt_t uCoincB = 0; uCoincB < vdCoincTimeHodoB.size(); ++uCoincB )

      if (uBestCandB < vdCoincTimeHodoB.size()) {
        hTimeDiffBest->Fill(dBestTimeDiffB, 2);

        Double_t dMeanTime = (vdCoincTimeHodoA[uCoincA] + dBestTimeB) / 2;
        hCoincNbEvo->Fill(dMeanTime * 1e-9);
        hTimeDiffEvoAB->Fill(dMeanTime * 1e-9, dBestTimeDiffB);

        vdCoincTimeHodoOr.push_back(dMeanTime);
      }  // If at least 1 match for each axis
    }    // for( UInt_t uCoincA = 0; uCoincA < vdCoincTimeHodoA.size(); ++uCoincA )

    /// clear memory
    vDigisHodoAX.clear();
    vDigisHodoAY.clear();
    vDigisHodoBX.clear();
    vDigisHodoBY.clear();
    vdCoincTimeHodoA.clear();
    vdCoincTimeHodoB.clear();
  }  // for( Long64_t liEntry = 0; liEntry < nentries; liEntry++)

  pFile->Close();

  /// Hits file
  /// Data Input
  TClonesArray* pHitsArraySts = new TClonesArray("CbmStsHit", 5000);
  std::vector<CbmStsHit*> vHitsHodoA;
  std::vector<CbmStsHit*> vHitsSts;
  std::vector<CbmStsHit*> vHitsHodoB;

  sInputFileNameHits += Form("_%04u.rec.root", uRunId);
  TFile* pFileHits = new TFile(sInputFileNameHits, "READ");
  gROOT->cd();

  if (nullptr == pFileHits) return kFALSE;

  TTree* pTreeHits = (TTree*) pFileHits->Get("cbmsim");

  TBranch* pBranchSts = pTreeHits->GetBranch("StsHit");
  pBranchSts->SetAddress(&pHitsArraySts);

  //read the number of entries in the tree
  Long64_t liNbEntriesHits = pTreeHits->GetEntries();

  std::cout << " Nb Entries: " << liNbEntriesHits << " Tree addr: " << pTreeHits << " Branch addr Sts: " << pBranchSts
            << std::endl;

  if (-1 == liNbEntryToRead || liNbEntriesHits < liNbEntryToRead) liNbEntryToRead = liNbEntriesHits;

  for (Long64_t liEntry = 1; liEntry < liNbEntryToRead; liEntry++) {
    pTreeHits->GetEntry(liEntry);

    UInt_t uNbHitsSts = pHitsArraySts->GetEntriesFast();

    if (0 == liEntry % 100)
      std::cout << "Event " << std::setw(6) << liEntry << " Nb Sts hits is " << std::setw(6) << uNbHitsSts << std::endl;

    hHitsNbEvoTs->Fill(liEntry, 0., uNbHitsSts);

    for (UInt_t uStsHit = 0; uStsHit < uNbHitsSts; ++uStsHit) {
      CbmStsHit* pHit = static_cast<CbmStsHit*>(pHitsArraySts->At(uStsHit));
      Double_t dTime  = pHit->GetTime();
      hHitsNbEvo->Fill(dTime * 1e-9, 0);

      UInt_t uAddress = pHit->GetAddress();

      switch (uAddress) {
        case 0x10008002:
        case 0x12008002: {
          hDetHitsNbEvoTs->Fill(liEntry, 0.);
          hDetHitsNbEvo->Fill(dTime * 1e-9, 0);
          vHitsHodoA.push_back(pHit);
          break;
        }  // HODO A
        case 0x10008012:
        case 0x12008012: {
          hDetHitsNbEvoTs->Fill(liEntry, 1.);
          hDetHitsNbEvo->Fill(dTime * 1e-9, 1);
          hStsPosAll->Fill(pHit->GetX(), pHit->GetY());
          vHitsSts.push_back(pHit);
          break;
        }  // STS
        case 0x10008022:
        case 0x12008022: {
          hDetHitsNbEvoTs->Fill(liEntry, 2.);
          hDetHitsNbEvo->Fill(dTime * 1e-9, 2);
          vHitsHodoB.push_back(pHit);
          break;
        }  // HODO A
        default: break;
      }  // switch( uAddress )
    }    // for( UInt_t uStsHit = 0; uStsHit < uNbHitsSts; ++uStsHit )

  }  // for( Long64_t liEntry = 0; liEntry < nentries; liEntry++)

  /// Efficient detection rely on time sorted arrays!
  /// Sort the array of coincidences
  std::sort(vdCoincTimeHodoOr.begin(), vdCoincTimeHodoOr.end());
  /// Sort the Hodo A array respective to time
  std::sort(vHitsHodoA.begin(), vHitsHodoA.end(), CompareCbmDataTime<CbmHit>);
  /// Sort the Sts array respective to time
  std::sort(vHitsSts.begin(), vHitsSts.end(), CompareCbmDataTime<CbmHit>);
  /// Sort the Hodo A array respective to time
  std::sort(vHitsHodoB.begin(), vHitsHodoB.end(), CompareCbmDataTime<CbmHit>);

  UInt_t uFirstCandLastOrHodoA = 0;
  UInt_t uFirstCandLastOrSts   = 0;
  UInt_t uFirstCandLastOrHodoB = 0;
  for (UInt_t uOrCoinc = 0; uOrCoinc < vdCoincTimeHodoOr.size(); ++uOrCoinc) {
    if (0 == uOrCoinc % 1000) {
      std::cout << "Coinc " << std::setw(8) << uOrCoinc << " / " << std::setw(8) << vdCoincTimeHodoOr.size()
                << std::endl;
      std::cout << "HodoA " << std::setw(8) << uFirstCandLastOrHodoA << " / " << std::setw(8) << vHitsHodoA.size()
                << std::endl;
      std::cout << "STS   " << std::setw(8) << uFirstCandLastOrSts << " / " << std::setw(8) << vHitsSts.size()
                << std::endl;
      std::cout << "HodoB " << std::setw(8) << uFirstCandLastOrHodoB << " / " << std::setw(8) << vHitsHodoB.size()
                << std::endl;
    }  // if( 0 == uOrCoinc % 1000 )

    for (UInt_t uHitA = uFirstCandLastOrHodoA; uHitA < vHitsHodoA.size(); ++uHitA) {
      Double_t dTimeA        = vHitsHodoA[uHitA]->GetTime();
      Double_t dTimeDiffHodo = dTimeA - vdCoincTimeHodoOr[uOrCoinc];

      hTimeDiffHitHodoA->Fill(dTimeDiffHodo);
      hTimeDiffEvoHitHodoA->Fill(vdCoincTimeHodoOr[uOrCoinc] * 1e-9, dTimeDiffHodo);

      if (dTimeDiffHodo < dHodoWinStart) {
        uFirstCandLastOrHodoA = uHitA;
        continue;
      }                                             // if( dTimeDiffHodo < dHodoWinStart )
      if (dHodoWinStop < dTimeDiffHodo) { break; }  // if( dHodoWinStop < dTimeDiffHodo )
    }  // for( UInt_t uHitA = uFirstCandStsHodoBA uHitA < vHitsHodoA.size(); ++uHitA )
    for (UInt_t uHitSts = uFirstCandLastOrSts; uHitSts < vHitsSts.size(); ++uHitSts) {
      Double_t dTimeSts      = vHitsSts[uHitSts]->GetTime();
      Double_t dTimeDiffHodo = dTimeSts - vdCoincTimeHodoOr[uOrCoinc];

      hTimeDiffHitSts->Fill(dTimeDiffHodo);
      hTimeDiffEvoHitSts->Fill(vdCoincTimeHodoOr[uOrCoinc] * 1e-9, dTimeDiffHodo);

      if (dTimeDiffHodo < dStsWinStart) {
        uFirstCandLastOrSts = uHitSts;
        continue;
      }                                            // if( dTimeDiffHodo < dStsWinStart )
      if (dStsWinStop < dTimeDiffHodo) { break; }  // if( dStsWinStop < dTimeDiffHodo )

      hStsPosHodo->Fill(vHitsSts[uHitSts]->GetX(), vHitsSts[uHitSts]->GetY());
    }  // for( UInt_t uHitSts = uFirstCandStsHodoSts; uHitSts < vHitsSts.size(); ++uHitSts )
    for (UInt_t uHitB = uFirstCandLastOrHodoB; uHitB < vHitsHodoB.size(); ++uHitB) {
      Double_t dTimeB        = vHitsHodoB[uHitB]->GetTime();
      Double_t dTimeDiffHodo = dTimeB - vdCoincTimeHodoOr[uOrCoinc];

      hTimeDiffHitHodoB->Fill(dTimeDiffHodo);
      hTimeDiffEvoHitHodoB->Fill(vdCoincTimeHodoOr[uOrCoinc] * 1e-9, dTimeDiffHodo);

      if (dTimeDiffHodo < dHodoWinStart) {
        uFirstCandLastOrHodoB = uHitB;
        continue;
      }                                             // if( dTimeDiffHodo < dHodoWinStart )
      if (dHodoWinStop < dTimeDiffHodo) { break; }  // if( dHodoWinStop < dTimeDiffHodo )
    }  // for( UInt_t uHitB = uFirstCandStsHodoB; uHitB < vHitsHodoB.size(); ++uHitB )
  }    // for( UInt_t uOrCoinc = 0; uOrCoinc < vdCoincTimeHodoOr.size(); ++ uOrCoinc )

  /// clear memory
  vHitsHodoA.clear();
  vHitsSts.clear();
  vHitsHodoB.clear();

  pFileHits->Close();

  /// Displaying
  TCanvas* cDigisNb = new TCanvas("cDigisNb", "Digis Nb, per OR channel, vs TS index and time in run");
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

  TCanvas* cTimeDiff = new TCanvas("cTimeDiff", "Hodo OR channels time difference");
  cTimeDiff->Divide(3, 2);

  cTimeDiff->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffAxis->Draw("colz");

  cTimeDiff->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffBest->Draw("colz");

  cTimeDiff->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoAY->Draw("colz");

  cTimeDiff->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoBX->Draw("colz");

  cTimeDiff->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoBY->Draw("colz");

  TCanvas* cCoinc = new TCanvas("cCoinc", "Hodo OR channels full coincidences");
  cCoinc->Divide(2);

  cCoinc->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  hCoincNbEvo->Draw("hist");

  cCoinc->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoAB->Draw("colz");

  TCanvas* cHitsNb = new TCanvas("cHitsNb", "Hits Nb, per system, vs TS index and time in run");
  cHitsNb->Divide(2, 2);

  cHitsNb->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hHitsNbEvoTs->Draw("colz");

  cHitsNb->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hHitsNbEvo->Draw("colz");

  cHitsNb->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hDetHitsNbEvoTs->Draw("colz");

  cHitsNb->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hDetHitsNbEvo->Draw("colz");

  TCanvas* cTimeDiffHits = new TCanvas("cTimeDiffHits", "Time difference between Hits and HODO OR coinc");
  cTimeDiffHits->Divide(3, 2);

  cTimeDiffHits->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  hTimeDiffHitHodoA->Draw("hist");

  cTimeDiffHits->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  hTimeDiffHitSts->Draw("hist");

  cTimeDiffHits->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  hTimeDiffHitHodoB->Draw("hist");

  cTimeDiffHits->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoHitHodoA->Draw("colz");

  cTimeDiffHits->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoHitSts->Draw("colz");

  cTimeDiffHits->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hTimeDiffEvoHitHodoB->Draw("colz");

  TFile* outFile = new TFile(Form("data/FindHodoOrOffsets_%04u.root", uRunId), "recreate");
  outFile->cd();

  hDigisNbEvoTs->Write();
  hDigisNbEvo->Write();

  hTimeDiffAxis->Write();
  hTimeDiffBest->Write();
  hTimeDiffEvoAY->Write();
  hTimeDiffEvoBX->Write();
  hTimeDiffEvoBY->Write();

  hCoincNbEvo->Write();
  hTimeDiffEvoAB->Write();

  hHitsNbEvoTs->Write();
  hHitsNbEvo->Write();
  hDetHitsNbEvoTs->Write();
  hDetHitsNbEvo->Write();

  hTimeDiffHitHodoA->Write();
  hTimeDiffHitHodoB->Write();
  hTimeDiffHitSts->Write();
  hTimeDiffEvoHitHodoA->Write();
  hTimeDiffEvoHitHodoB->Write();
  hTimeDiffEvoHitSts->Write();

  cDigisNb->Write();
  cTimeDiff->Write();
  cCoinc->Write();
  cHitsNb->Write();
  cTimeDiffHits->Write();

  gROOT->cd();
  outFile->Close();

  return kTRUE;
}
