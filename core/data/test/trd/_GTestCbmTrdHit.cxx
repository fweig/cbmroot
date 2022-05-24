/* Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmTrdHit.h"

#include "compareTrdHit.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmTrdHit, CheckDefaultConstructor)
{
  // Create object
  CbmTrdHit test;

  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 0, -1.);

  CbmTrdHit* test1 = new CbmTrdHit();

  compareTrdHitDataMembers(*test1, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 0, -1.);
}

TEST(_GTestCbmTrdHit, CheckStandardConstructor)
{
  int32_t address {12345};
  TVector3 pos {-3., 5.777, 123.765};
  TVector3 error {0.77, 0.88, 0.99};
  double dxy {1.2};
  int32_t refId {6};
  double eloss {123.};

  // Create object
  CbmTrdHit test(address, pos, error, dxy, refId, eloss);

  compareTrdHitDataMembers(test, kTRDHIT, pos.Z(), error.Z(), refId, address, nullptr, 0., 0., pos.X(), error.X(),
                           pos.Y(), error.Y(), dxy, 0, eloss);


  CbmTrdHit* test1 = new CbmTrdHit(address, pos, error, dxy, refId, eloss);

  compareTrdHitDataMembers(*test1, kTRDHIT, pos.Z(), error.Z(), refId, address, nullptr, 0., 0., pos.X(), error.X(),
                           pos.Y(), error.Y(), dxy, 0, eloss);
}

TEST(_GTestCbmTrdHit, CheckStandardConstructorWithTime)
{
  int32_t address {12345};
  TVector3 pos {-3., 5.777, 123.765};
  TVector3 error {0.77, 0.88, 0.99};
  double dxy {1.2};
  int32_t refId {6};
  double eloss {123.};
  double time {456.};
  double timeError {1.34};
  // Create object
  CbmTrdHit test(address, pos, error, dxy, refId, eloss, time, timeError);

  compareTrdHitDataMembers(test, kTRDHIT, pos.Z(), error.Z(), refId, address, nullptr, time, timeError, pos.X(),
                           error.X(), pos.Y(), error.Y(), dxy, 0, eloss);


  CbmTrdHit* test1 = new CbmTrdHit(address, pos, error, dxy, refId, eloss, time, timeError);

  compareTrdHitDataMembers(*test1, kTRDHIT, pos.Z(), error.Z(), refId, address, nullptr, time, timeError, pos.X(),
                           error.X(), pos.Y(), error.Y(), dxy, 0, eloss);
}

TEST(_GTestCbmTrdHit, CheckSetBits)
{
  // Create object
  CbmTrdHit test;

  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 0, -1.);

  test.SetMaxType(true);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 2, -1.);

  test.SetClassType(true);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 3, -1.);

  test.SetMaxType(false);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 1, -1.);

  test.SetRowCross(true);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 5, -1.);

  test.SetClassType(false);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 4, -1.);

  test.SetOverFlow(true);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 12, -1.);

  test.SetRowCross(false);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 8, -1.);

  test.SetOverFlow(false);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 0, -1.);
}

TEST(_GTestCbmTrdHit, CheckSetEloss)
{
  // Create object
  CbmTrdHit test;

  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 0, -1.);

  test.SetELoss(134.56);
  compareTrdHitDataMembers(test, kTRDHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0., 0., 0, 134.56);
}

TEST(_GTestCbmTrdHit, CheckToString)
{
  int32_t address {12345};
  TVector3 pos {-3., 5.777, 123.765};
  TVector3 error {0.77, 0.88, 0.99};
  double dxy {1.2};
  int32_t refId {6};
  double eloss {123.};
  double time {456.};
  double timeError {1.34};
  // Create object
  CbmTrdHit test(address, pos, error, dxy, refId, eloss, time, timeError);

  compareTrdHitDataMembers(test, kTRDHIT, pos.Z(), error.Z(), refId, address, nullptr, time, timeError, pos.X(),
                           error.X(), pos.Y(), error.Y(), dxy, 0, eloss);


  EXPECT_STREQ("CbmPixelHit: address=12345 pos=(-3,5.777,123.765) "
               "err=(0.77,0.88,0.99) dxy=1.2 refId=6\nCbmTrdHit1D: "
               "time[ns]=456+-1.34 eloss=123 typ[R] RC=n Ovf=n\n",
               test.ToString().c_str());
}


/*
TEST(_GTestCbmTrdHit , CheckCopyConstructor)
{
  // Create object
  CbmTrdHit test(5, 6.7);

  compareTrdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmTrdHit test2{test};

  compareTrdHitDataMembers(test2, 5, 6.7);

  // Test if the original object wasn't changed
  compareTrdHitDataMembers(test, 5, 6.7);
}

TEST(_GTestCbmTrdHit , CheckAssignmentOperator)
{
  // Create object
  CbmTrdHit test(5, 6.7);

  compareTrdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmTrdHit test2{};
  test2 = test;

  compareTrdHitDataMembers(test2, 5, 6.7);

  // Test if the original object wasn't changed
  compareTrdHitDataMembers(test, 5, 6.7);
}

TEST(_GTestCbmTrdHit , CheckMoveConstructor)
{
  // Create object
  CbmTrdHit test(5, 6.7);

  compareTrdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmTrdHit test2{std::move(test)};

  compareTrdHitDataMembers(test2, 5, 6.7);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareTrdHitDataMembers(test, 5, 6.7);
}

TEST(_GTestCbmTrdHit , CheckAssignmentMoveConstructor)
{
  // Create object
  CbmTrdHit test(5, 6.7);

  compareTrdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmTrdHit test2{};
  test2 = std::move(test);

  compareTrdHitDataMembers(test2, 5, 6.7);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareTrdHitDataMembers(test, 5, 6.7);
}



*/
