/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREFAIRMCPOINT_H
#define COMPAREFAIRMCPOINT_H 1

#include <FairMCPoint.h>

#include <TVector3.h>

#include "gtest/gtest.h"

void compareFairMCPointDataMembers(FairMCPoint& test, int32_t trackID, int32_t detID, TVector3 pos, TVector3 mom,
                                   double tof, double length, double eloss, int32_t eventid)
{
  int32_t retValInt {-222};
  double retValDouble {-222.};

  retValInt = test.GetDetectorID();
  EXPECT_EQ(detID, retValInt);

  retValInt = test.GetEventID();
  EXPECT_EQ(eventid, retValInt);

  retValInt = test.GetTrackID();
  EXPECT_EQ(trackID, retValInt);

  retValDouble = test.GetPx();
  EXPECT_FLOAT_EQ(mom.Px(), retValDouble);

  retValDouble = test.GetPy();
  EXPECT_FLOAT_EQ(mom.Py(), retValDouble);

  retValDouble = test.GetPz();
  EXPECT_FLOAT_EQ(mom.Pz(), retValDouble);

  retValDouble = test.GetX();
  EXPECT_FLOAT_EQ(pos.X(), retValDouble);

  retValDouble = test.GetY();
  EXPECT_FLOAT_EQ(pos.Y(), retValDouble);

  retValDouble = test.GetZ();
  EXPECT_FLOAT_EQ(pos.Z(), retValDouble);

  retValDouble = test.GetTime();
  EXPECT_FLOAT_EQ(tof, retValDouble);

  retValDouble = test.GetLength();
  EXPECT_FLOAT_EQ(length, retValDouble);

  retValDouble = test.GetEnergyLoss();
  EXPECT_FLOAT_EQ(eloss, retValDouble);
}

#endif  // COMPAREPSDDIGI_H
