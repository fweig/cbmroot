/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREPSDDIGI_H
#define COMPAREPSDDIGI_H 1

#include "gtest/gtest.h"

void comparePsdAddressDataMembers(Int_t address, ECbmModuleId systemid, Int_t moduleid, Int_t sectionid)
{
  Int_t retValInt {-222};

  retValInt = CbmPsdAddress::GetSystemId(address);
  EXPECT_EQ(ToIntegralType(systemid), retValInt);

  retValInt = CbmPsdAddress::GetModuleId(address);
  EXPECT_EQ(moduleid, retValInt);

  retValInt = CbmPsdAddress::GetSectionId(address);
  EXPECT_EQ(sectionid, retValInt);
}

#endif  // COMPAREPSDDIGI_H
