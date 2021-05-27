/** @file _GTestCbmTofDigi.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmTofDigi.h"

#include "compareTofDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmTofDigi, CheckDefaultConstructor)
{
  // Create object
  CbmTofDigi test;

  compareTofDigiDataMembers(test, 0, ECbmModuleId::kTof, 0., -1.);

  CbmTofDigi* test1 = new CbmTofDigi();

  compareTofDigiDataMembers(*test1, 0, ECbmModuleId::kTof, 0., -1.);
}

TEST(_GTestCbmTofDigi, CheckStandardConstructor)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  CbmTofDigi* test1 = new CbmTofDigi(111, 987654321., 23.);

  compareTofDigiDataMembers(*test1, 111, ECbmModuleId::kTof, 987654321., 23.);
}

TEST(_GTestCbmTofDigi, CheckCopyConstructor)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmTofDigi test2 {test};

  compareTofDigiDataMembers(test2, 111, ECbmModuleId::kTof, 987654321., 23.);

  // Test if the original object wasn't changed
  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);
}

TEST(_GTestCbmTofDigi, CheckAssignmentOperator)
{

  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmTofDigi test2;
  test2 = test;

  compareTofDigiDataMembers(test2, 111, ECbmModuleId::kTof, 987654321., 23.);

  // Test if the original object wasn't changed
  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);
}

TEST(_GTestCbmTofDigi, CheckMoveConstructor)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  // Create object by move constructing
  CbmTofDigi test2 {std::move(test)};

  compareTofDigiDataMembers(test2, 111, ECbmModuleId::kTof, 987654321., 23.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);
}

TEST(_GTestCbmTofDigi, CheckAssignmentMoveConstructor)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  // Create object by move constructing
  CbmTofDigi test2 {};
  test2 = std::move(test);

  compareTofDigiDataMembers(test2, 111, ECbmModuleId::kTof, 987654321., 23.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);
}


TEST(_GTestCbmTofDigi, CheckToString)
{
  // Create object
  CbmTofDigi test(12341234, 987654321., 23.);

  compareTofDigiDataMembers(test, 12341234, ECbmModuleId::kTof, 987654321., 23.);

  EXPECT_STREQ("CbmTofDigi: address = 0x00BC4FF2 time = 987654321.000000 tot = 23.000000", test.ToString().c_str());
}

TEST(_GTestCbmTofDigi, CheckGetClassName)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  EXPECT_STREQ("CbmTofDigi", test.GetClassName());
}

TEST(_GTestCbmTofDigi, CheckSetTime)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  test.SetTime(12345678.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 12345678., 23.);
}

TEST(_GTestCbmTofDigi, CheckSetAddress)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  test.SetAddress(12341234);

  compareTofDigiDataMembers(test, 12341234, ECbmModuleId::kTof, 987654321., 23.);
}

TEST(_GTestCbmTofDigi, CheckSetTot)
{
  // Create object
  CbmTofDigi test(111, 987654321., 23.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 23.);

  test.SetTot(45.);

  compareTofDigiDataMembers(test, 111, ECbmModuleId::kTof, 987654321., 45.);
}
