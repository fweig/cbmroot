/** @file _GTestCbmPsdAddress.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmPsdAddress.h"

#include "comparePsdAddress.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmPsdAddress, CheckAddress)
{
  Int_t moduleid {1};
  Int_t sectionid {0};
  Int_t address {24};

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
  Int_t moduleid {1};
  Int_t sectionid {0};
  Int_t address {24};

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);

  EXPECT_EQ(88, CbmPsdAddress::SetModuleId(24, 5));
}

TEST(_GTestCbmPsdAddress, CheckSetSectionId)
{
  Int_t moduleid {1};
  Int_t sectionid {0};
  Int_t address {24};

  comparePsdAddressDataMembers(address, ECbmModuleId::kPsd, moduleid, sectionid);

  EXPECT_EQ(5144, CbmPsdAddress::SetSectionId(24, 5));
}
