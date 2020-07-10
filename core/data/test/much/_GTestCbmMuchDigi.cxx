#include "CbmMuchDigi.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

#include "compareMuchDigi.h"

TEST(_GTestCbmMuchDigi, CheckDefaultConstructor) {
  // Create object
  CbmMuchDigi test;

  compareMuchDigiDataMembers(test, 0, ECbmModuleId::kMuch, 0, 0);

  CbmMuchDigi* test1 = new CbmMuchDigi();

  compareMuchDigiDataMembers(*test1, 0, ECbmModuleId::kMuch, 0, 0);
}


TEST(_GTestCbmMuchDigi, CheckStandardConstructor) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  CbmMuchDigi* test1 = new CbmMuchDigi(111, 23, 987654321);

  compareMuchDigiDataMembers(*test1, 111, ECbmModuleId::kMuch, 987654321, 23);
}

TEST(_GTestCbmMuchDigi, CheckCopyConstructor) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmMuchDigi test2 {test};


  compareMuchDigiDataMembers(test2, 111, ECbmModuleId::kMuch, 987654321, 23);

  // Test if the original object wasn't changed
  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);
}

TEST(_GTestCbmMuchDigi, CheckAssignmentOperator) {

  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmMuchDigi test2;
  test2 = test;

  compareMuchDigiDataMembers(test2, 111, ECbmModuleId::kMuch, 987654321, 23);

  // Test if the original object wasn't changed
  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);
}

TEST(_GTestCbmMuchDigi, CheckMoveConstructor) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  // Create object by move constructing
  CbmMuchDigi test2 {std::move(test)};

  compareMuchDigiDataMembers(test2, 111, ECbmModuleId::kMuch, 987654321, 23);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);
}

TEST(_GTestCbmMuchDigi, CheckAssignmentMoveConstructor) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  // Create object by move constructing
  CbmMuchDigi test2 {};
  test2 = std::move(test);

  compareMuchDigiDataMembers(test2, 111, ECbmModuleId::kMuch, 987654321, 23);

  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);
}


TEST(_GTestCbmMuchDigi, CheckToString) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  EXPECT_STREQ("", test.ToString().c_str());
}

TEST(_GTestCbmMuchDigi, CheckGetClassName) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  EXPECT_STREQ("CbmMuchDigi", test.GetClassName());
}

TEST(_GTestCbmMuchDigi, CheckSetTime) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  test.SetTime(12345678);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 12345678, 23);
}

TEST(_GTestCbmMuchDigi, CheckSetAddress) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  test.SetAddress(12341234);

  compareMuchDigiDataMembers(
    test, 12341234, ECbmModuleId::kMuch, 987654321, 23);
}

TEST(_GTestCbmMuchDigi, CheckSetAdc) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  test.SetAdc(12);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 12);
}

TEST(_GTestCbmMuchDigi, CheckGetChannelId) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  EXPECT_EQ(111, test.GetChannelId());
}

TEST(_GTestCbmMuchDigi, CheckGetAdcCharge) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  EXPECT_EQ(23, test.GetADCCharge());
}

TEST(_GTestCbmMuchDigi, CheckGetDTime) {
  // Create object
  CbmMuchDigi test(111, 23, 987654321);

  compareMuchDigiDataMembers(test, 111, ECbmModuleId::kMuch, 987654321, 23);

  EXPECT_EQ(0, test.GetDTime());
}

// TODO: Not tested yet is the setting of the saturation flag since there is
// no getter for this data member
// Is this data member needed at all if there is no accessor
// Also not tested is the function GetDetectorID since it needes knowledge
// of the internal address scheme
