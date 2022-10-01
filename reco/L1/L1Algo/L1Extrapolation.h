/* Copyright (C) 2007-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak, Sergey Gorbunov [committer], Igor Kulakov */

#ifndef L1Extrapolation_h
#define L1Extrapolation_h

#include "L1Def.h"
#include "L1Field.h"
#include "L1TrackPar.h"

void L1Extrapolate  // extrapolates track parameters and returns jacobian for extrapolation of CovMatrix
  (L1TrackPar& T,   // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
   fvec z_out,      // extrapolate to this z position
   fvec qp0,        // use Q/p linearisation at this value
   const L1FieldRegion& F, const fvec* w = nullptr);

void L1Extrapolate0(L1TrackPar& T,  // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
                    fvec z_out,     // extrapolate to this z position
                    L1FieldRegion& F);

void L1ExtrapolateTime(L1TrackPar& T,  // input track parameters (x,y,tx,ty,Q/p) and cov.matrix
                       fvec dz,        // extrapolate to this z position
                       fvec timeInfo = fvec::One());

void L1ExtrapolateLine(L1TrackPar& T, fvec z_out);

void L1ExtrapolateJXY  // is not used currently
  (fvec& tx, fvec& ty,
   fvec& qp,  // input track parameters
   fvec dz,   // extrapolate to this dz
   L1FieldRegion& F, fvec& extrDx, fvec& extrDy, fvec extrJ[]);

void L1ExtrapolateJXY0(fvec& tx,
                       fvec& ty,  // input track slopes
                       fvec dz,   // extrapolate to this dz position
                       L1FieldRegion& F, fvec& extrDx, fvec& extrDy, fvec& J04, fvec& J14);


inline void L1ExtrapolateXC00Line(const L1TrackPar& T, fvec z_out, fvec& x, fvec& C00)
{
  const fvec dz = (z_out - T.z);
  x             = T.x + T.tx * dz;
  C00           = T.C00 + dz * (2 * T.C20 + dz * T.C22);
}

inline void L1ExtrapolateYC11Line(const L1TrackPar& T, fvec z_out, fvec& y, fvec& C11)
{
  const fvec dz = (z_out - T.z);
  y             = T.y + T.ty * dz;
  C11           = T.C11 + dz * (2 * T.C31 + dz * T.C33);
}

inline void L1ExtrapolateC10Line(const L1TrackPar& T, fvec z_out, fvec& C10)
{
  const fvec dz = (z_out - T.z);
  C10           = T.C10 + dz * (T.C21 + T.C30 + dz * T.C32);
}

#endif
