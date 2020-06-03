#ifndef COMPARELINK_H
#define COMPARELINK_H 1

#include "CbmLink.h"

void compareLinkDataMembers(CbmLink& test, Int_t file, Int_t entry, Int_t index,
                            Double_t weight)
{
  Int_t intRetVal{-111};
  Float_t floatRetVal{-111};

  intRetVal = test.GetFile();
  EXPECT_EQ(file, intRetVal);

  intRetVal = test.GetEntry();
  EXPECT_EQ(entry, intRetVal);

  intRetVal = test.GetIndex();
  EXPECT_EQ(index,intRetVal);

  floatRetVal = test.GetWeight();
  EXPECT_FLOAT_EQ(weight, floatRetVal);
}

#endif //COMPARELINK_H