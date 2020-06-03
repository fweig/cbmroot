#include "CbmPsdHit.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#include "comparePsdHit.h"

TEST(_GTestCbmPsdHit , CheckDefaultConstructor)
{
  // Create object
  CbmPsdHit test;

  comparePsdHitDataMembers(test, -1, -1.);

  CbmPsdHit* test1 = new CbmPsdHit();

  comparePsdHitDataMembers(*test1, -1, -1.);

}

TEST(_GTestCbmPsdHit , CheckStandardConstructor)
{
  // Create object
  CbmPsdHit test(5, 6.7);

  comparePsdHitDataMembers(test, 5, 6.7);

  CbmPsdHit* test1 = new CbmPsdHit(2, 8.9);

  comparePsdHitDataMembers(*test1, 2, 8.9);
}

TEST(_GTestCbmPsdHit , CheckCopyConstructor)
{
  // Create object
  CbmPsdHit test(5, 6.7);

  comparePsdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmPsdHit test2{test};

  comparePsdHitDataMembers(test2, 5, 6.7);

  // Test if the original object wasn't changed
  comparePsdHitDataMembers(test, 5, 6.7);
}

TEST(_GTestCbmPsdHit , CheckAssignmentOperator)
{
  // Create object
  CbmPsdHit test(5, 6.7);

  comparePsdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmPsdHit test2{};
  test2 = test;

  comparePsdHitDataMembers(test2, 5, 6.7);

  // Test if the original object wasn't changed
  comparePsdHitDataMembers(test, 5, 6.7);
}

TEST(_GTestCbmPsdHit , CheckMoveConstructor)
{
  // Create object
  CbmPsdHit test(5, 6.7);

  comparePsdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmPsdHit test2{std::move(test)};

  comparePsdHitDataMembers(test2, 5, 6.7);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  comparePsdHitDataMembers(test, 5, 6.7);
}

TEST(_GTestCbmPsdHit , CheckAssignmentMoveConstructor)
{
  // Create object
  CbmPsdHit test(5, 6.7);

  comparePsdHitDataMembers(test, 5, 6.7);

  // Create object by copy constructing
  // test should be equal to test2 and 
  // test should not be changed
  CbmPsdHit test2{};
  test2 = std::move(test);

  comparePsdHitDataMembers(test2, 5, 6.7);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  comparePsdHitDataMembers(test, 5, 6.7);
}


TEST(_GTestCbmPsdHit , CheckPrint)
{
  // Create object
  CbmPsdHit test(5, 6.7);

  comparePsdHitDataMembers(test, 5, 6.7);

  testing::internal::CaptureStdout();
  test.Print("");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_STREQ("[INFO] module : 5 ELoss 6.7\n", output.c_str());
}
