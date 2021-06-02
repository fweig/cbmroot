/** @file comparePixelHit.h
  * @copyright Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef COMPAREPIXELHIT_H
#define COMPAREPIXELHIT_H 1

#include "CbmMatch.h"
#include "CbmPixelHit.h"

#include "compareHit.h"
#include "gtest/gtest.h"

void comparePixelHitDataMembers(CbmPixelHit& test, HitType type, Double_t z, Double_t dz, Int_t refid, Int_t address,
                                CbmMatch* match, Double_t time, Double_t errortime, Double_t x, Double_t dx, Double_t y,
                                Double_t dy, Double_t dxy)
{

  compareHitDataMembers(test, type, z, dz, refid, address, match, time, errortime);

  Float_t retValFloat {-111.};

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
