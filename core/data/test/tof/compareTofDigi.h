/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPARETOFDIGI_H
#define COMPARETOFDIGI_H 1

#include "gtest/gtest.h"

void compareTofDigiDataMembers(CbmTofDigi& test, int32_t address, ECbmModuleId systemid, double time, double tot)
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
  EXPECT_FLOAT_EQ(tot, retValDouble);

  retValDouble = test.GetTot();
  EXPECT_FLOAT_EQ(tot, retValDouble);
}

#endif  // COMPARETOFDIGI_H
