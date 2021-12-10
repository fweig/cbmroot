/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREHIT_H
#define COMPAREHIT_H 1

#include "CbmHit.h"
#include "CbmMatch.h"

#include "compareMatch.h"
#include "gtest/gtest.h"

void compareHitDataMembers(CbmHit& test, HitType type, double z, double dz, int32_t refid, int32_t address,
                           CbmMatch* match, double time, double errortime)
{
  int32_t retValInt {-111};
  float retValFloat {-111.};

  EXPECT_EQ(type, test.GetType());

  retValFloat = test.GetZ();
  EXPECT_FLOAT_EQ(z, retValFloat);

  retValFloat = test.GetDz();
  EXPECT_FLOAT_EQ(dz, retValFloat);

  retValInt = test.GetRefId();
  EXPECT_EQ(refid, retValInt);

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  if (match != nullptr) { compareMatchDataMembers(*test.GetMatch(), match->GetNofLinks(), match->GetTotalWeight()); }
  else {
    EXPECT_EQ(match, test.GetMatch());
  }

  retValFloat = test.GetTime();
  EXPECT_FLOAT_EQ(time, retValFloat);

  retValFloat = test.GetTimeError();
  EXPECT_FLOAT_EQ(errortime, retValFloat);
}

#endif  // COMPAREHIT_H
