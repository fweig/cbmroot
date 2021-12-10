/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmPsdAddress.h"

#include "comparePsdAddress.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmPsdAddress, CheckAddress)
{
  int32_t moduleid {1};
  int32_t sectionid {0};
  int32_t address {24};

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);

  moduleid  = 0;
  sectionid = 1;
  address   = 1032;

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);

  moduleid  = 5;
  sectionid = 5;
  address   = 5208;

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);
}

TEST(_GTestCbmPsdAddress, CheckSetModuleId)
{
  int32_t moduleid {1};
  int32_t sectionid {0};
  int32_t address {24};

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);

  EXPECT_EQ(88, CbmPsdAddress::SetModuleId(24, 5));
}

TEST(_GTestCbmPsdAddress, CheckSetSectionId)
{
  int32_t moduleid {1};
  int32_t sectionid {0};
  int32_t address {24};

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);

  EXPECT_EQ(5144, CbmPsdAddress::SetSectionId(24, 5));
}
