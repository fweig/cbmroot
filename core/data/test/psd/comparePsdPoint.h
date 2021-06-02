/** @file comparePsdPoint.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef COMPAREPSDPOINT_H
#define COMPAREPSDPOINT_H 1

#include "compareFairMCPoint.h"
#include "gtest/gtest.h"

void comparePsdPointDataMembers(CbmPsdPoint& test, Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t tof,
                                Double_t length, Double_t eLoss, Int_t moduleid, Int_t eventid)
{

  FairMCPoint test1 = static_cast<FairMCPoint>(test);
  compareFairMCPointDataMembers(test1, trackID, detID, pos, mom, tof, length, eLoss, eventid);

  Int_t retValInt {-222};

  retValInt = test.GetModuleID();
  EXPECT_EQ(moduleid, retValInt);
}

#endif  // COMPAREPSDDIGI_H
