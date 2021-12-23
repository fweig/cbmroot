/* Copyright (C) 2016-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "TimeClusterTrigger.h"

#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestTimeClusterTrigger, CheckTriggerAlgorithmSimple)
{
  SCOPED_TRACE("CheckTriggerAlgorithSimple");

  cbm::algo::TimeClusterTrigger trigger;
  std::vector<double> dataIn;

  const uint nInput         = 1000.;
  const double inputSpacing = 10.0;
  const double deadTime     = 5.;
  const double windowSize   = 1000.;
  const uint nMinNumber     = 100;

  for (uint i = 0; i < nInput; i++) {
    dataIn.push_back(i * inputSpacing);
  }

  std::vector<double> dataOut = trigger(dataIn, windowSize, nMinNumber, deadTime);
  EXPECT_EQ(dataOut.size(), 10);

  for (uint i = 0; i < dataOut.size(); i++) {
    EXPECT_EQ(dataOut[i], 495. + i * 1000.);
  }
}
