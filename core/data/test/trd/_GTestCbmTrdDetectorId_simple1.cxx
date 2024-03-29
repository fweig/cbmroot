/* Copyright (C) 2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#include "CbmDetectorList.h"
#include "CbmTrdDetectorId.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(CbmTrdDetectorIdTest, CheckDefaultSettings)
{
  CbmTrdDetectorId fTrdId;

  int32_t system           = 0;
  int32_t station          = 0;
  int32_t layer            = 0;
  int32_t moduleType       = 0;
  int32_t moduleNr         = 0;
  int32_t sector           = 0;
  int32_t detInfo_array[6] = {system, station, layer, moduleType, moduleNr, sector};

  int32_t retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(0, retVal);

  detInfo_array[0] = kTRD;
  detInfo_array[1] = 0;
  detInfo_array[2] = 0;
  detInfo_array[3] = 0;
  detInfo_array[4] = 0;
  detInfo_array[5] = 0;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(5, retVal);

  detInfo_array[0] = 0;
  detInfo_array[1] = 1;
  detInfo_array[2] = 0;
  detInfo_array[3] = 0;
  detInfo_array[4] = 0;
  detInfo_array[5] = 0;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(32, retVal);

  detInfo_array[0] = 0;
  detInfo_array[1] = 0;
  detInfo_array[2] = 1;
  detInfo_array[3] = 0;
  detInfo_array[4] = 0;
  detInfo_array[5] = 0;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(512, retVal);

  detInfo_array[0] = 0;
  detInfo_array[1] = 0;
  detInfo_array[2] = 0;
  detInfo_array[3] = 1;
  detInfo_array[4] = 0;
  detInfo_array[5] = 0;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(4096, retVal);

  detInfo_array[0] = 0;
  detInfo_array[1] = 0;
  detInfo_array[2] = 0;
  detInfo_array[3] = 0;
  detInfo_array[4] = 1;
  detInfo_array[5] = 0;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(131072, retVal);

  detInfo_array[0] = 0;
  detInfo_array[1] = 0;
  detInfo_array[2] = 0;
  detInfo_array[3] = 0;
  detInfo_array[4] = 0;
  detInfo_array[5] = 1;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(33554432, retVal);

  detInfo_array[0] = kTRD;
  detInfo_array[1] = 1;
  detInfo_array[2] = 1;
  detInfo_array[3] = 1;
  detInfo_array[4] = 1;
  detInfo_array[5] = 1;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(33690149, retVal);

  detInfo_array[0] = kTRD;
  detInfo_array[1] = 3;
  detInfo_array[2] = 3;
  detInfo_array[3] = 2;
  detInfo_array[4] = 34;
  detInfo_array[5] = 3;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(105129573, retVal);

  detInfo_array[0] = 0;
  detInfo_array[1] = 0;
  detInfo_array[2] = 0;
  detInfo_array[3] = 8;
  detInfo_array[4] = 0;
  detInfo_array[5] = 0;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(32768, retVal);

  detInfo_array[0] = kTRD;
  detInfo_array[1] = 2;
  detInfo_array[2] = 3;
  detInfo_array[3] = 5;
  detInfo_array[4] = 17;
  detInfo_array[5] = 2;

  retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(69359173, retVal);
}
