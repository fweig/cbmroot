/** @file _GTestCbmModuleList.cxx
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

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
