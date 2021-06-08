/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Pascal Raisig */

#ifndef COMPARETRDDIGI_H
#define COMPARETRDDIGI_H 1

#include "gtest/gtest.h"

void compareTrdDigiDataMembers(CbmTrdDigi& test, Int_t padChNr, ECbmModuleId systemid, ULong64_t time, Double_t charge)
{
  Int_t retValInt {-222};
  Double_t retValDouble {-222.};
  ECbmModuleId retVal {ECbmModuleId::kNotExist};

  retValInt = test.GetAddressChannel();
  EXPECT_EQ(padChNr, retValInt);

  retValInt = test.GetAddressModule();
  EXPECT_EQ((Int_t) systemid, retValInt);

  // GetAddress() returns the full Address part of the fInfo data member. However, since Module-5 translated via CbmTrdAddress corresponds to the value 0 it should return the setted channel number.
  retValInt = test.GetAddress();
  EXPECT_EQ(padChNr, retValInt);

  retVal = test.GetSystem();
  EXPECT_EQ(systemid, retVal);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(static_cast<Double_t>(time), retValDouble);

  retValDouble = test.GetCharge();
  EXPECT_FLOAT_EQ(static_cast<Double_t>(charge), retValDouble);
}

#endif  // COMPARETRDDIGI_H
