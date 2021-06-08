/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREPSDDIGI_H
#define COMPAREPSDDIGI_H 1

#include "gtest/gtest.h"

void comparePsdDigiDataMembers(CbmPsdDigi& test, Int_t address, ECbmModuleId systemid, Double_t time, Double_t edep)
{
  Int_t retValInt {-222};
  Double_t retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(time, retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(edep, retValDouble);

  retValDouble = test.GetEdep();
  EXPECT_FLOAT_EQ(edep, retValDouble);
}

#endif  // COMPAREPSDDIGI_H
