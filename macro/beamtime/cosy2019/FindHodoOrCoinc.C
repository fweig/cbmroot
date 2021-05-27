
Bool_t FindHodoOrCoinc(Long64_t liNbEntryToRead = -1, UInt_t uRunId = 25, Double_t dWinStart = -100.,
                       Double_t dWinStop = 100., TString sInputFileName = "data/unp_cosy")
{
  /// Ignore runs where the T0 FEE was not used
  if (uRunId < 14) return kFALSE;

  /// Axis variables
  UInt_t uNbTimeBins = dWinStop - dWinStart;

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

  TFile* outFile = new TFile(Form("data/FindHodoOrCoinc_%04u.root", uRunId), "recreate");
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

  cDigisNb->Write();
  cTimeDiff->Write();
  cCoinc->Write();

  gROOT->cd();
  outFile->Close();

  return kTRUE;
}
