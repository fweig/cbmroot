/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPARESTSDIGI_H
#define COMPARESTSDIGI_H 1

#include "gtest/gtest.h"

void compareStsDigiDataMembers(CbmStsDigi& test, int32_t address, uint16_t charge, uint16_t channel,
                               ECbmModuleId systemid, int64_t time)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(static_cast<double>(charge), retValDouble);

  retValInt = test.GetChannel();
  EXPECT_EQ(channel, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<double>(time), retValDouble);
}

#endif  // COMPARESTSDIGI_H
