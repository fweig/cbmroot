// -------------------------------------------------------------------------
// -----                 CbmFindPrimaryVertex source file              -----
// -----                  Created 28/11/05  by V. Friese               -----
// -------------------------------------------------------------------------
#include "CbmFindPrimaryVertex.h"

#include "CbmPrimaryVertexFinder.h"
#include "CbmVertex.h"

#include "FairLogger.h"
#include "FairRootManager.h"

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
  , fTimeTot(0.) {
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
  , fTimeTot(0.) {
  fName = "FindPrimaryVertex";
}
// -------------------------------------------------------------------------


// -----  Constructor with name and title  ---------------------------------
CbmFindPrimaryVertex::CbmFindPrimaryVertex(const char* name,
                                           const char*,
                                           CbmPrimaryVertexFinder* finder)
  : FairTask(name)
  , fTimer()
  , fFinder(finder)
  , fTracks(nullptr)
  , fPrimVert(nullptr)
  , fNofEvents(0)
  , fTimeTot(0.) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmFindPrimaryVertex::~CbmFindPrimaryVertex() {}
// -------------------------------------------------------------------------


// -----   Public method Init   --------------------------------------------
InitStatus CbmFindPrimaryVertex::Init() {

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

  // Create and register CbmVertex object
  fPrimVert = new CbmVertex("Primary Vertex", "Global");
  ioman->Register("PrimaryVertex.",
                  "Global",
                  fPrimVert,
                  IsOutputBranchPersistent("PrimaryVertex"));


  // Call the Init method of the vertex finder
  fFinder->Init();

  return kSUCCESS;
}
// -------------------------------------------------------------------------


// -----   Public method Exec   --------------------------------------------
void CbmFindPrimaryVertex::Exec(Option_t*) {

  // Event number
  Int_t iEvent = fNofEvents;

  // Reset primary vertex
  fTimer.Start();
  fPrimVert->Reset();

  // Call find method of vertex finder
  Int_t iFind = fFinder->FindPrimaryVertex(fTracks, fPrimVert);
  if (iFind)
    cout << "-W- CbmFindPrimaryVertex::Exec: "
         << "Vertex finder returned " << iFind << endl;

  // --- Event log
  fTimer.Stop();
  fNofEvents++;
  fTimeTot += fTimer.RealTime();
  LOG(info) << "+ " << setw(20) << GetName() << ": Event " << setw(6) << right
            << iEvent << ", real time " << fixed << setprecision(6)
            << fTimer.RealTime()
            << " s, tracks used: " << fPrimVert->GetNTracks();
  LOG(debug) << fPrimVert->ToString();
}
// -------------------------------------------------------------------------


// -----   Public method Finish   ------------------------------------------
void CbmFindPrimaryVertex::Finish() {
  fPrimVert->Reset();
  std::cout << std::endl;
  LOG(info) << "=====================================";
  LOG(info) << GetName() << ": Run summary";
  LOG(info) << "Events processed   : " << fNofEvents;
  LOG(info) << "Time per event     : " << setprecision(6)
            << fTimeTot / Double_t(fNofEvents) << " s ";
  LOG(info) << "=====================================";
}
// -------------------------------------------------------------------------


ClassImp(CbmFindPrimaryVertex)
