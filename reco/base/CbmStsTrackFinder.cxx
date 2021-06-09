/* Copyright (C) 2005-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Evgeny Lavrik, Florian Uhlig */

// -------------------------------------------------------------------------
// -----                   CbmStsTrackFinder source file               -----
// -----                  Created 02/02/05  by V. Friese               -----
// -------------------------------------------------------------------------


// Empty file, just there to please CINT

#include "CbmStsTrackFinder.h"

#include "CbmDigiManager.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"

#include "FairRootManager.h"

#include "TClonesArray.h"

CbmStsTrackFinder::CbmStsTrackFinder()
  : TNamed()
  , fDigiScheme(nullptr)
  , fField(nullptr)
  , fMvdHits(nullptr)
  , fStsHits(nullptr)
  , fTracks(nullptr)
  , fStsClusters(nullptr)
  , fVerbose(0)
{
}

double CbmStsTrackFinder::VecMedian(std::vector<double>& vec)
{
  if (vec.empty()) { return 0.; }

  auto mid = vec.size() / 2;
  std::nth_element(vec.begin(), vec.begin() + mid, vec.end());
  auto median = vec[mid];
  if (!(vec.size() & 1)) {
    auto max_it = std::max_element(vec.begin(), vec.begin() + mid);
    median      = (*max_it + median) / 2.0;
  }
  return median;
}

double CbmStsTrackFinder::CalculateEloss(CbmStsTrack* cbmStsTrack)
{
  if (!fStsClusters) {
    FairRootManager* ioman = FairRootManager::Instance();
    assert(ioman);

    fStsClusters = (TClonesArray*) ioman->GetObject("StsCluster");
    assert(fStsClusters);
  }

  CbmDigiManager* digiManager = CbmDigiManager::Instance();

  std::vector<double> dEdxAllveto;

  double dr = 1.;
  for (int iHit = 0; iHit < cbmStsTrack->GetNofStsHits(); ++iHit) {
    bool frontVeto = kFALSE, backVeto = kFALSE;
    CbmStsHit* stsHit = (CbmStsHit*) fStsHits->At(cbmStsTrack->GetStsHitIndex(iHit));

    double x, y, z, xNext, yNext, zNext;
    x = stsHit->GetX();
    y = stsHit->GetY();
    z = stsHit->GetZ();

    if (iHit != cbmStsTrack->GetNofStsHits() - 1) {
      CbmStsHit* stsHitNext = (CbmStsHit*) fStsHits->At(cbmStsTrack->GetStsHitIndex(iHit + 1));
      xNext                 = stsHitNext->GetX();
      yNext                 = stsHitNext->GetY();
      zNext                 = stsHitNext->GetZ();
      dr                    = sqrt((xNext - x) * (xNext - x) + (yNext - y) * (yNext - y) + (zNext - z) * (zNext - z))
           / (zNext - z);  // if *300um, you get real reconstructed dr
    }                      // else dr stay previous

    CbmStsCluster* frontCluster = (CbmStsCluster*) fStsClusters->At(stsHit->GetFrontClusterId());
    CbmStsCluster* backCluster  = (CbmStsCluster*) fStsClusters->At(stsHit->GetBackClusterId());

    if (!frontCluster || !backCluster) {
      LOG(info) << "CbmStsTrackFinder::CalculateEloss: no front or back cluster";
      continue;
    }

    //check if at least one digi in a cluster has overflow --- charge is registered in the last ADC channel #31
    for (int iDigi = 0; iDigi < frontCluster->GetNofDigis(); ++iDigi) {
      if (CbmStsTrackFinder::MaxAdcVal() == (digiManager->Get<CbmStsDigi>(frontCluster->GetDigi(iDigi))->GetCharge()))
        frontVeto = kTRUE;
    }
    for (int iDigi = 0; iDigi < backCluster->GetNofDigis(); ++iDigi) {
      if (CbmStsTrackFinder::MaxAdcVal() == (digiManager->Get<CbmStsDigi>(backCluster->GetDigi(iDigi))->GetCharge()))
        backVeto = kTRUE;
    }

    if (!frontVeto) dEdxAllveto.push_back((frontCluster->GetCharge()) / dr);
    if (!backVeto) dEdxAllveto.push_back((backCluster->GetCharge()) / dr);
  }

  float dEdXSTS = CbmStsTrack::ELossOverflow();
  if (dEdxAllveto.size() != 0) dEdXSTS = VecMedian(dEdxAllveto);
  return dEdXSTS;
}

void CbmStsTrackFinder::FillEloss()
{
  Int_t nStsTracks = fTracks->GetEntriesFast();
  for (Int_t stsTrackIndex = 0; stsTrackIndex < nStsTracks; stsTrackIndex++) {
    CbmStsTrack* cbmStsTrack = (CbmStsTrack*) fTracks->At(stsTrackIndex);
    double dEdXSTS           = CalculateEloss(cbmStsTrack);
    cbmStsTrack->SetELoss(dEdXSTS);
  }
}

ClassImp(CbmStsTrackFinder)
