/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Shreya Roy [committer], Pierre-Alain Loizeau, Norbert Herrmann, Volker Friese, Dominik Smith */

#include "DigiEventSelector.h"

#include "CbmStsDigi.h"

#include "TofConfig.h"

#include <iterator>
#include <map>
#include <unordered_set>

#include "AlgoFairloggerCompat.h"


namespace cbm::algo
{

  bool DigiEventSelector::operator()(const CbmDigiEvent& event) const
  {
    //TO DO: Investigate possible algorithms that use bitset instead of std::unordered_map.
    //This requires consequtive addresses for modules and stations.

    if (0 < fParams.fStsMinStations) {
      if (!CheckSts(event.fData.fSts.fDigis)) return false;
    }
    if (0 < fParams.fTofMinLayers) {
      if (!CheckTof(event.fData.fTof.fDigis)) return false;
    }
    return true;
  }

  bool DigiEventSelector::CheckSts(const std::vector<CbmStsDigi>& digis) const
  {
    const uint16_t chanPerSide = 1024;
    /// check for requested number of different STS stations
    std::unordered_set<uint32_t> setStations;
    std::unordered_map<int32_t, bool> mModules;

    for (auto& digi : digis) {
      const int32_t addr = digi.GetAddress();
      auto itModule      = mModules.find(addr);
      if (itModule == mModules.end()) {
        mModules[addr] = digi.GetChannel() / chanPerSide;  // = 0,1 for sides
      }
      else {
        if (digi.GetChannel() / chanPerSide != itModule->second) {
          /// Found other side => non-zero cluster chance, insert into stations set
          const uint32_t stationAddr = CbmStsAddress::GetElementId(addr, EStsElementLevel::kStsUnit);
          auto itStation             = setStations.find(stationAddr);
          if (itStation == setStations.end()) {
            setStations.insert(stationAddr);
            if (setStations.size() == fParams.fStsMinStations) break;
          }
        }
      }
    }
    if (setStations.size() < fParams.fStsMinStations) { return false; }
    else {
      return true;
    }
  }

  bool DigiEventSelector::CheckTof(const std::vector<CbmTofDigi>& digis) const
  {
    /// check for requested number of different RPCs
    std::unordered_set<int32_t> setRpcs;
    std::unordered_set<int32_t> setTofStation;
    std::unordered_map<int32_t, bool> mStrips;
    for (auto& digi : digis) {
      const int32_t addr      = digi.GetAddress();
      const int32_t stripAddr = CbmTofAddress::GetStripFullId(addr);

      auto itStrip = mStrips.find(stripAddr);
      if (itStrip == mStrips.end()) { mStrips[stripAddr] = digi.GetSide(); }
      else {
        if (digi.GetSide() != itStrip->second) {
          /// Found other end => full strip, insert into counter set
          const int32_t rpcAddr = CbmTofAddress::GetRpcFullId(addr);
          auto itRpc            = setRpcs.find(rpcAddr);
          if (itRpc == setRpcs.end()) {
            const int32_t smId         = CbmTofAddress::GetSmId(addr);
            const int32_t smType       = CbmTofAddress::GetSmType(addr);
            const int32_t rpcId        = CbmTofAddress::GetRpcId(addr);
            const int32_t TofStationId = TofConfig::GetTofTrackingStation(smType, smId, rpcId);
            setTofStation.insert(TofStationId);
            if (setTofStation.size() == fParams.fTofMinLayers) break;
          }
        }
      }
    }
    if (setTofStation.size() < fParams.fTofMinLayers) { return false; }
    else {
      return true;
    }
  }

}  // namespace cbm::algo
