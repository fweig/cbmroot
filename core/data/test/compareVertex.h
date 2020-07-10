void compareVertexDataMembers(CbmVertex& test,
                              Double_t x,
                              Double_t y,
                              Double_t z,
                              Double_t chi2,
                              Int_t ndf,
                              Int_t ntracks,
                              Double_t* cov) {
  Int_t retValInt {-111};
  Double_t retValDouble {-111.};

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
