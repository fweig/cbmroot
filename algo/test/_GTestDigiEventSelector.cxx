/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "TofConfig.h"

#include <unordered_set>

#include "DigiEventSelector.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

TEST(_GTestDigiEventSelector, CheckDigiEventSelectorAlgorithmSimple)
{
  SCOPED_TRACE("CheckDigiEventSelectorAlgorithSimple");

  //Test STS
  {
    const uint maxStsStations = 8;
    const uint maxStsModules  = 12;
    const uint maxStsLadders  = 4;

    for (uint numStations = 1; numStations < maxStsStations; numStations++) {
      //Prepare input
      CbmDigiEvent eventIn;
      //Produce digi pairs with valid addresses
      for (uint station = 0; station < numStations; station++) {
        for (uint module = 0; module < maxStsModules; module++) {
          for (uint ladder = 0; ladder < maxStsLadders; ladder++) {
            for (uint halfladder = 0; halfladder <= 1; halfladder++) {
              //add digis pairs
              int32_t address = CbmStsAddress::GetAddress(station, ladder, halfladder, module);
              eventIn.fData.fSts.fDigis.push_back(CbmStsDigi(address, 0, 0.0, 0.0));
              eventIn.fData.fSts.fDigis.push_back(CbmStsDigi(address, 1024, 0.0, 0.0));  //other side channel

              //add digis from next station without partner for intentionally failed test
              int32_t nextAddress = CbmStsAddress::GetAddress(numStations, ladder, 0, module);
              eventIn.fData.fSts.fDigis.push_back(CbmStsDigi(nextAddress, 1024, 0.0, 0.0));
            }
          }
        }
      }

      //Prepare event filter
      cbm::algo::DigiEventSelectorParams filterParams;
      filterParams.fStsMinStations = numStations;
      cbm::algo::DigiEventSelector evfilter(filterParams);

      EXPECT_EQ(evfilter(eventIn), true);

      //Test if digi without partner is properly disregarded
      filterParams.fStsMinStations = numStations + 1;
      evfilter.SetParams(filterParams);

      EXPECT_EQ(evfilter(eventIn), false);
    }
  }

  //Test TOF
  {
    //Prepare input
    CbmDigiEvent eventIn;

    const uint8_t numSmTypes         = 10;
    const uint8_t numRpc[numSmTypes] = {5, 3, 5, 1, 1, 1, 2, 2, 1, 2};
    const uint8_t numSm[numSmTypes]  = {5, 0, 1, 0, 0, 1, 1, 1, 2, 1};
    std::unordered_set<int32_t> setTofStation;

    for (uint smType = 0; smType < numSmTypes; smType++) {
      for (uint sm = 0; sm < numSm[smType]; sm++) {
        for (uint rpc = 0; rpc < numRpc[smType]; rpc++) {

          uint32_t addrFront = CbmTofAddress::GetUniqueAddress(sm, rpc, 0, 0, smType, 0);
          uint32_t addrBack  = CbmTofAddress::GetUniqueAddress(sm, rpc, 0, 1, smType, 0);
          eventIn.fData.fTof.fDigis.push_back(CbmTofDigi(addrFront, 0.0, 0.0));
          eventIn.fData.fTof.fDigis.push_back(CbmTofDigi(addrBack, 0.0, 0.0));

          const int32_t TofStationId = cbm::algo::TofConfig::GetTofTrackingStation(smType, sm, rpc);
          setTofStation.insert(TofStationId);

          cbm::algo::DigiEventSelectorParams filterParams;
          filterParams.fTofMinLayers = setTofStation.size();
          cbm::algo::DigiEventSelector evfilter(filterParams);

          EXPECT_EQ(evfilter(eventIn), true);

          filterParams.fTofMinLayers = setTofStation.size() + 1;
          evfilter.SetParams(filterParams);

          EXPECT_EQ(evfilter(eventIn), false);
        }
      }
    }
  }
}
