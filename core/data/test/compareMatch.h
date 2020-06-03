#ifndef COMPAREMATCH_H
#define COMPAREMATCH_H 1

#include "CbmMatch.h"
#include "CbmLink.h"

void compareMatchDataMembers(CbmMatch& testMatch, Int_t noflinks, Double_t weight)
{
  Int_t linkLength{-111};
  Double_t totalWeight{-111.};

  std::vector<CbmLink> link = testMatch.GetLinks();
  linkLength = link.size();
  EXPECT_EQ(noflinks, linkLength);

  linkLength = testMatch.GetNofLinks();
  EXPECT_EQ(noflinks, linkLength);

  totalWeight = testMatch.GetTotalWeight();
  EXPECT_FLOAT_EQ(weight, totalWeight);
}

#endif // COMPAREMATCH_H