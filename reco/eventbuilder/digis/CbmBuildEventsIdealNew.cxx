/** @file CbmStsBuildEventsIdealNew.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 13.03.2020
 **/

#include "CbmBuildEventsIdealNew.h"

#include <FairLogger.h>
#include <FairRootManager.h>
#include <TClonesArray.h>
#include <TStopwatch.h>
#include <cassert>
#include <iomanip>
#include <iostream>

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmEventStore.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"

#include "CbmMuchDigi.h"
#include "CbmMvdDigi.h"
#include "CbmPsdDigi.h"
#include "CbmRichDigi.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmTrdDigi.h"

using namespace std;


// =====   Constructor   =====================================================
CbmBuildEventsIdealNew::CbmBuildEventsIdealNew()
  : FairTask("BuildEventsIdealNew") {}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsIdealNew::~CbmBuildEventsIdealNew() {}
// ===========================================================================


// =====   Task execution   ==================================================
void CbmBuildEventsIdealNew::Exec(Option_t*) {

  TStopwatch timer;
  timer.Start();
  fEvents->Delete();
  std::map<Int_t, CbmEventStore*> eventMap;
  Int_t nEvents      = 0;
  UInt_t nDigisTot   = 0;
  UInt_t nDigisNoise = 0;

  for (ECbmModuleId& system : fSystems) {
    if (!fDigiMan->IsPresent(system)) continue;
    if (!fDigiMan->IsMatchPresent(system)) continue;
    Int_t nDigis  = fDigiMan->GetNofDigis(system);
    UInt_t nNoise = 0;

    for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
      const CbmMatch* match = fDigiMan->GetMatch(system, iDigi);
      assert(match);

      // This implementation uses only MC event number from
      // the matched link, i.e. that with the largest weight.
      // Can be refined later on.
      Int_t mcEventNr = match->GetMatchedLink().GetEntry();

      // Ignore digis with missing event number (noise)
      if (mcEventNr < 0) {
        nNoise++;
        continue;
      }

      // Get event pointer. If event is not yet present, create it.
      auto mapIt = eventMap.find(mcEventNr);
      if (mapIt == eventMap.end()) {
        eventMap[mcEventNr] = new CbmEventStore(nEvents++, kTRUE);
        mapIt               = eventMap.find(mcEventNr);
      }
      assert(mapIt != eventMap.end());
      CbmEventStore* event = mapIt->second;

      // Fill digi into event
      switch (system) {
        case ECbmModuleId::kMvd: {
          const CbmMvdDigi* digi = fDigiMan->Get<CbmMvdDigi>(iDigi);
          event->AddDigi<CbmMvdDigi>(digi, match);
          break;
        }
        case ECbmModuleId::kSts: {
          const CbmStsDigi* digi = fDigiMan->Get<CbmStsDigi>(iDigi);
          event->AddDigi<CbmStsDigi>(digi, match);
          break;
        }
        case ECbmModuleId::kRich: {
          const CbmRichDigi* digi = fDigiMan->Get<CbmRichDigi>(iDigi);
          event->AddDigi<CbmRichDigi>(digi, match);
          break;
        }
        case ECbmModuleId::kMuch: {
          const CbmMuchDigi* digi = fDigiMan->Get<CbmMuchDigi>(iDigi);
          event->AddDigi<CbmMuchDigi>(digi, match);
          break;
        }
        case ECbmModuleId::kTrd: {
          const CbmTrdDigi* digi = fDigiMan->Get<CbmTrdDigi>(iDigi);
          event->AddDigi<CbmTrdDigi>(digi, match);
          break;
        }
        case ECbmModuleId::kTof: {
          const CbmTofDigi* digi = fDigiMan->Get<CbmTofDigi>(iDigi);
          event->AddDigi<CbmTofDigi>(digi, match);
          break;
        }
        case ECbmModuleId::kPsd: {
          const CbmPsdDigi* digi = fDigiMan->Get<CbmPsdDigi>(iDigi);
          event->AddDigi<CbmPsdDigi>(digi, match);
          break;
        }
        default: break;
      }  //? detector

    }  //# digis

    nDigisTot += nDigis;
    nDigisNoise += nNoise;
  }  //# detectors

  // --- Fill output array
  for (auto& entry : eventMap) {
    assert(fEvents);
    UInt_t nEntry = fEvents->GetEntriesFast();
    new ((*fEvents)[nEntry]) CbmEventStore(*(entry.second));
    delete entry.second;
    CbmEventStore* event = (CbmEventStore*) fEvents->At(nEntry);
    assert(event);
    LOG(debug) << event->ToString();
  }
  LOG(debug) << "Created " << fEvents->GetEntriesFast() << " events";

  timer.Stop();

  // --- Execution log
  std::cout << std::endl;
  LOG(info) << "+ " << setw(15) << GetName() << ": Time-slice " << setw(3)
            << right << fNofEntries << ", events: " << setw(6) << nEvents
            << ", digis: " << nDigisTot << ", noise: " << nDigisNoise
            << ". Exec time " << fixed << setprecision(6) << timer.RealTime()
            << " s.";

  fNofEntries++;
}
// ===========================================================================


// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsIdealNew::Init() {

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- DigiManager instance
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  std::cout << std::endl;
  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";


  // --- Check input data
  for (ECbmModuleId system = ECbmModuleId::kMvd;
       system < ECbmModuleId::kNofSystems;
       ++system) {
    if (fDigiMan->IsMatchPresent(system)) {
      LOG(info) << GetName() << ": Found match branch for "
                << CbmModuleList::GetModuleNameCaps(system);
      fSystems.push_back(system);
    }
  }
  if (fSystems.empty()) {
    LOG(fatal) << GetName() << ": No match branch found!";
    return kFATAL;
  }

  // Register output array (CbmEvent)
  if (ioman->GetObject("CbmEventStore")) {
    LOG(fatal) << GetName() << ": Branch CbmEventStore already exists!";
    return kFATAL;
  }
  /*
  fEvents = new std::vector<CbmEventStore>();
  ioman->RegisterAny("CbmEventStore", fEvents, kTRUE);
  if ( ! fEvents ) {
    LOG(fatal) << GetName() << ": Output branch could not be created!";
    return kFATAL;
  }
  */
  fEvents = new TClonesArray("CbmEventStore", 100);
  ioman->Register("CbmEventStore", "Events", fEvents, kTRUE);


  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ===========================================================================


ClassImp(CbmBuildEventsIdealNew)
