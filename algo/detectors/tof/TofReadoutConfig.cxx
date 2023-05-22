/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer] */

#include "TofReadoutConfig.h"

#include "CbmTofAddress.h"

#include <bitset>
#include <iomanip>

#include "AlgoFairloggerCompat.h"
#include "gDpbMessv100.h"

using namespace std;

namespace cbm::algo
{
  // ---  Constructor  ------------------------------------------------------------------
  TofReadoutConfig::TofReadoutConfig() { Init(); }
  // ------------------------------------------------------------------------------------

  // ---   Destructor   -----------------------------------------------------------------
  TofReadoutConfig::~TofReadoutConfig() {}
  // ------------------------------------------------------------------------------------

  // ---   Equipment IDs   --------------------------------------------------------------
  std::vector<uint16_t> TofReadoutConfig::GetEquipmentIds()
  {
    std::vector<uint16_t> result;
    for (auto& entry : fReadoutMap)
      result.push_back(entry.first);
    return result;
  }
  // ------------------------------------------------------------------------------------

  // ---   Number of elinks for a component / equipment   -------------------------------
  size_t TofReadoutConfig::GetNumElinks(uint16_t equipmentId)
  {
    size_t result = 0;
    auto it       = fReadoutMap.find(equipmentId);
    if (it != fReadoutMap.end()) result = fReadoutMap[equipmentId].size();
    return result;
  }
  // ------------------------------------------------------------------------------------


  // ---  Mapping (equimentId, elink) -> address[channel]  ------------------------------
  std::vector<uint32_t> TofReadoutConfig::Map(uint16_t equipmentId, uint16_t elinkId)
  {
    std::vector<uint32_t> result;
    auto equipIter = fReadoutMap.find(equipmentId);
    if (equipIter != fReadoutMap.end()) {
      if (elinkId < equipIter->second.size()) { result = equipIter->second.at(elinkId); }
    }
    return result;
  }
  // ------------------------------------------------------------------------------------

  void TofReadoutConfig::Init()
  {
    // This here refers to the mCBM 2022 setup.
    // Taken from CbmMcbm2018TofPar in combination with macro/beamtime/mcbm2022/mTofCriParNickel.par

    // Array to hold the unique IDs (equipment ID) for all TOF DPBs
    uint16_t eqId[numComp] = {0xabc0, 0xabc1, 0xabc2, 0xabc3, 0xabc4, 0xabc5, 0xabc6, 0xabc7};

    // Constructing the map (equipmentId, eLink, channel) -> (TOF address)
    const uint32_t numChanPerComp = numChanPerAsic * numElinksPerComp;

    // Constructs the fviRpcChUId array
    BuildChannelsUidMap();

    for (uint16_t comp = 0; comp < numComp; comp++) {

      uint16_t equipment = eqId[comp];
      fReadoutMap[equipment].resize(numElinksPerComp);

      for (uint16_t elink = 0; elink < numElinksPerComp; elink++) {
        fReadoutMap[equipment][elink].resize(numChanPerAsic);
        const uint32_t asicId = ElinkIdxToGet4Idx(elink);

        for (uint16_t channel = 0; channel < numChanPerAsic; channel++) {
          const uint32_t febId        = (asicId / numAsicsPerFeb);
          const uint32_t chanInFeb    = (asicId % numAsicsPerFeb) * numChanPerAsic + channel;
          const uint32_t remappedChan = comp * numChanPerComp + febId * numChanPerFeb + Get4ChanToPadiChan(chanInFeb);
          const uint32_t chanUId      = fviRpcChUId[remappedChan];
          fReadoutMap[equipment][elink][channel] = chanUId;
        }  //# channel
      }    //# elink
    }      //# component
  }

  int32_t TofReadoutConfig::ElinkIdxToGet4Idx(uint32_t elink)
  {
    if (gdpbv100::kuChipIdMergedEpoch == elink) return elink;
    else if (elink < numElinksPerComp)
      return elink2Asic[elink % numElinksPerCrob] + numElinksPerCrob * (elink / numElinksPerCrob);
    else {
      LOG(fatal) << "CbmMcbm2018TofPar::ElinkIdxToGet4Idx => Index out of bound, " << elink << " vs "
                 << static_cast<uint32_t>(numElinksPerComp) << ", returning crazy value!";
      return -1;
    }
  }
  // -------------------------------------------------------------------------


