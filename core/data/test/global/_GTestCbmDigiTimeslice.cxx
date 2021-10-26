/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#include "CbmDigiTimeslice.h"
#include "CbmStsDigi.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <array>
#include <random>

#include "compareStsDigi.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestCbmDigiTimeslice, CheckSerialization)
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
  std::uniform_int_distribution<uint64_t> rTsIndex(0);        // Timeslice index
  std::uniform_int_distribution<uint64_t> rTsPos(0);          // Timeslice offset
  std::uniform_int_distribution<uint32_t> rTsNumMs(0);        // Number of microslices
  std::uniform_int_distribution<uint32_t> rTsNumCp(0);        // Number of components
  std::uniform_int_distribution<int32_t> rNumDigis(1, 10);    // Number of digis
  std::uniform_int_distribution<int32_t> rUnit(0, maxUnit);   // Digi address: unit
  std::uniform_int_distribution<int32_t> rLadd(0, maxLadd);   // Digi address: ladder
  std::uniform_int_distribution<int32_t> rHlad(0, maxHlad);   // Digi address: halfladder
  std::uniform_int_distribution<int32_t> rModu(0, maxModu);   // Digi address: module
  std::uniform_int_distribution<uint16_t> rChan(0, maxChan);  // Digi channel
  std::uniform_int_distribution<int32_t> rTime(0, maxTime);   // Digi time
  std::uniform_int_distribution<uint16_t> rChrg(0, maxChrg);  // Digi charge

  // Create DigiTimeslice
  CbmDigiTimeslice tsWrite;
  uint64_t tsIndex                   = rTsIndex(gen);
  uint64_t tsPos                     = rTsPos(gen);
  uint32_t tsNumMs                   = rTsNumMs(gen);
  uint32_t tsNumCp                   = rTsNumCp(gen);
  tsWrite.fDesc.index                = tsIndex;
  tsWrite.fDesc.ts_pos               = tsPos;
  tsWrite.fDesc.num_core_microslices = tsNumMs;
  tsWrite.fDesc.num_components       = tsNumCp;

  // Create a number of STS digis and add them to the event
  int32_t numDigis = rNumDigis(gen);
  numDigis         = 1;
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
    tsWrite.fData.fSts.fDigis.emplace_back(addr, chan, time, chrg);
  }

  // Serialize timeslice to string
  std::stringstream s;
  boost::archive::binary_oarchive outArchive(s);
  outArchive << tsWrite;

  // Deserialize event from string
  CbmDigiTimeslice tsRead;
  boost::archive::binary_iarchive inArchive(s);
  inArchive >> tsRead;

  // Compare output and input
  EXPECT_EQ(tsRead.fDesc.index, tsIndex);
  EXPECT_EQ(tsRead.fDesc.ts_pos, tsPos);
  EXPECT_EQ(tsRead.fDesc.num_core_microslices, tsNumMs);
  EXPECT_EQ(tsRead.fDesc.num_components, tsNumCp);
  for (int32_t iDigi = 0; iDigi < numDigis; iDigi++) {
    compareStsDigiDataMembers(tsRead.fData.fSts.fDigis[iDigi], vAddr[iDigi], vChrg[iDigi], vChan[iDigi],
                              ECbmModuleId::kSts, vTime[iDigi]);
  }
}
