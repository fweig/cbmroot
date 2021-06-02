/** @file _GTestCbmRichDigi.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmRichDigi.h"

#include "compareRichDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmRichDigi, CheckDefaultConstructor)
{
  // Create object
  CbmRichDigi test;

  compareRichDigiDataMembers(test, 0, ECbmModuleId::kRich, 0., -0.);

  CbmRichDigi* test1 = new CbmRichDigi();

  compareRichDigiDataMembers(*test1, 0, ECbmModuleId::kRich, 0., -0.);
}

TEST(_GTestCbmRichDigi, CheckStandardConstructor)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  CbmRichDigi* test1 = new CbmRichDigi(111, 987654321., 23.);

  compareRichDigiDataMembers(*test1, 111, ECbmModuleId::kRich, 987654321., 23.);
}

TEST(_GTestCbmRichDigi, CheckCopyConstructor)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmRichDigi test2 {test};

  compareRichDigiDataMembers(test2, 111, ECbmModuleId::kRich, 987654321., 23.);

  // Test if the original object wasn't changed
  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);
}

TEST(_GTestCbmRichDigi, CheckAssignmentOperator)
{

  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmRichDigi test2;
  test2 = test;

  compareRichDigiDataMembers(test2, 111, ECbmModuleId::kRich, 987654321., 23.);

  // Test if the original object wasn't changed
  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);
}

TEST(_GTestCbmRichDigi, CheckMoveConstructor)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  // Create object by move constructing
  CbmRichDigi test2 {std::move(test)};

  compareRichDigiDataMembers(test2, 111, ECbmModuleId::kRich, 987654321., 23.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);
}

TEST(_GTestCbmRichDigi, CheckAssignmentMoveConstructor)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  // Create object by move constructing
  CbmRichDigi test2 {};
  test2 = std::move(test);

  compareRichDigiDataMembers(test2, 111, ECbmModuleId::kRich, 987654321., 23.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);
}


TEST(_GTestCbmRichDigi, CheckToString)
{
  // Create object
  CbmRichDigi test(12341234, 987654321., 23.);

  compareRichDigiDataMembers(test, 12341234, ECbmModuleId::kRich, 987654321., 23.);

  EXPECT_STREQ("", test.ToString().c_str());
}

TEST(_GTestCbmRichDigi, CheckGetClassName)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  EXPECT_STREQ("CbmRichDigi", test.GetClassName());
}

TEST(_GTestCbmRichDigi, CheckSetTime)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  test.SetTime(12345678.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 12345678., 23.);
}

TEST(_GTestCbmRichDigi, CheckSetAddress)
{
  // Create object
  CbmRichDigi test(111, 987654321., 23.);

  compareRichDigiDataMembers(test, 111, ECbmModuleId::kRich, 987654321., 23.);

  test.SetAddress(12341234);

  compareRichDigiDataMembers(test, 12341234, ECbmModuleId::kRich, 987654321., 23.);
}
