#ifndef COMPARESTRIPHIT_H
#define COMPARESTRIPHIT_H 1

#include "CbmStripHit.h"
#include "CbmMatch.h"

#include "compareHit.h"

void compareStripHitDataMembers(CbmStripHit& test, HitType type, Double_t z, Double_t dz,
    Int_t refid, Int_t address, CbmMatch* match, Double_t time, Double_t errortime,
    Double_t u, Double_t du, Double_t phi, Double_t dphi)
{

  compareHitDataMembers(test, type, z, dz, refid, address, match, 
                        time, errortime);

  Float_t retValFloat{-111.};

  retValFloat = test.GetU();
  EXPECT_FLOAT_EQ(u, retValFloat);

  retValFloat = test.GetDu();
  EXPECT_FLOAT_EQ(du, retValFloat);

  retValFloat = test.GetPhi();
  EXPECT_FLOAT_EQ(phi, retValFloat);

  retValFloat = test.GetDphi();
  EXPECT_FLOAT_EQ(dphi, retValFloat);
}

#endif // COMPARESTRIPHIT_H