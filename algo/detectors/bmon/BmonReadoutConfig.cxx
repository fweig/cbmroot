/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "BmonReadoutConfig.h"

#include "CbmTofAddress.h"

#include <Logger.h>

#include <bitset>
#include <iomanip>

#include "gDpbMessv100.h"

using namespace std;

namespace cbm::algo
{
  // ---  Constructor  ------------------------------------------------------------------
  BmonReadoutConfig::BmonReadoutConfig() { Init(); }
  // ------------------------------------------------------------------------------------

  // ---   Destructor   -----------------------------------------------------------------
  BmonReadoutConfig::~BmonReadoutConfig() {}
  // ------------------------------------------------------------------------------------

  // ---   Equipment IDs   --------------------------------------------------------------
  std::vector<uint16_t> BmonReadoutConfig::GetEquipmentIds()
  {
    std::vector<uint16_t> result;
    for (auto& entry : fReadoutMap)
      result.push_back(entry.first);
    return result;
  }
  // ------------------------------------------------------------------------------------

  // ---   Number of elinks for a component / equipment   -------------------------------
  size_t BmonReadoutConfig::GetNumElinks(uint16_t equipmentId)
  {
    size_t result = 0;
    auto it       = fReadoutMap.find(equipmentId);
    if (it != fReadoutMap.end()) result = fReadoutMap[equipmentId].size();
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---  Mapping (equimentId, elink) -> address[channel]  ------------------------------
  std::vector<uint32_t> BmonReadoutConfig::Map(uint16_t equipmentId, uint16_t elinkId)
  {
    std::vector<uint32_t> result;
    auto equipIter = fReadoutMap.find(equipmentId);
    if (equipIter != fReadoutMap.end()) {
      if (elinkId < equipIter->second.size()) { result = equipIter->second.at(elinkId); }
    }
    return result;
  }
  // ------------------------------------------------------------------------------------

  void BmonReadoutConfig::Init()
  {
    // This here refers to the mCBM 2022 setup.
    // Taken from CbmMcbm2018TofPar in combination with macro/beamtime/mcbm2022/mBmonCriPar.par

    // Array to hold the unique IDs (equipment ID) for all TOF DPBs
    uint16_t eqId[numComp] = {0xabf3, 0xabf2, 0xabf1, 0xabf0};

    // Constructing the map (equipmentId, eLink, channel) -> (TOF address)
    const uint32_t numElinksPerComp = numFebsPerComp * numAsicsPerFeb;
    const uint32_t numChanPerComp   = numChanPerAsic * numElinksPerComp;

    // Constructs the fviRpcChUId array
    BuildChannelsUidMap();

    for (uint16_t comp = 0; comp < numComp; comp++) {
      uint16_t equipment = eqId[comp];
      fReadoutMap[equipment].resize(numElinksPerComp);

      for (uint16_t elink = 0; elink < numElinksPerComp; elink++) {
        fReadoutMap[equipment][elink].resize(numChanPerAsic);

        for (uint16_t channel = 0; channel < numChanPerAsic; channel++) {

          const uint32_t chanInComp = elink * numChanPerAsic + channel;
          uint32_t chanInSys        = comp * numChanPerComp + chanInComp;

          {  // hack? perhaps can be removed
            const int numFullFlims = 8;
            if (comp > numFullFlims) { chanInSys -= (comp - numFullFlims) * numChanPerComp / 2; }
          }

          const uint32_t chanUId                 = fviRpcChUId[chanInSys];
          fReadoutMap[equipment][elink][channel] = chanUId;
        }  //# channel
      }    //# elink
    }      //# component
  }

  // -------------------------------------------------------------------------
  void BmonReadoutConfig::BuildChannelsUidMap()
  {
    const uint32_t numAsics = numComp * numFebsPerComp * numAsicsPerFeb;
    const uint32_t numChan  = numAsics * numChanPerAsic;
    fviRpcChUId.resize(numChan);

    uint32_t uCh = 0;
    for (uint32_t uGbtx = 0; uGbtx < numCrob; ++uGbtx) {
      const uint32_t uCh0 = uCh;
      switch (rpcType[uGbtx]) {
        case 5: {
          /// Special Treatment for the T0 diamond
          BuildChannelsUidMapT0(uCh, uGbtx);
          break;
        }
        case 99: {
          /// Special Treatment for the 2022 T0 diamond, keep past behavior for older data!
          BuildChannelsUidMapT0_2022(uCh, uGbtx);
          break;
        }
        case -1: {
          LOG(info) << " Found unused GBTX link at uCh = " << uCh;
          uCh += 160;
          break;
        }
        default: {
          LOG(error) << "Invalid T0 Type specifier for GBTx " << std::setw(2) << uGbtx << ": " << rpcType[uGbtx];
        }
      }  // switch (rpcType[uGbtx])
      if ((int32_t)(uCh - uCh0) != numFebsPerComp * numAsicsPerFeb * numChanPerAsic / 2) {
        LOG(fatal) << "T0 mapping error for Gbtx " << uGbtx << ",  diff = " << uCh - uCh0;
      }
    }
  }

  // -------------------------------------------------------------------------
  void BmonReadoutConfig::BuildChannelsUidMapT0(uint32_t& uCh, uint32_t uGbtx)
  {
    LOG(info) << " Map diamond " << moduleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
    for (uint32_t uGet4 = 0; uGet4 < numElinksPerCrob; ++uGet4) {
      for (uint32_t uGet4Ch = 0; uGet4Ch < numChanPerAsic; ++uGet4Ch) {
        /// Mapping for the 2022 beamtime
        if (uGet4 < 32 && 0 == uGet4Ch && -1 < moduleId[uGbtx]) {
          uint32_t uChannelT0 = uGet4 + 32 * rpcSide[uGbtx];
          uChannelT0 /= 8;
          fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(moduleId[uGbtx], 0, uChannelT0, 0, rpcType[uGbtx]);
          printf("  T0 channel: %u from GBTx %2u, indx %d address %08x", uChannelT0, uGbtx, uCh, fviRpcChUId[uCh]);
        }  // Valid T0 channel
        else {
          fviRpcChUId[uCh] = 0;
        }  // Invalid T0 channel
        uCh++;
      }
    }
  }

  // -------------------------------------------------------------------------
  void BmonReadoutConfig::BuildChannelsUidMapT0_2022(uint32_t& uCh, uint32_t uGbtx)
  {
    LOG(info) << " Map 2022 diamond " << moduleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
    for (uint32_t uGet4 = 0; uGet4 < numElinksPerCrob; ++uGet4) {
      for (uint32_t uGet4Ch = 0; uGet4Ch < numChanPerAsic; ++uGet4Ch) {
        /// Mapping for the 2022 beamtime
        if (-1 < moduleId[uGbtx] && uGet4 < 32 && 0 == uGet4 % 4 && 0 == uGet4Ch) {
          /// 1 channel per physical GET4, 2 links per physical GET4, 4 physical GET4s per GBTx, 1 GBTx per comp.
          /// 16 channels for one side, 16 for the other
          uint32_t uChannelT0 = (uGet4 / 8 + 4 * (uGbtx / 2)) % 16;
          /// Type hard-coded to allow different parameter values to separate 2022 T0 and pre-2022 T0
          fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(moduleId[uGbtx], 0, uChannelT0, rpcSide[uGbtx], 5);
          printf("  Bmon channel: %u side %u from GBTx %2u, indx %d address %08x \n", uChannelT0, rpcSide[uGbtx], uGbtx,
                 uCh, fviRpcChUId[uCh]);
        }  // Valid T0 channel
        else {
          fviRpcChUId[uCh] = 0;
        }  // Invalid T0 channel
        uCh++;
      }
    }
  }
} /* namespace cbm::algo */
