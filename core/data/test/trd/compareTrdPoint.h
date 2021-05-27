/** @file compareTrdPoint.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#ifndef COMPARETRDPOINT_H
#define COMPARETRDPOINT_H 1

#include "compareFairMCPoint.h"
#include "gtest/gtest.h"

void compareTrdPointDataMembers(CbmTrdPoint& test, Int_t trackID, Int_t detID, TVector3 posin, TVector3 momin,
                                TVector3 posout, TVector3 momout, Double_t tof, Double_t length, Double_t eLoss,
                                Int_t eventid)
{

  FairMCPoint test1 = static_cast<FairMCPoint>(test);
  compareFairMCPointDataMembers(test1, trackID, detID, posin, momin, tof, length, eLoss, eventid);

  Int_t retValInt {-222};
  Double_t retValDouble {-222.};

  retValInt = test.GetModuleAddress();
  EXPECT_EQ(detID, retValInt);

  retValDouble = test.GetXIn();
  EXPECT_EQ(posin.X(), retValDouble);

  retValDouble = test.GetXOut();
  EXPECT_EQ(posout.X(), retValDouble);

  retValDouble = test.GetPxIn();
  EXPECT_EQ(momin.Px(), retValDouble);

  retValDouble = test.GetPxOut();
  EXPECT_EQ(momout.Px(), retValDouble);

  retValDouble = test.GetYIn();
  EXPECT_EQ(posin.Y(), retValDouble);

  retValDouble = test.GetYOut();
  EXPECT_EQ(posout.Y(), retValDouble);

  retValDouble = test.GetPyIn();
  EXPECT_EQ(momin.Py(), retValDouble);

  retValDouble = test.GetPyOut();
  EXPECT_EQ(momout.Py(), retValDouble);

  retValDouble = test.GetZIn();
  EXPECT_EQ(posin.Z(), retValDouble);

  retValDouble = test.GetZOut();
  EXPECT_EQ(posout.Z(), retValDouble);

  retValDouble = test.GetPzIn();
  EXPECT_EQ(momin.Pz(), retValDouble);

  retValDouble = test.GetPzOut();
  EXPECT_EQ(momout.Pz(), retValDouble);
}

#endif  // COMPARETRDDIGI_H
