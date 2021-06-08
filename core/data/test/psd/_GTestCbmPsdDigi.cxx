/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmPsdDigi.h"

#include "comparePsdDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmPsdDigi, CheckDefaultConstructor)
{
  // Create object
  CbmPsdDigi test;

  comparePsdDigiDataMembers(test, 0, ECbmModuleId::kPsd, -1., 0.);

  CbmPsdDigi* test1 = new CbmPsdDigi();

  comparePsdDigiDataMembers(*test1, 0, ECbmModuleId::kPsd, -1., 0.);
}

TEST(_GTestCbmPsdDigi, CheckStandardConstructor)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  CbmPsdDigi* test1 = new CbmPsdDigi(111, 987654321., 23.);

  comparePsdDigiDataMembers(*test1, 111, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckConstructorWithDetailedAssignment)
{
  // Create object
  CbmPsdDigi test(1, 5, 987654321., 23.);

  comparePsdDigiDataMembers(test, 5144, ECbmModuleId::kPsd, 23., 987654321.);

  CbmPsdDigi* test1 = new CbmPsdDigi(5, 5, 987654321., 23.);

  comparePsdDigiDataMembers(*test1, 5208, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckCopyConstructor)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmPsdDigi test2 {test};

  comparePsdDigiDataMembers(test2, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // Test if the original object wasn't changed
  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckAssignmentOperator)
{

  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmPsdDigi test2;
  test2 = test;

  comparePsdDigiDataMembers(test2, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // Test if the original object wasn't changed
  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckMoveConstructor)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // Create object by move constructing
  CbmPsdDigi test2 {std::move(test)};

  comparePsdDigiDataMembers(test2, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckAssignmentMoveConstructor)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  // Create object by move constructing
  CbmPsdDigi test2 {};  // = std::move(test);
  test2 = std::move(test);

  comparePsdDigiDataMembers(test2, 111, ECbmModuleId::kPsd, 23., 987654321.);


  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);
}


TEST(_GTestCbmPsdDigi, CheckToString)
{
  // Create object
  CbmPsdDigi test(12341234, 987654321., 23.);

  comparePsdDigiDataMembers(test, 12341234, ECbmModuleId::kPsd, 23., 987654321.);

  EXPECT_STREQ("CbmPsdDigi: address = 0x00BC4FF2 Charge = 987654321.000000 "
               "Time = 23.000000",
               test.ToString().c_str());
}

TEST(_GTestCbmPsdDigi, CheckGetClassName)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  EXPECT_STREQ("CbmPsdDigi", test.GetClassName());
}

TEST(_GTestCbmPsdDigi, CheckSetTime)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  test.SetTime(12345678.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 12345678., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckSetAddress)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  test.SetAddress(12341234);

  comparePsdDigiDataMembers(test, 12341234, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckSetAddressWithModuleAndSection)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  test.SetAddress(1, 5);

  comparePsdDigiDataMembers(test, 5144, ECbmModuleId::kPsd, 23., 987654321.);
}

TEST(_GTestCbmPsdDigi, CheckSetEdep)
{
  // Create object
  CbmPsdDigi test(111, 987654321., 23.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 987654321.);

  test.SetEdep(45.);

  comparePsdDigiDataMembers(test, 111, ECbmModuleId::kPsd, 23., 45.);
}
