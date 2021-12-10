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
}
