/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPARESTRIPHIT_H
#define COMPARESTRIPHIT_H 1

#include "CbmMatch.h"
#include "CbmStripHit.h"

#include "compareHit.h"
#include "gtest/gtest.h"

void compareStripHitDataMembers(CbmStripHit& test, HitType type, double z, double dz, int32_t refid, int32_t address,
                                CbmMatch* match, double time, double errortime, double u, double du, double phi,
                                double dphi)
{

  compareHitDataMembers(test, type, z, dz, refid, address, match, time, errortime);

  float retValFloat {-111.};

  retValFloat = test.GetU();
  EXPECT_FLOAT_EQ(u, retValFloat);

  retValFloat = test.GetDu();
  EXPECT_FLOAT_EQ(du, retValFloat);

  retValFloat = test.GetPhi();
  EXPECT_FLOAT_EQ(phi, retValFloat);

  retValFloat = test.GetDphi();
  EXPECT_FLOAT_EQ(dphi, retValFloat);
}

#endif  // COMPARESTRIPHIT_H
