/* Copyright (C) 2019-2020 IKF-UFra, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina [committer] */

/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: V. Akishina
 *
 *  e-mail : v.akishina@gsi.de 
 *
 *====================================================================
 *
 *  CbmL1GlobalTrackFinder source file
 *
 *====================================================================
 */
#include "CbmL1GlobalTrackFinder.h"

#include "CbmEvent.h"
#include "CbmGlobalTrack.h"
#include "CbmKFMath.h"
#include "CbmMuchTrack.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "CbmTofTrack.h"
#include "CbmTrack.h"
#include "CbmTrdTrack.h"

#include "FairHit.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"

#include "TClonesArray.h"
#include "TDatabasePDG.h"

#include <iostream>
#include <vector>

#include "L1Algo/L1Algo.h"

using std::cout;
using std::endl;
using std::vector;

ClassImp(CbmL1GlobalTrackFinder)

  // -----   Default constructor   -------------------------------------------
  CbmL1GlobalTrackFinder::CbmL1GlobalTrackFinder()
  : fGlobalTracks(nullptr)
  , fStsTracks(nullptr)
  , fMuchTracks(nullptr)
  , fTrdTracks(nullptr)
  , fTofTracks(nullptr)
{
  fName = "Global Track Finder L1";
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmL1GlobalTrackFinder::~CbmL1GlobalTrackFinder() {}
// -------------------------------------------------------------------------

// -----   Public method Init   --------------------------------------------
void CbmL1GlobalTrackFinder::Init() {}
// -------------------------------------------------------------------------


// -----   Copy tracks to output array   -----------------------------------
Int_t CbmL1GlobalTrackFinder::CopyL1Tracks(CbmEvent* event)
{
  CbmL1* L1 = CbmL1::Instance();
  if (!L1) return 0;
  Int_t globalTrackIndex = fGlobalTracks->GetEntriesFast();

  Int_t stsTrackIndex  = fStsTracks->GetEntriesFast();
  Int_t muchTrackIndex = fMuchTracks->GetEntriesFast();
  Int_t trdTrackIndex  = fTrdTracks->GetEntriesFast();
  Int_t tofTrackIndex  = fTofTracks->GetEntriesFast();

  LOG(debug) << "Copy L1 tracks : " << L1->fvRecoTracks.size() << " tracks in L1";

  for (vector<CbmL1Track>::iterator it = L1->fvRecoTracks.begin(); it != L1->fvRecoTracks.end(); ++it) {
    CbmL1Track& T = *it;
    //BEGIN add global track
    new ((*fGlobalTracks)[globalTrackIndex]) CbmGlobalTrack();
    if (event) event->AddData(ECbmDataType::kGlobalTrack, globalTrackIndex);
    CbmGlobalTrack* t = L1_DYNAMIC_CAST<CbmGlobalTrack*>(fGlobalTracks->At(globalTrackIndex++));
    t->SetFlag(0);
    FairTrackParam fpar(*t->GetParamFirst()), lpar(*t->GetParamLast());
    CbmKFMath::CopyTC2TrackParam(&fpar, T.T, T.C);
    CbmKFMath::CopyTC2TrackParam(&lpar, T.TLast, T.CLast);
    t->SetParamFirst(&fpar);
    t->SetParamLast(&lpar);
    t->SetChi2(T.chi2);
    //   t->SetLength(T.length);
    t->SetNDF(T.NDF);
    t->SetPidHypo(T.T[4] >= 0 ? 211 : -211);
    //     t->SetTime(T.Tpv[6]);
    //     t->SetTimeError(T.Cpv[20]);
    //END add global track
    //BEGIN create detector tracks if needed
    bool hasStsHits  = false;
    bool hasMuchHits = false;
    bool hasTrdHits  = false;
    bool hasTofHits  = false;
    for (vector<int>::iterator ih = it->Hits.begin(); ih != it->Hits.end(); ++ih) {
      CbmL1HitStore& h = L1->fvHitStore[*ih];

      if (h.Det == 1 && hasStsHits == false) {
        hasStsHits         = true;
        CbmStsTrack* track = new ((*fStsTracks)[stsTrackIndex]) CbmStsTrack();
        t->SetStsTrackIndex(stsTrackIndex);
        if (event) event->AddData(ECbmDataType::kStsTrack, stsTrackIndex);
        CbmL1TrackToCbmStsTrack(T, track, h.Det);
        track->AddStsHit(h.ExtIndex);
        stsTrackIndex++;
      }
      if (h.Det == 2 && hasMuchHits == false) {
        hasMuchHits = true;

        CbmMuchTrack* track = new ((*fMuchTracks)[muchTrackIndex]) CbmMuchTrack();
        t->SetMuchTrackIndex(muchTrackIndex);
        if (event) event->AddData(ECbmDataType::kMuchTrack, muchTrackIndex);
        CbmL1TrackToCbmMuchTrack(T, track, h.Det);
        track->AddMuchHit(h.ExtIndex);
        muchTrackIndex++;
      }
      if (h.Det == 3 && hasTrdHits == false) {
        hasTrdHits         = true;
        CbmTrdTrack* track = new ((*fTrdTracks)[trdTrackIndex]) CbmTrdTrack();
        t->SetTrdTrackIndex(trdTrackIndex);
        if (event) event->AddData(ECbmDataType::kTrdTrack, trdTrackIndex);
        CbmL1TrackToCbmTrdTrack(T, track, h.Det);
        track->AddTrdHit(h.ExtIndex);
        trdTrackIndex++;
      }
      if (h.Det == 4 && hasTofHits == false) {
        hasTofHits         = true;
        CbmTofTrack* track = new ((*fTofTracks)[tofTrackIndex]) CbmTofTrack();

        t->SetTofTrackIndex(tofTrackIndex);

        if (event) event->AddData(ECbmDataType::kTofTrack, tofTrackIndex);
        CbmL1TrackToCbmTofTrack(T, track, h.Det);
        tofTrackIndex++;
        track->AddTofHit(h.ExtIndex);

        if (event) event->AddData(ECbmDataType::kTofHit, h.ExtIndex);
      }
    }
    //END create detector tracks if needed
  }
  return globalTrackIndex;
}
// -------------------------------------------------------------------------


// -----   Public method CbmL1TrackToCbmTrack   ------------------------------------------
void CbmL1GlobalTrackFinder::CbmL1TrackToCbmTrack(CbmL1Track l1track, CbmTrack* track, int systemIdT)
{
  Int_t ndf = 0;

  CbmL1Track* T = &l1track;

  CbmL1* L1 = CbmL1::Instance();

  for (vector<int>::iterator ih = T->Hits.begin(); ih != T->Hits.end(); ++ih) {
    CbmL1HitStore& h = L1->fvHitStore[*ih];
    if (h.Det != systemIdT) continue;
  }
  ndf -= 5;
  if (ndf <= 0) ndf = 1;
  track->SetChiSq(T->chi2);
  track->SetNDF(ndf);
  //track->SetPreviousTrackId(T->GetPreviousTrackId());//???
  //track->SetFlag(T->GetQuality());//???

  FairTrackParam fpar(*track->GetParamFirst()), lpar(*track->GetParamLast());
  CbmKFMath::CopyTC2TrackParam(&fpar, T->T, T->C);
  CbmKFMath::CopyTC2TrackParam(&lpar, T->TLast, T->CLast);
}
// -------------------------------------------------------------------------

// -----   Public method CbmL1TrackToCbmStsTrack   ------------------------------------------
void CbmL1GlobalTrackFinder::CbmL1TrackToCbmStsTrack(CbmL1Track l1track, CbmStsTrack* track, int systemIdT)
{
  Int_t ndf = 0;

  CbmL1Track* T = &l1track;

  CbmL1* L1 = CbmL1::Instance();

  for (vector<int>::iterator ih = T->Hits.begin(); ih != T->Hits.end(); ++ih) {
    CbmL1HitStore& h = L1->fvHitStore[*ih];
    if (h.Det != systemIdT) continue;
    track->AddHit(h.ExtIndex, kSTSHIT);
  }

  ndf -= 5;
  if (ndf <= 0) ndf = 1;


  track->SetFlag(0);
  track->SetChiSq(T->chi2);
  track->SetNDF(T->NDF);
  track->SetPidHypo(T->T[4] >= 0 ? 211 : -211);
  track->SetTime(T->Tpv[6]);
  track->SetTimeError(T->Cpv[20]);

  FairTrackParam fpar(*track->GetParamFirst()), lpar(*track->GetParamLast());
  CbmKFMath::CopyTC2TrackParam(&fpar, T->T, T->C);
  CbmKFMath::CopyTC2TrackParam(&lpar, T->TLast, T->CLast);
}
// -------------------------------------------------------------------------

// -----   Public method CbmL1TrackToCbmMuchTrack   ------------------------------------------
void CbmL1GlobalTrackFinder::CbmL1TrackToCbmMuchTrack(CbmL1Track l1track, CbmMuchTrack* track, int systemIdT)
{
  Int_t ndf = 0;

  CbmL1Track* T = &l1track;

  CbmL1* L1 = CbmL1::Instance();

  for (vector<int>::iterator ih = T->Hits.begin(); ih != T->Hits.end(); ++ih) {
    CbmL1HitStore& h = L1->fvHitStore[*ih];
    if (h.Det != systemIdT) continue;
    track->AddHit(h.ExtIndex, kMUCHPIXELHIT);
  }
  ndf -= 5;
  if (ndf <= 0) ndf = 1;

  track->SetChiSq(T->chi2);
  track->SetNDF(ndf);
  //track->SetPreviousTrackId(T->GetPreviousTrackId());//???
  //track->SetFlag(T->GetQuality());//???

  FairTrackParam fpar(*track->GetParamFirst()), lpar(*track->GetParamLast());
  CbmKFMath::CopyTC2TrackParam(&fpar, T->T, T->C);
  CbmKFMath::CopyTC2TrackParam(&lpar, T->TLast, T->CLast);
}
// -------------------------------------------------------------------------

// -----   Public method CbmL1TrackToCbmTrdTrack   ------------------------------------------
void CbmL1GlobalTrackFinder::CbmL1TrackToCbmTrdTrack(CbmL1Track l1track, CbmTrdTrack* track, int systemIdT)
{
  Int_t ndf = 0;

  CbmL1Track* T = &l1track;

  CbmL1* L1 = CbmL1::Instance();

  for (vector<int>::iterator ih = T->Hits.begin(); ih != T->Hits.end(); ++ih) {
    CbmL1HitStore& h = L1->fvHitStore[*ih];
    if (h.Det != systemIdT) continue;
    track->AddHit(h.ExtIndex, kTRDHIT);
  }
  ndf -= 5;
  if (ndf <= 0) ndf = 1;


  track->SetChiSq(T->chi2);
  track->SetNDF(ndf);
  //track->SetPreviousTrackId(T->GetPreviousTrackId());//???
  //track->SetFlag(T->GetQuality());//???

  FairTrackParam fpar(*track->GetParamFirst()), lpar(*track->GetParamLast());
  CbmKFMath::CopyTC2TrackParam(&fpar, T->Tpv, T->Cpv);
  CbmKFMath::CopyTC2TrackParam(&lpar, T->TLast, T->CLast);
}
// -------------------------------------------------------------------------

// -----   Public method CbmL1TrackToCbmTofTrack   ------------------------------------------
void CbmL1GlobalTrackFinder::CbmL1TrackToCbmTofTrack(CbmL1Track l1track, CbmTofTrack* /*track*/, int systemIdT)
{
  Int_t ndf = 0;

  CbmL1Track* T = &l1track;

  CbmL1* L1 = CbmL1::Instance();

  for (vector<int>::iterator ih = T->Hits.begin(); ih != T->Hits.end(); ++ih) {
    CbmL1HitStore& h = L1->fvHitStore[*ih];
    if (h.Det != systemIdT) continue;
    // track->AddHit(h.ExtIndex, kTOFHIT);
  }
  ndf -= 5;
  if (ndf <= 0) ndf = 1;

  //track->SetChiSq(T->chi2);
  //track->SetNDF(ndf);
  //track->SetPreviousTrackId(T->GetPreviousTrackId());//???
  //track->SetFlag(T->GetQuality());//???

  //FairTrackParam fpar(*track->GetParamFirst()), lpar(*track->GetParamLast());
  //CbmKFMath::CopyTC2TrackParam(&fpar, T->T, T->C);
  //CbmKFMath::CopyTC2TrackParam(&lpar, T->TLast, T->CLast);
}
// -------------------------------------------------------------------------

// -----   Public method DoFind   ------------------------------------------
Int_t CbmL1GlobalTrackFinder::DoFind()
{

  if (!fTracks) {
    LOG(error) << "-E- CbmL1GlobalTrackFinder::DoFind: "
               << "Track array missing! ";
    return -1;
  }

  CbmL1* L1 = CbmL1::Instance();
  if (!L1) return 0;

  L1->Reconstruct();
  int ntracks = CopyL1Tracks();

  return ntracks;
}
// -------------------------------------------------------------------------


// -----   Track finding in one event   ------------------------------------
Int_t CbmL1GlobalTrackFinder::FindTracks(CbmEvent* event)
{

  CbmL1* l1 = CbmL1::Instance();
  if (!l1) return 0;

  l1->Reconstruct(event);
  int nTracks = CopyL1Tracks(event);

  return nTracks;
}
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void CbmL1GlobalTrackFinder::SetDefaultParticlePDG(int pdg)
{
  /// set a default particle mass for the track fit
  /// it is used during reconstruction for the multiple scattering estimation
  CbmL1* l1 = CbmL1::Instance();
  if (!l1 || !l1->fpAlgo) {
    LOG(fatal) << "L1 instance doesn't exist or is not initialised";
    return;
  }
  auto* p = TDatabasePDG::Instance()->GetParticle(pdg);
  if (!p) {
    LOG(fatal) << "Particle with pdg " << pdg << " doesn't exist";
    return;
  }
  l1->fpAlgo->SetDefaultParticleMass(p->Mass());
}
