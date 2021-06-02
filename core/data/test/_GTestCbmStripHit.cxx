/** @file _GTestCbmStripHit.cxx
  * @copyright Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmStripHit.h"

#include <TVector3.h>

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

#include "compareStripHit.h"

TEST(_GTestCbmStripHit, CheckDefaultConstructor)
{
  CbmStripHit test;
  {
    SCOPED_TRACE("CheckDefaultConstructor");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0.);
  }
}

TEST(_GTestCbmStripHit, CheckStandardConstructor)
{
  CbmStripHit test {-1, 0., 0., 0., 0., 0., 0., -1};
  {
    SCOPED_TRACE("CheckStandardConstructor");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0.);
  }
}

TEST(_GTestCbmStripHit, CheckStandardConstructorWithTime)
{
  CbmStripHit test {-1, 0., 0., 0., 0., 0., 0., -1, -2., -2.};
  {
    SCOPED_TRACE("CheckStandardConstructorWithTime");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -2., -2., 0., 0., 0., 0.);
  }
}

TEST(_GTestCbmStripHit, CheckStandardConstructorTVector3)
{
  TVector3 pos {
    0.,
    0.,
    0.,
  };
  TVector3 poserror {
    0.,
    0.,
    0.,
  };
  CbmStripHit test {-1, pos, poserror, -1};
  {
    SCOPED_TRACE("CheckStandardConstructor");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0.);
  }
}

TEST(_GTestCbmStripHit, CheckStandardConstructorTVector3WithTime)
{
  TVector3 pos {
    0.,
    0.,
    0.,
  };
  TVector3 poserror {
    0.,
    0.,
    0.,
  };
  CbmStripHit test {-1, pos, poserror, -1, -2., -2};
  {
    SCOPED_TRACE("CheckStandardConstructorWithTime");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -2., -2., 0., 0., 0., 0.);
  }
}

TEST(_GTestCbmStripHit, CheckSetters)
{
  CbmStripHit test;
  {
    SCOPED_TRACE("CheckSetters: Initial Test");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., 0., 0., 0., 0.);
  }

  test.SetU(-11.);
  {
    SCOPED_TRACE("CheckSetters: SetU");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., -11., 0., 0., 0.);
  }

  test.SetDu(-12.);
  {
    SCOPED_TRACE("CheckSetters: SetDu");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., -11., -12., 0., 0.);
  }

  test.SetPhi(-13.);
  {
    SCOPED_TRACE("CheckSetters: SetPhi");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., -11., -12., -13., 0.);
  }

  test.SetDphi(-14.);
  {
    SCOPED_TRACE("CheckSetters: SetDphi");
    compareStripHitDataMembers(test, kSTRIPHIT, 0., 0., -1, -1, nullptr, -1., -1., -11., -12., -13., -14.);
  }
}

TEST(_GTestCbmMatch, CheckToString)
{
  CbmStripHit test;
  EXPECT_STREQ("CbmStripHit: address=-1 pos=(0,0,0) err=(0,0,0) refId=-1\n", test.ToString().c_str());
}
