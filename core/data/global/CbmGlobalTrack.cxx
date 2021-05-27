// -------------------------------------------------------------------------
// -----                    CbmGlobalTrack source file                 -----
// -----                  Created 01/12/05  by V. Friese               -----
// -----                  Modified 04/06/09  by A. Lebedev             -----
// -------------------------------------------------------------------------
#include "CbmGlobalTrack.h"

#include <Logger.h>  // for Logger, LOG

#include <TObject.h>  // for TObject

// -----   Default constructor   -------------------------------------------
CbmGlobalTrack::CbmGlobalTrack()
  : TObject()
  , fStsTrack(-1)
  , fTrdTrack(-1)
  , fMuchTrack(-1)
  , fRichRing(-1)
  , fTofHit(-1)
  , fParamFirst()
  , fParamLast()
  , fParamPrimaryVertex()
  , fPidHypo(0)
  , fChi2(0.)
  , fNDF(0)
  , fFlag(0)
  , fLength(0.)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmGlobalTrack::~CbmGlobalTrack() {}
// -------------------------------------------------------------------------


// -----   Public method Print   -------------------------------------------
void CbmGlobalTrack::Print(Option_t*) const
{
  LOG(info) << "StsTrack " << fStsTrack << ", TrdTrack " << fTrdTrack << ", MuchTrack " << fMuchTrack << ", RichRing "
            << fRichRing << ", TofHit " << fTofHit;
  //  LOG(info) << "Parameters at first plane: ";
  //  fParamFirst.Print();
  //  LOG(info) << "Parameters at last plane: ";
  //  fParamLast.Print();
  LOG(info) << "chi2 = " << fChi2 << ", NDF = " << fNDF << ", Quality flag " << fFlag;
  LOG(info) << "length = " << fLength;
}
// -------------------------------------------------------------------------


ClassImp(CbmGlobalTrack)
