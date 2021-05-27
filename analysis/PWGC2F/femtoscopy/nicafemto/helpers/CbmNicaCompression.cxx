/*
 * CbmNicaCompression.cxx
 *
 *  Created on: 7 lut 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CbmNicaCompression.h"

#include "CbmMCTrack.h"
#include "CbmTrackMatchNew.h"
#include "CbmVertex.h"

#include "FairRootManager.h"

CbmNicaCompression::CbmNicaCompression() : fAllDep(kFALSE), fStsLinks(nullptr), fTofLinks(nullptr)
{
  fStsMatches = new NicaTrackClones("CbmTrackMatchNew", "StsTrackMatch", "STS");
  fTofMatches = new NicaTrackClones("CbmTrackMatchNew", "TofHitMatch", "TOF");
  fMCTracks   = new NicaTrackClones("CbmMCTrack", "MCTrack", "MC");
}

InitStatus CbmNicaCompression::Init()
{
  FairRootManager* mngr = FairRootManager::Instance();
  fTofMatches->GetFromTree();
  fStsMatches->GetFromTree();
  fMCTracks->GetFromTree();
  if (fTofMatches->GetArray() == nullptr) return kFATAL;
  if (fStsMatches->GetArray() == nullptr) return kFATAL;

  if (fMCTracks->GetArray() == nullptr) return kFATAL;

  mngr->Register("MCTrack", "MC", fMCTracks->GetArray(), kTRUE);
  fStsLinks = new TClonesArray("CbmTrackMatchNew");
  fTofLinks = new TClonesArray("CbmTrackMatchNew");
  mngr->Register("StsTrackMatch", "STS", fStsLinks, kTRUE);
  mngr->Register("TofHitMatch", "TOF", fTofLinks, kTRUE);
  mngr->Register("PrimaryVertex.", "PV", (CbmVertex*) mngr->GetObject("PrimaryVertex."), kTRUE);
  TClonesArray* sts_tracks  = (TClonesArray*) mngr->GetObject("StsTrack");
  TClonesArray* tof_tracks  = (TClonesArray*) mngr->GetObject("TofHit");
  TClonesArray* glob_tracks = (TClonesArray*) mngr->GetObject("GlobalTrack");

  mngr->Register("StsTrack", "STS", sts_tracks, kTRUE);
  mngr->Register("TofHit", "TOF", tof_tracks, kTRUE);
  mngr->Register("GlobalTrack", "STS", glob_tracks, kTRUE);

  return kSUCCESS;
}

void CbmNicaCompression::Exec(Option_t* opt)
{
  fMapUse.MakeBigger(fMCTracks->GetEntriesFast());
  fMapIndex.MakeBigger(fMCTracks->GetEntriesFast());
  fStsLinks->Clear();
  fTofLinks->Clear();
  fStsLinks->ExpandCreateFast(fStsMatches->GetEntriesFast());
  fTofLinks->ExpandCreateFast(fTofMatches->GetEntriesFast());
  for (int i = 0; i < fMCTracks->GetEntriesFast(); i++) {
    fMapUse[i]   = 0;
    fMapIndex[i] = -2;
  }
  if (fAllDep) { WithDep(); }
  else {
    NoDep();
  }
}

void CbmNicaCompression::NoDep()
{
  for (int i = 0; i < fStsMatches->GetEntriesFast(); i++) {
    CbmTrackMatchNew* match = (CbmTrackMatchNew*) fStsMatches->UncheckedAt(i);
    int index               = match->GetMatchedLink().GetIndex();
    if (index >= 0) {  // needed?
      fMapUse[index] = 1;
    }
  }
  for (int i = 0; i < fTofMatches->GetEntriesFast(); i++) {
    CbmTrackMatchNew* match = (CbmTrackMatchNew*) fTofMatches->UncheckedAt(i);
    int index               = match->GetMatchedLink().GetIndex();
    if (index >= 0) { fMapUse[index] = 1; }
  }
  int count = 0;
  for (int i = 0; i < fMCTracks->GetEntriesFast(); i++) {
    if (fMapUse[i] == 1) {
      fMapIndex[i] = count;
      ++count;
    }
    else {
      fMCTracks->GetArray()->RemoveAt(i);
    }
  }
  fMCTracks->GetArray()->Compress();
  for (int i = 0; i < fMCTracks->GetEntriesFast(); i++) {
    CbmMCTrack* mc = (CbmMCTrack*) fMCTracks->UncheckedAt(i);
    int mom        = mc->GetMotherId();
    if (mom >= 0) { mc->SetMotherId(fMapIndex[mom]); }
  }

  //ok let's fix matches
  for (int iTrack = 0; iTrack < fStsMatches->GetEntriesFast(); iTrack++) {
    CbmTrackMatchNew* match  = (CbmTrackMatchNew*) fStsMatches->UncheckedAt(iTrack);
    CbmLink link             = match->GetMatchedLink();
    CbmTrackMatchNew* match2 = (CbmTrackMatchNew*) fStsLinks->UncheckedAt(iTrack);
    match2->ClearLinks();
    int index = link.GetIndex();
    if (index >= 0) {
      link.SetIndex(fMapIndex[index]);
      link.SetWeight(1);
    }
    match2->AddLink(link);
  }
  for (int iTrack = 0; iTrack < fTofMatches->GetEntriesFast(); iTrack++) {
    CbmTrackMatchNew* match  = (CbmTrackMatchNew*) fTofMatches->UncheckedAt(iTrack);
    CbmTrackMatchNew* match2 = (CbmTrackMatchNew*) fTofLinks->UncheckedAt(iTrack);
    CbmLink link             = match->GetMatchedLink();
    match2->ClearLinks();
    int index = link.GetIndex();
    if (index >= 0) {
      link.SetIndex(fMapIndex[index]);
      link.SetWeight(1);
    }
    match2->AddLink(link);
  }
}

void CbmNicaCompression::WithDep()
{
  for (int i = 0; i < fStsMatches->GetEntriesFast(); i++) {
    CbmTrackMatchNew* match = (CbmTrackMatchNew*) fStsMatches->UncheckedAt(i);
    for (int j = 0; j < match->GetNofLinks(); j++) {
      int index = match->GetLink(j).GetIndex();
      if (index >= 0) { fMapUse[index] = 1; }
    }
  }
  for (int i = 0; i < fTofMatches->GetEntriesFast(); i++) {
    CbmTrackMatchNew* match = (CbmTrackMatchNew*) fTofMatches->UncheckedAt(i);
    for (int j = 0; j < match->GetNofLinks(); j++) {
      int index = match->GetLink(j).GetIndex();
      if (index >= 0) { fMapUse[index] = 1; }
    }
  }
  int count = 0;
  for (int i = 0; i < fMCTracks->GetEntriesFast(); i++) {
    if (fMapUse[i] == 1) {
      fMapIndex[i] = count;
      ++count;
    }
    else {
      fMCTracks->GetArray()->RemoveAt(i);
    }
  }
  fMCTracks->GetArray()->Compress();
  for (int i = 0; i < fMCTracks->GetEntriesFast(); i++) {
    CbmMCTrack* mc = (CbmMCTrack*) fMCTracks->UncheckedAt(i);
    int mom        = mc->GetMotherId();
    if (mom >= 0) { mc->SetMotherId(fMapIndex[mom]); }
  }

  //ok let's fix matches
  for (int iTrack = 0; iTrack < fStsMatches->GetEntriesFast(); iTrack++) {
    CbmTrackMatchNew* match  = (CbmTrackMatchNew*) fStsMatches->UncheckedAt(iTrack);
    CbmTrackMatchNew* match2 = (CbmTrackMatchNew*) fStsLinks->UncheckedAt(iTrack);
    match2->ClearLinks();
    for (int iLink = 0; iLink < match->GetNofLinks(); iLink++) {
      CbmLink link = match->GetLink(iLink);
      int index    = link.GetIndex();
      if (index >= 0) { link.SetIndex(fMapIndex[index]); }
      match2->AddLink(link);
    }
  }
  for (int iTrack = 0; iTrack < fTofMatches->GetEntriesFast(); iTrack++) {
    CbmTrackMatchNew* match  = (CbmTrackMatchNew*) fTofMatches->UncheckedAt(iTrack);
    CbmTrackMatchNew* match2 = (CbmTrackMatchNew*) fTofLinks->UncheckedAt(iTrack);
    match2->ClearLinks();
    for (int iLink = 0; iLink < match->GetNofLinks(); iLink++) {
      CbmLink link = match->GetLink(iLink);
      int index    = link.GetIndex();
      if (index >= 0) { link.SetIndex(fMapIndex[index]); }
      match2->AddLink(link);
    }
  }
}

CbmNicaCompression::~CbmNicaCompression() {}
