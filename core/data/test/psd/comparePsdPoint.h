/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREPSDPOINT_H
#define COMPAREPSDPOINT_H 1

#include "compareFairMCPoint.h"
#include "gtest/gtest.h"

void comparePsdPointDataMembers(CbmPsdPoint& test, int32_t trackID, int32_t detID, TVector3 pos, TVector3 mom,
                                double tof, double length, double eLoss, int32_t moduleid, int32_t eventid)
{

  FairMCPoint test1 = static_cast<FairMCPoint>(test);
  compareFairMCPointDataMembers(test1, trackID, detID, pos, mom, tof, length, eLoss, eventid);

  int32_t retValInt {-222};

  retValInt = test.GetModuleID();
  EXPECT_EQ(moduleid, retValInt);
}

#endif  // COMPAREPSDDIGI_H
