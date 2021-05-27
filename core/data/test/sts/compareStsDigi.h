/** @file compareStsDigi.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef COMPARESTSDIGI_H
#define COMPARESTSDIGI_H 1

#include "gtest/gtest.h"

void compareStsDigiDataMembers(CbmStsDigi& test, Int_t address, UShort_t charge, UShort_t channel,
                               ECbmModuleId systemid, Long64_t time)
{
  Int_t retValInt {-222};
  Double_t retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(static_cast<Double_t>(charge), retValDouble);

  retValInt = test.GetChannel();
  EXPECT_EQ(channel, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<Double_t>(time), retValDouble);
}

#endif  // COMPARESTSDIGI_H