  // -------------------------------------------------------------------------
  int32_t TofReadoutConfig::Get4ChanToPadiChan(uint32_t channelInFee)
  {
    if (channelInFee < numChanPerFeb) return asic2PadI[channelInFee];
    else {
      LOG(fatal) << "CbmMcbm2018TofPar::Get4ChanToPadiChan => Index out of bound, " << channelInFee << " vs "
                 << static_cast<uint32_t>(numChanPerFeb) << ", returning crazy value!";
      return -1;
    }
  }
  // -------------------------------------------------------------------------

  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMap()
  {
    uint32_t uNrOfGet4     = numComp * numFebsPerComp * numAsicsPerFeb;
    uint32_t uNrOfChannels = uNrOfGet4 * numChanPerAsic;
    fviRpcChUId.resize(uNrOfChannels);

    uint32_t uCh = 0;
    for (uint32_t uGbtx = 0; uGbtx < numCrob; ++uGbtx) {
      uint32_t uCh0 = uCh;
      switch (rpcType[uGbtx]) {
        case 2:  // intended fall-through
        case 0: {
          // CBM modules
          BuildChannelsUidMapCbm(uCh, uGbtx);
          break;
        }
        case 1: {
          // STAR eTOF  modules
          BuildChannelsUidMapStar(uCh, uGbtx);
          break;
        }
        case 78: {
          // cern-20-gap + ceramic module
          BuildChannelsUidMapCern(uCh, uGbtx);
        }
          [[fallthrough]];  // fall through is intended
        case 8:             // ceramics
        {
          BuildChannelsUidMapCera(uCh, uGbtx);
          break;
        }
        case 4:  // intended fallthrough
          [[fallthrough]];
        case 7: [[fallthrough]];
        case 9:  // Star2 boxes
        {
          BuildChannelsUidMapStar2(uCh, uGbtx);
          break;
        }
        case 6:  // Buc box
        {
          BuildChannelsUidMapBuc(uCh, uGbtx);
          break;
        }
        case 69: {
          /// 2022 case: 69 is followed by 4 and 9
          BuildChannelsUidMapBuc(uCh, uGbtx);
          /// Map also 4 and 9 (equivalent to fallthrough to 4 then 9 but without changing past behaviors)
          uCh -= 80;  // PAL, 2022/03/17: ?!?
          BuildChannelsUidMapStar2(uCh, uGbtx);
          uCh -= 80;  // PAL, 2022/03/17: ?!?
          break;
        }
        case -1: {
          LOG(info) << " Found unused GBTX link at uCh = " << uCh;
          uCh += 160;
          break;
        }
        default: {
          LOG(error) << "Invalid Tof Type specifier for GBTx " << std::setw(2) << uGbtx << ": " << rpcType[uGbtx];
        }
      }  // switch (rpcType[uGbtx])
      if ((int32_t)(uCh - uCh0) != numFebsPerComp * numAsicsPerFeb * numChanPerAsic / 2) {
        LOG(fatal) << "Tof mapping error for Gbtx " << uGbtx << ",  diff = " << uCh - uCh0 << ", type "
                   << rpcType[uGbtx];
      }
    }  // for (UInt_t uGbtx = 0; uGbtx < numCrob; ++uGbtx)
  }
  // -------------------------------------------------------------------------


  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMapCbm(uint32_t& uCh, uint32_t uGbtx)
  {
    LOG(info) << " Map mTof box " << moduleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
    if (rpcSide[uGbtx] < 2) {  // mTof modules
      LOG(debug) << " Map mTof box " << moduleId[uGbtx] << " at GBTX  -  uCh = " << uCh;
      const int32_t RpcMap[5] = {4, 2, 0, 3, 1};
      for (int32_t iRpc = 0; iRpc < numRpc[uGbtx]; iRpc++) {
        int32_t iStrMax  = 32;
        uint32_t uChNext = 1;

        for (int32_t iStr = 0; iStr < iStrMax; iStr++) {
          int32_t iStrMap = iStr;
          int32_t iRpcMap = RpcMap[iRpc];

          if (rpcSide[uGbtx] == 0) iStrMap = 31 - iStr;
          if (moduleId[uGbtx] > -1)
            fviRpcChUId[uCh] =
              CbmTofAddress::GetUniqueAddress(moduleId[uGbtx], iRpcMap, iStrMap, rpcSide[uGbtx], rpcType[uGbtx]);
          else
            fviRpcChUId[uCh] = 0;
          uCh += uChNext;
        }  // for (int32_t iStr = 0; iStr < iStrMax; iStr++)
      }    // for (int32_t iRpc = 0; iRpc < numRpc[uGbtx]; iRpc++)
    }      // if (rpcSide[uGbtx] < 2)
  }


  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMapStar(uint32_t& uCh, uint32_t uGbtx)
  {
    if (rpcSide[uGbtx] < 2) {
      // mTof modules
      LOG(info) << "Start eTOF module side " << rpcSide[uGbtx] << " at " << uCh;
      const int32_t RpcMap[3] = {0, 1, 2};
      for (int32_t iRpc = 0; iRpc < numRpc[uGbtx]; iRpc++) {
        int32_t iStrMax = 32;
        int32_t uChNext = 1;

        for (int32_t iStr = 0; iStr < iStrMax; iStr++) {
          int32_t iStrMap = iStr;
          int32_t iRpcMap = RpcMap[iRpc];

          if (rpcSide[uGbtx] == 0) iStrMap = 31 - iStr;
          if (moduleId[uGbtx] > -1)
            fviRpcChUId[uCh] =
              CbmTofAddress::GetUniqueAddress(moduleId[uGbtx], iRpcMap, iStrMap, rpcSide[uGbtx], rpcType[uGbtx]);
          else
            fviRpcChUId[uCh] = 0;
          uCh += uChNext;
        }
      }
    }
    uCh += 64;
  }

  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMapCern(uint32_t& uCh, uint32_t)
  {
    LOG(info) << " Map CERN 20 gap  at GBTX  -  uCh = " << uCh;
    // clang-format off
  const int32_t StrMap[32] = {0,  1,  2,  3,  4,  31, 5,  6,  7,  30, 8,
                            9,  10, 29, 11, 12, 13, 14, 28, 15, 16, 17,
                            18, 27, 26, 25, 24, 23, 22, 21, 20, 19};
    // clang-format on
    int32_t iModuleId   = 0;
    int32_t iModuleType = 7;
    int32_t iRpcMap     = 0;
    for (int32_t iFeet = 0; iFeet < 2; iFeet++) {
      for (int32_t iStr = 0; iStr < 32; iStr++) {
        int32_t iStrMap  = 31 - 12 - StrMap[iStr];
        int32_t iSideMap = iFeet;
        if (iStrMap < 20)
          fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(iModuleId, iRpcMap, iStrMap, iSideMap, iModuleType);
        else
          fviRpcChUId[uCh] = 0;
        uCh++;
      }
    }
    LOG(info) << " Map end CERN 20 gap  at GBTX  -  uCh = " << uCh;
  }

  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMapCera(uint32_t& uCh, uint32_t)
  {
    int32_t iModuleId   = 0;
    int32_t iModuleType = 8;
    for (int32_t iRpc = 0; iRpc < 8; iRpc++) {
      fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(iModuleId, 7 - iRpc, 0, 0, iModuleType);
      uCh++;
    }
    uCh += (24 + 2 * 32);
    LOG(info) << " Map end ceramics  box  at GBTX  -  uCh = " << uCh;
  }


  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMapStar2(uint32_t& uCh, uint32_t uGbtx)
  {
    LOG(info) << " Map Star2 box " << moduleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;
    const int32_t iRpc[5]  = {1, -1, 1, 0, 0};
    const int32_t iSide[5] = {1, -1, 0, 1, 0};
    for (int32_t iFeet = 0; iFeet < 5; iFeet++) {
      for (int32_t iStr = 0; iStr < 32; iStr++) {
        int32_t iStrMap  = iStr;
        int32_t iRpcMap  = iRpc[iFeet];
        int32_t iSideMap = iSide[iFeet];
        if (iSideMap == 0) iStrMap = 31 - iStr;
        switch (rpcSide[uGbtx]) {
          case 0:; break;
          case 1:;
            iRpcMap = 1 - iRpcMap;  // swap counters
            break;
          case 2:
            switch (iFeet) {
              case 1:
                iRpcMap  = iRpc[4];
                iSideMap = iSide[4];
                iStrMap  = 31 - iStrMap;
                break;
              case 4:
                iRpcMap  = iRpc[1];
                iSideMap = iSide[1];
                break;
              default:;
            }
            break;
          case 3:  // direct beam 20210524
            switch (iFeet) {
              case 0:
                iRpcMap  = 0;
                iSideMap = 0;
                iStrMap  = iStr;
                break;
              case 1:
                iRpcMap  = 0;
                iSideMap = 1;
                iStrMap  = 31 - iStr;
                break;
              default: iSideMap = -1;
            }
            break;
        }
        if (iSideMap > -1)
          fviRpcChUId[uCh] =
            CbmTofAddress::GetUniqueAddress(moduleId[uGbtx], iRpcMap, iStrMap, iSideMap, rpcType[uGbtx]);
        else
          fviRpcChUId[uCh] = 0;
        uCh++;
      }
    }
  }


