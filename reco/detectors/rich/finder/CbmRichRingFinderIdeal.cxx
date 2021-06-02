/** @file CbmRichRingFinderIdeal.cxx
  * @copyright Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors S. Lebedev (2016) (Initial version Supriya Das (2005)), Semen Lebedev, Andrey Lebedev, Semen Lebedev, Florian Uhlig **/

/**
 *  $Id: CbmRichRingFinderIdeal.cxx,v 1.5 2006/02/01 13:16:58 hoehne Exp $
 *
 *  Class  : CbmRichRingFinderIdeal
 *  Description: This is the implementation of the Ideal RichRingFinder. This
 *               takes the Rich hits and associates them with the MC Track.
 *
 *  Author : S. Lebedev (2016) (Initial version Supriya Das (2005))
 *
 */
#include "CbmRichRingFinderIdeal.h"

#include "CbmDigiManager.h"
#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmMatch.h"
#include "CbmMatchRecoToMC.h"
#include "CbmRichDigi.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <iostream>
#include <map>
#include <vector>

using namespace std;


CbmRichRingFinderIdeal::CbmRichRingFinderIdeal() {}

CbmRichRingFinderIdeal::~CbmRichRingFinderIdeal() {}

void CbmRichRingFinderIdeal::Init()
{
  FairRootManager* manager = FairRootManager::Instance();
  if (nullptr == manager) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): FairRootManager is nullptr.";

  CbmMCDataManager* mcManager = (CbmMCDataManager*) manager->GetObject("MCDataManager");
  if (mcManager == nullptr) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): MCDataManager is nullptr.";

  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();
  if (!fDigiMan->IsPresent(ECbmModuleId::kRich)) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): No RichDigi.";

  if (!fDigiMan->IsMatchPresent(ECbmModuleId::kRich)) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): No RichMatchDigi.";

  fMcTracks = mcManager->InitBranch("MCTrack");
  if (fMcTracks == nullptr) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): No MCTrack.";

  fRichPoints = mcManager->InitBranch("RichPoint");
  if (fRichPoints == nullptr) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): No RichPoint!";

  fEventList = (CbmMCEventList*) manager->GetObject("MCEventList.");
  if (fEventList == nullptr) LOG(fatal) << "CbmRichRingFinderIdeal::Init(): No MCEventList.";
}

Int_t CbmRichRingFinderIdeal::DoFind(CbmEvent* event, TClonesArray* hitArray, TClonesArray* /*projArray*/,
                                     TClonesArray* ringArray)
{

  if (event != nullptr) {
    LOG(fatal) << "CbmRichRingFinderIdeal::DoFind(): CbmEvent is not nullptr. "
                  "This class does not support time-based mode. Please switch to event-by-event mode.";
  }

  if (hitArray == nullptr) {
    LOG(error) << "CbmRichRingFinderIdeal::DoFind(), hitArray is nullptr.";
    return -1;
  }

  if (ringArray == nullptr) {
    LOG(error) << "CbmRichRingFinderIdeal::DoFind(): ringArray is nullptr.";
    return -1;
  }

  // Create STL map from MCtrack index to number of valid RichHits
  map<pair<Int_t, Int_t>, Int_t> hitMap;
  Int_t nofRichHits = hitArray->GetEntriesFast();
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    const CbmRichHit* richHit = static_cast<CbmRichHit*>(hitArray->At(iHit));
    if (richHit == nullptr) continue;
    Int_t eventId = GetEventIdForRichHit(richHit);
    vector<pair<Int_t, Int_t>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(fDigiMan, richHit, fRichPoints, fMcTracks, eventId);
    for (UInt_t i = 0; i < motherIds.size(); i++) {
      hitMap[motherIds[i]]++;
    }
  }

  // Create STL map from MCTrack index to RichRing index
  map<pair<Int_t, Int_t>, Int_t> ringMap;
  Int_t nofRings  = 0;
  Int_t nofEvents = fEventList->GetNofEvents();
  for (Int_t iE = 0; iE < nofEvents; iE++) {
    Int_t fileId  = fEventList->GetFileIdByIndex(iE);
    Int_t eventId = fEventList->GetEventIdByIndex(iE);

    // Create RichRings for McTracks
    Int_t nofMcTracks = fMcTracks->Size(fileId, eventId);
    for (Int_t iT = 0; iT < nofMcTracks; iT++) {
      const CbmMCTrack* mcTrack = static_cast<CbmMCTrack*>(fMcTracks->Get(fileId, eventId, iT));
      if (mcTrack == nullptr) continue;
      pair<Int_t, Int_t> val = std::make_pair(eventId, iT);
      if (hitMap[val] <= 0) continue;
      new ((*ringArray)[nofRings]) CbmRichRing();
      ringMap[val] = nofRings++;
    }
  }

  // Loop over RichHits. Get corresponding MCPoint and MCTrack index
  for (Int_t iHit = 0; iHit < nofRichHits; iHit++) {
    const CbmRichHit* richHit = static_cast<CbmRichHit*>(hitArray->At(iHit));
    if (richHit == nullptr) continue;
    Int_t eventId = GetEventIdForRichHit(richHit);

    vector<pair<Int_t, Int_t>> motherIds =
      CbmMatchRecoToMC::GetMcTrackMotherIdsForRichHit(fDigiMan, richHit, fRichPoints, fMcTracks, eventId);

    for (UInt_t i = 0; i < motherIds.size(); i++) {
      if (ringMap.find(motherIds[i]) == ringMap.end()) continue;
      Int_t ringIndex   = ringMap[motherIds[i]];
      CbmRichRing* ring = (CbmRichRing*) ringArray->At(ringIndex);
      if (ring == nullptr) continue;

      ring->AddHit(iHit);  // attach the hit to the ring
    }
  }

  return nofRings;
}


Int_t CbmRichRingFinderIdeal::GetEventIdForRichHit(const CbmRichHit* richHit)
{
  if (richHit == nullptr) return -1;
  Int_t digiIndex = richHit->GetRefId();
  if (digiIndex < 0) return -1;
  const CbmMatch* digiMatch = fDigiMan->GetMatch(ECbmModuleId::kRich, digiIndex);
  if (NULL == digiMatch) return -1;
  return digiMatch->GetMatchedLink().GetEntry();
}
