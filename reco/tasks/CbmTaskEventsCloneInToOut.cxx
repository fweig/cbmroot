/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */


#include "CbmTaskEventsCloneInToOut.h"

#include "CbmDefs.h"
#include "CbmEvent.h"

#include <FairFileSource.h>
#include <FairRootManager.h>
#include <Logger.h>

#include <TClonesArray.h>
#include <TStopwatch.h>

#include <algorithm>
#include <cassert>
#include <iomanip>


using namespace std;


// -----   Constructor   -----------------------------------------------------
CbmTaskEventsCloneInToOut::CbmTaskEventsCloneInToOut() : FairTask("EventsCloneInToOut") {}
// ---------------------------------------------------------------------------


// -----   Destructor   ------------------------------------------------------
CbmTaskEventsCloneInToOut::~CbmTaskEventsCloneInToOut() {}
// ---------------------------------------------------------------------------


// -----   Execution   -------------------------------------------------------
void CbmTaskEventsCloneInToOut::Exec(Option_t*)
{
  // --- Timer and counters
  TStopwatch timer;
  timer.Start();

  // --- No action if no CbmEvents branch is present
  if (!fRecoEventsIn) return;

  fRecoEventsOut->Delete();  // Bad way to do it according to ROOT docs, but memory leak otherwise

  // --- Copy all entries in input TClonesArray to output one
  // => Event loop
  for (Int_t eventNr = 0; eventNr < fRecoEventsIn->GetEntriesFast(); ++eventNr) {
    const CbmEvent* pEventIn = dynamic_cast<const CbmEvent*>((*fRecoEventsIn)[eventNr]);
    if (pEventIn) {
      /// Explicit creation-copy as no ranged Copy method for TClonesArray
      new ((*fRecoEventsOut)[eventNr]) CbmEvent(*pEventIn);
    }
    else {
      LOG(fatal) << GetName() << ": Failed to get input event " << eventNr << " in TS " << fNumTs;
    }
  }

  if (fRecoEventsIn->GetEntriesFast() != fRecoEventsOut->GetEntriesFast()) {
    LOG(fatal) << GetName() << ": Input size not matching output one: " << fRecoEventsIn->GetEntriesFast() << " VS "
               << fRecoEventsOut->GetEntriesFast() << " in TS " << fNumTs;
  }

  // --- Timeslice log
  timer.Stop();
  stringstream logOut;
  logOut << setw(20) << left << GetName() << " [";
  logOut << fixed << setw(8) << setprecision(1) << right << timer.RealTime() * 1000. << " ms] ";
  logOut << "TS " << fNumTs << ", events In " << fRecoEventsIn->GetEntriesFast() << ", events Out "
         << fRecoEventsOut->GetEntriesFast();
  LOG(info) << logOut.str();

  // --- Run statistics
  fNumTs++;
  fTimeTot += timer.RealTime();
  fNumEvents += fRecoEventsOut->GetEntriesFast();
}
// ----------------------------------------------------------------------------


// -----   End-of-timeslice action   ------------------------------------------
void CbmTaskEventsCloneInToOut::Finish()
{
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Timeslices         : " << fNumTs;
  LOG(info) << "Events             : " << fNumEvents;
  LOG(info) << "Time  / TS         : " << fixed << setprecision(2) << 1000. * fTimeTot / double(fNumTs) << " ms";
  LOG(info) << "=====================================";
  fRecoEventsOut->Delete();
}
// ----------------------------------------------------------------------------


// -----   Initialisation   ---------------------------------------------------
InitStatus CbmTaskEventsCloneInToOut::Init()
{

  LOG(info) << "==================================================";
  LOG(info) << GetName() << ": Initialising ";

  // --- Get FairRootManager instance
  FairRootManager* frm = FairRootManager::Instance();
  if (nullptr == frm) {
    LOG(error) << GetName() << ": Failed to get FairRootManager! Aborting...";
    return kFATAL;
  }

  // --- Try to get input vector (CbmDigiEvent)
  fpFileIn = dynamic_cast<FairFileSource*>(frm->GetSource());
  if (nullptr == fpFileIn) {
    LOG(error) << GetName() << ": Failed to get Fair file interface object! Aborting...";
    return kFATAL;
  }

  TTree* pTree = fpFileIn->GetInTree();
  if (pTree->GetBranch("CbmEvent")) {
    fRecoEventsIn = new TClonesArray("CbmEvent", 100);
    pTree->SetBranchAddress("CbmEvent", &fRecoEventsIn);
    pTree->SetBranchStatus("CbmEvent", 1);

    LOG(info) << GetName() << ": Found branch CbmEvent in input";

    // --- Event
    fRecoEventsOut = new TClonesArray("CbmEvent", 1);
    frm->Register("CbmEvent", "Reco events", fRecoEventsOut, kTRUE);
    LOG(info) << GetName() << ": created branch CbmEvent in output";
  }
  else {
    LOG(error) << GetName() << ": No CbmEvent branch found in file! Aborting...";
    return kFATAL;
  }

  LOG(info) << "==================================================";

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

ClassImp(CbmTaskEventsCloneInToOut)
