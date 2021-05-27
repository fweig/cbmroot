/** @file _GTestCbmAddress.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmAddress.h"
#include "CbmDefs.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmAddress, CheckDefaultConstructor)
{
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    CbmAddress test;
    CbmAddress* test1 = new CbmAddress();
    delete test1;
  }
}

TEST(_GTestCbmAddress, CheckGetNofSystemBits)
{
  SCOPED_TRACE("CheckGetNofSystemBits");
  CbmAddress test;
  EXPECT_EQ(4, test.GetNofSystemBits());
}

TEST(_GTestCbmAddress, CheckGetSytemId)
{
  SCOPED_TRACE("CheckGetNofSystemBits");
  CbmAddress test;

  UInt_t testID {ToIntegralType(ECbmModuleId::kSts)};
  EXPECT_EQ(ToIntegralType(ECbmModuleId::kSts), test.GetSystemId(testID));

  testID = ToIntegralType(ECbmModuleId::kTrd);
  EXPECT_EQ(ToIntegralType(ECbmModuleId::kTrd), test.GetSystemId(testID));

  testID = ToIntegralType(ECbmModuleId::kTof);
  EXPECT_EQ(ToIntegralType(ECbmModuleId::kTof), test.GetSystemId(testID));
}
