/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmDigiEvent.h"
#include "CbmStsDigi.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <array>
#include <random>

#include "compareStsDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmDigiEvent, CheckSerialization)
{

  // Limits for STS digi member values
  int32_t maxUnit  = (1 << 6) - 1;   //  6 bits for address - unit
  int32_t maxLadd  = (1 << 5) - 1;   //  5 bits for address - ladder
  int32_t maxHlad  = (1 << 1) - 1;   //  1 bit  for address - halfladder
  int32_t maxModu  = (1 << 5) - 1;   //  5 bits for address - module
  uint16_t maxChan = (1 << 11) - 1;  // 11 bits for channel number
  int32_t maxTime  = (1 << 30) - 1;  // 31 bits for time
  uint16_t maxChrg = (1 << 5) - 1;   //  5 bits for ADC

  // Random generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int32_t> rEvtNumber(1, 1000);  // Event number
  std::uniform_real_distribution<double> rEvtTime(0., 1.e10);  // Event time
  std::uniform_int_distribution<int32_t> rNumDigis(1, 10);     // Number of digis
  std::uniform_int_distribution<int32_t> rUnit(0, maxUnit);    // Digi address: unit
  std::uniform_int_distribution<int32_t> rLadd(0, maxLadd);    // Digi address: ladder
  std::uniform_int_distribution<int32_t> rHlad(0, maxHlad);    // Digi address: halfladder
  std::uniform_int_distribution<int32_t> rModu(0, maxModu);    // Digi address: module
  std::uniform_int_distribution<uint16_t> rChan(0, maxChan);   // Digi channel
  std::uniform_int_distribution<int32_t> rTime(0, maxTime);    // Digi time
  std::uniform_int_distribution<uint16_t> rChrg(0, maxChrg);   // Digi charge

  // Create DigiEvent
  CbmDigiEvent eventOut;
  int32_t evtNumber = rEvtNumber(gen);
  double evtTime    = rEvtTime(gen);
  eventOut.fNumber  = evtNumber;
  eventOut.fTime    = evtTime;

  // Create a number of STS digis and add them to the event
  int32_t numDigis = rNumDigis(gen);
  std::vector<int32_t> vAddr(numDigis);
  std::vector<uint16_t> vChan(numDigis);
  std::vector<int32_t> vTime(numDigis);
  std::vector<uint16_t> vChrg(numDigis);
  for (int32_t iDigi = 0; iDigi < numDigis; iDigi++) {
    int32_t unit  = rUnit(gen);
    int32_t ladd  = rLadd(gen);
    int32_t hlad  = rHlad(gen);
    int32_t modu  = rModu(gen);
    int32_t addr  = CbmStsAddress::GetAddress(unit, ladd, hlad, modu, 0, 0);
    uint16_t chan = rChan(gen);
    int32_t time  = rTime(gen);
    uint16_t chrg = rChrg(gen);
    vAddr[iDigi]  = addr;
    vChan[iDigi]  = chan;
    vTime[iDigi]  = time;
    vChrg[iDigi]  = chrg;
    eventOut.fData.fSts.fDigis.emplace_back(addr, chan, time, chrg);
  }

  // Serialize event to string
  std::stringstream s;
  boost::archive::binary_oarchive outArchive(s);
  outArchive << eventOut;

  // Deserialize event from string
  CbmDigiEvent eventIn;
  boost::archive::binary_iarchive inArchive(s);
  inArchive >> eventIn;

  // Compare output and input
  EXPECT_EQ(eventIn.fNumber, evtNumber);
  EXPECT_FLOAT_EQ(eventIn.fTime, evtTime);
  EXPECT_EQ(eventIn.fData.fSts.fDigis.size(), numDigis);
  for (int32_t iDigi = 0; iDigi < numDigis; iDigi++) {
    compareStsDigiDataMembers(eventIn.fData.fSts.fDigis[iDigi], vAddr[iDigi], vChrg[iDigi], vChan[iDigi],
                              ECbmModuleId::kSts, vTime[iDigi]);
  }
}
