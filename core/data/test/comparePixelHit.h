/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREPIXELHIT_H
#define COMPAREPIXELHIT_H 1

#include "CbmMatch.h"
#include "CbmPixelHit.h"

#include "compareHit.h"
#include "gtest/gtest.h"

void comparePixelHitDataMembers(CbmPixelHit& test, HitType type, double z, double dz, int32_t refid, int32_t address,
                                CbmMatch* match, double time, double errortime, double x, double dx, double y,
                                double dy, double dxy)
{

  compareHitDataMembers(test, type, z, dz, refid, address, match, time, errortime);

  float retValFloat {-111.};

  retValFloat = test.GetX();
  EXPECT_FLOAT_EQ(x, retValFloat);

  retValFloat = test.GetDx();
  EXPECT_FLOAT_EQ(dx, retValFloat);

  retValFloat = test.GetY();
  EXPECT_FLOAT_EQ(y, retValFloat);

  retValFloat = test.GetDy();
  EXPECT_FLOAT_EQ(dy, retValFloat);

  retValFloat = test.GetDxy();
  EXPECT_FLOAT_EQ(dxy, retValFloat);
}


#endif  // COMPAREPIXELHIT_H
