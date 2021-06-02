/** @file _GTestCbmDefs.cxx
  * @copyright Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmDetectorList.h"

#include <TString.h>

#include <iostream>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>
using std::cout;
using std::endl;

// Structure to pass filenames together with expected response into the
// parametrized test
struct InOutStructure {
  DetectorId detIdFromEnum;
  int detId;
  TString detName;
  TString detNameUppercase;
};

// Base class to use the same basic setup for parameterized and
// non-parameterized tests
// Here one defines everything which is common for all the different
// test cases
template<class T>
class _TestCbmDetectorListBase : public T {
protected:
  CbmDetectorList fList;


  virtual void SetUp() {}

  virtual void TearDown() {}
};

// This is the derived class for the non-parameterized test cases.
class CbmDetectorListTest : public _TestCbmDetectorListBase<testing::Test> {
};

// This is the derived class for the parameterized test cases.
class CbmDetectorListParamTest : public _TestCbmDetectorListBase<testing::TestWithParam<InOutStructure>> {
protected:
  DetectorId detIdFromEnum;
  Int_t detId;
  TString detName;
  TString detNameUppercase;

  virtual void SetUp()
  {
    InOutStructure const& p = GetParam();

    detIdFromEnum    = p.detIdFromEnum;
    detId            = p.detId;
    detName          = p.detName;
    detNameUppercase = p.detNameUppercase;
  }
};

TEST(CbmDetectorList, outOfBounds)
{
  CbmDetectorList fList;
  TString retName;

  fList.GetSystemName(111, retName);
  EXPECT_EQ("unknown", retName);

  fList.GetSystemName(-111, retName);
  EXPECT_EQ("unknown", retName);

  fList.GetSystemNameCaps(111, retName);
  EXPECT_EQ("unknown", retName);

  fList.GetSystemNameCaps(-111, retName);
  EXPECT_EQ("UNKNOWN", retName);
}

TEST_P(CbmDetectorListParamTest, checkUniqueIdCreation)
{
  TString retName;
  fList.GetSystemName(detIdFromEnum, retName);
  EXPECT_EQ(detName, retName);

  fList.GetSystemName(detId, retName);
  EXPECT_EQ(detName, retName);

  fList.GetSystemNameCaps(detIdFromEnum, retName);
  EXPECT_EQ(detNameUppercase, retName);

  fList.GetSystemNameCaps(detId, retName);
  EXPECT_EQ(detNameUppercase, retName);
}

InOutStructure val1  = {kREF, 0, "ref", "REF"};
InOutStructure val2  = {kMVD, 1, "mvd", "MVD"};
InOutStructure val3  = {kSTS, 2, "sts", "STS"};
InOutStructure val4  = {kRICH, 3, "rich", "RICH"};
InOutStructure val5  = {kMUCH, 4, "much", "MUCH"};
InOutStructure val6  = {kTRD, 5, "trd", "TRD"};
InOutStructure val7  = {kTOF, 6, "tof", "TOF"};
InOutStructure val8  = {kECAL, 7, "ecal", "ECAL"};
InOutStructure val9  = {kPSD, 8, "unknown", "PSD"};
InOutStructure val10 = {kSTT, 9, "unknown", "UNKNOWN"};
InOutStructure val11 = {kFHODO, 10, "fhodo", "FHODO"};
InOutStructure val12 = {kTutDet, 11, "tutdet", "TUTDET"};
InOutStructure val13 = {kNOFDETS, 12, "unknown", "UNKNOWN"};

INSTANTIATE_TEST_CASE_P(TestAllParameters, CbmDetectorListParamTest,
                        ::testing::Values(val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12));
