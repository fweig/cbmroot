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
#include "CbmKFMath.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"

#include "FairHit.h"
#include "FairMCPoint.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iostream>
#include <vector>

#include "L1Algo/L1Algo.h"

using std::cout;
using std::endl;
using std::vector;

ClassImp(CbmL1GlobalTrackFinder)

  // -----   Default constructor   -------------------------------------------
  CbmL1GlobalTrackFinder::CbmL1GlobalTrackFinder()
  : FairTask("CbmL1GlobalTrackFinder")
  , fEventNo(0)
{
  fName = "Global Track Finder L1";
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmL1GlobalTrackFinder::~CbmL1GlobalTrackFinder() {}
// -------------------------------------------------------------------------

// -----   Public method Init   --------------------------------------------
InitStatus CbmL1GlobalTrackFinder::Init()
{

  FairRootManager* ioman = FairRootManager::Instance();

  // Create and register track arrays
  fGlobalTracks = new TClonesArray("CbmGlobalTrack", 100);
  ioman->Register("GlobalTrack", "Global", fGlobalTracks, "GlobalTrack");

  {
    fMuchTracks = new TClonesArray("CbmMuchTrack", 100);
    ioman->Register("MuchTrack", "Much", fMuchTracks, "MuchTrack");
  }

  {
    fTrdTracks = new TClonesArray("CbmTrdTrack", 100);
    ioman->Register("TrdTrack", "Trd", fTrdTracks, "TrdTrack");
  }

  {
    fTofTracks = new TClonesArray("CbmTofTrack", 100);
    ioman->Register("TofTrack", "Tof", fTofTracks, "TofTrack");
  }

  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));

  LOG(info) << fEvents << " fEvents ";

  if (!fEvents) { LOG(warn) << GetName() << ": No event array! Will process entire tree."; }
  // --- Get input array (StsHits)
  fStsHits = (TClonesArray*) ioman->GetObject("StsHit");

  // Array of MvdHits
  fMvdHits = (TClonesArray*) ioman->GetObject("MvdHit");

  // Screen output
  LOG(info) << GetName() << ": successfully initialised.";
  LOG(info) << "=====================================";
  return kSUCCESS;
}

void CbmL1GlobalTrackFinder::Exec(Option_t* /*opt*/)
{

  // --- Clear output array
  fTracks->Delete();

  // --- Event loop (from event objects)
  if (fEvents) {
    Int_t nEvents = fEvents->GetEntriesFast();
    LOG(debug) << GetName() << ": reading time slice with " << nEvents << " events ";
    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {

      LOG(debug) << fEvents << " fEvents " << iEvent << " iEvent ";
      CbmEvent* event = static_cast<CbmEvent*>(fEvents->At(iEvent));
      ProcessEvent(event);
    }  //# events
  }    //? event branch present

  else  // Old event-by-event simulation without event branch
  {
    ProcessEvent(NULL);
    LOG(info) << "CbmL1GlobalTrackFinder::Exec event: " << fEventNo++;
  }
}

// ------   Process one event   --------------------------------------------
void CbmL1GlobalTrackFinder::ProcessEvent(CbmEvent* event)
{

  // --- Call track finder
  fTimer.Start();
  Int_t nTracks = FindTracks(event);
  fTimer.Stop();

  // --- Event log
  if (fEvents) {
    Int_t eventNumber = (event ? event->GetNumber() : fNofEvents);
    Int_t nHits       = (event ? event->GetNofData(kStsHit) : fStsHits->GetEntriesFast());
    LOG(info) << "+ " << setw(20) << GetName() << ": Event " << setw(6) << right << eventNumber << ", real time "
              << fixed << setprecision(6) << fTimer.RealTime() << " s, hits: " << nHits << ", tracks: " << nTracks << ;

    // --- Counters
    fNofEvents++;
    fNofHits += Double_t(nHits);
    fNofTracks += Double_t(nTracks);
    fTime += fTimer.RealTime();
  }
}


// -------------------------------------------------------------------------

