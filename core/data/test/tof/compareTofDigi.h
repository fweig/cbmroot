/** @file compareTofDigi.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef COMPARETOFDIGI_H
#define COMPARETOFDIGI_H 1

#include "gtest/gtest.h"

void compareTofDigiDataMembers(CbmTofDigi& test, Int_t address, ECbmModuleId systemid, Double_t time, Double_t tot)
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
  EXPECT_FLOAT_EQ(tot, retValDouble);

  retValDouble = test.GetTot();
  EXPECT_FLOAT_EQ(tot, retValDouble);
}

#endif  // COMPARETOFDIGI_H
