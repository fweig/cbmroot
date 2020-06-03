#ifndef COMPAREHIT_H
#define COMPAREHIT_H 1

#include "CbmHit.h"
#include "CbmMatch.h"

#include "compareMatch.h"

void compareHitDataMembers(CbmHit& test, HitType type, Double_t z, Double_t dz,
    Int_t refid, Int_t address, CbmMatch* match, Double_t time, Double_t errortime)
{
  Int_t retValInt{-111};
  Float_t retValFloat{-111.};

  EXPECT_EQ(type, test.GetType());

  retValFloat = test.GetZ();
  EXPECT_FLOAT_EQ(z, retValFloat);

  retValFloat = test.GetDz();
  EXPECT_FLOAT_EQ(dz, retValFloat);

  retValInt = test.GetRefId();
  EXPECT_EQ(refid, retValInt);

  retValInt = test.GetAddress();
  EXPECT_EQ(address, retValInt);

  if (match !=nullptr) {
    compareMatchDataMembers(*test.GetMatch(), match->GetNofLinks(), match->GetTotalWeight());
  } else {
    EXPECT_EQ(match, test.GetMatch());
  }

  retValFloat =  test.GetTime();
  EXPECT_FLOAT_EQ(time, retValFloat );

  retValFloat = test.GetTimeError();
  EXPECT_FLOAT_EQ(errortime, retValFloat);
}

#endif // COMPAREHIT_H