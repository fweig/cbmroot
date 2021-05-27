/** @file compareFairMCPoint.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef COMPAREFAIRMCPOINT_H
#define COMPAREFAIRMCPOINT_H 1

#include <FairMCPoint.h>

#include <TVector3.h>

#include "gtest/gtest.h"

void compareFairMCPointDataMembers(FairMCPoint& test, Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom,
                                   Double_t tof, Double_t length, Double_t eloss, Int_t eventid)
{
  Int_t retValInt {-222};
  Double_t retValDouble {-222.};

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
