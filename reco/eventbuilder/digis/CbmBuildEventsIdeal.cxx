/** @file CbmStsBuildEventsIdeal.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.09.2016
 **/

#include "CbmBuildEventsIdeal.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <TClonesArray.h>
#include <TStopwatch.h>
#include <FairLogger.h>
#include <FairRootManager.h>

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"
#include "CbmDigiManager.h"

using namespace std;


// =====   Constructor   =====================================================
CbmBuildEventsIdeal::CbmBuildEventsIdeal() : FairTask("BuildEventsIdeal") {
}
// ===========================================================================



// =====   Destructor   ======================================================
CbmBuildEventsIdeal::~CbmBuildEventsIdeal() {
}
// ===========================================================================



// =====   Task execution   ==================================================
void CbmBuildEventsIdeal::Exec(Option_t*) {

  TStopwatch timer;
  timer.Start();
  std::map<Int_t, CbmEvent*> eventMap;
  Int_t nEvents = 0;
  UInt_t nDigisTot = 0;
  UInt_t nDigisNoise = 0;

  // Clear output array
  fEvents->Delete();

  for (ECbmModuleId& system : fSystems) {
    if ( ! fDigiMan->IsPresent(system) ) continue;
    if ( ! fDigiMan->IsMatchPresent(system) ) continue;
    Int_t nDigis = fDigiMan->GetNofDigis(system);
    UInt_t nNoise = 0;
    LOG(info) << GetName() << ": System " << CbmModuleList::GetModuleNameCaps(system)
    << ", digis " << nDigis;

    for (Int_t iDigi= 0; iDigi < nDigis; iDigi++) {
      const CbmMatch* match = fDigiMan->GetMatch(system, iDigi);
      assert(match);

      // This implementation uses only MC event number from
      // the matched link, i.e. that with the largest weight.
      // Can be refined later on.
      Int_t mcEventNr = match->GetMatchedLink().GetEntry();

      // Ignore digis with missing event number (noise)
      if ( mcEventNr < 0 ) {
        nNoise++;
        continue;
      }

      // Get event pointer. If event is not yet present, create it.
      CbmEvent* event = NULL;
      if ( eventMap.find(mcEventNr) == eventMap.end() ) {
        event = new ( (*fEvents)[nEvents] ) CbmEvent(nEvents);
        eventMap[mcEventNr] = event;
        nEvents++;
      }
      else event = eventMap.at(mcEventNr);

      // Fill digi index into event
      switch (system) {
        case ECbmModuleId::kMvd:  event->AddData(ECbmDataType::kMvdDigi,  iDigi); break;
        case ECbmModuleId::kSts:  event->AddData(ECbmDataType::kStsDigi,  iDigi); break;
        case ECbmModuleId::kRich: event->AddData(ECbmDataType::kRichDigi, iDigi); break;
        case ECbmModuleId::kMuch: event->AddData(ECbmDataType::kMuchDigi, iDigi); break;
        case ECbmModuleId::kTrd:  event->AddData(ECbmDataType::kTrdDigi,  iDigi); break;
        case ECbmModuleId::kTof:  event->AddData(ECbmDataType::kTofDigi,  iDigi); break;
        case ECbmModuleId::kPsd:  event->AddData(ECbmDataType::kPsdDigi,  iDigi); break;
        default: break;
      } //? detector

    } //# digis
    LOG(debug) << GetName() <<": Detector "
               << CbmModuleList::GetModuleNameCaps(system) << ", digis " << nDigis
               << ", noise " << nNoise;
    nDigisTot += nDigis;
    nDigisNoise += nNoise;

  } //# detectors


  timer.Stop();
  assert( nEvents == fEvents->GetEntriesFast() );

  // --- Execution log
  std::cout << std::endl;
  LOG(info) << "+ " << setw(15) << GetName() << ": Time-slice " << setw(3)
            << right << fNofEntries << ", events: " << setw(6) << nEvents
            << ", digis: " << nDigisTot << ", noise: " << nDigisNoise
            << ". Exec time " << fixed << setprecision(6) << timer.RealTime()
            << " s.";

  // --- For debug: event info
  if (gLogger->IsLogNeeded(fair::Severity::debug)) {
    for (Int_t iEvent = 0; iEvent < fEvents->GetEntriesFast(); iEvent++ ) {
      CbmEvent* event = (CbmEvent*) fEvents->At(iEvent);
      LOG(info) << event->ToString();
    }
  }

  fNofEntries++;
}
// ===========================================================================



// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsIdeal::Init() {

	// --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert ( ioman );

  // --- DigiManager instance
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

  std::cout << std::endl;
  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising...";


  // --- Check input data
  for (ECbmModuleId system = ECbmModuleId::kMvd; system < ECbmModuleId::kNofSystems; ++system) {
    if ( fDigiMan->IsMatchPresent(system) ) {
      LOG(info) << GetName() << ": Found match branch for "
          << CbmModuleList::GetModuleNameCaps(system);
      fSystems.push_back(system);
    }
  }
  if ( fSystems.empty() ) {
    LOG(fatal) << GetName() << ": No match branch found!";
    return kFATAL;
  }

  // Register output array (CbmEvent)
  if ( ioman->GetObject("Event") ) {
    LOG(fatal) << GetName() << ": Branch Event already exists!";
    return kFATAL;
  }
  fEvents = new TClonesArray("CbmEvent",100);
  ioman->Register("Event", "CbmEvent", fEvents,
  			 	        IsOutputBranchPersistent("Event"));
  if ( ! fEvents ) {
    LOG(fatal) << GetName() << ": Output branch could not be created!";
    return kFATAL;
  }

  LOG(info) << "==================================================";
  std::cout << std::endl;

  return kSUCCESS;
}
// ===========================================================================


ClassImp(CbmBuildEventsIdeal)

