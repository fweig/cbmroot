/**
 * \file CbmLitFindMvdTracks.cxx
 * \brief MVD tracking based on littrack package.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmLitFindMvdTracks.h"
#include "base/CbmLitToolFactory.h"
#include "base/CbmLitTrackingGeometryConstructor.h"
#include "data/CbmLitHit.h"
#include "data/CbmLitPixelHit.h"
#include "data/CbmLitTrack.h"
#include "data/CbmLitTrackParam.h"
#include "finder/CbmLitTrackFinderNN.h"
#include "utils/CbmLitConverter.h"
#include "utils/CbmLitConverterFairTrackParam.h"
#include "utils/CbmLitMemoryManagment.h"

#include "CbmStsTrack.h"
#include "FairHit.h"
#include "FairLogger.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "TClonesArray.h"

#include <algorithm>
#include <iostream>

CbmLitFindMvdTracks::CbmLitFindMvdTracks()
  : fStsTracks(NULL)
  , fMvdHits(NULL)
  , fEvents(NULL)
  , fLitStsTracks()
  , fLitMvdHits()
  , fLitOutputTracks()
  , fFinder()
  , fEventNo(0) {}

CbmLitFindMvdTracks::~CbmLitFindMvdTracks() {}

InitStatus CbmLitFindMvdTracks::Init() {
  ReadAndCreateDataBranches();

  fFinder = CbmLitToolFactory::CreateTrackFinder("mvd_nn");

  return kSUCCESS;
}

void CbmLitFindMvdTracks::Exec(Option_t* opt) {
  if (fStsTracks != NULL && fMvdHits != NULL) {
    if (fEvents) {
      Int_t nEvents = fEvents->GetEntriesFast();
      LOG(debug) << GetName() << ": reading time slice with " << nEvents
                 << " events ";

      for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
        CbmEvent* event = static_cast<CbmEvent*>(fEvents->At(iEvent));
        ConvertInputData(event);
      }     //# events
    }       //? event branch present
    else {  // Old event-by-event simulation without event branch
      ConvertInputData(0);
    }

    RunTrackReconstruction();
    ConvertOutputData();
    ClearArrays();
  } else {
    LOG(warn) << "CbmLitFindMvdTracks::Exec: MVD tracking is not executed NO "
                 "StsTrack or MvdHit arrays.";
  }
  LOG(info) << "-I- Event: " << fEventNo++;
}

void CbmLitFindMvdTracks::SetParContainers() {
  FairRunAna* ana     = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  rtdb->getContainer("FairBaseParSet");
}

void CbmLitFindMvdTracks::Finish() {}

void CbmLitFindMvdTracks::ReadAndCreateDataBranches() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (NULL == ioman) {
    LOG(fatal) << "CbmLitFindMvdTracks::ReadAndCreateDataBranches "
                  "FairRootManager is not instantiated";
  }
  fMvdHits   = dynamic_cast<TClonesArray*>(ioman->GetObject("MvdHit"));
  fStsTracks = dynamic_cast<TClonesArray*>(ioman->GetObject("StsTrack"));
  fEvents    = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
}

void CbmLitFindMvdTracks::ConvertInputData(CbmEvent* event) {
  CbmLitConverter::StsTrackArrayToTrackVector(event, fStsTracks, fLitStsTracks);
  // Change last and first parameters of the track seeds
  for (Int_t iTrack = 0; iTrack < fLitStsTracks.size(); iTrack++) {
    CbmLitTrack* track               = fLitStsTracks[iTrack];
    const CbmLitTrackParam* parFirst = track->GetParamFirst();
    track->SetParamLast(parFirst);
  }
  LOG(info) << "-I- Number of STS tracks: " << fLitStsTracks.size();

  CbmLitConverter::MvdHitArrayToHitVector(fMvdHits, fLitMvdHits);
  // Make reverse order of the hits
  Int_t nofStations =
    CbmLitTrackingGeometryConstructor::Instance()->GetNofMvdStations();
  for (Int_t iHit = 0; iHit < fLitMvdHits.size(); iHit++) {
    CbmLitHit* hit = fLitMvdHits[iHit];
    hit->SetDetectorId(kLITMVD, nofStations - hit->GetStation() - 1);
  }
  LOG(info) << "-I- Number of MVD hits: " << fLitMvdHits.size();
}

void CbmLitFindMvdTracks::ConvertOutputData() {
  for (Int_t iTrack = 0; iTrack < fLitOutputTracks.size(); iTrack++) {
    CbmLitTrack* litTrack = fLitOutputTracks[iTrack];
    Int_t trackId         = litTrack->GetPreviousTrackId();
    CbmStsTrack* track    = static_cast<CbmStsTrack*>(fStsTracks->At(trackId));
    for (Int_t iHit = 0; iHit < litTrack->GetNofHits(); iHit++) {
      const CbmLitHit* litHit = litTrack->GetHit(iHit);
      Int_t refId             = litHit->GetRefId();
      FairHit* hit            = static_cast<FairHit*>(fMvdHits->At(refId));
      track->AddMvdHit(refId);
    }
    //track->SortMvdHits();

    FairTrackParam parFirst;
    CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(
      litTrack->GetParamLast(), &parFirst);
    track->SetParamFirst(&parFirst);
  }
}

void CbmLitFindMvdTracks::ClearArrays() {
  // Free memory
  for_each(fLitStsTracks.begin(), fLitStsTracks.end(), DeleteObject());
  for_each(fLitMvdHits.begin(), fLitMvdHits.end(), DeleteObject());
  for_each(fLitOutputTracks.begin(), fLitOutputTracks.end(), DeleteObject());
  fLitStsTracks.clear();
  fLitMvdHits.clear();
  fLitOutputTracks.clear();
}

void CbmLitFindMvdTracks::RunTrackReconstruction() {
  fFinder->DoFind(fLitMvdHits, fLitStsTracks, fLitOutputTracks);
}
