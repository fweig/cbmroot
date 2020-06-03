#ifndef COMPARECLUSTER_H
#define COMPARECLUSTER_H 1

#include "CbmCluster.h"
#include "CbmMatch.h"

#include "compareMatch.h"

void compareClusterDataMembers(CbmCluster& test, Int_t vectorsize, std::vector<Int_t> indices,
    Int_t address, CbmMatch* match)
{
  Int_t retValInt{-111};

  retValInt = test.GetNofDigis();
  EXPECT_EQ(vectorsize, retValInt);

  if (vectorsize > 0) {
    for (Int_t counter=0; counter < vectorsize; ++counter) {
      retValInt=test.GetDigi(counter);
      EXPECT_EQ(indices[counter], retValInt);
    }
    std::vector<Int_t> compare = test.GetDigis();
    EXPECT_TRUE( std::equal(indices.begin(), indices.end(), compare.begin()) );
  }

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  if (match !=nullptr) {
    compareMatchDataMembers(*test.GetMatch(), match->GetNofLinks(), match->GetTotalWeight());
  } else {
    EXPECT_EQ(match, test.GetMatch());
  }
}

#endif // COMPARECLUSTER_H