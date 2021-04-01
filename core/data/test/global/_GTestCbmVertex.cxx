#include "CbmVertex.h"

#include <Logger.h>

#include <TMatrixTSym.h>
#include <TVector3.h>

#include <regex>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

#include "../compareVertex.h"

TEST(_GTestCbmVertex, CheckDefaultConstructor) {
  CbmVertex test;
  Double_t val[6] = {0., 0., 0., 0., 0., 0.};
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    compareVertexDataMembers(test, 0., 0., 0., 0., 0, 0, val);
  }
}

TEST(_GTestCbmVertex, CheckStandardConstructor) {
  CbmVertex test {"Vertex", "Vertex"};
  Double_t val[6] = {0., 0., 0., 0., 0., 0.};
  {
    SCOPED_TRACE("CheckStandardConstructor");
    compareVertexDataMembers(test, 0., 0., 0., 0., 0, 0, val);
  }
}

TEST(_GTestCbmVertex, CheckConstructorAllArguments) {
  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;  // 0 1 2
  Cov(0, 1) = 1.;  // 1 3 4
  Cov(0, 2) = 2.;  // 2 4 5
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;

  Double_t val[6] = {0., 1., 2., 3., 4., 5.};
  CbmVertex test {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, Cov};
  {
    SCOPED_TRACE("CheckConstructorAllArguments");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }

  // In case a matrix of wrong dimension is passed all data members are initialized, except the cov matrix
  // where all elements are set to 0.
  // Additionally an error message is printed
  TMatrixFSym CovWrong(2);
  CovWrong(0, 0) = 0.;  // 0 1
  CovWrong(0, 1) = 1.;  // 1 2
  CovWrong(1, 0) = 1.;
  CovWrong(1, 1) = 2.;

  Double_t val1[6] = {0., 0., 0., 0., 0., 0.};

  testing::internal::CaptureStdout();
  CbmVertex test1 {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, CovWrong};
  std::string output = testing::internal::GetCapturedStdout();

  std::regex f("\\[ERROR(.*)\\](.*)(Wrong dimension of passed covariance "
               "matrix\\. Clear the covariance matrix)(.*)\\n");
  bool retval = std::regex_match(output, f);
  if (!retval) {
    std::cout << "  Actual: " << output << std::endl;
    std::cout << "Expected: "
              << "\\[ERROR\\](.*)(Wrong dimension of passed covariance "
                 "matrix\\. Clear the covariance matrix)(.*)\\n"
              << std::endl;
  }
  EXPECT_TRUE(retval);

  {
    SCOPED_TRACE("CheckConstructorAllArgumentsWrongCovMatrix");
    compareVertexDataMembers(test1, 1., 2., 3., 4., 5, 6, val1);
  }
}

TEST(_GTestCbmVertex, CheckReset) {
  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;
  Cov(0, 1) = 1.;
  Cov(0, 2) = 2.;
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;

  Double_t val[6] = {0., 1., 2., 3., 4., 5.};
  CbmVertex test {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, Cov};
  {
    SCOPED_TRACE("CheckReset: Initial Test");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }

  Double_t val1[6] = {0., 0., 0., 0., 0., 0.};
  test.Reset();
  {
    SCOPED_TRACE("CheckReset: Check after reset");
    compareVertexDataMembers(test, 0., 0., 0., 0., 0, 0, val1);
  }
}

TEST(_GTestCbmVertex, CheckGetPosition) {
  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;
  Cov(0, 1) = 1.;
  Cov(0, 2) = 2.;
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;

  Double_t val[6] = {0., 1., 2., 3., 4., 5.};
  CbmVertex test {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, Cov};
  {
    SCOPED_TRACE("CheckGetPosition: Initial Test");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }

  TVector3 testVect;

  test.Position(testVect);
  {
    SCOPED_TRACE("CheckGetPosition: Check after Position");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }
  EXPECT_FLOAT_EQ(1., testVect.X());
  EXPECT_FLOAT_EQ(2., testVect.Y());
  EXPECT_FLOAT_EQ(3., testVect.Z());
}


