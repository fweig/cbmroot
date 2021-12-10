/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPARETRDHIT_H
#define COMPARETRDHIT_H 1

#include "comparePixelHit.h"
#include "gtest/gtest.h"

void compareTrdHitDataMembers(CbmTrdHit& test, HitType type, double z, double dz, int32_t refid, int32_t address,
                              CbmMatch* match, double time, double errortime, double x, double dx, double y, double dy,
                              double dxy, uint8_t define, double eloss)
{

  CbmPixelHit phit = static_cast<CbmPixelHit>(test);
  comparePixelHitDataMembers(phit, type, z, dz, refid, address, match, time, errortime, x, dx, y, dy, dxy);

  int32_t retValInt {-222};
  double retValDouble {-222.};

  retValDouble = test.GetELoss();
  EXPECT_FLOAT_EQ(eloss, retValDouble);

  // Extract the first 4 bits from the integer
  // and compare it to the functions
  retValInt = test.GetClassType();
  EXPECT_EQ((define & 1) >> 0, retValInt);

  retValInt = test.GetMaxType();
  EXPECT_EQ((define & 2) >> 1, retValInt);

  retValInt = test.IsRowCross();
  EXPECT_EQ((define & 4) >> 2, retValInt);

  retValInt = test.HasOverFlow();
  EXPECT_EQ((define & 8) >> 3, retValInt);
}

#endif  // COMPARETRDHIT_H
