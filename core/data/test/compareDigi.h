/** @file compareDigi.h
  * @copyright Copyright (C) 2017-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef COMPAREDIGI_H
#define COMPAREDIGI_H 1

#include "gtest/gtest.h"

void compareDigiDataMembers(CbmTestDigi& test, Int_t address, Double_t charge, Int_t systemid, Double_t time)
{
  Int_t retValInt {-222};
  Double_t retValDouble {-222.};

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