TEST(_GTestCbmVertex, CheckGetCovMatrix) {
  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;
  Cov(0, 1) = 1.;
  Cov(0, 2) = 2.;
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;

  Double_t val[6] = {0., 1., 2., 3., 4., 5.};
  CbmVertex test {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, Cov};
  {
    SCOPED_TRACE("CheckGetCovMatrix: Initial Test");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }

  TMatrixFSym testCov(3);

  test.CovMatrix(testCov);
  {
    SCOPED_TRACE("CheckGetCovMatrix: Check after Position");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }
  for (Int_t i = 0; i < 3; ++i) {
    for (Int_t j = 0; j < 3; ++j) {
      Double_t origVal = Cov[i][j];
      Double_t testVal = testCov[i][j];
      EXPECT_FLOAT_EQ(testVal, origVal);
    }
  }
}


TEST(_GTestCbmVertex, CheckSetVertex) {

  CbmVertex test;
  Double_t val[6] = {0., 0., 0., 0., 0., 0.};
  {
    SCOPED_TRACE("CheckSetVertex: Initial Test");
    compareVertexDataMembers(test, 0., 0., 0., 0., 0, 0, val);
  }

  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;
  Cov(0, 1) = 1.;
  Cov(0, 2) = 2.;
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;

  Double_t val1[6] = {0., 1., 2., 3., 4., 5.};
  test.SetVertex(1., 2., 3., 4., 5, 6, Cov);
  {
    SCOPED_TRACE("CheckSetVertex: After call of SetVertex");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val1);
  }

  // In case a matrix of wrong dimension is passed all data members are initialized, except the cov matrix
  // where all elements are set to 0.
  // Additionally an error message is printed
  TMatrixFSym CovWrong(2);
  CovWrong(0, 0) = 0.;  // 0 1
  CovWrong(0, 1) = 1.;  // 1 2
  CovWrong(1, 0) = 1.;
  CovWrong(1, 1) = 2.;

  Double_t val2[6] = {0., 0., 0., 0., 0., 0.};

  testing::internal::CaptureStdout();
  test.SetVertex(-1., -2., -3., -4., -5, -6, CovWrong);
  std::string output = testing::internal::GetCapturedStdout();

  std::regex f("\\[ERROR(.*)\\](.*)(Wrong dimension of passed covariance "
               "matrix\\. Clear the covariance matrix)(.*)\\n");
  bool retval = std::regex_match(output, f);
  if (!retval) {
    std::cout << "  Actual: " << output << std::endl;
    std::cout << "Expected: "
              << "\\[ERROR\\](.*)(Wrong dimension of passed covariance "
                 "matrix\\. Clear the covariance matrix)(.*)\\n"
              << std::endl;
  }
  EXPECT_TRUE(retval);

  {
    SCOPED_TRACE("CheckSetVertex: Check after SetVertex with wrong cov matrix");
    compareVertexDataMembers(test, -1., -2., -3., -4., -5, -6, val2);
  }
}

TEST(_GTestCbmVertex, CheckPrint) {
  TMatrixFSym Cov(3);
  Cov(0, 0) = 0.;
  Cov(0, 1) = 1.;
  Cov(0, 2) = 2.;
  Cov(1, 0) = 1.;
  Cov(1, 1) = 3.;
  Cov(1, 2) = 4.;
  Cov(2, 0) = 2.;
  Cov(2, 1) = 4.;
  Cov(2, 2) = 5.;

  Double_t val[6] = {0., 1., 2., 3., 4., 5.};
  CbmVertex test {"Vertex", "Vertex", 1., 2., 3., 4., 5, 6, Cov};
  {
    SCOPED_TRACE("CheckSetVertex: Initial Test");
    compareVertexDataMembers(test, 1., 2., 3., 4., 5, 6, val);
  }

  EXPECT_STREQ("Vertex: position (1.0000, 2.0000, 3.0000) cm, chi2/ndf = "
               "0.8000, tracks used: 6",
               test.ToString().c_str());


  CbmVertex test1 {"Vertex", "Vertex", 1., 2., 3., 4., 0, 6, Cov};
  {
    SCOPED_TRACE("CheckSetVertex: Initial Test");
    compareVertexDataMembers(test1, 1., 2., 3., 4., 0, 6, val);
  }

  EXPECT_STREQ("Vertex: position (1.0000, 2.0000, 3.0000) cm, chi2/ndf = "
               "0.0000, tracks used: 6",
               test1.ToString().c_str());
}