  // -------------------------------------------------------------------------
  void TofReadoutConfig::BuildChannelsUidMapBuc(uint32_t& uCh, uint32_t uGbtx)
  {
    LOG(info) << " Map Buc box " << moduleId[uGbtx] << " at GBTX " << uGbtx << " -  uCh = " << uCh;

    int32_t iModuleIdMap   = moduleId[uGbtx];
    const int32_t iRpc[5]  = {0, -1, 0, 1, 1};
    const int32_t iSide[5] = {1, -1, 0, 1, 0};
    for (int32_t iFeet = 0; iFeet < 5; iFeet++) {
      for (int32_t iStr = 0; iStr < 32; iStr++) {
        int32_t iStrMap  = iStr;
        int32_t iRpcMap  = iRpc[iFeet];
        int32_t iSideMap = iSide[iFeet];
        switch (rpcSide[uGbtx]) {
          case 0:; break;
          case 1:  // HD cosmic 2019, Buc2018, v18n
            iStrMap = 31 - iStr;
            iRpcMap = 1 - iRpcMap;
            break;
          case 2:  // v18m_cosmicHD
            //   iStrMap=31-iStr;
            iSideMap = 1 - iSideMap;
            break;
          case 3:
            iStrMap  = 31 - iStr;
            iRpcMap  = 1 - iRpcMap;
            iSideMap = 1 - iSideMap;
            break;
          case 4:  // HD cosmic 2019, Buc2018, v18o
            iRpcMap = 1 - iRpcMap;
            break;
          case 5:  // HD cosmic 2020, Buc2018, v20a
            iStrMap = 31 - iStr;
            break;
          case 6:  //BUC special
          {
            switch (moduleId[uGbtx]) {
              case 0: iRpcMap = 0; break;
              case 1: iRpcMap = 1; break;
            }
            if (iFeet % 2 == 1) iModuleIdMap = 1;
            else
              iModuleIdMap = 0;

            switch (iFeet) {
              case 0:
              case 3: iSideMap = 0; break;
              case 1:
              case 2: iSideMap = 1; break;
            }
          } break;
          case 8: {
            // Special case for two channels in 2022
            // Fallthrough to 7 for all other channels
            if (2 == iFeet) {
              if (7 == iStr) {
                ///                                               SM Rpc St Si Type
                fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(0, 0, 0, 0, 8);
                uCh++;
                continue;
              }
              else if (23 == iStr) {
                ///                                               SM Rpc St Si Type
                fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(1, 0, 0, 0, 8);
                uCh++;
                continue;
              }
            }
          }
          case 7: {
            // clang-format off
          const int32_t iChMap[160]={
          124, 125, 126, 127,  12,  13,  14,  15,   4,   5,   6,   7,  28,  29,  30,  31, 120, 121, 122, 123,   8,  9,   10,  11, 104, 105, 106, 107, 108, 109, 110, 111,
           36,  37,  38,  39,  52,  53,  54,  55,  60,  61,  62,  63, 128, 129, 130, 131,  40,  41,  42,  43, 148, 149, 150, 151,  56,  57,  58,  59, 132, 133, 134, 135,
          136, 137, 138, 139, 140, 141, 142, 143,  96,  97,  98,  99,  64,  65,  66,  67, 100, 101, 102, 103,  84,  85,  86,  87, 152, 153, 154, 155,  68,  69,  70,  71,
          156, 157, 158, 159, 144, 145, 146, 147,  44,  45,  46,  47,  76,  77,  78,  79,  48,  49,  50,  51,  20,  21,  22,  23,  32,  33,  34,  35, 116, 117, 118, 119,
           75,  74,  73,  72,  92,  93,  94,  95,  16,  17,  18,  19,  80,  81,  82,  83, 115, 114, 113, 112,  24,  25,  26,  27,  88,  89,  90,  91,   0,   1,   2,   3
          };
            // clang-format on
            int32_t iInd = iFeet * 32 + iStr;
            int32_t i    = 0;
            for (; i < 160; i++)
              if (iInd == iChMap[i]) break;
            iStrMap          = i % 32;
            int32_t iFeetInd = (i - iStrMap) / 32;
            switch (iFeetInd) {
              case 0:
                iRpcMap  = 0;
                iSideMap = 1;
                break;
              case 1:
                iRpcMap  = 1;
                iSideMap = 1;
                break;
              case 2:
                iRpcMap  = 1;
                iSideMap = 0;
                break;
              case 3:
                iRpcMap  = 0;
                iSideMap = 0;
                break;
              case 4: iSideMap = -1; break;
            }
            iModuleIdMap = moduleId[uGbtx];
          } break;
          default:;
        }  // switch (rpcSide[uGbtx])
        if (iSideMap > -1)
          fviRpcChUId[uCh] = CbmTofAddress::GetUniqueAddress(iModuleIdMap, iRpcMap, iStrMap, iSideMap, rpcType[uGbtx]);
        else
          fviRpcChUId[uCh] = 0;

        uCh++;
      }  // for (int32_t iStr = 0; iStr < 32; iStr++)
    }    // for (int32_t iFeet = 0; iFeet < 5; iFeet++)
  }
  // -------------------------------------------------------------------------

} /* namespace cbm::algo */
