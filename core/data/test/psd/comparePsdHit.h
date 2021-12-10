/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREPSDHIT_H
#define COMPAREPSDHIT_H 1

#include "gtest/gtest.h"

void comparePsdHitDataMembers(CbmPsdHit& test, int32_t moduleid, double edep)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};

  retValInt = test.GetModuleID();
  EXPECT_EQ(moduleid, retValInt);

  retValDouble = test.GetEdep();
  EXPECT_FLOAT_EQ(edep, retValDouble);
}

#endif  // COMPAREPSDHIT_H
