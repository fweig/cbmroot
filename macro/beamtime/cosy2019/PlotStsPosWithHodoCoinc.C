/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

Bool_t PlotStsPosWithHodoCoinc(Long64_t liNbEntryToRead = -1, UInt_t uRunId = 12, Double_t dHodoWinStart = -600,
                               Double_t dHodoWinStop = 600, Double_t dStsWinStart = -1000, Double_t dStsWinStop = 1000)
{
  /// Data Input
  TClonesArray* pHitsArraySts =
    new TClonesArray("CbmStsHit",
                     5000);  //   TClonesArray * pHitsArrayT0   = new TClonesArray("CbmTofHit",5000);

  /// Temp storage
  std::vector<CbmStsHit*> vHitsHodoA;
  std::vector<CbmStsHit*> vHitsSts;
  std::vector<CbmStsHit*> vHitsHodoB;
  std::vector<CbmTofHit*> vHitsHodoT0;

  /// Histograms
  /// Raw
  TH2* hHitsNbEvoTs = new TH2D("hHitsNbEvoTs",
                               "Nb Hits vs Tree entry (TS), per system; Entry "
                               "[]; System []; Counts [Hits]",
                               10000, 0, 10000, 2, 0, 2);
  TH2* hHitsNbEvo   = new TH2D("hHitsNbEvo",
                             "Nb Hits vs Tree entry (TS), per system; Time in "
                             "run [s]; System []; Counts [Hits]",
                             12000, 0, 1200, 2, 0, 2);
  /// Detectors
  TH2* hDetHitsNbEvoTs = new TH2D("hDetHitsNbEvoTs",
                                  "Nb Hits vs Tree entry (TS), per detector; "
                                  "Entry []; System []; Counts [Hits]",
                                  10000, 0, 10000, 4, 0, 4);
  TH2* hDetHitsNbEvo   = new TH2D("hDetHitsNbEvo",
                                "Nb Hits vs Tree entry (TS), per detector; "
                                "Time in run [s]; System []; Counts [Hits]",
                                12000, 0, 1200, 4, 0, 4);
  /// Hodo Time difference
  TH1* hHodoTimeDiff = new TH1D("hHodoTimeDiff",
                                "Time difference between hits on the front and back hodoscopes; "
                                "T_B - T_A [ns]; Counts [Hits]",
                                static_cast<Int_t>(dHodoWinStop - dHodoWinStart), dHodoWinStart, dHodoWinStop);
  TH2* hHodoTimeDiffEvo =
    new TH2D("hHodoTimeDiffEvo",
             "Evolution of Time difference between hits on the front and back "
             "hodoscopes; Time in Run [s]; T_B - T_A [ns]; Counts [Hits]",
             120, 0, 1200, static_cast<Int_t>(dHodoWinStop - dHodoWinStart), dHodoWinStart, dHodoWinStop);

  /// Hodo-STS time difference
  TH1* hStsTimeDiff = new TH1D("hStsTimeDiff",
                               "Time difference between hits on the STS and "
                               "hodoscopes; T_Sts - T_Hodo [ns]; Counts [Hits]",
                               static_cast<Int_t>(dStsWinStop - dStsWinStart), dStsWinStart, dStsWinStop);
  TH2* hStsTimeDiffEvo =
    new TH2D("hStsTimeDiffEvo",
             "Evolution of Time difference between hits on the STS and "
             "hodoscopes; Time in Run [s]; T_Sts - T_Hodo [ns]; Counts [Hits]",
             120, 0, 1200, static_cast<Int_t>(dStsWinStop - dStsWinStart), dStsWinStart, dStsWinStop);

  /// STS if Hodo there
  TH2* hStsPosAll =
    new TH2D("hStsPosAll", "Position of all STS hits; X [cm]; Y [cm]; Counts [Hits]", 180, -4.0, 4.0, 700, -4.0, 4.0);
  TH2* hStsPosHodo = new TH2D("hStsPosHodo",
                              "Position of STS hits within time window of hodoscopes "
                              "coincidence; X [cm]; Y [cm]; Counts [Hits]",
                              180, -4.0, 4.0, 700, -4.0, 4.0);

  /// Data input
  TString sInputFileName = Form("data/cosy2019_%04u.rec.root", uRunId);
  TFile* pFile           = new TFile(sInputFileName, "READ");
  gROOT->cd();

  if (nullptr == pFile) return kFALSE;

  TTree* pTree = (TTree*) pFile->Get("cbmsim");

  TBranch* pBranchSts = pTree->GetBranch("StsHit");
  pBranchSts->SetAddress(&pHitsArraySts);
  /*
   TBranch *pBranchT0  = pTree->GetBranch("T0Hit");
   pBranchT0->SetAddress( &pHitsArrayT0 );
*/
  //read the number of entries in the tree
  Long64_t liNbEntries = pTree->GetEntries();

  std::cout << " Nb Entries: " << liNbEntries << " Tree addr: " << pTree << " Branch addr Sts: "
            << pBranchSts
            //             << " Branch addr Tof: " << pBranchTof
            << std::endl;

  if (-1 == liNbEntryToRead || liNbEntries < liNbEntryToRead) liNbEntryToRead = liNbEntries;

  for (Long64_t liEntry = 1; liEntry < liNbEntryToRead; liEntry++) {
    pTree->GetEntry(liEntry);

    UInt_t uNbHitsSts = pHitsArraySts->GetEntriesFast();
    //      UInt_t uNbHitsT0   = pHitsArrayT0->GetEntriesFast();

    if (0 == liEntry % 1000)
      std::cout << "Event " << std::setw(6) << liEntry << " Nb Sts hits is " << std::setw(6)
                << uNbHitsSts
                //                << " Nb T0 hits is "   << std::setw( 6 ) << uNbHitsT0
                << std::endl;

    hHitsNbEvoTs->Fill(liEntry, 0., uNbHitsSts);
    //      hHitsNbEvoTs->Fill( liEntry, 1., uNbHitsT0 );

    vHitsHodoA.clear();
    vHitsSts.clear();
    vHitsHodoB.clear();

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
         /*
      for( UInt_t uT0Hit = 0; uT0Hit < uNbHitsT0; ++uT0Hit )
      {
         Double_t dTime = static_cast<const CbmTofHitExp*>( pHitsArrayT0->At(uT0Hit) )->GetTime();
         hHitsNbEvo->Fill( dTime * 1e-9, 1 );

         hDetHitsNbEvoTs->Fill( liEntry, 3. );
         hDetHitsNbEvo->Fill( dTime * 1e-9, 3 );
      } // for( UInt_t uT0Hit = 0; uT0Hit < uNbHitsT0; ++uT0Hit )
*/

    /// Efficient detection rely on time sorted arrays!
    /// Sort the Hodo A array respective to time
    std::sort(vHitsHodoA.begin(), vHitsHodoA.end(), CompareCbmDataTime<CbmHit>);
    /// Sort the Sts array respective to time
    std::sort(vHitsSts.begin(), vHitsSts.end(), CompareCbmDataTime<CbmHit>);
    /// Sort the Hodo A array respective to time
    std::sort(vHitsHodoB.begin(), vHitsHodoB.end(), CompareCbmDataTime<CbmHit>);

    UInt_t uFirstCandLastHitHodo = 0;
    UInt_t uFirstCandLastHitSts  = 0;
    for (UInt_t uHitA = 0; uHitA < vHitsHodoA.size(); ++uHitA) {
      Double_t dTimeA = vHitsHodoA[uHitA]->GetTime();
      for (UInt_t uHitB = uFirstCandLastHitHodo; uHitB < vHitsHodoB.size(); ++uHitB) {
        Double_t dTimeB        = vHitsHodoB[uHitB]->GetTime();
        Double_t dTimeDiffHodo = dTimeB - dTimeA;

        hHodoTimeDiff->Fill(dTimeDiffHodo);
        hHodoTimeDiffEvo->Fill(dTimeA * 1e-9, dTimeDiffHodo);

        if (dTimeDiffHodo < dHodoWinStart) {
          uFirstCandLastHitHodo = uHitB;
          continue;
        }                                             // if( dTimeDiffHodo < dHodoWinStart )
        if (dHodoWinStop < dTimeDiffHodo) { break; }  // if( dHodoWinStop < dTimeDiffHodo )

        Double_t dTimeMeanHodo = (dTimeB + dTimeA) / 2.;
        for (UInt_t uSts = uFirstCandLastHitSts; uSts < vHitsSts.size(); ++uSts) {
          Double_t dTimeSts     = vHitsSts[uSts]->GetTime();
          Double_t dTimeDiffSts = dTimeSts - dTimeMeanHodo;

          hStsTimeDiff->Fill(dTimeDiffSts);
          hStsTimeDiffEvo->Fill(dTimeA * 1e-9, dTimeDiffSts);

          if (dTimeDiffSts < dStsWinStart) {
            uFirstCandLastHitSts = uSts;
            continue;
          }                                           // if( dTimeDiffSts < dStsWinStart )
          if (dStsWinStop < dTimeDiffSts) { break; }  // if( dStsWinStop < dTimeDiffSts )

          hStsPosHodo->Fill(vHitsSts[uSts]->GetX(), vHitsSts[uSts]->GetY());
        }  // for( UInt_t uSts = uFirstCandLastHitSts; uSts < vHitsSts.size(); ++uSts )
      }    // for( UInt_t uHitB = uFirstCandLastHitHodo; uHitB < vHitsHodoB.size(); ++uHitB )
    }      // for( UInt_t uHitA = 0; uHitA < vHitsHodoA.size(); ++uHitA )

  }  // for( Long64_t liEntry = 0; liEntry < nentries; liEntry++)

  pFile->Close();

  /// Analysis

  /// Displaying
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

  TCanvas* cTimeDiff = new TCanvas("cTimeDiff", "Time difference between hodos and between STS and HODO");
  cTimeDiff->Divide(2, 2);

  cTimeDiff->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  hHodoTimeDiff->Draw("hist");

  cTimeDiff->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hHodoTimeDiffEvo->Draw("colz");

  cTimeDiff->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  hStsTimeDiff->Draw("hist");

  cTimeDiff->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hStsTimeDiffEvo->Draw("colz");

  TCanvas* cStsPosHodo = new TCanvas("cStsPosHodo", "STS hits position if within window of HODO coincidence");
  cStsPosHodo->Divide(2);

  cStsPosHodo->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hStsPosAll->Draw("colz");

  cStsPosHodo->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hStsPosHodo->Draw("colz");

  /// Histos anc Canvases saving
  TFile* outFile = new TFile(Form("data/PlotStsPosWithHodoCoinc_%04u.root", uRunId), "recreate");
  outFile->cd();

  hHitsNbEvoTs->Write();
  hHitsNbEvo->Write();
  hDetHitsNbEvoTs->Write();
  hDetHitsNbEvo->Write();

  hHodoTimeDiff->Write();
  hHodoTimeDiffEvo->Write();
  hStsTimeDiff->Write();
  hStsTimeDiffEvo->Write();

  hStsPosHodo->Write();

  cHitsNb->Write();
  cTimeDiff->Write();
  cStsPosHodo->Write();

  gROOT->cd();
  outFile->Close();

  return kTRUE;
}
