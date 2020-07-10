#include "CbmMuchBeamTimeDigi.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

#include "compareMuchBeamTimeDigi.h"

TEST(_GTestCbmMuchBeamTimeDigi, CheckDefaultConstructor) {
  // Create object
  CbmMuchBeamTimeDigi test;

  compareMuchBeamTimeDigiDataMembers(
    test, 0, ECbmModuleId::kMuch, 0, 0, -1, -1, -1, -1, -1, -1);

  CbmMuchBeamTimeDigi* test1 = new CbmMuchBeamTimeDigi();

  compareMuchBeamTimeDigiDataMembers(
    *test1, 0, ECbmModuleId::kMuch, 0, 0, -1, -1, -1, -1, -1, -1);
}


TEST(_GTestCbmMuchBeamTimeDigi, CheckStandardConstructor) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  CbmMuchBeamTimeDigi* test1 = new CbmMuchBeamTimeDigi(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    *test1, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
}

TEST(_GTestCbmMuchBeamTimeDigi, CheckCopyConstructor) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  // Create object by copy constructing
  // test should be equal to test2 and
  // test should not be changed
  CbmMuchBeamTimeDigi test2 {test};


  compareMuchBeamTimeDigiDataMembers(
    test2, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  // Test if the original object wasn't changede
  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
}

TEST(_GTestCbmMuchBeamTimeDigi, CheckAssignmentOperator) {

  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  // Create object by copy assignment
  // test should be equal to test2 and
  // test should not be changed
  CbmMuchBeamTimeDigi test2;
  test2 = test;

  compareMuchBeamTimeDigiDataMembers(
    test2, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);


  // Test if the original object wasn't changed
  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
}

TEST(_GTestCbmMuchBeamTimeDigi, CheckMoveConstructor) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  // Create object by move constructing
  CbmMuchBeamTimeDigi test2 {std::move(test)};

  compareMuchBeamTimeDigiDataMembers(
    test2, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
}

TEST(_GTestCbmMuchBeamTimeDigi, CheckAssignmentMoveConstructor) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
  // Create object by move constructing
  CbmMuchBeamTimeDigi test2 {};
  test2 = std::move(test);

  compareMuchBeamTimeDigiDataMembers(
    test2, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
  // For objects with simple types move fall back to copy so
  // the original object is kept unchanged
  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);
}


TEST(_GTestCbmMuchBeamTimeDigi, CheckToString) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  EXPECT_STREQ("", test.ToString().c_str());
}

TEST(_GTestCbmMuchBeamTimeDigi, CheckGetClassName) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  EXPECT_STREQ("CbmMuchBeamTimeDigi", test.GetClassName());
}


TEST(_GTestCbmMuchDigi, CheckSetPadX) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  test.SetPadX(12);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, 12, -1, -1, -1, -1, -1);
}


TEST(_GTestCbmMuchDigi, CheckSetPadY) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  test.SetPadY(13);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, 13, -1, -1, -1, -1);
}


TEST(_GTestCbmMuchDigi, CheckSetRocId) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  test.SetRocId(14);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, 14, -1, -1, -1);
}

TEST(_GTestCbmMuchDigi, CheckSetNxId) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  test.SetNxId(15);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, 15, -1, -1);
}

TEST(_GTestCbmMuchDigi, CheckSetNxCh) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  test.SetNxCh(16);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, 16, -1);
}

TEST(_GTestCbmMuchDigi, CheckSetElink) {
  // Create object
  CbmMuchBeamTimeDigi test(111, 23, 987654321);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, -1);

  test.SetElink(17);

  compareMuchBeamTimeDigiDataMembers(
    test, 111, ECbmModuleId::kMuch, 987654321, 23, -1, -1, -1, -1, -1, 17);
}
