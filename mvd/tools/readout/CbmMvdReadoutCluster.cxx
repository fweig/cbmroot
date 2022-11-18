/* Copyright (C) 2016-2021 Institut furr Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Philipp Sitzmann [committer] */

// -------------------------------------------------------------------------
// -----                  CbmMvdReadoutCluster source file             -----
// -----                  Created 17/10/16 by P. Sitzmann              -----
// -------------------------------------------------------------------------

#include "CbmMvdReadoutCluster.h"

#include "CbmMvdCluster.h"

//-- Include from Fair --//
#include <FairRootManager.h>
#include <Logger.h>

//-- Include from Root --//
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TMath.h"
#include <TFile.h>

#include <iostream>
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::map;
using std::pair;

// -----   Default constructor   -------------------------------------------
CbmMvdReadoutCluster::CbmMvdReadoutCluster() : CbmMvdReadoutCluster::CbmMvdReadoutCluster("MvdReadoutCluster", 0) {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMvdReadoutCluster::CbmMvdReadoutCluster(const char* name, Int_t iVerbose)
  : FairTask(name, iVerbose)
  , foutFile(nullptr)
  , fshow(kFALSE)
  , fWordsPerRegion()
  , fWordsPerSuperRegion()
  , fMvdCluster(nullptr)
  , fEventNumber(0)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMvdReadoutCluster::~CbmMvdReadoutCluster() { ; }
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
InitStatus CbmMvdReadoutCluster::Init()
{
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) { LOG(fatal) << "RootManager not instantised!"; }

  fMvdCluster = (TClonesArray*) ioman->GetObject("MvdCluster");

  if (!fMvdCluster) LOG(fatal) << "Data array missing";

  SetupHistograms();

  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutCluster::SetupHistograms()
{
  for (Int_t i = 0; i < 350; ++i) {
    fWordsPerRegion[i]      = new TH1F(Form("fWordsPerRegionSensor%d", i), "Words send to a region", 65, 0, 64);
    fWordsPerSuperRegion[i] = new TH1F(Form("fWordsPerSuperRegionSensor%d", i), "Words send to a region", 16, 0, 15);
  }
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmMvdReadoutCluster::Exec(Option_t* /*opt*/)
{
  LOG(debug) << "//--------------- New Event " << fEventNumber << " -----------------------\\";

  for (Int_t nClusters = 0; nClusters < fMvdCluster->GetEntriesFast(); ++nClusters) {
    CbmMvdCluster* cluster                  = (CbmMvdCluster*) fMvdCluster->At(nClusters);
    Int_t sensorNr                          = cluster->GetSensorNr();
    map<pair<Int_t, Int_t>, Int_t> pixelMap = cluster->GetPixelMap();

    for (auto cntr = pixelMap.begin(); cntr != pixelMap.end(); ++cntr) {
      Int_t xPix = cntr->first.first;
      // Int_t yPix = cntr->first.second;
      Int_t regionNr = (Int_t)(xPix / fPixelsPerRegion);
      fWordsPerRegion[sensorNr]->Fill(regionNr);
      Int_t superRegionNr = (Int_t)(xPix / fPixelsPerSuperRegion);
      fWordsPerSuperRegion[sensorNr]->Fill(superRegionNr);
    }
  }

  LOG(debug) << "//--------------- End Event -----------------------\\";
  fEventNumber++;
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutCluster::Finish()
{
  foutFile->cd();

  Float_t scale = 1. / (Float_t) fEventNumber;
  for (Int_t i = 0; i < 350; ++i) {
    fWordsPerRegion[i]->Scale(scale, "nosw2");
    fWordsPerSuperRegion[i]->Scale(scale, "nosw2");
  }

  if (fshow) DrawHistograms();
  WriteHistograms();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutCluster::WriteHistograms()
{

  for (Int_t i = 0; i < 350; ++i) {
    fWordsPerRegion[i]->Write();
  }

  for (Int_t i = 0; i < 350; ++i) {
    fWordsPerSuperRegion[i]->Write();
  }
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void CbmMvdReadoutCluster::DrawHistograms() {}
// -------------------------------------------------------------------------

ClassImp(CbmMvdReadoutCluster);
