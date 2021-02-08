/** @file CbmStsBuildEventsQA.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 20.09.2016
 **/

#include "CbmBuildEventsQA.h"

#include "CbmDigiManager.h"
#include "CbmEvent.h"
#include "CbmLink.h"
#include "CbmMatch.h"
#include "CbmModuleList.h"
#include "CbmStsDigi.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;


// =====   Constructor   =====================================================
CbmBuildEventsQA::CbmBuildEventsQA()
  : FairTask("BuildEventsQA"), fEvents(NULL), fNofEntries(0) {}
// ===========================================================================


// =====   Destructor   ======================================================
CbmBuildEventsQA::~CbmBuildEventsQA() {}
// ===========================================================================


// =====   Task execution   ==================================================
void CbmBuildEventsQA::Exec(Option_t*) {

  // --- Time and counters
  TStopwatch timer;
  timer.Start();
  Int_t nMCEvents = 0;

  // --- Event loop
  Int_t nEvents = fEvents->GetEntriesFast();
  for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
    //LOG(info) << "iEvent = " << iEvent << " , nEvents = " << nEvents;
    CbmEvent* event = (CbmEvent*) fEvents->At(iEvent);

    // --- Match event to MC
    MatchEvent(event);
    Int_t mcEventNr = event->GetMatch()->GetMatchedLink().GetEntry();
    LOG(info) << GetName() << ": Event " << event->GetNumber()
              << ", data objects : " << event->GetNofData()
              << ", links: " << event->GetMatch()->GetNofLinks()
              << ", matched MC event number " << mcEventNr;

    nMCEvents += event->GetMatch()->GetNofLinks();

    // --- Loop over all detector systems
    for (ECbmModuleId& system : fSystems) {

      // --- Skip system if no data branch or no match match present
      if (!fDigiMan->IsPresent(system)) continue;
      if (!fDigiMan->IsMatchPresent(system)) continue;

      // --- Counters
      Int_t nDigis        = event->GetNofData(GetDigiType(system));
      Int_t nDigiCorrect  = 0;
      Int_t nLinks        = 0;
      Int_t nLinksCorrect = 0;

      //LOG(info) << GetName() << ": Detector "
      //          << CbmModuleList::GetModuleNameCaps(system)
      //          << ", nDigis = " << nDigis;

      // --- Loop over digis
      for (Int_t iDigi = 0; iDigi < nDigis; iDigi++) {
        UInt_t index = event->GetIndex(GetDigiType(system), iDigi);

        const CbmMatch* digiMatch = fDigiMan->GetMatch(system, index);
        assert(digiMatch);

        // --- Check MC event of digi match
        if (digiMatch->GetMatchedLink().GetEntry() == mcEventNr) nDigiCorrect++;

        //if (system == ECbmModuleId::kTof){ dev
        //     LOG(info) << "index = " <<index;
        //}

        for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
          Int_t entry = digiMatch->GetLink(iLink).GetEntry();
          nLinks++;
          if (entry == mcEventNr) nLinksCorrect++;
        }  //# links in digi
      }    //# digis

      // --- QA output
      LOG(info) << GetName() << ": Detector "
                << CbmModuleList::GetModuleNameCaps(system)
                << ", correct digis " << nDigiCorrect << " / " << nDigis
                << " = " << 100. * Double_t(nDigiCorrect) / Double_t(nDigis)
                << " %, correct digi links " << nLinksCorrect << " / " << nLinks
                << " = " << 100. * Double_t(nLinksCorrect) / Double_t(nLinks)
                << " % ";
    }
  }  //# events

  // Timer and counters
  fNofEntries++;
  timer.Stop();

  // --- Execution log
  LOG(info) << "+ " << setw(20) << GetName() << ": Entry " << setw(6) << right
            << fNofEntries << ", real time " << fixed << setprecision(6)
            << timer.RealTime() << " s, events: " << fEvents->GetEntriesFast()
            << ", MC events: " << nMCEvents;
}
// ===========================================================================


// =====   Task initialisation   =============================================
InitStatus CbmBuildEventsQA::Init() {

  // --- Get FairRootManager instance
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // --- Get input array (CbmEvent)
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
  if (nullptr == fEvents) {
    LOG(fatal) << "CbmBuildEventsQA::Init"
               << "No CbmEvent TClonesArray found!";
  }

  // --- DigiManager instance
  fDigiMan = CbmDigiManager::Instance();
  fDigiMan->Init();

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

  return kSUCCESS;
}
// ===========================================================================


// =====   Match event   =====================================================
void CbmBuildEventsQA::MatchEvent(CbmEvent* event) {

  // TODO: This functionality should later be moved to the class
  // CbmMatchRecoToMC

  // --- Get event match object. If present, will be cleared first. If not,
  // --- it will be created.
  CbmMatch* match = event->GetMatch();
  if (!match) {
    LOG(info) << "No match data found in event. Creating new.";
    match = new CbmMatch();
    event->SetMatch(match);
  } else {
    LOG(info) << "Match data found in event. Clearing.";
    match->ClearLinks();
  }
  //  LOG(info) << GetName() << ": Event " << event->GetNumber()
  //              << ", STS digis : " << event->GetNofData(ECbmDataType::kStsDigi);

  // --- Loop over all detector systems
  for (ECbmModuleId& system : fSystems) {

    // --- Skip system if no data branch or no match match present
    if (!fDigiMan->IsPresent(system)) continue;
    if (!fDigiMan->IsMatchPresent(system)) continue;

    //only use reference detector for matching to MC event
    if (system != fRefDetector) continue;

    // --- Loop over digis
    for (Int_t iDigi = 0; iDigi < event->GetNofData(GetDigiType(system));
         iDigi++) {
      Int_t index               = event->GetIndex(GetDigiType(system), iDigi);
      const CbmMatch* digiMatch = fDigiMan->GetMatch(system, index);
      assert(digiMatch);

      // --- Update event match with digi links
      // --- N.b.: The member "index" of CbmLink has here no meaning, since
      // --- there is only one MC event per tree entry.
      for (Int_t iLink = 0; iLink < digiMatch->GetNofLinks(); iLink++) {
        Int_t file      = digiMatch->GetLink(iLink).GetFile();
        Int_t entry     = digiMatch->GetLink(iLink).GetEntry();
        Double_t weight = digiMatch->GetLink(iLink).GetWeight();
        //     LOG(info) << "Adding link (weight, entry, file): " << weight << " "
        //		<< entry << " " << file;
        match->AddLink(weight, 0, entry, file);
      }  //# links in digi
    }    //#digis
  }
}
// ===========================================================================


// =====  Get digi type  =====================================================
ECbmDataType CbmBuildEventsQA::GetDigiType(ECbmModuleId system) {
  switch (system) {
    case ECbmModuleId::kMvd: return ECbmDataType::kMvdDigi;
    case ECbmModuleId::kSts: return ECbmDataType::kStsDigi;
    case ECbmModuleId::kRich: return ECbmDataType::kRichDigi;
    case ECbmModuleId::kMuch: return ECbmDataType::kMuchDigi;
    case ECbmModuleId::kTrd: return ECbmDataType::kTrdDigi;
    case ECbmModuleId::kTof: return ECbmDataType::kTofDigi;
    case ECbmModuleId::kPsd: return ECbmDataType::kPsdDigi;
    default: return ECbmDataType::kUnknown;
  }
}
// ===========================================================================


ClassImp(CbmBuildEventsQA)
