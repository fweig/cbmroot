/* Copyright (C) 2016-2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "EventBuilder.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestEventBuilder, CheckEventBuilderAlgorithmSimple)
{
  SCOPED_TRACE("CheckEventBuilderAlgorithSimple");

  //Initialize event builder
  cbm::algo::EventBuilder evbuild;
  evbuild.AddSystem(ECbmModuleId::kMuch);
  evbuild.AddSystem(ECbmModuleId::kSts);
  evbuild.AddSystem(ECbmModuleId::kTof);
  evbuild.AddSystem(ECbmModuleId::kTrd);
  evbuild.AddSystem(ECbmModuleId::kRich);
  evbuild.AddSystem(ECbmModuleId::kPsd);
  evbuild.AddSystem(ECbmModuleId::kT0);
  evbuild.SetTriggerWindow(ECbmModuleId::kMuch, -45.0, 45.0);
  evbuild.SetTriggerWindow(ECbmModuleId::kSts, -45.0, 45.0);
  evbuild.SetTriggerWindow(ECbmModuleId::kTof, -45.0, 45.0);
  evbuild.SetTriggerWindow(ECbmModuleId::kTrd, -45.0, 45.0);
  evbuild.SetTriggerWindow(ECbmModuleId::kRich, -45.0, 45.0);
  evbuild.SetTriggerWindow(ECbmModuleId::kPsd, -45.0, 45.0);
  evbuild.SetTriggerWindow(ECbmModuleId::kT0, -45.0, 45.0);

  CbmDigiTimeslice tsIn;
  const uint nInput         = 1000;
  const double inputSpacing = 10.0;

  //Produce digis with some arbitrary but valid addresses
  for (uint i = 0; i < nInput; i++) {
    tsIn.fData.fMuch.fDigis.push_back(CbmMuchDigi(1111, 1, i * inputSpacing));
    tsIn.fData.fSts.fDigis.push_back(CbmStsDigi(268502050, 1, i * inputSpacing, 1.0));
    tsIn.fData.fTof.fDigis.push_back(CbmTofDigi(1111, i * inputSpacing, 1.0));
    //tsIn.fData.fTrd.fDigis.push_back(CbmTrdDigi(0, 1.0, 1.0, i * inputSpacing));
    tsIn.fData.fRich.fDigis.push_back(CbmRichDigi(1111, i * inputSpacing, 1.0));
    tsIn.fData.fPsd.fDigis.push_back(CbmPsdDigi(1111, i * inputSpacing, 1.0));
    tsIn.fData.fT0.fDigis.push_back(CbmTofDigi(1111, i * inputSpacing, 1.0));
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
    EXPECT_EQ(eventsOut[i].fData.fMuch.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fData.fSts.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fData.fTof.fDigis.size(), 9);
    //EXPECT_EQ(eventsOut[i].fData.fTrd.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fData.fRich.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fData.fPsd.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fData.fT0.fDigis.size(), 9);
    EXPECT_EQ(eventsOut[i].fTime, triggerIn[i]);
  }
}
