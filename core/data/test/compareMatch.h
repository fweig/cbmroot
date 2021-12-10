/* Copyright (C) 2017-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREMATCH_H
#define COMPAREMATCH_H 1

#include "CbmLink.h"
#include "CbmMatch.h"

#include "gtest/gtest.h"

void compareMatchDataMembers(CbmMatch& testMatch, int32_t noflinks, double weight)
{
  int32_t linkLength {-111};
  double totalWeight {-111.};

  std::vector<CbmLink> link = testMatch.GetLinks();
  linkLength                = link.size();
  EXPECT_EQ(noflinks, linkLength);

  linkLength = testMatch.GetNofLinks();
  EXPECT_EQ(noflinks, linkLength);

  totalWeight = testMatch.GetTotalWeight();
  EXPECT_FLOAT_EQ(weight, totalWeight);
}

#endif  // COMPAREMATCH_H
