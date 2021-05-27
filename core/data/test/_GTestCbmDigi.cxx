/** @file _GTestCbmDigi.cxx
  * @copyright Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [orginator] **/

#include "CbmDigi.h"
#include "CbmMatch.h"

#include <utility>  // std::forward

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

// Since CbmDigi is an abstract base class which can't be instantiated directly we have
// to create a derived class without any data members which simply forwards the function
// calls to the abstract base class

class CbmTestDigi : public CbmDigi {

public:
  CbmTestDigi() : CbmDigi() { ; }

  /** Copy constructor  **/
  CbmTestDigi(const CbmTestDigi& digi) : CbmDigi(digi) { ; }

  /** Move constructor  **/
  CbmTestDigi(CbmTestDigi&& digi) : CbmDigi(std::forward<CbmTestDigi>(digi)) { ; }

  /** Destructor  **/
  virtual ~CbmTestDigi() { ; }

  /** Assignment operator  **/
  CbmTestDigi& operator=(const CbmTestDigi& other)
  {
    if (this != &other) { CbmDigi::operator=(other); }
    return *this;
  }

  /** Move Assignment operator  **/
  CbmTestDigi& operator=(CbmTestDigi&& other)
  {
    if (this != &other) { CbmDigi::operator=(std::forward<CbmTestDigi>(other)); }
    return *this;
  }

  /** Unique channel address  **/
  Int_t GetAddress() const { return CbmDigi::GetAddress(); }


  /** System (enum DetectorId) **/
  Int_t GetSystemId() const { return CbmDigi::GetSystemId(); };


  /** Absolute time [ns]  **/
  Double_t GetTime() const { return CbmDigi::GetTime(); }

  std::string ToString() const { return CbmDigi::ToString(); }
};

#include "compareDigi.h"

TEST(_GTestCbmDigi, CheckDefaultConstructor)
{
  // Create abstract base class via derived class
  CbmTestDigi test;

  compareDigiDataMembers(test, -111, 0., -111, -111.);

  CbmTestDigi* test1 = new CbmTestDigi();

  compareDigiDataMembers(*test1, -111, 0., -111, -111.);
}

TEST(_GTestCbmDigi, CheckCopyConstructor)
{
  // Create abstract base class via derived class
  CbmTestDigi test;

  compareDigiDataMembers(test, -111, 0., -111, -111.);

  // Create object by copy constructing
  // test should be equal to test2 and test should be existing
  CbmTestDigi test2 {test};

  compareDigiDataMembers(test2, -111, 0., -111, -111.);

  // Test if the original object wasn't changed
  compareDigiDataMembers(test, -111, 0., -111, -111.);
}

TEST(_GTestCbmDigi, CheckAssignmentOperator)
{
  // Create abstract base class via derived class
  CbmTestDigi test;

  compareDigiDataMembers(test, -111, 0., -111, -111.);

  // Create object by copy constructing
  // test should be equal to test2 and test should be existing
  CbmTestDigi test2;
  test2 = test;

  compareDigiDataMembers(test2, -111, 0., -111, -111.);

  // Test if the original object wasn't changed
  compareDigiDataMembers(test, -111, 0., -111, -111.);


  // Create object by copy constructing
  // test should be equal to test2 and test should be existing
  CbmTestDigi test3;
  test3 = test;

  compareDigiDataMembers(test3, -111, 0., -111, -111.);

  // Test if the original object wasn't changed
  compareDigiDataMembers(test, -111, 0., -111, -111.);
}

TEST(_GTestCbmDigi, CheckMoveConstructor)
{
  // Create abstract base class via derived class
  // After creation there is no CbmMatch added such
  // that the pointer is a nullptr
  CbmTestDigi test;

  compareDigiDataMembers(test, -111, 0., -111, -111.);

  // Create object by move constructing
  // test2 should now contain the pointer to the CbmMatch object and
  // test should contain a nullptr
  CbmTestDigi test2 {std::move(test)};

  compareDigiDataMembers(test2, -111, 0., -111, -111.);

  compareDigiDataMembers(test, -111, 0., -111, -111.);
}

TEST(_GTestCbmDigi, CheckAssignmentMoveConstructor)
{
  // Create abstract base class via derived class
  // After creation there is no CbmMatch added such
  // that the pointer is a nullptr
  CbmTestDigi test;

  compareDigiDataMembers(test, -111, 0., -111, -111.);

  // Create object by move constructing
  // test2 should now contain the pointer to the CbmMatch object and
  // test should contain a nullptr
  CbmTestDigi test2;  // = std::move(test);
  test2 = std::move(test);


  compareDigiDataMembers(test2, -111, 0., -111, -111.);

  compareDigiDataMembers(test, -111, 0., -111, -111.);
}

TEST(_GTestCbmDigi, CheckToString)
{
  CbmTestDigi test;

  compareDigiDataMembers(test, -111, 0., -111, -111.);

  EXPECT_STREQ("Digi: System -111 | address -111 | time -111 | charge 0", test.ToString().c_str());
}
