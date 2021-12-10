/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmStsAddress.h"
#include "CbmStsDigi.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "compareStsDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

static const int32_t kTestAddress = CbmStsAddress::GetAddress(5, 6, 1, 8, 0, 0, 1);


TEST(_GTestCbmStsDigi, CheckDefaultConstructor)
{
  // Create object
  CbmStsDigi test;

  const int32_t defaultAddr = CbmStsAddress::GetAddress(0, 0, 0, 0, 0, 0, 1);

  compareStsDigiDataMembers(test, defaultAddr, 0, 0, ECbmModuleId::kSts, 0);

  CbmStsDigi* test1 = new CbmStsDigi();

  compareStsDigiDataMembers(*test1, defaultAddr, 0, 0, ECbmModuleId::kSts, 0);
}

TEST(_GTestCbmStsDigi, CheckStandardConstructor)
{
  // Create object
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  CbmStsDigi* test1 = new CbmStsDigi(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(*test1, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckCopyConstructor)
{
  // Create object
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmStsDigi test2 {test};

  compareStsDigiDataMembers(test2, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // Test if the original object wasn't changed
  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckAssignmentOperator)
{

  // Create object
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmStsDigi test2;
  test2 = test;

  compareStsDigiDataMembers(test2, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // Test if the original object wasn't changed
  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckMoveConstructor)
{
  // Create object
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by move constructing
  CbmStsDigi test2 {std::move(test)};

  compareStsDigiDataMembers(test2, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckAssignmentMoveConstructor)
{
  // Create object
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by move constructing
  CbmStsDigi test2 {};
  test2 = std::move(test);

  compareStsDigiDataMembers(test2, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckSetTime)
{
  // Create object
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  test.SetTime(897654321.0);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 897654321);
}

TEST(_GTestCbmStsDigi, CheckToString)
{
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  std::string expected =
    "StsDigi: address " + std::to_string(kTestAddress) + " | channel 42 | charge 23 | time 987654321";

  EXPECT_STREQ(expected.c_str(), test.ToString().c_str());
}


TEST(_GTestCbmStsDigi, CheckGetClassName)
{
  CbmStsDigi test(kTestAddress, 42, 987654321, 23);

  compareStsDigiDataMembers(test, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);

  EXPECT_STREQ("CbmStsDigi", test.GetClassName());
}

TEST(_GTestCbmStsDigi, CheckSerialization)
{
  CbmStsDigi write(kTestAddress, 42, 987654321, 23);
  std::stringstream s;
  boost::archive::binary_oarchive outArchive(s);
  outArchive << write;
  CbmStsDigi read;
  boost::archive::binary_iarchive inArchive(s);
  inArchive >> read;
  compareStsDigiDataMembers(read, kTestAddress, 23, 42, ECbmModuleId::kSts, 987654321);
}
