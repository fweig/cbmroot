/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

Bool_t PlotClusterCharge(Long64_t liNbEntryToRead = -1, UInt_t uRunId = 12)
{
  /// Data Input
  TClonesArray* pClustersArraySts = new TClonesArray("CbmStsCluster", 5000);

  /// Histograms
  /// Cluster Charge VS position
  TH2* hClustChargeHodoA = new TH2D("hClustChargeHodoA",
                                    "Cluster charge vs cluster position in Hodo A; Cluster position "
                                    "(strip) []; Cluster Charge []; Counts [Clusters]",
                                    128, -0.5, 127.5, 200, 0., 400000.);
  TH2* hClustChargeSts   = new TH2D("hClustChargeSts",
                                  "Cluster charge vs cluster position in Hodo B; Cluster position "
                                  "(strip) []; Cluster Charge []; Counts [Clusters]",
                                  2048, -0.5, 2047.5, 200, 0., 400000.);
  TH2* hClustChargeHodoB = new TH2D("hClustChargeHodoB",
                                    "Cluster charge vs cluster position in Hodo B; Cluster position "
                                    "(strip) []; Cluster Charge []; Counts [Clusters]",
                                    128, -0.5, 127.5, 200, 0., 400000.);
  /// Cluster Charge VS size
  TH2* hClustChargeSizeHodoA = new TH2D("hClustChargeSizeHodoA",
                                        "Cluster charge vs cluster size in Hodo A; Cluster size [chan]; "
                                        "Cluster Charge []; Counts [Clusters]",
                                        128, -0.5, 127.5, 200, 0., 400000.);
  TH2* hClustChargeSizeSts   = new TH2D("hClustChargeSizeSts",
                                      "Cluster charge vs cluster size in Hodo B; Cluster size [chan]; "
                                      "Cluster Charge []; Counts [Clusters]",
                                      255, -0.5, 254.5, 200, 0., 400000.);
  TH2* hClustChargeSizeHodoB = new TH2D("hClustChargeSizeHodoB",
                                        "Cluster charge vs cluster size in Hodo B; Cluster position "
                                        "[chan]; Cluster Charge []; Counts [Clusters]",
                                        128, -0.5, 127.5, 200, 0., 400000.);
  /// Cluster Size VS position
  TH2* hClustSizePosHodoA = new TH2D("hClustSizePosHodoA",
                                     "Cluster size vs cluster position in Hodo A; Cluster position "
                                     "(strip) []; Cluster size [chan]; Counts [Clusters]",
                                     128, -0.5, 127.5, 128, -0.5, 127.5);
  TH2* hClustSizePosSts   = new TH2D("hClustSizePosSts",
                                   "Cluster size vs cluster position in Hodo B; Cluster position "
                                   "(strip) []; Cluster size [chan]; Counts [Clusters]",
                                   2048, -0.5, 2047.5, 255, -0.5, 254.5);
  TH2* hClustSizePosHodoB = new TH2D("hClustSizePosHodoB",
                                     "Cluster size vs cluster position in Hodo B; Cluster position "
                                     "(strip) []; Cluster position [chan]; Counts [Clusters]",
                                     128, -0.5, 127.5, 128, -0.5, 127.5);


  /// Data input
  TString sInputFileName = Form("data/cosy2019_%04u.rec.root", uRunId);
  TFile* pFile           = new TFile(sInputFileName, "READ");
  gROOT->cd();

  if (nullptr == pFile) return kFALSE;

  TTree* pTree = (TTree*) pFile->Get("cbmsim");

  TBranch* pBranchSts = pTree->GetBranch("StsCluster");
  pBranchSts->SetAddress(&pClustersArraySts);

  //read the number of entries in the tree
  Long64_t liNbEntries = pTree->GetEntries();

  std::cout << " Nb Entries: " << liNbEntries << " Tree addr: " << pTree << " Branch addr Sts: " << pBranchSts
            << std::endl;

  if (-1 == liNbEntryToRead || liNbEntries < liNbEntryToRead) liNbEntryToRead = liNbEntries;

  for (Long64_t liEntry = 3; liEntry < liNbEntryToRead; liEntry++) {
    pTree->GetEntry(liEntry);

    UInt_t uNbClustersSts = pClustersArraySts->GetEntriesFast();

    if (0 == liEntry % 1000)
      std::cout << "Event " << std::setw(6) << liEntry << " Nb Sts clusters is " << std::setw(6) << uNbClustersSts
                << std::endl;

    for (UInt_t uStsClust = 0; uStsClust < uNbClustersSts; ++uStsClust) {
      CbmStsCluster* pClust = static_cast<CbmStsCluster*>(pClustersArraySts->At(uStsClust));

      UInt_t uAddress = pClust->GetAddress();

      switch (uAddress) {
        case 0x10008002:
        case 0x12008002: {
          hClustChargeHodoA->Fill(pClust->GetPosition(), pClust->GetCharge());
          hClustChargeSizeHodoA->Fill(pClust->GetSize(), pClust->GetCharge());
          hClustSizePosHodoA->Fill(pClust->GetPosition(), pClust->GetSize());
          break;
        }  // HODO A
        case 0x10008012:
        case 0x12008012: {
          hClustChargeSts->Fill(pClust->GetPosition(), pClust->GetCharge());
          hClustChargeSizeSts->Fill(pClust->GetSize(), pClust->GetCharge());
          hClustSizePosSts->Fill(pClust->GetPosition(), pClust->GetSize());
          break;
        }  // STS
        case 0x10008022:
        case 0x12008022: {
          hClustChargeHodoB->Fill(pClust->GetPosition(), pClust->GetCharge());
          hClustChargeSizeHodoB->Fill(pClust->GetSize(), pClust->GetCharge());
          hClustSizePosHodoB->Fill(pClust->GetPosition(), pClust->GetSize());
          break;
        }  // HODO A
        default: break;
      }  // switch( uAddress )
    }    // for( UInt_t uStsHit = 0; uStsHit < uNbHitsSts; ++uStsHit )
  }      // for( Long64_t liEntry = 0; liEntry < nentries; liEntry++)

  pFile->Close();

  /// Analysis

  /// Displaying
  TCanvas* cClustCharge = new TCanvas("cClustCharge", "Cluster charge VS position for each detector");
  cClustCharge->Divide(3);

  cClustCharge->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustChargeHodoA->Draw("colz");

  cClustCharge->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustChargeSts->Draw("colz");

  cClustCharge->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustChargeHodoB->Draw("colz");

  TCanvas* cClustChargeSize = new TCanvas("cClustChargeSize", "Cluster charge VS size for each detector");
  cClustChargeSize->Divide(3);

  cClustChargeSize->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustChargeSizeHodoA->Draw("colz");

  cClustChargeSize->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustChargeSizeSts->Draw("colz");

  cClustChargeSize->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustChargeSizeHodoB->Draw("colz");


  TCanvas* cClustSizePos = new TCanvas("cClustSizePos", "Cluster size VS position for each detector");
  cClustSizePos->Divide(3);

  cClustSizePos->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustSizePosHodoA->Draw("colz");

  cClustSizePos->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustSizePosSts->Draw("colz");

  cClustSizePos->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogz();
  hClustSizePosHodoB->Draw("colz");

  /// Histos anc Canvases saving
  TFile* outFile = new TFile(Form("data/PlotClusterCharge_%04u.root", uRunId), "recreate");
  outFile->cd();

  hClustChargeHodoA->Write();
  hClustChargeSts->Write();
  hClustChargeHodoB->Write();

  hClustChargeSizeHodoA->Write();
  hClustChargeSizeSts->Write();
  hClustChargeSizeHodoB->Write();

  hClustSizePosHodoA->Write();
  hClustSizePosSts->Write();
  hClustSizePosHodoB->Write();

  cClustCharge->Write();
  cClustChargeSize->Write();
  cClustSizePos->Write();

  gROOT->cd();
  outFile->Close();

  return kTRUE;
}
