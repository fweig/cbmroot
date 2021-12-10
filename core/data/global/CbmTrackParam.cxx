/* Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----                    CbmTrackParam source file                 -----
// -----                  Created 05/02/16  by T. Ablyazimov          -----
// -------------------------------------------------------------------------
#include "CbmTrackParam.h"

#include <FairTrackParam.h>  // for FairTrackParam

#include <cmath>

void CbmTrackParam::Set(const FairTrackParam& ftp, double time, double timeError)
{
  SetX(ftp.GetX());
  SetY(ftp.GetY());
  SetZ(ftp.GetZ());
  SetTx(ftp.GetTx());
  SetTy(ftp.GetTy());
  SetQp(ftp.GetQp());
  fTime  = time;
  fDTime = timeError;
  double cov[15];
  ftp.CovMatrix(cov);
  SetCovMatrix(cov);
  double p          = (abs(ftp.GetQp()) > 1.e-4) ? 1. / abs(ftp.GetQp()) : 1.e4;
  double norma      = sqrt(ftp.GetTx() * ftp.GetTx() + ftp.GetTy() * ftp.GetTy() + 1);
  fPz               = p / norma;
  fPx               = ftp.GetTx() * fPz;
  fPy               = ftp.GetTy() * fPz;
  double DpzByDqp   = -p * p / norma;
  double DpzByDtx   = -p * ftp.GetTx() / (norma * norma * norma);
  double DpzByDty   = -p * ftp.GetTy() / (norma * norma * norma);
  fDpz =
    sqrt(DpzByDqp * DpzByDqp * cov[14] + DpzByDtx * DpzByDtx * cov[9] + DpzByDty * DpzByDty * cov[12]
         + 2 * DpzByDqp * DpzByDtx * cov[11] + 2 * DpzByDqp * DpzByDty * cov[13] + 2 * DpzByDtx * DpzByDty * cov[10]);
  double DpxByDqp = ftp.GetTx() * DpzByDqp;
  double DpxByDtx = p * (ftp.GetTy() * ftp.GetTy() + 1) / (norma * norma * norma);
  double DpxByDty = -p * ftp.GetTx() * ftp.GetTy() / (norma * norma * norma);
  fDpx =
    sqrt(DpxByDqp * DpxByDqp * cov[14] + DpxByDtx * DpxByDtx * cov[9] + DpxByDty * DpxByDty * cov[12]
         + 2 * DpxByDqp * DpxByDtx * cov[11] + 2 * DpxByDqp * DpxByDty * cov[13] + 2 * DpxByDtx * DpxByDty * cov[10]);
  double DpyByDqp = ftp.GetTy() * DpzByDqp;
  double DpyByDtx = -p * ftp.GetTx() * ftp.GetTy() / (norma * norma * norma);
  double DpyByDty = p * (ftp.GetTx() * ftp.GetTx() + 1) / (norma * norma * norma);
  fDpy =
    sqrt(DpyByDqp * DpyByDqp * cov[14] + DpyByDtx * DpyByDtx * cov[9] + DpyByDty * DpyByDty * cov[12]
         + 2 * DpyByDqp * DpyByDtx * cov[11] + 2 * DpyByDqp * DpyByDty * cov[13] + 2 * DpyByDtx * DpyByDty * cov[10]);
}

ClassImp(CbmTrackParam)
