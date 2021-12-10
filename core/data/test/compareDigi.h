/* Copyright (C) 2017-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREDIGI_H
#define COMPAREDIGI_H 1

#include "gtest/gtest.h"

void compareDigiDataMembers(CbmTestDigi& test, int32_t address, double charge, int32_t systemid, double time)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(charge, retValDouble);

  retValInt = test.GetSystemId();
  EXPECT_EQ(systemid, retValInt);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(time, retValDouble);
}

#endif  // COMPAREDIGI_H
