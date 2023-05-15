/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Dominik Smith [committer] */

#include "Trd2dReadoutConfig.h"

//#include "CbmTrdAddress.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>

using std::pair;
using std::setw;

namespace cbm::algo
{

  // ---  Constructor  ------------------------------------------------------------------
  Trd2dReadoutConfig::Trd2dReadoutConfig() {}

  // ------------------------------------------------------------------------------------


  // ---   Destructor   -----------------------------------------------------------------
  Trd2dReadoutConfig::~Trd2dReadoutConfig() {}
  // ------------------------------------------------------------------------------------


  // ---   Equipment IDs   --------------------------------------------------------------
  std::vector<uint16_t> Trd2dReadoutConfig::GetEquipmentIds()
  {
    std::vector<uint16_t> result;
    for (auto& entry : fReadoutMap)
      result.push_back(entry.first);
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---   Number of Asics for a component / equipment   -------------------------------
  size_t Trd2dReadoutConfig::GetNumAsics(uint16_t equipmentId)
  {
    size_t result = 0;
    auto it       = fChannelMap.find(equipmentId);
    if (it != fChannelMap.end()) result = fChannelMap[equipmentId].size();
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---   Number of Channels for a component / equipment, asic pair  ---------------------
  size_t Trd2dReadoutConfig::GetNumChans(uint16_t equipmentId, uint16_t asicId)
  {
    size_t result = 0;
    auto it       = fChannelMap.find(equipmentId);
    if (it != fChannelMap.end()) {
      if (asicId < fChannelMap[equipmentId].size()) { result = fChannelMap[equipmentId][asicId].size(); }
    }
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---  Initialise the component mapping structure   ----------------------------------
  void Trd2dReadoutConfig::InitComponentMap(const std::map<uint32_t, uint16_t[NCROBMOD]>& map)
  {
    // Receive map (moduleId, crobId) -> (equipId)
    // Invert to obtain component map (equipId) -> (module iq, crob id)
    for (auto& entry : map) {
      uint16_t mod_id = entry.first;
      for (uint8_t crob_id = 0; crob_id < NCROBMOD; crob_id++) {
        uint16_t eq_id     = entry.second[crob_id];
        fReadoutMap[eq_id] = std::make_pair(mod_id, crob_id);
      }
    }
  }

  // ---  Initialise the mapping structure   --------------------------------------------
  void Trd2dReadoutConfig::InitChannelMap(
    const std::map<size_t, std::map<size_t, std::map<size_t, std::tuple<int32_t, bool, uint64_t>>>>& channelMap)
  {
    // Constructing the map (equipId, asicId, chanId) -> (pad address, R pairing flag, daq offset)
    for (auto compMap : channelMap) {
      uint16_t equipmentId = compMap.first;
      uint16_t numAsics    = compMap.second.size();
      fChannelMap[equipmentId].resize(numAsics);

      for (auto asicMap : compMap.second) {
        uint16_t asicId   = asicMap.first;
        uint16_t numChans = asicMap.second.size();
        fChannelMap[equipmentId][asicId].resize(numChans);

        for (auto chanMap : asicMap.second) {
          uint16_t chanId                              = chanMap.first;
          std::tuple<int32_t, bool, uint64_t> chanPars = chanMap.second;
          fChannelMap[equipmentId][asicId][chanId]     = chanPars;
        }
      }
    }
  }
  // ------------------------------------------------------------------------------------


  // ---  Mapping (equimentId, asicId, channel) -> (pad address, R pairing flag, daq offset)  -----
  std::tuple<int32_t, bool, uint64_t> Trd2dReadoutConfig::ChanMap(uint16_t equipId, uint16_t asic, uint16_t chan)
  {
    std::tuple<int32_t, bool, uint64_t> result = std::make_tuple(-1, false, 0);
    auto it                                    = fChannelMap.find(equipId);
    if (it != fChannelMap.end()) {
      if (asic < fChannelMap[equipId].size()) {
        if (chan < fChannelMap[equipId][asic].size()) { result = fChannelMap[equipId][asic][chan]; }
      }
    }
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---  Mapping (equimentId) -> (module id, crob id)  ---------------------------------
  std::pair<uint16_t, uint8_t> Trd2dReadoutConfig::CompMap(uint16_t equipId)
  {
    std::pair<uint16_t, uint8_t> result(0, 0);
    auto equipIter = fReadoutMap.find(equipId);
    if (equipIter != fReadoutMap.end()) { result = equipIter->second; }
    return result;
  }
  // ------------------------------------------------------------------------------------


  // -----   Print readout map   ------------------------------------------------
  std::string Trd2dReadoutConfig::PrintReadoutMap()
  {
    std::stringstream ss;
    for (auto comp : fReadoutMap) {
      uint16_t equipmentId = comp.first;
      auto value           = comp.second;
      uint16_t moduleId    = value.first;
      uint16_t crobId      = value.second;
      ss << "Equipment " << equipmentId << " Module " << moduleId << " Crob " << crobId << "\n";
    }
    ss << "\n";

    for (auto asicMap : fChannelMap) {
      uint16_t equipmentId = asicMap.first;
      uint16_t numAsics    = asicMap.second.size();
      ss << "\n Equipment " << equipmentId << " nAsics " << numAsics;
      for (size_t asicId = 0; asicId < numAsics; asicId++) {

        uint16_t numChans = asicMap.second.at(asicId).size();
        ss << "\n Equipment " << equipmentId << " AsicId " << asicId << " nChans " << numChans;
        for (size_t chanId = 0; chanId < numChans; chanId++) {
          auto entry         = asicMap.second.at(asicId).at(chanId);
          int32_t address    = std::get<0>(entry);
          bool hasPairingR   = std::get<1>(entry);
          uint64_t daqOffset = std::get<2>(entry);
          ss << "\n Equipment " << equipmentId << " AsicId " << asicId << " chanID " << chanId << " pad address "
             << address << " pairingR " << hasPairingR << " daq offset " << daqOffset;
        }
      }
    }
    ss << "\n";
    return ss.str();
  }
  // ----------------------------------------------------------------------------


} /* namespace cbm::algo */
