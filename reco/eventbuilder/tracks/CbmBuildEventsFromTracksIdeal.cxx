/** @file CbmBuildEventsFromTracksIdeal.cxx
 ** @author Valentina Akishina <v.akishina@gsi.de>
 ** @date 14.03.2017
 **/

#include "CbmBuildEventsFromTracksIdeal.h"

#include "CbmEvent.h"
#include "CbmStsDigi.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

#include "CbmMCDataArray.h"
#include "CbmMCDataManager.h"
#include "CbmMCEventList.h"
#include "CbmMCTrack.h"
#include "CbmTrackMatchNew.h"

using namespace std;


// =====   Constructor   =====================================================
CbmBuildEventsFromTracksIdeal::CbmBuildEventsFromTracksIdeal()
  : FairTask("CbmBuildEventsFromTracksIdeal")
  , fStsDigis(nullptr)
  , fStsTracks(nullptr)
  , fMCTracks(nullptr)
  , fStsHits(nullptr)
  , fMcPoints(nullptr)
  , fEvents(nullptr)
  , fStsTrackMatchArray(nullptr)
  , fEventList(nullptr) {}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsFromTracksIdeal::~CbmBuildEventsFromTracksIdeal() {
  //   fEvents->Delete();
}
// ===========================================================================

// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsFromTracksIdeal::Init() {


  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmStsDigi)
  //   fStsDigis = (TClonesArray*) ioman->GetObject("StsDigi");
  //   assert ( fStsDigis );

  CbmMCDataManager* mcManager =
    (CbmMCDataManager*) ioman->GetObject("MCDataManager");
  if (mcManager == nullptr) LOG(fatal) << GetName() << ": No CbmMCDataManager!";

  fMCTracks = (CbmMCDataArray*) mcManager->InitBranch("MCTrack");
  if (fMCTracks == nullptr) LOG(fatal) << GetName() << ": No MCTrack data!";

  fEventList = (CbmMCEventList*) ioman->GetObject("MCEventList.");
  if (fEventList == 0) {
    Error("CbmKFParticleFinderQA::Init", "MC Event List not found!");
    return kERROR;
  }

  // open MCTrack array
  fStsTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  assert(fStsTracks);

  fStsTrackMatchArray = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  if (fStsTrackMatchArray == 0) {
    Error("CbmKFParticleFinderQA::Init", "track match array not found!");
    return kERROR;
  }

  fStsHits = (TClonesArray*) ioman->GetObject("StsHit");
  assert(fStsHits);

  fMcPoints = (TClonesArray*) ioman->GetObject("StsPoint");
  assert(fMcPoints);


  // Register output array (CbmStsDigi)
  fEvents = new TClonesArray("CbmEvent", 100);
  ioman->Register(
    "CbmEvent", "Cbm_Event", fEvents, IsOutputBranchPersistent("CbmEvent"));

  return kSUCCESS;
}
// ===========================================================================

// =====   Task execution   ==================================================
void CbmBuildEventsFromTracksIdeal::Exec(Option_t*) {

  TStopwatch timer;
  timer.Start();
  std::map<Int_t, CbmEvent*> eventMap;

  // Clear output array
  fEvents->Delete();

  UInt_t nTracks = fStsTracks->GetEntriesFast();


  vector<vector<UInt_t>> vKFPTrack1;

  int nMCEvents = fEventList->GetNofEvents();

  vKFPTrack1.resize(nMCEvents);


  for (unsigned int iTrack = 0; iTrack < nTracks; iTrack++) {

    //    CbmStsTrack* track = (CbmStsTrack*) fStsTracks->At(iTrack);

    CbmTrackMatchNew* stsTrackMatch =
      (CbmTrackMatchNew*) fStsTrackMatchArray->At(iTrack);
    if (stsTrackMatch->GetNofLinks() == 0) continue;
    Float_t bestWeight  = 0.f;
    Float_t totalWeight = 0.f;
    Int_t mcTrackId     = -1;
    int mcEvent         = -1;
    CbmLink link;
    for (int iLink = 0; iLink < stsTrackMatch->GetNofLinks(); iLink++) {
      totalWeight += stsTrackMatch->GetLink(iLink).GetWeight();
      if (stsTrackMatch->GetLink(iLink).GetWeight() > bestWeight) {
        bestWeight   = stsTrackMatch->GetLink(iLink).GetWeight();
        int iMCTrack = stsTrackMatch->GetLink(iLink).GetIndex();
        link         = stsTrackMatch->GetLink(iLink);


        mcEvent   = link.GetEntry();
        mcTrackId = iMCTrack;
      }
    }
    if (bestWeight / totalWeight < 0.7 || mcTrackId < 0) continue;

    //    CbmMCTrack *mcTrack = (CbmMCTrack*)fMCTracks->Get(0, mcEvent, mcTrackId);

    vKFPTrack1[mcEvent].push_back(iTrack);
  }


  for (unsigned int iEvent = 0; iEvent < vKFPTrack1.size(); iEvent++) {
    CbmEvent* event = nullptr;
    Int_t nEvents   = fEvents->GetEntriesFast();

    if (vKFPTrack1[iEvent].size() > 1) {

      event = new ((*fEvents)[nEvents]) CbmEvent(iEvent);
      event->SetStsTracks(vKFPTrack1[iEvent]);
    }
  }

  timer.Stop();
}


struct CbmBuildEventMCTrack {
  CbmBuildEventMCTrack()
    : fMCFileId(-1)
    , fMCEventId(-1)
    , fMCTrackId(-1)
    , fRecoTrackId()
    , fRecoEventId() {}

  int fMCFileId;
  int fMCEventId;
  int fMCTrackId;

  vector<int> fRecoTrackId;
  vector<int> fRecoEventId;
};


// ===========================================================================

ClassImp(CbmBuildEventsFromTracksIdeal)
