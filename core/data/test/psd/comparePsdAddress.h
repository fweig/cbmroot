/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREPSDDIGI_H
#define COMPAREPSDDIGI_H 1

#include "gtest/gtest.h"

void comparePsdAddressDataMembers(int32_t address, ECbmModuleId systemid, int32_t moduleid, int32_t sectionid)
{
  int32_t retValInt {-222};

  retValInt = CbmPsdAddress::GetSystemId(address);
  EXPECT_EQ(ToIntegralType(systemid), retValInt);

  retValInt = CbmPsdAddress::GetModuleId(address);
  EXPECT_EQ(moduleid, retValInt);

  retValInt = CbmPsdAddress::GetSectionId(address);
  EXPECT_EQ(sectionid, retValInt);
}

#endif  // COMPAREPSDDIGI_H
