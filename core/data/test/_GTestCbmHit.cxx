#include "CbmHit.h"

#include "CbmMatch.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#include "compareMatch.h"
#include "compareHit.h"

// Since some functions in CbmHit are protected we have
// to create a derived class without any data members
// which simply forwards the function
// calls to the base class
class CbmTestHit : public CbmHit
{
  public:
    CbmTestHit() : CbmHit() {;}

    /** Copy constructor  **/
    CbmTestHit(const CbmTestHit& hit) : CbmHit(hit) {;}

    /** Destructor  **/
    virtual ~CbmTestHit() {;}

    /** Assignment operator  **/
    CbmTestHit& operator=(const CbmTestHit& other)
    {
      if (this != &other) {
        CbmHit::operator=(other);
      }
      return *this;
    }
};

TEST(_GTestCbmHit, CheckDefaultConstructor)
{
  CbmHit test;
  compareHitDataMembers(test, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);
}

TEST(_GTestCbmHit, CheckStandardConstructor)
{
  CbmHit test{kHIT, 0., 0., -1, -1};
  compareHitDataMembers(test, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);
}

TEST(_GTestCbmHit, CheckStandardConstructorWithTime)
{
  CbmHit test{kHIT, 0., 0., -1, -1, -2.,-2.};
  compareHitDataMembers(test, kHIT, 0., 0., -1, -1, nullptr, -2., -2.);
}

TEST(_GTestCbmHit, CheckCopyConstructor)
{
  CbmTestHit test;
  {
    SCOPED_TRACE("CheckCopyConstructor: Initial Test");
    compareHitDataMembers(test, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);
  }

  CbmMatch* testMatch = new CbmMatch();

  compareMatchDataMembers(*testMatch, 0, 0.);

  test.SetMatch(testMatch);
  {
    SCOPED_TRACE("CheckCopyConstructor: Test with CbmMatch");
    compareHitDataMembers(test, kHIT, 0., 0., -1, -1, testMatch, -1., -1.);
  }

  CbmTestHit test2{test};

  // Test if the new object has the same values for all data members
  // as the original object
  CbmMatch* testMatch1 = test2.GetMatch();

  EXPECT_EQ(nullptr, testMatch1);
  if (testMatch1) {
    compareMatchDataMembers(*testMatch1, 0, 0.);
  }

  {
    SCOPED_TRACE("CheckCopyConstructor: Test new object after copy construction");
    compareHitDataMembers(test2, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);
  }
  // Test if the original object wasn't changed
  {
    SCOPED_TRACE("CheckCopyConstructor: Test original object after copy construction");
    compareHitDataMembers(test, kHIT, 0., 0., -1, -1, testMatch, -1., -1.);
  }
}


TEST(_GTestCbmHit, CheckAssignmentOperator)
{
  CbmTestHit test;
  {
    SCOPED_TRACE("CheckAssignmentOperator: Initial Test");
    compareHitDataMembers(test, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);
  }

  CbmMatch* testMatch = new CbmMatch();

  compareMatchDataMembers(*testMatch, 0, 0.);

  test.SetMatch(testMatch);
  {
    SCOPED_TRACE("CheckAssignmentOperator: Test with CbmMatch");
    compareHitDataMembers(test, kHIT, 0., 0., -1, -1, testMatch, -1., -1.);
  }

  CbmTestHit test2;
  test2=test;

  // Test if the new object has the same values for all data members
  // as the original object
  CbmMatch* testMatch1 = test2.GetMatch();

  EXPECT_EQ(nullptr, testMatch1);
  if (testMatch1) {
    compareMatchDataMembers(*testMatch1, 0, 0.);
  }

  {
    SCOPED_TRACE("CheckAssignmentOperator: Test new object after assignment");
    compareHitDataMembers(test2, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);
  }
  // Test if the original object wasn't changed
  {
    SCOPED_TRACE("CheckAssignmentOperator: Test original object after assignment");
    compareHitDataMembers(test, kHIT, 0., 0., -1, -1, testMatch, -1., -1.);
  }
}

TEST(_GTestCbmHit, TestSettersAndGetters)
{
  CbmHit test;
  compareHitDataMembers(test, kHIT, 0., 0., -1, -1, nullptr, -1., -1.);

  test.SetZ(-2.);
  compareHitDataMembers(test, kHIT, -2., 0., -1, -1, nullptr, -1., -1.);

  test.SetDz(-2.);
  compareHitDataMembers(test, kHIT, -2., -2., -1, -1, nullptr, -1., -1.);

  test.SetRefId(-2);
  compareHitDataMembers(test, kHIT, -2., -2., -2, -1, nullptr, -1., -1.);

  test.SetAddress(-2);
  compareHitDataMembers(test, kHIT, -2., -2., -2, -2, nullptr, -1., -1.);

  test.SetTime(-2.);
  compareHitDataMembers(test, kHIT, -2., -2., -2, -2, nullptr, -2., -1.);

  test.SetTimeError(-2.);
  compareHitDataMembers(test, kHIT, -2., -2., -2, -2, nullptr, -2., -2.);

  test.SetTime(-3., -3.);
  compareHitDataMembers(test, kHIT, -2., -2., -2, -2, nullptr, -3., -3.);

  Int_t retValInt = test.GetPlaneId();
  EXPECT_EQ(-1, retValInt);

  // Test if we can add an empty CbmMatch, get it back, and extract
  // the expected values
  CbmMatch* testMatch = new CbmMatch();

  compareMatchDataMembers(*testMatch, 0, 0.);

  test.SetMatch(testMatch);
  CbmMatch* testMatch1 = test.GetMatch();

  compareMatchDataMembers(*testMatch1, 0, 0.);
}

TEST(_GTestCbmHit , ToString)
{
  CbmHit test{kHIT, 0., 0., -1, -1};
  EXPECT_STREQ("Has to be implemented in derrived class", test.ToString().c_str());
}
