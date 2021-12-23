/* Copyright (C) 2016-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "EventBuilder.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestEventBuilder, CheckEventBuilderAlgorithmSimple)
{
  SCOPED_TRACE("CheckEventBuilderAlgorithSimple");

  cbm::algo::EventBuilder evbuild;
  evbuild.SetTriggerWindow(ECbmModuleId::kSts, -45.0, 45.0);

  CbmDigiTimeslice tsIn;
  const uint nInput         = 1000;
  const double inputSpacing = 10.0;

  for (uint i = 0; i < nInput; i++) {
    tsIn.fData.fSts.fDigis.push_back(CbmStsDigi(268502050, 1, i * inputSpacing, 1.0));
  }

  std::vector<double> triggerIn;
  const uint nTrigger         = 99;
  const double triggerSpacing = 100.0;

  for (uint i = 1; i <= nTrigger; i++) {
    triggerIn.push_back(i * triggerSpacing);
  }

  std::vector<CbmDigiEvent> eventsOut = evbuild(tsIn, triggerIn);
  EXPECT_EQ(eventsOut.size(), nTrigger);

  for (uint i = 0; i < eventsOut.size(); i++) {
    EXPECT_EQ(eventsOut[i].fData.fSts.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fTime, triggerIn[i]);
  }
}
