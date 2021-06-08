/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmPsdPoint.h"

#include "comparePsdPoint.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"


TEST(_GTestCbmPsdPoint, CheckDefaultConstructor)
{
  // Create object
  CbmPsdPoint test;

  comparePsdPointDataMembers(test, -1, -1, TVector3(0., 0., 0.), TVector3(0., 0., 0.), 0., 0., 0., 0, 0);

  CbmPsdPoint* test1 = new CbmPsdPoint();

  comparePsdPointDataMembers(*test1, -1, -1, TVector3(0., 0., 0.), TVector3(0., 0., 0.), 0., 0., 0., 0, 0);
}

TEST(_GTestCbmPsdPoint, CheckStandardConstructor)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  CbmPsdPoint* test1 = new CbmPsdPoint(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(*test1, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);
}

TEST(_GTestCbmPsdPoint, CheckCopyConstructor)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmPsdPoint test2 {test};

  comparePsdPointDataMembers(test2, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // Test if the original object wasn't changed
  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);
}

TEST(_GTestCbmPsdPoint, CheckCopyAssignmentOperator)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmPsdPoint test2 {};
  test2 = test;

  comparePsdPointDataMembers(test2, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // Test if the original object wasn't changed
  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);
}

TEST(_GTestCbmPsdPoint, CheckMoveConstructor)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmPsdPoint test2 {std::move(test)};

  comparePsdPointDataMembers(test2, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);
}

TEST(_GTestCbmPsdPoint, CheckAssignmentOperator)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmPsdPoint test2 {};
  test2 = std::move(test);

  comparePsdPointDataMembers(test2, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);
}

TEST(_GTestCbmPsdPoint, CheckSetModuleID)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);


  test.SetModuleID(111);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, 111, eventid);
}

TEST(_GTestCbmPsdPoint, CheckToString)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  EXPECT_STREQ("PSD point for track 34 in detector 45\n    Position (-2.1, "
               "6.1, 23) cm\n    Momentum (2.5, 4.5, 78) GeV\n    Time 34.56 "
               "ns,  Length 1.2 cm,  Energy loss 456700 keV",
               test.ToString().c_str());
}

TEST(_GTestCbmPsdPoint, CheckPrint)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 pos {-2.1, 6.1, 23.};
  TVector3 mom {2.5, 4.5, 78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  Int_t moduleid {0};
  // Create object
  CbmPsdPoint test(trackid, detid, pos, mom, tof, length, eloss);

  comparePsdPointDataMembers(test, trackid, detid, pos, mom, tof, length, eloss, moduleid, eventid);

  testing::internal::CaptureStdout();
  test.Print("");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_STREQ("[INFO] PSD point for track 34 in detector 45\n    Position "
               "(-2.1, 6.1, 23) cm\n    Momentum (2.5, 4.5, 78) GeV\n    Time "
               "34.56 ns,  Length 1.2 cm,  Energy loss 456700 keV\n",
               output.c_str());
}
