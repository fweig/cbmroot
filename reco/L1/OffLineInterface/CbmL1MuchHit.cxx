/* Copyright (C) 2007 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#include "CbmL1MuchHit.h"

#include "CbmKF.h"
#include "CbmKFTrackInterface.h"
#include "CbmMuchHit.h"

ClassImp(CbmL1MuchHit);

void CbmL1MuchHit::Create(CbmMuchHit* h, int ind)
{
  FitPoint.x    = h->GetX();
  FitPoint.y    = h->GetY();
  FitPoint.z    = h->GetZ();
  FitPoint.V[0] = h->GetDx() * h->GetDx();
  FitPoint.V[1] = 0;
  FitPoint.V[2] = h->GetDy() * h->GetDy();

  CbmKF* KF     = CbmKF::Instance();
  iStation      = h->GetStationNr() - 1;
  MaterialIndex = KF->GetMaterialIndex(KF->MuchStation2MCIDMap[iStation]);
  time          = h->GetTime(0);
  busy          = 0;
  index         = ind;
}

Int_t CbmL1MuchHit::Filter(CbmKFTrackInterface& track, Bool_t downstream, Double_t& QP0)
{
  Bool_t err = 0;
  err        = err || track.Propagate(FitPoint.z, QP0);
  err        = err || FitPoint.Filter(track);
  return err;
}
