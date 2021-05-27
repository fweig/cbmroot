/** @file FindOffsetsCosy2019.C
  * @copyright Copyright (C) 2019-2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [orginator] **/

Bool_t FindOffsetsCosy2019(Long64_t liNbEntryToRead = -1, UInt_t uRunId = 12, UInt_t uHodoWinLimClk = 100,
                           UInt_t uStsWinLimClk = 500, TString sInputFileName = "data/unp_cosy")
{
  /// Constants
  static const UInt_t kuNbChanPerAsic = 128;
  static const UInt_t kuNbAsicPerFeb  = 8;
  static const UInt_t kuNbFebs        = 2;
  static const UInt_t kuNbAsics       = kuNbAsicPerFeb * kuNbFebs;
  static const Double_t kdClkCycle    = 6.25;  // ns

  /// Temp storage
  std::vector<CbmStsDigi*> vDigisHodoA;
  std::vector<std::vector<CbmStsDigi*>> vDigisSts;
  std::vector<std::vector<CbmStsDigi*>> vDigisStsFirstChan;
  std::vector<std::vector<CbmStsDigi*>> vDigisStsLastChan;
  std::vector<CbmStsDigi*> vDigisHodoB;
  std::vector<CbmTofDigi*> vDigisHodoT0;

  vDigisSts.resize(kuNbAsics);
  vDigisStsFirstChan.resize(kuNbAsics);
  vDigisStsLastChan.resize(kuNbAsics);

  /// Time windows
  Double_t dHodoWinStart = (1 + 2 * uHodoWinLimClk) * kdClkCycle / 2 * -1;
  Double_t dHodoWinStop  = (1 + 2 * uHodoWinLimClk) * kdClkCycle / 2;
  Double_t dStsWinStart  = (1 + 2 * uStsWinLimClk) * kdClkCycle / 2 * -1;
  Double_t dStsWinStop   = (1 + 2 * uStsWinLimClk) * kdClkCycle / 2;

  /// Histograms
  /// Hodo Time difference
  TH1* hHodoTimeDiff     = new TH1D("hHodoTimeDiff",
                                "Time difference between Digis in the front and back hodoscopes; "
                                "T_B - T_A [ns]; Counts [Digis]",
                                (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);
  TH2* hSameHodoTimeDiff = new TH2D("hSameHodoTimeDiff",
                                    "Time difference between Digis in the same hodoscopes; T_m - T_n "
                                    "[ns]; Hodo;  Counts [Digis]",
                                    10000, -0.5, 9999.5, 2, -0.5, 1.5);
  TH2* hHodoTimeDiffEvo  = new TH2D("hHodoTimeDiffEvo",
                                   "Evolution of Time difference between Digis in the front and back "
                                   "hodoscopes; Time in Run [s]; T_B - T_A [ns]; Counts [Digis]",
                                   120, 0, 1200, (2 * uHodoWinLimClk + 1), dHodoWinStart, dHodoWinStop);

  /// Hodo-STS time difference
  TH2* hStsTimeDiff = new TH2D("hStsTimeDiff",
                               "Time difference between Digis on the STS and hodoscopes; T_Sts - "
                               "T_Hodo [ns]; ASIC; Counts [Digis]",
                               (2 * uStsWinLimClk + 1), dStsWinStart, dStsWinStop, kuNbAsics, -0.5, kuNbAsics - 0.5);
  TH2* hStsAsicsTimeDiff =
    new TH2D("hStsAsicsTimeDiff",
             "Time difference between Digis on the First and last channel of "
             "STS ASICs; T_StsM0 - T_StsN127 [ns]; ASIC N; Counts [Digis]",
             (2 * uStsWinLimClk + 1), dStsWinStart, dStsWinStop, kuNbAsics, -0.5, kuNbAsics - 0.5);
  TH2* hSameStsTimeDiff = new TH2D("hSameStsTimeDiff",
                                   "Time difference between Digis on the same STS ASIC; T_m - T_n "
                                   "[ns]; ASIC; Counts [Digis]",
                                   10000, -0.5, 9999.5, kuNbAsics, -0.5, kuNbAsics - 0.5);
  /*
   TH2 * hStsTimeDiffEvo = new TH2D( "hStsTimeDiffEvo", "Evolution of Time difference between Digis on the STS and hodoscopes; Time in Run [s]; T_Sts - T_Hodo [ns]; Counts [Digis]",
                                   120, 0, 1200,
                                   static_cast< Int_t >(dStsWinStop - dStsWinStart), dStsWinStart, dStsWinStop );
*/

  /// Data Input
  std::vector<CbmStsDigi>* vDigisSts = new std::vector<CbmStsDigi>();

  /// File input
  sInputFileName += Form("_%04u.root", uRunId);
  TFile* pFile = new TFile(sInputFileName, "READ");
  gROOT->cd();

  if (nullptr == pFile) return kFALSE;

  TTree* pTree = (TTree*) pFile->Get("cbmsim");

  TBranch* pBranchSts = pTree->GetBranch("CbmStsDigi");
  pTree->SetBranchAddress("StsDigi", &vDigisSts);

  //read the number of entries in the tree
  Long64_t liNbEntries = pTree->GetEntries();

  std::cout << " Nb Entries: " << liNbEntries << " Tree addr: " << pTree << std::endl;

  if (-1 == liNbEntryToRead || liNbEntries < liNbEntryToRead) liNbEntryToRead = liNbEntries;

  for (Long64_t liEntry = 1; liEntry < liNbEntryToRead; liEntry++) {
    pTree->GetEntry(liEntry);

    UInt_t uNbDigisSts = vDigisSts->size();

    if (0 == liEntry % 1000)
      std::cout << "Event " << std::setw(6) << liEntry << " Nb Sts Digis is " << std::setw(6) << uNbDigisSts
                << std::endl;

    vDigisHodoA.clear();
    for (UInt_t uAsic = 0; uAsic < kuNbAsics; ++uAsic)
      vDigisSts[uAsic].clear();
    vDigisHodoB.clear();

    for (UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi) {
      CbmStsDigi& pDigi = vDigisSts->at(uStsDigi);
      Double_t dTime    = pDigi.GetTime();

      UInt_t uAddress = pDigi.GetAddress();

      switch (uAddress) {
        case 0x10008002:
        case 0x12008002: {
          vDigisHodoA.push_back(pDigi);
          break;
        }  // HODO A
        case 0x10008012:
        case 0x12008012: {
          UInt_t uAsic       = pDigi.GetChannel() / kuNbChanPerAsic;
          UInt_t uChanInAsic = pDigi.GetChannel() % kuNbChanPerAsic;
          vDigisSts[uAsic].push_back(pDigi);
          if (0 == uChanInAsic) vDigisStsFirstChan[uAsic].push_back(pDigi);
          if (kuNbChanPerAsic - 1 == uChanInAsic) vDigisStsLastChan[uAsic].push_back(pDigi);
          break;
        }  // STS
        case 0x10008022:
        case 0x12008022: {
          vDigisHodoB.push_back(pDigi);
          break;
        }  // HODO A
        default: break;
      }  // switch( uAddress )
    }    // for( UInt_t uStsDigi = 0; uStsDigi < uNbDigisSts; ++uStsDigi )

    /// Efficient detection rely on time sorted arrays!
    /// Sort the Hodo A array respective to time
    std::sort(vDigisHodoA.begin(), vDigisHodoA.end(), CompareCbmDataTime<CbmDigi>);
    /// Sort the Sts array respective to time
    for (UInt_t uAsic = 0; uAsic < kuNbAsics; ++uAsic) {
      std::sort(vDigisSts[uAsic].begin(), vDigisSts[uAsic].end(), CompareCbmDataTime<CbmDigi>);
      std::sort(vDigisStsFirstChan[uAsic].begin(), vDigisStsFirstChan[uAsic].end(), CompareCbmDataTime<CbmDigi>);
      std::sort(vDigisStsLastChan[uAsic].begin(), vDigisStsLastChan[uAsic].end(), CompareCbmDataTime<CbmDigi>);
    }  // for( UInt_t uAsic = 0; uAsic < kuNbAsics; ++uAsic )
       /// Sort the Hodo A array respective to time
    std::sort(vDigisHodoB.begin(), vDigisHodoB.end(), CompareCbmDataTime<CbmDigi>);

    UInt_t uFirstCandLastDigiHodo = 0;
    std::vector<UInt_t> vuFirstCandLastDigiSts(kuNbAsics, 0);
    Double_t dPrevTime = -1;
    for (UInt_t uDigiA = 0; uDigiA < vDigisHodoA.size(); ++uDigiA) {
      Double_t dTimeA = vDigisHodoA[uDigiA]->GetTime();

      if (-1 < dPrevTime) { hSameHodoTimeDiff->Fill(dTimeA - dPrevTime, 0.); }  // dPrevTime
      dPrevTime = dTimeA;

      for (UInt_t uDigiB = uFirstCandLastDigiHodo; uDigiB < vDigisHodoB.size(); ++uDigiB) {
        Double_t dTimeB        = vDigisHodoB[uDigiB]->GetTime();
        Double_t dTimeDiffHodo = dTimeB - dTimeA;

        hHodoTimeDiff->Fill(dTimeDiffHodo);
        hHodoTimeDiffEvo->Fill(dTimeA * 1e-9, dTimeDiffHodo);

        if (dTimeDiffHodo < dHodoWinStart) {
          uFirstCandLastDigiHodo = uDigiB;
          continue;
        }                                             // if( dTimeDiffHodo < dHodoWinStart )
        if (dHodoWinStop < dTimeDiffHodo) { break; }  // if( dHodoWinStop < dTimeDiffHodo )

      }  // for( UInt_t uDigiB = uFirstCandLastDigiHodo; uDigiB < vDigisHodoB.size(); ++uDigiB )
      for (UInt_t uAsic = 0; uAsic < kuNbAsics; ++uAsic)
        for (UInt_t uSts = vuFirstCandLastDigiSts[uAsic]; uSts < vDigisSts[uAsic].size(); ++uSts) {
          Double_t dTimeSts     = vDigisSts[uAsic][uSts]->GetTime();
          Double_t dTimeDiffSts = dTimeSts - dTimeA;

          hStsTimeDiff->Fill(dTimeDiffSts, uAsic);

          if (dTimeDiffSts < dStsWinStart) {
            vuFirstCandLastDigiSts[uAsic] = uSts;
            continue;
          }                                           // if( dTimeDiffSts < dStsWinStart )
          if (dStsWinStop < dTimeDiffSts) { break; }  // if( dStsWinStop < dTimeDiffSts )
        }  // for( UInt_t uSts = uFirstCandLastDigiSts[ uAsic ]; uSts < vDigisSts[ uAsic ].size(); ++uSts )
    }      // for( UInt_t uDigiA = 0; uDigiA < vDigisHodoA.size(); ++uDigiA )

    dPrevTime = -1;
    for (UInt_t uDigiB = 0; uDigiB < vDigisHodoB.size(); ++uDigiB) {
      Double_t dTimeB = vDigisHodoB[uDigiB]->GetTime();

      if (-1 < dPrevTime) { hSameHodoTimeDiff->Fill(dTimeB - dPrevTime, 1.); }  // dPrevTime
      dPrevTime = dTimeB;
    }  // for( UInt_t uDigiB = 0; uDigiB < vDigisHodoB.size(); ++uDigiB )

    dPrevTime = -1;
    for (UInt_t uAsic = 0; uAsic < kuNbAsics; ++uAsic) {
      /// Check interval between consecutive digis
      for (UInt_t uSts = 0; uSts < vDigisSts[uAsic].size(); ++uSts) {
        Double_t dTimeSts = vDigisSts[uAsic][uSts]->GetTime();

        if (-1 < dPrevTime) { hSameStsTimeDiff->Fill(dTimeSts - dPrevTime, uAsic); }  // dPrevTime
        dPrevTime = dTimeSts;
      }  // for( UInt_t uSts = 0; uSts < vDigisSts[ uAsic ].size(); ++uSts )

      /// Check time offset between ASICs
      UInt_t uFirstCandLastDigi = 0;
      UInt_t uNextAsic = (uAsic + 1) % kuNbAsics;  /// Loop over thanks to P and N counting in reverse channel order

      for (UInt_t uStsA = 0; uStsA < vDigisStsLastChan[uAsic].size(); ++uStsA) {
        Double_t dTimeStsA = vDigisStsLastChan[uAsic][uStsA]->GetTime();
        for (UInt_t uStsB = uFirstCandLastDigi; uStsB < vDigisStsFirstChan[uNextAsic].size(); ++uStsB) {
          Double_t dTimeStsB    = vDigisStsFirstChan[uNextAsic][uStsB]->GetTime();
          Double_t dTimeDiffSts = dTimeStsB - dTimeStsA;

          hStsAsicsTimeDiff->Fill(dTimeDiffSts, uAsic);

          if (dTimeDiffSts < dStsWinStart) {
            uFirstCandLastDigi = uStsB;
            continue;
          }                                           // if( dTimeDiffSts < dStsWinStart )
          if (dStsWinStop < dTimeDiffSts) { break; }  // if( dStsWinStop < dTimeDiffSts )
        }  // for( UInt_t uStsB = uFirstCandLastDigi; uStsB < vDigisStsFirstChan[ uNextAsic ].size(); ++uStsB )
      }    // for( UInt_t uStsA = 0; uStsA < vDigisStsLastChan[ uAsic ].size(); ++uStsA )
    }      // for( UInt_t uAsic = 0; uAsic < kuNbAsics; ++uAsic )
  }        // for( Long64_t liEntry = 0; liEntry < nentries; liEntry++)

  pFile->Close();

  /// Analysis

  /// Displaying

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
  gPad->SetLogz();
  hStsTimeDiff->Draw("colz");

  TCanvas* cTimeDiffSame = new TCanvas("cTimeDiffSame", "Time difference between same ASIC digis in STS and HODO");
  cTimeDiffSame->Divide(1, 2);

  cTimeDiffSame->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hSameHodoTimeDiff->Draw("colz");

  cTimeDiffSame->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hSameStsTimeDiff->Draw("colz");

  /// Histos anc Canvases saving
  TFile* outFile = new TFile(Form("data/FindOffsetCosy2019_%04u.root", uRunId), "recreate");
  outFile->cd();

  hHodoTimeDiff->Write();
  hHodoTimeDiffEvo->Write();
  hStsTimeDiff->Write();

  hSameHodoTimeDiff->Write();
  hStsAsicsTimeDiff->Write();
  hSameStsTimeDiff->Write();

  cTimeDiff->Write();
  cTimeDiffSame->Write();

  gROOT->cd();
  outFile->Close();

  return kTRUE;
}
