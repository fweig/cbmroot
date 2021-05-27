/** @file _GTestCbmTrdPoint.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmTrdPoint.h"

#include "compareTrdPoint.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmTrdPoint, CheckDefaultConstructor)
{
  // Create object
  CbmTrdPoint test;

  compareTrdPointDataMembers(test, -1, -1, TVector3(0., 0., 0.), TVector3(0., 0., 0.), TVector3(0., 0., 0.),
                             TVector3(0., 0., 0.), 0., 0., 0., 0);

  CbmTrdPoint* test1 = new CbmTrdPoint();

  compareTrdPointDataMembers(*test1, -1, -1, TVector3(0., 0., 0.), TVector3(0., 0., 0.), TVector3(0., 0., 0.),
                             TVector3(0., 0., 0.), 0., 0., 0., 0);
}

TEST(_GTestCbmTrdPoint, CheckStandardConstructor)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  CbmTrdPoint* test1 = new CbmTrdPoint(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(*test1, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);
}

TEST(_GTestCbmTrdPoint, CheckCopyConstructor)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmTrdPoint test2 {test};

  compareTrdPointDataMembers(test2, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // Test if the original object wasn't changed
  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);
}

TEST(_GTestCbmTrdPoint, CheckCopyAssignmentOperator)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmTrdPoint test2 {};
  test2 = test;

  compareTrdPointDataMembers(test2, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // Test if the original object wasn't changed
  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);
}

TEST(_GTestCbmTrdPoint, CheckMoveConstructor)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmTrdPoint test2 {std::move(test)};

  compareTrdPointDataMembers(test2, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);
}

TEST(_GTestCbmTrdPoint, CheckAssignmentOperator)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmTrdPoint test2 {};
  test2 = std::move(test);

  compareTrdPointDataMembers(test2, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);
}

TEST(_GTestCbmTrdPoint, CheckPrint)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);

  testing::internal::CaptureStdout();
  test.Print("");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_STREQ("[INFO] TRD point for track 34 in detector 45\n[INFO]     "
               "Position In (-2.1, 6.1, 23) cm\n[INFO]     Momentum In (2.5, "
               "4.5, 78) GeV\n[INFO]     Position Out (2.1, -6.1, -23) "
               "cm\n[INFO]     Momentum Out (-2.5, -4.5, -78) GeV\n[INFO]     "
               "Time 34.56 ns,  Length 1.2 cm,  Energy loss 456700 keV\n",
               output.c_str());
}


TEST(_GTestCbmTrdPoint, CheckPositionOut)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);


  TVector3 testVect {0., 0., 0.};

  test.PositionOut(testVect);

  EXPECT_FLOAT_EQ(posout.X(), testVect.X());
  EXPECT_FLOAT_EQ(posout.Y(), testVect.Y());
  EXPECT_FLOAT_EQ(posout.Z(), testVect.Z());
}

TEST(_GTestCbmTrdPoint, CheckMomentumOut)
{
  Int_t trackid {34};
  Int_t detid {45};
  TVector3 posin {-2.1, 6.1, 23.};
  TVector3 momin {2.5, 4.5, 78.};
  TVector3 posout {2.1, -6.1, -23.};
  TVector3 momout {-2.5, -4.5, -78.};
  Double_t tof {34.56};
  Double_t length {1.2};
  Double_t eloss {0.4567};
  Int_t eventid {0};
  // Create object
  CbmTrdPoint test(trackid, detid, posin, momin, posout, momout, tof, length, eloss);

  compareTrdPointDataMembers(test, trackid, detid, posin, momin, posout, momout, tof, length, eloss, eventid);


  TVector3 testVect {0., 0., 0.};

  test.MomentumOut(testVect);

  EXPECT_FLOAT_EQ(momout.X(), testVect.X());
  EXPECT_FLOAT_EQ(momout.Y(), testVect.Y());
  EXPECT_FLOAT_EQ(momout.Z(), testVect.Z());
}
