// -------------------------------------------------------------------------
// -----                 CbmFindPrimaryVertex source file              -----
// -----                  Created 28/11/05  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmFindPrimaryVertex.h"

#include "CbmEvent.h"
#include "CbmPrimaryVertexFinder.h"
#include "CbmVertex.h"

#include "FairRootManager.h"
#include <Logger.h>

#include "TClonesArray.h"

#include <iomanip>
#include <iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
CbmFindPrimaryVertex::CbmFindPrimaryVertex()
  : FairTask()
  , fTimer()
  , fFinder(nullptr)
  , fTracks(nullptr)
  , fPrimVert(nullptr)
  , fNofEvents(0)
  , fTimeTot(0.)
{
  fName = "FindPrimaryVertex";
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
CbmFindPrimaryVertex::CbmFindPrimaryVertex(CbmPrimaryVertexFinder* pvFinder)
  : FairTask()
  , fTimer()
  , fFinder(pvFinder)
  , fTracks(nullptr)
  , fPrimVert(nullptr)
  , fNofEvents(0)
  , fTimeTot(0.)
{
  fName = "FindPrimaryVertex";
}
// -------------------------------------------------------------------------


// -----  Constructor with name and title  ---------------------------------
CbmFindPrimaryVertex::CbmFindPrimaryVertex(const char* name, const char*, CbmPrimaryVertexFinder* finder)
  : FairTask(name)
  , fTimer()
  , fFinder(finder)
  , fTracks(nullptr)
  , fPrimVert(nullptr)
  , fNofEvents(0)
  , fTimeTot(0.)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmFindPrimaryVertex::~CbmFindPrimaryVertex() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmFindPrimaryVertex::Init()
{

  // Check for vertex finder
  if (!fFinder) {
    cout << "-E- CbmFindPrimaryVertex::Init : "
         << "No vertex finder selected! " << endl;
    return kERROR;
  }

  // Get FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman) {
    cout << "-E- CbmFindPrimaryVertex::Init: "
         << "RootManager not instantised!" << endl;
    return kFATAL;
  }

  // Get CbmStsTrack array
  fTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  if (!fTracks) {
    cout << "-W- CbmFindPrimaryVertex::Init: No STSTrack array!" << endl;
    return kERROR;
  }

  // Get CbmEvent array
  fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("Event"));
  if (fEvents) LOG(info) << GetName() << ": Found Event branch; run event-by-event";
  else {
    fEvents = dynamic_cast<TClonesArray*>(ioman->GetObject("CbmEvent"));
    if (fEvents) LOG(info) << GetName() << ": Found CbmEvent branch; run event-by-event";
    else {
      LOG(info) << GetName() << ": No event branch found! Run in time-based mode.";
      LOG(warning) << GetName() << "*** The primary vertex finder is not designed to find mutiple vertices.";
      LOG(warning) << GetName() << "*** Running it on an entire time slice will not produce sensible results.";
    }
  }

  // Create and register CbmVertex object
  if (!fEvents) {
    fPrimVert = new CbmVertex("Primary Vertex", "Global");
    ioman->Register("PrimaryVertex.", "Global", fPrimVert, IsOutputBranchPersistent("PrimaryVertex"));
  }

  // Call the Init method of the vertex finder
  fFinder->Init();

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmFindPrimaryVertex::Exec(Option_t*)
{

  // Timer
  TStopwatch timer;
  timer.Start();

  // Local variables
  Int_t nEvents     = 0;
  Int_t nTracks     = fTracks->GetEntriesFast();
  Int_t nTracksUsed = 0;
  Int_t result      = 0;

  // Time-based mode: process entire track array
  if (!fEvents) {
    fPrimVert->Reset();
    nTracksUsed = fFinder->FindPrimaryVertex(fTracks, fPrimVert);
  }

  // Event-based mode
  else {
    nEvents = fEvents->GetEntriesFast();
    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
      CbmEvent* event = dynamic_cast<CbmEvent*>(fEvents->At(iEvent));
      assert(event);
      result = fFinder->FindEventVertex(event, fTracks);
      LOG(debug) << GetName() << ": Event " << iEvent << " " << event->GetVertex()->ToString();
      nTracksUsed += result;
    }
  }

  // Log to screen
  timer.Stop();
  if (fEvents)
    LOG(info) << std::setw(20) << std::left << GetName() << fixed << setprecision(2) << " [" << timer.RealTime() * 1000.
              << " ms]: TS " << fNofTs << ", events " << nEvents << ", tracks used " << nTracksUsed << " / " << nTracks;
  else
    LOG(info) << std::setw(20) << std::left << GetName() << fixed << setprecision(2) << " [" << timer.RealTime() * 1000.
              << " ms] TS " << fNofTs << ", vertex " << fPrimVert->ToString() << ", tracks used " << nTracksUsed
              << " / " << nTracks;

  // Counters
  fNofTs++;
  fNofEvents += nEvents;
  fNofTracks += nTracks;
  fNofTracksUsed += nTracksUsed;
  fTimeTot += timer.RealTime();
}
// -------------------------------------------------------------------------


// -----   Public method Finish   ------------------------------------------
void CbmFindPrimaryVertex::Finish()
{

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Time slices         : " << fNofTs;
  LOG(info) << "Tracks/ TS          : " << fixed << setprecision(2) << fNofTracks / Double_t(fNofTs);
  LOG(info) << "Used Tracks / TS    : " << fixed << setprecision(2) << fNofTracksUsed / Double_t(fNofTs);
  LOG(info) << "Time  / TS          : " << fixed << setprecision(2) << 1000. * fTimeTot / Double_t(fNofTs) << " ms";
  if (fEvents) {
    LOG(info) << "Events              : " << fNofEvents;
    LOG(info) << "Events / TS         : " << fixed << setprecision(2) << Double_t(fNofEvents) / Double_t(fNofTs);
    LOG(info) << "Tracks  / event     : " << fixed << setprecision(2) << Double_t(fNofTracks) / Double_t(fNofEvents);
    LOG(info) << "Used tracks / event : " << fixed << setprecision(2)
              << Double_t(fNofTracksUsed) / Double_t(fNofEvents);
  }
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


ClassImp(CbmFindPrimaryVertex)
