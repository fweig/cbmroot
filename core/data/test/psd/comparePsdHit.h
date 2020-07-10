#ifndef COMPAREPSDHIT_H
#define COMPAREPSDHIT_H 1

void comparePsdHitDataMembers(CbmPsdHit& test, Int_t moduleid, Double_t edep) {
  Int_t retValInt {-222};
  Double_t retValDouble {-222.};

  retValInt = test.GetModuleID();
  EXPECT_EQ(moduleid, retValInt);

  retValDouble = test.GetEdep();
  EXPECT_FLOAT_EQ(edep, retValDouble);
}

#endif  // COMPAREPSDHIT_H
