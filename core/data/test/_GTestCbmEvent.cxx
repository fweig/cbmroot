/** @file _GTestCbmEvent.cxx
  * @copyright Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmDefs.h"
#include "CbmEvent.h"
#include "CbmMatch.h"
#include "CbmVertex.h"

#include <TMatrixTSym.h>

#include <vector>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

#include "compareVertex.h"

void compareEventDataMembers(CbmEvent& test, Int_t evnumber, Double_t starttime, Double_t endtime, Int_t numobjects,
                             CbmMatch* match, CbmVertex* vertex)
{
  Int_t retValInt {-111};
  Double_t retValDouble {-111.};

  retValInt = test.GetNumber();
  EXPECT_EQ(evnumber, retValInt);

  retValDouble = test.GetStartTime();
  EXPECT_EQ(starttime, retValDouble);

  retValDouble = test.GetEndTime();
  EXPECT_EQ(endtime, retValDouble);

  retValInt = test.GetNofData();
  EXPECT_EQ(numobjects, retValInt);

  EXPECT_EQ(match, test.GetMatch());

  if (!vertex) {
    Double_t val[6] = {0., 0., 0., 0., 0., 0.};
    compareVertexDataMembers(*(test.GetVertex()), 0., 0., 0., 0., 0, 0, val);
  }
  else {
    Double_t val[6] = {vertex->GetCovariance(0, 0), vertex->GetCovariance(0, 1), vertex->GetCovariance(0, 2),
                       vertex->GetCovariance(1, 1), vertex->GetCovariance(1, 2), vertex->GetCovariance(2, 2)};
    compareVertexDataMembers(*(test.GetVertex()), vertex->GetX(), vertex->GetY(), vertex->GetZ(), vertex->GetChi2(),
                             vertex->GetNDF(), vertex->GetNTracks(), val);
  }
}

void compareEventMap(CbmEvent& test, Int_t numobjects, Int_t numobjectstype, ECbmDataType type,
                     std::vector<UInt_t> indices)
{
  Int_t retValInt {-111};
  UInt_t retValUInt {111};

  retValInt = test.GetNofData();
  EXPECT_EQ(numobjects, retValInt);

  retValInt = test.GetNofData(type);
  EXPECT_EQ(numobjectstype, retValInt);

  if (numobjectstype > 0) {
    for (Int_t i = 0; i < numobjectstype; ++i) {
      retValUInt = test.GetIndex(type, i);
      EXPECT_EQ(indices[i], retValUInt);
    }
  }
}


TEST(_GTestCbmEvent, CheckDefaultConstructor)
{
  CbmEvent test;
  {
    SCOPED_TRACE("CheckDefaultConstructor");
  }
}

TEST(_GTestCbmEvent, CheckStandardConstructor)
{
  CbmEvent test {-111};
  {
    SCOPED_TRACE("CheckStandardConstructor");
    compareEventDataMembers(test, -111, 0., 0., 0, nullptr, nullptr);
  }
}


TEST(_GTestCbmEvent, CheckSettersAndGetters)
{
  CbmEvent test {-111, 1., 2.};
  {
    SCOPED_TRACE("CheckSettersAndGetters: Initial Test");
    compareEventDataMembers(test, -111, 1., 2., 0, nullptr, nullptr);
  }

  test.SetStartTime(-23.);
  {
    SCOPED_TRACE("CheckSettersAndGetters: SetStartTime");
    compareEventDataMembers(test, -111, -23., 2., 0, nullptr, nullptr);
  }

  test.SetEndTime(-45.);
  {
    SCOPED_TRACE("CheckSettersAndGetters: SetEndTime");
    compareEventDataMembers(test, -111, -23., -45., 0, nullptr, nullptr);
  }

  CbmMatch* testMatch = new CbmMatch();
  test.SetMatch(testMatch);
  {
    SCOPED_TRACE("CheckSettersAndGetters: SetMatch");
    compareEventDataMembers(test, -111, -23., -45., 0, testMatch, nullptr);
  }
}

TEST(_GTestCbmEvent, CheckAddData)
{
  CbmEvent test {-111, 1., 2.};
  {
    SCOPED_TRACE("CheckAddData: Initial Test");
    compareEventDataMembers(test, -111, 1., 2., 0, nullptr, nullptr);
  }

  std::vector<UInt_t> mctrack;
  std::vector<UInt_t> stspoint;
  std::vector<UInt_t> stsdigi;
  std::vector<UInt_t> stscluster;
  std::vector<UInt_t> stshit;
  std::vector<UInt_t> ststrack;


  mctrack.push_back(11);
  test.AddData(ECbmDataType::kMCTrack, 11);
  {
    SCOPED_TRACE("CheckAddData: Add first MCTrack");
    compareEventDataMembers(test, -111, 1., 2., 1, nullptr, nullptr);
    compareEventMap(test, 1, 1, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 1, -1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 1, -1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 1, -1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 1, -1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 1, -1, ECbmDataType::kStsTrack, ststrack);
  }

  mctrack.push_back(23);
  test.AddData(ECbmDataType::kMCTrack, 23);
  {
    SCOPED_TRACE("CheckAddData: Add second MCTrack");
    compareEventDataMembers(test, -111, 1., 2., 2, nullptr, nullptr);
    compareEventMap(test, 2, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 2, -1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 2, -1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 2, -1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 2, -1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 2, -1, ECbmDataType::kStsTrack, ststrack);
  }

  UInt_t retValUInt = test.GetIndex(ECbmDataType::kMCTrack, 25);
  EXPECT_EQ(-2, retValUInt);

  stspoint.push_back(1);
  test.AddData(ECbmDataType::kStsPoint, 1);
  {
    SCOPED_TRACE("CheckAddData: Add StsPoint");
    compareEventDataMembers(test, -111, 1., 2., 3, nullptr, nullptr);
    compareEventMap(test, 3, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 3, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 3, -1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 3, -1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 3, -1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 3, -1, ECbmDataType::kStsTrack, ststrack);
  }

  stsdigi.push_back(2);
  test.AddData(ECbmDataType::kStsDigi, 2);
  {
    SCOPED_TRACE("CheckAddData: Add StsDigi");
    compareEventDataMembers(test, -111, 1., 2., 4, nullptr, nullptr);
    compareEventMap(test, 4, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 4, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 4, 1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 4, -1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 4, -1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 4, -1, ECbmDataType::kStsTrack, ststrack);
  }

  stscluster.push_back(3);
  test.AddData(ECbmDataType::kStsCluster, 3);
  {
    SCOPED_TRACE("CheckAddData: Add StsCluster");
    compareEventDataMembers(test, -111, 1., 2., 5, nullptr, nullptr);
    compareEventMap(test, 5, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 5, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 5, 1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 5, 1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 5, -1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 5, -1, ECbmDataType::kStsTrack, ststrack);
  }

  stshit.push_back(4);
  test.AddData(ECbmDataType::kStsHit, 4);
  {
    SCOPED_TRACE("CheckAddData: Add StsHit");
    compareEventDataMembers(test, -111, 1., 2., 6, nullptr, nullptr);
    compareEventMap(test, 6, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 6, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 6, 1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 6, 1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 6, 1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 6, -1, ECbmDataType::kStsTrack, ststrack);
  }

  ststrack.push_back(5);
  test.AddData(ECbmDataType::kStsTrack, 5);
  {
    SCOPED_TRACE("CheckAddData: Add StsTrack");
    compareEventDataMembers(test, -111, 1., 2., 7, nullptr, nullptr);
    compareEventMap(test, 7, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 7, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 7, 1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 7, 1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 7, 1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 7, 1, ECbmDataType::kStsTrack, ststrack);
  }

  ststrack.push_back(6);
  test.AddStsTrack(6);
  {
    SCOPED_TRACE("CheckAddData: Add StsTrack via AddStsTrack");
    compareEventDataMembers(test, -111, 1., 2., 8, nullptr, nullptr);
    compareEventMap(test, 8, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 8, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 8, 1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 8, 1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 8, 1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 8, 2, ECbmDataType::kStsTrack, ststrack);
  }

  retValUInt = test.GetStsTrackIndex(0);
  EXPECT_EQ(5, retValUInt);

  retValUInt = test.GetStsTrackIndex(1);
  EXPECT_EQ(6, retValUInt);

  retValUInt = test.GetStsTrackIndex(2);
  EXPECT_EQ(-2, retValUInt);

  retValUInt = test.GetNofStsTracks();
  EXPECT_EQ(2, retValUInt);

  ststrack.clear();
  ststrack.push_back(11);
  ststrack.push_back(12);
  ststrack.push_back(13);

  test.SetStsTracks(ststrack);
  retValUInt = test.GetNofStsTracks();
  EXPECT_EQ(3, retValUInt);
  {
    SCOPED_TRACE("CheckAddData: Add StsTracks via StsStsTrack");
    compareEventDataMembers(test, -111, 1., 2., 9, nullptr, nullptr);
    compareEventMap(test, 9, 2, ECbmDataType::kMCTrack, mctrack);
    compareEventMap(test, 9, 1, ECbmDataType::kStsPoint, stspoint);
    compareEventMap(test, 9, 1, ECbmDataType::kStsDigi, stsdigi);
    compareEventMap(test, 9, 1, ECbmDataType::kStsCluster, stscluster);
    compareEventMap(test, 9, 1, ECbmDataType::kStsHit, stshit);
    compareEventMap(test, 9, 3, ECbmDataType::kStsTrack, ststrack);
  }

  EXPECT_STREQ("Event -111 at t = 1 ns. Registered data types: 6, data "
               "objects: 9, without matches\n          -- Data type 0, number "
               "of data 2\n          -- Data type 200, number of data 1\n      "
               "    -- Data type 201, number of data 1\n          -- Data type "
               "202, number of data 1\n          -- Data type 203, number of "
               "data 1\n          -- Data type 204, number of data 3\n",
               test.ToString().c_str());
}

TEST(_GTestCbmEvent, CheckSetVertex)
{

  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;  // 0 1 2
  Cov(0, 1) = 1.;  // 1 3 4
  Cov(0, 2) = 2.;  // 2 4 5
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;
  CbmVertex testVertex {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, Cov};

  CbmEvent test;
  test.SetVertex(1., 2., 3., 4., 5, 6, Cov);
  {
    SCOPED_TRACE("CheckSetVertex");
    compareEventDataMembers(test, -1, 0., 0., 0, nullptr, &testVertex);
  }
}
