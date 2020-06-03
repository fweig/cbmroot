/** @file CbmFindPrimaryVertexEvents.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 16.06.2014
 ** @date 02.11.2016
 **/

#include "CbmFindPrimaryVertexEvents.h"


#include <cassert>
#include <iomanip>
#include <iostream>
#include "TClonesArray.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "CbmEvent.h"
#include "CbmPrimaryVertexFinder.h"
#include "CbmVertex.h"

using namespace std;


// -----   Default constructor   -------------------------------------------
CbmFindPrimaryVertexEvents::CbmFindPrimaryVertexEvents()
  : FairTask(),
    fTimer(),
    fFinder(NULL),
	fEvents(NULL),
    fTracks(NULL),
    fPrimVert(NULL),
	fNofEvents(0),
	fTimeTot(0.)
{
	fName = "FindPrimaryVertex";
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
CbmFindPrimaryVertexEvents::CbmFindPrimaryVertexEvents(CbmPrimaryVertexFinder* pvFinder)
  : FairTask(),
    fTimer(),
    fFinder(pvFinder),
	fEvents(NULL),
    fTracks(NULL),
    fPrimVert(NULL),
	fNofEvents(0),
	fTimeTot(0.)
{
	fName = "FindPrimaryVertex";
}
// -------------------------------------------------------------------------



// -----  Constructor with name and title  ---------------------------------
CbmFindPrimaryVertexEvents::CbmFindPrimaryVertexEvents(const char* name,
					   const char*, 
					   CbmPrimaryVertexFinder* finder) 
  : FairTask(name),
    fTimer(),
    fFinder(finder),
	fEvents(NULL),
    fTracks(NULL),
    fPrimVert(NULL),
	fNofEvents(0),
	fTimeTot(0.)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmFindPrimaryVertexEvents::~CbmFindPrimaryVertexEvents() { }
// -------------------------------------------------------------------------



// -----   Initialisation  --------------------------------------------------
InitStatus CbmFindPrimaryVertexEvents::Init() {

  assert(fFinder);

  // Get FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  assert(ioman);

  // Get event array
  fEvents = (TClonesArray*) ioman->GetObject("Event");
  assert(fEvents);

  // Get CbmStsTrack array
  fTracks = (TClonesArray*) ioman->GetObject("StsTrack");
  assert(fTracks);

  // Create and register CbmVertex object
  fPrimVert = new CbmVertex("Primary Vertex", "Global");
  ioman->Register("PrimaryVertex.", "Global", fPrimVert,
		          IsOutputBranchPersistent("PrimaryVertex"));


  // Initialise vertex finder
  fFinder->Init();

  return kSUCCESS;
}
// -------------------------------------------------------------------------



// -----   Task execution   ------------------------------------------------
void CbmFindPrimaryVertexEvents::Exec(Option_t*) {

  // --- Event loop
  Int_t nEvents = fEvents->GetEntriesFast();
  LOG(debug) << GetName() << ": reading time slice with " << nEvents
             << " events ";
  for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {

	fTimer.Start();
    CbmEvent* event = static_cast<CbmEvent*> (fEvents->At(iEvent));

    // --- Call find method of vertex finder
//    Int_t status = fFinder->FindEventVertex(event, fTracks);
    fFinder->FindEventVertex(event, fTracks);

    // --- Event log
    LOG(info) << "+ " << setw(20) << GetName() << ": Event " << setw(6)
    		      << right << event->GetNumber()
    		      << ", real time " << fixed << setprecision(6)
	              << fTimer.RealTime() << " s, tracks used: "
		      << event->GetVertex()->GetNTracks();
    LOG(debug) << fPrimVert->ToString();

    // --- Counters
    fNofEvents++;
    fTimeTot        += fTimer.RealTime();

  } //# events

}
// -------------------------------------------------------------------------



// -----   End-of-run action   ---------------------------------------------
void CbmFindPrimaryVertexEvents::Finish() {

  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed   : " << fNofEvents;
  LOG(info) << "Time per event     : " << fTimeTot / Double_t(fNofEvents)
            << " s ";
  LOG(info) << "=====================================";

}
// -------------------------------------------------------------------------



ClassImp(CbmFindPrimaryVertexEvents)
