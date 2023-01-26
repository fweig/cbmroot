/* Copyright (C) 2023 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmBmonDigi.h"

#include "compareBmonDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmBmonDigi, CheckDefaultConstructor)
{
  // Create object
  CbmBmonDigi test;

  compareBmonDigiDataMembers(test, ToIntegralType<ECbmModuleId>(ECbmModuleId::kT0), ECbmModuleId::kT0, -1., -1.);

  CbmBmonDigi* test1 = new CbmBmonDigi();

  compareBmonDigiDataMembers(*test1, ToIntegralType<ECbmModuleId>(ECbmModuleId::kT0), ECbmModuleId::kT0, -1., -1.);
}

TEST(_GTestCbmBmonDigi, CheckStandardConstructor)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  CbmBmonDigi* test1 = new CbmBmonDigi(111, 987654321., 23.);

  compareBmonDigiDataMembers(*test1, 111, ECbmModuleId::kT0, 987654321., 23.);
}

TEST(_GTestCbmBmonDigi, CheckCopyConstructor)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmBmonDigi test2 {test};

  compareBmonDigiDataMembers(test2, 111, ECbmModuleId::kT0, 987654321., 23.);

  // Test if the original object wasn't changed
  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);
}

TEST(_GTestCbmBmonDigi, CheckAssignmentOperator)
{

  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmBmonDigi test2;
  test2 = test;

  compareBmonDigiDataMembers(test2, 111, ECbmModuleId::kT0, 987654321., 23.);

  // Test if the original object wasn't changed
  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);
}

TEST(_GTestCbmBmonDigi, CheckMoveConstructor)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  // Create object by move constructing
  CbmBmonDigi test2 {std::move(test)};

  compareBmonDigiDataMembers(test2, 111, ECbmModuleId::kT0, 987654321., 23.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);
}

TEST(_GTestCbmBmonDigi, CheckAssignmentMoveConstructor)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  // Create object by move constructing
  CbmBmonDigi test2 {};
  test2 = std::move(test);

  compareBmonDigiDataMembers(test2, 111, ECbmModuleId::kT0, 987654321., 23.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);
}

TEST(_GTestCbmBmonDigi, CheckGetClassName)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  EXPECT_STREQ("CbmBmonDigi", test.GetClassName());
}

TEST(_GTestCbmBmonDigi, CheckSetTime)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  test.SetTime(12345678.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 12345678., 23.);
}

TEST(_GTestCbmBmonDigi, CheckSetAddress)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  test.SetAddress(12341234);

  compareBmonDigiDataMembers(test, 12341234, ECbmModuleId::kT0, 987654321., 23.);
}

TEST(_GTestCbmBmonDigi, CheckSetCharge)
{
  // Create object
  CbmBmonDigi test(111, 987654321., 23.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 23.);

  test.SetCharge(45.);

  compareBmonDigiDataMembers(test, 111, ECbmModuleId::kT0, 987654321., 45.);
}
