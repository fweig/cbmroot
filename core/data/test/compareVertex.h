/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "gtest/gtest.h"

void compareVertexDataMembers(CbmVertex& test, double x, double y, double z, double chi2, int32_t ndf, int32_t ntracks,
                              double* cov)
{
  int32_t retValInt {-111};
  double retValDouble {-111.};

  retValDouble = test.GetX();
  EXPECT_FLOAT_EQ(x, retValDouble);

  retValDouble = test.GetY();
  EXPECT_FLOAT_EQ(y, retValDouble);

  retValDouble = test.GetZ();
  EXPECT_FLOAT_EQ(z, retValDouble);

  retValDouble = test.GetChi2();
  EXPECT_FLOAT_EQ(chi2, retValDouble);

  retValInt = test.GetNDF();
  EXPECT_EQ(ndf, retValInt);

  retValInt = test.GetNTracks();
  EXPECT_EQ(ntracks, retValInt);

  if (cov) {
    retValDouble = test.GetCovariance(0, 0);
    EXPECT_FLOAT_EQ(cov[0], retValDouble);

    retValDouble = test.GetCovariance(0, 1);
    EXPECT_FLOAT_EQ(cov[1], retValDouble);

    retValDouble = test.GetCovariance(0, 2);
    EXPECT_FLOAT_EQ(cov[2], retValDouble);

    retValDouble = test.GetCovariance(1, 1);
    EXPECT_FLOAT_EQ(cov[3], retValDouble);

    retValDouble = test.GetCovariance(1, 2);
    EXPECT_FLOAT_EQ(cov[4], retValDouble);

    retValDouble = test.GetCovariance(2, 2);
    EXPECT_FLOAT_EQ(cov[5], retValDouble);
  }
}
