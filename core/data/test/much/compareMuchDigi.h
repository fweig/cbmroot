/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREMUCHDIGI_H
#define COMPAREMUCHDIGI_H 1

#include "gtest/gtest.h"

void compareMuchDigiDataMembers(CbmMuchDigi& test, int32_t address, ECbmModuleId systemid, uint64_t time,
                                int32_t charge)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<double>(time), retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_EQ(static_cast<double>(charge), retValDouble);

  retValInt = test.GetAdc();
  EXPECT_EQ(charge, retValInt);
}

#endif  // COMPAREMUCHDIGI_H
