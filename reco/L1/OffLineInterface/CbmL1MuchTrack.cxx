
#include "CbmL1MuchTrack.h"

#include "CbmKFMath.h"
#include "CbmStsTrack.h"

ClassImp(CbmL1MuchTrack);

void CbmL1MuchTrack::SetStsTrack(CbmStsTrack* track)
{
  CbmKFMath::CopyTrackParam2TC(track->GetParamLast(), T, C);
  chi2 = 0;  //track->GetChi2();
  NDF  = 0;  //track->GetNDF();
}
