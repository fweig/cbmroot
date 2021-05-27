/** CbmMuchFindTracks.cxx
 *@author A.Lebedev <Andrey.Lebedev@gsi.de>
 *@since 2007
 **/
#include "CbmMuchFindTracks.h"

#include "CbmMuchTrackFinder.h"

#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iostream>

// -----   Default constructor   -------------------------------------------
CbmMuchFindTracks::CbmMuchFindTracks() : FairTask(), fFinder(NULL), fTrackArray(NULL), fNofTracks(0) {}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
CbmMuchFindTracks::CbmMuchFindTracks(const char* name, const char*, CbmMuchTrackFinder* finder)
  : FairTask(name)
  , fFinder(finder)
  , fTrackArray(NULL)
  , fNofTracks(0)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmMuchFindTracks::~CbmMuchFindTracks() { fTrackArray->Delete(); }
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmMuchFindTracks::Init()
{
  // Check for Track finder
  if (fFinder == NULL) Fatal("CbmMuchFindTracks::Init", "No track finder selected!");

  // Get and check FairRootManager
  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) Fatal("CbmMuchFindTracks::Init", "RootManager not instantised!");

  // Create and register MuchTrack array
  fTrackArray = new TClonesArray("CbmMuchTrack", 100);
  ioman->Register("MuchTrack", "Much", fTrackArray, IsOutputBranchPersistent("MuchTrack"));

  fFinder->Init();
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmMuchFindTracks::SetParContainers() {}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmMuchFindTracks::Exec(Option_t*)
{
  fTrackArray->Delete();

  fNofTracks = fFinder->DoFind(fTrackArray);
}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmMuchFindTracks::Finish() { fTrackArray->Clear(); }
// -------------------------------------------------------------------------

ClassImp(CbmMuchFindTracks);
