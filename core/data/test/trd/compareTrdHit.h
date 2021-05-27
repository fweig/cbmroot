#ifndef COMPARETRDHIT_H
#define COMPARETRDHIT_H 1

#include "comparePixelHit.h"
#include "gtest/gtest.h"

void compareTrdHitDataMembers(CbmTrdHit& test, HitType type, Double_t z, Double_t dz, Int_t refid, Int_t address,
                              CbmMatch* match, Double_t time, Double_t errortime, Double_t x, Double_t dx, Double_t y,
                              Double_t dy, Double_t dxy, UChar_t define, Double_t eloss)
{

  CbmPixelHit phit = static_cast<CbmPixelHit>(test);
  comparePixelHitDataMembers(phit, type, z, dz, refid, address, match, time, errortime, x, dx, y, dy, dxy);

  Int_t retValInt {-222};
  Double_t retValDouble {-222.};

  retValDouble = test.GetELoss();
  EXPECT_FLOAT_EQ(eloss, retValDouble);

  // Extract the first 4 bits from the integer
  // and compare it to the functions
  retValInt = test.GetClassType();
  EXPECT_EQ((define & 1) >> 0, retValInt);

  retValInt = test.GetMaxType();
  EXPECT_EQ((define & 2) >> 1, retValInt);

  retValInt = test.IsRowCross();
  EXPECT_EQ((define & 4) >> 2, retValInt);

  retValInt = test.HasOverFlow();
  EXPECT_EQ((define & 8) >> 3, retValInt);
}

#endif  // COMPARETRDHIT_H
