#include "CbmModuleList.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmModuleList, CheckDefaultConstructor)
{
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    CbmModuleList test;
    //    EXPECT_STREQ("magnet", CbmModuleList::GetModuleName(kMagnet));
  }
}

//TEST(_GTestCbmModuleList, CheckDefaultConstructorNew)
//{
//  {
//   SCOPED_TRACE("CheckDefaultConstructor");
//    CbmModuleList* test1 = new CbmModuleList();
//    delete test1;
////    EXPECT_STREQ("magnet", CbmModuleList::GetModuleName(kMagnet));
//  }
//}
