/** @file _GTestCbmStsDigi.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmStsDigi.h"

#include "compareStsDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmStsDigi, CheckDefaultConstructor)
{
  // Create object
  CbmStsDigi test;

  compareStsDigiDataMembers(test, 0, 0, 0, ECbmModuleId::kSts, 0);

  CbmStsDigi* test1 = new CbmStsDigi();

  compareStsDigiDataMembers(*test1, 0, 0, 0, ECbmModuleId::kSts, 0);
}

TEST(_GTestCbmStsDigi, CheckStandardConstructor)
{
  // Create object
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  CbmStsDigi* test1 = new CbmStsDigi(111, 42, 897654321, 23);

  compareStsDigiDataMembers(*test1, 111, 23, 42, ECbmModuleId::kSts, 897654321);
}

TEST(_GTestCbmStsDigi, CheckCopyConstructor)
{
  // Create object
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmStsDigi test2 {test};

  compareStsDigiDataMembers(test2, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // Test if the original object wasn't changed
  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckAssignmentOperator)
{

  // Create object
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmStsDigi test2;
  test2 = test;

  compareStsDigiDataMembers(test2, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // Test if the original object wasn't changed
  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckMoveConstructor)
{
  // Create object
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by move constructing
  CbmStsDigi test2 {std::move(test)};

  compareStsDigiDataMembers(test2, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckAssignmentMoveConstructor)
{
  // Create object
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // Create object by move constructing
  CbmStsDigi test2 {};
  test2 = std::move(test);

  compareStsDigiDataMembers(test2, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);
}

TEST(_GTestCbmStsDigi, CheckSetTime)
{
  // Create object
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  test.SetTime(897654321.0);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 897654321);
}

TEST(_GTestCbmStsDigi, CheckToString)
{
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  EXPECT_STREQ("StsDigi: address 111 | channel 42 | charge 23 | time 987654321", test.ToString().c_str());
}

TEST(_GTestCbmStsDigi, CheckGetClassName)
{
  CbmStsDigi test(111, 42, 987654321, 23);

  compareStsDigiDataMembers(test, 111, 23, 42, ECbmModuleId::kSts, 987654321);

  EXPECT_STREQ("CbmStsDigi", test.GetClassName());
}
