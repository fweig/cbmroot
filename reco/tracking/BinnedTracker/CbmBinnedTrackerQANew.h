/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CbmBinnedTrackerQANew.h
 * Author: Timur
 *
 * Created on May 14, 2018, 4:03 PM
 */

#ifndef CBMBINNEDTRACKERQANEW_H
#define CBMBINNEDTRACKERQANEW_H

#include "CbmMCDataArray.h"
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"
#include "CbmTrdTrack.h"
#include "FairTask.h"
#include "Settings.h"
#include "TClonesArray.h"

class CbmBinnedTrackerQANew : public FairTask {
public:
  CbmBinnedTrackerQANew();
  CbmBinnedTrackerQANew(const CbmBinnedTrackerQANew&) = delete;
  CbmBinnedTrackerQANew& operator=(const CbmBinnedTrackerQANew&) = delete;
  InitStatus Init();     // Overridden from FairTask
  void Exec(Option_t*);  // Overridden from FairTask
  void Finish();         // Overridden from FairTask
  void SetParContainers();

  void SetPrimaryParticlePdg(Int_t v) { fPrimaryParticlePdg = v; }

private:
  Int_t fPrimaryParticlePdg =
    -1;  // Negative value means 'true' primary particles (not 'daugthers' of J/psi and other 'interesting' particles).
  CbmBinnedSettings* fSettings = nullptr;
  CbmMCDataArray* fMCTracks    = nullptr;
  CbmMCDataArray* fStsPoints   = nullptr;
  CbmMCDataArray* fMuchPoints  = nullptr;
  CbmMCDataArray* fTrdPoints   = nullptr;
  CbmMCDataArray* fTofPoints   = nullptr;

  TClonesArray* fStsDigis        = nullptr;
  TClonesArray* fStsDigiMatches  = nullptr;
  TClonesArray* fMuchDigis       = nullptr;
  TClonesArray* fMuchDigiMatches = nullptr;
  TClonesArray* fTrdDigis        = nullptr;
  TClonesArray* fTofDigis        = nullptr;

  TClonesArray* fTrdDigiMatches      = nullptr;
  TClonesArray* fTofHitDigiMatches   = nullptr;
  TClonesArray* fTofDigiPointMatches = nullptr;

  TClonesArray* fStsClusters  = nullptr;
  TClonesArray* fMuchClusters = nullptr;
  TClonesArray* fTrdClusters  = nullptr;

  TClonesArray* fStsHits  = nullptr;
  TClonesArray* fMuchHits = nullptr;
  TClonesArray* fTrdHits  = nullptr;
  TClonesArray* fTofHits  = nullptr;

  TClonesArray* fStsTracks  = nullptr;
  TClonesArray* fMuchTracks = nullptr;
  TClonesArray* fTrdTracks  = nullptr;

  TClonesArray* fGlobalTracks = nullptr;

  uint fMinTrackLength = 0;

  ClassDef(CbmBinnedTrackerQANew, 1)
};

#endif /* CBMBINNEDTRACKERQANEW_H */
