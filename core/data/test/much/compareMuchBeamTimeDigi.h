/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef COMPAREMUCHBEAMTIMEDIGI_H
#define COMPAREMUCHBEAMTIMEDIGI_H 1

#include "compareMuchDigi.h"
#include "gtest/gtest.h"

void compareMuchBeamTimeDigiDataMembers(CbmMuchBeamTimeDigi& test, int32_t address, ECbmModuleId systemid,
                                        uint64_t time, int32_t charge, int32_t padx, int32_t pady, int32_t rocid,
                                        int32_t nxid, int32_t nxch, int32_t elink)
{
  CbmMuchDigi bla = static_cast<CbmMuchDigi>(test);
  compareMuchDigiDataMembers(bla, address, systemid, time, charge);

  int32_t retValInt {-222};

  retValInt = test.GetPadX();
  EXPECT_EQ(padx, retValInt);

  retValInt = test.GetPadY();
  EXPECT_EQ(pady, retValInt);

  retValInt = test.GetRocId();
  EXPECT_EQ(rocid, retValInt);

  retValInt = test.GetNxId();
  EXPECT_EQ(nxid, retValInt);

  retValInt = test.GetNxCh();
  EXPECT_EQ(nxch, retValInt);

  retValInt = test.GetElink();
  EXPECT_EQ(elink, retValInt);
}

#endif  // COMPAREMUCHBEAMTIMEDIGI_H
