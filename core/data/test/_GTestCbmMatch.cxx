#include "CbmMatch.h"
#include "CbmLink.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#include "compareMatch.h"
#include "compareLink.h"

TEST(_GTestCbmMatch, CheckDefaultConstructor)
{
  CbmMatch test;
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    compareMatchDataMembers(test, 0, 0.);
  }
}

TEST(_GTestCbmMatch, AddLink1)
{
  CbmMatch test;
  {
    SCOPED_TRACE("AddLink1: Initial Test");
    compareMatchDataMembers(test, 0, 0.);
  }

  test.AddLink(-2., -2);
  {
    SCOPED_TRACE("AddLink1: Add first link");
    compareMatchDataMembers(test, 1, -2.);
  }

  test.AddLink(-2., -2);
  {
    SCOPED_TRACE("AddLink1: Add second link");
    compareMatchDataMembers(test, 1, -4.);
  }

  test.AddLink(-8., -3);
  {
    SCOPED_TRACE("AddLink1: Add third link");
    compareMatchDataMembers(test, 2, -12.);
  }
}

TEST(_GTestCbmMatch, AddLink2)
{

  CbmMatch test;
  {
    SCOPED_TRACE("AddLink2: Initial Test");
    compareMatchDataMembers(test, 0, 0.);
  }

  test.AddLink(-2., -2, -2, -2);
  {
    SCOPED_TRACE("AddLink2: Add first link");
    compareMatchDataMembers(test, 1, -2.);
  }

  test.AddLink(-2., -2, -2, -2);
  {
    SCOPED_TRACE("AddLink2: Add second link");
    compareMatchDataMembers(test, 1, -4.);
  }

  test.AddLink(-8., -3, -2, -2);
  {
    SCOPED_TRACE("AddLink2: Add third link");
    compareMatchDataMembers(test, 2, -12.);
  }

  test.AddLink(8., -3, -3, -2);
  {
    SCOPED_TRACE("AddLink2: Add forth link");
    compareMatchDataMembers(test, 3, -4.);
  }

  test.AddLink(4., -3, -3, -3);
  {
    SCOPED_TRACE("AddLink2: Add fifth link");
    compareMatchDataMembers(test, 4, 0.);
  }

}

CbmMatch TestAddLinks3()
{
  CbmMatch test;

  CbmLink testLink1{};
  CbmLink testLink2{};
  CbmLink testLink3{-8., -3, -2, -2};
  CbmLink testLink4{8., -3, -3, -2};
  CbmLink testLink5{2., -3, -3, -3};

  {
    SCOPED_TRACE("AddLink3: Initial Test");
    compareMatchDataMembers(test, 0, 0.);
  }

  test.AddLink(testLink1);
  {
    SCOPED_TRACE("AddLink3: Add first link");
    compareMatchDataMembers(test, 1, -1.);
  }

  test.AddLink(testLink2);
  {
    SCOPED_TRACE("AddLink3: Add second link");
    compareMatchDataMembers(test, 1, -2.);
  }

  test.AddLink(testLink3);
  {
    SCOPED_TRACE("AddLink3: Add third link");
    compareMatchDataMembers(test, 2, -10.);
  }

  test.AddLink(testLink4);
  {
    SCOPED_TRACE("AddLink3: Add forth link");
    compareMatchDataMembers(test, 3, -2.);
  }

  test.AddLink(testLink5);
  {
    SCOPED_TRACE("AddLink3: Add fifth link");
    compareMatchDataMembers(test, 4, 0.);
  }

  return test;
}

TEST(_GTestCbmMatch, AddLink3)
{

  CbmMatch test = TestAddLinks3();
  CbmMatch test2;

  test2.AddLinks(test);
  {
    SCOPED_TRACE("AddLink3: Add fifth link");
    compareMatchDataMembers(test2, 4, 0.);
  }

}

TEST(_GTestCbmMatch, ClearLink)
{

  CbmMatch test = TestAddLinks3();
  {
    SCOPED_TRACE("ClearLink: Initial Test");
    compareMatchDataMembers(test, 4, 0.);
  }

  test.ClearLinks();
  {
    SCOPED_TRACE("ClearLink: Clear Link");
    compareMatchDataMembers(test, 0, 0.);
  }

}


TEST(_GTestCbmMatch, GetLink)
{

  CbmMatch test = TestAddLinks3();
  {
    SCOPED_TRACE("GetLink: Initial Test");
    compareMatchDataMembers(test, 4, 0.);
  }

  CbmLink testLink{};
  testLink = test.GetLink(0);
  {
    SCOPED_TRACE("GetLink: Get Link 0");
    compareLinkDataMembers(testLink, -1, -1, -1, -2.);
  }

  testLink = test.GetLink(1);
  {
    SCOPED_TRACE("GetLink: Get Link 1");
    compareLinkDataMembers(testLink, -2, -2, -3, -8.);
  }

  testLink = test.GetLink(2);
  {
    SCOPED_TRACE("GetLink: Get Link 2");
    compareLinkDataMembers(testLink, -2, -3, -3, 8.);
  }

  testLink = test.GetLink(3);
  {
    SCOPED_TRACE("GetLink: Get Link 3");
    compareLinkDataMembers(testLink, -3, -3, -3, 2.);
  }
}

TEST(_GTestCbmMatch, GetLinks)
{

  CbmMatch test = TestAddLinks3();
  {
    SCOPED_TRACE("GetLinks: Initial Test");
    compareMatchDataMembers(test, 4, 0.);
  }

  std::vector<CbmLink> link = test.GetLinks();

  CbmLink testLink = link[0];
  {
    SCOPED_TRACE("GetLinks: Get Link 0");
    compareLinkDataMembers(testLink, -1, -1, -1, -2.);
  }

  testLink = link[1];
  {
    SCOPED_TRACE("GetLinks: Get Link 1");
    compareLinkDataMembers(testLink, -2, -2, -3, -8.);
  }

  testLink = link[2];
  {
    SCOPED_TRACE("GetLinks: Get Link 2");
    compareLinkDataMembers(testLink, -2, -3, -3, 8.);
  }

  testLink = link[3];
  {
    SCOPED_TRACE("GetLinks: Get Link 3");
    compareLinkDataMembers(testLink, -3, -3, -3, 2.);
  }
}

TEST(_GTestCbmMatch, GetMatchedLink)
{

  CbmMatch test = TestAddLinks3();
  {
    SCOPED_TRACE("GetMatchedLink: Initial Test");
    compareMatchDataMembers(test, 4, 0.);
  }

  // Get the link with the highest weight
  // which is in our test setup Link3
  CbmLink testLink = test.GetMatchedLink();
  {
    SCOPED_TRACE("GetMatchedLink: Test Links");
    compareLinkDataMembers(testLink, -2, -3, -3, 8);
  }
}


TEST(_GTestCbmMatch, CheckToString)
{

  CbmMatch test;

  EXPECT_STREQ("CbmMatch: nofLinks=0\n totalWeight=0, matchedIndex=-1\n", test.ToString().c_str());

  CbmLink testLink{-8., -3, -2, -2};
  test.AddLink(testLink);

  EXPECT_STREQ("CbmMatch: nofLinks=1\nCbmLink: weight=-8 index=-3 entry=-2 file=-2\n totalWeight=-8, matchedIndex=0\n", test.ToString().c_str());
}
