#include "CbmTrdDigi.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#include "compareTrdDigi.h"

TEST(_GTestCbmTrdDigi , CheckDefaultConstructor)
{
  // Create object
  CbmTrdDigi test;

  compareTrdDigiDataMembers(test, ToIntegralType(ECbmModuleId::kTrd), ECbmModuleId::kTrd, 0, 0);

  CbmTrdDigi* test1 = new CbmTrdDigi();

  compareTrdDigiDataMembers(*test1, ToIntegralType(ECbmModuleId::kTrd), ECbmModuleId::kTrd, 0, 0);

}

TEST(_GTestCbmTrdDigi, CheckGetClassName)
{
  // Create object
  CbmTrdDigi test;

  compareTrdDigiDataMembers(test, ToIntegralType(ECbmModuleId::kTrd), ECbmModuleId::kTrd, 0, 0);

  EXPECT_STREQ("CbmTrdDigi", test.GetClassName());
}

