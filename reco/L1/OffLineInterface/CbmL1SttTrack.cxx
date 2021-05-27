/** @file CbmL1SttTrack.cxx
  * @copyright Copyright (C) 2008 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Sergey Gorbunov [orginator] **/

#include "CbmL1SttTrack.h"

#include "CbmKFMath.h"
#include "CbmMuchTrack.h"
#include "CbmStsTrack.h"

ClassImp(CbmL1SttTrack);

void CbmL1SttTrack::SetStsTrack(CbmStsTrack* track)
{
  CbmKFMath::CopyTrackParam2TC(track->GetParamLast(), T, C);
  chi2 = 0;  //track->GetChi2();
  NDF  = 0;  //track->GetNDF();
}

void CbmL1SttTrack::SetMuchTrack(CbmMuchTrack* track)
{
  CbmKFMath::CopyTrackParam2TC(track->GetMuchTrack(), T, C);
  chi2 = 0;  //track->GetChi2();
  NDF  = 0;  //track->GetNDF();
}
