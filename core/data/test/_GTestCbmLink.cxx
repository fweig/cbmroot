#include "CbmLink.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

//define functions which does the comparision 
#include "compareLink.h"

TEST(_GTestCbmLink , CheckDefaultConstructor)
{
  CbmLink test;
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    compareLinkDataMembers(test, -1, -1, -1, -1.);
  }
}

TEST(_GTestCbmLink , CheckStandardConstructor1)
{
  CbmLink test{-2., -2};
  {
    SCOPED_TRACE("CheckStandardConstructor1");
    compareLinkDataMembers(test, -1, -1, -2, -2.);
  }
}

TEST(_GTestCbmLink , CheckStandardConstructor2)
{
  CbmLink test{-2., -2, -2, -2};
  {
    SCOPED_TRACE("CheckStandardConstructor2");
    compareLinkDataMembers(test, -2, -2, -2, -2.);
  }
}

TEST(_GTestCbmLink , TestSettersAndGetters)
{
  CbmLink test{-2., -2, -2, -2};
  {
    SCOPED_TRACE("TestSettersAndGetters: Initial test");
    compareLinkDataMembers(test, -2, -2, -2, -2.);
  }

  test.SetFile(-3);
  {
    SCOPED_TRACE("TestSettersAndGetters: Test SetFile");
    compareLinkDataMembers(test, -3, -2, -2, -2.);
  }

  test.SetEntry(-3);
  {
    SCOPED_TRACE("TestSettersAndGetters: Test SetEntry");
    compareLinkDataMembers(test, -3, -3, -2, -2.);
  }

  test.SetIndex(-3);
  {
    SCOPED_TRACE("TestSettersAndGetters: Test SetIndex");
    compareLinkDataMembers(test, -3, -3, -3, -2.);
  }

  test.SetWeight(-3.);
  {
    SCOPED_TRACE("TestSettersAndGetters: Test SetWeight");
    compareLinkDataMembers(test, -3, -3, -3, -3.);
  }

  test.AddWeight(-3);
  {
    SCOPED_TRACE("TestSettersAndGetters: Test AddWeight");
    compareLinkDataMembers(test, -3, -3, -3, -6.);
  }
}

TEST(_GTestCbmLink , ToString)
{
  CbmLink test{-2., -2, -2, -2};
  EXPECT_STREQ("CbmLink: weight=-2 index=-2 entry=-2 file=-2\n", test.ToString().c_str());
}


TEST(_GTestCbmLink , CompareEqual)
{
  CbmLink test{-2., -2, -2, -2};
  CbmLink test1{-2., -2, -2, -2};
  CbmLink test2{-1., -2, -2, -2};
  CbmLink test3{-2., -1, -2, -2};
  CbmLink test4{-2., -2, -1, -2};
  CbmLink test5{-2., -2, -2, -1};

  EXPECT_TRUE(test==test1);
  EXPECT_TRUE(test==test2); //equals only check file, entry, and index ids but not the weight
  EXPECT_FALSE(test==test3);
  EXPECT_FALSE(test==test4);
  EXPECT_FALSE(test==test5);
}

TEST(_GTestCbmLink , CompareGreater)
{
  CbmLink test{2., 2, 2, 2};
  CbmLink test1{2., 2, 2, 2};
  CbmLink test2{2., 1, 2, 2};
  CbmLink test3{2., 2, 1, 2};
  CbmLink test4{2., 2, 2, 1};

  EXPECT_FALSE(test>test1);
  EXPECT_TRUE(test>test2);
  EXPECT_FALSE(test2>test);
  EXPECT_TRUE(test>test3);
  EXPECT_FALSE(test3>test);
  EXPECT_TRUE(test>test4);
  EXPECT_FALSE(test4>test);
}

TEST(_GTestCbmLink , CompareSmaller)
{
  CbmLink test{2., 2, 2, 2};
  CbmLink test1{2., 2, 2, 2};
  CbmLink test2{2., 1, 2, 2};
  CbmLink test3{2., 2, 1, 2};
  CbmLink test4{2., 2, 2, 1};

  EXPECT_FALSE(test<test1);
  EXPECT_FALSE(test<test2);
  EXPECT_TRUE(test2<test);
  EXPECT_FALSE(test<test3);
  EXPECT_TRUE(test3<test);
  EXPECT_FALSE(test<test4);
  EXPECT_TRUE(test4<test);
}
