/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifndef COMPAREBMONDIGI_H
#define COMPAREBMONDIGI_H 1

#include "gtest/gtest.h"

void compareBmonDigiDataMembers(CbmBmonDigi& test, int32_t address, ECbmModuleId systemid, double time, double charge)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(time, retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(charge, retValDouble);
}

#endif  // COMPARETOFDIGI_H
