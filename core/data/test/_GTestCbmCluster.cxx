#include "CbmCluster.h"

#include "CbmMatch.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#include "compareCluster.h"

#include <vector>

TEST(_GTestCbmCluster, CheckDefaultConstructor)
{
  CbmCluster test;
  std::vector<Int_t> testvector;
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    compareClusterDataMembers(test, 0, testvector, 0, nullptr);
  }
}

TEST(_GTestCbmCluster, CheckStandardConstructor)
{
  std::vector<Int_t> testvector{1,2,3,4,5};
  CbmCluster test{testvector,-11};
  {
    SCOPED_TRACE("CheckStandardConstructor");
    compareClusterDataMembers(test, 5, testvector, -11, nullptr);
  }
}

TEST(_GTestCbmCluster, CheckSetDigis)
{
  std::vector<Int_t> testvector{1,2,3,4,5};
  CbmCluster test{testvector,-11};
  {
    SCOPED_TRACE("CheckSetDigis: InitialTest");
    compareClusterDataMembers(test, 5, testvector, -11, nullptr);
  }

  std::vector<Int_t> testvector1{11,21,31,41,51,61};
  test.SetDigis(testvector1);
  {
    SCOPED_TRACE("CheckSetDigis: SetDigis");
    compareClusterDataMembers(test, 6, testvector1, -11, nullptr);
  }
}

TEST(_GTestCbmCluster, CheckAddDigi)
{
  std::vector<Int_t> testvector{1,2,3,4,5};
  std::vector<Int_t> testvector1{1,2,3,4,5,111};
  std::vector<Int_t> testvector2{1,2,3,4,5,111,222};

  CbmCluster test{testvector,-11};
  {
    SCOPED_TRACE("CheckAddDigi: InitialTest");
    compareClusterDataMembers(test, testvector.size(), testvector, -11, nullptr);
  }

  test.AddDigi(111);
  {
    SCOPED_TRACE("CheckAddDigis: AddDigi 1");
    compareClusterDataMembers(test, testvector.size()+1, testvector1, -11, nullptr);
  }

  test.AddDigi(222);
  {
    SCOPED_TRACE("CheckAddDigis: AddDigi 2");
    compareClusterDataMembers(test, testvector.size()+2, testvector2, -11, nullptr);
  }
}

TEST(_GTestCbmCluster, CheckAddDigis)
{
  std::vector<Int_t> testvector{1,2,3,4,5};
  std::vector<Int_t> testvector1{11,21,31,41,51,61};
  std::vector<Int_t> testvector2{1,2,3,4,5,11,21,31,41,51,61};

  CbmCluster test{testvector,-11};
  {
    SCOPED_TRACE("CheckAddDigis: InitialTest");
    compareClusterDataMembers(test, testvector.size(), testvector, -11, nullptr);
  }

  test.AddDigis(testvector1);
  {
    SCOPED_TRACE("CheckAddDigis: AddDigis");
    compareClusterDataMembers(test, testvector2.size(), testvector2, -11, nullptr);
  }
}

TEST(_GTestCbmCluster, CheckClearDigis)
{
  std::vector<Int_t> testvector{1,2,3,4,5};
  std::vector<Int_t> testvector1;

  CbmCluster test{testvector,-11};
  {
    SCOPED_TRACE("CheckClearDigis: InitialTest");
    compareClusterDataMembers(test, testvector.size(), testvector, -11, nullptr);
  }

  test.ClearDigis();
  {
    SCOPED_TRACE("CheckClearDigis: AddDigis");
    compareClusterDataMembers(test, 0, testvector1, -11, nullptr);
  }
}

TEST(_GTestCbmCluster, CheckSetters)
{
  std::vector<Int_t> testvector{1,2,3,4,5};

  CbmCluster test{testvector,-11};
  {
    SCOPED_TRACE("CheckSetters: InitialTest");
    compareClusterDataMembers(test, testvector.size(), testvector, -11, nullptr);
  }

  test.SetAddress(-111);
  {
    SCOPED_TRACE("CheckSetters: SetAddress");
    compareClusterDataMembers(test, testvector.size(), testvector, -111, nullptr);
  }

  CbmMatch testMatch;
  test.SetMatch(&testMatch);
  {
    SCOPED_TRACE("CheckSetters: SetAddress");
    compareClusterDataMembers(test, testvector.size(), testvector, -111, &testMatch);
  }

}

TEST(_GTestCbmCluster, CheckToString)
{
  std::vector<Int_t> testvector{1,2,3,4,5};
  CbmCluster test{testvector,-11};

  EXPECT_STREQ("CbmCluster: nofDigis=5 | 1 2 3 4 5  | address=-11\n", test.ToString().c_str());
}
