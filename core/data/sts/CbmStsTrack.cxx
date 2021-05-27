/**
 ** \file CbmStsTrack.h
 ** \author V.Friese <v.friese@gsi.de>
 ** \since 26.01.05
 ** \date 07.09.15
 **/
#include "CbmStsTrack.h"

#include <FairTrackParam.h>  // for FairTrackParam

#include <sstream>  // for operator<<, basic_ostream, stringstream

using std::stringstream;


// -----   Constructor   ---------------------------------------------------
CbmStsTrack::CbmStsTrack() : CbmTrack(), fMvdHitIndex(), fB(0.) {}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
CbmStsTrack::~CbmStsTrack() {}
// -------------------------------------------------------------------------


// -----   Debug   ---------------------------------------------------------
std::string CbmStsTrack::ToString() const
{
  stringstream ss;
  ss << "CbmStsTrack: time " << fTime << " ns | hits STS " << GetNofStsHits() << " MVD " << GetNofMvdHits() << " | q/p "
     << GetParamFirst()->GetQp() << " | chisq " << GetChiSq() << " | NDF " << GetNDF() << " | STS hits ";
  for (Int_t iHit = 0; iHit < GetNofStsHits(); iHit++) {
    ss << GetStsHitIndex(iHit) << " ";
  }
  return ss.str();
}
// -------------------------------------------------------------------------


ClassImp(CbmStsTrack)