Int_t CbmL1GlobalTrackFinder::CreateGlobalTrackArray(CbmEvent* event, TClonesArray* globalTracks,
                                                     TClonesArray* stsTracks, TClonesArray* trdTracks,
                                                     TClonesArray* muchTracks, TClonesArray* tofTracks)
{

  CbmL1* L1 = CbmL1::Instance();
  if (!L1) return 0;

  Int_t nTracks = 0;

  // Loop over STS tracks and create GlobalTrack for each StsTrack

  Int_t globalTrackNo = globalTracks->GetEntriesFast();

  Int_t stsTrackNo  = (stsTracks != NULL) ? stsTracks->GetEntriesFast() : 0;
  Int_t trdTrackNo  = (trdTracks != NULL) ? trdTracks->GetEntriesFast() : 0;
  Int_t muchTrackNo = (muchTracks != NULL) ? muchTracks->GetEntriesFast() : 0;
  Int_t tofTrackNo  = (tofTracks != NULL) ? tofTracks->GetEntriesFast() : 0;

  for (vector<CbmL1Track>::iterator it = L1->vRTracks.begin(); it != L1->vRTracks.end(); ++it) {

    CbmL1Track& T = *it;

    CbmGlobalTrack* globalTrack = new ((*globalTracks)[globalTrackNo]) CbmGlobalTrack();

    globalTrack->SetStsTrackIndex(globalTrackNo);

    Int_t iTrack = event ? event->GetIndex(kStsTrack, globalTrackNo) : globalTrackNo;

    globalTrackNo++;
    nTracks++;

    if (event) event->AddData(kGlobalTrack, iTrack);

    CbmStsTrack* trackSts   = new CbmStsTrack();
    CbmMuchTrack* trackMuch = new CbmMuchTrack();
    CbmTrdTrack* trackTrd   = new CbmTrdTrack();
    CbmTofTrack* trackTof   = new CbmTofTrack();

    // Set last parameter of the CbmGlobal track to be last parameter of CbmLitTrack
    FairTrackParam fpar(*globalTrack->GetParamFirst()), lpar(*globalTrack->GetParamLast());
    CbmKFMath::CopyTC2TrackParam(&fpar, T.T, T.C);
    CbmKFMath::CopyTC2TrackParam(&lpar, T.TLast, T.CLast);
    globalTrack->SetParamLast(&lpar);
    globalTrack->SetParamFirst(&fpar);

    trackSts->SetParamLast(&lpar);
    trackSts->SetParamFirst(&fpar);

    trackMuch->SetParamLast(&lpar);
    trackMuch->SetParamFirst(&fpar);

    trackTrd->SetParamLast(&lpar);
    trackTrd->SetParamFirst(&fpar);

    for (vector<int>::iterator ih = it->StsHits.begin(); ih != it->StsHits.end(); ++ih) {
      CbmL1HitStore& h = L1->vHitStore[*ih];

      if (h.Det == 2) {
        //CbmLitTrackToCbmTrack(litTrack, trackMuch, kLITMUCH);
        trackMuch->AddHit(h.ExtIndex, kMUCHPIXELHIT);
      }
      if (h.Det == 3) {
        //CbmLitTrackToCbmTrack(litTrack, trackMuch, kLITMUCH);
        trackTrd->AddHit(h.ExtIndex, kTRDHIT);
      }
      if (h.Det == 1) {
        //CbmLitTrackToCbmTrack(litTrack, trackMuch, kLITMUCH);
        trackSts->AddHit(h.ExtIndex, kSTSHIT);
      }
      if (h.Det == 4) {

        CbmTofTrack* track  = new ((*tofTracks)[tofTrackNo++]) CbmTofTrack();
        Int_t globalTrackId = stsTrackNo - 1;
        Int_t tofHitId      = h.ExtIndex;
        track->SetTofHitIndex(tofHitId);
        track->SetTrackIndex(globalTrackId);
        // track->SetDistance(litTofTrack->GetDistance());!!!
        FairTrackParam par;
        //                 CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(litTofTrack->GetTrackParam(), &par);
        //                 track->SetTrackParameter(&par);

        CbmGlobalTrack* gTrack = static_cast<CbmGlobalTrack*>(globalTracks->At(globalTrackId));
        gTrack->SetTofHitIndex(tofHitId);

        if (event) event->AddData(kTofHit, tofHitId);
      }
    }  //StsHits loop

    if (trackMuch->GetNofHits() > 0) {
      CbmMuchTrack* track = new ((*muchTracks)[muchTrackNo]) CbmMuchTrack();
      int ndf             = 0;
      double chiSq        = 0.;
      //TODO calculate chiSq (sum of chiSq for each hit)
      for (int i = 0; i < trackMuch->GetNofHits(); i++) {
        track->AddHit(trackMuch->GetHitIndex(i), kMUCHPIXELHIT);
        ndf += 2;
      }
      ndf -= 5;
      if (ndf <= 0) ndf = 1;
      track->SetChiSq(chiSq);
      track->SetNDF(ndf);
      //TODO fill prev. track id and flag if required
      //track->SetPreviousTrackId(litTrack->GetPreviousTrackId());
      //track->SetFlag(litTrack->GetQuality());
      track->SetParamLast(trackMuch->GetParamLast());
      track->SetParamFirst(trackMuch->GetParamFirst());

      if (event) event->AddData(kMuchTrack, muchTrackNo);
      globalTrack->SetMuchTrackIndex(muchTrackNo);
      ++muchTrackNo;
    }
    if (trackTrd->GetNofHits() > 0) {
      CbmTrdTrack* track = new ((*trdTracks)[trdTrackNo]) CbmTrdTrack();
      int ndf            = 0;
      double chiSq       = 0.;
      //TODO calculate chiSq (sum of chiSq for each hit)
      for (int i = 0; i < trackTrd->GetNofHits(); i++) {
        track->AddHit(trackTrd->GetHitIndex(i), kTRDHIT);
        ndf += 2;
      }
      ndf -= 5;
      if (ndf <= 0) ndf = 1;
      track->SetChiSq(chiSq);
      track->SetNDF(ndf);
      //TODO fill prev. track id and flag if required
      //track->SetPreviousTrackId(litTrack->GetPreviousTrackId());
      //track->SetFlag(litTrack->GetQuality());
      track->SetParamLast(trackTrd->GetParamLast());
      track->SetParamFirst(trackTrd->GetParamFirst());

      if (event) event->AddData(kTrdTrack, trdTrackNo);
      globalTrack->SetTrdTrackIndex(trdTrackNo);
      ++trdTrackNo;
    }
    if (trackSts->GetNofHits() > 0) {
      CbmStsTrack* track = new ((*stsTracks)[stsTrackNo]) CbmStsTrack();
      int ndf            = 0;
      double chiSq       = 0.;
      //TODO calculate chiSq (sum of chiSq for each hit)
      for (int i = 0; i < trackSts->GetNofHits(); i++) {
        track->AddHit(trackSts->GetHitIndex(i), kSTSHIT);
        ndf += 2;
      }
      ndf -= 5;
      if (ndf <= 0) ndf = 1;
      track->SetChiSq(chiSq);
      track->SetNDF(ndf);
      //TODO fill prev. track id and flag if required
      //track->SetPreviousTrackId(litTrack->GetPreviousTrackId());
      //track->SetFlag(litTrack->GetQuality());
      track->SetParamLast(trackSts->GetParamLast());
      track->SetParamFirst(trackSts->GetParamFirst());

      if (event) event->AddData(kStsTrack, stsTrackNo);
      globalTrack->SetStsTrackIndex(stsTrackNo);
      ++stsTrackNo;
    }

  }  //vRTracks loop

  return nTracks;
}


