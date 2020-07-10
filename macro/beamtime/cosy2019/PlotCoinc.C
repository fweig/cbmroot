
void PlotCoinc(TString sFilename,
               Int_t iNbTs       = -1,
               Double_t dOffsetX = 0.0,
               Double_t dOffsetY = 0.0) {
  Double_t dPosZHodoA = 50.0;
  Double_t dPosZSts   = 85.0;
  Double_t dPosZHodoB = 120.0;
  Double_t dHodoDistZ = dPosZHodoB - dPosZHodoA;
  Double_t dStsDistZ  = dPosZSts - dPosZHodoA;

  Double_t dMidStsHodoA = (dPosZHodoA + dPosZSts) / 2.0;
  Double_t dMidStsHodoB = (dPosZHodoB + dPosZSts) / 2.0;

  /// Data access
  TClonesArray* arrayClusters = new TClonesArray("CbmStsCluster");
  TClonesArray* arrayHits     = new TClonesArray("CbmStsHit");
  TFile* pFile                = new TFile(sFilename, "READ");
  TTree* pTree                = dynamic_cast<TTree*>(pFile->Get("cbmsim"));
  pTree->SetBranchAddress("StsCluster", &arrayClusters);
  pTree->SetBranchAddress("StsHit", &arrayHits);


  /// Histograms
  Int_t iCoincLimitClk = 200;
  Double_t dClockCycle = 3.125;  // ns
  Double_t dCoincLimit = iCoincLimitClk * dClockCycle;
  TH2* phHitsPositionHodoA =
    new TH2I("phHitsPositionHodoA",
             "Position of the hits in hodoscope A; X [cm]; Y [cm]; Hits []",
             80,
             -4.0,
             4.0,
             80,
             -4.0,
             4.0);
  TH2* phHitsPositionSts =
    new TH2I("phHitsPositionSts",
             "Position of the hits in hodoscope B; X [cm]; Y [cm]; Hits []",
             200,
             -10.0,
             10.0,
             80,
             -4.0,
             4.0);
  TH2* phHitsPositionHodoB =
    new TH2I("phHitsPositionHodoB",
             "Position of the hits in hodoscope B; X [cm]; Y [cm]; Hits []",
             80,
             -4.0,
             4.0,
             80,
             -4.0,
             4.0);
  ///---------------------------------------------------------------------///

  TH2* phNbHitsCompHodo     = new TH2I("phNbHitsCompHodo",
                                   "Number of hits per TS in Hodo A vs Hodo B; "
                                   "Nb Hits A[]; Nb Hits B []; TS []",
                                   10,
                                   0.0,
                                   10.0,
                                   10,
                                   0.0,
                                   10.0);
  TH2* phNbHitsCompStsHodoA = new TH2I("phNbHitsCompStsHodoA",
                                       "Number of hits per TS in STS vs Hodo "
                                       "A; Nb Hits STS[]; Nb Hits A []; TS []",
                                       10,
                                       0.0,
                                       10.0,
                                       10,
                                       0.0,
                                       10.0);
  TH2* phNbHitsCompStsHodoB = new TH2I("phNbHitsCompStsHodoB",
                                       "Number of hits per TS in STS vs Hodo "
                                       "B; Nb Hits STS[]; Nb Hits B []; TS []",
                                       10,
                                       0.0,
                                       10.0,
                                       10,
                                       0.0,
                                       10.0);
  ///---------------------------------------------------------------------///

  TH2* phHitsCoincCorrXX = new TH2I(
    "phHitsCoincCorrXX",
    "XX correlation of the coincident hits; X_A [cm]; X_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsCoincCorrYY = new TH2I(
    "phHitsCoincCorrYY",
    "YY correlation of the coincident hits; Y_A [cm]; Y_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsCoincCorrXY = new TH2I(
    "phHitsCoincCorrXY",
    "XY correlation of the coincident hits; X_A [cm]; Y_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsCoincCorrYX = new TH2I(
    "phHitsCoincCorrYX",
    "YX correlation of the coincident hits; Y_A [cm]; X_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);

  TH2* phHitsPositionCoincA = new TH2I(
    "phHitsPositionCoincA",
    "Position of the coincident hits in hodoscope A; X [cm]; Y [cm]; Hits []",
    80,
    -4.0,
    4.0,
    80,
    -4.0,
    4.0);
  TH2* phHitsPositionCoincB = new TH2I(
    "phHitsPositionCoincB",
    "Position of the coincident hits in hodoscope B; X [cm]; Y [cm]; Hits []",
    80,
    -4.0,
    4.0,
    80,
    -4.0,
    4.0);
  TH2* phHitsPositionDiff =
    new TH2I("phHitsPositionDiff",
             "Position difference of the coincident hits; X_B - X_A [cm]; Y_B- "
             "Y_A [cm]; Hits []",
             160,
             -8.0,
             8.0,
             160,
             -8.0,
             8.0);
  TH1* phHitsTimeDiff =
    new TH1I("phHitsTimeDiff",
             "Time difference of the coincident hits; t_B - t_A [ns]; Hits []",
             2 * iCoincLimitClk,
             -dCoincLimit,
             dCoincLimit);

  TH1* phHitsCoincDist =
    new TH1I("phHitsCoincDist",
             "XY distance of the coincident hits; Dist. [cm]; Hits []",
             100,
             0.0,
             10.0);
  TH1* phHitsCoincAngle =
    new TH1I("phHitsCoincAngle",
             "Vertical angle of the coincident hits; Angle [deg.]; Hits []",
             180,
             -90.0,
             90.0);
  ///---------------------------------------------------------------------///

  TH2* phHitsSingleCoincCorrXX = new TH2I(
    "phHitsSingleCoincCorrXX",
    "XX correlation of the coincident hits; X_A [cm]; X_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsSingleCoincCorrYY = new TH2I(
    "phHitsSingleCoincCorrYY",
    "YY correlation of the coincident hits; Y_A [cm]; Y_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsSingleCoincCorrXY = new TH2I(
    "phHitsSingleCoincCorrXY",
    "XY correlation of the coincident hits; X_A [cm]; Y_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsSingleCoincCorrYX = new TH2I(
    "phHitsSingleCoincCorrYX",
    "YX correlation of the coincident hits; Y_A [cm]; X_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);

  TH2* phHitsSinglePositionCoincA = new TH2I(
    "phHitsSinglePositionCoincA",
    "Position of the coincident hits in hodoscope A; X [cm]; Y [cm]; Hits []",
    80,
    -4.0,
    4.0,
    80,
    -4.0,
    4.0);
  TH2* phHitsSinglePositionCoincB = new TH2I(
    "phHitsSinglePositionCoincB",
    "Position of the coincident hits in hodoscope B; X [cm]; Y [cm]; Hits []",
    80,
    -4.0,
    4.0,
    80,
    -4.0,
    4.0);
  TH2* phHitsSinglePositionDiff =
    new TH2I("phHitsSinglePositionDiff",
             "Position difference of the coincident hits; X_B - X_A [cm]; Y_B- "
             "Y_A [cm]; Hits []",
             160,
             -8.0,
             8.0,
             160,
             -8.0,
             8.0);
  TH1* phHitsSingleTimeDiff =
    new TH1I("phHitsSingleTimeDiff",
             "Time difference of the coincident hits; t_B - t_A [ns]; Hits []",
             2 * iCoincLimitClk,
             -dCoincLimit,
             dCoincLimit);

  TH1* phHitsSingleCoincDist =
    new TH1I("phHitsSingleCoincDist",
             "XY distance of the coincident hits; Dist. [cm]; Hits []",
             100,
             0.0,
             10.0);
  TH1* phHitsSingleCoincAngle =
    new TH1I("phHitsSingleCoincAngle",
             "Vertical angle of the coincident hits; Angle [deg.]; Hits []",
             180,
             -90.0,
             90.0);
  ///---------------------------------------------------------------------///

  TH2* phHitsBestCoincCorrXX = new TH2I(
    "phHitsBestCoincCorrXX",
    "XX correlation of the coincident hits; X_A [cm]; X_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsBestCoincCorrYY = new TH2I(
    "phHitsBestCoincCorrYY",
    "YY correlation of the coincident hits; Y_A [cm]; Y_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsBestCoincCorrXY = new TH2I(
    "phHitsBestCoincCorrXY",
    "XY correlation of the coincident hits; X_A [cm]; Y_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);
  TH2* phHitsBestCoincCorrYX = new TH2I(
    "phHitsBestCoincCorrYX",
    "YX correlation of the coincident hits; Y_A [cm]; X_B [cm]; Hits []",
    160,
    -8.0,
    8.0,
    160,
    -8.0,
    8.0);

  TH2* phHitsBestPositionCoincA = new TH2I(
    "phHitsBestPositionCoincA",
    "Position of the coincident hits in hodoscope A; X [cm]; Y [cm]; Hits []",
    80,
    -4.0,
    4.0,
    80,
    -4.0,
    4.0);
  TH2* phHitsBestPositionCoincB = new TH2I(
    "phHitsBestPositionCoincB",
    "Position of the coincident hits in hodoscope B; X [cm]; Y [cm]; Hits []",
    80,
    -4.0,
    4.0,
    80,
    -4.0,
    4.0);
  TH2* phHitsBestPositionDiff =
    new TH2I("phHitsBestPositionDiff",
             "Position difference of the coincident hits; X_B - X_A [cm]; Y_B- "
             "Y_A [cm]; Hits []",
             160,
             -8.0,
             8.0,
             160,
             -8.0,
             8.0);
  TH1* phHitsBestTimeDiff =
    new TH1I("phHitsBestTimeDiff",
             "Time difference of the coincident hits; t_B - t_A [ns]; Hits []",
             2 * iCoincLimitClk,
             -dCoincLimit,
             dCoincLimit);

  TH1* phHitsBestCoincDist =
    new TH1I("phHitsBestCoincDist",
             "XY distance of the coincident hits; Dist. [cm]; Hits []",
             100,
             0.0,
             10.0);
  TH1* phHitsBestCoincAngle =
    new TH1I("phHitsBestCoincAngle",
             "Vertical angle of the coincident hits; Angle [deg.]; Hits []",
             180,
             -90.0,
             90.0);
  ///---------------------------------------------------------------------///
  TH1* phHitsStsTimeDiffBestHodo =
    new TH1I("phHitsStsTimeDiffBestHodo",
             "Position difference of STS hit with the best coincident hits; "
             "t_Sts - t_(AB) [ns]; Hits []",
             2 * iCoincLimitClk,
             -dCoincLimit,
             dCoincLimit);
  TH2* phHitsStsPosDiffBestHodo =
    new TH2I("phHitsStsPosDiffBestHodo",
             "Position difference of STS hit with the best coincident hits; "
             "X_Sts - X_extr(AB) [cm]; Y_Sts- Y_extr(AB) [cm]; Hits []",
             400,
             -2.0,
             2.0,
             400,
             -2.0,
             2.0);

  /// Looping variables
  Int_t iNbTsInFile = pTree->GetEntries();
  Int_t iNbClusters = 0;
  Int_t iNbHits     = 0;
  std::vector<CbmStsHit*> vHitsHodoA;
  std::vector<CbmStsHit*> vHitsSts;
  std::vector<CbmStsHit*> vHitsHodoB;

  if (-1 == iNbTs || iNbTsInFile < iNbTs) iNbTs = iNbTsInFile;

  for (Int_t iTs = 0; iTs < iNbTs; ++iTs) {
    if (0 == iTs % 10000)
      std::cout << "Processing TS " << std::setw(7) << iTs << std::endl;

    pTree->GetEntry(iTs);
    iNbHits = arrayHits->GetEntriesFast();

    vHitsHodoA.clear();
    vHitsSts.clear();
    vHitsHodoB.clear();
    for (Int_t iHit = 0; iHit < iNbHits; ++iHit) {
      CbmStsHit* pHit = dynamic_cast<CbmStsHit*>(arrayHits->UncheckedAt(iHit));
      Double_t dX     = pHit->GetX();
      Double_t dY     = pHit->GetY();
      Double_t dZ     = pHit->GetZ();

      /// Check if the hit is in Hodo A or B or in STS
      if (dZ < dMidStsHodoA) {
        vHitsHodoA.push_back(pHit);
        phHitsPositionHodoA->Fill(dX, dY);
      }  // if( dZ < dMidStsHodoA ) => if Hodo A
      else if (dZ < dMidStsHodoB) {
        vHitsSts.push_back(pHit);
        phHitsPositionSts->Fill(dX, dY);
      }  // else if( dZ < dMidStsHodoB ) of if( dZ < dMidStsHodoA ) => if STS
      else {
        vHitsHodoB.push_back(pHit);
        phHitsPositionHodoB->Fill(dX, dY);
      }  // else of if( dZ < dMidStsHodoB ) => if Hodo B
    }    // for( Int_t iHit = 0; iHit < iNbHits; ++iHit)

    phNbHitsCompHodo->Fill(vHitsHodoA.size(), vHitsHodoB.size());
    phNbHitsCompStsHodoA->Fill(vHitsSts.size(), vHitsHodoA.size());
    phNbHitsCompStsHodoB->Fill(vHitsSts.size(), vHitsHodoB.size());

    for (UInt_t uHitA = 0; uHitA < vHitsHodoA.size(); ++uHitA) {
      Double_t dBestTime = 1e9;
      UInt_t uBestB      = vHitsHodoB.size();

      Double_t dTimeA = vHitsHodoA[uHitA]->GetTime();
      for (UInt_t uHitB = 0; uHitB < vHitsHodoB.size(); ++uHitB) {
        Double_t dTimeB = vHitsHodoB[uHitB]->GetTime();

        if (TMath::Abs(dTimeB - dTimeA) < dCoincLimit) {
          phHitsCoincCorrXX->Fill(vHitsHodoA[uHitA]->GetX(),
                                  vHitsHodoB[uHitB]->GetX());
          phHitsCoincCorrYY->Fill(vHitsHodoA[uHitA]->GetY(),
                                  vHitsHodoB[uHitB]->GetY());
          phHitsCoincCorrXY->Fill(vHitsHodoA[uHitA]->GetX(),
                                  vHitsHodoB[uHitB]->GetY());
          phHitsCoincCorrYX->Fill(vHitsHodoA[uHitA]->GetY(),
                                  vHitsHodoB[uHitB]->GetX());

          phHitsPositionCoincA->Fill(vHitsHodoA[uHitA]->GetX(),
                                     vHitsHodoA[uHitA]->GetY());
          phHitsPositionCoincB->Fill(vHitsHodoB[uHitB]->GetX(),
                                     vHitsHodoB[uHitB]->GetY());
          phHitsPositionDiff->Fill(
            vHitsHodoB[uHitB]->GetX() - vHitsHodoA[uHitA]->GetX() - dOffsetX,
            vHitsHodoB[uHitB]->GetY() - vHitsHodoA[uHitA]->GetY() - dOffsetY);
          phHitsTimeDiff->Fill(dTimeB - dTimeA);

          Double_t dHitsDistXY = TMath::Sqrt(
            (vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uHitB]->GetX() - dOffsetX)
              * (vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uHitB]->GetX()
                 - dOffsetX)
            + (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uHitB]->GetY() - dOffsetY)
                * (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uHitB]->GetY()
                   - dOffsetY));
          Double_t dHitsDistZ =
            TMath::Abs(vHitsHodoA[uHitA]->GetZ() - vHitsHodoB[uHitB]->GetZ());
          Double_t dAngle =
            TMath::RadToDeg() * TMath::ATan2(dHitsDistXY, dHitsDistZ);
          phHitsCoincDist->Fill(dHitsDistXY);
          phHitsCoincAngle->Fill(dAngle);

          if (TMath::Abs(dTimeB - dTimeA) < dBestTime) {
            dBestTime = TMath::Abs(dTimeB - dTimeA);
            uBestB    = uHitB;
          }  // if( TMath::Abs( dTimeB - dTimeA )  < dBestTime )
        }    // if( TMath::Abs( dTimeB - dTimeA ) < dCoincLimit )
      }      // for( UInt_t uHitB = 0; uHitB < vHitsHodoB.size(); ++uHitB )
      if (uBestB < vHitsHodoB.size()) {
        Double_t dTimeB = vHitsHodoB[uBestB]->GetTime();

        if (TMath::Abs(dTimeB - dTimeA) < dCoincLimit) {
          phHitsBestCoincCorrXX->Fill(vHitsHodoA[uHitA]->GetX(),
                                      vHitsHodoB[uBestB]->GetX());
          phHitsBestCoincCorrYY->Fill(vHitsHodoA[uHitA]->GetY(),
                                      vHitsHodoB[uBestB]->GetY());
          phHitsBestCoincCorrXY->Fill(vHitsHodoA[uHitA]->GetX(),
                                      vHitsHodoB[uBestB]->GetY());
          phHitsBestCoincCorrYX->Fill(vHitsHodoA[uHitA]->GetY(),
                                      vHitsHodoB[uBestB]->GetX());

          phHitsBestPositionCoincA->Fill(vHitsHodoA[uHitA]->GetX(),
                                         vHitsHodoA[uHitA]->GetY());
          phHitsBestPositionCoincB->Fill(vHitsHodoB[uBestB]->GetX(),
                                         vHitsHodoB[uBestB]->GetY());
          phHitsBestPositionDiff->Fill(
            vHitsHodoB[uBestB]->GetX() - vHitsHodoA[uHitA]->GetX() - dOffsetX,
            vHitsHodoB[uBestB]->GetY() - vHitsHodoA[uHitA]->GetY() - dOffsetY);
          phHitsBestTimeDiff->Fill(dTimeB - dTimeA);

          Double_t dHitsDistXY = TMath::Sqrt(
            (vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uBestB]->GetX() - dOffsetX)
              * (vHitsHodoA[uHitA]->GetX() - vHitsHodoB[uBestB]->GetX()
                 - dOffsetX)
            + (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uBestB]->GetY()
               - dOffsetY)
                * (vHitsHodoA[uHitA]->GetY() - vHitsHodoB[uBestB]->GetY()
                   - dOffsetY));
          Double_t dHitsDistZ =
            TMath::Abs(vHitsHodoA[uHitA]->GetZ() - vHitsHodoB[uBestB]->GetZ());
          Double_t dAngle =
            TMath::RadToDeg() * TMath::ATan2(dHitsDistXY, dHitsDistZ);
          phHitsBestCoincDist->Fill(dHitsDistXY);
          phHitsBestCoincAngle->Fill(dAngle);

          if (1 == vHitsHodoA.size() && 1 == vHitsHodoB.size()) {
            phHitsSingleCoincCorrXX->Fill(vHitsHodoA[uHitA]->GetX(),
                                          vHitsHodoB[uBestB]->GetX());
            phHitsSingleCoincCorrYY->Fill(vHitsHodoA[uHitA]->GetY(),
                                          vHitsHodoB[uBestB]->GetY());
            phHitsSingleCoincCorrXY->Fill(vHitsHodoA[uHitA]->GetX(),
                                          vHitsHodoB[uBestB]->GetY());
            phHitsSingleCoincCorrYX->Fill(vHitsHodoA[uHitA]->GetY(),
                                          vHitsHodoB[uBestB]->GetX());

            phHitsSinglePositionCoincA->Fill(vHitsHodoA[uHitA]->GetX(),
                                             vHitsHodoA[uHitA]->GetY());
            phHitsSinglePositionCoincB->Fill(vHitsHodoB[uBestB]->GetX(),
                                             vHitsHodoB[uBestB]->GetY());
            phHitsSinglePositionDiff->Fill(
              vHitsHodoB[uBestB]->GetX() - vHitsHodoA[uHitA]->GetX() - dOffsetX,
              vHitsHodoB[uBestB]->GetY() - vHitsHodoA[uHitA]->GetY()
                - dOffsetY);
            phHitsSingleTimeDiff->Fill(dTimeB - dTimeA);
            phHitsSingleCoincDist->Fill(dHitsDistXY);
            phHitsSingleCoincAngle->Fill(dAngle);
          }  // if( 1 == vHitsHodoA.size() && 1 == vHitsHodoB.size() )
        }    // if( TMath::Abs( dTimeB - dTimeA ) < dCoincLimit )

        Double_t dBestTimeSts = 1e9;
        UInt_t uBestSts       = vHitsSts.size();

        Double_t dHodoMeanTime = (dTimeA + dTimeB) / 2.0;
        Double_t dHodoExtrX =
          vHitsHodoA[uHitA]->GetX()
          + (vHitsHodoB[uBestB]->GetX() - vHitsHodoA[uHitA]->GetX()) * dStsDistZ
              / dHodoDistZ;
        Double_t dHodoExtrY =
          vHitsHodoA[uHitA]->GetY()
          + (vHitsHodoB[uBestB]->GetY() - vHitsHodoA[uHitA]->GetY()) * dStsDistZ
              / dHodoDistZ;

        for (UInt_t uHitSts = 0; uHitSts < vHitsSts.size(); ++uHitSts) {
          Double_t dTimeSts     = vHitsSts[uHitSts]->GetTime();
          Double_t dTimeDiffSts = dTimeSts - dHodoMeanTime;
          phHitsStsTimeDiffBestHodo->Fill(dTimeDiffSts);

          if (TMath::Abs(dTimeDiffSts) < dCoincLimit) {
            if (TMath::Abs(dTimeDiffSts) < dBestTimeSts) {
              dBestTimeSts = TMath::Abs(dTimeDiffSts);
              uBestSts     = uHitSts;
            }  // if( TMath::Abs( dTimeB - dTimeA )  < dBestTimeSts )
            else
              std::cout << iTs << " " << vHitsHodoA.size() << " "
                        << vHitsSts.size() << " " << vHitsHodoB.size() << " "
                        << TMath::Abs(dTimeDiffSts) << " " << dBestTimeSts
                        << std::endl;
          }  // if( TMath::Abs( dTimeSts - dHodoMeanTime ) < dCoincLimit )
        }    // for( UInt_t uHitSts = 0; uHitSts < vHitsSts.size(); ++uHitSts )

        if (uBestSts < vHitsSts.size())
          phHitsStsPosDiffBestHodo->Fill(
            vHitsSts[uBestSts]->GetX() - dHodoExtrX,
            vHitsSts[uBestSts]->GetY() - dHodoExtrY);
      }  //  if( uBestB < vHitsHodoB.size() )
    }    // for( UInt_t uHitA = 0; uHitA < vHitsHodoA.size(); ++uHitA )

  }  // for( Int_t iTs = 0; iTs < iNbTs; ++iTs )

  /// Displaying
  TCanvas* cHitsPos =
    new TCanvas("cHitsPos", "Hits position in each Hodoscope");
  cHitsPos->Divide(3);

  cHitsPos->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phHitsPositionHodoA->Draw("colz");

  cHitsPos->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phHitsPositionSts->Draw("colz");

  cHitsPos->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phHitsPositionHodoB->Draw("colz");

  ///---------------------------------------------------------------------///
  TCanvas* cHitsNbComp =
    new TCanvas("cHitsNbComp", "Hits Nb per TS comparison between Hodoscope");
  cHitsNbComp->Divide(3);

  cHitsNbComp->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phNbHitsCompHodo->Draw("colz");

  cHitsNbComp->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phNbHitsCompStsHodoA->Draw("colz");

  cHitsNbComp->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phNbHitsCompStsHodoB->Draw("colz");
  /*
///---------------------------------------------------------------------///
   TCanvas * cHodoCorr = new TCanvas( "cHodoCorr", "Correlations between Hodoscopes" );
   cHodoCorr->Divide( 2, 2 );

   cHodoCorr->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsCoincCorrXX->Draw( "colz" );

   cHodoCorr->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsCoincCorrYY->Draw( "colz" );

   cHodoCorr->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsCoincCorrXY->Draw( "colz" );

   cHodoCorr->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsCoincCorrYX->Draw( "colz" );

   TCanvas * cHodoCoinc = new TCanvas( "cHodoCoinc", "Coincidences between Hodoscopes" );
   cHodoCoinc->Divide( 2, 2 );

   cHodoCoinc->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsPositionCoincA->Draw( "colz" );

   cHodoCoinc->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsPositionCoincB->Draw( "colz" );

   cHodoCoinc->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsPositionDiff->Draw( "colz" );

   cHodoCoinc->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsTimeDiff->Draw( "hist" );

   TCanvas * cHitsAngle = new TCanvas( "cHitsAngle", "Angle of coincident Hits " );
   cHitsAngle->Divide( 2 );

   cHitsAngle->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsCoincDist->Draw( "hist" );

   cHitsAngle->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsCoincAngle->Draw( "hist" );
///---------------------------------------------------------------------///

   TCanvas * cHodoCorrSingle = new TCanvas( "cHodoCorrSingle", "Correlations between Hodoscopes, pick Single pair" );
   cHodoCorrSingle->Divide( 2, 2 );

   cHodoCorrSingle->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSingleCoincCorrXX->Draw( "colz" );

   cHodoCorrSingle->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSingleCoincCorrYY->Draw( "colz" );

   cHodoCorrSingle->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSingleCoincCorrXY->Draw( "colz" );

   cHodoCorrSingle->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSingleCoincCorrYX->Draw( "colz" );

   TCanvas * cHodoCoincSingle = new TCanvas( "cHodoCoincSingle", "Coincidences between Hodoscopes, pick Single pair" );
   cHodoCoincSingle->Divide( 2, 2 );

   cHodoCoincSingle->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSinglePositionCoincA->Draw( "colz" );

   cHodoCoincSingle->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSinglePositionCoincB->Draw( "colz" );

   cHodoCoincSingle->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsSinglePositionDiff->Draw( "colz" );

   cHodoCoincSingle->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsSingleTimeDiff->Draw( "hist" );

   TCanvas * cHitsAngleSingle = new TCanvas( "cHitsAngleSingle", "Angle of coincident Hits, pick Single pair" );
   cHitsAngleSingle->Divide( 2 );

   cHitsAngleSingle->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsSingleCoincDist->Draw( "hist" );

   cHitsAngleSingle->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsSingleCoincAngle->Draw( "hist" );
///---------------------------------------------------------------------///

   TCanvas * cHodoCorrBest = new TCanvas( "cHodoCorrBest", "Correlations between Hodoscopes, pick Best pair" );
   cHodoCorrBest->Divide( 2, 2 );

   cHodoCorrBest->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestCoincCorrXX->Draw( "colz" );

   cHodoCorrBest->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestCoincCorrYY->Draw( "colz" );

   cHodoCorrBest->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestCoincCorrXY->Draw( "colz" );

   cHodoCorrBest->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestCoincCorrYX->Draw( "colz" );

   TCanvas * cHodoCoincBest = new TCanvas( "cHodoCoincBest", "Coincidences between Hodoscopes, pick Best pair" );
   cHodoCoincBest->Divide( 2, 2 );

   cHodoCoincBest->cd(1);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestPositionCoincA->Draw( "colz" );

   cHodoCoincBest->cd(2);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestPositionCoincB->Draw( "colz" );

   cHodoCoincBest->cd(3);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogz();
   phHitsBestPositionDiff->Draw( "colz" );

   cHodoCoincBest->cd(4);
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsBestTimeDiff->Draw( "hist" );

   TCanvas * cHitsAngleBest = new TCanvas( "cHitsAngleBest", "Angle of coincident Hits, pick Best pair" );
   cHitsAngleBest->Divide( 2 );

   cHitsAngleBest->cd( 1 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsBestCoincDist->Draw( "hist" );

   cHitsAngleBest->cd( 2 );
   gPad->SetGridx();
   gPad->SetGridy();
   gPad->SetLogy();
   phHitsBestCoincAngle->Draw( "hist" );
///---------------------------------------------------------------------///
*/
  TCanvas* cHodoSelComp =
    new TCanvas("cHodoSelComp", "Comparison of pair selections");
  cHodoSelComp->Divide(3);

  THStack* pStackTimeDiff = new THStack(
    "pStackTimeDiff", "Time difference depending on pair selection criteria");
  THStack* pStackCoincDist = new THStack(
    "pStackCoincDist", "XY distance depending on pair selection criteria");
  THStack* pStackCoincAngle = new THStack(
    "pStackCoincAngle", "Tack angle depending on pair selection criteria");

  phHitsTimeDiff->SetLineColor(kBlack);
  phHitsCoincDist->SetLineColor(kBlack);
  phHitsCoincAngle->SetLineColor(kBlack);

  phHitsBestTimeDiff->SetLineColor(kRed);
  phHitsBestCoincDist->SetLineColor(kRed);
  phHitsBestCoincAngle->SetLineColor(kRed);

  phHitsSingleTimeDiff->SetLineColor(kBlue);
  phHitsSingleCoincDist->SetLineColor(kBlue);
  phHitsSingleCoincAngle->SetLineColor(kBlue);

  pStackTimeDiff->Add(phHitsTimeDiff);
  pStackTimeDiff->Add(phHitsBestTimeDiff);
  pStackTimeDiff->Add(phHitsSingleTimeDiff);

  pStackCoincDist->Add(phHitsCoincDist);
  pStackCoincDist->Add(phHitsBestCoincDist);
  pStackCoincDist->Add(phHitsSingleCoincDist);

  pStackCoincAngle->Add(phHitsCoincAngle);
  pStackCoincAngle->Add(phHitsBestCoincAngle);
  pStackCoincAngle->Add(phHitsSingleCoincAngle);

  cHodoSelComp->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  pStackTimeDiff->Draw("hist nostack");

  cHodoSelComp->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  pStackCoincDist->Draw("hist nostack");

  cHodoSelComp->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  pStackCoincAngle->Draw("hist nostack");
  ///---------------------------------------------------------------------///

  TCanvas* cStsCorr =
    new TCanvas("cStsCorr", "Correlations between Hodoscopes and STS");
  cStsCorr->Divide(2);

  cStsCorr->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  phHitsStsPosDiffBestHodo->Draw("colz");

  cStsCorr->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  phHitsStsTimeDiffBestHodo->Draw("hist");
}
