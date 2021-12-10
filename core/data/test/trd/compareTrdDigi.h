/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Pascal Raisig */

#ifndef COMPARETRDDIGI_H
#define COMPARETRDDIGI_H 1

#include "gtest/gtest.h"

void compareTrdDigiDataMembers(CbmTrdDigi& test, int32_t padChNr, ECbmModuleId systemid, uint64_t time, double charge)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddressChannel();
  EXPECT_EQ(padChNr, retValInt);

  retValInt = test.GetAddressModule();
  EXPECT_EQ((int32_t) systemid, retValInt);

  // GetAddress() returns the full Address part of the fInfo data member. However, since Module-5 translated via CbmTrdAddress corresponds to the value 0 it should return the setted channel number.
  retValInt = test.GetAddress();
  EXPECT_EQ(padChNr, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<double>(time), retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(static_cast<double>(charge), retValDouble);
}

void compareTrdDigiDataMembers(CbmTrdDigi& test, int32_t padChNr, ECbmModuleId systemid, uint64_t time,
                               CbmTrdDigi::eTriggerType triggerType, double charge)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddressChannel();
  EXPECT_EQ(padChNr, retValInt);

  retValInt = test.GetAddressModule();
  EXPECT_EQ((int32_t) systemid, retValInt);

  // GetAddress() returns the full Address part of the fInfo data member. However, since Module-5 translated via CbmTrdAddress corresponds to the value 0 it should return the setted channel number.
  retValInt = test.GetAddress();
  EXPECT_EQ(padChNr, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<double>(time), retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(static_cast<double>(charge), retValDouble);

  retValInt = test.GetTriggerType();
  EXPECT_EQ(static_cast<int32_t>(triggerType), retValInt);
}

#endif  // COMPARETRDDIGI_H