// -----   Copy tracks to output array   -----------------------------------
Int_t CbmL1GlobalTrackFinder::CopyL1Tracks(CbmEvent* event)
{

  CbmL1* L1 = CbmL1::Instance();
  if (!L1) return 0;

  Int_t trackIndex = fTracks->GetEntriesFast();
  Int_t nTracks    = 0;
  LOG(debug) << "Copy L1 tracks : " << L1->vRTracks.size() << " tracks in L1";
  for (vector<CbmL1Track>::iterator it = L1->vRTracks.begin(); it != L1->vRTracks.end(); ++it) {
    CbmL1Track& T = *it;
    new ((*fTracks)[trackIndex]) CbmStsTrack();
    nTracks++;
    if (event) event->AddData(kStsTrack, trackIndex);
    CbmStsTrack* t = L1_DYNAMIC_CAST<CbmStsTrack*>(fTracks->At(trackIndex++));
    t->SetFlag(0);
    FairTrackParam fpar(*t->GetParamFirst()), lpar(*t->GetParamLast());
    CbmKFMath::CopyTC2TrackParam(&fpar, T.T, T.C);
    CbmKFMath::CopyTC2TrackParam(&lpar, T.TLast, T.CLast);
    t->SetParamFirst(&fpar);
    t->SetParamLast(&lpar);
    t->SetChiSq(T.chi2);
    t->SetNDF(T.NDF);
    t->SetPidHypo(T.T[4] >= 0 ? 211 : -211);
    t->SetTime(T.Tpv[6]);
    t->SetTimeError(T.Cpv[20]);

    for (vector<int>::iterator ih = it->StsHits.begin(); ih != it->StsHits.end(); ++ih) {
      CbmL1HitStore& h = L1->vHitStore[*ih];
      //    double zref = L1->algo->vStations[h.iStation].z[0];
      if (h.ExtIndex < 0) {
        // CbmMvdHit tmp;
        // tmp.SetZ(zref);
        t->AddMvdHit(-h.ExtIndex - 1);  //, &tmp );
      }
      else {
        //CbmStsHit tmp;
        //tmp.SetZ(zref);
        t->AddHit(h.ExtIndex, kSTSHIT);  //, &tmp );
      }
    }
  }

  return nTracks;
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
  /*
  CbmL1 *l1 = CbmL1::Instance();
  if( ! l1 ) return 0;

  l1->Reconstruct(event);
  */
  if (fTrdTracks != NULL) fTrdTracks->Delete();
  if (fMuchTracks != NULL) fMuchTracks->Delete();
  if (fTofTracks != NULL) fTofTracks->Delete();
  fGlobalTracks->Clear();

  int nTracks = CreateGlobalTrackArray(event, fGlobalTracks, fTracks, fTrdTracks, fMuchTracks, fTofTracks);

  return nTracks;
}
// -------------------------------------------------------------------------


// -----   End-of-run action   ---------------------------------------------
void CbmL1GlobalTrackFinder::Finish()
{
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed   : " << fNofEvents;
  LOG(info) << "Hits / event       : " << fNofHits / Double_t(fNofEvents);
  LOG(info) << "Tracks / event     : " << fNofTracks / Double_t(fNofEvents);
  LOG(info) << "Hits per track     : " << fNofHits / fNofTracks;
  LOG(info) << "Time per event     : " << fTime / Double_t(fNofEvents) << " s ";
  LOG(info) << "=====================================";
}
//
