/** @file _GTestCbmTrdDetectorId_simple.cxx
  * @copyright Copyright (C) 2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#include "CbmDetectorList.h"
#include "CbmTrdDetectorId.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(CbmTrdDetectorIdTest, CheckDefaultSettings)
{
  CbmTrdDetectorId fTrdId;

  Int_t system           = 0;
  Int_t station          = 0;
  Int_t layer            = 0;
  Int_t moduleType       = 0;
  Int_t moduleNr         = 0;
  Int_t sector           = 0;
  Int_t detInfo_array[6] = {system, station, layer, moduleType, moduleNr, sector};

  Int_t retVal = fTrdId.SetDetectorInfo(detInfo_array);
  EXPECT_EQ(0, retVal);
}
