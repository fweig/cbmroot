// -------------------------------------------------------------------------
// -----                   CbmStsTrackFinder source file               -----
// -----                  Created 02/02/05  by V. Friese               -----
// -------------------------------------------------------------------------


// Empty file, just there to please CINT

#include "CbmStsTrackFinder.h"

//#include "CbmStsDigiScheme.h"

//#include "FairField.h"

//#include "TClonesArray.h"

CbmStsTrackFinder::CbmStsTrackFinder()
  : TNamed(),
    fDigiScheme(nullptr), 
    fField(nullptr),     
    fMvdHits(nullptr),  
    fStsHits(nullptr),    
    fTracks(nullptr), 
    fVerbose(0)
{
}

ClassImp(CbmStsTrackFinder)
